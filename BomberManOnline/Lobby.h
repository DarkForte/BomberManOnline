#pragma once
#include "SceneBase.h"
#include "GameState.h"

class CLobby : public CSceneBase
{
public:
	CLobby();
	CLobby(CResourceManager* p_res_manager);
	
	GameState HandleLButtonDown(UINT nFlags, CPoint point);
	void Render(CDC* pDC);

};

