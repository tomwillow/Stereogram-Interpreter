#include "HSB.h"

HSB operator+(const HSB& l, const HSB& r)
{
	return { l.h + r.h,l.s + r.s,l.b + r.b };
}

HSB operator-(const HSB& l, const HSB& r)
{
	return { l.h - r.h,l.s - r.s,l.b - r.b };
}

HSB operator*(const HSB& l, float f)
{
	return { l.h * f,l.s * f,l.b * f };
}

COLORREF HSB2RGB(HSB hsb)
{
	COLORREF cr;

	byte* rgb = ((byte*)&cr);
	int hi = int(hsb.h / 60.0) % 6;
	float f = hsb.h / 60.0 - hi;
	unsigned char v = hsb.b * 255.0;
	unsigned char p = hsb.b * (1.0 - hsb.s) * 255.0;
	unsigned char q = hsb.b * (1.0 - f * hsb.s) * 255.0;
	unsigned char t = hsb.b * (1.0 - (1.0 - f) * hsb.s) * 255.0;
	switch (hi)
	{
	case 0:rgb[0] = v; rgb[1] = t; rgb[2] = p; break;
	case 1:rgb[0] = q; rgb[1] = v; rgb[2] = p; break;
	case 2:rgb[0] = p; rgb[1] = v; rgb[2] = t; break;
	case 3:rgb[0] = p; rgb[1] = q; rgb[2] = v; break;
	case 4:rgb[0] = t; rgb[1] = p; rgb[2] = v; break;
	case 5:rgb[0] = v; rgb[1] = p; rgb[2] = q; break;
	}
	rgb[3] = 0;
	return cr;
}

HSB RGB2HSB(COLORREF cr)
{
	byte* rgb = ((byte*)&cr);
	byte r = rgb[0];
	byte g = rgb[1];
	byte b = rgb[2];
	byte max = max(max(r, g), b);
	byte min = min(min(r, g), b);

	float hsbB = max / 255.0f;
	float hsbS = max == 0 ? 0 : (max - min) / (float)max;

	float hsbH = 0;
	if (max == r && g >= b)
	{
		hsbH = (g - b) * 60.f / (max - min) + 0;
	}
	else
		if (max == r && g < b)
		{
			hsbH = (g - b) * 60.f / (max - min) + 360;
		}
		else
			if (max == g)
			{
				hsbH = (b - r) * 60.f / (max - min) + 120;
			}
			else
				if (max == b) 
				{
					hsbH = (r - g) * 60.f / (max - min) + 240;
				}

	return { hsbH, hsbS, hsbB };
}