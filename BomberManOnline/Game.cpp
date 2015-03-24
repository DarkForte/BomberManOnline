#include "stdafx.h"
#include "Game.h"


CGame::CGame(void)
{
}

CGame::~CGame(void)
{
}

CGame::CGame(CResourceManager *res_manager)
{
	p_res_manager = res_manager;
}

void CGame::Init(int player_num)
{
	map_area.SetRect(PADDING, PADDING, GRIDNUM_WIDTH * GRID_WIDTH, GRID_HEIGHT * GRIDNUM_HEIGHT);
	
	player[1].Init(0,0,0);
	player[2].Init(MAP_WIDTH - SPRITE_WIDTH, 0, 0);
	player[3].Init(0, MAP_HEIGHT - SPRITE_HEIGHT, 0);
	player[4].Init(MAP_WIDTH - SPRITE_WIDTH, MAP_HEIGHT - SPRITE_HEIGHT,0);
	
	my_player = player_num;
}

void CGame::Render(CDC *pDC)
{
	p_res_manager->map_back.Draw(*pDC, map_area);

	int i;
	for(i=1;i<=MAX_PLAYER;i++)
	{
		p_res_manager->player_sprite.Draw(*pDC,
			player[i].GetXPixel(),player[i].GetYPixel(),SPRITE_WIDTH, SPRITE_HEIGHT, 
			SPRITE_WIDTH*3*(i-1), 0, SPRITE_WIDTH, SPRITE_HEIGHT);
	}
}

void CGame::HandleKeyDown(UINT nchar)
{
	if(nchar == VK_DOWN)
	{
		player[my_player].Move(DOWN);
	}
	if(nchar == VK_UP)
	{
		player[my_player].Move(UP);
	}
	if(nchar == VK_LEFT)
	{
		player[my_player].Move(LEFT);
	}
	if(nchar == VK_RIGHT)
	{
		player[my_player].Move(RIGHT);
	}
}