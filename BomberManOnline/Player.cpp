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
	status = PLAYER_STATUS::NONE;
	moving_state = 0;

	now_bombs = 0;
	
	now_frame = 1;
	rest_frametime = frame_time;
	facing = DOWN;

	special_access = make_pair(CPoint(0,0), false);
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

	if(direction != STOP)
	{
		pos.x += DIRECT_VEC[direction].x * speed.x * game_time;
		pos.y += DIRECT_VEC[direction].y * speed.y * game_time;
	}

	if(direction != STOP)
		facing = direction;

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
	for(i=1; i<=6; i++)
	{
		if(items[i].first == Item::NONE)
		{
			items[i].first = item;
			items[i].second = number;
		}
		break;
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
