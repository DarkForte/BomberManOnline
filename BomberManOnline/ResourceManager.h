#pragma once
#include "d2dimage.h"
#include "D2D1Header.h"
class CResourceManager
{
public:
	CD2DImage map_back;
	//CImage player_sprite;
	CD2DImage bomb_sprite;
	CD2DImage fire_sprite;

	CD2DImage player_sprite[MAX_PLAYER+1];

	IDWriteTextFormat* p_text_format;

	
public:
	CResourceManager(void);
	~CResourceManager(void);

	void LoadPics(IWICImagingFactory * pIWICFactory, ID2D1HwndRenderTarget* render_target);
	void InitTextFormat(IDWriteFactory* write_factory);
};

