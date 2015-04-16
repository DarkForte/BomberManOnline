#include "MovingObjects.h"


CMovingObjects::CMovingObjects(void)
{
}


CMovingObjects::~CMovingObjects(void)
{
}

PointF CMovingObjects::GetPosPixel()
{
	return pos;
}

void CMovingObjects::SetPosPixel(float x, float y)
{
	pos.SetPoint(x,y);
}

int CMovingObjects::GetXJudgeGrid()
{
	float tmp_x = pos.x / GRID_WIDTH;
	return int(tmp_x + 0.5);
}

int CMovingObjects::GetYJudgeGrid()
{
	float tmp_y = pos.y / GRID_HEIGHT;
	return int(tmp_y + 0.5);
}

CPoint CMovingObjects::GetPosJudgeGrid()
{
	int tmp_x = GetXJudgeGrid();
	int tmp_y = GetYJudgeGrid();
	return CPoint(tmp_x, tmp_y);
}

int CMovingObjects::GetXRealGrid()
{
	return int(floor(pos.x / GRID_WIDTH));
}

int CMovingObjects::GetYRealGrid()
{
	return int(floor(pos.y / GRID_HEIGHT));
}

CPoint CMovingObjects::GetPosRealGrid()
{
	int tmp_x = GetXRealGrid();
	int tmp_y = GetYRealGrid();
	return CPoint(tmp_x, tmp_y);
}

float CMovingObjects::GetXPixel()
{
	return pos.x;
}

float CMovingObjects::GetYPixel()
{
	return pos.y;
}

float CMovingObjects::GetXSpeed()
{
	return speed.x;
}

float CMovingObjects::GetYSpeed()
{
	return speed.y;
}

void CMovingObjects::SetXSpeed(float x)
{
	speed.x=x;
}
void CMovingObjects::SetYSpeed(float y)
{
	speed.y=y;
}

void CMovingObjects::SetSpeed(float x, float y)
{
	SetXSpeed(x);
	SetYSpeed(y);
}

void CMovingObjects::Move(float game_time)
{
	if(direction != STOP)
	{
		pos.x += DIRECT_VEC[direction].x * speed.x * game_time;
		pos.y += DIRECT_VEC[direction].y * speed.y * game_time;
	}
}

PointF CMovingObjects::GetSpeed()
{
	return speed;
}

CPoint CMovingObjects::NextGrid(int direction)
{
	CPoint real_grid = GetPosRealGrid();
	if(direction == STOP)
		return real_grid;
	return real_grid + DIRECT_VEC[direction];
}

PointF CMovingObjects::TryMove(float game_time)
{
	PointF ret = pos;
	direction = GetMovingDirection();
	if(direction != STOP)
	{
		ret.x += DIRECT_VEC[direction].x * speed.x * game_time;
		ret.y += DIRECT_VEC[direction].y * speed.y * game_time;
	}
	return ret;
}