#include "stdafx.h"
#include "Room.h"
#include "D2D1Header.h"

using namespace std;

GameState ButtonDown_room_BEGIN()
{
	return GameState::INGAME;
}

GameState ButtonDown_room_CLOSE()
{
	PostMessage(NULL, WM_QUIT,0,0);
	return GameState::LOBBY;
}

CRoom::CRoom(CResourceManager* p_res_manager)
{
	this->p_res_manager = p_res_manager;
	button[1].Init(1150, 120, 130, 80, 1, 1, NULL);
	button[2].Init(1150, 220, 130, 80, 1, 1, NULL);
	button[3].Init(1150, 320, 130, 80, 1, 1, NULL);
	button[4].Init(1150, 420, 130, 80, 1, 1, NULL);
	button[5].Init(1150, 520, 130, 80, 1, 1, ButtonDown_room_CLOSE);
	button[6].Init(25, 211, 400, 105, 1, 1, NULL);
	button[7].Init(435, 211, 400, 105, 1, 1, NULL);
	button[8].Init(25, 326, 400, 105, 1, 1, NULL);
	button[9].Init(435, 326, 400, 105, 1, 1, NULL);
	button[10].Init(885, 211, 245, 127, 1, 1, ButtonDown_room_BEGIN);
	button[11].Init(885, 211, 245, 127, 1, 1, NULL);

	chat.Init(PointF(885, 582), "message", 245, 26, true, false, 10,true,188);

	isMessage = false;
	msg_button.Init(838, 365, 90, 30, 1, 1, NULL);
}

GameState CRoom::HandleLButtonDown(CPoint point)
{
	if (isMessage)
	{
		if (point.x >= msg_button.GetXPixel() &&
			point.x <= msg_button.GetXPixel() + msg_button.GetWidth() &&
			point.y >= msg_button.GetYPixel() &&
			point.y <= msg_button.GetYPixel() + msg_button.GetHeight())
		{
			msg_button.SetStatus(BUTTON_STATUS::MOUSE_DOWN);
		}
	}
	else
	{
		for (int i = 1; i <= LOBBY_MAX_BUTTON; i++)
		{
			if (point.x >= button[i].GetXPixel() &&
				point.x <= button[i].GetXPixel() + button[i].GetWidth() &&
				point.y >= button[i].GetYPixel() &&
				point.y <= button[i].GetYPixel() + button[i].GetHeight())
			{
				button[i].SetStatus(BUTTON_STATUS::MOUSE_DOWN);
			}
		}
	}
	return GameState::LOBBY;
}

GameState CRoom::HandleLButtonMove(CPoint point)
{
	if (isMessage)
	{
		if (point.x >= msg_button.GetXPixel() &&
			point.x <= msg_button.GetXPixel() + msg_button.GetWidth() &&
			point.y >= msg_button.GetYPixel() &&
			point.y <= msg_button.GetYPixel() + msg_button.GetHeight())
		{
			msg_button.SetStatus(BUTTON_STATUS::MOUSE_ON);
		}
		else
		{
			msg_button.SetStatus(BUTTON_STATUS::IDLE);
		}
	}
	else
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
	}
	return GameState::LOBBY;
}

GameState CRoom::HandleLButtonUp(CPoint point)
{
	GameState state = GameState::LOBBY;
	
	if (isMessage)
	{
		if (point.x >= msg_button.GetXPixel() &&
			point.x <= msg_button.GetXPixel() + msg_button.GetWidth() &&
			point.y >= msg_button.GetYPixel() &&
			point.y <= msg_button.GetYPixel() + msg_button.GetHeight())
		{
			isMessage = false;
		}
	}
	else
	{
		for (int i = 1; i <= LOBBY_MAX_BUTTON; i++)
		{
			if (point.x >= button[i].GetXPixel() &&
				point.x <= button[i].GetXPixel() + button[i].GetWidth() &&
				point.y >= button[i].GetYPixel() &&
				point.y <= button[i].GetYPixel() + button[i].GetHeight())
			{
				if (i == 4)
				{
					isMessage = true;
					msg_string = "Help Message";
				}
				else if (button[i].ButtonDown != NULL)
				{
					state = button[i].ButtonDown();
				}
			}
		}
	}
	return state;
}

void CRoom::RenderText(ID2D1HwndRenderTarget* render_target, wstring str, int x, int y,
	IDWriteTextFormat* format, ID2D1SolidColorBrush* brush)
{
	int len = str.length();
	D2D1_RECT_F rect = D2D1::RectF(x, y, x + format->GetFontSize()*len, y + format->GetFontSize()*len);
	render_target->DrawText(str.c_str(), str.length(), format, rect, brush);
}

void CRoom::Render(ID2D1HwndRenderTarget* render_target)
{
	ID2D1SolidColorBrush* brush;
	render_target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &brush);

	p_res_manager->room_ui.DrawImage(render_target, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0);

	for (int i = 1; i <= LOBBY_MAX_BUTTON; i++)
	{
		if (button[i].GetStatus() == BUTTON_STATUS::IDLE)
		{
			p_res_manager->room_button_sprite[i][0].DrawImage(render_target,
				button[i].GetXPixel(), button[i].GetYPixel(),
				button[i].GetWidth(), button[i].GetHeight(),
				0, 0,
				button[i].GetXScale(), button[i].GetYScale());
		}
		else if (button[i].GetStatus() == BUTTON_STATUS::MOUSE_ON)
		{
			p_res_manager->room_button_sprite[i][1].DrawImage(render_target,
				button[i].GetXPixel(), button[i].GetYPixel(),
				button[i].GetWidth(), button[i].GetHeight(),
				0, 0,
				button[i].GetXScale(), button[i].GetYScale());
		}
		else if (button[i].GetStatus() == BUTTON_STATUS::MOUSE_DOWN)
		{
			p_res_manager->room_button_sprite[i][2].DrawImage(render_target,
				button[i].GetXPixel(), button[i].GetYPixel(),
				button[i].GetWidth(), button[i].GetHeight(),
				0, 0,
				button[i].GetXScale(), button[i].GetYScale());
		}
	}

	chat.Draw(render_target, p_res_manager);

	//white background
	p_res_manager->room_icon_sprite[1].DrawImage(render_target, 25, 211, 105, 105, 0, 0);
	p_res_manager->room_icon_sprite[1].DrawImage(render_target, 435, 211, 105, 105, 0, 0);
	p_res_manager->room_icon_sprite[1].DrawImage(render_target, 25, 326, 105, 105, 0, 0);
	p_res_manager->room_icon_sprite[1].DrawImage(render_target, 435, 326, 105, 105, 0, 0);
	//user image
	p_res_manager->room_icon_sprite[2].DrawImage(render_target, 786, 56, 64, 64, 0, 0);
	//player icon
	p_res_manager->room_icon_sprite[2].DrawImage(render_target, 43, 230, 67, 67, 0, 0);
	p_res_manager->room_icon_sprite[2].DrawImage(render_target, 453, 230, 67, 67, 0, 0);
	p_res_manager->room_icon_sprite[2].DrawImage(render_target, 43, 345, 67, 67, 0, 0);
	p_res_manager->room_icon_sprite[2].DrawImage(render_target, 453, 345, 67, 67, 0, 0);

	wstring o_text;
	CString text;
	ID2D1SolidColorBrush* brush_black;
	render_target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &brush_black);

	//lobby text
	text.Format(L"ROOM");
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

	//player number text
	text.Format(L"01");
	o_text = text.GetString();
	RenderText(render_target, o_text, 141, 222, p_res_manager->p_text_format_Arial_32_bold, brush);

	text.Format(L"02");
	o_text = text.GetString();
	RenderText(render_target, o_text, 551, 222, p_res_manager->p_text_format_Arial_32_bold, brush);

	text.Format(L"03");
	o_text = text.GetString();
	RenderText(render_target, o_text, 141, 337, p_res_manager->p_text_format_Arial_32_bold, brush);

	text.Format(L"04");
	o_text = text.GetString();
	RenderText(render_target, o_text, 551, 337, p_res_manager->p_text_format_Arial_32_bold, brush);

	//player name text
	text.Format(L"PLAYER NAME");
	o_text = text.GetString();
	RenderText(render_target, o_text, 187, 222, p_res_manager->p_text_format_Arial_32_bold, brush);

	text.Format(L"PLAYER NAME");
	o_text = text.GetString();
	RenderText(render_target, o_text, 597, 222, p_res_manager->p_text_format_Arial_32_bold, brush);

	text.Format(L"PLAYER NAME");
	o_text = text.GetString();
	RenderText(render_target, o_text, 187, 337, p_res_manager->p_text_format_Arial_32_bold, brush);

	text.Format(L"PLAYER NAME");
	o_text = text.GetString();
	RenderText(render_target, o_text, 597, 337, p_res_manager->p_text_format_Arial_32_bold, brush);

	//ready
	text.Format(L"READY");
	o_text = text.GetString();
	RenderText(render_target, o_text, 938, 251, p_res_manager->p_text_format_Arial_40_bold, brush);

	SafeRelease(&brush);

	if (isMessage)
	{
		RenderMessage(render_target);
	}

	return;
}

void CRoom::HandleKeyDown(UINT nchar)
{
	chat.HandleKeyDown(nchar);
}