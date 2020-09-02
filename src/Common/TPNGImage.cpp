#include "TPNGImage.h"

#include <algorithm>
#include <assert.h>
#include "stb_image.h"

#pragma comment(lib,"Msimg32.lib")

using namespace std;

//创建位图，返回HBITMAP，传入指针地址，修改数据可直接影响位图
//来自：https://github.com/setoutsoft/soui/blob/master/components/render-gdi/render-gdi.cpp
HBITMAP CreateGDIBitmap(int nWid, int nHei,int nChannels, void** ppBits)
{
	BITMAPINFO bmi;
	memset(&bmi, 0, sizeof(bmi));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = nWid;
	bmi.bmiHeader.biHeight = -nHei; // top-down image 
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = nChannels*8;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = 0;

	HDC hdc = GetDC(NULL);
	HBITMAP hBmp = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, ppBits, 0, 0);
	ReleaseDC(NULL, hdc);
	return hBmp;
}

void GetResourceByte(HINSTANCE hInst, int id,std::string type, void *&data,int &size)
{
	HRSRC hResource = FindResource(hInst, MAKEINTRESOURCE(id), type.c_str());
	assert(hResource);
	HGLOBAL hLoadResource = LoadResource(hInst, hResource);
	assert(hLoadResource);
	data = LockResource(hLoadResource);
	size = SizeofResource(hInst, hResource);
}

TPNGImage::TPNGImage(HINSTANCE hInst, int id, std::string type)
{
	this->hInst = hInst;
	this->id = id;

	//读取资源
	unsigned char* png = nullptr;
	int pngSize = 0;
	GetResourceByte(hInst, id, type, (void*&)png, pngSize);

	//得到png数据
	//stbi_set_flip_vertically_on_load(true);
	unsigned char* stb_data = stbi_load_from_memory(png, pngSize, &width, &height, &channels, 0);

	//创建新位图
	unsigned char* data = nullptr;
	hBitmapImage = CreateGDIBitmap(width, height, channels, (void**)&data);

	//png数据复制至位图
	memcpy(data, stb_data, width * height * channels);

	if (channels == 3)
	{
		//交换R和B通道
		unsigned char* pos = data;
		for (int i = 0; i < width * height; ++i)
		{
			swap(pos[0], pos[2]);
			pos += 3;
		}
	}

	if (channels == 4)
	{
		//交换R和B通道
		unsigned char* pos = data;
		for (int i = 0; i < width * height; ++i)
		{
			swap(pos[0], pos[2]);
			pos += 4;
		}
	}

	//释放png
	stbi_image_free(stb_data);

	//初始化mem并载入位图
	hMemImage = CreateCompatibleDC(NULL);
	SelectObject(hMemImage, hBitmapImage);
}

void TPNGImage::Draw_inner(HDC hdc, int dest_x, int dest_y, int img_x, int img_y, DWORD rop)
{
	int newWidth = abs(width * iWidth);
	int dWidth = width - newWidth;
	int newX = dest_x + dWidth / 2;

	switch (channels)
	{
	case 3:

		assert(StretchBlt(hdc, newX, dest_y, newWidth, height * iHeight, hMemImage, img_x, img_y, width, height, rop));
		break;
	case 4:
	{
		//使用临时dc
		HDC tempDC = CreateCompatibleDC(NULL);

		//临时位图
		HBITMAP hBitmapTemp = CreateGDIBitmap(width, height, 4, nullptr);
		SelectObject(tempDC, hBitmapTemp);
		DeleteObject(hBitmapTemp);

		if (iWidth<0)
			assert(StretchBlt(tempDC, width, 0, -width, height, hMemImage, img_x, img_y, width, height, rop));
		else
			assert(StretchBlt(tempDC, 0, 0, width, height, hMemImage, img_x, img_y, width, height, rop));

		//以alpha通道进行叠加
		BLENDFUNCTION bf = { AC_SRC_OVER,0,0xff,AC_SRC_ALPHA };
		assert(::AlphaBlend(hdc, newX, dest_y, newWidth, height * iHeight, tempDC, img_x, img_y, width, height, bf));
		DeleteDC(tempDC);
		break;
	}
	default:
		assert(0);
	}

}