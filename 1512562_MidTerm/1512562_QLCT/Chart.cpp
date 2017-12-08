#include "stdafx.h"
#include "Chart.h"


float Chart::GetSweepAngle(int index)
{
	int total = 0;
	for (int i = 0; i < this->vCat.size(); ++i)
	{
		total += this->vCat[i].amount;
	}

	float percent = (float)this->vCat[index].amount / total;
	return float(percent * 360);
}

void Chart::UpdateCoordParts(vector<TypeCategory> _vCat)
{
	for (int i = 0; i < this->vCat.size(); ++i)
	{
		this->vCat[i].type = _vCat[i].type;
		this->vCat[i].amount = _vCat[i].totalAmount;
	}

	for (int i = 0; i < this->vCat.size(); ++i)
	{
		this->vCat[i].sweepAngle = this->GetSweepAngle(i);
	}

	this->vCat[0].startAngle = 90;

	for (int i = 1; i < this->vCat.size(); ++i)
	{
		this->vCat[i].startAngle = this->vCat[i - 1].startAngle + this->vCat[i - 1].sweepAngle;
	}
}

Chart::Chart()
{

}

Chart::Chart(Coord _origin, int _radius)
{
	this->mOrigin = _origin;
	this->iRadius = _radius;

	this->vCat.resize(6);
	this->vCat[0].color = { 231, 76, 60 };
	this->vCat[1].color = { 52, 152, 219 };
	this->vCat[2].color = { 39, 174, 96 };
	this->vCat[3].color = { 142, 68, 173 };
	this->vCat[4].color = { 211, 84, 0 };
	this->vCat[5].color = { 44, 62, 80 };
}


void Chart::Draw(HDC hdc)
{
	for (int i = 0; i < this->vCat.size(); ++i)
	{
		if (this->vCat[i].amount != 0)
		{
			SetDCBrushColor(hdc, RGB(this->vCat[i].color.r, this->vCat[i].color.g, this->vCat[i].color.b));
			SetDCPenColor(hdc, RGB(241, 241, 241));
			SelectObject(hdc, GetStockObject(DC_BRUSH));
			SelectObject(hdc, GetStockObject(DC_PEN));

			BeginPath(hdc);
			MoveToEx(hdc, this->mOrigin.x, this->mOrigin.y, NULL);
			AngleArc(hdc, this->mOrigin.x, this->mOrigin.y, this->iRadius, this->vCat[i].startAngle, this->vCat[i].sweepAngle);
			LineTo(hdc, this->mOrigin.x, this->mOrigin.y);
			EndPath(hdc);
			StrokeAndFillPath(hdc);
		}
	}
}

void Chart::ShowNote(HDC hdc)
{
	HFONT hFontInside = CreateFont(25, 0, 0, 0, 800, FALSE, FALSE, FALSE, VIETNAMESE_CHARSET,
		OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Calibri"));
	HFONT hFontOutside = CreateFont(20, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, VIETNAMESE_CHARSET,
		OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Calibri"));
	vector<Coord> vCoord = {
		{ 660, 470 },
		{ 860, 470 },
		{ 1060, 470 },
		{ 660, 540 },
		{ 860, 540 },
		{ 1060, 540 },
	};
	SetBkMode(hdc, TRANSPARENT);
	SelectObject(hdc, hFontInside);
	SetTextColor(hdc, RGB(255, 255, 255));

	for (int i = 0; i < this->vCat.size(); ++i)
	{
		SetDCBrushColor(hdc, RGB(vCat[i].color.r, vCat[i].color.g, vCat[i].color.b));
		Rectangle(hdc, vCoord[i].x, vCoord[i].y, vCoord[i].x + 50, vCoord[i].y + 50);
		RECT rect = { vCoord[i].x, vCoord[i].y, vCoord[i].x + 50, vCoord[i].y + 50 };
		DrawText(hdc, (to_wstring((int)round(this->GetSweepAngle(i) / 360 * 100)) + L'%').c_str(), -1, &rect, DT_SINGLELINE | DT_NOCLIP | DT_CENTER | DT_VCENTER);
	}

	SelectObject(hdc, hFontInside);
	SetTextColor(hdc, RGB(0, 0, 0));
	for (int i = 0; i < this->vCat.size(); ++i)
	{
		RECT rect = { vCoord[i].x + 60, vCoord[i].y, vCoord[i].x + 150, vCoord[i].y + 50 };
		DrawText(hdc, vCat[i].type.c_str(), -1, &rect, DT_SINGLELINE | DT_NOCLIP | DT_LEFT | DT_VCENTER);
	}
}

Chart::~Chart()
{
}
