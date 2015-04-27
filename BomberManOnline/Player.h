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
	GHOST=20, FLY=21, QUICK=22, UBW=23, PANDA=24, DEMON=25, CRAB=26, KILLER=27,
	NONE=0, TRANS_START=1, TRANS_END=2
};

class CPlayer : public CMovingObjects
{
	static float frame_time;
	int now_frame;
	float rest_frametime;

	int moving_state;
	pair<PLAYER_STATUS, float> status;
	pair<PLAYER_TRANSFORM, float> trans;
	PLAYER_TRANSFORM target_transform;
	
	int max_capacity;
	int max_power;
	float max_speed;
	
	int bomb_capacity;
	int bomb_power;

	int type;

	int team;
	int now_bombs;
	int facing;

	pair<CPoint, bool> special_access;
	pair<Item, int> items[MAX_ITEMS+1+1];

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

	void SetBombPower(int r);
	void SetBombCapacity(int r);
	void SetMovingState(int next_state);
	void SetNowBombs(int bombs);
	void CancelMovingState(int state);
	
	PLAYER_STATUS Status() const { return status.first; }
	float StatusTime() const {return status.second; }
	void SetStatus(PLAYER_STATUS v1, float v2=0);
	
	PLAYER_TRANSFORM Trans() const {return trans.first; }
	float TransTime() const {return trans.second;}
	void SetTrans(PLAYER_TRANSFORM v1, float v2, PLAYER_TRANSFORM next_transform);
	
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
	void SetSpecialItem(Item item, int number=1);

	int GetMovingDirection();
	void Move(float game_time);
	PointF TryMove(float game_time);
	void Update(float game_time);

	int Team() const { return team; }
	void Team(int val) { team = val; }

	void ClearMovingState();

protected:
	PointF CalcRealSpeed();
};

