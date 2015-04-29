#pragma once
#include "stdafx.h"
using namespace std;

class CBomb
{
	int owner;
	
	CPoint pos;
	double remain_time;
	
	int power;

public:
	CBomb();
	~CBomb();
	/*Create a new bomb with position x,y, _remain_time (in ms), power*/
	CBomb(int owner, int x, int y, double _remain_time, int _power);
	
	/*Get the position of the bomb, in grids, max is (30,20)*/
	CPoint GetPos();
	int GetX();
	int GetY();

	int GetPower();

	double GetRemainTime() const { return remain_time; }
	void SetRemainTime(double val) { remain_time = val; }

	void SetX(int x);
	void SetY(int y);
	void SetPos(int x, int y);
	void SetPower(int p);

	int Owner() const { return owner; }
	void SetOwner(int val) { owner = val; }

	void DropTime(float game_time);
};

