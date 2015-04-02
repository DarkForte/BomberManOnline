#pragma once
class CResourceManager
{
public:
	CImage map_back;
	//CImage player_sprite;
	CImage bomb_sprite;
	CImage fire_sprite;

	CImage player_sprite[MAX_PLAYER+1];
public:
	CResourceManager(void);
	~CResourceManager(void);
};

