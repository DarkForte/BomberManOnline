#pragma once
#include "stdafx.h"
#include "PointF.h"
#include <iostream>
using namespace std;

enum class MAP_ELEMENTS
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
	bool NoCollision(PointF next_point, int direction, pair<CPoint, bool> special_access);
	bool InBound(PointF next_point);
	bool InBound(CPoint p);

	/*Set the grid to target with index, range: 0~GRIDNUM_WIDTH-1, 0~GRIDNUM_HEIGHT-1*/
	void SetGrid(int x, int y, MAP_ELEMENTS target, int index=0);
	MAP_ELEMENTS GridType(int x, int y);
	int GetIndex(int x, int y);
	void Update(float game_time);
	
	void Init();
	void Init(int map_number);
	//PointF AdjustPoint(PointF next_point, int direction);
};

