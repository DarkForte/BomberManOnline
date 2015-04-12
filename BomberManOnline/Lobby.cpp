#include "stdafx.h"
#include "Lobby.h"
#include "D2D1Header.h"

using namespace std;

GameState ButtonDown_lobby_ROOM()
{
	return GameState::ROOM;
}

GameState ButtonDown_lobby_CLOSE()
{
	PostMessage(NULL, WM_QUIT,0,0);
	return GameState::LOBBY;
}

CLobby::CLobby(CResourceManager* p_res_manager)
{
	this->p_res_manager = p_res_manager;
	button[1].Init(1150, 120, 130, 80, 1, 1, NULL);
	button[2].Init(1150, 220, 130, 80, 1, 1, NULL);
	button[3].Init(1150, 320, 130, 80, 1, 1, NULL);
	button[4].Init(1150, 420, 130, 80, 1, 1, NULL);
	button[5].Init(1150, 520, 130, 80, 1, 1, ButtonDown_lobby_CLOSE);
	button[6].Init(25, 211, 400, 105, 1, 1, ButtonDown_lobby_ROOM);
	button[7].Init(435, 211, 400, 105, 1, 1, ButtonDown_lobby_ROOM);
	button[8].Init(25, 326, 400, 105, 1, 1, ButtonDown_lobby_ROOM);
	button[9].Init(435, 326, 400, 105, 1, 1, ButtonDown_lobby_ROOM);
	button[10].Init(25, 441, 400, 105, 1, 1, ButtonDown_lobby_ROOM);
	button[11].Init(435, 441, 400, 105, 1, 1, ButtonDown_lobby_ROOM);
	button[12].Init(25, 557, 400, 105, 1, 1, ButtonDown_lobby_ROOM);
	button[13].Init(435, 557, 400, 105, 1, 1, ButtonDown_lobby_ROOM);

	chat.Init(PointF(885, 635), "message", 245, 26, true, false, 10,true,424);
}

GameState CLobby::HandleLButtonDown(CPoint point)
{
	for (int i = 1; i <= LOBBY_MAX_BUTTON; i++)
	{
		if (point.x >= button[i].GetXPixel()&&
			point.x <= button[i].GetXPixel() + button[i].GetWidth() &&
			point.y >= button[i].GetYPixel() &&
			point.y <= button[i].GetYPixel() + button[i].GetHeight())
		{
			button[i].SetStatus(BUTTON_STATUS::MOUSE_DOWN);
		}
	}
	
	chat.HandleLButtonDown(point);

	return GameState::LOBBY;
}

GameState CLobby::HandleLButtonMove(CPoint point)
{
	for (int i = 1; i <= LOBBY_MAX_BUTTON; i++)
	{
		if (point.x >= button[i].GetXPixel() &&
			point.x <= button[i].GetXPixel() + button[i].GetWidth() &&
			point.y >= button[i].GetYPixel() &&
			point.y <= button[i].GetYPixel() + button[i].GetHeight())
		{
			button[i].SetStatus(BUTTON_STATUS::MOUSE_ON);
		}
		else
		{
			button[i].SetStatus(BUTTON_STATUS::IDLE);
		}
	}

	return GameState::LOBBY;
}

GameState CLobby::HandleLButtonUp(CPoint point)
{
	GameState state = GameState::LOBBY;
	for (int i = 1; i <= LOBBY_MAX_BUTTON; i++)
	{
		if (point.x >= button[i].GetXPixel()&&
			point.x <= button[i].GetXPixel() + button[i].GetWidth() &&
			point.y >= button[i].GetYPixel()&&
			point.y <= button[i].GetYPixel() + button[i].GetHeight())
		{
			if (button[i].ButtonDown != NULL)
			{
				state = button[i].ButtonDown();
			}
		}
	}

	return state;
}

void CLobby::RenderText(ID2D1HwndRenderTarget* render_target, wstring str, int x, int y,
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

	p_res_manager->lobby_ui.DrawImage(render_target, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0);

	for (int i = 1; i <= LOBBY_MAX_BUTTON; i++)
	{
		if (button[i].GetStatus() == BUTTON_STATUS::IDLE)
		{
			p_res_manager->lobby_button_sprite[i][0].DrawImage(render_target,
				button[i].GetXPixel(), button[i].GetYPixel(),
				button[i].GetWidth(), button[i].GetHeight(),
				0, 0,
				button[i].GetXScale(), button[i].GetYScale());
		}
		else if (button[i].GetStatus() == BUTTON_STATUS::MOUSE_ON)
		{
			p_res_manager->lobby_button_sprite[i][1].DrawImage(render_target,
				button[i].GetXPixel(), button[i].GetYPixel(),
				button[i].GetWidth(), button[i].GetHeight(),
				0, 0,
				button[i].GetXScale(), button[i].GetYScale());
		}
		else if (button[i].GetStatus() == BUTTON_STATUS::MOUSE_DOWN)
		{
			p_res_manager->lobby_button_sprite[i][2].DrawImage(render_target,
				button[i].GetXPixel(), button[i].GetYPixel(),
				button[i].GetWidth(), button[i].GetHeight(),
				0, 0,
				button[i].GetXScale(), button[i].GetYScale());
		}
	}

	chat.Draw(render_target, p_res_manager);

	//white background
	p_res_manager->lobby_icon_sprite[1].DrawImage(render_target, 25, 211, 105, 105, 0, 0);
	p_res_manager->lobby_icon_sprite[1].DrawImage(render_target, 435, 211, 105, 105, 0, 0);
	p_res_manager->lobby_icon_sprite[1].DrawImage(render_target, 25, 326, 105, 105, 0, 0);
	p_res_manager->lobby_icon_sprite[1].DrawImage(render_target, 435, 326, 105, 105, 0, 0);
	p_res_manager->lobby_icon_sprite[1].DrawImage(render_target, 25, 441, 105, 105, 0, 0);
	p_res_manager->lobby_icon_sprite[1].DrawImage(render_target, 435, 441, 105, 105, 0, 0);
	p_res_manager->lobby_icon_sprite[1].DrawImage(render_target, 25, 557, 105, 105, 0, 0);
	p_res_manager->lobby_icon_sprite[1].DrawImage(render_target, 435, 557, 105, 105, 0, 0);
	//user image
	p_res_manager->lobby_icon_sprite[2].DrawImage(render_target, 786, 56, 64, 64, 0, 0);
	//map icon
	p_res_manager->lobby_icon_sprite[3].DrawImage(render_target, 43, 230, 67, 67, 0, 0);
	p_res_manager->lobby_icon_sprite[3].DrawImage(render_target, 453, 230, 67, 67, 0, 0);
	p_res_manager->lobby_icon_sprite[3].DrawImage(render_target, 43, 345, 67, 67, 0, 0);
	p_res_manager->lobby_icon_sprite[3].DrawImage(render_target, 453, 345, 67, 67, 0, 0);
	p_res_manager->lobby_icon_sprite[3].DrawImage(render_target, 43, 460, 67, 67, 0, 0);
	p_res_manager->lobby_icon_sprite[3].DrawImage(render_target, 453, 460, 67, 67, 0, 0);
	p_res_manager->lobby_icon_sprite[3].DrawImage(render_target, 43, 576, 67, 67, 0, 0);
	p_res_manager->lobby_icon_sprite[3].DrawImage(render_target, 453, 576, 67, 67, 0, 0);

	wstring o_text;
	CString text;
	ID2D1SolidColorBrush* brush_black;
	render_target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &brush_black);

	//lobby text
	text.Format(L"LOBBY");
	o_text = text.GetString();
	RenderText(render_target, o_text, 115, 48, p_res_manager->p_text_format_Arial_72_bold, brush);

	//user details
	text.Format(L"USER NAME");
	o_text = text.GetString();
	RenderText(render_target, o_text, 860, 51, p_res_manager->p_text_format_Arial_32_bold, brush);

	text.Format(L"RANK");
	o_text = text.GetString();
	RenderText(render_target, o_text, 860, 88, p_res_manager->p_text_format_Arial_32_bold, brush);

	text.Format(L"MONEY");
	o_text = text.GetString();
	RenderText(render_target, o_text, 998, 88, p_res_manager->p_text_format_Arial_32_bold, brush);

	//room number text
	text.Format(L"01");
	o_text = text.GetString();
	RenderText(render_target, o_text, 141, 222, p_res_manager->p_text_format_Arial_32_bold, brush);

	text.Format(L"02");
	o_text = text.GetString();
	RenderText(render_target, o_text, 551, 222, p_res_manager->p_text_format_Arial_32_bold, brush);

	text.Format(L"03");
	o_text = text.GetString();
	RenderText(render_target, o_text, 141, 336, p_res_manager->p_text_format_Arial_32_bold, brush);

	text.Format(L"04");
	o_text = text.GetString();
	RenderText(render_target, o_text, 551, 336, p_res_manager->p_text_format_Arial_32_bold, brush);

	text.Format(L"05");
	o_text = text.GetString();
	RenderText(render_target, o_text, 141, 451, p_res_manager->p_text_format_Arial_32_bold, brush);

	text.Format(L"06");
	o_text = text.GetString();
	RenderText(render_target, o_text, 551, 451, p_res_manager->p_text_format_Arial_32_bold, brush);

	text.Format(L"07");
	o_text = text.GetString();
	RenderText(render_target, o_text, 141, 568, p_res_manager->p_text_format_Arial_32_bold, brush);

	text.Format(L"08");
	o_text = text.GetString();
	RenderText(render_target, o_text, 551, 568, p_res_manager->p_text_format_Arial_32_bold, brush);

	//room name text
	text.Format(L"MAP NAME");
	o_text = text.GetString();
	RenderText(render_target, o_text, 222, 222, p_res_manager->p_text_format_Arial_32_bold, brush);

	text.Format(L"MAP NAME");
	o_text = text.GetString();
	RenderText(render_target, o_text, 618, 222, p_res_manager->p_text_format_Arial_32_bold, brush);

	text.Format(L"MAP NAME");
	o_text = text.GetString();
	RenderText(render_target, o_text, 222, 336, p_res_manager->p_text_format_Arial_32_bold, brush);

	text.Format(L"MAP NAME");
	o_text = text.GetString();
	RenderText(render_target, o_text, 618, 336, p_res_manager->p_text_format_Arial_32_bold, brush);

	text.Format(L"MAP NAME");
	o_text = text.GetString();
	RenderText(render_target, o_text, 208, 451, p_res_manager->p_text_format_Arial_32_bold, brush);

	text.Format(L"MAP NAME");
	o_text = text.GetString();
	RenderText(render_target, o_text, 618, 451, p_res_manager->p_text_format_Arial_32_bold, brush);

	text.Format(L"MAP NAME");
	o_text = text.GetString();
	RenderText(render_target, o_text, 208, 568, p_res_manager->p_text_format_Arial_32_bold, brush);

	text.Format(L"MAP NAME");
	o_text = text.GetString();
	RenderText(render_target, o_text, 618, 568, p_res_manager->p_text_format_Arial_32_bold, brush);

	//room detail
	text.Format(L"1/4");
	o_text = text.GetString();
	RenderText(render_target, o_text, 355, 265, p_res_manager->p_text_format_Arial_32_bold, brush);

	text.Format(L"0/4");
	o_text = text.GetString();
	RenderText(render_target, o_text, 765, 265, p_res_manager->p_text_format_Arial_32_bold, brush);

	text.Format(L"1/4");
	o_text = text.GetString();
	RenderText(render_target, o_text, 355, 380, p_res_manager->p_text_format_Arial_32_bold, brush);

	text.Format(L"0/4");
	o_text = text.GetString();
	RenderText(render_target, o_text, 765, 380, p_res_manager->p_text_format_Arial_32_bold, brush);

	text.Format(L"1/4");
	o_text = text.GetString();
	RenderText(render_target, o_text, 355, 495, p_res_manager->p_text_format_Arial_32_bold, brush);

	text.Format(L"0/4");
	o_text = text.GetString();
	RenderText(render_target, o_text, 765, 495, p_res_manager->p_text_format_Arial_32_bold, brush);

	text.Format(L"1/4");
	o_text = text.GetString();
	RenderText(render_target, o_text, 355, 611, p_res_manager->p_text_format_Arial_32_bold, brush);

	text.Format(L"0/4");
	o_text = text.GetString();
	RenderText(render_target, o_text, 765, 611, p_res_manager->p_text_format_Arial_32_bold, brush);

	SafeRelease(&brush);
	return;
}

void CLobby::HandleKeyDown(UINT nchar)
{
	chat.HandleKeyDown(nchar);
}