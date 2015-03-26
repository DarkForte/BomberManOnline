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
			player[i].GetXPixel() + PADDING, player[i].GetYPixel()+PADDING,
			SPRITE_WIDTH, SPRITE_HEIGHT, 
			SPRITE_WIDTH*3*(i-1), 0, 
			SPRITE_WIDTH, SPRITE_HEIGHT);
	}
}

void CGame::HandleKeyDown(UINT nchar)
{
	if(nchar == VK_DOWN)
	{
		player[my_player].SetMovingState(DOWN);
	}
	if(nchar == VK_UP)
	{
		player[my_player].SetMovingState(UP);
	}
	if(nchar == VK_LEFT)
	{
		player[my_player].SetMovingState(LEFT);
	}
	if(nchar == VK_RIGHT)
	{
		player[my_player].SetMovingState(RIGHT);
	}
	if(nchar == VK_SPACE)
	{
		int now_bombs = player[my_player].NowBombs();
		if(now_bombs < player[my_player].BombCapacity())
		{
			player[my_player].SetNowBombs(now_bombs +1);

			CPoint player_pos = player[my_player].GetPosJudgeGrid();
			CBomb bomb(player_pos.x, player_pos.y, DEFAULT_BOMBTIME, player[my_player].GetBombPower());
			int bomb_index = bomb_manager.AddBomb(&bomb);
			game_map.SetGrid(player_pos.x, player_pos.y, BOMB, bomb_index);
		}
	}
}

void CGame::HandleKeyUp(UINT nchar)
{
	if(nchar == VK_DOWN)
	{
		player[my_player].CancelMovingState(DOWN);
	}
	if(nchar == VK_UP)
	{
		player[my_player].CancelMovingState(UP);
	}
	if(nchar == VK_LEFT)
	{
		player[my_player].CancelMovingState(LEFT);
	}
	if(nchar == VK_RIGHT)
	{
		player[my_player].CancelMovingState(RIGHT);
	}
}

void CGame::Update(float game_time)
{
	int i;
	for(i=1; i<=MAX_PLAYER; i++)
	{
		PointF next_pos_pixel = player[i].TryMove(game_time);
		if(game_map.VerifyPoint(next_pos_pixel))
		{
			//OutputDebugPrintf("%lf %lf\n", player[my_player].GetPosPixel().x, player[my_player].GetPosPixel().y);
			player[i].Move(game_time);
		}
	}
}