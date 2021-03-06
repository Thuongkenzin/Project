// PaintDLL.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "PaintDLL.h"

void DrawLineDll(Graphics *graphic, Pen *pen, Point x, Point y)
{
	graphic->DrawLine(pen, x, y);
}

void DrawRectDll(Graphics *graphic, Pen *pen, Point x, Point y)
{
	graphic->DrawRectangle(pen, x.X, x.Y, abs(y.X - x.X), abs(y.Y - x.Y));
}