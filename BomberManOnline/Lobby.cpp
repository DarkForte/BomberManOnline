#include "stdafx.h"
#include "Lobby.h"

CLobby::CLobby( CResourceManager* p_res_manager )
{
	this->p_res_manager = p_res_manager;
}

GameState CLobby::HandleLButtonDown( UINT nFlags, CPoint point )
{
	return INGAME;
}

void CLobby::Render(CDC* pDC)
{
	pDC->TextOut(100,100,L"µã»÷¿ªÊ¼");

	return;
}
