#pragma once
struct PointF
{
	float x,y;
	PointF(){}
	PointF(float _x, float _y)
	{
		SetPoint(_x, _y);
	}
	void SetPoint(float _x, float _y)
	{
		x=_x;
		y=_y;
	}
	PointF operator + (const PointF &b) const
	{
		return PointF(x+b.x, y+b.y);
	}
};

