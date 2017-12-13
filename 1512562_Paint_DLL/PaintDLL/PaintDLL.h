#pragma once
#include <ObjIdl.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;

void DrawLineDll(Graphics *graphic, Pen *pen, Point x, Point y);
void DrawRectDll(Graphics *graphic,Pen *pen, Point x, Point y);