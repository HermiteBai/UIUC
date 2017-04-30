#include "image.h"

/**
 * Flips the image about a vertical line through its center by swapping pixels.
 **/

void Image::flipleft()
{
	size_t mid = (this->width())/2;
	for(size_t i = 0; i < mid; i++)
	{
		for(size_t j = 0; j < (this->height()); j++)
		{
			RGBAPixel * temp = new RGBAPixel();
			* temp = * this->operator()(i, j);
			* this->operator()(i, j) = * this->operator()(this->width()-i-1, j);
			* this->operator()(this->width()-i-1, j) = *temp;
			delete temp;
		}
	}
}

/**
 * Adds to the red, green, and blue parts of each pixel in the image.
 **/

void Image::adjustbrightness(int r, int g, int b)
{
	RGBAPixel *pixel;
	for (size_t i = 0; i < (this->width()); i++)
	{
		for (size_t j = 0; j < (this->height()); j++)
		{
			pixel = this->operator()(i, j);
			if ((pixel->red + r) <= 255 && (pixel->red + r) >= 0)
				pixel->red += r;
			else if ((pixel->red + r) > 255)
				pixel->red = 255;
			else
				pixel->red = 0;

			if ((pixel->green + g) <= 255 && (pixel->green + g) >= 0)
			    pixel->green += g;
		    else if ((pixel->green + g)> 255)	 
				pixel->green = 255;
		    else	
				pixel->green = 0;

			if ((pixel->blue + b) <= 255 && (pixel->blue + b) >= 0)
			    pixel->blue += b;
		    else if ((pixel->blue + b) > 255)
		        pixel->blue = 255;
			else
				pixel->blue = 0;
		}
	}
}

/**
 * Makes each RGB component of each pixel iequal to 255 minus its original value
 **/

void Image::invertcolors()
{
	for (size_t i = 0; i < (this->width()); i++)
	{
		for (size_t j = 0; j < (this->height()); j++)
		{
			RGBAPixel currPixel = *this->operator()(i, j);
			RGBAPixel pixel(255 - currPixel.red, 255 - currPixel.green, 255 - currPixel.blue);
			*this->operator()(i, j) = pixel;
		}
	}
}

