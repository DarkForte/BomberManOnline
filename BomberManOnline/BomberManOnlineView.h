#include "Game.h"

#pragma once

// CChildView ����

enum GameState
{
	LOBBY,INGAME
};

class CBomberManOnlineView : public CWnd
{
	float last_time;
	float now_time;

	CRect client_rect;
	CDC cacheDC;
	CBitmap cache_bitmap;

	GameState game_state;

	CGame *p_game;
	CResourceManager *p_res_manager;


// ����
public:
	CBomberManOnlineView();

// ����
public:
	void Init();
	void LobbyRender(CDC *pDC);

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
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
};

