#pragma once

#include "TImage.h"
#include <string>

class TPNGImage :
	public TImage
{
private:
	int channels=-1;
	virtual void Draw_inner(HDC hdc, int dest_x, int dest_y, int img_x = 0, int img_y = 0, DWORD rop = SRCCOPY) override;
public:
	//type: "PNG" etc
	TPNGImage(HINSTANCE hInst, int id,std::string type);
};

