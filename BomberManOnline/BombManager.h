#pragma once
#include "Bomb.h"
#include <iostream>
#include <vector>
#include "stdafx.h"
using namespace std;

const int MAX_BOMBS = GRIDNUM_HEIGHT * GRIDNUM_WIDTH;

class CBombManager
{
	int cnt_bombs;
	pair<CBomb, bool> bombs[MAX_BOMBS];
	int p_bombs_start;

public:
	CBombManager(void);
	~CBombManager(void);

	int AddBomb(CBomb &p_bomb);
	vector<CBomb> Update(float game_time);
};

