#include "stdafx.h"
#include "SceneBase.h"


CSceneBase::CSceneBase(void)
{
}


CSceneBase::~CSceneBase(void)
{
}

CSceneBase::CSceneBase(CResourceManager* p_res_manager)
{
	this->p_res_manager = p_res_manager;
}