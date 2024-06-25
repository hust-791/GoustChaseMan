#include "stdafx.h"
#include "Engine.h"
#include "Entry.h"
#include "UIControl.h"
#include "ClashCheck.h"
#include "GoustManager.h"

using namespace std;

const int FRAMES_PER_SECOND = 20;
const int SKIP_TICKS = 1000 / FRAMES_PER_SECOND;
DWORD game_start_time = GetTickCount64();
DWORD next_game_tick = GetTickCount64();
int sleep_time = 0;

bool game_is_running = true;

void onInitGame()
{
	srand((unsigned)time(NULL));
	HWND consoleWindow = GetConsoleWindow(); // 获取控制台窗口的句柄
	SetWindowLong(consoleWindow, GWL_STYLE, GetWindowLong(consoleWindow, GWL_STYLE) & ~WS_OVERLAPPEDWINDOW); // 去掉窗口边框
	ShowWindow(consoleWindow, SW_SHOWMAXIMIZED); // 最大化窗口
}

void static forward()
{
	Player::getInstance()->forward();
}
void static back()
{
	Player::getInstance()->back();
}
void static left()
{
	Player::getInstance()->left();
}
void static right()
{
	Player::getInstance()->right();
}
void static attack()
{
	goTo(Player::getInstance()->m_pos);
	string a = "\033[1;35;47m";
	string b = "              ";
	string c = "\033[0m";
	cout << a + b + c;
}
void static esc()
{
	exit(0);
}

void onKeyTransition()
{
	for (int key = 'A'; key <= 'Z'; ++key)
	{
		if (GetAsyncKeyState(key) & 0x8000)
		{
			std::string keyStr(1, key);
			Excommond(keyStr);
		}
	}
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
	{
		Excommond("esc");
	}
}

void preStartGame()
{
	goTo(Pos((RightWall / 4) - 5, TopWall));
	std::cout << "START :按V开始游戏";
	goTo(Pos((RightWall / 4) - 4, TopWall + 1));
	std::cout << "WASD :上左下右";

	while (game_is_running)
	{
		if (GetAsyncKeyState('V') & 0x8000)
		{
			system("cls");
			break;
		}
		else if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
		{
			Excommond("esc");
		}
	}
}

int main()
{
	onInitGame();

	preStartGame();

	Player::getInstance();

	for (int i = 0; i < 6; ++i)
	{
		new Wall(Pos(5, i + 3));
	}

	Goust* a6 = new Goust(Pos(10, 10));
	Goust* a7 = new Goust(Pos(13, 11));
	Goust* a8 = new Goust(Pos(15, 12));

	bool b = false;

	while (game_is_running)
	{
		onKeyTransition();

		ClashCheckManager::getInstance().clashCheck();
		UIControlManager::getUICtrl().upDataGameUI();

		next_game_tick += SKIP_TICKS;
		sleep_time = next_game_tick - GetTickCount64();
		if (sleep_time >= 0) 
		{
			Sleep(sleep_time);
		}
		if (b && GetTickCount64() - game_start_time > 10000)
		{
			GoustManager::getInstance().startAllChase();
			b = false;
		}
	}
}

AutoDoRegisterFunctionBegin
RegisterFunc("W", forward);
RegisterFunc("S", back);
RegisterFunc("A", left);
RegisterFunc("D", right);
RegisterFunc("J", attack);
RegisterFunc("esc", esc);
AutoRegisterFunctionEnd

