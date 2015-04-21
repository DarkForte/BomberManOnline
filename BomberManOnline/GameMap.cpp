#include "stdafx.h"
#include "GameMap.h"
#include <fstream>
using namespace std;

CGameMap::CGameMap(void)
{
	Init();
}

void CGameMap::Init()
{
	int i,j;
	for(i=0;i<GRIDNUM_WIDTH;i++)
	{
		for(j=0;j<GRIDNUM_HEIGHT;j++)
		{
			grid[i][j].first = MAP_ELEMENTS::NONE;
			grid[i][j].second = 0;
		}
	}

}

void CGameMap::Init( int map_number )
{
	Init();

	char buf[20];
	sprintf_s(buf, "data\\map%d.txt", map_number);
	ifstream fin(buf);
	
	int nowx, nowy, type, index;
	while(true)
	{
		fin>>nowx>>nowy>>type>>index;
		if(nowx == -1)
			break;
		
		grid[nowy][nowx].first = (MAP_ELEMENTS)type;
		grid[nowy][nowx].second = index;
	}
}


CGameMap::~CGameMap(void)
{
}


bool CGameMap::NoCollision(PointF next_point, int direction, pair<CPoint, bool> special_access)
{
	//No judge if I see the point is in special access.

	int i,j;
	PointF points[5];
	points[1] = next_point;
	points[2] = next_point + PointF(SPRITE_WIDTH, 0);
	points[3] = next_point + PointF(0, SPRITE_HEIGHT);
	points[4] = next_point + PointF(SPRITE_WIDTH, SPRITE_HEIGHT);

	//collision with obstacles
	
	//adjust, make character a little smaller in order to avoid stucking at the edge
	const float fix=10;
	//1:left up 2:right up 3:left down 4:right down
	points[1] = points[1] + PointF(fix, fix);
	points[2] = points[2] + PointF(-fix, fix);
	points[3] = points[3] + PointF(fix, -fix);
	points[4] = points[4] + PointF(-fix, -fix);

	bool ok[5];

	for(i=1;i<=4; i++)
	{
		int x = int(points[i].x / GRID_WIDTH);
		int y = int(points[i].y / GRID_HEIGHT);

		if(special_access.second==true && special_access.first == CPoint(x,y))
		{
			ok[i]=true;
			continue;
		}

		if(grid[x][y].first == MAP_ELEMENTS::OBSTACLE || grid[x][y].first == MAP_ELEMENTS::DESTROYABLE || grid[x][y].first == MAP_ELEMENTS::BOMB)
			ok[i] = false;
		else
			ok[i] = true;
	}

	//1:left up 2:right up 3:left down 4:right down

	if(direction == UP)
	{
		return ok[1] & ok[2];
	}
	else if(direction == DOWN)
	{
		return ok[3] & ok[4];
	}
	else if(direction == LEFT)
	{
		return ok[1] & ok[3];
	}
	else //if (direction == RIGHT)
	{
		return ok[2] & ok[4]; 
	}
}

void CGameMap::SetGrid(int x, int y, MAP_ELEMENTS target, int index)
{
	grid[x][y].first = target;
	grid[x][y].second = index;
}

MAP_ELEMENTS CGameMap::GridType(int x, int y)
{
	return grid[x][y].first;
}

void CGameMap::Update( float game_time )
{
	int i,j;
	//reduce fire
	for(i=0; i<GRIDNUM_WIDTH; i++)
	{
		for (j=0; j<GRIDNUM_HEIGHT; j++)
		{
			if(grid[i][j].first == MAP_ELEMENTS::FIRE)
			{
				grid[i][j].second -= game_time;
				if(grid[i][j].second <= 0)
				{
					grid[i][j].first = MAP_ELEMENTS::NONE;
				}
			}
		}
	}
}

bool CGameMap::InBound( PointF next_point )
{
	PointF points[5];
	points[1] = next_point;
	points[2] = next_point + PointF(SPRITE_WIDTH, 0);
	points[3] = next_point + PointF(0, SPRITE_HEIGHT);
	points[4] = next_point + PointF(SPRITE_WIDTH, SPRITE_HEIGHT);

	//map boundary
	int i;
	for(i=1; i<=4; i++)
	{
		if(points[i].x<0 || points[i].x > MAP_WIDTH || points[i].y<0 || points[i].y>MAP_HEIGHT)
			return false;
	}

	return true;
}

bool CGameMap::InBound( CPoint p )
{
	if(0 <= p.x && p.x <= GRIDNUM_WIDTH-1 && 0 <= p.y && p.y <= GRIDNUM_HEIGHT-1)
		return true;
	else
		return false;
	
}

int CGameMap::GetIndex( int x, int y )
{
	return grid[x][y].second;
}

PointF CGameMap::RepelPoint( PointF next_point, int direction )
{
	PointF points[5];
	points[1] = next_point;
	points[2] = next_point + PointF(SPRITE_WIDTH, 0);
	points[3] = next_point + PointF(0, SPRITE_HEIGHT);
	points[4] = next_point + PointF(SPRITE_WIDTH, SPRITE_HEIGHT);

	if(direction == UP) // Repel to down
	{
		return PointF(points[1].x, ceil(points[1].y/GRID_HEIGHT)*GRID_HEIGHT);
	}
	else if(direction == DOWN) //Repel to up
	{
		points[3].y = floor(points[3].y/GRID_HEIGHT)*GRID_HEIGHT;
		return points[3] + PointF(0, -SPRITE_HEIGHT);
	}
	else if(direction == LEFT) //Repel to right
	{
		points[1].x = ceil(points[1].x/GRID_WIDTH)*GRID_WIDTH;
		return points[1];
	}
	else /*if (direction == RIGHT)*/
	{
		points[2].x = floor(points[2].x/GRID_WIDTH)*GRID_WIDTH;
		return points[2] + PointF(-SPRITE_WIDTH, 0);
	}
}
