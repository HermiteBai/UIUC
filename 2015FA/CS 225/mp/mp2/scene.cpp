#include "scene.h"
#include <iostream>
using namespace std;
/**
 * parameter: 
 * 			int max
 * 			int *x
 * 			int *y
 * 			Image **images
 **/

//constructor that sets the max number of images for the scene
Scene::Scene(int max)
{
	if (max > 0)
	{
		this->max = max;
		x = new int[max];
		y = new int[max];
		images = new Image*[max];
		for (int i = 0; i < max; i++)
		{
			images[i] = NULL;
		}
	}
}
//standard destructor
Scene::~Scene()
{
	for (int i = 0; i < max; i++)
	{	
		if (images[i] != NULL)
		{
			delete images[i];
			images[i] = NULL;
		}
	}
	delete[] images;
	images = NULL;
	delete[] x;
	x = NULL;
	delete[] y;
	y = NULL;
}
//copy constructor makes independent copy of source
Scene::Scene(const Scene &source)
{
	this->max = source.max;
	x = new int[max];
	y = new int[max];
	images = new Image*[max];
	for (int i = 0; i < max; i++)
	{
		x[i] = source.x[i];
		y[i] = source.y[i];
		if (source.images[i] != NULL)
		{
			images[i] = new Image(*source.images[i]);
		}
		else
		{
			images[i] = NULL;
		}
	}
}
//assignment operator for scene class
const Scene & Scene:: operator=(const Scene &source)
{
	delete[] x;
	x = NULL;
	delete[] y;
	y = NULL;
	for (int i = 0; i < max; i++)
	{
		if (images[i] != NULL)
		{
			delete images[i];
			images[i] = NULL;
		}
	}
	delete images;
	this->max = source.max;
	this->x = new int[max];
	this->y = new int[max];
	this->images = new Image*[max];

	for (int i = 0; i < max; i++)
	{
		x[i] = source.x[i];
		y[i] = source.y[i];
		if (source.images[i] != NULL)
			images[i] = new Image(*source.images[i]);
		else
			images[i] = NULL;
	}
	return *this;
}
//modifies the size of the array of Image pointers without changing their indices
void Scene::changemaxlayers(int newmax)
{
	if(newmax >= 0)
	{
		Scene temp(newmax);
		for (int i = 0; i < max; i++)
		{
			if (images[i] != NULL && i < newmax)
			{
				temp.x[i] = this->x[i];
				temp.y[i] = this->y[i];
				temp.images[i] = this->images[i];
			}
			else
			{
				delete[] temp.x;
				delete[] temp.y;
				delete[] temp.images;
			}
		}
		delete[] x;
		this->x = temp.x;
		delete[] y;
		this->y = temp.y;
		delete[] images;
		this->images = temp.images;
		this->max = max;
	}
}
//adds a picture to scene by placing in cell at given index and storing its x and y coords
void Scene::addpicture(const char *FileName, int index, int x, int y)
{
	if (index < 0 || index >= max)
		cout << "index out of bounds" << endl;
	else
	{
		Image * picture = new Image();
		if (!picture->readFromFile(FileName))
		{
			delete picture;
		}
		else
		{
			if (images[index] != NULL)
			{
				delete images[index];
				images[index] = NULL;
			}
			images[index] = picture;
			this->x[index] = x;
			this->y[index] = y;
			picture = NULL;	
		}
	}
}
//Moves an Image from one layer to another
void Scene::changelayer(int index, int newindex)
{
	if (index < 0 || index >= max || newindex < 0 || newindex >= max)
	{
		cout << "invalid index" << endl;
	}
	else
	{
		if (index != newindex)
		{
			if (images[newindex] != NULL)
			{
				delete images[newindex];
				images[newindex] = NULL;
			}
			images[newindex] = images[index];
			x[newindex] = x[index];
			y[newindex] = y[index];
			if (images[index] != NULL)
				images[index] = NULL;
		}
	}
}
//Changes the x and y coordinates of the Image in the specified layer
void Scene::translate(int index, int xcoord, int ycoord)
{
	if (index < 0 || index >= max)
		cout << "invalid index" << endl;
	else
	{
		this->x[index] = xcoord;
		this->y[index] = ycoord;
	}
}
//deletes the image at the given index
void Scene::deletepicture(int index)
{
	if (index < 0 || index >= max)
		cout << "invalid index" << endl;
	else
	{
		if (images[index] != NULL)
		{
			delete images[index];
			images[index] = NULL;
		}
	}
}
//returns a pointer to the image at the index, not a copy
Image * Scene::getpicture(int index)const
{
	if (index < 0 || index >= max)
	{
		cout << "invalid index" << endl;
		return NULL;
	}
	else
		return images[index];
}
//draws the whole scene on one image and returns that image by value
Image Scene::drawscene()const
{
	size_t maxwidth = 0;
	size_t maxheight = 0;
	for (int i = 0; i < max; i++)
	{
		if (images[i] != NULL)
		{
			if((images[i]->width()+x[i]) > (unsigned int)maxwidth)
				maxwidth = images[i]->width() + x[i];
			if((images[i]->height() + y[i]) > (unsigned int)maxheight)
				maxheight = images[i]->height() + y[i];
		}
	}
	Image temp = Image();
	temp.resize(maxwidth, maxheight);
	for (int i = 0; i < max; i++)
	{
		if (images[i] != NULL)
		{
			for (int j = x[i]; j < (x[i] + images[i]->width()); j++)
			{
				for (int k = y[i]; k < (y[i] + images[i]->height()); k++)
				{
					*((&temp)->operator()(j,k)) = *(images[i]->operator()(j-x[i], k-y[i]));
				}
			}
		}
	}
	return temp;
}

