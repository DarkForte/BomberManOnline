#include "Game.h"

#pragma once

// CChildView ����

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

// ����
public:
	CBomberManOnlineView();

// ����
public:
	void Init();
	void LobbyRender(CDC *pDC);
	void GameRender(CDC *pDC);

// ��д
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CBomberManOnlineView();

	// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

