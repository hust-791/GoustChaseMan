#pragma once

//UIøÿ÷∆¿‡
class UIControlManager
{
public:
	static UIControlManager& getUICtrl();

	void UpDataGameUI();
	void addEntry(Entry* ent);
	void deleteEntry(Entry* ent);

	UIControlManager(const UIControlManager&) = delete;
	UIControlManager& operator=(const UIControlManager&) = delete;

private:
	UIControlManager();
	~UIControlManager();
	void __clearScreenBuffer();
	void __drawEntrys();
	void __drawStatusBar();
	void __renderBuffer(HANDLE handle);

	HANDLE m_hOutput, m_hOutBuf;
	DWORD m_bytes = 0;
	CHAR_INFO** m_screenBuffer;
	std::vector<Entry*> m_managerEntrys;
};
