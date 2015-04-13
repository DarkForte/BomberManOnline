#pragma once
#include "SceneBase.h"
#include "GameState.h"
#include "D2D1Header.h"
#include "Button.h"
#include "Edit.h"

class CLobby : public CSceneBase
{
	Button button[LOBBY_MAX_BUTTON + 1];
	CEdit chat;
public:
	CLobby();
	CLobby(CResourceManager* p_res_manager);
	
	GameState HandleLButtonDown(CPoint point);
	GameState HandleLButtonUp(CPoint point);
	GameState HandleLButtonMove(CPoint point);
	void Render(ID2D1HwndRenderTarget* render_target);
	void RenderText(ID2D1HwndRenderTarget* render_target, wstring str, int x, int y,
		IDWriteTextFormat* format, ID2D1SolidColorBrush* brush);
	void HandleKeyDown(UINT nchar);
};

