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
	/*map_back.Load(L"pic\\meadow.png");

	for(int i=1; i<=MAX_PLAYER; i++)
	{
		WCHAR buf[60];
		swprintf_s(buf, L"pic\\player%d.png", i);

		player_sprite[i].Load(buf);
		TransparentPNG(&player_sprite[i]);
	}

	bomb_sprite.Load(L"pic\\bomb_sprite.png");
	fire_sprite.Load(L"pic\\fire_sprite.png");

	
	TransparentPNG(&bomb_sprite);
	TransparentPNG(&fire_sprite);*/

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
}

