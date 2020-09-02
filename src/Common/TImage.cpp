#include "TImage.h"

#include <assert.h>

using namespace std;

HANDLE Load(std::string fileName)
{
	return LoadImage(0, fileName.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
}

void TImage::Init(bool hasMask)
{
	BITMAP bitmap;
	GetObject(hBitmapImage, sizeof(BITMAP), &bitmap);
	width = bitmap.bmWidth;
	height = bitmap.bmHeight;

	hMemImage = CreateCompatibleDC(NULL);
	SelectObject(hMemImage, hBitmapImage);

	BITMAPINFO bmi;
	memset(&bmi, 0, sizeof(bmi));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	//bmi.bmiHeader.biWidth = width;
	//bmi.bmiHeader.biHeight = -height; // top-down image 
	//bmi.bmiHeader.biPlanes = 1;
	//bmi.bmiHeader.biBitCount = 24;
	//bmi.bmiHeader.biCompression = BI_RGB;
	//bmi.bmiHeader.biSizeImage = 0;
	assert(GetDIBits(hMemImage, hBitmapImage, 0, 0, 0, &bmi, DIB_RGB_COLORS));

	//BYTE* p = nullptr;
	BYTE* p = new BYTE[bmi.bmiHeader.biSizeImage];
	bmi.bmiHeader.biHeight = height;
	bmi.bmiHeader.biCompression = BI_RGB;
	void* v = &(bmi.bmiHeader);
	assert(GetDIBits(hMemImage, hBitmapImage, 0, height, p, (BITMAPINFO*)&bmi, DIB_RGB_COLORS));

	for (int i=0;i<height;++i)
		for (int j = 0; j < width; ++j)
		{
			uint32_t* pu = (uint32_t*)&p[i * height + width];
			*pu = 0xff00ff00;
		}

	assert(SetDIBits(hMemImage, hBitmapImage, 0, height, p, (BITMAPINFO*)&bmi, DIB_RGB_COLORS));

	if (hasMask)
	{
		BITMAP bitmapMask;

		GetObject(hBitmapMask, sizeof(BITMAP), &bitmapMask);

		hdcMemMask = CreateCompatibleDC(NULL);
		SelectObject(hdcMemMask, hBitmapMask);

		BitBlt(hMemImage, 0, 0, width, height, hdcMemMask, 0, 0, SRCAND);//Í¼Æ¬±³¾°±äºÚ

		BitBlt(hdcMemMask, 0, 0, width, height, hdcMemMask, 0, 0, DSTINVERT);//ÑÚÂëÍ¼·´Ïò

	}
	else
	{
		hBitmapMask = 0;
		hdcMemMask = 0;
	}
}

TImage::TImage(HINSTANCE hInst,int id,int maskId):hInst(hInst),id(id),maskId(maskId),iWidth(1.0),iHeight(1.0)
{
	hBitmapImage = LoadBitmap(hInst, (LPCSTR)id);
	assert(hBitmapImage);

	if (maskId)
	{
		hBitmapMask = LoadBitmap(hInst, (LPCSTR)maskId);
		assert(hBitmapMask);
	}

	Init(maskId);
}

TImage::TImage(HINSTANCE hInst, std::string fileName, std::string maskFileName) :hInst(hInst), id(0), maskId(0), iWidth(1.0), iHeight(1.0)
{
	hBitmapImage = (HBITMAP)Load(fileName);
	assert(hBitmapImage);

	if (!maskFileName.empty())
	{
		hBitmapMask = (HBITMAP)Load(maskFileName);
		assert(hBitmapMask);
	}

	Init(!maskFileName.empty());
}

//TImage::TImage(const TImage& img):TImage(img.hInst,img.id,img.maskId)
//{
//
//}

TImage::~TImage()
{
	DeleteObject(hBitmapImage);
	DeleteDC(hMemImage);

	if (maskId)
	{
		DeleteObject(hBitmapMask);
		DeleteDC(hdcMemMask);
	}
}


void TImage::Draw(HDC hdc,POINT pt)
{
	Draw_inner(hdc, pt.x, pt.y);
}

void TImage::Draw_inner(HDC hdc,int dest_x,int dest_y,int img_x,int img_y,DWORD rop)
{
	int newWidth = width * iWidth;
	int dWidth = width - width * iWidth;
	int newX = dest_x + dWidth / 2;

	if (maskId)
	{
		StretchBlt(hdc, newX, dest_y, newWidth, height * iHeight, hdcMemMask, img_x, img_y, width, height, SRCAND);//ÍÚ¶´
		StretchBlt(hdc, newX, dest_y, newWidth, height * iHeight, hMemImage, img_x, img_y, width, height, SRCPAINT);//ÌîÄÚÈÝ
	}
	else
		assert(StretchBlt(hdc, newX, dest_y, newWidth, height * iHeight, hMemImage, img_x, img_y, width, height, rop));
}

void TImage::Fill(HDC hdc,const RECT &rect)
{
	for (int x = 0; x < rect.right; x+= width)
		for (int y = 0; y < rect.bottom;y+= height)
			Draw_inner(hdc, x, y);
}