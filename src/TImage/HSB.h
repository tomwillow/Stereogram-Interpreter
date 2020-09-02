#pragma once
#include <Windows.h>

struct HSB
{
	float h, s, b;
};
HSB operator+(const HSB& l, const HSB& r);
HSB operator-(const HSB& l, const HSB& r);
HSB operator*(const HSB& l, float f);
COLORREF HSB2RGB(HSB hsb);
HSB RGB2HSB(COLORREF cr);

#define R(cr) (*(byte*)&cr)
#define G(cr) (*((byte*)&cr)+1)
#define B(cr) (*((byte*)&cr)+2)