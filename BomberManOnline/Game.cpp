#include "stdafx.h"
#include "Game.h"
#include <algorithm>
#include "RenderText.h"
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

void CGame::Init(int player_num, int map_num)
{
	SetD2D1Rect(&bottom_rect, PADDING, PADDING + MAP_HEIGHT, 
		PADDING + MAP_WIDTH, WINDOW_HEIGHT - PADDING);
	SetD2D1Rect(&time_rect, PADDING + MAP_WIDTH, PADDING,
		WINDOW_WIDTH - PADDING, 200);
	SetD2D1Rect(&panel_rect, bottom_rect.right, bottom_rect.top, 
		time_rect.right, bottom_rect.bottom);

	SetD2D1Rect(&map_area, PADDING, PADDING, MAP_WIDTH + PADDING, MAP_HEIGHT + PADDING);

	game_map.Init(map_num);
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
    WCHAR buf[20];
#pragma region Draw UI

	using namespace D2D1;
	ID2D1SolidColorBrush *black_brush;
	render_target->CreateSolidColorBrush(ColorF(ColorF::Black), &black_brush);

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
	float target_width = p_res_manager->avatar_back.GetWidth();
	float target_height = p_res_manager->avatar_back.GetHeight();
	float avatar_width = p_res_manager->avatar.GetWidth();
	float avatar_height = p_res_manager->avatar.GetHeight();

	for(i=1;i<=MAX_PLAYER;i++)
	{
		int now_avatar_back_up = time_rect.bottom + (i-1)*target_height;

		p_res_manager->avatar_back.DrawImage(render_target, 
			time_rect.left, now_avatar_back_up, 
			target_width, target_height, 0, 0);

		p_res_manager->avatar.DrawImage(render_target, 
			time_rect.left, now_avatar_back_up, 
			avatar_width, avatar_height, 0, 0, 
			target_width/avatar_width, target_height/avatar_height);

		p_res_manager->userinfo_rect[i].DrawImage(render_target, 
			time_rect.left + target_width, now_avatar_back_up,
			p_res_manager->userinfo_rect[i].GetWidth(), p_res_manager->userinfo_rect[i].GetHeight(), 0,0);
	}

	////////Bottom Icons
	const int ICON_LEFT = bottom_rect.left + 20;
	const int ICON_TOP = bottom_rect.top + 20;
	const int ICON_OFFSET = 10;
	const int ICON_BOX_DIS = 70;

	float icon_width = p_res_manager->bottom_icon[1].GetWidth();
	float icon_height = p_res_manager->bottom_icon[1].GetHeight();
	target_width = icon_width;
	target_height = icon_height;
	int offset = ICON_OFFSET;
	int now_left = ICON_LEFT;
	for(i=1;i<=3;i++)
	{
		p_res_manager->bottom_icon[i].DrawImage(render_target,
			now_left, ICON_TOP, icon_width, icon_height, 0,0);
		now_left += icon_width + offset;
	}

	/////////Item Boxes
	icon_width = p_res_manager->item_box.GetWidth();
	icon_height = p_res_manager->item_box.GetHeight();
	now_left += ICON_BOX_DIS;
	for(i=1;i<=MAX_ITEMS;i++)
	{
		p_res_manager->item_box.DrawImage(render_target,
			now_left, ICON_TOP, icon_width, icon_height, 0,0, 
			target_width/icon_width, target_height/icon_height);
		now_left += target_width + offset;
	}

	//////////Corner Numbers
	now_left = ICON_LEFT;
	swprintf_s(buf, L"%d", player[my_player].GetBombCapacity() );
	RenderText(render_target, wstring(buf), now_left + 0.8*target_width, ICON_TOP + 0.8*target_height, 
		p_res_manager->p_corner_number_format, black_brush);

	now_left += ICON_OFFSET + target_width;
	swprintf_s(buf, L"%d", player[my_player].GetBombPower() );
	RenderText(render_target, wstring(buf), now_left + 0.8*target_width, ICON_TOP + 0.8*target_height, 
		p_res_manager->p_corner_number_format, black_brush);

	OutputDebugPrintf("haha: %lf\n", DEFAULT_SPEED);

	now_left += ICON_OFFSET + target_width;
	swprintf_s(buf, L"%d",  int( (player[my_player].GetXSpeed() - DEFAULT_SPEED)/DELTA_SPEED +1));
	RenderText(render_target, wstring(buf), now_left + 0.8*target_width, ICON_TOP + 0.8*target_height, 
		p_res_manager->p_corner_number_format, black_brush);

#pragma endregion

	//Draw Map, this draws the bottom panel of the map
	for(i=0;i<GRIDNUM_WIDTH;i++)
	{
		for(j=0; j<GRIDNUM_HEIGHT; j++)
		{
			MAP_ELEMENTS now_gridtype = game_map.GridType(i,j);
			int target_x = i*GRID_WIDTH + PADDING;
			int target_y = j*GRID_HEIGHT + PADDING;
				
			p_res_manager->map_none.DrawImage(render_target,
				target_x, target_y,
				GRID_WIDTH, GRID_HEIGHT,
				0, 0);
		}
	}

	render_nodes.clear();

	//Draw Players
	for(i=1;i<=MAX_PLAYER;i++)
	{
		if(player[i].Status() == PLAYER_STATUS::NONE)
		{
			render_nodes.push_back(RenderNode(player[i].GetPosPixel(), RenderType::PLAYER, i));
		}
	}

	//Draw Map Elements
	for(i=0;i<GRIDNUM_WIDTH;i++)
	{
		for(j=0; j<GRIDNUM_HEIGHT; j++)
		{
			MAP_ELEMENTS now_gridtype = game_map.GridType(i,j);
			int now_index = game_map.GetIndex(i,j);
			int target_x = i*GRID_WIDTH + PADDING;
			int target_y = j*GRID_HEIGHT + PADDING;
			if(now_gridtype == MAP_ELEMENTS::OBSTACLE)
			{
				render_nodes.push_back(RenderNode(float(target_x), float(target_y), RenderType::MAPELE_OBSTACLE) );
			}
			else if(now_gridtype == MAP_ELEMENTS::BOMB)
			{
				render_nodes.push_back(RenderNode(float(target_x), float(target_y), RenderType::MAPELE_BOMB));
			}
			else if(now_gridtype == MAP_ELEMENTS::FIRE)
			{
				render_nodes.push_back(RenderNode(float(target_x), float(target_y), RenderType::MAPELE_FIRE));
			}
			else if(now_gridtype == MAP_ELEMENTS::DESTROYABLE)
			{
				render_nodes.push_back(RenderNode(float(target_x), float(target_y), RenderType::MAPELE_DESTROYABLE));
			}
			else if(now_gridtype == MAP_ELEMENTS::ITEM)
			{
				render_nodes.push_back(RenderNode(float(target_x), float(target_y), RenderType::MAPELE_ITEM, now_index));
			}
		}
	}

	sort(render_nodes.begin(), render_nodes.end());
	for(RenderNode now : render_nodes)
	{
		if(now.type == RenderType::MAPELE_BOMB)
		{
			p_res_manager->bomb_sprite.DrawImage(render_target,
				now.pos.x, now.pos.y,
				SPRITE_WIDTH, SPRITE_HEIGHT,
				SPRITE_WIDTH*9, 0);
		}
		else if(now.type == RenderType::MAPELE_DESTROYABLE)
		{
			p_res_manager->map_destroyable.DrawImage(render_target,
				now.pos.x, now.pos.y-GRID_HEIGHT,
				GRID_WIDTH, GRID_HEIGHT*2,
				0, 0);
		}
		else if(now.type == RenderType::MAPELE_OBSTACLE)
		{
			p_res_manager->map_obstacle.DrawImage(render_target,
				now.pos.x, now.pos.y- GRID_HEIGHT,
				GRID_WIDTH, GRID_HEIGHT*2,
				0, 0);
		}
		else if(now.type == RenderType::MAPELE_FIRE)
		{
			p_res_manager->fire_sprite.DrawImage(render_target,
				now.pos.x, now.pos.y,
				SPRITE_WIDTH, SPRITE_HEIGHT,
				SPRITE_WIDTH*9, 0);
		}
		else if(now.type == RenderType::MAPELE_ITEM)
		{
			p_res_manager->icons.DrawImage(render_target,
				now.pos.x, now.pos.y,
				GRID_WIDTH, GRID_HEIGHT,
				now.index%10*GRID_WIDTH, now.index/10*GRID_HEIGHT);
		}
		else if(now.type == RenderType::PLAYER)
		{
			int now_i = now.index;
			p_res_manager->player_sprite[now_i].DrawImage(render_target,
				now.pos.x, now.pos.y,
				SPRITE_WIDTH, SPRITE_HEIGHT, 
				SPRITE_WIDTH * player[now_i].NowFrame(), SPRITE_HEIGHT * player[now_i].Facing());
		}
	}

	//Draw Map Border
	render_target->DrawRectangle(&map_area, stroke_brush, 1);

	SafeRelease(&black_brush);
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
		if(game_map.InBound(next_pos_pixel) && 
			game_map.NoCollision(next_pos_pixel, player[i].GetMovingDirection(), player[i].SpecialAccess() ) )
		{
			//OutputDebugPrintf("%lf %lf\n", player[my_player].GetPosPixel().x, player[my_player].GetPosPixel().y);
			player[i].Move(game_time);

			CPoint now_judgegrid = player[i].GetPosJudgeGrid();
			if(now_judgegrid != player[i].SpecialAccess().first)
			{
				player[i].ShutSpecialAccess();
			}

			if(game_map.GridType(next_pos_judge.x, next_pos_judge.y) == MAP_ELEMENTS::ITEM)
			{
				TouchItem(i, game_map.GetIndex(now_judgegrid.x, now_judgegrid.y) );
				game_map.SetGrid(now_judgegrid.x, now_judgegrid.y, MAP_ELEMENTS::NONE);
			}
		}
		/*else if(player[i].GetMovingDirection() != STOP)
		{
			PointF adjusted_point = game_map.AdjustPoint(next_pos_pixel, player[i].GetMovingDirection());
			player[i].SetPosPixel(adjusted_point.x, adjusted_point.y);
		}*/
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

			while(game_map.InBound(now_point)
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

			if(game_map.InBound(now_point))
			{
				if(game_map.GridType(now_point.x, now_point.y) == MAP_ELEMENTS::DESTROYABLE)
				{
					int bomb_result = CalcBombResult();
					if(bomb_result == -1)
					{
						game_map.SetGrid(now_point.x, now_point.y, MAP_ELEMENTS::NONE);
					}
					else
					{
						game_map.SetGrid(now_point.x, now_point.y, MAP_ELEMENTS::ITEM, bomb_result);
					}
				}
				else if(game_map.GridType(now_point.x, now_point.y) == MAP_ELEMENTS::BOMB)
				{
					int bomb_index = game_map.GetIndex(now_point.x, now_point.y);
					bomb_manager.SuddenExplode(bomb_index);
				}

			}
			
		}

		int now_owner = now_bomb.Owner();
		player[now_owner].SetNowBombs(player[now_owner].NowBombs()-1);
	}

	if(player[my_player].Status() == PLAYER_STATUS::DEAD)
	{
		return LOGIN;
	}

	return INGAME;
}

int CGame::CalcBombResult()
{
	return rand()%4;
}

void CGame::TouchItem( int num, int item_index )
{
	if(item_index == int(Item::CAPACITY_UP))
	{
		if(player[num].BombCapacity() +1 <= player[num].MaxCapacity() )
			player[num].SetBombCapacity(player[num].BombCapacity() +1);
	}
	else if(item_index == int(Item::POWER_UP))
	{
		if(player[num].GetBombPower() +1 <= player[num].MaxPower() )
			player[num].SetBombPower(player[num].GetBombPower() +1);
	}
	else if(item_index == int(Item::SPEED_UP))
	{
		PointF now_speed = player[num].GetSpeed();

		if(now_speed.x + DELTA_SPEED <= player[num].MaxSpeed() && now_speed.y + DELTA_SPEED <= player[num].MaxSpeed() )
			player[num].SetSpeed(now_speed.x + DELTA_SPEED, now_speed.y + DELTA_SPEED);
	}
	else if(item_index == int(Item::RANDOM_EFFECT))
	{
		int target = rand()%3;
		int delta = rand()%2;
		if(delta == 0)
			delta = -1;

		if(target == 0)
		{
			if(player[num].BombCapacity() + delta >= 1 &&
				player[num].BombCapacity() + delta <= player[num].MaxCapacity())
			{
				player[num].SetBombCapacity(player[num].BombCapacity() + delta);
			}
		}
		else if(target == 1)
		{
			if(player[num].GetBombPower() + delta >= 1 &&
				player[num].GetBombPower() + delta <= player[num].MaxPower())
			{
				player[num].SetBombPower(player[num].GetBombPower() + delta);
			}
		}
		else if(target == 2)
		{
			float vx = player[num].GetXSpeed();
			float vy = player[num].GetYSpeed();

			if(vx + float(delta)*DELTA_SPEED >= DEFAULT_SPEED && vy + float(delta)*DELTA_SPEED >= DEFAULT_SPEED
				&& vx + float(delta)*DELTA_SPEED <= player[num].MaxSpeed() && vy + float(delta)*DELTA_SPEED <=player[num].MaxSpeed())
			{
				player[num].SetSpeed(vx + float(delta)*DELTA_SPEED, vy + float(delta)*DELTA_SPEED);
			}
		}
	}
	else if(item_index == int(Item::COIN_50))
	{
		money[num] += 50;
	}
	else if(item_index == int(Item::COIN_100))
	{
		money[num] += 100;
	}
	else if(item_index == int(Item::COIN_500))
	{
		money[num] += 500;
	}
	else if(item_index >= 10 && item_index < 20) //Disposable item
	{

	}
	else if(item_index >= 20 && item_index < 30) //Transform
	{

	}
}
