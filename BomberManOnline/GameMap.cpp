#include "stdafx.h"
#include "GameMap.h"


CGameMap::CGameMap(void)
{
	int i,j;
	for(i=0;i<MAP_WIDTH;i++)
	{
		for(j=0;j<MAP_HEIGHT;j++)
		{
			grid[i][j].first = NONE;
			grid[i][j].second = 0;
		}
	}
}


CGameMap::~CGameMap(void)
{
}

bool CGameMap::VerifyPoint(PointF next_point)
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

	//collision with obstacles
	for(i=1;i<=4; i++)
	{
		int x = points[i].x / GRID_WIDTH;
		int y = points[i].y / GRID_HEIGHT;

		if(grid[x][y].first == OBSTACLE || grid[x][y].first == DESTROYABLE)
			return false;
	}
	return true;
}

void CGameMap::SetGrid(int x, int y, MAP_ELEMENTS target, int index=0)
{
	grid[x][y].first = target;
	grid[x][y].second = index;
}