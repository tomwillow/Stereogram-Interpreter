#include "TMyImage.h"

#include <algorithm>
#include <assert.h>
#include "stb_image.h"
#include "stb_image_write.h"

using namespace std;

//����λͼ������HBITMAP������ָ���ַ���޸����ݿ�ֱ��Ӱ��λͼ
//���ԣ�https://github.com/setoutsoft/soui/blob/master/components/render-gdi/render-gdi.cpp
HBITMAP CreateGDIBitmap(int nWid, int nHei, int nChannels, void** ppBits)
{
	BITMAPINFO bmi;
	memset(&bmi, 0, sizeof(bmi));
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = nWid;
	bmi.bmiHeader.biHeight = -nHei; // top-down image 
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = nChannels * 8;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = 0;

	HDC hdc = GetDC(NULL);
	HBITMAP hBmp = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, ppBits, 0, 0);
	ReleaseDC(NULL, hdc);
	return hBmp;
}

void GetResourceByte(HINSTANCE hInst, int id, std::string type, void*& data, int& size)
{
	HRSRC hResource = FindResource(hInst, MAKEINTRESOURCE(id), type.c_str());
	assert(hResource);
	HGLOBAL hLoadResource = LoadResource(hInst, hResource);
	assert(hLoadResource);
	data = LockResource(hLoadResource);
	size = SizeofResource(hInst, hResource);
}

unsigned char* TMyImage::LoadFromResource(int id, std::string type)
{
	this->hInst = GetModuleHandle(NULL);

	//��ȡ��Դ
	unsigned char* origin_data = nullptr;
	int origin_data_size = 0;
	GetResourceByte(hInst, id, type, (void*&)origin_data, origin_data_size);

	//�õ�����
	return stbi_load_from_memory(origin_data, origin_data_size, &width, &height, &channels, 0);
}

unsigned char* TMyImage::LoadFromFile(std::string file_name)
{
	//�õ�����
	return stbi_load(file_name.c_str(), &width, &height, &channels, 0);
}

void TMyImage::InitFromDIB(unsigned char* dib_data)
{
	//
	byte_per_line_add = (4 - (width * channels) % 4) % 4;

	//������λͼ
	hBitmap = CreateGDIBitmap(width, -height, channels, (void**)&data);//dib����������˳��
	assert(hBitmap);

	//���ݸ�����λͼ
	memcpy(data, dib_data, width * height * channels);

	//��ʼ��mem������λͼ
	hdcMem = CreateCompatibleDC(NULL);
	SelectObject(hdcMem, hBitmap);
}

//����stb_image��������ݽ��г�ʼ��
void  TMyImage::InitFromStbData(unsigned char* stb_data)
{
	//
	byte_per_line_add = (4 - (width * channels) % 4) % 4;

	//������λͼ
	hBitmap = CreateGDIBitmap(width, height, channels, (void**)&data);
	assert(hBitmap);

	//���ݸ�����λͼ
	if (channels == 3)
	{
		//stb_data��RGB˳��洢����data��BGR˳��
		//����R��Bͨ��
		unsigned char* origin = stb_data;
		unsigned char* dest = data;
		for (int i = 0; i < height; ++i)
		{
			for (int j = 0; j < width; ++j)
			{
				dest[0] = origin[2];//b
				dest[1] = origin[1];//g
				dest[2] = origin[0];//r
				dest += channels;
				origin += channels;
			}
			dest += byte_per_line_add;
		}
	}
	else
	{
		memcpy(data, stb_data, width * height * channels);

		//stb_data��RGB˳��洢����data��BGR˳��
		//����R��Bͨ��
		unsigned char* pos = data;
		for (int i = 0; i < width * height; ++i)
		{
			swap(pos[0], pos[2]);
			pos += channels;
		}
	}

	//�ͷ�
	stbi_image_free(stb_data);


	//��ʼ��mem������λͼ
	hdcMem = CreateCompatibleDC(NULL);
	SelectObject(hdcMem, hBitmap);
}

TMyImage::TMyImage(std::string file_name)
{
	unsigned char* stb_data = LoadFromFile(file_name);
	assert(stb_data);

	InitFromStbData(stb_data);
	data_size = (width * channels + byte_per_line_add) * height;
}

TMyImage::TMyImage(unsigned char* img_data,int img_data_size, bool isDIB)
{
	if (isDIB)
	{
		//dib���ݽṹ��[Header][Color Palette][Data]
		PBITMAPINFOHEADER pBitmapInfoHeader = (PBITMAPINFOHEADER)img_data;
		width = pBitmapInfoHeader->biWidth;
		height = pBitmapInfoHeader->biHeight;
		channels = pBitmapInfoHeader->biBitCount/8;
		DWORD header_size = pBitmapInfoHeader->biSize;

		InitFromDIB(img_data + header_size);
	}
	else
	{
		stbi_load_from_memory(img_data, img_data_size, &width, &height, &channels, 0);
		InitFromStbData(img_data);
	}
	data_size = (width * channels + byte_per_line_add) * height;
}

TMyImage::TMyImage(int id, std::string type)
{
	unsigned char* stb_data = LoadFromResource(id, type);
	assert(stb_data);

	InitFromStbData(stb_data);
	data_size = (width * channels + byte_per_line_add) * height;
}

TMyImage::~TMyImage()
{
	DeleteObject(hBitmap);
	DeleteDC(hdcMem);
}

void TMyImage::Draw(HDC hdc, int x, int y, int iwidth, int iheight)
{
	if (iwidth == 0) iwidth = width;
	if (iheight == 0) iheight = height;
	//assert(BitBlt(hdc, x, y, width, height, hdcMem, 0, 0, SRCCOPY));
	assert(StretchBlt(hdc, x, y, iwidth, iheight, hdcMem, 0, 0, width, height, SRCCOPY));
}
