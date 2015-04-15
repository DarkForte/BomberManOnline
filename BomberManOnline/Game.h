
#pragma once
#include "stdafx.h"
#include "ResourceManager.h"
#include "Player.h"
#include "GameMap.h"
#include "BombManager.h"
#include "GameState.h"
#include "SceneBase.h"
#include "D2D1Header.h"


#pragma region RenderType Defination

enum class RenderType
{
	MAPELE_DESTROYABLE, MAPELE_OBSTACLE, MAPELE_BOMB, MAPELE_FIRE, PLAYER
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
	
	int my_player;

	CGameMap game_map;
	CBombManager bomb_manager;

	vector<CBomb> exploding_bombs;
	vector<RenderNode> render_nodes;

public:
	CGame(void);
	~CGame(void);
	CGame(CResourceManager *res_manager);

	void Init(int player_num, int map_num);
	void Render(ID2D1HwndRenderTarget* render_target);
	void HandleKeyDown(UINT nchar);
	void HandleKeyUp(UINT nchar);
	GameState Update(float game_time);

protected:
	void OperateBombs();
};

