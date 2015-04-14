
#pragma once
#include "stdafx.h"
#include "ResourceManager.h"
#include "Player.h"
#include "GameMap.h"
#include "BombManager.h"
#include "GameState.h"
#include "SceneBase.h"
#include "D2D1Header.h"


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

