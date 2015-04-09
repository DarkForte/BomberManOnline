#pragma once
#include "SceneBase.h"
#include "GameState.h"
#include "D2D1Header.h"
#include "Button.h"

class CLogin : public CSceneBase
{
	Button button[LOBBY_MAX_BUTTON + 1];
public:
	CLogin();
	CLogin(CResourceManager* p_res_manager);
	
	GameState HandleLButtonDown(CPoint point);
	GameState HandleLButtonUp(CPoint point);
	GameState HandleLButtonMove(CPoint point);
	void Render(ID2D1HwndRenderTarget* render_target);

};

