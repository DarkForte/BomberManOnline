#include "stdafx.h"
#include "GameMap.h"


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


CGameMap::~CGameMap(void)
{
}

bool CGameMap::VerifyPoint(PointF next_point, int direction)
{
	//if(next_point.y<300)
	//OutputDebugPrintf("Upper_left: %lf, %lf\n", next_point.x, next_point.y);
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

/*PointF CGameMap::AdjustPoint( PointF next_point, int direction )
{
	if(VerifyPoint(next_point, direction) == true)
		return next_point;

	PointF points[5];
	points[1] = next_point;
	points[2] = next_point + PointF(SPRITE_WIDTH, 0);
	points[3] = next_point + PointF(0, SPRITE_HEIGHT);
	points[4] = next_point + PointF(SPRITE_WIDTH, SPRITE_HEIGHT);

	
}*/
