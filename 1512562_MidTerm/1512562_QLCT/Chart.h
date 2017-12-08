#ifndef __CHART_H__
#define __CHART_H__


#include "stdafx.h"
#include <vector>
#include <string>
using namespace std;
struct Coord {
	int x, y;
};

struct RGBColor
{
	int r, g, b;
};

struct ChartCategory
{
	wstring type;
	RGBColor color;
	int amount;
	float startAngle;
	float sweepAngle;
};

struct TypeCategory
{
	wstring type;
	int totalAmount;
};
class Chart
{
private:
	Coord mOrigin;
	int iRadius;
	vector<ChartCategory> vCat;
	float GetSweepAngle(int index);
	//Coord GetCoordCenterTwoPoints(Coord p1, Coord p2, float percent);

public:
	Chart();
	Chart(Coord _origin, int _radius);
	void Draw(HDC hdc);
	void UpdateCoordParts(vector<TypeCategory> _vCat);
	void ShowNote(HDC hdc);
	~Chart();
};

#endif