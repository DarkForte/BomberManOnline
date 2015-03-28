#include "stdafx.h"
#include "BombManager.h"
using namespace std;

CBombManager::CBombManager(void)
{
	cnt_bombs = 0; 
	p_bombs_start = 0;
}


CBombManager::~CBombManager(void)
{
}

int CBombManager::AddBomb(CBomb &p_bomb)
{
	bombs[cnt_bombs % MAX_BOMBS].first = p_bomb;
	bombs[cnt_bombs % MAX_BOMBS].second = true;
	cnt_bombs++;;
	return cnt_bombs;
}

vector<CBomb> CBombManager::Update(float game_time)
{
	vector<CBomb> ret;
	ret.clear();
	int i;
	for(i = p_bombs_start; i != cnt_bombs % MAX_BOMBS; i = (i+1)%MAX_BOMBS)
	{
		bombs[i].first.DropTime(game_time);
		if(bombs[i].first.GetRemainTime() <= 0)
		{
			ret.push_back(bombs[i].first);
			bombs[i].second = false;
			if(i==p_bombs_start)
				p_bombs_start++;
		}
	}
	return ret;
}