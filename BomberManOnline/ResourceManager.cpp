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
	
}


void CResourceManager::LoadPics( IWICImagingFactory * pIWICFactory, ID2D1HwndRenderTarget* render_target )
{
	map_back.Load(pIWICFactory, render_target, L"pic\\meadow.png");
	for(int i=1; i<=MAX_PLAYER; i++)
	{
		WCHAR buf[60];
		swprintf_s(buf, L"pic\\player%d.png", i);

		player_sprite[i].Load(pIWICFactory, render_target, buf);
	}

	bomb_sprite.Load(pIWICFactory, render_target, L"pic\\bomb_sprite.png");
	fire_sprite.Load(pIWICFactory, render_target, L"pic\\fire_sprite.png");

	/*Game UI*/

	bottom_rect.Load(pIWICFactory, render_target, L"pic\\game_ui\\bottom_rect.png");
	capacity_icon.Load(pIWICFactory, render_target, L"pic\\game_ui\\capacity_icon.png");
	power_icon.Load(pIWICFactory, render_target, L"pic\\game_ui\\power_icon.png");
	speed_icon.Load(pIWICFactory, render_target, L"pic\\game_ui\\speed_icon.png");
	item_box.Load(pIWICFactory, render_target, L"pic\\game_ui\\item_box.png");

	info_rect.Load(pIWICFactory, render_target, L"pic\\game_ui\\info_rect.png");

	exit_icon.Load(pIWICFactory, render_target, L"pic\\game_ui\\exit_icon.png");
	help_icon.Load(pIWICFactory, render_target, L"pic\\game_ui\\help_icon.png");
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
}

