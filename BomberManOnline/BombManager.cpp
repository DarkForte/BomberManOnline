#include "stdafx.h"
#include "BombManager.h"
using namespace std;

CBombManager::CBombManager(void)
{
	memset(bombs,0,sizeof(bombs));
	cnt_bombs = 0; 
	p_bombs_start = 0;
}


CBombManager::~CBombManager(void)
{
}

int CBombManager::AddBomb(CBomb *p_bomb)
{
	bombs[cnt_bombs] = p_bomb;
	cnt_bombs++;
	return cnt_bombs;
}

vector<CBomb*> CBombManager::Update(float game_time)
{
	vector<CBomb*> ret;
	ret.clear();
	int i;
	for(i=p_bombs_start; i<=cnt_bombs; i++)
	{
		bombs[i]->DropTime(game_time);
		if(bombs[i]->GetRemainTime() <= 0)
		{
			ret.push_back(bombs[i]);
			bombs[i] = NULL;
			if(i==p_bombs_start)
				p_bombs_start++;
		}
	}
	return ret;
}