
// ChildView.cpp : CChildView 类的实现
//

#include "stdafx.h"
#include "BomberManOnlineView.h"
#include "Game.h"
#include "Login.h"

void CBomberManOnlineView::Init()
{
	p_res_manager = new CResourceManager();

	p_game = new CGame(p_res_manager);
	p_login = new CLogin(p_res_manager);
	p_lobby = new CLobby(p_res_manager);
	p_room = new CRoom(p_res_manager);
	game_state = LOGIN;

	return;
}

CBomberManOnlineView::CBomberManOnlineView()
{
	
}

CBomberManOnlineView::~CBomberManOnlineView()
{
	
}

void CBomberManOnlineView::OnLButtonDown(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(game_state == LOGIN)
	{
		GameState next_state = p_login->HandleLButtonDown(point);
	}
	else if (game_state == LOBBY)
	{
		GameState next_state = p_lobby->HandleLButtonDown(point);
	}
	else if (game_state == ROOM)
	{
		GameState next_state = p_room->HandleLButtonDown(point);
	}
}

void CBomberManOnlineView::OnLButtonUp(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (game_state == LOGIN)
	{
		GameState next_state = p_login->HandleLButtonUp(point);
		if (next_state == LOBBY)
		{
			game_state = LOBBY;
		}

	}
	else if (game_state == LOBBY)
	{
		GameState next_state = p_lobby->HandleLButtonUp(point);
		if (next_state == ROOM)
		{
			game_state = ROOM;
		}

	}
	else if (game_state == ROOM)
	{
		GameState next_state = p_room->HandleLButtonUp(point);
		if (next_state == INGAME)
		{
			p_game->Init(1,1);
			game_state = INGAME;
		}

	}
}

void CBomberManOnlineView::OnLButtonMove(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (game_state == LOGIN)
	{
		GameState next_state = p_login->HandleLButtonMove(point);
	}
	else if (game_state == LOBBY)
	{
		GameState next_state = p_lobby->HandleLButtonMove(point);
	}
	else if (game_state == ROOM)
	{
		GameState next_state = p_room->HandleLButtonMove(point);
	}
}


void CBomberManOnlineView::OnKeyDown(UINT nChar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (game_state == LOGIN)
	{
		p_login->HandleKeyDown(nChar);
	}
	else if (game_state == LOBBY)
	{
		p_lobby->HandleKeyDown(nChar);
	}
	else if (game_state == ROOM)
	{
		p_room->HandleKeyDown(nChar);
	}
	else if(game_state == INGAME)
	{
		p_game->HandleKeyDown(nChar);
	}
}

void CBomberManOnlineView::OnKeyUp(UINT nChar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if(game_state == INGAME)
	{
		p_game->HandleKeyUp(nChar);
	}
}


int CBomberManOnlineView::OnCreate()
{
	Init();
	InitializeD2D();
	CreateDeviceResources();

	return 0;
}

HRESULT CBomberManOnlineView::OnRender()
{
	HRESULT hr = S_OK;
	HWND hwnd = GetHwnd();
	GetClientRect(hwnd, &client_rect);

	//hr = CreateDeviceResources();
	ID2D1HwndRenderTarget* render_target = CDirect2DMFCBase::GetRenderTarget();

	if (SUCCEEDED(hr))
	{
		render_target->BeginDraw();
		render_target->SetTransform(D2D1::Matrix3x2F::Identity());
		render_target->Clear(D2D1::ColorF(D2D1::ColorF::Black));

		D2D1_SIZE_F rtSize = render_target->GetSize();

		if(game_state == GameState::LOGIN)
		{
			p_login->Render(render_target);
		}
		else if (game_state == GameState::LOBBY)
		{
			p_lobby->Render(render_target);
		}
		else if(game_state == GameState::INGAME)
		{
			p_game->Render(render_target);
		}
		else if (game_state == GameState::ROOM)
		{
			p_room->Render(render_target);
		}

		hr = render_target->EndDraw();
	}
	ValidateRect(hwnd, client_rect);

	if (hr == D2DERR_RECREATE_TARGET)
	{
		hr = S_OK;
		DiscardDeviceResources();
	}

	return hr;
}

HRESULT CBomberManOnlineView::CreateDeviceResources()
{
	HRESULT hr = CDirect2DMFCBase::CreateDeviceResources();

	p_res_manager->LoadPics(CDirect2DMFCBase::GetWICImagingFactory(), CDirect2DMFCBase::GetRenderTarget());
	p_res_manager->InitTextFormat(GetWriteFactory());

	return hr;
}

void CBomberManOnlineView::Update(float game_time)
{
	if(game_state == INGAME)
	{
		GameState next_gamestate = p_game->Update(game_time);
		game_state = next_gamestate;
	}
	
}



void CBomberManOnlineView::OnDestroy()
{
	// TODO: 在此处添加消息处理程序代码

	delete(p_res_manager);
	delete(p_login);
	delete(p_game);
}
