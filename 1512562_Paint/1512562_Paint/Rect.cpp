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
	graphic->DrawRectangle(pen, start.X, start.Y, end.X, end.Y);
}