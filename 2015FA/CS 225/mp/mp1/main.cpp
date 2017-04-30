#include <iostream>

#include "rgbapixel.h"
#include "png.h"

using namespace std;

int main()
{
	//Open .png file
	PNG image("in.png");
	//Constuct a temporary file
	PNG temp(image.width(), image.height());

	//Rotate the in.png image
	for (size_t i = 0; i < image.width(); i++)
	{
		for (size_t j = 0; j < image.height(); j++)
		{
			RGBAPixel color(image(i, j)->red, image(i, j)->green, image(i, j)->blue);
			*temp(image.width()-i-1, image.height()-j-1) = color;
		}
	}

	//Write it back to file and output a .png
	//file that was already rotated.
	temp.writeToFile("out.png");
	return 0;
}
