#include <cstdlib>
#include "png.h"
#include <iostream>
using namespace std;
//
// sets up the output image
PNG * setupOutput(int w, int h)
{
    PNG * image = new PNG(w, h);
    return image;
}


// Returns my favorite color
RGBAPixel * myFavoriteColor(int intensity)
{
    RGBAPixel * color = new RGBAPixel(0, intensity/2, intensity);
    return color;
}


void sketchify()
{
    // Load in.png
    PNG * original = new PNG("in.png");
    cout << "Reached line 28" << endl;
    //original->readFromFile("in.png");
    cout << "Reached line 30" << endl;
    int width  = original->width();
    int height = original->height();
    cout << "Reached line 33" << endl;
    // Create out.png
    PNG * output;
    cout << "Reached line 36" << endl;
    output = setupOutput(width, height);
    cout << "Reached line 38" << endl;
    // Load our favorite color to color the outline
    RGBAPixel * myPixel = myFavoriteColor(192);
	cout << myPixel->red << endl;
	cout << myPixel->green << endl;
	cout << myPixel->blue << endl;
    cout << "Reached line 39" << endl;
    // Go over the whole image, and if a pixel differs from that to its upper
    // left, color it my favorite color in the output
    for (int y = 1; y < height; y++)
    {
        for (int x = 1; x < width; x++)
        {
            // Calculate the pixel difference
            RGBAPixel * prev = (*original)(x-1, y-1);
        //cout << "Reached line 50" << endl;
            RGBAPixel * curr = (*original)(x  , y  );
            //cout << "Reached line 52" << endl;
            int diff = abs(curr->red   - prev->red  ) +
                       abs(curr->green - prev->green) +
                       abs(curr->blue  - prev->blue );

            // If the pixel is an edge pixel,
            // color the output pixel with my favorite color
            RGBAPixel * currOutPixel = (*output)(x,y);
            if (diff > 100)
			{
                *currOutPixel = *myPixel;
			}
        }
    }

    // Save the output file
    output->writeToFile("out.png");
    // Clean up memory
    delete output;
    delete original;
}
