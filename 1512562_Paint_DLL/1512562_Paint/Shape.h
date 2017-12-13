
#pragma once
#include <objidl.h>
#include <gdiplus.h>
#pragma comment (lib,"Gdiplus.lib")
using namespace Gdiplus;
class Shape
{
protected:
	Pen* pen;
	Brush *brush;
	Point start;
	Point end;
public:
	Shape(Point _bt, Point _kt);
	virtual void Draw(Graphics *) = 0;
	Shape();
	~Shape();
};
typedef void(*MYPROC)(Graphics*, Pen*, Point, Point);
class Line :public Shape
{
public:
	void Draw(Graphics *);
	Line(Point _bt, Point _kt);

	Line();
	~Line();
};

class CRect :public Shape
{
public:
	void Draw(Graphics *);
	CRect(Point _bt, Point _kt);

	CRect();
	~CRect();
};
class CElip :public Shape
{
public:
	CElip(Point _bt, Point _kt);
	void Draw(Graphics *);
	CElip();
	~CElip();
};

