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
		}
	}
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
	//for (int key = 'A'; key <= 'Z'; ++key)
	//{
	//	if (GetAsyncKeyState(key) & 0x8000)
	//	{
	//		std::string keyStr(1, key);
	//		Excommond(keyStr);
	//		break;
	//	}
	//}
	//if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
	//{
	//	Excommond("esc");
	//}
	if (GetAsyncKeyState('W') & 0x8000)
	{
		Excommond("W");
	}
	else if (GetAsyncKeyState('S') & 0x8000)
	{
		Excommond("S");
	}
	else if (GetAsyncKeyState('A') & 0x8000)
	{
		Excommond("A");
	}
	else if (GetAsyncKeyState('D') & 0x8000)
	{
		Excommond("D");
	}
	else if (GetAsyncKeyState('J') & 0x8000)
	{
		Excommond("J");
	}
	else if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
	{
		Excommond("esc");
	}
}

void preStartGame()
{
	goTo(Pos((RightWall / 2) - 5, TopWall));
	std::cout << "START :按V开始游戏";
	goTo(Pos((RightWall / 2) - 4, TopWall + 1));
	std::cout << "WASD :上左下右";

	while (game_is_running)
	{
		if (GetAsyncKeyState('V') & 0x8000)
		{
			system("cls");
			Excommond("V");
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

	bool b = true;

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
RegisterFunc("V", drawWall);
RegisterFunc("J", attack);
RegisterFunc("esc", esc);
AutoRegisterFunctionEnd

