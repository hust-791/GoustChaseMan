#include "stdafx.h"
#include "Entry.h"
#include "UIControl.h"
#include "ClashCheck.h"
#include "GoustManager.h"

using namespace std;

void goTo(Pos ps)
{
	COORD coord;
	coord.X = (2 * ps.x);
	coord.Y = ps.y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void gotoxy(double x, double y) {
	COORD coord;
	coord.X = (2 * x);
	coord.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

Entry::Entry(Pos ps) :m_pos(ps)
{
	m_Lastpos = DefaultPos;
	m_type = EntryType::en_None;
	m_towards = EntryTowards::en_None;
	m_name = L' ';
	m_hp = 10;
	m_attack = 0;
	m_speed = 1.0;
};

void Entry::move(Pos ps)
{
	m_Lastpos = m_pos;
	m_pos = ps;
}

void Entry::forward()
{
	if (m_pos.y > TopWall + 1)
	{
		m_Lastpos = m_pos;
		m_pos.y -= m_speed / FRAMES_PER_SECOND;
		m_towards = EntryTowards::en_Forward;
	}
}

void Entry::back()
{
	if (m_pos.y < BottomWall - 1)
	{
		m_Lastpos = m_pos;
		m_pos.y+= m_speed / FRAMES_PER_SECOND;
		m_towards = EntryTowards::en_Back;
	}
}

void Entry::left()
{
	if (m_pos.x > LeftWall + 2)
	{
		m_Lastpos = m_pos;
		m_pos.x -= m_speed / FRAMES_PER_SECOND;
		m_towards = EntryTowards::en_Left;
	}
}

void Entry::right()
{
	if (m_pos.x < RightWall_UI - 2)
	{
		m_Lastpos = m_pos;
		m_pos.x += m_speed / FRAMES_PER_SECOND;
		m_towards = EntryTowards::en_Right;
	}
}

bool Entry::isClash(const Entry& ent)
{
	return this->m_pos == ent.m_pos;
}

void Entry::recover(size_t val)
{
	m_hp += (int)val;
}

void Entry::damage(size_t val)
{
	m_hp -= (int)val;
}

void Entry::setAttack(int att)
{
	m_attack = att;
}

void Entry::setSpeed(double speed)
{
	m_speed = speed;
}


////////////////Self////////////////

Player* Player::getInstance()
{
	static Player _instance(Pos(DefaultXpos, DefaultYpos));
	return &_instance;
}

void Player::_Clash(const Entry& ent)
{
	switch (ent.m_type)
	{
	case EntryType::en_None:
	case EntryType::en_Self:
		break;

	case EntryType::en_Ghost:
	{
		damage(ent.getAttack());
	}break;
	case EntryType::en_Wall:
	{
		m_pos = m_Lastpos;
		m_Lastpos = DefaultPos;
	}break;
	case EntryType::en_Medicine:
	{

	}break;
	case EntryType::en_Door:
	{

	}break;
	case EntryType::en_Key:
	{

	}break;
	default:
		break;
	}
}

Player::Player(Pos ps) :Entry(ps)
{
	m_Lastpos = DefaultPos;
	m_type = EntryType::en_Self;
	m_name = L'我';
	setAttack(1);
	setSpeed(10);

	UIControlManager::getUICtrl().addEntry(this);
	ClashCheckManager::getInstance().addMovingEntry(this);
}

Player::~Player()
{
	UIControlManager::getUICtrl().deleteEntry(this);
	ClashCheckManager::getInstance().deleteMovingEntry(this);
}



////////////////Wall////////////////

Wall::Wall(Pos ps) :Entry(ps)
{
	m_type = EntryType::en_Wall;
	m_name = L'墙';
	UIControlManager::getUICtrl().addEntry(this);
	ClashCheckManager::getInstance().addStillEntry(this);
}

Wall::~Wall()
{
	UIControlManager::getUICtrl().deleteEntry(this);
	ClashCheckManager::getInstance().deleteStillEntry(this);
}


////////////////Goust////////////////


Goust::Goust(Pos ps) :Entry(ps)
{
	m_Lastpos = DefaultPos;
	m_type = EntryType::en_Ghost;
	m_status = GoustStatus::en_Sleep;
	m_name = L'鬼';
	setAttack(2);
	setSpeed(5);

	UIControlManager::getUICtrl().addEntry(this);
	ClashCheckManager::getInstance().addMovingEntry(this);
	GoustManager::getInstance().addGoust(this);
}

Goust::~Goust()
{
	UIControlManager::getUICtrl().deleteEntry(this);
	ClashCheckManager::getInstance().deleteMovingEntry(this);
	GoustManager::getInstance().deleteGoust(this);
}

void Goust::_Clash(const Entry& ent)
{
	switch (ent.m_type)
	{
	case EntryType::en_None:
		break;
	case EntryType::en_Self:
	{
		delete this;
	}break;
	case EntryType::en_Ghost:
	case EntryType::en_Wall:
	{
		m_pos = m_Lastpos;
		m_Lastpos = DefaultPos;
	}break;
	default:
		break;
	}
}

void Goust::Chase()
{
	Pos vec = Player::getInstance()->m_pos - this->m_pos;

	bool b = (rand() % 6 + 1) > 3;

	if (vec.x == 0)
	{
		if (vec.y > 0)
			this->back();
		else
			this->forward();
	}
	else if(vec.y == 0)
	{
		if (vec.x > 0)
			this->right();
		else
			this->left();
	}
	else
	{
		if (vec.x > 0)
		{
			if (vec.y > 0)
			{
				if (b)
					this->back();
				else
					this->right();
			}
			else
			{
				if (b)
					this->forward();
				else
					this->right();
			}
		}
		else
		{
			if (vec.y > 0)
			{
				if (b)
					this->back();
				else
					this->left();
			}
			else
			{
				if (b)
					this->forward();
				else
					this->left();
			}
		}
	}
}



Buff::Buff(Pos ps):Entry(ps)
{
	m_Lastpos = DefaultPos;
	m_type = EntryType::en_Medicine;
	m_name = L'药';
	UIControlManager::getUICtrl().addEntry(this);
	ClashCheckManager::getInstance().addStillEntry(this);
}

Buff::~Buff()
{
	UIControlManager::getUICtrl().deleteEntry(this);
	ClashCheckManager::getInstance().deleteStillEntry(this);
}
