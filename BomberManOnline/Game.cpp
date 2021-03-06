#include "stdafx.h"
#include "Game.h"
#include <algorithm>
#include "RenderText.h"
#include <sstream>
using namespace std;

void SetD2D1Rect(D2D1_RECT_F *r, float left, float top, float right, float bottom)
{
	r->left = left;
	r->top = top;
	r->right = right;
	r->bottom = bottom;
}

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



CPoint GetJudgePoint(PointF point_pixel)
{
	float tmp_x = point_pixel.x/GRID_WIDTH;
	float tmp_y = point_pixel.y/GRID_HEIGHT;
	return CPoint(int(tmp_x+0.5), int(tmp_y+0.5));
}

int operator * (const CPoint &a, const CPoint &b)
{
	return a.x * b.x + a.y *b.y;
}

PointF GetPixelPoint(CPoint point_judge)
{
	float tmp_x = float(point_judge.x) * GRID_WIDTH;
	float tmp_y = float(point_judge.y) * GRID_HEIGHT;
	return PointF(tmp_x, tmp_y);
}

void CGame::Init(int player_num, int map_num, int room_num, unsigned int rand_seed, int player_types[], CString player_names[])
{
	SetD2D1Rect(&bottom_rect, PADDING, PADDING + MAP_HEIGHT, 
		PADDING + MAP_WIDTH, WINDOW_HEIGHT - PADDING);
	SetD2D1Rect(&time_rect, PADDING + MAP_WIDTH, PADDING,
		WINDOW_WIDTH - PADDING, 200);
	SetD2D1Rect(&panel_rect, bottom_rect.right, bottom_rect.top, 
		time_rect.right, bottom_rect.bottom);
	SetD2D1Rect(&message_rect, 0, WINDOW_HEIGHT/2-30, WINDOW_WIDTH, WINDOW_HEIGHT/2 + 30);

	SetD2D1Rect(&map_area, PADDING, PADDING, MAP_WIDTH + PADDING, MAP_HEIGHT + PADDING);

	game_map.Init(map_num);
	bomb_manager.Init();

	player[1].Init(0,0, player_types[0]+1 , 1);
	player[2].Init(MAP_WIDTH - SPRITE_WIDTH, 0, player_types[1]+1, 1);
	player[3].Init(0, MAP_HEIGHT - SPRITE_HEIGHT, player_types[2]+1, 2);
	player[4].Init(MAP_WIDTH - SPRITE_WIDTH, MAP_HEIGHT - SPRITE_HEIGHT, player_types[3]+1, 2);
	
	for(int i=0;i<=MAX_PLAYER-1; i++)
	{
		this->player_names[i+1] = player_names[i].GetString();
	}

	memset(money,0,sizeof(money));

	my_player = player_num;
	room_number = room_num;
	srand(rand_seed);
	rest_time = 4*60*1000;

	game_result = GAME_RESULT::INGAME;
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

	ID2D1SolidColorBrush *message_brush;
	render_target->CreateSolidColorBrush(ColorF(ColorF::LightBlue), &message_brush);

	ID2D1SolidColorBrush *name_brush;
	render_target->CreateSolidColorBrush(ColorF(ColorF::Gainsboro), &name_brush);

	////////Rectangles
	p_res_manager->bottom_rect.DrawImage(render_target, bottom_rect.left, bottom_rect.top, 
		p_res_manager->bottom_rect.GetWidth(), p_res_manager->bottom_rect.GetHeight(), 0,0);

	p_res_manager->timer_rect.DrawImage(render_target, time_rect.left, time_rect.top,
		p_res_manager->timer_rect.GetWidth(), p_res_manager->timer_rect.GetHeight(), 0, 0 );

	p_res_manager->panel_rect.DrawImage(render_target, panel_rect.left, panel_rect.top,
		p_res_manager->panel_rect.GetWidth(), p_res_manager->panel_rect.GetHeight(), 0, 0);

	////////Time
	RenderText(render_target, L"TIME", (time_rect.left + time_rect.right)/2-50, time_rect.top + 20, 
		p_res_manager->p_text_format_Arial_40_bold, black_brush);

	swprintf_s(buf, L"%02d:%02d", int(rest_time/1000/60), int(rest_time/1000)%60);
	RenderText(render_target, buf, (time_rect.left + time_rect.right)/2-120, time_rect.top + 90, 
		p_res_manager->p_timer_number_format, black_brush);

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

		RenderText(render_target, player_names[i], time_rect.left + target_width + 5, now_avatar_back_up+20,
			p_res_manager->p_text_format_Arial_48_bold, name_brush);
	}

	////////Bottom Icons
	const int ICON_LEFT = bottom_rect.left + 20;
	const int ICON_TOP = bottom_rect.top + 20;
	const int ICON_OFFSET = 15;
	const int ICON_BOX_DIS = 50;

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
	for(i=1;i<=MAX_ITEMS+1;i++)
	{
		p_res_manager->item_box.DrawImage(render_target,
			now_left, ICON_TOP, icon_width, icon_height, 0,0, 
			target_width/icon_width, target_height/icon_height);

		if(i<=MAX_ITEMS)
			swprintf_s(buf, L"%d", i);
		else 
			swprintf_s(buf, L"Q");
		RenderText(render_target, buf, now_left+7, ICON_TOP+5,
			p_res_manager->p_corner_number_format, black_brush);

		int now_item = int(player[my_player].PeekItem(i).first);
		if(now_item != int(Item::NONE))
		{
			p_res_manager->icons.DrawImage(render_target,
				now_left+15,ICON_TOP+15, ITEM_WIDTH, ITEM_HEIGHT, 
				(now_item%10)*ITEM_WIDTH, (now_item/10)*ITEM_HEIGHT, 
				0.6*target_width/ITEM_WIDTH, 0.6*target_height/ITEM_HEIGHT);

			int now_amount = player[my_player].PeekItem(i).second;
			if(now_amount >= 1 && now_amount <= 10000)
			{
				swprintf_s(buf, L"x %d", now_amount);
			}
			else
			{
				memset(buf,0,sizeof(buf));
			}
			RenderText(render_target, buf, now_left + target_width -10, ICON_TOP + target_height-5,
				p_res_manager->p_corner_number_format, black_brush);
		}
		now_left += target_width + offset;
		if(i==MAX_ITEMS)
			now_left += offset;
	}

	//////////Ability Corner Numbers
	now_left = ICON_LEFT;
	swprintf_s(buf, L"%d", player[my_player].GetBombCapacity() );
	RenderText(render_target, wstring(buf), now_left + 0.8*target_width, ICON_TOP + 0.8*target_height, 
		p_res_manager->p_corner_number_format, black_brush);

	now_left += ICON_OFFSET + target_width;
	swprintf_s(buf, L"%d", player[my_player].GetBombPower() );
	RenderText(render_target, wstring(buf), now_left + 0.8*target_width, ICON_TOP + 0.8*target_height, 
		p_res_manager->p_corner_number_format, black_brush);


	now_left += ICON_OFFSET + target_width;
	swprintf_s(buf, L"%d",  int( (player[my_player].GetXSpeed() - DEFAULT_SPEED)/DELTA_SPEED +1 +1e-6));
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
		if(player[i].Status() != PLAYER_STATUS::DEAD && player[i].Trans()!=PLAYER_TRANSFORM::FLY)
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
				int now_owner = bomb_manager.GetBomb(now_index).Owner();
				if(player[now_owner].Status() == PLAYER_STATUS::BOMB_INVIS)
				{
					if(player[now_owner].Team() == player[my_player].Team())
						render_nodes.push_back(RenderNode(float(target_x), float(target_y), RenderType::MAPELE_BOMB));
				}
				else
				{
					render_nodes.push_back(RenderNode(float(target_x), float(target_y), RenderType::MAPELE_BOMB));
				}
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
				now.index%10*ITEM_WIDTH, now.index/10*ITEM_HEIGHT,
				GRID_WIDTH/ITEM_WIDTH, GRID_HEIGHT/ITEM_HEIGHT);
		}
		else if(now.type == RenderType::PLAYER)
		{
			int now_i = now.index;
			PLAYER_TRANSFORM now_trans = player[now_i].Trans();
			if(now_trans == PLAYER_TRANSFORM::NONE)
			{
				int now_actor = player[now_i].Actor();
				if(player[now_i].Status() == PLAYER_STATUS::WRAPPED)
				{
					float now_time = player[now_i].StatusTime();
					p_res_manager->player_sprite[now_actor].DrawImage(render_target,
						now.pos.x, now.pos.y,
						SPRITE_WIDTH, SPRITE_HEIGHT,
						SPRITE_WIDTH * player[now_i].NowFrame(), SPRITE_HEIGHT * player[now_i].Facing(),
						now_time/DEFAULT_WRAPTIME, now_time/DEFAULT_WRAPTIME);
				}
				else
				{
					p_res_manager->player_sprite[now_actor].DrawImage(render_target,
						now.pos.x, now.pos.y,
						SPRITE_WIDTH, SPRITE_HEIGHT, 
						SPRITE_WIDTH * player[now_i].NowFrame(), SPRITE_HEIGHT * player[now_i].Facing());
				}
				
			}
			
			else if(now_trans == PLAYER_TRANSFORM::TRANS_START || now_trans == PLAYER_TRANSFORM::TRANS_END)
			{
				int now_actor = player[now_i].Actor();
				p_res_manager->player_sprite[now_actor].DrawImage(render_target,
					now.pos.x, now.pos.y,
					SPRITE_WIDTH, SPRITE_HEIGHT, 
					SPRITE_WIDTH * player[now_i].NowFrame(), SPRITE_HEIGHT * player[now_i].Facing(),
					1, 1, (sin(player[now_i].TransTime()/50)+1)/2);
			}
			else if(now_trans == PLAYER_TRANSFORM::GHOST)
			{
				int trans_index = int(now_trans);
				float opacity;
				if(player[now_i].Team() == player[my_player].Team())
				{
					opacity = 1;
				}
				else
				{
					opacity = 0.2;
				}

				p_res_manager->trans_sprite[trans_index].DrawImage(render_target,
					now.pos.x, now.pos.y,
					SPRITE_WIDTH, SPRITE_HEIGHT,
					SPRITE_WIDTH * player[now_i].NowFrame(), SPRITE_HEIGHT * player[now_i].Facing(),
					1.0, 1.0, opacity);
			}
			else if(now_trans != PLAYER_TRANSFORM::FLY) // Fly will be handled later
			{
				int trans_index = int(now_trans);
				p_res_manager->trans_sprite[trans_index].DrawImage(render_target,
					now.pos.x, now.pos.y,
					SPRITE_WIDTH, SPRITE_HEIGHT,
					SPRITE_WIDTH * player[now_i].NowFrame(), SPRITE_HEIGHT * player[now_i].Facing());
			}
		}
	}

	//Draw darts
	for(CMovingObjects now_dart : darts)
	{
		PointF now_pos = now_dart.GetPosPixel();
		p_res_manager->icons.DrawImage(render_target,
			now_pos.x, now_pos.y, 
			GRID_WIDTH, GRID_HEIGHT,
			0, 4*ITEM_HEIGHT);
	}

	//Draw flying bombs
	for(FlyingBomb now_bomb : flying_bombs)
	{	
		PointF startF = GetPixelPoint(now_bomb.start);
		PointF endF = GetPixelPoint(now_bomb.end);
		
		float past_time = now_bomb.total_time - now_bomb.rest_time;
		float now_x = startF.x + (endF.x - startF.x)/now_bomb.total_time * past_time;
		if(now_x>MAP_WIDTH) 
			now_x -= MAP_WIDTH;
		else if(now_x < 0)
			now_x += MAP_WIDTH;

		float now_y = startF.y + (endF.y - startF.y)/now_bomb.total_time * past_time;
		if(now_y > MAP_HEIGHT) 
			now_y -= MAP_HEIGHT;
		else if(now_y < 0)
			now_y += MAP_HEIGHT;

		p_res_manager->bomb_sprite.DrawImage(render_target,
			now_x, now_y, SPRITE_WIDTH, SPRITE_HEIGHT, SPRITE_WIDTH*9, 0);
	}

	//Draw Flying Players
	for(i=1; i<=MAX_PLAYER; i++)
	{
		if(player[i].Trans() == PLAYER_TRANSFORM::FLY)
		{
			p_res_manager->trans_sprite[int(PLAYER_TRANSFORM::FLY)].DrawImage(render_target,
				player[i].GetXPixel(), player[i].GetYPixel(),
				SPRITE_WIDTH, SPRITE_HEIGHT, 
				SPRITE_WIDTH * player[i].NowFrame(), SPRITE_HEIGHT * player[i].Facing());
		}
	}

	//Draw Map Border
	render_target->DrawRectangle(&map_area, stroke_brush, 1);

	if(game_result != GAME_RESULT::INGAME)
	{
		wstring message;
		if(game_result == GAME_RESULT::WIN)
		{
			message = L"Congratulations! You win the match!";
		}
		else if(game_result == GAME_RESULT::LOSE)
		{
			message = L"Too bad! You lose the match!";
		}
		else if(game_result == GAME_RESULT::TIE)
		{
			message = L"It is a tie match! Battle again!";
		}

		render_target->FillRectangle(message_rect, message_brush);
		RenderText(render_target, message, message_rect.left + 150, message_rect.top+2, 
			p_res_manager->p_text_format_Arial_48_bold, black_brush);
	}

	SafeRelease(&black_brush);
	SafeRelease(&stroke_brush);
	SafeRelease(&message_brush);
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
	if(game_result != GAME_RESULT::INGAME)
	{
		game_result = GAME_RESULT::ACKNOWLEDGED;
		return;
	}
	operations.push(make_pair(Event::KEY_DOWN, nchar));
}

void CGame::HandleKeyUp(UINT nchar)
{
	operations.push(make_pair(Event::KEY_UP, nchar));
}

GameState CGame::Update(float game_time)
{
	if(game_result == GAME_RESULT::ACKNOWLEDGED)
	{
		return ROOM;
	}
	else if(game_result != GAME_RESULT::INGAME)
	{
		return INGAME;
	}

	CMessage msg = MakeMessage(game_time);
	CMessage recv = p_res_manager->m_Client.SendMessage(msg);
	stringstream input(recv.msg);
	int i;

	rest_time -= game_time;
	float game_times[MAX_PLAYER+1]={0};

	//analyze recv operations
	for(i=1; i<=MAX_PLAYER; i++)
	{
		int n;
		input>>n;
		input>>game_times[i];
		for(int oper_i = 1; oper_i <= n; oper_i++)
		{
			int now_event;
			UINT nchar;
			input>>now_event>>nchar;

			if(now_event == int(Event::KEY_UP))
				HandleKeyUpInUpdate(i, nchar);
			else //if(now_event == int(Event::KEY_DOWN))
				HandleKeyDownInUpdate(i, nchar);
		}
	}

	int out_member[3]={0};
	//update player
	for(i=1; i<=MAX_PLAYER; i++)
	{
		player[i].Update(game_times[i]);
		if(player[i].Status() == PLAYER_STATUS::DEAD)
		{
			out_member[player[i].Team()] ++;
			continue;
		}
		PointF next_pos_pixel = player[i].TryMove(game_times[i]);
		CPoint next_pos_judge = GetJudgePoint(next_pos_pixel);
		int now_direction = player[i].GetMovingDirection();

		bool no_collision = game_map.NoCollision(next_pos_pixel, now_direction, player[i].SpecialAccess());
		if(player[i].Trans() == PLAYER_TRANSFORM::FLY)
			no_collision = true;

		if(game_map.InBound(next_pos_pixel) && no_collision )
		{
			//OutputDebugPrintf("%lf %lf\n", player[my_player].GetPosPixel().x, player[my_player].GetPosPixel().y);
			player[i].Move(game_times[i]);

			CPoint now_judgegrid = player[i].GetPosJudgeGrid();
			if(player[i].SpecialAccess().second == true && now_judgegrid != player[i].SpecialAccess().first)
			{
				player[i].ShutSpecialAccess();
			}

			if( player[i].Trans() != PLAYER_TRANSFORM::FLY
				&& game_map.GridType(next_pos_judge.x, next_pos_judge.y) == MAP_ELEMENTS::ITEM 
				&& (player[i].SpecialAccess().second==false || player[i].SpecialAccess().first != now_judgegrid))
			{
				TouchItem(i, game_map.GetIndex(now_judgegrid.x, now_judgegrid.y) );
				game_map.SetGrid(now_judgegrid.x, now_judgegrid.y, MAP_ELEMENTS::NONE);
			}

			if(player[i].Status() != PLAYER_STATUS::WRAPPED)
			{
				int target_i;
				for(target_i=1;target_i<=MAX_PLAYER;target_i++)
				{
					if( target_i != i && player[target_i].Status() == PLAYER_STATUS::WRAPPED 
						&& player[target_i].Team() == player[i].Team() && player[target_i].GetPosJudgeGrid() == now_judgegrid)

					{
						player[target_i].SetStatus(PLAYER_STATUS::NONE);
					}
				}
			}
		}
		else if(now_direction != STOP) // collide with border/obstacle
		{
			if(player[i].Status() == PLAYER_STATUS::FLIPPING)
			{
				player[i].SetStatus(PLAYER_STATUS::NONE);
				player[i].ClearMovingState();
			}
			if(no_collision)
			{
				PointF adjusted_point = game_map.RepelPoint(next_pos_pixel, now_direction);
				player[i].SetPosPixel(adjusted_point.x, adjusted_point.y);
			}
		}
	}

	//update darts
	for(list<CMovingObjects>::iterator it = darts.begin(); it!=darts.end(); )
	{
		PointF next_pos_pixel = it->TryMove(game_time);
		CPoint next_pos_judge = GetJudgePoint(next_pos_pixel);
		int now_direction = it->GetMovingDirection();

		if(game_map.InBound(next_pos_pixel) && 
			game_map.NoCollision(next_pos_pixel, now_direction ) )
		{
			//OutputDebugPrintf("%lf %lf\n", player[my_player].GetPosPixel().x, player[my_player].GetPosPixel().y);
			it->Move(game_time);
			it++;
		}
		else
		{
			next_pos_pixel = game_map.RepelPoint(next_pos_pixel, now_direction);
			next_pos_judge = GetJudgePoint(next_pos_pixel) + DIRECT_VEC[now_direction];
			next_pos_pixel.x = next_pos_judge.x*GRID_WIDTH;
			next_pos_pixel.y = next_pos_judge.y*GRID_HEIGHT;
			if(game_map.InBound(next_pos_pixel) )
			{
				/*if(game_map.GridType(next_pos_judge.x, next_pos_judge.y) != MAP_ELEMENTS::BOMB)
				{
					next_pos_judge += DIRECT_VEC[it->GetMovingDirection()];
				}*/
				if(game_map.GridType(next_pos_judge.x, next_pos_judge.y) == MAP_ELEMENTS::BOMB)
				{
					int index = game_map.GetIndex(next_pos_judge.x, next_pos_judge.y);
					bomb_manager.SuddenExplode(index);
				}

			}
			it=darts.erase(it);
		}
	}

	//update flying bombs
	for(list<FlyingBomb>::iterator it = flying_bombs.begin(); it!=flying_bombs.end();)
	{
		it->rest_time -= game_time;
		if(it->rest_time <= 0)
		{
			CPoint map_pos;
			map_pos.SetPoint((it->end.x + GRIDNUM_WIDTH) % GRIDNUM_WIDTH, (it->end.y + GRIDNUM_HEIGHT) % GRIDNUM_HEIGHT);
			MAP_ELEMENTS grid_type = game_map.GridType(map_pos.x, map_pos.y);
			if(grid_type == MAP_ELEMENTS::BOMB || grid_type == MAP_ELEMENTS::FIRE)
			{
				bomb_manager.SuddenExplode(it->bomb_index);
			}
			else
			{
				game_map.SetGrid(map_pos.x, map_pos.y, MAP_ELEMENTS::BOMB, it->bomb_index);
				int i;
				for(i=1;i<=MAX_PLAYER; i++)
				{
					if(player[i].GetPosJudgeGrid() == map_pos)
						player[i].SetSpecialAccess(map_pos);
				}
				bomb_manager.ActivateBomb(it->bomb_index);
			}
			it = flying_bombs.erase(it);
		}
		else
		{
			it++;
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

			while(game_map.InBound(now_point)
				&& now_power + 1 <= now_bomb.GetPower() 
				&& game_map.GridType(now_point.x, now_point.y) != MAP_ELEMENTS::OBSTACLE 
				&& game_map.GridType(now_point.x, now_point.y) != MAP_ELEMENTS::DESTROYABLE
				&& game_map.GridType(now_point.x, now_point.y) != MAP_ELEMENTS::BOMB)
			{
				game_map.SetGrid(now_point.x, now_point.y, MAP_ELEMENTS::FIRE, DEFAULT_FIRETIME);
				for(int i_player=1; i_player<=MAX_PLAYER; i_player++)
				{
					if(player[i_player].GetPosJudgeGrid() == now_point 
						&& player[i_player].Status() != PLAYER_STATUS::DEAD && player[i_player].Status() != PLAYER_STATUS::WRAPPED
						&& player[i_player].Trans() != PLAYER_TRANSFORM::TRANS_END && player[i_player].Trans() != PLAYER_TRANSFORM::TRANS_START)
					{
						if(player[i_player].Trans() == PLAYER_TRANSFORM::NONE)
							player[i_player].SetStatus(PLAYER_STATUS::WRAPPED, DEFAULT_WRAPTIME);
						else
						{
							player[i_player].SetTransTime(0);//Do not use SetTrans because canceling the effects happens when turning from current form to trans_end.
						}
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

	//Win lose
	if( (out_member[1] == out_member[2] && out_member[2] == 2) || rest_time<=0)
	{
		game_result = GAME_RESULT::TIE;
		SendQuitMessage();
		SendRewardMessage();
		
	}
	else if(out_member[player[my_player].Team()] ==2)
	{
		game_result = GAME_RESULT::LOSE;
		SendQuitMessage();
		SendRewardMessage();
		
	}
	else if(out_member[3-player[my_player].Team()] ==2)
	{
		game_result = GAME_RESULT::WIN;
		SendQuitMessage();
		SendRewardMessage();
		
	}

	return INGAME;
}

int CGame::CalcBombResult()
{
	//return int(Item::KILLER);
	/*50% rate of dropping an item*/
	int r = rand()%100;
	if(r>=50)
		return int(Item::NONE);
	
	/*40% - 40% - 20% rate of dropping 3 kinds of items*/
	r = rand()%100;
	if(r<40)
	{
		r = rand()%7;
	}
	else if(r<80)
	{
		r = 10 + rand()%8;
	}
	else
	{
		r = 20 + rand()%8;
	}
	return r;
}

void CGame::TouchItem( int num, int item_index )
{
	Item now_item = Item(item_index);

	if(now_item == Item::CAPACITY_UP)
	{
		if(player[num].BombCapacity() +1 <= player[num].MaxCapacity() )
			player[num].SetBombCapacity(player[num].BombCapacity() +1);
	}
	else if(now_item == Item::POWER_UP)
	{
		if(player[num].GetBombPower() +1 <= player[num].MaxPower() )
			player[num].SetBombPower(player[num].GetBombPower() +1);
	}
	else if(now_item == Item::SPEED_UP)
	{
		PointF now_speed = player[num].GetSpeed();

		if(now_speed.x + DELTA_SPEED <= player[num].MaxSpeed() && now_speed.y + DELTA_SPEED <= player[num].MaxSpeed() )
			player[num].SetSpeed(now_speed.x + DELTA_SPEED, now_speed.y + DELTA_SPEED);
	}
	else if(now_item == Item::RANDOM_EFFECT)
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
	else if(now_item == Item::COIN_50)
	{
		money[num] += 50;
	}
	else if(now_item == Item::COIN_100)
	{
		money[num] += 100;
	}
	else if(now_item == Item::COIN_500)
	{
		money[num] += 500;
	}

	//Disposable item
	else if(item_index >= 10 && item_index < 20) 
	{
		if(now_item == Item::GLUE)
		{
			player[num].AddItem(Item::GLUE, 3);
		}
		else
		{
			player[num].AddItem(now_item);
		}
	}

	//Transform
	else if(item_index >= 20 && item_index < 30) 
	{
		if(player[num].Trans() == PLAYER_TRANSFORM::NONE)
		{
			player[num].SetTrans(PLAYER_TRANSFORM::TRANS_START, TRANS_BEGINTIME, PLAYER_TRANSFORM(item_index));
		}
	}

	//Trap item
	else if(item_index >= 30 && item_index < 40)
	{
		if(now_item == Item::TRAP_BANANA)
		{
			player[num].SetStatus(PLAYER_STATUS::FLIPPING);
		}
		else if(now_item == Item::TRAP_GLUE)
		{
			player[num].SetStatus(PLAYER_STATUS::SLOW, 10000);
		}
		else if(now_item == Item::TRAP_LASER)
		{
			player[num].SetStatus(PLAYER_STATUS::WRAPPED, DEFAULT_WRAPTIME);
		}
	}
}

void CGame::UseItem( int user, Item item )
{
	CPoint now_pos = player[user].GetPosJudgeGrid();
	if(item == Item::BANANA || item == Item::GLUE || item == Item::LASER)
	{
		game_map.SetGrid(now_pos.x, now_pos.y, MAP_ELEMENTS::ITEM, int(item)+20);
		player[user].SetSpecialAccess(now_pos);
	}
	else if(item == Item::BOMB_INVIS)
	{
		player[user].SetStatus(PLAYER_STATUS::BOMB_INVIS, 10);
	}
	else if(item == Item::OXYGEN)
	{
		if(player[user].Status() == PLAYER_STATUS::WRAPPED)
		{
			player[user].SetStatus(PLAYER_STATUS::WRAPPED, player[user].StatusTime() + 1000);
		}
	}
	else if(item == Item::DART)
	{
		CMovingObjects new_dart(player[user].GetPosJudgeGrid(), DART_SPEED, player[user].Facing());
		darts.push_back(new_dart);
	}
	else if(item == Item::ESCAPE)
	{
		if(player[user].Status() == PLAYER_STATUS::WRAPPED)
		{
			player[user].SetStatus(PLAYER_STATUS::NONE);
		}
	}
	else if(item == Item::CONTROLLER)
	{
		bomb_manager.ExplodeAllBy(user);
	}
}

void CGame::KickBomb( int index, int direction )
{
	bomb_manager.PauseBomb(index);
	CBomb now_bomb = bomb_manager.GetBomb(index);
	CPoint now_pos = now_bomb.GetPos();
	CPoint end_pos = now_pos;
	game_map.SetGrid(now_pos.x, now_pos.y, MAP_ELEMENTS::NONE);
	
	end_pos.x = end_pos.x + DIRECT_VEC[direction].x * KICK_DISTANCE ;
	end_pos.y = end_pos.y + DIRECT_VEC[direction].y * KICK_DISTANCE ;
	float flying_time = KICK_DISTANCE * KICK_SINGLEFLYTIME;
	CPoint map_pos;
	while(true)
	{
		map_pos.x = (end_pos.x + GRIDNUM_WIDTH) % GRIDNUM_WIDTH;
		map_pos.y = (end_pos.y + GRIDNUM_HEIGHT) % GRIDNUM_HEIGHT;
		if(game_map.GridType(map_pos.x, map_pos.y) == MAP_ELEMENTS::NONE)
			break;
		end_pos.x = end_pos.x + DIRECT_VEC[direction].x ;
		end_pos.y = end_pos.y + DIRECT_VEC[direction].y ;
		flying_time += KICK_SINGLEFLYTIME;
	}
	bomb_manager.SetBombPos(index, map_pos);

	FlyingBomb now_flyingbomb(now_pos, end_pos, index, flying_time, direction);
	flying_bombs.push_back(now_flyingbomb);
}

void CGame::HandleKeyUpInUpdate( int player_num, UINT nchar )
{
	if(nchar == VK_DOWN)
	{
		player[player_num].CancelMovingState(DOWN);
	}
	if(nchar == VK_UP)
	{
		player[player_num].CancelMovingState(UP);
	}
	if(nchar == VK_LEFT)
	{
		player[player_num].CancelMovingState(LEFT);
	}
	if(nchar == VK_RIGHT)
	{
		player[player_num].CancelMovingState(RIGHT);
	}
}

void CGame::HandleKeyDownInUpdate( int player_num, UINT nchar )
{
	if(nchar == VK_DOWN)
	{
		player[player_num].SetMovingState(DOWN);
	}
	if(nchar == VK_UP)
	{
		player[player_num].SetMovingState(UP);
	}
	if(nchar == VK_LEFT)
	{
		player[player_num].SetMovingState(LEFT);
	}
	if(nchar == VK_RIGHT)
	{
		player[player_num].SetMovingState(RIGHT);
	}
	if(nchar == VK_SPACE)
	{
		PLAYER_STATUS now_status = player[player_num].Status();
		if(now_status != PLAYER_STATUS::DEAD && now_status != PLAYER_STATUS::WRAPPED)
		{
			int now_bombs = player[player_num].NowBombs();
			if(player[player_num].Trans() == PLAYER_TRANSFORM::DEMON || player[player_num].Trans() == PLAYER_TRANSFORM::UBW)
				now_bombs = 0;

			CPoint player_pos = player[player_num].GetPosJudgeGrid();

			if(now_bombs < player[player_num].BombCapacity() && game_map.GridType(player_pos.x, player_pos.y) == MAP_ELEMENTS::NONE)
			{
				player[player_num].SetNowBombs(now_bombs +1);

				CBomb bomb(player_num, player_pos.x, player_pos.y, DEFAULT_BOMBTIME, player[player_num].GetBombPower());
				int bomb_index = bomb_manager.AddBomb(bomb);
				game_map.SetGrid(player_pos.x, player_pos.y, MAP_ELEMENTS::BOMB, bomb_index);

				player[player_num].SetSpecialAccess(player_pos);
			}
		}
	}
	if('1' <= nchar && nchar <= '6' || nchar == 'Q')
	{
		PLAYER_STATUS now_status = player[player_num].Status();
		if(now_status != PLAYER_STATUS::DEAD)
		{
			if(player[player_num].Trans() == PLAYER_TRANSFORM::PANDA && nchar == 'Q')
			{
				CPoint next_point = player[player_num].GetPosJudgeGrid() + DIRECT_VEC[player[player_num].Facing()];
				if(game_map.GridType(next_point.x, next_point.y) == MAP_ELEMENTS::BOMB)
				{
					int index = game_map.GetIndex(next_point.x, next_point.y);
					KickBomb(index, player[player_num].Facing());
				}
			}
			else
			{
				int index = (nchar == 'Q')? MAX_ITEMS+1 : nchar-'0';
				pair<Item, int> now_item = player[player_num].PeekItem(index);
				if(now_item.first != Item::NONE)
				{
					UseItem(player_num, now_item.first);
					player[player_num].PopItem(index);
				}
			}
		}
	}
}

CMessage CGame::MakeMessage(float game_time)
{
	CMessage ret;
	ret.type1 = MSG_GAME;
	ret.type2 = MSG_GAME_OPERATION;
	ret.para1 = room_number;
	ret.para2 = my_player;
	sprintf_s(ret.msg, "%u %f", operations.size(), game_time);

	while(!operations.empty())
	{
		pair<Event, UINT> now = operations.front();
		operations.pop();
		
		sprintf_s(ret.msg, "%s %d %u", ret.msg, int(now.first), now.second);
	}
	return ret;
}

void CGame::SendQuitMessage()
{
	CMessage msg;
	msg.type1 = MSG_GAME;
	msg.type2 = MSG_GAME_QUIT;
	msg.para1 = room_number;
	msg.para2 = my_player;
	msg = p_res_manager->m_Client.SendMessage(msg);
	return;
}

void CGame::HandleLButtonDown()
{
	if(game_result != GAME_RESULT::INGAME)
	{
		game_result = GAME_RESULT::ACKNOWLEDGED;
	}
	return;
}

void CGame::SendRewardMessage()
{
	CMessage msg;
	msg.type1 = MSG_DATA;
	msg.type2 = MSG_DATA_GET_MONEY;
	msg.para1 = p_res_manager->account.user_id;
	msg = p_res_manager->m_Client.SendMessage(msg);
	
	CMessage set_msg;
	int now_money = msg.para1 + money[my_player];
	set_msg.type1 = MSG_DATA;
	set_msg.type2 = MSG_DATA_SET_MONEY;
	set_msg.para1 = p_res_manager->account.user_id;
	set_msg.para2 = now_money;
	set_msg = p_res_manager->m_Client.SendMessageW(set_msg);

	msg.type1 = MSG_DATA;
	msg.type2 = MSG_DATA_GET_EXP;
	msg.para1 = p_res_manager->account.user_id;
	msg = p_res_manager->m_Client.SendMessage(msg);

	int now_exp = msg.para1;

	if(game_result == GAME_RESULT::WIN)
		now_exp += 10;
	else if(game_result == GAME_RESULT::TIE)
		now_exp +=5;
	else if(game_result == GAME_RESULT::LOSE)
	{
		now_exp +=1;
	}

	set_msg.type1 = MSG_DATA;
	set_msg.type2 = MSG_DATA_SET_EXP;
	set_msg.para1 = p_res_manager->account.user_id;
	set_msg.para2 = now_exp;
	set_msg = p_res_manager->m_Client.SendMessage(set_msg);

}
