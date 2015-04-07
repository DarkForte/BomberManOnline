#pragma once
#include "SceneBase.h"
#include "GameState.h"
#include "D2D1Header.h"

class CLobby : public CSceneBase
{
public:
	CLobby();
	CLobby(CResourceManager* p_res_manager);
	
	GameState HandleLButtonDown(CPoint point);
	void Render(ID2D1HwndRenderTarget* render_target);

};

