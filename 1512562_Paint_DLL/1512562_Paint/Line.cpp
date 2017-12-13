#include "stdafx.h"
#include"Shape.h"
Line::Line()
{
}
Line::Line(Point _bd, Point _kt)
{
	start = _bd;
	end = _kt;

}
void Line::Draw(Graphics *graphic)
{
	//graphic->DrawLine(pen, start, end);
	Point x(start.X, start.Y);
	Point y(end.X, end.Y);
	HINSTANCE hinstLib = LoadLibrary(L"PaintDLL.dll");
	MYPROC pointer = (MYPROC)GetProcAddress(hinstLib, "Draw_Line");
	if (pointer != NULL)
		pointer(graphic, pen, x, y);

}

Line::~Line()
{
}