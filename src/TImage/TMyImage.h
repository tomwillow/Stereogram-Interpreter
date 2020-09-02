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
	int width, height;//����
	int channels; //ͨ���� 3 or 4
	int byte_per_line_add;//ÿ��byte���� ĩβ��ԭʼbyte���� ���ӵ��ֽ�������init�����ʼ��
	unsigned char* data;//ԭʼ���� ��init�����ʼ��
	int data_size;
private:
	HINSTANCE hInst;
	HBITMAP hBitmap;
	HDC hdcMem;
	unsigned char* LoadFromResource(int id, std::string type);//����Դ����
	unsigned char* LoadFromFile(std::string file_name);//���ļ�������
	void InitFromStbData(unsigned char* stb_data); 
	void InitFromDIB(unsigned char* dib_data);
};

