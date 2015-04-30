#include "RenderText.h"

void RenderText(ID2D1HwndRenderTarget* render_target, wstring str, int x, int y,
						IDWriteTextFormat* format, ID2D1SolidColorBrush* brush)
{
	int len = str.length();
	D2D1_RECT_F rect = D2D1::RectF(x,y, x+format->GetFontSize()*len, y+format->GetFontSize()*len);
	render_target->DrawText(str.c_str(), str.length(), format, rect, brush);
}