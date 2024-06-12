#pragma once

#define TopWall 0
#define BottomWall 41
#define LeftWall 0
#define RightWall 81

#define XRange (RightWall -1)
#define YRange (BottomWall -1)

#define DefaultXpos (RightWall + 2) 
#define DefaultYpos (BottomWall - 2)

#define DefaultPos Pos(DefaultXpos,DefaultYpos)
struct Pos
{
	int x;
	int y;
	Pos(int a, int b) :x(a), y(b) {};
	Pos() { x = DefaultXpos; y = DefaultYpos; };
	bool operator==(const Pos& ps)
	{
		return(x == ps.x && y == ps.y);
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

void gotoxy(int x, int y);


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

class Entry
{
public:
	Entry(Pos ps);
	virtual ~Entry() {};
	virtual void _Clash(const Entry& ent) {};
	void UpDataUI();

	void move(Pos ps);
	virtual void forward();
	virtual void back();
	virtual void left();
	virtual	void right();

	void recover(size_t val);
	void damage(size_t val);

	int getAttack();
	int getAttack() const;
	void setAttack(int att);
					
private:

	virtual void __Descreption();

public:
	Pos m_pos;
	Pos m_Lastpos;
	EntryType m_type;

private:
	int m_hp;
	int m_attack;
	double m_speed;
};


class Self :public Entry
{
public:
	static Self* getInstance();

	virtual void _Clash(const Entry& ent) override;

	Self(const Self&) = delete;
	Self& operator=(const Self&) = delete;
private:
	Self(Pos ps);

	virtual ~Self();

	virtual void __Descreption() override;
};


class Wall :public Entry
{
public:
	Wall(Pos ps);
	virtual ~Wall();
private:
	virtual void __Descreption() override;
};

class Goust :public Entry
{
public:
	Goust(Pos ps);
	virtual ~Goust();
	virtual void _Clash(const Entry& ent) override;

	void Chase();
private:
	virtual void __Descreption() override;

};