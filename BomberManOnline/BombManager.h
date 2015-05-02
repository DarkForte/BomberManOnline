#pragma once
#include "Bomb.h"
#include <iostream>
#include <vector>
#include "stdafx.h"
using namespace std;

const int MAX_BOMBS = GRIDNUM_HEIGHT * GRIDNUM_WIDTH;

enum class BOMB_STATUS
{
	COUNTING, EXPLODED, PAUSE
};

class CBombManager
{
	int cnt_bombs;
	pair<CBomb, BOMB_STATUS> bombs[MAX_BOMBS];
	int p_bombs_start;

public:
	CBombManager(void);
	~CBombManager(void);

	void Init();
	int AddBomb(CBomb &p_bomb);
	vector<CBomb> Update(float game_time);
	void SuddenExplode(int index);
	void ExplodeAllBy(int user);
	CBomb GetBomb(int index);
	void PauseBomb(int index);
	void ActivateBomb(int index);
	void SetBombPos(int index, CPoint pos);
};

