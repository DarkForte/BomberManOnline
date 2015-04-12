#include "Game.h"
#include "Login.h"
#include "Lobby.h"
#include "Room.h"
#include "GameState.h"
#include "Direct2DMFCBase.h"
#pragma once

class CBomberManOnlineView : public CDirect2DMFCBase
{


	float last_time;
	float now_time;

	CRect client_rect;

	GameState game_state;

	CGame *p_game;
	CLogin *p_login;
	CLobby *p_lobby;
	CRoom *p_room;
	CResourceManager *p_res_manager;

	UINT render_timer_id;
// 构造
public:
	CBomberManOnlineView();

// 操作
public:
	void Init();

// 实现
public:
	virtual ~CBomberManOnlineView();

public:
	void OnLButtonDown(CPoint point);
	void OnLButtonUp(CPoint point);
	void OnLButtonMove(CPoint point);
	void OnKeyDown(UINT nChar);
	int OnCreate();
	void OnTimer(UINT_PTR nIDEvent);
	void OnKeyUp(UINT nChar);

	void Update(float game_time);

	HRESULT OnRender();
	HRESULT CreateDeviceResources();
	void OnDestroy();
};

