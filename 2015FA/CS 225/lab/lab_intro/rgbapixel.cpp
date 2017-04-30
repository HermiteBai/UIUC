#include "rgbapixel.h"

/**
 * Default constructor
 **/
RGBAPixel::RGBAPixel()
{
	red = 255;
	green = 255;
	blue = 255;
	alpha = 255;
}

/**
 * Constructor with args
 **/
RGBAPixel::RGBAPixel(uint8_t red, uint8_t green, uint8_t blue)
{
	this->red = red;
	this->green = green;
	this->blue = blue;
	alpha = 255;
}



