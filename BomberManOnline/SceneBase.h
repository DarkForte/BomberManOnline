#pragma once
#include "ResourceManager.h"


class CSceneBase
{
protected:
	CResourceManager *p_res_manager;

public:
	CSceneBase(void);
	~CSceneBase(void);

	CSceneBase(CResourceManager* p_res_manager);
	virtual void Render(CDC* pDC) = 0;
};

