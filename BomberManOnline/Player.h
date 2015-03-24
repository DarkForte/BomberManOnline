#pragma once
class CPlayer
{
	CPoint pos;
	int status;
	int bomb_capacity;
	int bomb_power;
	CPoint speed;

	bool hover, reverse;
	int type;


public:
	CPlayer(void);
	~CPlayer(void);
	CPlayer(int x, int y, int type);
	void Init(int x, int y, int type);

	CPoint GetPosPixel();
	CPoint GetPosGrid();
	void SetPosPixel(int x, int y);

	/*Get the X in pixel of the player, max is 900*/
	int GetXPixel();
	int GetYPixel();

	/*Get the X in grid of the player, max is 30*/
	int GetXGrid();
	int GetYGrid();

	int GetStatus();
	int GetBombPower();
	int GetBombCapacity();
	/*Get the speed in pixels*/
	CPoint GetSpeed();
	/*pixels*/
	double GetXSpeed();
	double GetYSpeed();
	bool Hover();
	bool Reverse();
	int Type();
	
	void SetHover(bool x);
	void SetReverse(bool x);
	void SetXSpeed(int x);
	void SetYSpeed(int y);
	void SetSpeed(int x, int y);
	void SetBombPower(int r);
	void SetBombCapacity(int r);
	
	void Move(int direction);
	CPoint NextGrid(int direction);

};

