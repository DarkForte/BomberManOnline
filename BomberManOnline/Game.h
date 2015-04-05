
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
	CRect map_area;

	CPlayer player[MAX_PLAYER+1];
	int my_player;

	CGameMap game_map;
	CBombManager bomb_manager;

	vector<CBomb> exploding_bombs;

public:
	CGame(void);
	~CGame(void);
	CGame(CResourceManager *res_manager);

	void Init(int player_num);
	void Render(ID2D1HwndRenderTarget* render_target);
	void HandleKeyDown(UINT nchar);
	void HandleKeyUp(UINT nchar);
	GameState Update(float game_time);

protected:
	void OperateBombs();
};

