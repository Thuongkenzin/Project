//#pragma once
//#include <objidl.h>
//#include <gdiplus.h>
//#pragma comment (lib,"Gdiplus.lib")
//using namespace Gdiplus;
//
//class Shape
//{
//protected:
//	Pen *pen;
//	Brush *brush;
//	Point start;
//	Point end;
//public:
//	Shape();
//	virtual void Draw(Graphics *);
//	Shape(int, int, int, int);
//	~Shape();
//};
//
//class Line : public Shape
//{
//public:
//	Line();
//	/*Line(POINT, POINT);
//	Line(int, int, int, int);*/
//	void Draw(Graphics *);
//	~Line();
//
//};
//
//class Rect : public Shape
//{
//public:
//	Rect();
//	Rect(POINT, POINT);
//	Rect(int, int, int, int);
//	~Rect();
//	void Draw(Graphics *);
//
//};
//
//class Ellip :public Shape
//{
//public:
//	Ellip();
//
//	~Ellip();
//	void Draw(Graphics *);
//		
//};
//
//class Point :public Shape
//{
//public:
//	Point();
//	Point(POINT);
//	Point(int, int);
//	~Point();
//};
//

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

