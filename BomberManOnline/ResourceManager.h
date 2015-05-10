#pragma once
#include "d2dimage.h"
#include "D2D1Header.h"
#include "Client.h"
#include "Account.h"
class CResourceManager
{
public:
	CD2DImage map_back;
	CD2DImage bomb_sprite;
	CD2DImage fire_sprite;

	CD2DImage player_sprite[MAX_PLAYER+1];

#pragma region Lobby UI Definations
	CD2DImage lobby_button_sprite[LOBBY_MAX_BUTTON + 1][3];
	CD2DImage lobby_icon_sprite[LOBBY_MAX_ICON + 1];
	CD2DImage login_button_sprite[LOGIN_MAX_BUTTON + 1][3];
	CD2DImage login_icon_sprite[LOGIN_MAX_ICON + 1];
	CD2DImage room_button_sprite[ROOM_MAX_BUTTON + 1][3];
	CD2DImage room_icon_sprite[ROOM_MAX_ICON + 1];

	CD2DImage login_ui;
	CD2DImage lobby_ui;
	CD2DImage room_ui;

	CD2DImage edit;
	CD2DImage edit_focus;
#pragma endregion
	
#pragma region Font Definations
	IDWriteTextFormat* p_text_format;
	IDWriteTextFormat* p_text_format_Stencil_120_bold;
	IDWriteTextFormat* p_text_format_Arial_80;
	IDWriteTextFormat* p_text_format_Arial_72_bold;
	IDWriteTextFormat* p_text_format_Arial_48_bold;
	IDWriteTextFormat* p_text_format_Arial_40_bold;
	IDWriteTextFormat* p_text_format_Arial_32;
	IDWriteTextFormat* p_text_format_Arial_32_bold;
	IDWriteTextFormat* p_text_format_Arial_28;
	IDWriteTextFormat* p_text_format_Arial_28_bold;
	IDWriteTextFormat* p_corner_number_format;
	IDWriteTextFormat* p_timer_number_format;
#pragma endregion

#pragma region Game UI Definations
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
	CD2DImage map_obstacle;
	CD2DImage map_destroyable;

	CD2DImage icons;
#pragma endregion

	CClient m_Client;
	CD2DImage message_bg;
	CAccount account;
public:
	CResourceManager(void);
	~CResourceManager(void);

	void LoadPics(IWICImagingFactory * pIWICFactory, ID2D1HwndRenderTarget* render_target);
	void InitTextFormat(IDWriteFactory* write_factory);
	bool InitClient();
};

