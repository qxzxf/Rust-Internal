#pragma once
#include <Windows.h>
#define MAX_ITEMS 250

#define T_FOLDER 1
#define T_OPTION 2
#define T_INT 3
#define T_BOOL 4
#define T_FLOAT 5
#define T_SPACER 6

#define LineH 15

struct Options {
    LPCSTR Name;
    int* Function;
    BYTE Type;
    int lower_limit = 0;
    int upper_limit = 2;
    int increase_by = 1;
};


struct MenuS {
    LPCSTR Title;
    int x;
    int y;
    int w;
};



class Menu
{
public:
    Menu(void);
    void Init_Menu(LPCSTR Title, int x, int y);

    void Draw_Menu();
    void Navigation();
    void AddFolder(LPCSTR Name, int* Pointer, int limit);
    //void AddOption(std::string Name, int* Pointer, int* Folder, int type);
    void AddOption(LPCSTR Name, int* Pointer, int* Folder, int type, int lower_limit, int upper_limit, int increase_by);

    bool IsReady();

    DWORD Color_Font;
    DWORD Color_On;
    DWORD Color_Off;
    DWORD Color_Folder;
    DWORD Color_Current;

public:

    bool Is_Ready, Visible;
    int Items, Cur_Pos;


    Options sOptions[MAX_ITEMS];
    MenuS sMenu;

};