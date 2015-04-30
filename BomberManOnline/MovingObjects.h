#pragma once
#include "stdafx.h"
#include "PointF.h"

class CMovingObjects
{
protected:
	
	PointF pos;
	PointF speed;

	int direction;

public:
	CMovingObjects(void);
	virtual ~CMovingObjects(void);
	CMovingObjects(CPoint p, float s, int dir);

	void SetPosPixel(float x, float y);

	/*Get the X in pixel of the player, max is 900*/
	float GetXPixel();
	float GetYPixel();
	PointF GetPosPixel();

	/*Get the X in grid of the player, max is 30*/
	/*GetJudgeGrid means : if a grid width is 30, you are in 16, then I see you
		as you are in the next grid, as long as you touched width/2
	*/
	int GetXJudgeGrid();
	int GetYJudgeGrid();
	CPoint GetPosJudgeGrid();

	/*GetRealGrid means: if a grid width is 30, you are in 16, then I see you 
	//in this grid until you reach 31.
	*/
	int GetXRealGrid();
	int GetYRealGrid();
	CPoint GetPosRealGrid();

	void SetXSpeed(float x);
	void SetYSpeed(float y);
	void SetSpeed(float x, float y);

	/*Get the speed in pixels*/
	PointF GetSpeed();
	/*pixels*/
	float GetXSpeed();
	float GetYSpeed();

	virtual int GetMovingDirection(){return direction;}
	virtual void Move(float game_time);
	CPoint NextGrid(int direction);
	virtual PointF TryMove(float game_time);
};

