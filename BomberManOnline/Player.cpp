#include "stdafx.h"
#include "Player.h"


CPlayer::CPlayer(void)
{
	pos.SetPoint(0,0);
	type=0;
}


CPlayer::~CPlayer(void)
{
}

CPlayer::CPlayer(int x, int y, int type)
{
	Init(x,y,type);
}

void CPlayer::Init(int x, int y, int type)
{
	pos.SetPoint(x,y);
	this->type = type;
	
	speed.SetPoint(10,10);
	hover = false;
	reverse=false;

	bomb_capacity = 1;
	bomb_power = 1;
	status=0;
}

CPoint CPlayer::GetPosPixel()
{
	return pos;
}

void CPlayer::SetPosPixel(int x, int y)
{
	pos.SetPoint(x,y);
}

CPoint CPlayer::GetPosGrid()
{
	double tmp_x = pos.x / GRID_WIDTH;
	double tmp_y = pos.y / GRID_HEIGHT;
	return CPoint( int(tmp_x + 0.5), int(tmp_y + 0.5) );
}

int CPlayer::GetStatus()
{
	return status;
}

int CPlayer::GetBombPower()
{
	return bomb_power;
}

int CPlayer::GetBombCapacity()
{
	return bomb_capacity;
}

CPoint CPlayer::GetSpeed()
{
	return speed;
}

bool CPlayer::Hover()
{
	return hover;
}

bool CPlayer::Reverse()
{
	return reverse;
}

int CPlayer::Type()
{
	return type;
}

void CPlayer::SetHover(bool x)
{
	hover = x;
}
void CPlayer::SetReverse(bool x)
{
	reverse = x;
}

void CPlayer::SetXSpeed(int x)
{
	speed.x=x;
}
void CPlayer::SetYSpeed(int y)
{
	speed.y=y;
}

void CPlayer::SetSpeed(int x, int y)
{
	SetXSpeed(x);
	SetYSpeed(y);
}

void CPlayer::SetBombPower(int r)
{
	bomb_power = r;
}
void CPlayer::SetBombCapacity(int r)
{
	bomb_capacity = r;
}

int CPlayer::GetXPixel()
{
	return pos.x;
}

int CPlayer::GetYPixel()
{
	return pos.y;
}

void CPlayer::Move(int direction)
{
	pos.x += DIRECT_VEC[direction].x * speed.x;
	pos.y += DIRECT_VEC[direction].y * speed.y;
}