#pragma once
#include <Windows.h>
#include <string>

class TImage
{
protected:
	HINSTANCE hInst;
	int id;
	int width, height;
	double iWidth, iHeight;
	HBITMAP hBitmapImage;
	HDC hMemImage;
	TImage():
		hInst(0),
		width(0),height(0),
		iWidth(1.0), iHeight(1.0),
		id(0),
		hBitmapImage(0),hMemImage(0),
		maskId(0),hBitmapMask(0),hdcMemMask(0) {}
	virtual void Draw_inner(HDC hdc, int dest_x, int dest_y, int img_x = 0, int img_y = 0, DWORD rop = SRCCOPY);
private:
	int maskId;
	HBITMAP hBitmapMask;
	HDC hdcMemMask;

	void Init(bool hasMask);
public:
	TImage(HINSTANCE hInst, int id, int maskId = 0);
	TImage(HINSTANCE hInst, std::string fileName, std::string maskFileName = "");
	//TImage(const TImage& img);
	~TImage();

	void SetIWidth(double iWidth)
	{
		this->iWidth = iWidth;
	}

	int GetWidth()
	{
		return width;
	}

	int GetHeight()
	{
		return height;
	}

	void Draw(HDC hdc, POINT pt = { 0,0 });
	void Fill(HDC hdc, const RECT &rect);
};

