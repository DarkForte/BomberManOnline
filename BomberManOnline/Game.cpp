#include "stdafx.h"
#include "Game.h"
using namespace std;

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
	map_area.SetRect(PADDING, PADDING, GRIDNUM_WIDTH * GRID_WIDTH + PADDING, GRID_HEIGHT * GRIDNUM_HEIGHT + PADDING);

	game_map.Init();

	player[1].Init(0,0,0);
	player[2].Init(MAP_WIDTH - SPRITE_WIDTH, 0, 0);
	player[3].Init(0, MAP_HEIGHT - SPRITE_HEIGHT, 0);
	player[4].Init(MAP_WIDTH - SPRITE_WIDTH, MAP_HEIGHT - SPRITE_HEIGHT,0);
	
	my_player = player_num;

}

void CGame::Render(ID2D1HwndRenderTarget* render_target)
{
	p_res_manager->map_back.DrawImage(render_target, PADDING, PADDING, MAP_WIDTH, MAP_HEIGHT, 0, 0);
	int i,j;

	//Draw Map Elements
	for(i=0;i<GRIDNUM_WIDTH;i++)
	{
		for(j=0; j<GRIDNUM_HEIGHT; j++)
		{
			MAP_ELEMENTS now_gridtype = game_map.GridType(i,j);
			int target_x = i*GRID_WIDTH + PADDING;
			int target_y = j*GRID_HEIGHT + PADDING;

			if(now_gridtype == MAP_ELEMENTS::BOMB)
			{
				p_res_manager->bomb_sprite.DrawImage(render_target,
					target_x, target_y,
					SPRITE_WIDTH, SPRITE_HEIGHT,
					SPRITE_WIDTH*9, 0);
			}
			else if(now_gridtype == MAP_ELEMENTS::FIRE)
			{
				p_res_manager->fire_sprite.DrawImage(render_target,
					target_x,target_y,
					SPRITE_WIDTH, SPRITE_HEIGHT,
					SPRITE_WIDTH*9, 0);
				/*p_res_manager->fire_sprite.AlphaBlend(*pDC,
					target_x,target_y,
					SPRITE_WIDTH, SPRITE_HEIGHT,
					SPRITE_WIDTH*9, 0, SPRITE_WIDTH, SPRITE_HEIGHT, 128);*/
			}
		}
	}

	//Draw Players
	for(i=1;i<=MAX_PLAYER;i++)
	{
		if(player[i].Status() == PLAYER_STATUS::NONE)
		{
			p_res_manager->player_sprite[i].DrawImage(render_target,
				player[i].GetXPixel() + PADDING, player[i].GetYPixel()+PADDING,
				SPRITE_WIDTH, SPRITE_HEIGHT, 
				SPRITE_WIDTH * player[i].NowFrame(), SPRITE_HEIGHT * player[i].Facing());
		}
	}
}

bool CheckSpecialOk(CPoint pos, int direction)
{
	if(pos.x == 0 && direction == LEFT)
		return false;
	if(pos.x == GRIDNUM_WIDTH - 1 && direction == RIGHT)
		return false;
	if(pos.y == 0 && direction == UP)
		return false;
	if(pos.y == GRIDNUM_HEIGHT - 1 && direction == DOWN)
		return false;

	return true;
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
		CPoint player_pos = player[my_player].GetPosJudgeGrid();

		if(now_bombs < player[my_player].BombCapacity() && game_map.GridType(player_pos.x, player_pos.y) == MAP_ELEMENTS::NONE)
		{
			player[my_player].SetNowBombs(now_bombs +1);

			CBomb bomb(my_player, player_pos.x, player_pos.y, DEFAULT_BOMBTIME, player[my_player].GetBombPower());
			int bomb_index = bomb_manager.AddBomb(bomb);
			game_map.SetGrid(player_pos.x, player_pos.y, MAP_ELEMENTS::BOMB, bomb_index);

			if(CheckSpecialOk(player_pos, player[my_player].GetMovingDirection() ))
				player[my_player].SetSpecialAccess(player_pos);
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

void CGame::OperateBombs()
{

}

CPoint GetJudgePoint(PointF point_pixel)
{
	float tmp_x = point_pixel.x/GRID_WIDTH;
	float tmp_y = point_pixel.y/GRID_HEIGHT;
	return CPoint(int(tmp_x+0.5), int(tmp_y+0.5));
}

GameState CGame::Update(float game_time)
{
	//move player
	int i;
	for(i=1; i<=MAX_PLAYER; i++)
	{
		PointF next_pos_pixel = player[i].TryMove(game_time);
		CPoint next_pos_judge = GetJudgePoint(next_pos_pixel);
		if((player[i].SpecialAccess().second == true && player[i].SpecialAccess().first == next_pos_judge)
			|| game_map.VerifyPoint(next_pos_pixel, player[my_player].GetMovingDirection()))
		{
			//OutputDebugPrintf("%lf %lf\n", player[my_player].GetPosPixel().x, player[my_player].GetPosPixel().y);
			player[i].Move(game_time);

			CPoint now_judgegrid = player[i].GetPosJudgeGrid();
			if(now_judgegrid != player[i].SpecialAccess().first)
			{
				player[i].ShutSpecialAccess();
			}
		}
	}
	
	//update map
	game_map.Update(game_time);

	//operate bombs
	exploding_bombs = bomb_manager.Update(game_time);
	for(i=0; i<exploding_bombs.size(); i++)
	{
		CBomb now_bomb = exploding_bombs[i];

		game_map.SetGrid(now_bomb.GetX(),now_bomb.GetY(), MAP_ELEMENTS::FIRE, DEFAULT_FIRETIME);

		for(int i_direct = 0; i_direct<=3;i_direct++)
		{
			int now_power = 0;
			CPoint now_point = now_bomb.GetPos();

			while(0 <= now_point.x && now_point.x < GRIDNUM_WIDTH
				&& 0<= now_point.y && now_point.y < GRIDNUM_HEIGHT 
				&& now_power + 1 <= now_bomb.GetPower() 
				&& game_map.GridType(now_point.x, now_point.y) != MAP_ELEMENTS::OBSTACLE 
				&& game_map.GridType(now_point.x, now_point.y) != MAP_ELEMENTS::DESTROYABLE
				&& game_map.GridType(now_point.x, now_point.y) != MAP_ELEMENTS::BOMB)
			{
				game_map.SetGrid(now_point.x, now_point.y, MAP_ELEMENTS::FIRE, DEFAULT_FIRETIME);
				for(int i_player=1; i_player<=MAX_PLAYER; i_player++)
				{
					if(player[i_player].GetPosJudgeGrid() == now_point)
					{
						player[i_player].SetStatus(PLAYER_STATUS::DEAD);
					}
				}
				now_point += DIRECT_VEC[i_direct];
				now_power++;
			}

			if(game_map.GridType(now_point.x, now_point.y) == MAP_ELEMENTS::DESTROYABLE)
			{
				game_map.SetGrid(now_point.x, now_point.y, MAP_ELEMENTS::NONE);
			}
			else if(game_map.GridType(now_point.x, now_point.y) == MAP_ELEMENTS::BOMB)
			{
				//later
			}
		}

		int now_owner = now_bomb.Owner();
		player[now_owner].SetNowBombs(player[now_owner].NowBombs()-1);
	}

	if(player[my_player].Status() == PLAYER_STATUS::DEAD)
	{
		return LOBBY;
	}

	return INGAME;
}