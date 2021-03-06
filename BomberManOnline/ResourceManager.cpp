#include "stdafx.h"
#include "ResourceManager.h"
using namespace std;

void TransparentPNG(CImage *png)
{
	for(int i = 0; i <png->GetWidth(); i++)
	{
		for(int j = 0; j <png->GetHeight(); j++)
		{
			unsigned char* pucColor = reinterpret_cast<unsigned char*>(png->GetPixelAddress(i , j));
			pucColor[0] = pucColor[0] *pucColor[3] / 255;
			pucColor[1] = pucColor[1] *pucColor[3] / 255;
			pucColor[2] = pucColor[2] *pucColor[3] / 255;
		}
	}
}

CResourceManager::CResourceManager(void)
{
}


CResourceManager::~CResourceManager(void)
{
	m_Client.Stop();
	m_Client.UnloadSocketLib();
}


void CResourceManager::LoadPics( IWICImagingFactory * pIWICFactory, ID2D1HwndRenderTarget* render_target )
{
	//map_back.Load(pIWICFactory, render_target, L"pic\\meadow.png");
	for(int i=1; i<=6; i++)
	{
		WCHAR buf[60];
		swprintf_s(buf, L"pic\\sprites\\player%d.png", i);

		player_sprite[i].Load(pIWICFactory, render_target, buf);
	}
	for (int i=20;i<=27;i++)
	{
		WCHAR buf[60];
		swprintf_s(buf, L"pic\\sprites\\trans%d.png", i);

		trans_sprite[i].Load(pIWICFactory, render_target, buf);
	}

	bomb_sprite.Load(pIWICFactory, render_target, L"pic\\bomb_sprite.png");
	fire_sprite.Load(pIWICFactory, render_target, L"pic\\fire_sprite.png");

#pragma region Lobby UI Loading

	for (int i = 1; i <= ROOM_MAX_BUTTON; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			WCHAR buf[60];
			swprintf_s(buf, L"pic\\scene_ui\\button2%d%d.png", i, j);
			room_button_sprite[i][j].Load(pIWICFactory, render_target, buf);
		}
	}

	for (int i = 1; i <= ROOM_MAX_ICON; i++)
	{
		WCHAR buf[60];
		swprintf_s(buf, L"pic\\scene_ui\\icon0%d.png", i);

		room_icon_sprite[i].Load(pIWICFactory, render_target, buf);
	}

	for (int i = 1; i <= LOBBY_MAX_BUTTON; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			WCHAR buf[60];
			swprintf_s(buf, L"pic\\scene_ui\\button1%d%d.png", i, j);
			lobby_button_sprite[i][j].Load(pIWICFactory, render_target, buf);
		}
	}

	for (int i = 1; i <= LOBBY_MAX_ICON; i++)
	{
		WCHAR buf[60];
		swprintf_s(buf, L"pic\\scene_ui\\icon0%d.png", i);

		lobby_icon_sprite[i].Load(pIWICFactory, render_target, buf);
	}

	for (int i = 1; i <= LOGIN_MAX_BUTTON; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			WCHAR buf[60];
			swprintf_s(buf, L"pic\\scene_ui\\button0%d%d.png", i, j);
			login_button_sprite[i][j].Load(pIWICFactory, render_target, buf);
		}
	}

	for (int i = 1; i <= LOGIN_MAX_ICON; i++)
	{
		WCHAR buf[60];
		swprintf_s(buf, L"pic\\scene_ui\\icon0%d.png", i);
		login_icon_sprite[i].Load(pIWICFactory, render_target, buf);
	}

	login_ui.Load(pIWICFactory, render_target, L"pic\\scene_ui\\login_b.png");
	lobby_ui.Load(pIWICFactory, render_target, L"pic\\scene_ui\\lobby_b.png");
	room_ui.Load(pIWICFactory, render_target, L"pic\\scene_ui\\room_b.png");

	edit.Load(pIWICFactory, render_target, L"pic\\scene_ui\\edit.png");
	edit_focus.Load(pIWICFactory, render_target, L"pic\\scene_ui\\edit_focus.png");

	message_bg.Load(pIWICFactory, render_target, L"pic\\scene_ui\\message_bg.png");

	ready.Load(pIWICFactory, render_target, L"pic\\scene_ui\\ready.png");

#pragma endregion

#pragma region Game UI Loading

	bottom_rect.Load(pIWICFactory, render_target, L"pic\\game_ui\\bottom_rect.png");
	for(int i=1; i<=3; i++)
	{
		WCHAR buf[60];
		swprintf_s(buf, L"pic\\game_ui\\bottom_icon%d.png", i);
		bottom_icon[i].Load(pIWICFactory, render_target, buf);
	}
	item_box.Load(pIWICFactory, render_target, L"pic\\game_ui\\item_box.png");

	avatar_back.Load(pIWICFactory, render_target, L"pic\\game_ui\\avatar_back.png");

	exit_icon.Load(pIWICFactory, render_target, L"pic\\game_ui\\exit_icon.png");
	help_icon.Load(pIWICFactory, render_target, L"pic\\game_ui\\help_icon.png");

	timer_rect.Load(pIWICFactory, render_target, L"pic\\game_ui\\timer_rect.png");

	panel_rect.Load(pIWICFactory, render_target, L"pic\\game_ui\\panel_rect.png");

	avatar.Load(pIWICFactory, render_target, L"pic\\game_ui\\avatar.png");

	for(int i=1; i<=MAX_PLAYER; i++)
	{
		WCHAR buf[60]={0};
		swprintf_s(buf, L"pic\\game_ui\\user_back%d.png", i);
		userinfo_rect[i].Load(pIWICFactory, render_target, buf);
	}

	map_none.Load(pIWICFactory, render_target, L"pic\\game_ui\\map_none.png");
	map_obstacle.Load(pIWICFactory, render_target, L"pic\\game_ui\\map_obstacle.png");
	map_destroyable.Load(pIWICFactory, render_target, L"pic\\game_ui\\map_destroyable.png");
	icons.Load(pIWICFactory, render_target, L"pic\\icons.png");

#pragma endregion

}

void CResourceManager::InitTextFormat(IDWriteFactory* write_factory)
{
	 write_factory->CreateTextFormat(
		L"Arial",                   // Font family name
		NULL,                          // Font collection(NULL sets it to the system font collection)
		DWRITE_FONT_WEIGHT_REGULAR,    // Weight
		DWRITE_FONT_STYLE_NORMAL,      // Style
		DWRITE_FONT_STRETCH_NORMAL,    // Stretch
		50.0f,                         // Size    
		L"en-us",                      // Local
		&p_text_format                 // Pointer to receive the created object
		);
		
	 write_factory->CreateTextFormat(
		 L"Arial",                   // Font family name
		 NULL,                          // Font collection(NULL sets it to the system font collection)
		 DWRITE_FONT_WEIGHT_REGULAR,    // Weight
		 DWRITE_FONT_STYLE_NORMAL,      // Style
		 DWRITE_FONT_STRETCH_NORMAL,    // Stretch
		 80.0f,                         // Size    
		 L"en-us",                      // Local
		 &p_text_format_Arial_80        // Pointer to receive the created object
		 );
	 write_factory->CreateTextFormat(
		 L"Arial",                   // Font family name
		 NULL,                          // Font collection(NULL sets it to the system font collection)
		 DWRITE_FONT_WEIGHT_REGULAR,    // Weight
		 DWRITE_FONT_STYLE_NORMAL,      // Style
		 DWRITE_FONT_STRETCH_NORMAL,    // Stretch
		 32.0f,                         // Size    
		 L"en-us",                      // Local
		 &p_text_format_Arial_32        // Pointer to receive the created object
		 );
	 write_factory->CreateTextFormat(
		 L"Arial",                   // Font family name
		 NULL,                          // Font collection(NULL sets it to the system font collection)
		 DWRITE_FONT_WEIGHT_BOLD,    // Weight
		 DWRITE_FONT_STYLE_NORMAL,      // Style
		 DWRITE_FONT_STRETCH_NORMAL,    // Stretch
		 28.0f,                         // Size    
		 L"en-us",                      // Local
		 &p_text_format_Arial_28_bold        // Pointer to receive the created object
		 );
	 write_factory->CreateTextFormat(
		 L"Arial",                   // Font family name
		 NULL,                          // Font collection(NULL sets it to the system font collection)
		 DWRITE_FONT_WEIGHT_BOLD,    // Weight
		 DWRITE_FONT_STYLE_NORMAL,      // Style
		 DWRITE_FONT_STRETCH_NORMAL,    // Stretch
		 40.0f,                         // Size    
		 L"en-us",                      // Local
		 &p_text_format_Arial_40_bold        // Pointer to receive the created object
		 );
	 write_factory->CreateTextFormat(
		 L"Arial",                   // Font family name
		 NULL,                          // Font collection(NULL sets it to the system font collection)
		 DWRITE_FONT_WEIGHT_BOLD,    // Weight
		 DWRITE_FONT_STYLE_NORMAL,      // Style
		 DWRITE_FONT_STRETCH_NORMAL,    // Stretch
		 48.0f,                         // Size    
		 L"en-us",                      // Local
		 &p_text_format_Arial_48_bold        // Pointer to receive the created object
		 );
	 write_factory->CreateTextFormat(
		 L"Arial",                   // Font family name
		 NULL,                          // Font collection(NULL sets it to the system font collection)
		 DWRITE_FONT_WEIGHT_BOLD,    // Weight
		 DWRITE_FONT_STYLE_NORMAL,      // Style
		 DWRITE_FONT_STRETCH_NORMAL,    // Stretch
		 72.0f,                         // Size    
		 L"en-us",                      // Local
		 &p_text_format_Arial_72_bold        // Pointer to receive the created object
		 );
	 write_factory->CreateTextFormat(
		 L"Arial",                   // Font family name
		 NULL,                          // Font collection(NULL sets it to the system font collection)
		 DWRITE_FONT_WEIGHT_BOLD,    // Weight
		 DWRITE_FONT_STYLE_NORMAL,      // Style
		 DWRITE_FONT_STRETCH_NORMAL,    // Stretch
		 32.0f,                         // Size    
		 L"en-us",                      // Local
		 &p_text_format_Arial_32_bold        // Pointer to receive the created object
		 );
	 write_factory->CreateTextFormat(
		 L"Arial",                   // Font family name
		 NULL,                          // Font collection(NULL sets it to the system font collection)
		 DWRITE_FONT_WEIGHT_REGULAR,    // Weight
		 DWRITE_FONT_STYLE_NORMAL,      // Style
		 DWRITE_FONT_STRETCH_NORMAL,    // Stretch
		 28.0f,                         // Size    
		 L"en-us",                      // Local
		 &p_text_format_Arial_28        // Pointer to receive the created object
		 );
	 write_factory->CreateTextFormat(
		 L"Stencil",                   // Font family name
		 NULL,                          // Font collection(NULL sets it to the system font collection)
		 DWRITE_FONT_WEIGHT_REGULAR,    // Weight
		 DWRITE_FONT_STYLE_NORMAL,      // Style
		 DWRITE_FONT_STRETCH_NORMAL,    // Stretch
		 120.0f,                         // Size    
		 L"en-us",                      // Local
		 &p_text_format_Stencil_120_bold        // Pointer to receive the created object
		 );
	 write_factory->CreateTextFormat(
		 L"Arial",                   // Font family name
		 NULL,                          // Font collection(NULL sets it to the system font collection)
		 DWRITE_FONT_WEIGHT_REGULAR,    // Weight
		 DWRITE_FONT_STYLE_NORMAL,      // Style
		 DWRITE_FONT_STRETCH_NORMAL,    // Stretch
		 20.0f,                         // Size    
		 L"en-us",                      // Local
		 &p_corner_number_format        // Pointer to receive the created object
		 );
	 write_factory->CreateTextFormat(
		 L"Subatomic Tsoonami",                   // Font family name
		 NULL,                          // Font collection(NULL sets it to the system font collection)
		 DWRITE_FONT_WEIGHT_REGULAR,    // Weight
		 DWRITE_FONT_STYLE_NORMAL,      // Style
		 DWRITE_FONT_STRETCH_NORMAL,    // Stretch
		 80.0f,                         // Size    
		 L"en-us",                      // Local
		 &p_timer_number_format        // Pointer to receive the created object
		 );
}

bool CResourceManager::InitClient()
{
	int nPort = DEFAULT_PORT;
	int nThreads = DEFAULT_THREADS;
	CString strIP;
	strIP = DEFAULT_IP;

	ifstream fin;
	char ip[20];
	fin.open("ip.txt");

	if (fin)
	{
		fin >> ip;
		USES_CONVERSION;
		strIP = "";
		strIP.Append(CA2T(ip));
	}

	// 给CClient设置参数
	m_Client.LoadSocketLib();
	m_Client.SetIP(strIP);
	m_Client.SetPort(nPort);
	m_Client.SetThreads(nThreads);

	// 开始
	if (!m_Client.Start())
	{
		return false;
	}
	
	account.ready = false;
	account.user_name = "";

	account.actor_id = 0;

	for (int i = 0; i < 8; i++)
	{
		account.room_num[i] = 0;
	}

	for (int i = 0; i < 4; i++)
	{
		account.room_actor[i] = 0;
		account.room_exp[i] = 0;
		account.room_money[i] = 0;
		account.room_ready[i] = 0;
		account.room_vip[i] = 0;
	}

	return true;
}
