#include "stdafx.h"
#include "SceneBase.h"

CSceneBase::CSceneBase(void)
{
}


CSceneBase::~CSceneBase(void)
{
}

CSceneBase::CSceneBase(CResourceManager* p_res_manager)
{
	this->p_res_manager = p_res_manager;
}

void CSceneBase::RenderMessage(ID2D1HwndRenderTarget* render_target)
{
	ID2D1SolidColorBrush* brush;
	render_target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &brush);
	

	p_res_manager->message_bg.DrawImage(render_target, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0);

	wstring o_text;
	o_text = msg_string.GetString();
	RenderText(render_target, o_text, 230, 260, p_res_manager->p_text_format_Arial_48_bold, brush);

	o_text = msg_string2.GetString();
	RenderText(render_target, o_text, 230, 310, p_res_manager->p_text_format_Arial_48_bold, brush);

	if (msg_button.GetStatus() == BUTTON_STATUS::IDLE)
	{
		p_res_manager->login_button_sprite[6][0].DrawImage(render_target,
			msg_button.GetXPixel(), msg_button.GetYPixel(),
			msg_button.GetWidth(), msg_button.GetHeight(),
			0, 0,
			msg_button.GetXScale(), msg_button.GetYScale());
	}
	else if (msg_button.GetStatus() == BUTTON_STATUS::MOUSE_ON)
	{
		p_res_manager->login_button_sprite[6][1].DrawImage(render_target,
			msg_button.GetXPixel(), msg_button.GetYPixel(),
			msg_button.GetWidth(), msg_button.GetHeight(),
			0, 0,
			msg_button.GetXScale(), msg_button.GetYScale());
	}
	else if (msg_button.GetStatus() == BUTTON_STATUS::MOUSE_DOWN)
	{
		p_res_manager->login_button_sprite[6][2].DrawImage(render_target,
			msg_button.GetXPixel(), msg_button.GetYPixel(),
			msg_button.GetWidth(), msg_button.GetHeight(),
			0, 0,
			msg_button.GetXScale(), msg_button.GetYScale());
	}

	CString text;
	text.Format(L"OK");
	o_text = text.GetString();
	RenderText(render_target, o_text, 862, 364, p_res_manager->p_text_format_Arial_28_bold, brush);

	SafeRelease(&brush);
}

void CSceneBase::RenderText(ID2D1HwndRenderTarget* render_target, wstring str, int x, int y,
	IDWriteTextFormat* format, ID2D1SolidColorBrush* brush)
{
	int len = str.length();
	D2D1_RECT_F rect = D2D1::RectF(x, y, x + format->GetFontSize()*len, y + format->GetFontSize()*len);
	render_target->DrawText(str.c_str(), str.length(), format, rect, brush);
}