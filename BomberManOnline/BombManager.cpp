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

int CBombManager::AddBomb(CBomb &new_bomb)
{
	bombs[cnt_bombs % MAX_BOMBS].first = new_bomb;
	bombs[cnt_bombs % MAX_BOMBS].second = true;
	cnt_bombs++;
	return cnt_bombs-1;
}

vector<CBomb> CBombManager::Update(float game_time)
{
	vector<CBomb> ret;
	ret.clear();
	int i;
	for(i = p_bombs_start; i != cnt_bombs % MAX_BOMBS; i = (i+1)%MAX_BOMBS)
	{
		if(bombs[i].second == false)
			continue;

		bombs[i].first.DropTime(game_time);
		if(bombs[i].first.GetRemainTime() <= 0)
		{
			ret.push_back(bombs[i].first);
			bombs[i].second = false;
			if(i==p_bombs_start)
				p_bombs_start = (p_bombs_start + 1) % MAX_BOMBS;
		}
	}
	//OutputDebugPrintf("Pbombstart: %d\n", p_bombs_start);
	//OutputDebugPrintf("Cnt: %d\n", cnt_bombs);

	return ret;
}

void CBombManager::Init()
{
	cnt_bombs = 0; 
	p_bombs_start = 0;
}

void CBombManager::SuddenExplode( int index )
{
	ASSERT(index >= 0);

	bombs[index%MAX_BOMBS].first.SetRemainTime(0);
	return;
}

void CBombManager::ExplodeAllBy( int user )
{
	int i;
	for(i=p_bombs_start; i!=cnt_bombs; i=(i+1)%MAX_BOMBS)
	{
		if(bombs[i].second == true && bombs[i].first.Owner() == user)
		{
			SuddenExplode(i);
		}
	}
}

CBomb CBombManager::GetBomb( int index )
{
	ASSERT(index>=0 && bombs[index%MAX_BOMBS].second == true);

	return bombs[index%MAX_BOMBS].first;
}
