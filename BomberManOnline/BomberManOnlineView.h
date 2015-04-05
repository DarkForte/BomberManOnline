#include "Game.h"
#include "Lobby.h"
#include "GameState.h"
#include "Direct2DMFCBase.h"
#pragma once

// CChildView 窗口

class CBomberManOnlineView : public CWnd, public CDirect2DMFCBase
{
	float last_time;
	float now_time;

	CRect client_rect;
	CDC cacheDC;
	CBitmap cache_bitmap;

	GameState game_state;

	CGame *p_game;
	CLobby *p_lobby;
	CResourceManager *p_res_manager;

	//UINT render_timer_id;
// 构造
public:
	CBomberManOnlineView();

// 操作
public:
	void Init();

// 重写
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CBomberManOnlineView();

	// 生成的消息映射函数
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

	//static void OnNewTimer(UINT wTimerID, UINT msg,DWORD dwUser, DWORD dwl,DWORD dw2);

	HRESULT OnRender();
	HRESULT CreateDeviceResources();
};

