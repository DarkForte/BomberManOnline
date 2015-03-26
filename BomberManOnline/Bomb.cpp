#include "stdafx.h"
#include "Bomb.h"


CBomb::CBomb()
{
}


CBomb::~CBomb()
{
}

CBomb::CBomb(int _owner, int x, int y, double _r, int _p)
{
	owner = _owner;
	pos.SetPoint(x,y);
	remain_time = _r;
	power = _p;
}

int CBomb::GetX()
{
	return pos.x;
}

int CBomb::GetY()
{
	return pos.y;
}

CPoint CBomb::GetPos()
{
	return pos;
}

int CBomb::GetPower()
{
	return power;
}

double CBomb::GetRemainTime()
{
	return remain_time;
}

void CBomb::DropTime(float game_time)
{
	remain_time -= game_time;
}

