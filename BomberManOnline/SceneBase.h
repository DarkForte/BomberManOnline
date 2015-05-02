#pragma once
#include "ResourceManager.h"
#include "D2D1Header.h"
#include "Button.h"

class CSceneBase
{
protected:
	CResourceManager *p_res_manager;
	bool isMessage;
	Button msg_button;
	CString msg_string;

public:
	CSceneBase(void);
	~CSceneBase(void);

	CSceneBase(CResourceManager* p_res_manager);
	virtual void Render(ID2D1HwndRenderTarget* render_target) = 0;

	void SetIsMessage(bool _isMessage){ isMessage = _isMessage; }
	bool GetIsMessage(){ return isMessage; }
	void SetMessage(CString _msg){ msg_string = _msg; }
	CString GetMessage(){ return msg_string; }
	void RenderMessage(ID2D1HwndRenderTarget* render_target);
	void RenderText(ID2D1HwndRenderTarget* render_target, wstring str, int x, int y,
		IDWriteTextFormat* format, ID2D1SolidColorBrush* brush);
};

