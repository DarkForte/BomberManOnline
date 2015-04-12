#include "stdafx.h"
#include "Edit.h"

CEdit::CEdit()
{
	
}

CEdit::CEdit(PointF _pos, CString _text, int _width, int _height, bool _focus, bool _password, int _max_char, bool _multline, int _top)
{
	Init(_pos, _text, _width, _height, _focus, _password, _max_char, _multline, _top);
}

void CEdit::Init(PointF _pos, CString _text, int _width, int _height, bool _focus, bool _password, int _max_char, bool _multline, int _top)
{
	pos = _pos;
	max_char = _max_char;
	setText(_text);
	width = _width;
	height = _height;
	focus = _focus;
	password = _password;
	upper = false;
	multline = _multline;
	top = _top;
	line_num = 0;
}

void CEdit::setPos(PointF _pos)
{
	pos = _pos;
}

void CEdit::setText(CString _text)
{
	int len = _text.GetLength();
	if (len > max_char)
	{
		text = _text.Left(max_char);
	}
	else
	{
		text = _text;
	}
}

void CEdit::setWidth(int _width)
{
	width = _width;
}

void CEdit::setHeight(int _height)
{
	height = _height;
}

void CEdit::setFocus(bool _focus)
{
	focus = _focus;
}

CString CEdit::getDisplayText()
{
	if (password)
	{
		
		CString temp;
		for (int i = 0; i < text.GetLength();i++)
		{
			temp.AppendChar('*');
		}
		return temp;
	}
	else
	{
		return getText();
	}
}

void CEdit::Draw(ID2D1HwndRenderTarget* render_target, CResourceManager* p_res_manager)
{
	ID2D1SolidColorBrush* brush;
	render_target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::Black), &brush);

	if (focus == false)
	{
		p_res_manager->edit.DrawImage(render_target, pos.x, pos.y, width, height, 1, 1, 1, 1, 1);
	}
	else
	{
		p_res_manager->edit_focus.DrawImage(render_target, pos.x, pos.y, width, height, 1, 1, 1, 1, 1);
	}

	
	int len = getDisplayText().GetLength();
	D2D1_RECT_F rect = D2D1::RectF(pos.x + 5, pos.y - 5, pos.x + p_res_manager->p_text_format_Arial_28->GetFontSize()*len + 5, pos.y + p_res_manager->p_text_format_Arial_28->GetFontSize() - 5);
	render_target->DrawText(getDisplayText().GetString(), getDisplayText().GetLength(), p_res_manager->p_text_format_Arial_28, rect, brush);
	if (multline)
	{
		max_line_num = top / p_res_manager->p_text_format_Arial_28->GetFontSize();
		for (int i = 0; i < line_num; i++)
		{
			int line_len = history[i].GetLength();
			ID2D1SolidColorBrush* brush_white;
			render_target->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::White), &brush_white);
			D2D1_RECT_F rect = D2D1::RectF(
				pos.x + 5,
				pos.y - top + p_res_manager->p_text_format_Arial_28->GetFontSize() * i,
				pos.x + 5 + p_res_manager->p_text_format_Arial_28->GetFontSize() * line_len,
				pos.y - top + p_res_manager->p_text_format_Arial_28->GetFontSize() * (i + 1));
			render_target->DrawText(history[i].GetString(), history[i].GetLength(), p_res_manager->p_text_format_Arial_28, rect, brush_white);
		}
	}
}

void CEdit::HandleLButtonDown(CPoint point)
{
	if (point.x >= pos.x &&
		point.x <= pos.x + width &&
		point.y >= pos.y &&
		point.y <= pos.y + height)
	{
		focus = true;
	}
	else
	{
		focus = false;
	}
}

void CEdit::HandleKeyDown(UINT nchar)
{
	if (focus)
	{
		if (nchar == VK_BACK)
		{
			if (text.GetLength())
			{
				text = text.Left(text.GetLength() - 1);
			}
		}
		else if (nchar == VK_CAPITAL)
		{
			upper = !upper;
		}
		else if (nchar >= 48 && nchar <= 57)
		{
			if (text.GetLength() + 1 < max_char)
			{
				text.AppendChar(nchar);
			}
		}
		else if (nchar >= 65 && nchar <= 90)
		{
			if (text.GetLength() + 1 < max_char)
			{
				if (upper)
				{
					text.AppendChar(nchar);
				}
				else
				{
					text.AppendChar(nchar+'a'-'A');
				}
			}
		}
		else if (nchar == VK_RETURN)
		{
			line_num++;
			if (line_num > max_line_num)
			{
				line_num--;
				for (int i = 0; i < line_num; i++)
				{
					history[i] = history[i + 1];
				}
			}
			history[line_num-1] = "Player:";
			history[line_num-1].Append(text);
			text = "";
		}
	}
}