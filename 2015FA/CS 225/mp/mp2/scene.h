#include "image.h"
#ifndef _SCENE_H_
#define _SCENE_H_

class Scene
{
public:
	//constructor that sets the max number of images for the scene
	Scene(int max);
	//standard destructor
	~Scene();
	//copy constructor makes independent copy of source
	Scene(const Scene &source);
	//assignment operator for scene class
	const Scene & operator=(const Scene &source);
	//modifies the size of the array of Image pointers without changing their indices
	void changemaxlayers(int newmax);
	//adds a picture to scene by placing in cell at given index and storing its x and y coords
	void addpicture(const char *FileName, int index, int x, int y);
	//Moves an Image from one layer to another
	void changelayer(int index, int newindex);
	//Changes the x and y coordinates of the Image in the specified layer
	void translate(int index, int xcoord, int ycoord);
	//deletes the image at the given index
	void deletepicture(int index);
	//returns a pointer to the image at the index, not a copy
	Image * getpicture(int index)const;
	//draws the whole scene on one image and returns that image by value
	Image drawscene()const;

private:
	int max;
	int * x;
	int * y;
	Image** images;
};

#endif
