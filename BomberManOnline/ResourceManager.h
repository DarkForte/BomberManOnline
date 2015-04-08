#pragma once
#include "d2dimage.h"
#include "D2D1Header.h"
class CResourceManager
{
public:
	CD2DImage map_back;
	CD2DImage bomb_sprite;
	CD2DImage fire_sprite;

	CD2DImage player_sprite[MAX_PLAYER+1];

	IDWriteTextFormat* p_text_format;

/*Game UI*/
	CD2DImage bottom_rect;
	CD2DImage bottom_icon[4];
	CD2DImage item_box;

	CD2DImage avatar_back;

	CD2DImage timer_rect;
	
	CD2DImage panel_rect;
	CD2DImage exit_icon;
	CD2DImage help_icon;

	CD2DImage avatar;
	CD2DImage userinfo_rect[MAX_PLAYER +1]; 
	
	CD2DImage map_none;

public:
	CResourceManager(void);
	~CResourceManager(void);

	void LoadPics(IWICImagingFactory * pIWICFactory, ID2D1HwndRenderTarget* render_target);
	void InitTextFormat(IDWriteFactory* write_factory);
};

