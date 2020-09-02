#pragma once
#include "TMyImage.h"
class TStereogram :
	public TMyImage
{
public:
	TStereogram(std::string file_name);
	TStereogram(unsigned char* img_data, int data_size, bool isDIB);
	~TStereogram();

	void SetOffset(int offset);
	void Restore();
private:
	unsigned char* origin_data;
};

