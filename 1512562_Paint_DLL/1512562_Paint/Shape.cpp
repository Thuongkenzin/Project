#include "stdafx.h"
#include "Shape.h"


Shape::Shape()
{
	pen = new Pen(Color(255, 0, 0, 0));
}

Shape::Shape(Point _bd, Point _kt)
{
	start = _bd;
	end = _kt;

}

Shape::~Shape()
{
}
void Shape::Draw(Graphics *graphic)
{

}


