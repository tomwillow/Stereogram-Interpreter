#pragma once
#include <Windows.h>
#include <string>

class TMyImage
{
public:
	TMyImage(int id, std::string type);
	TMyImage(std::string file_name);
	TMyImage(unsigned char *img_data,int img_data_size,bool isDIB);
	~TMyImage();

	void Draw(HDC hdc, int x = 0, int y = 0,int iwidth=0,int iheight=0);
protected:
	int width, height;//宽，高
	int channels; //通道数 3 or 4
	int byte_per_line_add;//每行byte数据 末尾比原始byte数据 增加的字节数，有init负责初始化
	unsigned char* data;//原始数据 由init负责初始化
	int data_size;
private:
	HINSTANCE hInst;
	HBITMAP hBitmap;
	HDC hdcMem;
	unsigned char* LoadFromResource(int id, std::string type);//从资源载入
	unsigned char* LoadFromFile(std::string file_name);//从文件名载入
	void InitFromStbData(unsigned char* stb_data); 
	void InitFromDIB(unsigned char* dib_data);
};

