
#pragma once
#include "stdafx.h"
#include "ResourceManager.h"
#include "Player.h"
#include "GameMap.h"
#include "BombManager.h"
#include "GameState.h"
#include "SceneBase.h"
#include "D2D1Header.h"
#include "Item.h"
#include <list>
#include <queue>
#include "Message.h"
#include "Client.h"

#pragma region RenderType Defination

enum class RenderType
{
	MAPELE_DESTROYABLE, MAPELE_OBSTACLE, MAPELE_BOMB, MAPELE_FIRE, MAPELE_ITEM, PLAYER
};

struct RenderNode
{
	PointF pos;//pos: the bottom point
	RenderType type;
	int index;
	RenderNode(PointF p, RenderType t, int i=0)
	{
		pos=p;
		type=t;
		index=i;
	}
	RenderNode(float x, float y, RenderType t, int i=0)
	{
		pos.x=x;
		pos.y=y;
		type=t;
		index=i;
	}

	bool operator < (const RenderNode &b)const
	{
		return pos.y < b.pos.y;
	}
};



#pragma endregion

struct FlyingBomb
{
	CPoint start;
	CPoint end;
	int bomb_index;
	float total_time;
	float rest_time;
	int direction;
	
	FlyingBomb(){}
	FlyingBomb(CPoint s, CPoint e, int index, float total_t, int direct)
	{
		start=s;
		end=e;
		bomb_index = index;
		total_time = total_t;
		rest_time = total_t;
		direction = direct;
	}
	
};

class CGame : public CSceneBase
{
public:
	D2D1_RECT_F map_area;
	D2D1_RECT_F bottom_rect;
	D2D1_RECT_F time_rect;
	//D2D1_RECT_F info_rect;
	D2D1_RECT_F panel_rect;
	D2D1_RECT_F user_rect;

	CPlayer player[MAX_PLAYER+1];
	float rest_time;
	
	int money[MAX_PLAYER+1];

	int my_player;
	int room_number;
	queue< pair<Event, UINT> > operations;

	CGameMap game_map;
	CBombManager bomb_manager;

	vector<CBomb> exploding_bombs;
	vector<RenderNode> render_nodes;
	list<CMovingObjects> darts;
	list<FlyingBomb> flying_bombs;

public:
	CGame(void);
	~CGame(void);
	CGame(CResourceManager *res_manager);

	void Init(int player_num, int map_num, int room_num, unsigned int rand_seed, int player_type);
	void Render(ID2D1HwndRenderTarget* render_target);
	void HandleKeyDown(UINT nchar);
	void HandleKeyUp(UINT nchar);
	GameState Update(float game_time);
	void SendQuitMessage();

protected:
	int CalcBombResult();
	void TouchItem(int player_num, int item_index);
	void UseItem(int user, Item item);
	void KickBomb(int index, int direction);
	void HandleKeyUpInUpdate(int player_num, UINT nchar);
	void HandleKeyDownInUpdate(int player_num, UINT nchar);
	CMessage MakeMessage(float game_time);
	
};

