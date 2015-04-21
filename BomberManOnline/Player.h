#pragma once
#include "PointF.h"
#include "Item.h"
#include "MovingObjects.h"
using namespace std;

enum class PLAYER_STATUS
{
	NONE, WRAPPED, DEAD, FLIPPING, SLOW, BOMB_INVIS
};

enum class PLAYER_TRANSFORM
{
	NONE
};

class CPlayer : public CMovingObjects
{
	static float frame_time;
	int now_frame;
	float rest_frametime;

	int moving_state;
	pair<PLAYER_STATUS, float> status;
	
	int max_capacity;
	int max_power;
	float max_speed;
	
	int bomb_capacity;
	int bomb_power;

	/*PointF back_speed;
	int back_capacity;
	int back_power;*/

	bool hover, reverse;
	int type;

	int team;
	int now_bombs;
	int facing;

	pair<CPoint, bool> special_access;
	pair<Item, int> items[MAX_ITEMS+1];

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
	
	PLAYER_STATUS Status() const { return status.first; }
	float StatusTime() const {return status.second; }
	void SetStatus(PLAYER_STATUS v1, float v2=0);
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
	PointF TryMove(float game_time);
	void Update(float game_time);

	int Team() const { return team; }
	void Team(int val) { team = val; }

	void ClearMovingState();
protected:
	/*void BackupStatus();
	void RestoreStatus();*/
};

