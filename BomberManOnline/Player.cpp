#include "stdafx.h"
#include "Player.h"
#include <iostream>
#include <math.h>

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

	bomb_capacity = 3;
	bomb_power = DEFAULT_BOMBPOWER;
	status = PLAYER_STATUS::NONE;
	moving_state = 0;

	now_bombs = 0;
}

PointF CPlayer::GetPosPixel()
{
	return pos;
}

void CPlayer::SetPosPixel(float x, float y)
{
	pos.SetPoint(x,y);
}

int CPlayer::GetXJudgeGrid()
{
	float tmp_x = pos.x / GRID_WIDTH;
	return int(tmp_x + 0.5);
}

int CPlayer::GetYJudgeGrid()
{
	float tmp_y = pos.y / GRID_HEIGHT;
	return int(tmp_y + 0.5);
}

CPoint CPlayer::GetPosJudgeGrid()
{
	int tmp_x = GetXJudgeGrid();
	int tmp_y = GetYJudgeGrid();
	return CPoint(tmp_x, tmp_y);
}

int CPlayer::GetXRealGrid()
{
	return int(floor(pos.x / GRID_WIDTH));
}

int CPlayer::GetYRealGrid()
{
	return int(floor(pos.y / GRID_HEIGHT));
}

CPoint CPlayer::GetPosRealGrid()
{
	int tmp_x = GetXRealGrid();
	int tmp_y = GetYRealGrid();
	return CPoint(tmp_x, tmp_y);
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

CPoint CPlayer::NextGrid(int direction)
{
	CPoint real_grid = GetPosRealGrid();
	if(direction == STOP)
		return real_grid;
	return real_grid + DIRECT_VEC[direction];
}

PointF CPlayer::TryMove(float game_time)
{
	PointF ret = pos;
	int direction = GetMovingDirection();
	if(direction != STOP)
	{
		ret.x += DIRECT_VEC[direction].x * speed.x * game_time;
		ret.y += DIRECT_VEC[direction].y * speed.y * game_time;
	}
	return ret;
}

int CPlayer::NowBombs()
{
	return now_bombs;
}

int CPlayer::BombCapacity()
{
	return bomb_capacity;
}

void CPlayer::SetNowBombs(int bombs)
{
	now_bombs = bombs;
}