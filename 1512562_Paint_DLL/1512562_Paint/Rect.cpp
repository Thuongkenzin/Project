#include  "stdafx.h"
#include "Shape.h"
CRect::~CRect()
{}
CRect::CRect()
{
}

CRect::CRect(Point _bd, Point _kt)
{
	start = _bd;
	end = _kt;

}
void CRect::Draw(Graphics *graphic)
{
	Point x(start.X, start.Y);
	Point y(end.X, end.Y);
	HINSTANCE hinstLib = LoadLibrary(L"PaintDLL.dll");
	MYPROC pointer = (MYPROC)GetProcAddress(hinstLib, "Draw_Rect");
	if (pointer != NULL)
		pointer(graphic, pen, x, y);
}