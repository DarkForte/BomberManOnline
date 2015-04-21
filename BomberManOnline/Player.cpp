#include "stdafx.h"
#include "Player.h"
#include <iostream>
#include <math.h>

float CPlayer::frame_time = 120;

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
	if(type == 0)
	{
		SetMaxCapacity(5);
		SetMaxPower(10);
		SetMaxSpeed(0.3);
	}
	
	speed.SetPoint(0.2, 0.2);
	hover = false;
	reverse=false;

	bomb_capacity = 3;
	bomb_power = DEFAULT_BOMBPOWER;
	status.first = PLAYER_STATUS::NONE;
	status.second = 0;
	moving_state = 0;
	team=1;

	now_bombs = 0;
	
	now_frame = 1;
	rest_frametime = frame_time;
	facing = DOWN;

	special_access = make_pair(CPoint(0,0), false);

	for (int i=1; i<=MAX_ITEMS; i++)
	{
		items[i].first = Item::NONE;
		items[i].second = 0;
	}
}

int CPlayer::GetBombPower()
{
	return bomb_power;
}

int CPlayer::GetBombCapacity()
{
	return bomb_capacity;
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

void CPlayer::Move(float game_time)
{
	direction = GetMovingDirection();

	PointF real_speed = speed;

	PLAYER_STATUS now_status = Status();

	if(now_status == PLAYER_STATUS::SLOW)
	{
		real_speed.SetPoint(0.15, 0.15);
	}
	else if(now_status == PLAYER_STATUS::WRAPPED)
	{
		real_speed.SetPoint(0,0);
	}

	if(direction != STOP)
	{
		pos.x += DIRECT_VEC[direction].x * real_speed.x * game_time;
		pos.y += DIRECT_VEC[direction].y * real_speed.y * game_time;
	}

	if(direction != STOP)
		facing = direction;

	if(now_status != PLAYER_STATUS::FLIPPING && now_status!=PLAYER_STATUS::DEAD)
	{
		if(direction != STOP)
		{
			rest_frametime -= game_time;
			if(rest_frametime <= 0)
			{
				now_frame = (now_frame +1)%4;
				rest_frametime += frame_time;
			}
		}
		else
		{
			now_frame = 2;
			rest_frametime = frame_time;
		}
	}
}


void CPlayer::SetBombPower(int r)
{
	bomb_power = r;
}
void CPlayer::SetBombCapacity(int r)
{
	bomb_capacity = r;
}

void CPlayer::SetMovingState(int next_state)
{
	if(Status() == PLAYER_STATUS::FLIPPING)
		return;
	//next state can be 0,1,2,3, we can have multiple states in order to cope with key up
	moving_state |= 1<<next_state;
}

void CPlayer::CancelMovingState(int state)
{
	if(Status() == PLAYER_STATUS::FLIPPING)
		return;

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

void CPlayer::ShutSpecialAccess()
{
	special_access.second = false;
}

void CPlayer::SetSpecialAccess( CPoint pos )
{
	special_access.first = pos;
	special_access.second = true;
}

void CPlayer::AddItem( Item item, int number )
{
	int i;
	for(i=1; i<=MAX_ITEMS; i++)
	{
		if(items[i].first == Item::NONE)
		{
			items[i].first = item;
			items[i].second = number;
			break;
		}
	}
	return;
}

Item CPlayer::PopItem( int index )
{
	if(items[index].first == Item::NONE)
		return Item::NONE;

	Item ret = items[index].first;
	items[index].second --;
	if(items[index].second <= 0)
	{
		items[index].first = Item::NONE;
	}
	return ret;
}

pair<Item, int> CPlayer::PeekItem( int index )
{
	return items[index];
}

void CPlayer::Update( float game_time )
{
	if(status.first == PLAYER_STATUS::SLOW)
	{
		status.second -= game_time;
		if(status.second <=0)
		{
			SetStatus(PLAYER_STATUS::NONE);
		}
	}
	else if(status.first == PLAYER_STATUS::WRAPPED)
	{
		status.second -= game_time;
		if(status.second <=0)
		{
			SetStatus(PLAYER_STATUS::DEAD);
		}
	}

	return;
}

void CPlayer::SetStatus( PLAYER_STATUS v1, float v2 )
{
	status.first = v1;
	status.second = v2;
	return;
}
PointF CPlayer::TryMove( float game_time )
{
	PointF ret = pos;
	direction = GetMovingDirection();

	PointF real_speed = speed;

	PLAYER_STATUS now_status = Status();

	if(now_status == PLAYER_STATUS::SLOW)
	{
		real_speed.SetPoint(0.15, 0.15);
	}
	else if(now_status == PLAYER_STATUS::WRAPPED)
	{
		real_speed.SetPoint(0,0);
	}

	if(direction != STOP)
	{
		ret.x += DIRECT_VEC[direction].x * real_speed.x * game_time;
		ret.y += DIRECT_VEC[direction].y * real_speed.y * game_time;
	}
	return ret;
}

void CPlayer::ClearMovingState()
{
	moving_state = 0;
}

/*void CPlayer::BackupStatus()
{
	back_speed = speed;
	back_capacity = bomb_capacity;
	back_power = bomb_power;
}

void CPlayer::RestoreStatus()
{
	speed = back_speed;
	bomb_capacity = back_capacity;
	bomb_power = back_power;
}*/
