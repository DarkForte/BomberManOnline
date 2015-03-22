#include "stdafx.h"
#include "Game.h"


CGame::CGame(void)
{
	Init();
}


CGame::~CGame(void)
{
}

void CGame::Init()
{
	map_area.SetRect(PADDING, PADDING, GRIDNUM_WIDTH * GRID_WIDTH, GRID_HEIGHT * GRIDNUM_HEIGHT);
}