#include "stdafx.h"
#include "Login.h"
#include "D2D1Header.h"

using namespace std;

GameState ButtonDown_Login_OK()
{
	return GameState::LOBBY;
}

GameState ButtonDown_Login_CLOSE()
{
	PostMessage(NULL, WM_QUIT,0,0);
	return GameState::LOGIN;
}

CLogin::CLogin(CResourceManager* p_res_manager)
{
	this->p_res_manager = p_res_manager;

	//init button
	button[1].Init(1150, 120, 130, 80, 1, 1, NULL);
	button[2].Init(1150, 220, 130, 80, 1, 1, NULL);
	button[3].Init(1150, 320, 130, 80, 1, 1, NULL);
	button[4].Init(1150, 420, 130, 80, 1, 1, NULL);
	button[5].Init(1150, 520, 130, 80, 1, 1, &ButtonDown_Login_CLOSE);
	button[6].Init(738, 365, 90, 30, 1, 1, &ButtonDown_Login_OK);

	//init cedit
	user_name.Init(PointF(628, 296), "user name", 200, 26, true, false, 10, false, 0);
	user_password.Init(PointF(628, 331), "*****", 200, 26, false, true, 10, false, 0);
}

GameState CLogin::HandleLButtonDown(CPoint point)
{
	for (int i = 1; i <= LOGIN_MAX_BUTTON; i++)
	{
		if (point.x >= button[i].GetXPixel()&&
			point.x <= button[i].GetXPixel() + button[i].GetWidth() &&
			point.y >= button[i].GetYPixel() &&
			point.y <= button[i].GetYPixel() + button[i].GetHeight())
		{
			button[i].SetStatus(BUTTON_STATUS::MOUSE_DOWN);
		}
	}
	
	user_name.HandleLButtonDown(point);
	user_password.HandleLButtonDown(point);

	return GameState::LOGIN;
}

GameState CLogin::HandleLButtonMove(CPoint point)
{
	for (int i = 1; i <= LOGIN_MAX_BUTTON; i++)
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

	return GameState::LOGIN;
}

GameState CLogin::HandleLButtonUp(CPoint point)
{
	GameState state = GameState::LOGIN;
	for (int i = 1; i <= LOGIN_MAX_BUTTON; i++)
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

void RenderText(ID2D1HwndRenderTarget* render_target, wstring str, int x, int y, 
				IDWriteTextFormat* format, ID2D1SolidColorBrush* brush)
{
	int len = str.length();
	D2D1_RECT_F rect = D2D1::RectF(x,y, x+format->GetFontSize()*len, y+format->GetFontSize()*len);
	render_target->DrawText(str.c_str(), str.length(), format, rect, brush);
}

void CLogin::Render(ID2D1HwndRenderTarget* render_target)
{
	ID2D1SolidColorBrush* brush;
	render_target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &brush);

	p_res_manager->login_ui.DrawImage(render_target, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0);

	for (int i = 1; i <= LOGIN_MAX_BUTTON; i++)
	{
		if (button[i].GetStatus() == BUTTON_STATUS::IDLE)
		{
			p_res_manager->login_button_sprite[i][0].DrawImage(render_target,
				button[i].GetXPixel(), button[i].GetYPixel(),
				button[i].GetWidth(), button[i].GetHeight(),
				0, 0,
				button[i].GetXScale(), button[i].GetYScale());
		}
		else if (button[i].GetStatus() == BUTTON_STATUS::MOUSE_ON)
		{
			p_res_manager->login_button_sprite[i][1].DrawImage(render_target,
				button[i].GetXPixel(), button[i].GetYPixel(),
				button[i].GetWidth(), button[i].GetHeight(),
				0, 0,
				button[i].GetXScale(), button[i].GetYScale());
		}
		else if (button[i].GetStatus() == BUTTON_STATUS::MOUSE_DOWN)
		{
			p_res_manager->login_button_sprite[i][2].DrawImage(render_target,
				button[i].GetXPixel(), button[i].GetYPixel(),
				button[i].GetWidth(), button[i].GetHeight(),
				0, 0,
				button[i].GetXScale(), button[i].GetYScale());
		}
	}

	user_name.Draw(render_target, p_res_manager);
	user_password.Draw(render_target, p_res_manager);

	p_res_manager->login_icon_sprite[1].DrawImage(render_target, 315, 250, 150, 150, 0, 0);
	p_res_manager->login_icon_sprite[2].DrawImage(render_target, 315, 250, 150, 150, 0, 0);

	SYSTEMTIME stTime;
	GetLocalTime(&stTime);
	wstring o_text;
	CString text;
	ID2D1SolidColorBrush* brush_black;
	render_target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &brush_black);

	int wYear = stTime.wYear;
	int wMonth = stTime.wMonth;
	int wDay = stTime.wDay;
	int wHour = stTime.wHour;
	int wMinute = stTime.wMinute;
	int wSecond = stTime.wSecond;

	text.Format(L"%02d:%02d", wHour, wMinute);
	o_text = text.GetString();
	RenderText(render_target, o_text, 30, 584, p_res_manager->p_text_format_Arial_80, brush);

	text.Format(L"%04d/%02d/%02d", wYear, wMonth, wDay);
	o_text = text.GetString();
	RenderText(render_target, o_text, 240, 593, p_res_manager->p_text_format_Arial_32, brush);

	text.Format(L"Version %d.%d", MAIN_VERSION, MINOR_VERSION);
	o_text = text.GetString();
	RenderText(render_target, o_text, 240, 630, p_res_manager->p_text_format_Arial_32, brush);

	text.Format(L"OK");
	o_text = text.GetString();
	RenderText(render_target, o_text, 762, 364, p_res_manager->p_text_format_Arial_28_bold, brush_black);

	text.Format(L"Sign In");
	o_text = text.GetString();
	RenderText(render_target, o_text, 475, 250, p_res_manager->p_text_format_Arial_40_bold, brush);

	text.Format(L"ID:");
	o_text = text.GetString();
	RenderText(render_target, o_text, 579, 292, p_res_manager->p_text_format_Arial_28_bold, brush);

	text.Format(L"Password:");
	o_text = text.GetString();
	RenderText(render_target, o_text, 475, 325, p_res_manager->p_text_format_Arial_28_bold, brush);

	ID2D1SolidColorBrush* brush_green;
	render_target->CreateSolidColorBrush(D2D1::ColorF(295556), &brush_green);

	text.Format(L"BOMBERMAN");
	o_text = text.GetString();
	RenderText(render_target, o_text, 204, 58, p_res_manager->p_text_format_Stencil_120_bold, brush_green);

	SafeRelease(&brush);
	return;
}

void CLogin::HandleKeyDown(UINT nchar)
{
	user_name.HandleKeyDown(nchar);
	user_password.HandleKeyDown(nchar);
}