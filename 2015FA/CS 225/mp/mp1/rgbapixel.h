#ifndef RGBAPIXEL_H
#define RGBAPIXEL_H

#include <cstdint>
using std::uint8_t;

class RGBAPixel
{
public:
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;

	/**
    * This function constructs a default RGBAPixel
	**/
	RGBAPixel();
	
	/**
	 * This function constructs an opaque  RGBAPixel with the give rgb values
	 **/
	RGBAPixel(uint8_t red, uint8_t green, uint8_t blue);
};

#endif	
   
