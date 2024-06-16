#include "stdafx.h"
#include "Entry.h"
#include "UIControl.h"

#define  BACKGROUND_WHILE BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE
#define  FOREGROUND_WHILE FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE
#define  BACKGROUND_BLACK 0x07


 UIControlManager& UIControlManager::getUICtrl()
{
	static UIControlManager _instance;
	return _instance;
}

 void UIControlManager::addEntry(Entry* ent)
 {
	 m_managerEntrys.push_back(ent);
 }

 void UIControlManager::deleteEntry(Entry* ent)
 {
	 auto it = std::find(m_managerEntrys.begin(), m_managerEntrys.end(), ent);
	 if (it != m_managerEntrys.end())
		 m_managerEntrys.erase(it);
 }

 UIControlManager::UIControlManager()
 {
	 m_hOutput = CreateConsoleScreenBuffer(
		 GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	 m_hOutBuf = CreateConsoleScreenBuffer(
		 GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	 CONSOLE_CURSOR_INFO cci;
	 cci.bVisible = 0;
	 cci.dwSize = 1;
	 SetConsoleCursorInfo(m_hOutput, &cci);
	 SetConsoleCursorInfo(m_hOutBuf, &cci);
	 
	 m_screenBuffer = new CHAR_INFO* [YRange];
	 for (int i = 0; i < YRange; ++i)
	 {
		 m_screenBuffer[i] = new CHAR_INFO[XRange];
		 for (int j = 0; j < XRange; ++j)
		 {
			 m_screenBuffer[i][j].Char.UnicodeChar = L' ';
			 m_screenBuffer[i][j].Attributes = BACKGROUND_BLACK;
		 }
	 }
 }

 UIControlManager::~UIControlManager()
 {
	 for (int i = 0; i < YRange; ++i)
	 {
		 delete[] m_screenBuffer[i];
	 }
	 delete[] m_screenBuffer;
 }

void UIControlManager::UpDataGameUI()
{
	__clearScreenBuffer();
	__drawEntrys();
	__drawStatusBar();

	static bool exchangeBuffer = true;
	if (exchangeBuffer)
		__renderBuffer(m_hOutput);
	else
		__renderBuffer(m_hOutBuf);
	exchangeBuffer = !exchangeBuffer;
}

void UIControlManager::__clearScreenBuffer()
{
	for (int i = 0; i < YRange; ++i) 
	{
		for (int j = 0; j < XRange; ++j)
		{
			m_screenBuffer[i][j].Char.UnicodeChar = L' ';
			m_screenBuffer[i][j].Attributes = BACKGROUND_BLACK;
		}
	}
}

void UIControlManager::__drawEntrys()
{
	for (auto entry : m_managerEntrys)
	{
		if (entry)
		{
			int yPos = (int)entry->m_pos.y;
			int xPos = (int)entry->m_pos.x;

			m_screenBuffer[yPos][xPos].Char.UnicodeChar = entry->m_name;
			m_screenBuffer[yPos][xPos].Attributes = FOREGROUND_WHILE;
		}
	}
}

void UIControlManager::__drawStatusBar()
{
	int hp = Player::getInstance()->getHp();
	std::wstring HpText = L"HP: " + std::to_wstring(hp);
	std::wstring HpBar = L"";
	for (int i = 0; i < hp; ++i)
		HpBar += L" ";

	std::wstring posInfo = L"Player Position: (" +
		std::to_wstring(Player::getInstance()->m_pos.x) + L", " +
		std::to_wstring(Player::getInstance()->m_pos.y) + L")";

	for (size_t i = 0; i < HpBar.size() && i < XRange; ++i)
	{
		m_screenBuffer[StatusBarTop + 1][i].Char.UnicodeChar = HpBar[i];
		m_screenBuffer[StatusBarTop + 1][i].Attributes = (hp <= 4 ? BACKGROUND_RED : BACKGROUND_WHILE);
	}

	for (size_t i = 0; i < HpText.size() && i < XRange; ++i) 
	{
		m_screenBuffer[StatusBarTop + 2][i].Char.UnicodeChar = HpText[i];
		m_screenBuffer[StatusBarTop + 2][i].Attributes = FOREGROUND_WHILE;
	}

	for (size_t i = 0; i < posInfo.size() && i < XRange; ++i)
	{
		m_screenBuffer[StatusBarTop + 3][i].Char.UnicodeChar = posInfo[i];
		m_screenBuffer[StatusBarTop + 3][i].Attributes = FOREGROUND_WHILE;
	}
}

void UIControlManager::__renderBuffer(HANDLE handle)
{
	SMALL_RECT writeArea = { 0, 0, XRange - 1, YRange - 1 };

	for (int i = 0; i < YRange; ++i) {
		writeArea.Top = i;
		writeArea.Bottom = i;
		WriteConsoleOutput(handle, m_screenBuffer[i], { SHORT(XRange), 1 }, { 0, 0 }, &writeArea);
	}

	SetConsoleActiveScreenBuffer(handle);
}