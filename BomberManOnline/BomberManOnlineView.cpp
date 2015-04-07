
// ChildView.cpp : CChildView 类的实现
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

	render_timer_id = 12121;

	return;
}

CBomberManOnlineView::CBomberManOnlineView()
{
	
}

CBomberManOnlineView::~CBomberManOnlineView()
{
	
}


BEGIN_MESSAGE_MAP(CBomberManOnlineView, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_KEYUP()
	ON_WM_DESTROY()
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

	return TRUE;
}

void CBomberManOnlineView::OnPaint() 
{
	OnRender();

	// 不要为绘制消息而调用 CWnd::OnPaint()
}



void CBomberManOnlineView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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

	Init();
	InitializeD2D();
	CreateDeviceResources();

	//SetTimer(TIMER_RENDER, 1000/MAX_FPS, NULL);
	render_timer_id = timeSetEvent(1000/MAX_FPS, 1, (LPTIMECALLBACK)OnMMTimer, (DWORD)this, TIME_PERIODIC);
	last_time = timeGetTime();
}


void CBomberManOnlineView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	/*if(nIDEvent == TIMER_RENDER)
	{
		OnPaint();
		now_time = timeGetTime();
		if(game_state == INGAME)
		{
			OutputDebugPrintf("%lf\n", now_time - last_time);
			GameState next_gamestate = p_game->Update(now_time - last_time);
			game_state = next_gamestate;
		}
		last_time = timeGetTime();
	}*/
	//CWnd::OnTimer(nIDEvent);
}

HRESULT CBomberManOnlineView::OnRender()
{
	HRESULT hr = S_OK;
	GetClientRect(&client_rect);

	//hr = CreateDeviceResources();
	ID2D1HwndRenderTarget* render_target = CDirect2DMFCBase::GetRenderTarget();

	if (SUCCEEDED(hr))
	{
		render_target->BeginDraw();
		render_target->SetTransform(D2D1::Matrix3x2F::Identity());
		render_target->Clear(D2D1::ColorF(D2D1::ColorF::Black));

		D2D1_SIZE_F rtSize = render_target->GetSize();

		if(game_state == GameState::LOBBY)
		{
			p_lobby->Render(render_target);
		}
		else if(game_state == GameState::INGAME)
		{
			p_game->Render(render_target);
		}

		hr = render_target->EndDraw();
	}
	ValidateRect(client_rect);

	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		DiscardDeviceResources();
	}

	return hr;
}

HRESULT CBomberManOnlineView::CreateDeviceResources()
{
	HWND hwnd = AfxGetMainWnd()->m_hWnd;
	SetHwnd(hwnd);
	
	HRESULT hr = CDirect2DMFCBase::CreateDeviceResources();

	p_res_manager->LoadPics(CDirect2DMFCBase::GetWICImagingFactory(), CDirect2DMFCBase::GetRenderTarget());
	p_res_manager->InitTextFormat(GetWriteFactory());

	return hr;
}

void CBomberManOnlineView::OnMMTimer( UINT wTimerID, UINT msg,DWORD dwUser, DWORD dwl,DWORD dw2 )
{
	//OnPaint();
	CBomberManOnlineView * view = (CBomberManOnlineView *)dwUser;
	view->OnPaint();
	view->now_time = timeGetTime();
	
	if(view->game_state == INGAME)
	{
		OutputDebugPrintf("%lf\n", view->now_time - view->last_time);
		GameState next_gamestate = view->p_game->Update(view->now_time - view->last_time);
		view->game_state = next_gamestate;
	}
	view->last_time = timeGetTime();
	
}



void CBomberManOnlineView::OnDestroy()
{
	__super::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	timeKillEvent(render_timer_id);

	delete(p_res_manager);
	delete(p_lobby);
	delete(p_game);
}
