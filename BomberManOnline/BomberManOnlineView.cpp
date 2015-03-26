
// ChildView.cpp : CChildView 类的实现
//

#include "stdafx.h"
#include "mmsystem.h"
#include "BomberManOnline.h"
#include "BomberManOnlineView.h"
#pragma comment(lib, "winmm.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


void CBomberManOnlineView::Init()
{
	p_res_manager = new CResourceManager();
	p_game = new CGame(p_res_manager);
	p_game->Init(1);

	game_state = LOBBY;

	return;
}

CBomberManOnlineView::CBomberManOnlineView()
{
	
}

CBomberManOnlineView::~CBomberManOnlineView()
{
	delete(p_res_manager);
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



// CChildView 消息处理程序

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

void CBomberManOnlineView::LobbyRender(CDC *pDC)
{
	pDC->TextOut(100,100,L"点击开始");

	return;
}

void CBomberManOnlineView::OnPaint() 
{
	CDC *pDC = GetDC();

	GetClientRect(&client_rect);
	cacheDC.CreateCompatibleDC(NULL);
	cache_bitmap.CreateCompatibleBitmap(pDC, client_rect.Width(), client_rect.Height());
	cacheDC.SelectObject(&cache_bitmap);
	// TODO: 在此处添加消息处理程序代码
	
	if(game_state == LOBBY)
	{
		LobbyRender(&cacheDC);
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

	// 不要为绘制消息而调用 CWnd::OnPaint()
}



void CBomberManOnlineView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(game_state == LOBBY)
	{
		game_state = INGAME;
		Invalidate();
	}


	CWnd::OnLButtonDown(nFlags, point);
}


void CBomberManOnlineView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(game_state == INGAME)
	{
		p_game->HandleKeyDown(nChar);
	}


	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CBomberManOnlineView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if(nIDEvent == TIMER_RENDER)
	{
		OnPaint();
		if(game_state == INGAME)
		{
			now_time = timeGetTime();
			//OutputDebugPrintf("%lf\n", now_time - last_time);
			p_game->Update(now_time - last_time);
		}
	}
	last_time = timeGetTime();
	//CWnd::OnTimer(nIDEvent);
}


