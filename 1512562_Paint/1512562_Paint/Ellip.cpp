#include  "stdafx.h"
#include "Shape.h"

CElip::CElip()
{
}

CElip::~CElip()
{
}
CElip::CElip(Point _bd, Point _kt)
{
	start = _bd;
	end = _kt;

}
void CElip::Draw(Graphics *graphic)
{
	graphic->DrawEllipse(pen, start.X, start.Y, end.X, end.Y);
}