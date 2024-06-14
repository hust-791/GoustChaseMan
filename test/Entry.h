#pragma once

#define TopWall 0.
#define BottomWall 41.
#define LeftWall 0.
#define RightWall 81.

#define StatusBarTop (BottomWall + 1)
#define StatusBarBottom (BottomWall + 10)
#define StatusBarLeft LeftWall
#define StatusBarRight RightWall

#define XRange RightWall
#define YRange StatusBarBottom

#define DefaultXpos (RightWall + 2) 
#define DefaultYpos (BottomWall - 2)

#define DefaultPos Pos(DefaultXpos,DefaultYpos)
struct Pos
{
	double x;
	double y;
	Pos(double a, double b) :x(a), y(b) {};
	Pos() { x = DefaultXpos; y = DefaultYpos; };


	bool operator==(const Pos& ps)
	{
		if (x+1 <= ps.x || ps.x +1 <= x)
			return false;
		if (y+1 <= ps.y || ps.y +1 <= y)
			return false;

		return true;
	}

	Pos operator-(const Pos& ps)
	{
		Pos pos(this->x - ps.x, this->y - ps.y);
		return pos;
	}

	Pos operator+(const Pos& ps)
	{
		Pos pos(this->x + ps.x, this->y + ps.y);
		return pos;
	}
};

void goTo(Pos ps);

void gotoxy(double x, double y);

void clearConsoleArea(Pos leftTop, Pos rigthBottom);


enum class EntryType
{
	en_None = 0,
	en_Self,
	en_Ghost,
	en_Wall,
	en_Medicine,
	en_Door,
	en_Key
};

enum class EntryTowards
{
	en_None = 0,
	en_Forward,
	en_Back,
	en_Left,
	en_Right
};

class Entry
{
public:
	Entry(Pos ps);
	virtual ~Entry() {};
	virtual void _Clash(const Entry& ent) {};
	virtual void UpDataUI() {};

	void move(Pos ps);
	virtual void forward();
	virtual void back();
	virtual void left();
	virtual	void right();

	virtual	bool isClash(const Entry& ent);

	void recover(size_t val);
	void damage(size_t val);

	void setAttack(int att);
	int getAttack() { return m_attack; }
	int getAttack() const { return m_attack; }

	void setSpeed(double speed);
	int getSpeed() { return m_speed; }
	int getSpeed() const { return m_speed; }

	void setHp(int hp) { m_hp = hp; }
	int getHp() { return m_hp; }
	int getHp() const { return m_hp; }

public:
	Pos m_pos;
	Pos m_Lastpos;
	EntryType m_type;
	EntryTowards m_towards;
private:
	int m_hp;
	int m_attack;
	double m_speed;
};


class Self :public Entry
{
public:
	static Self* getInstance();
	virtual void UpDataUI() override;

	virtual void _Clash(const Entry& ent) override;

	Self(const Self&) = delete;
	Self& operator=(const Self&) = delete;
private:
	Self(Pos ps);

	virtual ~Self();

};


class Wall :public Entry
{
public:
	Wall(Pos ps);
	virtual ~Wall();
	virtual void UpDataUI() override;
};

enum GoustStatus
{
	en_Sleep = 0,
	en_Chase,
};

class Goust :public Entry
{
public:
	Goust(Pos ps);
	virtual ~Goust();
	virtual void UpDataUI() override;
	virtual void _Clash(const Entry& ent) override;

	void Chase();
	void setStatus(GoustStatus status) { m_status = status; }
	GoustStatus getStatus() { return m_status; }
private:
	GoustStatus m_status;
};