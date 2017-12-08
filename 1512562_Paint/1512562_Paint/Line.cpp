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
	graphic->DrawLine(pen, start, end);
}

Line::~Line()
{
}