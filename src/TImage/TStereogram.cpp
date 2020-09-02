#include "TStereogram.h"

#include "HSB.h"

using namespace std;

TStereogram::TStereogram(std::string file_name) :TMyImage(file_name)
{
	int line_byte = width * channels + byte_per_line_add;
	int size = line_byte * height;

	//��ʼ��origin_data
	origin_data = new unsigned char[size];
	memcpy(origin_data, data, size);
}

TStereogram::TStereogram(unsigned char* img_data, int img_data_size, bool isDIB):TMyImage(img_data,img_data_size,isDIB)
{
	int line_byte = width * channels + byte_per_line_add;
	int size = line_byte * height;

	//��ʼ��origin_data
	origin_data = new unsigned char[size];
	memcpy(origin_data, data, size);
}

TStereogram::~TStereogram()
{
	delete[] origin_data;
}

void TStereogram::SetOffset(int offset)
{
	
	bool offset_positive = offset >= 0;//�Ƿ�ƫ����Ϊ��

	int abs_offset = abs(offset);//ƫ��������ֵ

	if (abs_offset > width)//ƫ��������ֵ����ͼ�������и���
		return;

	double value = 0;//������ֵ�������Ʋ���ȷ��ƫ����

	int line_byte = width * channels + byte_per_line_add;//ÿ��byte��
	int size = line_byte * height;//�ܴ�С

	//unsigned char* cover = new unsigned char[size];//�������

	memcpy(data, origin_data, size);
	for (int y = 0; y < height; ++y)
	{
		unsigned char* dest = data+y*line_byte;

		if (offset_positive)
		{
			dest += abs_offset * channels;//destƫ��
			unsigned char* op = origin_data+y*line_byte;
			for (int x = 0; x < width - abs_offset; ++x)
			{
				dest[0] = abs((int)dest[0] - op[0]);
				dest[1] = abs((int)dest[1] - op[1]);
				dest[2] = abs((int)dest[2] - op[2]);
				dest += channels;
				op += channels;
			}
		}
		else
		{
			unsigned char* op = origin_data+y*line_byte+abs_offset*channels;//opƫ��
			for (int x = 0; x < width - abs_offset; ++x)
			{
				dest[0] = abs((int)dest[0] - op[0]);
				dest[1] = abs((int)dest[1] - op[1]);
				dest[2] = abs((int)dest[2] - op[2]);
				dest += channels;
				op += channels;
			}
		}
	}

	string info = to_string(value) + "\n";
	OutputDebugString(info.c_str());
}

void TStereogram::Restore()
{
	memcpy(data, origin_data, data_size);
}
