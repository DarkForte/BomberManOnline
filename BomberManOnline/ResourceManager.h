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

	CD2DImage lobby_button_sprite[LOBBY_MAX_BUTTON + 1][3];
	CD2DImage lobby_icon_sprite[LOBBY_MAX_ICON + 1];
	CD2DImage login_ui;
	CD2DImage lobby_ui;
	CD2DImage room_ui;

	IDWriteTextFormat* p_text_format;
	IDWriteTextFormat* p_text_format_Arial_80;
	IDWriteTextFormat* p_text_format_Arial_40_block;
	IDWriteTextFormat* p_text_format_Arial_32;
	IDWriteTextFormat* p_text_format_Arial_28_block;
	
public:
	CResourceManager(void);
	~CResourceManager(void);

	void LoadPics(IWICImagingFactory * pIWICFactory, ID2D1HwndRenderTarget* render_target);
	void InitTextFormat(IDWriteFactory* write_factory);
};

