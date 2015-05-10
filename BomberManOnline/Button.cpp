#include "stdafx.h"
#include "Button.h"

Button::Button(void)
{
	pos.SetPoint(0,0);
	status = BUTTON_STATUS::IDLE;
	height = 0;
	width = 0;
}

Button::~Button(void)
{
}

Button::Button(float x, float y, float w, float h, float _XScale, float _YScale)
{
	Init(x, y, w, h, _XScale, _YScale);
}

void Button::Init(float x, float y, float w, float h, float _XScale, float _YScale)
{
	pos.SetPoint(x,y);
	status = BUTTON_STATUS::IDLE;
	height = h;
	width = w;
	XScale = _XScale;
	YScale = _YScale;
}