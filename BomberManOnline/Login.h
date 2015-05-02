#pragma once
#include "SceneBase.h"
#include "GameState.h"
#include "D2D1Header.h"
#include "Button.h"
#include "Edit.h"
#include "Client.h"
#pragma comment(lib,"ws2_32.lib")
#include "RenderText.h"

class CLogin : public CSceneBase
{
private:
	Button button[LOGIN_MAX_BUTTON + 1];
	CEdit user_name;
	CEdit user_password;
	
public:
	CLogin();
	CLogin(CResourceManager* p_res_manager);
	
	GameState HandleLButtonDown(CPoint point);
	GameState HandleLButtonUp(CPoint point);
	GameState HandleLButtonMove(CPoint point);
	void HandleKeyDown(UINT nchar);
	void Render(ID2D1HwndRenderTarget* render_target);

};

