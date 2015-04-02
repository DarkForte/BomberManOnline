
// ChildView.cpp : CChildView ���ʵ��
//

#include "stdafx.h"
#include "mmsystem.h"
#include "BomberManOnline.h"
#include "BomberManOnlineView.h"
#include "Game.h"
#include "Lobby.h"
#pragma comment(lib, "winmm.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


void CBomberManOnlineView::Init()
{
	p_res_manager = new CResourceManager();

	p_game = new CGame(p_res_manager);
	p_lobby = new CLobby(p_res_manager);

	game_state = LOBBY;

	return;
}

CBomberManOnlineView::CBomberManOnlineView()
{
	
}

CBomberManOnlineView::~CBomberManOnlineView()
{
	delete(p_res_manager);
	delete(p_lobby);
	delete(p_game);
}


BEGIN_MESSAGE_MAP(CBomberManOnlineView, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_KEYUP()
END_MESSAGE_MAP()



// CChildView ��Ϣ�������

BOOL CBomberManOnlineView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);


	Init();
	return TRUE;
}

void CBomberManOnlineView::OnPaint() 
{
	CDC *pDC = GetDC();

	GetClientRect(&client_rect);
	cacheDC.CreateCompatibleDC(NULL);
	cache_bitmap.CreateCompatibleBitmap(pDC, client_rect.Width(), client_rect.Height());
	cacheDC.SelectObject(&cache_bitmap);
	// TODO: �ڴ˴������Ϣ����������
	
	if(game_state == LOBBY)
	{
		p_lobby->Render(&cacheDC);
	}
	else if(game_state == INGAME)
	{
		p_game->Render(&cacheDC);
	}


	pDC->BitBlt(0, 0, client_rect.Width(), client_rect.Height(), &cacheDC,0,0,SRCCOPY);  

	ValidateRect(client_rect);
	cache_bitmap.DeleteObject();
	cacheDC.DeleteDC();
	ReleaseDC(pDC);

	// ��ҪΪ������Ϣ������ CWnd::OnPaint()
}



void CBomberManOnlineView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(game_state == LOBBY)
	{
		GameState next_state = p_lobby->HandleLButtonDown(nFlags, point);
		if(next_state == INGAME)
		{
			p_game->Init(1);
		}
		game_state = INGAME;
	}


	//CWnd::OnLButtonDown(nFlags, point);
}


void CBomberManOnlineView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(game_state == INGAME)
	{
		p_game->HandleKeyDown(nChar);
	}


	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CBomberManOnlineView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(game_state == INGAME)
	{
		p_game->HandleKeyUp(nChar);
	}
	CWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}


int CBomberManOnlineView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetTimer(TIMER_RENDER, 1000/MAX_FPS, NULL);
	last_time = timeGetTime();
}


void CBomberManOnlineView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if(nIDEvent == TIMER_RENDER)
	{
		OnPaint();
		now_time = timeGetTime();
		if(game_state == INGAME)
		{
			//OutputDebugPrintf("%lf\n", now_time - last_time);
			GameState next_gamestate = p_game->Update(now_time - last_time);
			game_state = next_gamestate;
		}
		last_time = timeGetTime();
	}
	//CWnd::OnTimer(nIDEvent);
}


