#pragma once
#include "stdafx.h"
#include "D2D1Header.h"
#include <string>
using namespace std;

void RenderText(ID2D1HwndRenderTarget* render_target, wstring str, int x, int y,
				IDWriteTextFormat* format, ID2D1SolidColorBrush* brush);