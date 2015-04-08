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

void SetD2D1Rect(D2D1_RECT_F *r, int left, int top, int right, int bottom)
{
	r->left = left;
	r->top = top;
	r->right = right;
	r->bottom = bottom;
}

void CGame::Init(int player_num)
{
	SetD2D1Rect(&bottom_rect, PADDING, PADDING + MAP_HEIGHT, 
		PADDING + MAP_WIDTH, WINDOW_HEIGHT - PADDING);
	SetD2D1Rect(&time_rect, PADDING + MAP_WIDTH, PADDING,
		WINDOW_WIDTH - PADDING, 200);
	SetD2D1Rect(&panel_rect, bottom_rect.right, bottom_rect.top, 
		time_rect.right, bottom_rect.bottom);

	SetD2D1Rect(&map_area, PADDING, PADDING, MAP_WIDTH + PADDING, MAP_HEIGHT + PADDING);

	game_map.Init();
	bomb_manager.Init();

	player[1].Init(0,0,0);
	player[2].Init(MAP_WIDTH - SPRITE_WIDTH, 0, 0);
	player[3].Init(0, MAP_HEIGHT - SPRITE_HEIGHT, 0);
	player[4].Init(MAP_WIDTH - SPRITE_WIDTH, MAP_HEIGHT - SPRITE_HEIGHT,0);
	
	my_player = player_num;

}

void CGame::Render(ID2D1HwndRenderTarget* render_target)
{
	int i,j;
	
	//Draw UI

	using namespace D2D1;
	ID2D1SolidColorBrush *brush;
	render_target->CreateSolidColorBrush(ColorF(ColorF::Gainsboro), &brush);

	ID2D1SolidColorBrush *stroke_brush;
	render_target->CreateSolidColorBrush(ColorF(ColorF::DarkGray), &stroke_brush);

	////////Rectangles
	p_res_manager->bottom_rect.DrawImage(render_target, bottom_rect.left, bottom_rect.top, 
		p_res_manager->bottom_rect.GetWidth(), p_res_manager->bottom_rect.GetHeight(), 0,0);

	p_res_manager->timer_rect.DrawImage(render_target, time_rect.left, time_rect.top,
		p_res_manager->timer_rect.GetWidth(), p_res_manager->timer_rect.GetHeight(), 0, 0 );

	p_res_manager->panel_rect.DrawImage(render_target, panel_rect.left, panel_rect.top,
		p_res_manager->panel_rect.GetWidth(), p_res_manager->panel_rect.GetHeight(), 0, 0);

	////////Avatars
	int avatar_width = p_res_manager->avatar_back.GetWidth();
	int avatar_height = p_res_manager->avatar_back.GetHeight();
	for(i=1;i<=MAX_PLAYER;i++)
	{
		int now_avatar_back_up = time_rect.bottom + (i-1)*avatar_height;

		p_res_manager->avatar_back.DrawImage(render_target, 
			time_rect.left, now_avatar_back_up, 
			avatar_width, avatar_height, 0, 0);

		p_res_manager->avatar.DrawImage(render_target, 
			time_rect.left + 20, now_avatar_back_up + 20, 
			p_res_manager->avatar.GetWidth(), p_res_manager->avatar.GetHeight(), 0, 0);

		p_res_manager->userinfo_rect[i].DrawImage(render_target, 
			time_rect.left + avatar_width, now_avatar_back_up,
			p_res_manager->userinfo_rect[i].GetWidth(), p_res_manager->userinfo_rect[i].GetHeight(), 0,0);
	}

	////////Bottom Icons
	int icon_width = p_res_manager->bottom_icon[1].GetWidth();
	int icon_height = p_res_manager->bottom_icon[1].GetHeight();
	int offset = 10;
	int now_left = bottom_rect.left + 10;
	for(i=1;i<=3;i++)
	{
		p_res_manager->bottom_icon[i].DrawImage(render_target,
			now_left, bottom_rect.top + 10, icon_width, icon_height, 0,0);
		now_left += icon_width + offset;
	}

	/////////Item Boxes
	icon_width = p_res_manager->item_box.GetWidth();
	icon_height = p_res_manager->item_box.GetHeight();
	offset = 5;
	now_left += 10;
	for(i=1;i<=MAX_ITEMS;i++)
	{
		p_res_manager->item_box.DrawImage(render_target,
			now_left, bottom_rect.top + 10, icon_width, icon_height, 0,0);
		now_left += icon_width + offset;
	}

	//Draw Map

	p_res_manager->map_back.DrawImage(render_target, PADDING, PADDING, MAP_WIDTH, MAP_HEIGHT, 0, 0);

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

	//Draw Map Border
	render_target->DrawRectangle(&map_area, stroke_brush, 1);

	SafeRelease(&brush);
	SafeRelease(&stroke_brush);
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