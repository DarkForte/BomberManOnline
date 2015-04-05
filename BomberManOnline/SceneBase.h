#pragma once
#include "ResourceManager.h"
#include "D2D1Header.h"

class CSceneBase
{
protected:
	CResourceManager *p_res_manager;

public:
	CSceneBase(void);
	~CSceneBase(void);

	CSceneBase(CResourceManager* p_res_manager);
	virtual void Render(ID2D1HwndRenderTarget* render_target) = 0;
};

