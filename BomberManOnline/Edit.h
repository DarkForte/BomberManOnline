#pragma once
#include "PointF.h"
#include "stdafx.h"
#include "D2D1Header.h"
#include "ResourceManager.h"

class CEdit{
private:
	PointF pos;
	CString text;
	int max_char;
	int width;
	int height;
	bool focus;
	bool password;
	bool isChat;
	bool upper;
	bool multline;
	int top;
	int max_line_num;
	int line_num;
	int total_line_num;
	CString history[MAX_CHAT_HISTORY];
	CResourceManager* p_res_manager;
public:
	CEdit();
	CEdit(PointF _pos, CString _text, int _width, int _height, bool _focus, bool _password, int _max_char, bool _multline, int _top);
	void Init(PointF _pos, CString _text, int _width, int _height, bool _focus, bool _password, int _max_char, bool _multline, int _top);
	PointF getPos(){ return pos; }
	CString getText(){ return text; }
	int getWidth(){ return width; }
	int getHeight(){ return height; }
	int getLineNum(){ return line_num; }
	int getTotalLineNum(){ return total_line_num; }
	bool getFocus(){ return focus; }
	void setClient(CResourceManager* _p_res_manager){ p_res_manager = _p_res_manager; }
	void setIsChat(bool _isChat){ isChat = _isChat; }
	void setPos(PointF _pos);
	void setText(CString _text);
	void setWidth(int _width);
	void setHeight(int _height);
	void setFocus(bool _focus);
	CString getDisplayText();
	void Draw(ID2D1HwndRenderTarget* render_target, CResourceManager* p_res_manager);
	void HandleLButtonDown(CPoint point);
	void HandleKeyDown(UINT nchar);
	void AddMessage(CString _msg);
};