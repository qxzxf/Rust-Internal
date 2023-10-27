#pragma once
#include <Windows.h>
#include <ctime>
#include <map>
#include <Psapi.h>
#include <urlmon.h>
#include <iomanip> 
#include <wininet.h>
#include <codecvt>
#include <cstdint>
#include <stdio.h>
#include <fstream>
#include <emmintrin.h>
#include <vector>
//v#include <C:\\Program Files (x86)\Windows Kits\10\Include\10.0.18362.0\ucrt\math.h>
#include "il2cpp.h"
#include "chams.h"

//BasePlayer Offsets
#define OFFSET_PLAYER_HEALTH				0x224		//BaseCombatEntity -> _health.					Updated 05/09/21.
#define OFFSET_PLAYER_MODEL					0x4C0		//BasePlayer -> playerModel.					Updated 05/09/21.
#define OFFSET_PLAYER_INPUT					0x4E0		//BasePlayer -> input.							Updated 05/09/21.
#define OFFSET_PLAYER_BASE_MOVEMENT			0x4E8		//BasePlayer -> movement.						Updated 05/09/21.
#define OFFSET_PLAYER_CURRENT_TEAM			0x590		//BasePlayer -> currentTeam.					Updated 05/09/21.
#define OFFSET_PLAYER_ACTIVE_WEAPON_ID		0x5C8		//BasePlayer -> clActiveItem.					Updated 05/09/21.
#define OFFSET_PLAYER_FLAGS					0x650		//BasePlayer -> playerFlags.					Updated 05/09/21.
#define OFFSET_PLAYER_INVENTORY				0x660		//BasePlayer -> inventory.						Updated 05/09/21. 
#define OFFSET_PLAYER_USERID				0x698		//BasePlayer -> userID.							Updated 05/09/21.
#define OFFSET_PLAYER_NAME					0x6B0		//BasePlayer -> _displayName.					Updated 05/09/21.
#define OFFSET_PLAYER_CLIENTTICKINTERVAL	0x620		//BasePlayer -> clientTickInterval.				Updated 12/01/20.
#define OFFSET_PLAYER_EYES					0x658		//BasePlayer -> playerEyes.						Updated 02/10/21.
#define OFFSET_PLAYER_MODEL_STATE			0x5E0		//BasePlayer -> modelState.						Updated 02/15/21.

//PlayerWalkMovement Offsets
#define OFFSET_PWM_GRAVITY_MULTIPLIER		0x74		//PlayerWalkMovement -> gravityMultiplier.		Updated 05/09/21.
#define OFFSET_PWM_MAX_ANGLE_WALKING		0x7C		//PlayerWalkMovement -> maxAngleWalking.		Updated 05/09/21.
#define OFFSET_PWN_MAX_ANGLE_CLIMBING		0x80		//PlayerWalkMovement -> maxAngleClimbing.		Updated 05/09/21.
#define OFFSET_PWM_GROUND_ANGLE				0xB4		//PlayerWalkMovement -> groundAngle.			Updated 05/09/21.
#define OFFSET_PWM_GROUND_ANGLE_NEW			0xB8		//PlayerWalkMovement -> groundAngleNew.			Updated 05/09/21.
#define OFFSET_PWM_GROUND_TIME				0xBC		//PlayerWalkMovement -> groundTime.				Updated 05/09/21.
#define OFFSET_PWM_FLYING					0x13C		//PlayerWalkMovement -> flying.					Updated 05/09/21.


//Item Offsets
#define OFFSET_ITEM_INFO					0x20		//Item -> info.									Updated 05/09/21.
#define OFFSET_ITEM_UID						0x28		//Item -> uid.									Updated 05/09/21.
#define OFFSET_ITEM_AMOUNT					0x30		//Item -> amount.								Updated 05/09/21.
#define OFFSET_ITEM_HELD_ENTITY				0x98		//Item -> heldEntity.							Updated 05/09/21.

#define OFFSET_VIEWOFFSET					0x38		//Vector3 viewOffset 

namespace Globals
{
	int rWidth;
	int rHeight;
}

enum BoneList : int
{
	l_hip = 1,
	l_knee,
	l_foot,
	l_toe,
	l_ankle_scale,
	pelvis,
	penis,
	GenitalCensor,
	GenitalCensor_LOD0,
	Inner_LOD0,
	GenitalCensor_LOD1,
	GenitalCensor_LOD2,
	r_hip,
	r_knee,
	r_foot,
	r_toe,
	r_ankle_scale,
	spine1,
	spine1_scale,
	spine2,
	spine3,
	spine4,
	l_clavicle,
	l_upperarm,
	l_forearm,
	l_hand,
	l_index1,
	l_index2,
	l_index3,
	l_little1,
	l_little2,
	l_little3,
	l_middle1,
	l_middle2,
	l_middle3,
	l_prop,
	l_ring1,
	l_ring2,
	l_ring3,
	l_thumb1,
	l_thumb2,
	l_thumb3,
	IKtarget_righthand_min,
	IKtarget_righthand_max,
	l_ulna,
	neck,
	head,
	jaw,
	eyeTranform,
	l_eye,
	l_Eyelid,
	r_eye,
	r_Eyelid,
	r_clavicle,
	r_upperarm,
	r_forearm,
	r_hand,
	r_index1,
	r_index2,
	r_index3,
	r_little1,
	r_little2,
	r_little3,
	r_middle1,
	r_middle2,
	r_middle3,
	r_prop,
	r_ring1,
	r_ring2,
	r_ring3,
	r_thumb1,
	r_thumb2,
	r_thumb3,
	IKtarget_lefthand_min,
	IKtarget_lefthand_max,
	r_ulna,
	l_breast,
	r_breast,
	BoobCensor,
	BreastCensor_LOD0,
	BreastCensor_LOD1,
	BreastCensor_LOD2,
	collision,
	displacement
};

enum class KeyCode
{
	Backspace = 8,
	Delete = 127,
	Tab = 9,
	Clear = 12,
	Return = 13,
	Pause = 19,
	Escape = 27,
	Space = 32,
	Keypad0 = 256,
	Keypad1 = 257,
	Keypad2 = 258,
	Keypad3 = 259,
	Keypad4 = 260,
	Keypad5 = 261,
	Keypad6 = 262,
	Keypad7 = 263,
	Keypad8 = 264,
	Keypad9 = 265,
	KeypadPeriod = 266,
	KeypadDivide = 267,
	KeypadMultiply = 268,
	KeypadMinus = 269,
	KeypadPlus = 270,
	KeypadEnter = 271,
	KeypadEquals = 272,
	UpArrow = 273,
	DownArrow = 274,
	RightArrow = 275,
	LeftArrow = 276,
	Insert = 277,
	Home = 278,
	End = 279,
	PageUp = 280,
	PageDown = 281,
	F1 = 282,
	F2 = 283,
	F3 = 284,
	F4 = 285,
	F5 = 286,
	F6 = 287,
	F7 = 288,
	F8 = 289,
	F9 = 290,
	F10 = 291,
	F11 = 292,
	F12 = 293,
	F13 = 294,
	F14 = 295,
	F15 = 296,
	Alpha0 = 48,
	Alpha1 = 49,
	Alpha2 = 50,
	Alpha3 = 51,
	Alpha4 = 52,
	Alpha5 = 53,
	Alpha6 = 54,
	Alpha7 = 55,
	Alpha8 = 56,
	Alpha9 = 57,
	Exclaim = 33,
	DoubleQuote = 34,
	Hash = 35,
	Dollar = 36,
	Percent = 37,
	Ampersand = 38,
	Quote = 39,
	LeftParen = 40,
	RightParen = 41,
	Asterisk = 42,
	Plus = 43,
	Comma = 44,
	Minus = 45,
	Period = 46,
	Slash = 47,
	Colon = 58,
	Semicolon = 59,
	Less = 60,
	Equals = 61,
	Greater = 62,
	Question = 63,
	At = 64,
	LeftBracket = 91,
	Backslash = 92,
	RightBracket = 93,
	Caret = 94,
	Underscore = 95,
	BackQuote = 96,
	A = 97,
	B = 98,
	C = 99,
	D = 100,
	E = 101,
	F = 102,
	G = 103,
	H = 104,
	I = 105,
	J = 106,
	K = 107,
	L = 108,
	M = 109,
	N = 110,
	O = 111,
	P = 112,
	Q = 113,
	R = 114,
	S = 115,
	T = 116,
	U = 117,
	V = 118,
	W = 119,
	X = 120,
	Y = 121,
	Z = 122,
	LeftCurlyBracket = 123,
	Pipe = 124,
	RightCurlyBracket = 125,
	Tilde = 126,
	Numlock = 300,
	CapsLock = 301,
	ScrollLock = 302,
	RightShift = 303,
	LeftShift = 304,
	RightControl = 305,
	LeftControl = 306,
	RightAlt = 307,
	LeftAlt = 308,
	LeftCommand = 310,
	LeftApple = 310,
	LeftWindows = 311,
	RightCommand = 309,
	RightApple = 309,
	RightWindows = 312,
	AltGr = 313,
	Help = 315,
	Pr = 316,
	SysReq = 317,
	Break = 318,
	Menu = 319,
	Mouse0 = 323,
	Mouse1 = 324,
	Mouse2 = 325,
	Mouse3 = 326,
	Mouse4 = 327,
	Mouse5 = 328,
	Mouse6 = 329
};

enum class Layers
{
	Terrain = 8388608,
	World = 65536,
	Ragdolls = 512,
	Construction = 2097152,
	ConstructionSocket = 4194304,
	Craters = 1,
	GameTrace = 16384,
	Trigger = 262144,
	VehiclesDetailed = 8192,
	RainFall = 1101070337,
	Deploy = 1235288065,
	DefaultDeployVolumeCheck = 537001984,
	BuildLineOfSightCheck = 2097152,
	ProjectileLineOfSightCheck = 2162688,
	ProjectileLineOfSightCheckTerrain = 10551296,
	MeleeLineOfSightCheck = 2162688,
	EyeLineOfSightCheck = 2162688,
	EntityLineOfSightCheck = 1218519041,
	PlayerBuildings = 18874624,
	PlannerPlacement = 161546496,
	Solid = 1218652417,
	VisCulling = 10551297,
	AltitudeCheck = 1218511105,
	HABGroundEffect = 1218511105,
	AILineOfSight = 1218519297,
	DismountCheck = 1486946561,
	AIPlacement = 278986753,
	WheelRay = 1235321089,
};

enum BUTTON // TypeDefIndex: 10093
{
	// Fields
	value__, // 0x0
	FORWARD = 2, // 0x0
	BACKWARD = 4, // 0x0
	LEFT = 8, // 0x0
	RIGHT = 16, // 0x0
	JUMP = 32, // 0x0
	DUCK = 64, // 0x0
	SPRINT = 128, // 0x0
	USE = 256, // 0x0
	FIRE_PRIMARY = 1024, // 0x0
	FIRE_SECONDARY = 2048, // 0x0
	RELOAD = 8192, // 0x0
	SPRINT_DUCK_FORWARD = 450,
	SPRINT_DUCK_BACKWARD = 452,
	SPRINT_DUCK_LEFT = 456,
	SPRINT_DUCK_RIGHT = 464,
	FIRE_THIRD = 134217728, // 0x0
	FIRE_THIRD_USE_SPRINT_DUCK = 134218176
};

class Vector3
{
public:
	float x, y, z;

	inline Vector3() {
		x = y = z = 0.0f;
	}

	inline Vector3(float X, float Y, float Z) {
		x = X; y = Y; z = Z;
	}

	inline float operator[](int i) const {
		return ((float*)this)[i];
	}

	inline Vector3& operator-=(float v) {
		x -= v; y -= v; z -= v; return *this;
	}

	inline Vector3 operator*(float v) const {
		return Vector3(x * v, y * v, z * v);
	}

	inline Vector3 operator/(float v) const
	{
		return Vector3(x / v, y / v, z / v);
	}

	inline Vector3& operator+=(const Vector3& v) {
		x += v.x; y += v.y; z += v.z; return *this;
	}

	inline Vector3& operator-=(const Vector3& v) {
		x -= v.x; y -= v.y; z -= v.z; return *this;
	}

	inline Vector3 operator-(const Vector3& v) const {
		return Vector3(x - v.x, y - v.y, z - v.z);
	}

	inline Vector3 operator+(const Vector3& v) const {
		return Vector3(x + v.x, y + v.y, z + v.z);
	}

	inline float Length() {
		return sqrtf(x * x + y * y + z * z);
	}

	inline Vector3 Normalize()
	{
		float mag = Length();
		if (mag > 0.00001f)
		{
			x = x / mag;
			y = y / mag;
			z = z / mag;
		}
		else
		{
			x = 0;
			y = 0;
			z = 0;
		}
		return *this;
	}

	inline bool operator==(const Vector3& v) const {
		if (this->x == v.x && this->y == v.y && this->z == v.z)
			return true;
		else
			return false;
	}

	inline bool operator!=(const Vector3& v) const {
		if (this->x == v.x && this->y == v.y && this->z == v.z)
			return false;
		else
			return true;
	}
};

class Vector2 {
public:
	float x, y;

	inline Vector2() {
		x = y = 0.0f;
	}

	inline Vector2 operator/(float v) const {
		return Vector2(x / v, y / v);
	}
	inline Vector2(float X, float Y) {
		x = X; y = Y;
	}

	inline Vector2 operator-(const Vector2& v) const {
		return Vector2(x - v.x, y - v.y);
	}

	inline Vector2 operator+(const Vector2& v) const {
		return Vector2(x + v.x, y + v.y);
	}

	inline Vector2& operator+=(const Vector2& v) {
		x += v.x; y += v.y; return *this;
	}

	inline Vector2& operator-=(const Vector2& v) {
		x -= v.x; y -= v.y; return *this;
	}

	inline bool Zero() const {
		return (x > -0.1f && x < 0.1f && y > -0.1f && y < 0.1f);
	}

	inline Vector2& operator/=(float fl)
	{
		float oofl = 1.0f / fl;
		x *= oofl;
		y *= oofl;
		return *this;
	}

	inline bool operator==(const Vector2& v) const {
		if (this->x == v.x && this->y == v.y)
			return true;
		else
			return false;
	}


	inline Vector2 Clamp()
	{
		if (x > 180)
			x -= 360;
		else if (x < -180)
			x += 360;

		if (y > 180)
			y -= 360;
		else if (y < -180)
			y += 360;

		if (x < -89)
			x = -89;

		if (x > 89)
			x = 89;

		while (y < -180.0f)
			y += 360.0f;

		while (y > 180.0f)
			y -= 360.0f;

		return *this;
	}
};

class Vector4 {
public:
	float x, y, z, w;

public:
	inline Vector4() {
		x = y = z = w = 0.0f;
	}

	inline Vector4(float X, float Y, float Z, float W) {
		x = X; y = Y; z = Z; w = W;
	}

	inline float operator[](int i) const {
		return ((float*)this)[i];
	}
};

struct Matrix4x4 {
	union {
		struct {
			float        _11, _12, _13, _14;
			float        _21, _22, _23, _24;
			float        _31, _32, _33, _34;
			float        _41, _42, _43, _44;

		}; float m[4][4];
	};
};

namespace Math
{
#define M_PI 3.14159265358979323846f
#define M_RADPI	57.295779513082f
#define M_PI_F ((float)(M_PI))
#define RAD2DEG(x) ((float)(x) * (float)(180.f / M_PI_F))
#define DEG2RAD(x) ((float)(x) * (float)(M_PI_F / 180.f))
	//#define atan2(a, b) ((float)FC_NTDLL(atan2, (double)(a), (double)(b)))
	static volatile const double Tiny = 0x1p-1022;
	static volatile const double Infinity = INFINITY;
	typedef struct { double d0, d1; } double2;

	static inline double2 Mul112(double a, double b)
	{
		static const double c = 0x1p27 + 1;

		double
			ap = a * c, bp = b * c,
			a0 = (a - ap) + ap, b0 = (b - bp) + bp,
			a1 = a - a0, b1 = b - b0,
			d0 = a * b,
			d1 = a0 * b0 - d0 + a0 * b1 + a1 * b0 + a1 * b1;
		return double2{ d0, d1 };
	}

	static inline double2 Mul222(double2 a, double2 b)
	{
		static const double c = 0x1p27 + 1;

		double
			ap = a.d0 * c, bp = b.d0 * c,
			a0 = (a.d0 - ap) + ap, b0 = (b.d0 - bp) + bp,
			a1 = a.d0 - a0, b1 = b.d0 - b0,
			m0 = a.d0 * b.d0,
			m1 = a0 * b0 - m0 + a0 * b1 + a1 * b0 + a1 * b1 + (a.d0 * b.d1 + a.d1 * b.d0),
			d0 = m0 + m1,
			d1 = m0 - d0 + m1;
		return double2{ d0, d1 };
	}

	static inline double Mul121Special(double a, double2 b)
	{
		static const double c = 0x1p27 + 1;

		double
			ap = a * c, bp = b.d0 * c,
			a0 = (a - ap) + ap, b0 = (b.d0 - bp) + bp,
			a1 = a - a0, b1 = b.d0 - b0,
			m1 = a0 * b0 - 1 + a0 * b1 + a1 * b0 + a1 * b1 + a * b.d1;
		return m1;
	}

	static inline double2 Add212RightSmaller(double2 a, double b)
	{
		double
			c0 = a.d0 + b,
			c1 = a.d0 - c0 + b + a.d1,
			d0 = c0 + c1,
			d1 = c0 - d0 + c1;
		return double2{ d0, d1 };
	}

	static inline double Add221RightSmaller(double2 a, double2 b)
	{
		double
			c0 = a.d0 + b.d0,
			c1 = a.d0 - c0 + b.d0 + b.d1 + a.d1,
			d0 = c0 + c1;
		return d0;
	}

	float abs(float a) {
		if (a < 0.f) return -a;
		else return a;
	}

	float asin(float x) {
		float negate = float(x < 0);
		x = abs(x);
		float ret = -0.0187293;
		ret *= x;
		ret += 0.0742610;
		ret *= x;
		ret -= 0.2121144;
		ret *= x;
		ret += 1.5707288;
		ret = 3.14159265358979 * 0.5 - sqrt(1.0 - x) * ret;
		return ret - 2 * negate * ret;
	}

	__forceinline float Dot(const Vector3& Vec1, const Vector3& Vec2) {
		return Vec1[0] * Vec2[0] + Vec1[1] * Vec2[1] + Vec1[2] * Vec2[2];
	}

	__forceinline float Calc3D_Dist(const Vector3& Src, const Vector3& Dst) {
		return sqrtf(pow((Src.x - Dst.x), 2) + pow((Src.y - Dst.y), 2) + pow((Src.z - Dst.z), 2));
	}

	__forceinline float Calc2D_Dist(const Vector2& Src, const Vector2& Dst) {
		return sqrt(powf(Src.x - Dst.x, 2) + powf(Src.y - Dst.y, 2));
	}
#define RAD2DEG(x) ((float)(x) * (float)(180.f / M_PI_F))
	__forceinline Vector2 CalcAngle(const Vector3& Src, const Vector3& Dst) {
		Vector3 dir = Src - Dst;
		float length = dir.Length();
		return Vector2{ RAD2DEG(asin(dir.y / length)), RAD2DEG(-atan2(dir.x, -dir.z)) };
	}

	void Normalize(float& Yaw, float& Pitch) {
		if (Pitch < -89) Pitch = -89;
		else if (Pitch > 89) Pitch = 89;
		if (Yaw < -360) Yaw += 360;
		else if (Yaw > 360) Yaw -= 360;
	}

}

std::string ws2s(const std::wstring& wstr)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(wstr);
}

std::wstring s2ws(const std::string& str)
{
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}

std::wstring read_unicode(uint64_t address, std::size_t size)
{
	const auto buffer = std::make_unique<wchar_t[]>(size);
	mem.Read(address, buffer.get(), size * 2);
	return std::wstring(buffer.get());
}

std::string read_ascii(uint64_t address, std::size_t size)
{
	std::unique_ptr<char[]> buffer(new char[size]);
	mem.Read(address, buffer.get(), size);
	return std::string(buffer.get());
}

class il2cppstring
{
public:
	char pad_0000[16];
	uint32_t ReadAmount;
	wchar_t buffer[];
};

class ItemModProjectile
{
public:
	uintptr_t mod;

public:
	float get_projectile_velocity() {
		return mem.Read<float>(mod + 0x34);
	}

	float projectileSpread() {
		return mem.Read<float>(mod + 0x30);
	}

	float projectileVelocitySpread() {
		return mem.Read<float>(mod + 0x38);
	}

	void set_projectile_spread(float new_spread)
	{
		mem.Write<float>(mod + 0x30, new_spread);
	}

	void set_projectile_velocity_spread(float new_spread)
	{
		mem.Write<float>(mod + 0x38, new_spread);
	}
};

class ItemClass
{
public:
	uintptr_t Item;
	uintptr_t ObjectClass;
	char buffer[0x1000];

public:
	std::map<std::wstring, float> BulletSpeeds = {
		/* For 5.56 Fed Weapons */
		{ std::wstring(L"ammo.rifle"), 1.0f },
		{ std::wstring(L"ammo.rifle.hv"), 1.2f },
		{ std::wstring(L"ammo.rifle.explosive"), 0.6f },
		{ std::wstring(L"ammo.rifle.incendiary"), 0.6f },

		/* For Pistol Ammunition Fed Weapons */
		{ std::wstring(L"ammo.pistol"), 1.0f },
		{ std::wstring(L"ammo.pistol.hv"), 1.33333f },
		{ std::wstring(L"ammo.pistol.incendiary"), 0.75f },

		/* For Weapons That Use Arrows */
		{ std::wstring(L"arrow.wooden"), 1.0f },
		{ std::wstring(L"arrow.hv"), 1.6f },
		{ std::wstring(L"arrow.fire"), 0.8f },
		{ std::wstring(L"arrow.bone"), 0.9f },

		/* For Shotguns */
		{ std::wstring(L"ammo.handmade.shell"), 1.0f },
		{ std::wstring(L"ammo.shotgun.slug"), 2.25f },
		{ std::wstring(L"ammo.shotgun.fire"), 1.0f },
		{ std::wstring(L"ammo.shotgun"), 2.25f },

		{ std::wstring(L"ammo.nailgun.nails"), 1.0f }
	};

	std::map<std::wstring, float> BulletGravity = {
		/* For 5.56 Fed Weapons */
		{ std::wstring(L"ammo.rifle"), 1.0f },
		{ std::wstring(L"ammo.rifle.hv"), 1.0f },
		{ std::wstring(L"ammo.rifle.explosive"), 1.25f },
		{ std::wstring(L"ammo.rifle.incendiary"), 1.0f },

		/* For Pistol Ammunition Fed Weapons */
		{ std::wstring(L"ammo.pistol"), 1.0f },
		{ std::wstring(L"ammo.pistol.hv"), 1.0f },
		{ std::wstring(L"ammo.pistol.incendiary"), 1.0f },

		/* For Weapons That Use Arrows */
		{ std::wstring(L"arrow.wooden"), 0.75f },
		{ std::wstring(L"arrow.hv"), 0.5f },
		{ std::wstring(L"arrow.fire"), 1.0f },
		{ std::wstring(L"arrow.bone"), 0.75f },

		/* For Shotguns */
		{ std::wstring(L"ammo.handmade.shell"), 1.0f },
		{ std::wstring(L"ammo.shotgun.slug"), 1.0f },
		{ std::wstring(L"ammo.shotgun.fire"), 1.0f },
		{ std::wstring(L"ammo.shotgun"), 1.0f },

		{ std::wstring(L"ammo.nailgun.nails"), 0.75f }
	};

	bool operator==(ItemClass p)
	{
		if (p.Item == this->Item)
			return true;
		else
			return false;
	}

public:
	int GetAmount()
	{
		if (!Item)
			return 0;

		return mem.Read<int>(Item + OFFSET_ITEM_AMOUNT); //Item -> amount.
	}

	int GetUID()
	{
		if (!Item)
			return 0;

		return mem.Read<int>(Item + OFFSET_ITEM_UID); //Item -> uid.
	}

	ItemModProjectile GetItemModProjectile()
	{
		if (!Item)
			return ItemModProjectile();

		ItemModProjectile projectile;
		projectile.mod = mem.Read<uintptr_t>(Item + 0x4E0); //BasePlayer -> input.
		return projectile;
	}

	std::wstring GetItemName()
	{
		if (!(uintptr_t)Item)
		{
			//std::cout << "GIN !Item!" << std::endl;
			Sleep(2500);
			return L"No Item!";
		}

		auto Info = mem.Read<uintptr_t>((uintptr_t)Item + OFFSET_ITEM_INFO); //Item -> info.
		if (!Info)
		{
			//std::cout << "GIN !Info!" << std::endl;
			Sleep(2500);
			return L"No Item!";
		}

		auto ShortName = mem.Read<uintptr_t>(Info + 0x20); //ItemDefinitnion -> shortname.
		if (!ShortName)
		{
			//std::cout << "GIN !ShortName!" << std::endl;
			Sleep(2500);
			return L"No Item!";
		}


		auto wname = read_unicode(ShortName + 0x14, 64);
		if (!wname.empty())
		{
			return wname;
		}
		//std::cout << "GIN wname == empty!" << std::endl;
		Sleep(2500);
		return L"No Item!";
	}

	bool IsItemGun()
	{
		if (!Item)
			return false;

		auto ItemName = GetItemName();
		if (ItemName == L"No Item!")
			return false;


		if (ItemName.find(L"rifle.") != std::string::npos && ItemName.find(L"ammo.") == std::string::npos)
		{
			return true;
		}
		else if (ItemName.find(L"pistol.") != std::string::npos && ItemName.find(L"ammo.") == std::string::npos)
		{
			return true;
		}
		else if (ItemName.find(L"bow.") != std::string::npos && ItemName.find(L"ammo.") == std::string::npos)
		{
			return true;
		}
		else if (ItemName.find(L"lmg.") != std::string::npos && ItemName.find(L"ammo.") == std::string::npos)
		{
			return true;
		}
		else if (ItemName.find(L"shotgun.") != std::string::npos && ItemName.find(L"ammo.") == std::string::npos)
		{
			return true;
		}
		else if (ItemName.find(L"smg.") != std::string::npos && ItemName.find(L"ammo.") == std::string::npos)
		{
			return true;
		}
		else
			return false;
	}

	std::string GetItemClassName()
	{
		auto Held = mem.Read<uintptr_t>(Item + OFFSET_ITEM_HELD_ENTITY);
		const auto baseObject = mem.Read<uintptr_t>(Held + 0x10);

		const auto object = mem.Read<uintptr_t>(baseObject + 0x30);
		char className[64];
		auto name_pointer = mem.Read<uint64_t>(object + 0x60);
		mem.Read(name_pointer, &className, sizeof(className));
		return className;
	}

	std::wstring GetAmmoName()
	{
		auto Held = mem.Read<uintptr_t>(Item + OFFSET_ITEM_HELD_ENTITY); //Item -> heldEntity. //Leads to BaseProjectile.
		if (!Held)
			return L"No Item!";

		if (!IsItemGun())
			return L"No Item!";

		auto primaryMagazine = mem.Read<uintptr_t>(Held + 0x2A0);
		if (!primaryMagazine)
			return L"No Item!";

		auto Info = mem.Read<uintptr_t>(primaryMagazine + 0x20); //Item -> info.
		if (!Info)
		{
			//std::cout << "!Info!" << std::endl;
			return L"No Item!";
		}

		auto ShortName = mem.Read<uintptr_t>(Info + 0x20); //ItemDefinitnion -> shortname.
		if (!ShortName)
		{
			//std::cout << "!ShortName!" << std::endl;
			return L"No Item!";
		}


		auto wname = read_unicode(ShortName + 0x14, 64);
		if (!wname.empty())
		{
			return wname;
		}
		//std::cout << "wname == empty!" << std::endl;
		return L"No Item!";
	}

	float GetWeaponSpeed()
	{
		auto WeaponName = ws2s(GetItemName());
		float Speed = 0.f;

		/* Rifles */
		if (WeaponName == "rifle.ak")
		{
			Speed += 375.0f;
		}
		if (WeaponName == "rifle.lr300")
		{
			Speed += 375.0f;
		}
		if (WeaponName == "rifle.bolt")
		{
			Speed += 656.0f;
		}
		if (WeaponName == "rifle.l96")
		{
			Speed += 1125.0f;
		}
		if (WeaponName == "rifle.m39")
		{
			Speed += 469.0f;
		}
		if (WeaponName == "rifle.semiauto")
		{
			Speed += 375.0f;
		}
		/* LMG */
		if (WeaponName == "lmg.m249")
		{
			Speed += 488.0f;
		}
		/* SMGs */
		if (WeaponName == "smg.thompson")
		{
			Speed += 300.0f;
		}
		if (WeaponName == "smg.custom")
		{
			Speed += 240.0f;
		}
		if (WeaponName == "smg.mp5")
		{
			Speed += 240.0f;
		}
		/* Pistols */
		if (WeaponName == "pistol.python")
		{
			Speed += 300.0f;
		}
		if (WeaponName == "pistol.semi")
		{
			Speed += 300.0f;
		}
		if (WeaponName == "pistol.revolver")
		{
			Speed += 300.0f;
		}
		if (WeaponName == "pistol.m92")
		{
			Speed += 300.0f;
		}
		if (WeaponName == "pistol.eoka")
		{
			Speed += 100.0f;
		}
		if (WeaponName == "pistol.nailgun")
		{
			Speed += 50.0f;
		}
		/* Bows/Arrows */
		if (WeaponName == "crossbow")
		{
			Speed += 75.0f;
		}
		if (WeaponName == "bow.compound")
		{
			Speed += 100.0f;
		}
		if (WeaponName == "bow.hunting")
		{
			Speed += 60.0f;
		}
		/* Shotguns */
		if (WeaponName == "shotgun.pump")
		{
			Speed += 100.0f;
		}
		if (WeaponName == "shotgun.spas12")
		{
			Speed += 100.0f;
		}
		if (WeaponName == "shotgun.waterpipe")
		{
			Speed += 100.0f;
		}
		if (WeaponName == "shotgun.doublebarrel")
		{
			Speed += 100.0f;
		}
		if (Speed == 0.f)
		{
			Speed += 300.0f;
		}

		return Speed;
	}

	float GetBulletSpeed()
	{

		auto bullet_name = ws2s(GetAmmoName());
		auto ItemName = GetItemName();
		if (bullet_name == "ammo.rifle")
		{
			if (ItemName.find(L"L96") != std::string::npos)
			{
				return 1125.f;
			}
			if (ItemName.find(L"bolt") != std::string::npos)
			{
				return 656.f;
			}
			return 375.f;
		}
		else if (bullet_name == "ammo.rifle.explosive")
		{
			if (ItemName.find(L"L96") != std::string::npos)
			{
				return 1125.f;
			}
			if (ItemName.find(L"bolt") != std::string::npos)
			{
				return 656.f;
			}
			return 225.f;
		}
		else if (bullet_name == "ammo.rifle.hv")
		{
			if (ItemName.find(L"L96") != std::string::npos)
			{
				return 1125.f;
			}
			if (ItemName.find(L"bolt") != std::string::npos)
			{
				return 656.f;
			}
			return 450.f;
		}
		else if (bullet_name == "ammo.rifle.incendiary")
		{
			if (ItemName.find(L"L96") != std::string::npos)
			{
				return 1125.f;
			}
			if (ItemName.find(L"bolt") != std::string::npos)
			{
				return 656.f;
			}
			return 225.f;
		}
		else if (bullet_name == "ammo.pistol")
		{
			return 300.f;
		}
		else if (bullet_name == "ammo.pistol.fire")
		{
			return 225.f;
		}
		else if (bullet_name == "ammo.pistol.hv")
		{
			return 400.f;
		}
		else if (bullet_name == "ammo.shotgun")
		{
			return 225.f;
		}
		else if (bullet_name == "ammo.shotgun.slug")
		{
			return 225.f;
		}
		else if (bullet_name == "ammo.shotgun.fire")
		{
			return 225.f;
		}
		else if (bullet_name == "ammo.handmade.shell")
		{
			return 225.f;
		}
		else if (bullet_name == "arrow.wooden")
		{
			return 225.f;
		}
		else if (bullet_name == "arrow.hv")
		{
			return 225.f;
		}
		else if (bullet_name == "arrow.fire")
		{
			return 225.f;
		}
		else if (bullet_name == "arrow.bone")
		{
			return 225.f;
		}
		else if (bullet_name == "ammo.nailgun.nails")
		{
			return 225.f;
		}
	}

	void SetAutomatic()
	{
		auto Held = mem.Read<uintptr_t>(Item + OFFSET_ITEM_HELD_ENTITY); //Item -> heldEntity. //Leads to BaseProjectile.
		if (!Held)
			return;
		if (!IsItemGun())
			return;
		mem.Write<bool>(Held + 0x288, true); //BaseProjectile -> automatic.
	}

	void SetRapidFire()
	{
		auto Held = mem.Read<uintptr_t>(Item + OFFSET_ITEM_HELD_ENTITY); //Item -> heldEntity. //Leads to AttackEntity.
		if (!Held)
			return;
		if (!IsItemGun())
			return;
		mem.Write<float>(Held + 0x1E4, 0.05f); //AttackEntity (Extended from HeldEntity) -> repeatDelay.
	}

	void SetAlwaysEoka()
	{
		auto ItemName = GetItemName();
		if (ItemName.find(L"eoka") != std::string::npos)
		{
			auto Held = mem.Read<uintptr_t>(Item + OFFSET_ITEM_HELD_ENTITY); //Item -> heldEntity. //Leads to AttackEntity.
			if (!Held)
				return;
			if (!IsItemGun())
				return;

			mem.Write<float>(Held + 0x360, 100.f); //FlintStrikeWeapon (Extended from BaseProjectile) -> successFraction.
		}
	}

	void SetFastBow()
	{
		auto ItemName = GetItemName();
		if (ItemName.find(L"bow") != std::string::npos)
		{
			auto Held = mem.Read<uintptr_t>(Item + OFFSET_ITEM_HELD_ENTITY);

			if (!Held)
				return;

			if (!IsItemGun())
				return;

			mem.Write<float>(Held + 0x364, 0.f); //arrowback
		}
	}

	Vector3 GetVisualPosition()
	{
		if (!ObjectClass)
		{
			return Vector3();
		}

		auto Transform = mem.Read<uintptr_t>(ObjectClass + 0x8);
		if (!Transform)
		{
			return Vector3();
		}

		auto VisualState = mem.Read<uintptr_t>(Transform + 0x38);
		if (!VisualState)
		{
			return Vector3();
		}

		return mem.Read<Vector3>(VisualState + 0x90);
		//return *(Vector3*)(VisualState + 0x90);
	}

	float GetBulletVelocity()
	{
		/* Read held item to a string value */
		auto held_item = GetItemName();

		auto ammoname = GetAmmoName();

		if (ammoname == L"No Item!")
		{
			std::cout << "Ammo Name Bad" << std::endl;
			return 300.f;
		}

		if (ammoname.empty() || !(ammoname.size() > 0))
		{
			std::cout << "Ammo Name Bad 2" << std::endl;
			return 300.f;
		}

		float Ammunition_Multiplier = BulletSpeeds[ammoname];

		/* Return some bogus value if it isn't held just a safety check if other functions don't work */
		if (held_item == L"No Item")
		{
			std::cout << "Ammo Name Bad 3" << std::endl;
			return 300.0f;
		}

		/*
		* Decide weapons velocity based off name (( DEFAULT BULLET TYPE ))
		*/
		//std::cout << "Held Item: " << ws2s(held_item) << std::endl;
		/* Normal 556 rifle ammunition */
		if (held_item.find(L"rifle.ak") != std::string::npos)				 return 375.0f * Ammunition_Multiplier;
		if (held_item.find(L"rifle.lr300") != std::string::npos)             return 375.0f * Ammunition_Multiplier;
		if (held_item.find(L"rifle.bolt") != std::string::npos)              return 656.0f * Ammunition_Multiplier;
		if (held_item.find(L"rifle.l96") != std::string::npos)               return 1125.0f * Ammunition_Multiplier;
		if (held_item.find(L"rifle.m39") != std::string::npos)               return 469.0f * Ammunition_Multiplier;
		if (held_item.find(L"rifle.semiauto") != std::string::npos)          return 375.0f * Ammunition_Multiplier;

		/* Default 556 ammunition */
		if (held_item.find(L"lmg.m249") != std::string::npos)                return 488.0f * Ammunition_Multiplier;

		/* Normal pistol bullet ammunition */
		if (held_item.find(L"smg.thompson") != std::string::npos)            return 300.0f * Ammunition_Multiplier;
		if (held_item.find(L"smg.custom") != std::string::npos)              return 240.0f * Ammunition_Multiplier;
		if (held_item.find(L"smg.mp5") != std::string::npos)                 return 240.0f * Ammunition_Multiplier;

		/* Normal pistol bullet ammunition, hand made shell for Eoka  */
		if (held_item.find(L"pistol.python") != std::string::npos)           return 300.0f * Ammunition_Multiplier;
		if (held_item.find(L"pistol.semi") != std::string::npos)             return 300.0f * Ammunition_Multiplier;
		if (held_item.find(L"pistol.revolver") != std::string::npos)         return 300.0f * Ammunition_Multiplier;
		if (held_item.find(L"pistol.m92") != std::string::npos)              return 300.0f * Ammunition_Multiplier;
		if (held_item.find(L"pistol.eoka") != std::string::npos)             return 100.0f * Ammunition_Multiplier;
		if (held_item.find(L"pistol.nailgun") != std::string::npos)          return 50.0f;

		/* Plain arrow */
		if (held_item.find(L"crossbow") != std::string::npos)                return 75.0f * Ammunition_Multiplier;
		if (held_item.find(L"bow.compound") != std::string::npos)            return 100.0f * Ammunition_Multiplier;
		if (held_item.find(L"bow.hunting") != std::string::npos)             return 50.0f * Ammunition_Multiplier;

		/* Hand made shell */
		if (held_item.find(L"shotgun.pump") != std::string::npos)            return 100.0f * Ammunition_Multiplier;
		if (held_item.find(L"shotgun.spas12") != std::string::npos)          return 100.0f * Ammunition_Multiplier;
		if (held_item.find(L"shotgun.waterpipe") != std::string::npos)       return 100.0f * Ammunition_Multiplier;
		if (held_item.find(L"shotgun.doublebarrel") != std::string::npos)    return 100.0f * Ammunition_Multiplier;

		/* If all fails then return a zero value to void all post calculations */
		return 300.0f;
	}

	float GetBulletGravity()
	{
		auto ammoname = GetAmmoName();
		if (ammoname == L"No Item!")
			return 1;

		if (ammoname.empty() || !(ammoname.size() > 0))
			return 1;

		return BulletGravity[ammoname];
	}
};

class ItemContainer
{
public:
	uintptr_t ItemContainer;

public:
	ItemClass GetItem(int id)
	{
		ItemClass item;
		item.Item = NULL;
		auto item_list = mem.Read<uintptr_t>((uintptr_t)ItemContainer + 0x38); //ItemContainer -> itemList.

		if (!item_list)
		{
			//std::cout << "!belt_list" << std::endl;
			return ItemClass();
		}

		auto Items = mem.Read<uintptr_t>((uintptr_t)item_list + 0x10);

		if (!Items)
		{
			//std::cout << "!Items" << std::endl;
			return item;
		}

		auto TheItem = mem.Read<uintptr_t>(Items + 0x20 + (id * 0x8));
		if (!TheItem)
		{
			//std::cout << "!Item" << std::endl;
			return item;
		}

		item.Item = TheItem;
		return item;
	}
};

class PlayerInventory
{
public:
	uintptr_t Inventory;
public:
	ItemContainer GetBelt()
	{
		ItemContainer ic;
		ic.ItemContainer = mem.Read<uintptr_t>((uintptr_t)Inventory + 0x28); //PlayerInventory -> containerBelt.
		return ic;
	}
};

class PlayerModel
{
public:
	uintptr_t Model;

public:
	bool IsVisible()
	{
		return mem.Read<bool>(Model + 0x260); //PlayerModel -> visible.
	}
};

class InputMessage
{
public:
	uintptr_t InputMessage;

public:
	void SetAimAngles(Vector3 newAngle)
	{
		if (!InputMessage)
			return;

		mem.Write<Vector3>(InputMessage + 0x18, newAngle); //InputMessage -> aimAngles.
	}
};

class PlayerState
{
public:
	uintptr_t State;

public:
	InputMessage GetCurrent()
	{
		if (!State)
			return InputMessage{};

		InputMessage inputmsg;
		inputmsg.InputMessage = mem.Read<uintptr_t>(State + 0x10); //PlayerState -> current.
		return inputmsg;
	}
};

class PlayerInput
{
public:
	uintptr_t Input;

public:
	Vector2 GetViewAngles()
	{
		return mem.Read<Vector2>(Input + 0x3C); //PlayerInput -> bodyAngles
	}

	Vector2 GetRecoilAngle()
	{
		return mem.Read<Vector2>(Input + 0x64); //PlayerInput -> recoilAngles
	}

	PlayerState GetState()
	{
		if (!Input)
			return PlayerState{};

		PlayerState state;
		state.State = mem.Read<uintptr_t>(Input + 0x20); //PlayerInput -> state.
		return state;
	}

	void SetViewAngles(Vector2 angle)
	{
		mem.Write<Vector2>(Input + 0x3C, angle); //PlayerInput -> bodyAngles
	}

	void SetRecoilAngle(Vector2 angle)
	{
		mem.Write<Vector2>(Input + 0x64, angle); //PlayerInput -> recoilAngles
	}
};

class PlayerTeam
{
public:
	void Test()
	{
		return;
	}
};

class PlayerEyes
{
public:
	uintptr_t eyes = NULL;

public:
	Vector4 GetBodyRotations()
	{
		if (!eyes)
			return Vector4();

		return mem.Read<Vector4>(eyes + 0x44);
	}

	void SetBodyRotations(Vector4 new_rotation)
	{
		if (!eyes)
		{
			std::cout << "!eyes!" << std::endl;
			return;
		}

		return mem.Write<Vector4>(eyes + 0x44, new_rotation);
	}
};

class ModelState
{
public:
	uintptr_t state = NULL;

public:
	void remove_flag(int flag)
	{
		int flags = mem.Read<int>(state + 0x40); //BasePlayer -> playerFlags.

		flags &= ~(int)flag;

		mem.Write<int>(state + 0x40, flags); //BasePlayer -> playerFlags.
	}

	void add_flag(int flag)
	{
		int flags = mem.Read<int>(state + 0x40); //BasePlayer -> playerFlags.

		flags |= (int)flag;

		mem.Write<int>(state + 0x40, flags); //BasePlayer -> playerFlags.
	}
};




class BasePlayer
{
public:
	uintptr_t Player = NULL;
	uintptr_t ObjectClass = NULL;
	bool IsTeamMate = false;
	bool IsLocalPlayer = false;

public:
	bool operator==(BasePlayer p)
	{
		if (p.Player == this->Player)
			return true;
		else
			return false;
	}

	BasePlayer()
	{
		Player = NULL;
		ObjectClass = NULL;
	}

public:
	std::wstring GetName()
	{
		if (!Player)
			return L"";

		auto nameptr = mem.Read<uintptr_t>(Player + OFFSET_PLAYER_NAME); //BasePlayer -> _displayName.
		if (!nameptr)
			return L"";

		return read_unicode(nameptr + 0x14, 64);
	}

	uintptr_t get_player_model() {
		return *reinterpret_cast<uintptr_t*>((uintptr_t)this + OFFSET_PLAYER_MODEL);
	}

	long GetUID()
	{
		if (!Player)
			return NULL;

		return mem.Read<long>(Player + OFFSET_PLAYER_USERID);
	}

	float GetHealth()
	{
		if (!Player)
			return NULL;

		return mem.Read<float>(Player + OFFSET_PLAYER_HEALTH); //BaseCombatEntity -> _health.
	}

	int GetActiveWeaponID()
	{
		if (!Player)
			return NULL;

		return mem.Read<int>(Player + OFFSET_PLAYER_ACTIVE_WEAPON_ID); //BasePlayer -> clActiveItem.
	}

	bool IsSleeping()
	{
		if (!Player)
			return NULL;

		auto Flags = mem.Read<int>(Player + OFFSET_PLAYER_FLAGS); //BasePlayer -> playerFlags.
		return Flags & 16;
	}

	void SetAdminFlag()
	{
		if (!Player)
			return;

		int flags = mem.Read<int>(Player + OFFSET_PLAYER_FLAGS); //BasePlayer -> playerFlags.

		flags |= 4;

		mem.Write<uintptr_t>(Player + OFFSET_PLAYER_FLAGS, flags); //BasePlayer -> playerFlags.
	}

	float GetGravity()
	{
		if (!Player)
			return NULL;

		auto PlayerWalkMovement = mem.Read<uintptr_t>(Player + OFFSET_PLAYER_BASE_MOVEMENT); //BasePlayer -> movement.
		if (!PlayerWalkMovement)
			return 2.5f;

		return mem.Read<float>(PlayerWalkMovement + OFFSET_PWM_GRAVITY_MULTIPLIER); //PlayerWalkMovement (Extended from BaseMovement) -> gravityMultiplier.
	}

	void SetGravity(float grav)
	{
		if (!Player)
			return;

		auto PlayerWalkMovement = mem.Read<uintptr_t>(Player + OFFSET_PLAYER_BASE_MOVEMENT); //BasePlayer -> movement.
		if (!PlayerWalkMovement)
			return;

		mem.Write<float>(PlayerWalkMovement + OFFSET_PWM_GRAVITY_MULTIPLIER, grav); //PlayerWalkMovement (Extended from BaseMovement) -> gravityMultiplier.
	}

	void DoSpider()
	{
		if (!Player)
			return;

		auto BaseMovement = mem.Read<uintptr_t>(Player + OFFSET_PLAYER_BASE_MOVEMENT); //BasePlayer -> movement.
		if (!BaseMovement)
			return;

		mem.Write<float>(BaseMovement + OFFSET_PWM_GROUND_ANGLE, 0.f); //PlayerWalkMovement (Extended from BaseMovement) -> groundAngle;
		mem.Write<float>(BaseMovement + OFFSET_PWM_GROUND_ANGLE_NEW, 0.f); //PlayerWalkMovement (Extended from BaseMovement) -> groundAngleNew;
		mem.Write<float>(BaseMovement + OFFSET_PWM_MAX_ANGLE_WALKING, 999.f); //PlayerWalkMovement (Extended from BaseMovement) -> maxAngleWalking;
		mem.Write<float>(BaseMovement + OFFSET_PWN_MAX_ANGLE_CLIMBING, 999.f); //PlayerWalkMovement (Extended from BaseMovement) -> maxAngleClimbing;
		mem.Write<float>(BaseMovement + OFFSET_PWM_GROUND_TIME, 99999999999999.f); //PlayerWalkMovement (Extended from BaseMovement) -> groundTime;
	}

	float longneckupwards = 1.5f;

	void LongNeckUpwards() {

		auto eyes = *reinterpret_cast<uintptr_t*>((uintptr_t)this + 0x658); //public PlayerEyes eyes; // 0x600
		mem.Write<Vector3>(eyes + OFFSET_VIEWOFFSET, Vector3(0, (longneckupwards), 0)); //private Vector3 viewOffset; // 0x38

	}

	void SetFlying(bool value)
	{
		if (!Player)
			return;

		auto BaseMovement = mem.Read<uintptr_t>(Player + OFFSET_PLAYER_BASE_MOVEMENT); //BasePlayer -> movement.
		if (!BaseMovement)
			return;

		mem.Write<bool>(BaseMovement + OFFSET_PWM_FLYING, value); //PlayerWalkMovement (Extended from BaseMovement) -> flying;
	}

	void DoWaterWalk()
	{
		if (!Player)
			return;

		auto BaseMovement = mem.Read<uintptr_t>(Player + OFFSET_PLAYER_BASE_MOVEMENT); //BasePlayer -> movement.
		if (!BaseMovement)
			return;

		mem.Write<float>(BaseMovement + OFFSET_PWM_GROUND_ANGLE, 0.f); //PlayerWalkMovement (Extended from BaseMovement) -> groundAngle;
		mem.Write<float>(BaseMovement + OFFSET_PWM_GROUND_ANGLE_NEW, 0.f); //PlayerWalkMovement (Extended from BaseMovement) -> groundAngleNew;
		mem.Write<float>(BaseMovement + OFFSET_PWM_GRAVITY_MULTIPLIER, 0.f); //PlayerWalkMovement (Extended from BaseMovement) -> gravityMultiplier;
		mem.Write<bool>(BaseMovement + OFFSET_PWM_FLYING, 1); //PlayerWalkMovement (Extended from BaseMovement) -> flying;
	}

	void UndoWaterWalk()
	{
		if (!Player)
			return;

		auto BaseMovement = mem.Read<uintptr_t>(Player + OFFSET_PLAYER_BASE_MOVEMENT); //BasePlayer -> movement.
		if (!BaseMovement)
			return;

		mem.Write<float>(BaseMovement + OFFSET_PWM_GRAVITY_MULTIPLIER, 2.5f); //PlayerWalkMovement (Extended from BaseMovement) -> gravityMultiplier;
		mem.Write<bool>(BaseMovement + OFFSET_PWM_FLYING, 0); //PlayerWalkMovement (Extended from BaseMovement) -> flying;
	}

	ModelState GetModelState()
	{
		if (!Player)
			return ModelState{};

		ModelState model;
		model.state = mem.Read<uintptr_t>(Player + OFFSET_PLAYER_MODEL_STATE); //BasePlayer -> playerModel.
		return model;
	}

	PlayerModel GetPlayerModel()
	{
		if (!Player)
			return PlayerModel{};

		PlayerModel model;
		model.Model = mem.Read<uintptr_t>(Player + OFFSET_PLAYER_MODEL); //BasePlayer -> playerModel.
		return model;
	}

	PlayerInput GetPlayerInput()
	{
		if (!Player)
			return PlayerInput{};

		PlayerInput input;
		input.Input = mem.Read<uintptr_t>(Player + OFFSET_PLAYER_INPUT); //BasePlayer -> input.
		return input;
	}

	PlayerInventory GetPlayerInventory()
	{
		if (!Player)
			return PlayerInventory{};

		PlayerInventory pi;
		pi.Inventory = mem.Read<uintptr_t>((uintptr_t)Player + OFFSET_PLAYER_INVENTORY); //BasePlayer -> inventory.
		return pi;
	}

	PlayerEyes GetPlayerEyes()
	{
		if (!Player)
			return PlayerEyes();

		PlayerEyes pe;
		pe.eyes = mem.Read<uintptr_t>(Player + OFFSET_PLAYER_EYES); //BasePlayer -> eyes.
		return pe;
	}

	ItemClass GetHeldItem()
	{
		ItemClass item;
		item.Item = NULL;

		if (!(uintptr_t)Player || (uintptr_t)Player == NULL)
		{
			//std::cout << "GHI Player is NULL!" << std::endl;
			//Sleep(2500);
			return item;
		}

		auto active_item_id = GetActiveWeaponID();
		//std::cout << "Active Item ID: " << active_item_id << std::endl;
		if (!active_item_id || active_item_id == NULL)
		{
			//std::cout << "GHI active item id is NULL!" << std::endl;
			//Sleep(2500);
			return item;
		}

		int ActiveWeapon;

		for (int i = 0; i < 6; i++)
		{
			auto Inventory = GetPlayerInventory();
			//std::cout << "GetPlayerInventory!" << std::endl;
			auto Belt = Inventory.GetBelt();
			//std::cout << "GetBelt!" << std::endl;
			ItemClass weapon = Belt.GetItem(i);
			if (!weapon.Item || weapon.Item == NULL)
			{
				//std::cout << "Failed to GET ITEM!" << std::endl;
				return item;
			}
			//std::cout << "GetItem!" << std::endl;

			ActiveWeapon = weapon.GetUID();
			//std::cout << "GetUID!" << std::endl;
			//std::cout << "ActiveWeapon: " << ActiveWeapon << std::endl;
			if (active_item_id == ActiveWeapon)
			{
				return weapon;
			}
		}
		//std::cout << "GHI Found no item!" << std::endl;
		Sleep(2500);
		return item;
	}

	Vector3 GetVisualPosition()
	{
		auto Transform = mem.Read<uintptr_t>(ObjectClass + 0x8);
		auto VisualState = mem.Read<uintptr_t>(Transform + 0x38);
		return mem.Read<Vector3>(VisualState + 0x90);
	}

	Vector3 GetTransformPosition(uintptr_t transform)
	{
		if (!transform) return Vector3{ 0.f, 0.f, 0.f };

		struct Matrix34 { BYTE vec0[16]; BYTE vec1[16]; BYTE vec2[16]; };
		const __m128 mulVec0 = { -2.000, 2.000, -2.000, 0.000 };
		const __m128 mulVec1 = { 2.000, -2.000, -2.000, 0.000 };
		const __m128 mulVec2 = { -2.000, -2.000, 2.000, 0.000 };

		int Index = mem.Read<int>(transform + 0x40);// *(PINT)(transform + 0x40);
		uintptr_t pTransformData = mem.Read<uintptr_t>(transform + 0x38);
		uintptr_t transformData[2];
		mem.Read((pTransformData + 0x18), &transformData, 16);
		//mem.Read(&transformData, (PVOID)(pTransformData + 0x18), 16);
		//safe_memcpy(&transformData, (PVOID)(pTransformData + 0x18), 16);

		size_t sizeMatriciesBuf = 48 * Index + 48;
		size_t sizeIndicesBuf = 4 * Index + 4;

		PVOID pMatriciesBuf = malloc(sizeMatriciesBuf);
		PVOID pIndicesBuf = malloc(sizeIndicesBuf);

		if (pMatriciesBuf && pIndicesBuf)
		{
			// Read Matricies array into the buffer
			mem.Read(transformData[0], pMatriciesBuf, sizeMatriciesBuf);
			//impl::memory->read(transformData[0], pMatriciesBuf, sizeMatriciesBuf);
			// Read Indices array into the buffer
			mem.Read(transformData[1], pIndicesBuf, sizeIndicesBuf);

			__m128 result = *(__m128*)((ULONGLONG)pMatriciesBuf + 0x30 * Index);
			int transformIndex = *(int*)((ULONGLONG)pIndicesBuf + 0x4 * Index);

			while (transformIndex >= 0)
			{
				Matrix34 matrix34 = *(Matrix34*)((ULONGLONG)pMatriciesBuf + 0x30 * transformIndex);
				__m128 xxxx = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x00));
				__m128 yyyy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x55));
				__m128 zwxy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x8E));
				__m128 wzyw = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0xDB));
				__m128 zzzz = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0xAA));
				__m128 yxwy = _mm_castsi128_ps(_mm_shuffle_epi32(*(__m128i*)(&matrix34.vec1), 0x71));
				__m128 tmp7 = _mm_mul_ps(*(__m128*)(&matrix34.vec2), result);

				result = _mm_add_ps(
					_mm_add_ps(
						_mm_add_ps(
							_mm_mul_ps(
								_mm_sub_ps(
									_mm_mul_ps(_mm_mul_ps(xxxx, mulVec1), zwxy),
									_mm_mul_ps(_mm_mul_ps(yyyy, mulVec2), wzyw)),
								_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0xAA))),
							_mm_mul_ps(
								_mm_sub_ps(
									_mm_mul_ps(_mm_mul_ps(zzzz, mulVec2), wzyw),
									_mm_mul_ps(_mm_mul_ps(xxxx, mulVec0), yxwy)),
								_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x55)))),
						_mm_add_ps(
							_mm_mul_ps(
								_mm_sub_ps(
									_mm_mul_ps(_mm_mul_ps(yyyy, mulVec0), yxwy),
									_mm_mul_ps(_mm_mul_ps(zzzz, mulVec1), zwxy)),
								_mm_castsi128_ps(_mm_shuffle_epi32(_mm_castps_si128(tmp7), 0x00))),
							tmp7)), *(__m128*)(&matrix34.vec0));
				try {
					transformIndex = *(int*)((ULONGLONG)pIndicesBuf + 0x4 * transformIndex);
				}
				catch (...)
				{
					// Do nothing
				}
			}

			free(pMatriciesBuf);
			free(pIndicesBuf);
			//ZeroMemory(&pMatriciesBuf, sizeof(pMatriciesBuf));
			//ZeroMemory(&pIndicesBuf, sizeof(pIndicesBuf));
			//delete[] pMatriciesBuf;
			//delete[] pIndicesBuf;
			return Vector3(result.m128_f32[0], result.m128_f32[1], result.m128_f32[2]);
		}
	}

	Vector3 GetBonePosition(int Bone)
	{
		if (!Player)
			return Vector3{ 0,0,0 };

		uintptr_t player_model = mem.Read<uintptr_t>(Player + 0x128); //BaseEntity -> model.
		if (!player_model)
			return Vector3{ 0,0,0 };

		uintptr_t BoneTransforms = mem.Read<uintptr_t>(player_model + 0x48); //Model -> boneTransforms.
		if (!BoneTransforms)
			return Vector3{ 0,0,0 };

		uintptr_t entity_bone = mem.Read<uintptr_t>(BoneTransforms + (0x20 + (Bone * 0x8)));
		if (!entity_bone)
			return Vector3{ 0,0,0 };

		return GetTransformPosition(mem.Read<uintptr_t>(entity_bone + 0x10));
	}

	Vector3 GetVelocity()
	{
		uintptr_t player_model = mem.Read<uintptr_t>(Player + OFFSET_PLAYER_MODEL); //BasePlayer -> playerModel.
		return mem.Read<Vector3>(player_model + 0x214); //PlayerModel -> newVelocity.
	}

	long GetCurrentTeamID()
	{
		//return mem.Read<long>(Player + OFFSET_PLAYER_CURRENT_TEAM); //BasePlayer -> currentTeam.
		uintptr_t player_team = mem.Read<uintptr_t>(Player + OFFSET_PLAYER_CURRENT_TEAM);

		return mem.Read<long>(player_team + 0x18);
	}

	std::vector<long> GetTeamList()
	{
		std::vector<long> teamlist;
		uintptr_t player_team = mem.Read<uintptr_t>(Player + OFFSET_PLAYER_CURRENT_TEAM);

		uintptr_t player_team_member = mem.Read<uintptr_t>(player_team + 0x30);

		uintptr_t player_team_member_list = mem.Read<uintptr_t>(player_team_member + 0x10);

		int player_team_size = mem.Read<std::uint32_t>(player_team_member + 0x18);
		//std::cout << "Player Team Size: " << player_team_size << std::endl;
		for (int index = 0; index < player_team_size; index++)
		{

			uintptr_t player = mem.Read<uintptr_t>(player_team_member_list + 0x20 + (index * 0x8));

			long player_uid = mem.Read<long>(player + 0x20);
			//std::cout << "Player UID: " << player_uid << std::endl;

			teamlist.push_back(player_uid);
		}
		return teamlist;
	}

	void SetClientTickInterval(float tick)
	{
		if (!Player)
			return;

		mem.Write<float>(Player + OFFSET_PLAYER_CLIENTTICKINTERVAL, tick);
	}

	void DoFakeLag()
	{
		SetClientTickInterval(0.8f);
		SetClientTickInterval(0.10f);
	}

	void DoNoRecoil()
	{
		GetPlayerInput().SetRecoilAngle(Vector2(0, 0));
	}

	void DoSpinbot()
	{
		auto newangle = Vector3(100, rand() % 999 + -999, rand() % 999 + -999);
		GetPlayerInput().GetState().GetCurrent().SetAimAngles(newangle);
	}

	bool is_key_down(BUTTON key) {
		auto current = GetPlayerInput().GetState().GetCurrent().InputMessage;
		if (!current)
			return false;

		int buttons = mem.Read<int>(current + 0x14);
		//std::cout << "Buttons: " << buttons << std::endl;
		return ((buttons & (int)key) == (int)key);
	}

	int get_key_down()
	{
		auto current = GetPlayerInput().GetState().GetCurrent().InputMessage;
		if (!current)
			return false;

		return mem.Read<int>(current + 0x14);
	}
};

class list {
public:
	template <typename T>
	T get_value() {
		auto list = *reinterpret_cast<uintptr_t*>(this + 0x10);
		if (!list)
			return 0;

		auto value = *reinterpret_cast<T*>(list + 0x28);
		if (!value)
			return 0;
		return value;
	}

	int get_size() {
		auto value = get_value<uintptr_t>();
		if (!value)
			return 0;

		auto size = mem.Read<int>(value + 0x10);
		if (!size)
			return 0;
	}

	template <typename T>
	T get_buffer() {
		auto value = get_value<uintptr_t>();

		return *reinterpret_cast<uintptr_t*>(value + 0x18);
	}
};

namespace test
{

	namespace il2cpp {
		typedef int(__stdcall* GPC)(DWORD64);
		typedef DWORD64(__stdcall* AGI)(DWORD64);
		typedef DWORD64(__stdcall* Dm)();
		typedef DWORD64(__stdcall* CFN)(DWORD64, const char*, const char*);
		typedef DWORD64(__stdcall* Call)(DWORD64, DWORD64);
		typedef DWORD64(__stdcall* Ptr)(DWORD64, DWORD64*);
		typedef DWORD64* (__cdecl* DGA)(void* domain, DWORD64* size);

		Ptr class_get_methods = 0;
		Ptr class_get_fields = 0;
		Dm domain_get = 0;
		DGA domain_get_assemblies = 0;
		AGI assembly_get_image = 0;
		CFN class_from_name = 0;
		AGI field_get_offset = 0;
		Ptr field_static_get_value = 0;
		GPC method_get_param_count = 0;
		Call array_new = 0;

		static void InitIL() {
			method_get_param_count = (GPC)(GetExport(GetModBase(), "il2cpp_method_get_param_count"));
			field_static_get_value = (Ptr)(GetExport(GetModBase(), "il2cpp_field_static_get_value"));
			field_get_offset = (AGI)(GetExport(GetModBase(), "il2cpp_field_get_offset"));
			class_from_name = (CFN)(GetExport(GetModBase(), "il2cpp_class_from_name"));
			assembly_get_image = (AGI)(GetExport(GetModBase(), "il2cpp_assembly_get_image"));
			domain_get_assemblies = (DGA)(GetExport(GetModBase(), "il2cpp_domain_get_assemblies"));
			domain_get = (Dm)(GetExport(GetModBase(), "il2cpp_domain_get"));
			class_get_fields = (Ptr)(GetExport(GetModBase(), "il2cpp_class_get_fields"));
			class_get_methods = (Ptr)(GetExport(GetModBase(), "il2cpp_class_get_methods"));
			array_new = (Call)(GetExport(GetModBase(), "il2cpp_array_new"));

		}

	}
}

DWORD64 il2cpp_array(DWORD64 klass, DWORD64 size) {
	DWORD64 arr = test::il2cpp::array_new(klass, size);
	return arr;
}

void il2cpp_hook(uint64_t method, PVOID hook, PDWORD64 ret) {
	uint64_t orig = mem.Read(method);
	if (orig != method)
		mem.Write<uint64_t>((uint64_t)ret, orig); //we dont override original pointer
	mem.Write<PVOID>(method, hook);
}
DWORD64 il2cpp_field(DWORD64 klass, const char* nm, bool get_offset = true) {
	DWORD64 iter = 0;
	DWORD64 f;

	while (f = test::il2cpp::class_get_fields(klass, &iter)) {

		char* name = (char*)*reinterpret_cast<uintptr_t*>(f);
		if (!name) break;
		printf("checking field 0x%llX: %s\n", f, name);
		if (!strcmp(name, (char*)nm)) {
			continue;
		}
		if (!get_offset) {
			return f;
		}
		std::cout << "Getting field offset!" << std::endl;
		DWORD64 offset = test::il2cpp::field_get_offset(f);
		std::cout << "Returning field offset!" << std::endl;
		return NULL;
	}
	return 0;
}
DWORD64 il2cpp_value(DWORD64 klass, const char* name) {
	DWORD64 fl = il2cpp::field(klass, name, false);
	printf("fl: 0x%x\n", fl);
	uintptr_t tt = 0;

	DWORD64 _t = il2cpp::methods::field_static_get_value(fl, &tt);
	printf("tt: 0x%x\n", tt);
	printf("Static Value: 0x%x\n", _t);
	return tt;
}
DWORD64 il2cpp_method(DWORD64 kl, const char* name, int arg = -1, const char* aname = "", int totalarg = -1) {
	DWORD64 iter = 0;
	DWORD64 f;

	while (f = test::il2cpp::class_get_methods(kl, &iter)) {

		auto test = mem.Read(f + 0x10);
		char* st = reinterpret_cast<char*>(test);

		if (strcmp(st, (char*)name)) {
			if (arg >= 0 && aname) {
				auto args = mem.Read(f + 0x28);
				int method_count = test::il2cpp::method_get_param_count(f);
				if (arg > method_count || (totalarg >= 0 && method_count != totalarg)) continue;

				char* argname;
				if (method_count > 0) {
					argname = (char*)mem.Read(args + (arg - 1) * 0x18);
				}
				else argname = (char*)"-";

				if (!argname || !strcmp(argname, (char*)aname)) continue;
			}

			return f;
		}
	}
	return 0;
}
DWORD64 InitClass(const char* name, const char* ns = "") {
	DWORD64 domain = test::il2cpp::domain_get();

	DWORD64 nrofassemblies = 0;
	DWORD64* assemblies;
	assemblies = test::il2cpp::domain_get_assemblies((void*)domain, &nrofassemblies);

	for (int i = 0; i < nrofassemblies; i++) {
		DWORD64 img = test::il2cpp::assembly_get_image(assemblies[i]);

		DWORD64 kl = test::il2cpp::class_from_name(img, ns, name);
		if (!kl) continue;

		return kl;
	}
	return 0;
}

uint64_t GetValue(const char* klass, const char* name, bool get_offset = true)
{
	auto kls = InitClass(klass);

	if (get_offset)
	{
		//auto field_offset = il2cpp_field(kls, )
	}

	return NULL;
}
#define CLASS InitClass
#define METHOD(...) mem.Read<uint64_t>(il2cpp_method(__VA_ARGS__))
#define OFFSET il2cpp_field

void(*orig_baseplayer_client_input)(uintptr_t, uintptr_t) = nullptr;
void(*orig_baseplayer_block_sprint)(uintptr_t, float) = nullptr;
bool(*orig_baseprojectile_can_attack)(uintptr_t) = nullptr;
void(*orig_playerwalkmovement_client_input)(uintptr_t, uintptr_t, uintptr_t) = nullptr;
Vector3(*orig_aimconeutil_getmodifiedaimconedirection)(float, Vector3, bool) = nullptr;
bool(*get_key)(KeyCode) = nullptr;
bool(*line_of_sight)(Vector3, Vector3, Layers layer, float padding) = nullptr;
void (*set_sprinting)(uintptr_t, bool value) = nullptr;
void (*set_flying)(uintptr_t, bool) = nullptr;
void (*set_onLadder)(uintptr_t, bool) = nullptr;

bool is_visible(Vector3 source, Vector3 destination) {
	auto layer = (int)Layers::ProjectileLineOfSightCheck | (int)Layers::Terrain;

	return line_of_sight(source, destination, Layers(layer), 0);
}