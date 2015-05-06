#pragma once
#include "SceneBase.h"
#include "GameState.h"
#include "D2D1Header.h"
#include "Button.h"
#include "Edit.h"
#include "RenderText.h"

class CRoom : public CSceneBase
{
	Button button[LOBBY_MAX_BUTTON + 1];
	CEdit chat;
public:
	CRoom();
	CRoom(CResourceManager* p_res_manager);
	
	GameState HandleLButtonDown(CPoint point);
	GameState HandleLButtonUp(CPoint point);
	GameState HandleLButtonMove(CPoint point);
	GameState Update();
	void Render(ID2D1HwndRenderTarget* render_target);
	
	void HandleKeyDown(UINT nchar);
};

