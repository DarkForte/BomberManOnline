#pragma once
#include "stdafx.h"
#include "PointF.h"
#include <iostream>
using namespace std;

enum MAP_ELEMENTS
{
	NONE, OBSTACLE, DESTROYABLE, ITEM, FIRE, BOMB
};

class CGameMap
{
	pair<MAP_ELEMENTS, int> grid[GRIDNUM_WIDTH][GRIDNUM_HEIGHT];

public:
	CGameMap(void);
	~CGameMap(void);

	/*Verify whether next_point is ok to go*/
	bool VerifyPoint(PointF next_point, int direction);
	/*Set the grid to target with index, range: 0~GRIDNUM_WIDTH-1, 0~GRIDNUM_HEIGHT-1*/
	void SetGrid(int x, int y, MAP_ELEMENTS target, int index=0);
	MAP_ELEMENTS GridType(int x, int y);
};

