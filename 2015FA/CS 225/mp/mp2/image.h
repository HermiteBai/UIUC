#ifndef _IMAGE_H_
#define _IMAGE_H_
#include "png.h"
class Image : public PNG
{
public:
	void flipleft();
	void adjustbrightness(int r, int g, int b);
	void invertcolors();
};

#endif
