#pragma once
#include "PointF.h"
#include "Item.h"
#include "MovingObjects.h"
using namespace std;

enum class PLAYER_STATUS
{
	NONE, DEAD
};

class CPlayer : public CMovingObjects
{
	static float frame_time;
	int now_frame;
	int rest_frametime;

	int moving_state;
	PLAYER_STATUS status;
	
	int max_capacity;
	int max_power;
	float max_speed;
	
	int bomb_capacity;
	int bomb_power;


	bool hover, reverse;
	int type;

	int now_bombs;
	int facing;

	pair<CPoint, bool> special_access;
	pair<Item, int> items[7];

public:
	CPlayer(void);
	~CPlayer(void);
	CPlayer(float x, float y, int type);
	void Init(float x, float y, int type);

	int GetBombPower();
	int GetBombCapacity();

	bool Hover();
	bool Reverse();
	int Type();
	int NowBombs();
	int BombCapacity();
	
	void SetHover(bool x);
	void SetReverse(bool x);

	void SetBombPower(int r);
	void SetBombCapacity(int r);
	void SetMovingState(int next_state);
	void SetNowBombs(int bombs);
	void CancelMovingState(int state);
	
	
	PLAYER_STATUS Status() const { return status; }
	void SetStatus(PLAYER_STATUS val) { status = val; }
	int Facing() const { return facing; }

	pair<CPoint, bool> SpecialAccess() const { return special_access; }
	void ShutSpecialAccess();
	void SetSpecialAccess(CPoint pos);

	int NowFrame() const { return now_frame; }

	int MaxCapacity() const { return max_capacity; }
	void SetMaxCapacity(int val) { max_capacity = val; }
	int MaxPower() const { return max_power; }
	void SetMaxPower(int val) { max_power = val; }
	float MaxSpeed() const { return max_speed; }
	void SetMaxSpeed(float val) { max_speed = val; }

	void AddItem(Item item, int number = 1);
	Item PopItem(int index);
	pair<Item, int> PeekItem(int index);

	int GetMovingDirection();
	void Move(float game_time);
};

