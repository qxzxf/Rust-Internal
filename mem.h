#pragma once
#include <type_traits>
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <algorithm>

class Memory
{
public:
	void* copymem(void* memTo, const void* memFrom, size_t size)
	{
		if (!memTo || !memFrom)
			return nullptr;
		const char* temFrom = (const char*)memFrom;
		char* temTo = (char*)memTo;
		while (size-- > 0)
			*temTo++ = *temFrom++;
		return memTo;
	}

	template <class T = uint64_t>
	T Read(uint64_t address)
	{
		if (address < 0xffffff)
			return T();
		if (address > 0x7fffffff0000)
			return T();

		if (!address)
		{
			T response{};
			return response;
		}
		else
		{
			return *(T*)(address);
		}
	}

	bool Read(uint64_t address, void* out, SIZE_T size)
	{
		if (!address)
			return false;

		memcpy(out, reinterpret_cast<const void*>(address), size);
		return true;
	}

	template <class T>
	void Write(uint64_t address, T value)
	{
		if (!address)
			return;

		*(T*)(address) = value;
	}
};

Memory mem;

ULONG64 GetExport(ULONG64 hDll, const char* Name)
{
	PIMAGE_NT_HEADERS NT_Head = (PIMAGE_NT_HEADERS)(hDll + ((PIMAGE_DOS_HEADER)hDll)->e_lfanew);
	PIMAGE_EXPORT_DIRECTORY ExportDir = (PIMAGE_EXPORT_DIRECTORY)(hDll + NT_Head->OptionalHeader.DataDirectory[0].VirtualAddress);

	for (DWORD i = 0; i < ExportDir->NumberOfNames; i++)
	{
		USHORT Ordinal = ((USHORT*)(hDll + ExportDir->AddressOfNameOrdinals))[i];
		const char* ExpName = (const char*)hDll + ((DWORD*)(hDll + ExportDir->AddressOfNames))[i];

		for (int i = 0; ExpName[i] == Name[i]; i++) if (!ExpName[i])
			return hDll + ((DWORD*)(hDll + ExportDir->AddressOfFunctions))[Ordinal];
	} return 0;
}

DWORD64 g_Base = 0;
ULONG64 GetModBase(const char* Name = "GameAssembly.dll")
{
	return (ULONG64)(GetModuleHandleA(Name));
}

//FACE Utils v4
namespace FC
{
	//Static Macro
#define RVA(Instr, InstrSize) ((DWORD64)Instr + InstrSize + *(LONG*)((DWORD64)Instr + (InstrSize - sizeof(LONG))))
#define ConstStrLen(Str) ((sizeof(Str) - sizeof(Str[0])) / sizeof(Str[0]))
#define ToLower(Char) ((Char >= 'A' && Char <= 'Z') ? (Char + 32) : Char)

//StrCompare (with StrInStrI(Two = false))
	template <typename StrType, typename StrType2>
	bool StrCmp(StrType Str, StrType2 InStr, bool Two) {
		if (!Str || !InStr) return false;
		wchar_t c1, c2; do {
			c1 = *Str++; c2 = *InStr++;
			c1 = ToLower(c1); c2 = ToLower(c2);
			if (!c1 && (Two ? !c2 : 1)) return true;
		} while (c1 == c2); return false;
	}

	//CRC16 StrHash	
	template <typename StrType> __declspec(noinline) constexpr unsigned short HashStr(StrType Data, int Len) {
		unsigned short CRC = 0xFFFF; while (Len--) {
			auto CurChar = *Data++; if (!CurChar) break;
			CRC ^= ToLower(CurChar) << 8; for (int i = 0; i < 8; i++)
				CRC = CRC & 0x8000 ? (CRC << 1) ^ 0x8408 : CRC << 1;
		} return CRC;
	}
#define ConstHashStr(Str) [](){ constexpr unsigned short CRC = FC::HashStr(Str, ConstStrLen(Str)); return CRC; }()

	//EncryptDecryptPointer
	template <typename PtrType>
	__forceinline PtrType EPtr(PtrType Ptr) {
		typedef union { struct { USHORT Key1; USHORT Key2; USHORT Key3; USHORT Key4; }; ULONG64 Key; } CryptData;
		CryptData Key{ ConstHashStr(__TIME__), ConstHashStr(__DATE__), ConstHashStr(__TIMESTAMP__), ConstHashStr(__TIMESTAMP__) };
		return (PtrType)((DWORD64)Ptr ^ Key.Key);
	}
#define EPtr(Ptr) FC::EPtr(Ptr)


	//Signature Scan
	PBYTE FindPattern_Wrapper(const char* Pattern, const char* Module = nullptr)
	{
		//find pattern utils
#define InRange(x, a, b) (x >= a && x <= b) 
#define GetBits(x) (InRange(x, '0', '9') ? (x - '0') : ((x - 'A') + 0xA))
#define GetByte(x) ((BYTE)(GetBits(x[0]) << 4 | GetBits(x[1])))

//get module range
		PBYTE ModuleStart = (PBYTE)GetModBase(Module); if (!ModuleStart) return nullptr;
		PIMAGE_NT_HEADERS NtHeader = ((PIMAGE_NT_HEADERS)(ModuleStart + ((PIMAGE_DOS_HEADER)ModuleStart)->e_lfanew));
		PBYTE ModuleEnd = (PBYTE)(ModuleStart + NtHeader->OptionalHeader.SizeOfImage - 0x1000); ModuleStart += 0x1000;

		//scan pattern main
		PBYTE FirstMatch = nullptr;
		const char* CurPatt = Pattern;
		for (; ModuleStart < ModuleEnd; ++ModuleStart)
		{
			bool SkipByte = (*CurPatt == '\?');
			if (SkipByte || *ModuleStart == GetByte(CurPatt)) {
				if (!FirstMatch) FirstMatch = ModuleStart;
				SkipByte ? CurPatt += 2 : CurPatt += 3;
				if (CurPatt[-1] == 0) return FirstMatch;
			}

			else if (FirstMatch) {
				ModuleStart = FirstMatch;
				FirstMatch = nullptr;
				CurPatt = Pattern;
			}
		}

		return nullptr;
	}
#define FindPattern FC::FindPattern_Wrapper

	//Spoof Return Address
	PVOID ReturnAddressStubEnc = 0;
	extern "C" void* _spoofer_stub();
	template<typename Ret = void, typename First = void*, typename Second = void*, typename Third = void*, typename Fourth = void*, typename... Stack>
	__forceinline Ret SpoofCall(void* Func, First a1 = First{}, Second a2 = Second{}, Third a3 = Third{}, Fourth a4 = Fourth{}, Stack... args) {
		struct shell_params { const void* a1; void* a2; void* a3; };
		shell_params CallCtx = { EPtr(ReturnAddressStubEnc), Func, nullptr };
		typedef Ret(*ShellFn)(First, Second, Third, Fourth, PVOID, PVOID, Stack...);
		return ((ShellFn)&FC::_spoofer_stub)(a1, a2, a3, a4, &CallCtx, nullptr, args...);
	}
	__forceinline void SetSpoofStub(PVOID R15_Stub) { ReturnAddressStubEnc = EPtr(R15_Stub); }
#define SpoofCall FC::SpoofCall

	//Spoof VFTable Return Address
	template<typename Ret = void, typename... Args>
	__forceinline Ret VCall(PVOID Class, int Index, Args... AddArgs) {
		return SpoofCall<Ret>((*(PVOID**)Class)[Index], Class, AddArgs...);
	}
#define VCall FC::VCall

	//Spoof Call Export Return Address
	PVOID GetExportAddress(PBYTE hDll, const char* Name)
	{
		//process image data
		PIMAGE_NT_HEADERS NT_Head = (PIMAGE_NT_HEADERS)(hDll + ((PIMAGE_DOS_HEADER)hDll)->e_lfanew);
		PIMAGE_EXPORT_DIRECTORY ExportDir = (PIMAGE_EXPORT_DIRECTORY)(hDll + NT_Head->OptionalHeader.DataDirectory[0].VirtualAddress);

		//process list
		for (DWORD i = 0; i < ExportDir->NumberOfNames; i++)
		{
			//get ordinal & name
			USHORT Ordinal = ((USHORT*)(hDll + ExportDir->AddressOfNameOrdinals))[i];
			const char* ExpName = (const char*)hDll + ((DWORD*)(hDll + ExportDir->AddressOfNames))[i];
			if (StrCmp(Name, ExpName, true))
				return (PVOID)(hDll + ((DWORD*)(hDll + ExportDir->AddressOfFunctions))[Ordinal]);
		} return nullptr;
	}
#define FC(Mod, Name, ...) [&](){ static PVOID FAddr = nullptr; \
		if (!FAddr) FAddr = EPtr(FC::GetExportAddress(FC::GetModuleBase_Wrapper(E(#Mod)), E(#Name))); \
		return SpoofCall<decltype(Name(__VA_ARGS__))>(EPtr(FAddr), __VA_ARGS__); \
	}()
}

//FACE STR Helper v2
#define FC_FORMAT_A(...) [&](){wchar_t Str[64];FC(ntdll, _swprintf, Str, __VA_ARGS__);return Str;}()
#define FC_FORMAT_W(...) [&](){wchar_t Str[64];FC(ntdll, _swprintf, Str, __VA_ARGS__);return Str;}()
#define FC_WCHAR(a) [&](){wchar_t Str[64];for(int i=0;;i++){if(!(Str[i]=a[i]))break;}return Str;}()
#define FC_CHAR(a) [&](){char Str[64];for(int i=0;;i++){if(!(Str[i]=a[i]))break;}return Str;}()

//FACE DBG Helper v2
//#define DEBUG
#ifdef DEBUG
template<typename... Args>
void Print(const char* Format, Args... args)
{
	static bool Once = false;
	if (!Once) {
		FC(kernel32, AllocConsole);
		Once = true;
	}

	char Buff[128];
	FC(ntdll, sprintf, Buff, Format, args...);
	int StrLengthOut = FC(ntdll, strlen, Buff);
	HANDLE Out = FC(kernel32, GetStdHandle, STD_OUTPUT_HANDLE);
	FC(kernel32, WriteConsoleA, Out, Buff, StrLengthOut, NULL, NULL);
}
#define dp(a) Print(E("DEC (%d): %d\n"), __LINE__, (a))
#define hp(a) Print(E("HEX (%d): %p\n"), __LINE__, (a))
#define sp(a) Print(E("STR (%d): %s\n"), __LINE__, (a))
#define wsp(a) Print(E("STR (%d): %ws\n"), __LINE__, (a))
#define fp(a) Print(E("FLOAT (%d): %d.%d\n"), __LINE__, (int)(a), (int)(((a) - (int)(a)) * 1000000));
#define v2p(a) { fp(a.x); fp(a.y); }
#define v3p(a) { fp(a.x); fp(a.y); fp(a.z); }
#endif

//FACE VMT Hook v4
int VMT_FilledRecords = 0;
#pragma comment(linker,"/SECTION:.hkdata,RW")
#pragma bss_seg(push, ".hkdata")
PVOID VMTHookData[10000];
#pragma bss_seg(pop)
class VMT_Hook
{
private:
	PVOID* TblEnc = nullptr; DWORD Size = 0;
	__forceinline bool InExecMemory(PVOID Addr, DWORD64 ModBase, PIMAGE_NT_HEADERS NtHeader) {
		PIMAGE_SECTION_HEADER Sect = IMAGE_FIRST_SECTION(NtHeader);
		for (DWORD Cnt = 0; Cnt < NtHeader->FileHeader.NumberOfSections; Cnt++, Sect++) {
			if ((Sect->Characteristics & IMAGE_SCN_MEM_EXECUTE) &&
				((DWORD64)Addr >= (ModBase + Sect->VirtualAddress)) &&
				((DWORD64)Addr < (ModBase + Sect->VirtualAddress + Sect->Misc.VirtualSize)))
				return true;
		} return false;
	}

public:
	template <typename T>
	VMT_Hook(const char* Module, T Class) {
		DWORD64 ModBase = (DWORD64)GetModBase(Module); Size = 0; PVOID* VFTable = *(PVOID**)Class;
		PIMAGE_NT_HEADERS NtHeader = (PIMAGE_NT_HEADERS)(ModBase + ((PIMAGE_DOS_HEADER)ModBase)->e_lfanew);
		while (InExecMemory(VFTable[Size], ModBase, NtHeader)) Size++;
		PVOID* VFT_New = (PVOID*)&VMTHookData[VMT_FilledRecords];
		__movsq((PDWORD64)VFT_New, (const PDWORD64)VFTable, Size);
		*(PVOID**)Class = VFT_New; TblEnc = (PVOID*)EPtr(VFT_New);
		VMT_FilledRecords += Size;
	}

	template <typename T>
	__forceinline void SetupHook(T* Orginal, PVOID Hook, int Index) {
		PVOID* Tbl = (PVOID*)EPtr(TblEnc);
		*Orginal = (T)Tbl[Index];
		Tbl[Index] = Hook;
	}
};

inline const void* gadget(wchar_t* moduleName)
{
	static const void* jmprbx = nullptr;
	if (!jmprbx) {
		const auto ntdll = reinterpret_cast<const unsigned char*>(::GetModuleHandleW(moduleName));
		const auto dos = reinterpret_cast<const IMAGE_DOS_HEADER*>(ntdll);
		const auto nt = reinterpret_cast<const IMAGE_NT_HEADERS*>(ntdll + dos->e_lfanew);
		const auto sections = IMAGE_FIRST_SECTION(nt);
		const auto num_sections = nt->FileHeader.NumberOfSections;

		constexpr char section_name[5]{ '.', 't', 'e', 'x', 't' };
		const auto     section = std::find_if(sections, sections + num_sections, [&](const auto& s) {
			return std::equal(s.Name, s.Name + 5, section_name);
			});

		constexpr unsigned char instr_bytes[2]{ 0xFF, 0x26 };
		const auto              va = ntdll + section->VirtualAddress;
		jmprbx = std::search(va, va + section->Misc.VirtualSize, instr_bytes, instr_bytes + 2);
	}
	return jmprbx;
}

namespace detail
{
	extern "C" void* _spoofer_stub();

	template <typename Ret, typename... Args>
	static inline auto shellcode_stub_helper(
		const void* shell,
		Args... args
	) -> Ret
	{
		auto fn = (Ret(*)(Args...))(shell);
		return fn(args...);
	}

	template <std::size_t Argc, typename>
	struct argument_remapper
	{
		// At least 5 params
		template<
			typename Ret,
			typename First,
			typename Second,
			typename Third,
			typename Fourth,
			typename... Pack
		>
			static auto do_call(
				const void* shell,
				void* shell_param,
				First first,
				Second second,
				Third third,
				Fourth fourth,
				Pack... pack
			) -> Ret
		{
			return shellcode_stub_helper<
				Ret,
				First,
				Second,
				Third,
				Fourth,
				void*,
				void*,
				Pack...
			>(
				shell,
				first,
				second,
				third,
				fourth,
				shell_param,
				nullptr,
				pack...
				);
		}
	};

	template <std::size_t Argc>
	struct argument_remapper<Argc, std::enable_if_t<Argc <= 4>>
	{
		// 4 or less params
		template<
			typename Ret,
			typename First = void*,
			typename Second = void*,
			typename Third = void*,
			typename Fourth = void*
		>
			static auto do_call(
				const void* shell,
				void* shell_param,
				First first = First{},
				Second second = Second{},
				Third third = Third{},
				Fourth fourth = Fourth{}
			) -> Ret
		{
			return shellcode_stub_helper<
				Ret,
				First,
				Second,
				Third,
				Fourth,
				void*,
				void*
			>(
				shell,
				first,
				second,
				third,
				fourth,
				shell_param,
				nullptr
				);
		}
	};
}


template <typename Ret, typename... Args>
static inline auto spoof_call(
	const void* trampoline,
	Ret(*fn)(Args...),
	Args... args
) -> Ret
{
	struct shell_params
	{
		const void* trampoline;
		void* function;
		void* rdx;
	};

	shell_params p{ trampoline, reinterpret_cast<void*>(fn) };
	using mapper = detail::argument_remapper<sizeof...(Args), void>;
	return mapper::template do_call<Ret, Args...>((const void*)&detail::_spoofer_stub, &p, args...);
}