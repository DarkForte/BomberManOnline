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

CPlayer::CPlayer(float x, float y, int type)
{
	Init(x,y,type);
}

void CPlayer::Init(float x, float y, int type)
{
	pos.SetPoint(x,y);
	this->type = type;
	
	speed.SetPoint(0.2, 0.2);
	hover = false;
	reverse=false;

	bomb_capacity = 1;
	bomb_power = 1;
	status=0;
	moving_state = 0;
}

PointF CPlayer::GetPosPixel()
{
	return pos;
}

void CPlayer::SetPosPixel(float x, float y)
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

PointF CPlayer::GetSpeed()
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

void CPlayer::SetXSpeed(float x)
{
	speed.x=x;
}
void CPlayer::SetYSpeed(float y)
{
	speed.y=y;
}

void CPlayer::SetSpeed(float x, float y)
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

float CPlayer::GetXPixel()
{
	return pos.x;
}

float CPlayer::GetYPixel()
{
	return pos.y;
}

void CPlayer::Move(float game_time)
{
	int direction = GetMovingDirection();
	if(direction != STOP)
	{
		pos.x += DIRECT_VEC[direction].x * speed.x * game_time;
		pos.y += DIRECT_VEC[direction].y * speed.y * game_time;
	}
}

void CPlayer::SetMovingState(int next_state)
{
	//next state can be 0,1,2,3, we can have multiple states in order to cope with key up
	moving_state |= 1<<next_state;
}

void CPlayer::CancelMovingState(int state)
{
	moving_state -= 1<<state;
}

int CPlayer::GetMovingDirection()
{
	//get the highest 1 in moving state
	int now=1;
	int ret=0;
	while(moving_state >=now)
	{
		now*=2;
		ret++;
	}
	return ret-1;
}