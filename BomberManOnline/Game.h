#include "stdafx.h"
#include "ResourceManager.h"
#include "Player.h"

#pragma once
class CGame
{
public:
	CRect map_area;
	CResourceManager *p_res_manager;

	CPlayer player[MAX_PLAYER+1];
	int my_player;

public:
	CGame(void);
	~CGame(void);
	CGame(CResourceManager* res_manager);

	void Init(int player_num);
	void Render(CDC *pDC);
	void HandleKeyDown(UINT nchar);
};

