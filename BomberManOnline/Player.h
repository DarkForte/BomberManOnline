#include "PointF.h"

#pragma once
class CPlayer
{
	PointF pos;
	int moving_state;
	int status;
	int bomb_capacity;
	int bomb_power;
	PointF speed;

	bool hover, reverse;
	int type;


public:
	CPlayer(void);
	~CPlayer(void);
	CPlayer(float x, float y, int type);
	void Init(float x, float y, int type);

	PointF GetPosPixel();
	CPoint GetPosGrid();
	void SetPosPixel(float x, float y);

	/*Get the X in pixel of the player, max is 900*/
	float GetXPixel();
	float GetYPixel();

	/*Get the X in grid of the player, max is 30*/
	int GetXGrid();
	int GetYGrid();

	int GetStatus();
	int GetBombPower();
	int GetBombCapacity();
	/*Get the speed in pixels*/
	PointF GetSpeed();
	/*pixels*/
	float GetXSpeed();
	float GetYSpeed();
	bool Hover();
	bool Reverse();
	int Type();
	
	void SetHover(bool x);
	void SetReverse(bool x);
	void SetXSpeed(float x);
	void SetYSpeed(float y);
	void SetSpeed(float x, float y);
	void SetBombPower(int r);
	void SetBombCapacity(int r);
	void SetMovingState(int next_state);
	void CancelMovingState(int state);
	int GetMovingDirection();
	
	void Move(float game_time);
	PointF NextGrid(int direction);

};

