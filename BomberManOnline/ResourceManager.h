#pragma once
#include "d2dimage.h"
class CResourceManager
{
public:
	CD2DImage map_back;
	//CImage player_sprite;
	CD2DImage bomb_sprite;
	CD2DImage fire_sprite;

	CD2DImage player_sprite[MAX_PLAYER+1];
public:
	CResourceManager(void);
	~CResourceManager(void);

	void LoadPics(IWICImagingFactory * pIWICFactory, ID2D1HwndRenderTarget* render_target);
};

