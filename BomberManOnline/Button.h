#include "PointF.h"
#include "GameState.h"
//#include "SceneBase.h"
#include "ButtonState.h"
#pragma once
using namespace std;



class Button
{
private:
	PointF pos;
	BUTTON_STATUS status;
	float height;
	float width;
	float XScale;
	float YScale;
	

public:
	Button(void);
	~Button(void);
	Button(float x, float y, float w, float h, float _XScale, float _YScale);
	void Init(float x, float y, float w, float h, float _XScale, float _YScale);
	float GetXPixel(){ return pos.x; }
	float GetYPixel(){ return pos.y; }
	float GetHeight(){ return height; }
	float GetWidth(){ return width; }
	float GetXScale(){ return XScale; }
	float GetYScale(){ return YScale; }
	BUTTON_STATUS GetStatus(){ return status; }
	void SetStatus(BUTTON_STATUS _status){ status = _status; }
};