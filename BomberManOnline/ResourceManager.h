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
	CD2DImage capacity_icon;
	CD2DImage power_icon;
	CD2DImage speed_icon;
	CD2DImage item_box;

	CD2DImage info_rect;

	CD2DImage timer_rect;
	
	CD2DImage panel_rect;
	CD2DImage exit_icon;
	CD2DImage help_icon;
	
public:
	CResourceManager(void);
	~CResourceManager(void);

	void LoadPics(IWICImagingFactory * pIWICFactory, ID2D1HwndRenderTarget* render_target);
	void InitTextFormat(IDWriteFactory* write_factory);
};

