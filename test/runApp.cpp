#include "stdafx.h"
#include "Engine.h"
#include "Entry.h"
#include "UIControl.h"
#include "ClashCheck.h"

using namespace std;

const int FRAMES_PER_SECOND = 20;
const int SKIP_TICKS = 1000 / FRAMES_PER_SECOND;
DWORD next_game_tick = GetTickCount64();

int sleep_time = 0;
bool game_is_running = true;

void onKeyTransition()
{
	if (GetAsyncKeyState('W') & 0x8000)
	{
		Excommond("w");
	}
	else if (GetAsyncKeyState('S') & 0x8000)
	{
		Excommond("s");
	}
	else if (GetAsyncKeyState('A') & 0x8000)
	{
		Excommond("a");
	}
	else if (GetAsyncKeyState('D') & 0x8000)
	{
		Excommond("d");
	}
	else if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
	{
		Excommond("esc");
	}
	else if (GetAsyncKeyState('V') & 0x8000)
	{
		Excommond("v");
	}
}


int main()
{
	srand((unsigned)time(NULL));
	HWND console = GetConsoleWindow();
	if (console == NULL) 
		return 1;

	HANDLE consoleOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (consoleOut == INVALID_HANDLE_VALUE) 
		return 1;

	CONSOLE_CURSOR_INFO cursor_info = { 1, 0 };
	SetConsoleCursorInfo(consoleOut,&cursor_info);

	HWND hwnd = GetForegroundWindow();
	int cx = GetSystemMetrics(SM_CXSCREEN);            /* 屏幕宽度 像素 */
	int cy = GetSystemMetrics(SM_CYSCREEN);            /* 屏幕高度 像素 */
	LONG l_WinStyle = GetWindowLong(hwnd, GWL_STYLE);   /* 获取窗口信息 */
	/* 设置窗口信息 最大化 取消标题栏及边框 */
	SetWindowLong(hwnd, GWL_STYLE, (l_WinStyle | WS_POPUP | WS_MAXIMIZE) & ~WS_CAPTION & ~WS_THICKFRAME & ~WS_BORDER);
	SetWindowPos(hwnd, HWND_TOP, 0, 0, cx, cy, 0);
	SetWindowLong(console, GWL_STYLE, GetWindowLong(console, GWL_STYLE) & ~(WS_VSCROLL)); // 禁止垂直滚动条

	COORD newSize = { RightWall, BottomWall }; // 控制台窗口的新缓冲区大小
	SetConsoleScreenBufferSize(consoleOut, newSize); // 设置控制台屏幕缓冲区大小

	std::cout << "START:按V开始游戏"<<endl;
	std::cout << "WASD :上左下右" << endl;

	Self::getInstance();

	Wall* a = new Wall(Pos(3, 3));
	Wall* a1 = new Wall(Pos(3, 4));
	Wall* a2 = new Wall(Pos(3, 5));
	Goust* a3 = new Goust(Pos(33, 36));
	Goust* a4 = new Goust(Pos(35, 37));
	Goust* a5 = new Goust(Pos(16, 12));

	while (game_is_running)
	{
		onKeyTransition();

		ClashCheckManager::getInstance().clashCheck();
		UIControlManager::getUICtrl().UpDataGameUI();

		next_game_tick += SKIP_TICKS;
		sleep_time = next_game_tick - GetTickCount64();
		if (sleep_time >= 0) 
		{
			Sleep(sleep_time);
		}
	}
}

void drawWall()
{
	for (int i = 0; i < BottomWall; ++i)
	{
		for (int j = 0; j < RightWall; ++j)
		{
			if (i == 0 || i == BottomWall - 1)
			{
				gotoxy(j, i);
				cout << "墙";
			}
			else if (j == 0 || j == RightWall - 1)
			{
				gotoxy(j, i);
				cout << "墙";
			}
			else
			{
				gotoxy(j, i);
				cout << "  ";
			}
		}
	}
}
void static forward()
{
	Self::getInstance()->forward();
}
void static back()
{
	Self::getInstance()->back();
}
void static left()
{
	Self::getInstance()->left();
}
void static right()
{
	Self::getInstance()->right();
}
void static esc()
{
	exit(0);
}

AutoDoRegisterFunctionBegin
RegisterFunc("w", forward);
RegisterFunc("s", back);
RegisterFunc("a", left);
RegisterFunc("d", right);
RegisterFunc("v", drawWall);
RegisterFunc("esc", esc);
AutoRegisterFunctionEnd
