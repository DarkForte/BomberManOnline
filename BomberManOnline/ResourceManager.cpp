#include "stdafx.h"
#include "ResourceManager.h"

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
	map_back.Load(L"pic\\meadow.png");
	player_sprite.Load(L"pic\\player_sprite.png");
	bomb_sprite.Load(L"pic\\bomb_sprite.png");

	TransparentPNG(&player_sprite);
	TransparentPNG(&bomb_sprite);
}


CResourceManager::~CResourceManager(void)
{
}

