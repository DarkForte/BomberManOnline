#include "PointF.h"
#pragma once
using namespace std;

enum class PLAYER_STATUS
{
	NONE, DEAD
};

class CPlayer
{
	static float frame_time;

	PointF pos;
	int moving_state;
	PLAYER_STATUS status;
	
	int bomb_capacity;
	int bomb_power;
	PointF speed;

	bool hover, reverse;
	int type;

	int now_bombs;

	int now_frame;
	int rest_frametime;
	int facing;

	pair<CPoint, bool> special_access;
	

public:
	CPlayer(void);
	~CPlayer(void);
	CPlayer(float x, float y, int type);
	void Init(float x, float y, int type);

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
	int NowBombs();
	int BombCapacity();
	
	void SetHover(bool x);
	void SetReverse(bool x);
	void SetXSpeed(float x);
	void SetYSpeed(float y);
	void SetSpeed(float x, float y);
	void SetBombPower(int r);
	void SetBombCapacity(int r);
	void SetMovingState(int next_state);
	void SetNowBombs(int bombs);
	void CancelMovingState(int state);
	int GetMovingDirection();
	
	void Move(float game_time);
	CPoint NextGrid(int direction);
	PointF TryMove(float game_time);

	PLAYER_STATUS Status() const { return status; }
	void SetStatus(PLAYER_STATUS val) { status = val; }

	int NowFrame() const { return now_frame; }
	int Facing() const { return facing; }

	pair<CPoint, bool> SpecialAccess() const { return special_access; }
	void ShutSpecialAccess();
	void SetSpecialAccess(CPoint pos);

};

