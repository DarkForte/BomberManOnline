#include "stdafx.h"
#include "Lobby.h"
#include "D2D1Header.h"
using namespace std;

CLobby::CLobby( CResourceManager* p_res_manager )
{
	this->p_res_manager = p_res_manager;
}

GameState CLobby::HandleLButtonDown( UINT nFlags, CPoint point )
{
	return INGAME;
}

void RenderText(ID2D1HwndRenderTarget* render_target, wstring str, int x, int y, 
				IDWriteTextFormat* format, ID2D1SolidColorBrush* brush)
{
	int len = str.length();
	D2D1_RECT_F rect = D2D1::RectF(x,y, x+format->GetFontSize()*len, y+format->GetFontSize()*len);
	render_target->DrawText(str.c_str(), str.length(), format, rect, brush);
}

void CLobby::Render(ID2D1HwndRenderTarget* render_target)
{
	ID2D1SolidColorBrush* brush;
	render_target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &brush);
	RenderText(render_target, L"µã»÷¿ªÊ¼", 100, 100, p_res_manager->p_text_format, brush);

	SafeRelease(&brush);
	return;
}
