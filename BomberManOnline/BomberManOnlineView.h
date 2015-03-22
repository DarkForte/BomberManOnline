#include "Game.h"

#pragma once

// CChildView 窗口

enum GameState
{
	LOBBY,INGAME
};

class CBomberManOnlineView : public CWnd
{
	CRect client_rect;
	CDC cacheDC;
	CBitmap cache_bitmap;

	GameState game_state;

	CGame game;

// 构造
public:
	CBomberManOnlineView();

// 操作
public:
	void Init();
	void LobbyRender(CDC *pDC);
	void GameRender(CDC *pDC);

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
};

