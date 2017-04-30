// **************************************************************
// *		   
// *  quadtree.cpp
// *		   
// *  Quadtree class
// *		   
// *  CS 225 Spring 2008
// *		   
// **************************************************************

#include "quadtree.h"
using namespace std;
//#include <iostream>

/**
 * Default constructor for Quadtree Class
 */
Quadtree::Quadtree()
{
	root = NULL;
	size = 0;
}

/**
 * Quadtree constructor with varieables
 * @param The resource image to base this Quadtree on
 * @param resolution	The width and height of the sides of the image to be represented
 */
Quadtree::Quadtree(PNG const &source, int resolution)
{
	root = NULL;
	size = resolution;
	//std::cout << "DONE" << std::endl;
	buildTree(source, resolution);

}

 /**
  * Copy constructor
  * other	The Quadtree to make a copy of
  */
Quadtree::Quadtree(Quadtree const & other)
{
	size = other.size;
	root = copy(other.root);
}

/**
 * Private helper copy function
 */
Quadtree::QuadtreeNode * Quadtree::copy(const Quadtree::QuadtreeNode *ptr)const
{
	if (ptr == NULL)
		return NULL;

	QuadtreeNode *temp = new QuadtreeNode;
	temp->element = ptr->element;
	temp->nwChild = copy(ptr->nwChild);
	temp->neChild = copy(ptr->neChild);
	temp->swChild = copy(ptr->swChild);
	temp->seChild = copy(ptr->seChild);

	return temp;
}

 /**
  * Destructor
  * frees all memory associated with this Quadtree.
  */
Quadtree::~Quadtree()
{
	clear(root);
	root = NULL;
}

 /**
  * Private clear function
  */
void Quadtree::clear(Quadtree::QuadtreeNode * ptr)const
{
	if (ptr == NULL)
		return;
	clear(ptr->nwChild);
	clear(ptr->neChild);
	clear(ptr->swChild);
	clear(ptr->seChild);

	delete ptr;
}

 /**
  * Assignment operator; frees memory associated with this Quadtree and sets its contents to be equal to the parameter's.
  * @param other	The Quadtree to make a copy of
  * @return A constant reference to the Quadtree value that was copied
  */
Quadtree const & Quadtree::operator=(Quadtree const & other)
{
	clear(root);
	size = other.size;
	root = copy(other.root);
	return *this;
}


 /**
  * Deletes the current contents of this Quadtree object, then turns it into a Quadtree object representing the upper-left d by d block of source.
  * You may assume that d is a power of two, and that the width and height of source are each at least d.
  * @param source	The source image to base this Quadtree on
  * @param resolution	The width and height of the sides of the image to be represented
  */
void Quadtree::buildTree(PNG const & source, int resolution)
{
	clear(root);
	size = resolution;
	root = build(source, 0, 0, size);
}

Quadtree::QuadtreeNode *Quadtree::build(const PNG & source, int x, int y, int d)const
{
	if (d == 1)
	{
		QuadtreeNode *temp = new QuadtreeNode;
		temp->element = *source(x, y); 
		temp->nwChild = temp->neChild = temp->swChild = temp->seChild = NULL;
		return temp;
	}

	QuadtreeNode *ret = new QuadtreeNode;

	ret->nwChild = build(source, x, y, d/2);
	ret->neChild = build(source, x+d/2, y, d/2);
	ret->swChild = build(source, x, y+d/2, d/2);
	ret->seChild = build(source, x+d/2, y+d/2, d/2);

	ret->element.red = (ret->nwChild->element.red + ret->neChild->element.red + ret->swChild->element.red + ret->seChild->element.red)/4;

	ret->element.green = (ret->nwChild->element.green + ret->neChild->element.green + ret->swChild->element.green + ret->seChild->element.green)/4;

	ret->element.blue = (ret->nwChild->element.blue + ret->neChild->element.blue + ret->swChild->element.blue + ret->seChild->element.blue)/4;
	return ret;
}

	
 /**
  * Gets the RGBAPixel corresponding to the pixel at coordinates (x, y) in the bitmap image which the Quadtree represents.
  * Note that the Quadtree may not contain a node specifically corresponding to this pixel (due, for instance, to pruning - see below). In this case, getPixel will retrieve the pixel (i.e. the color) of the square region within which the smaller query grid cell would lie. (That is, it will return the element of the nonexistent leaf's deepest surviving ancestor.) If the supplied coordinates fall outside of the bounds of the underlying bitmap, or if the current Quadtree is "empty" (i.e., it was created by the default constructor) then the returned RGBAPixel should be the one which is created by the default RGBAPixel constructor.
  * @param x	The x coordinate of the pixel to be retrieved
  * @param y	The y coordinate of the pixel to be retrieved
  * @return The pixel at the given (x, y) location
  */
RGBAPixel Quadtree::getPixel(int x, int y)const
{
	if (x >= 0 && x < size && y >= 0 && y < size)
		return get(root, x, y, size);
	else
		return RGBAPixel();
}

RGBAPixel Quadtree::get(QuadtreeNode *cRoot, int x, int y, int d)const
{
	if (cRoot == NULL)
		return RGBAPixel();
	if (cRoot->neChild == NULL || cRoot->nwChild == NULL || cRoot->seChild == NULL || cRoot->swChild == NULL)
	{
		return cRoot->element;
	}
	
	if (x <= d/2 && y <= d/2)
		return get(cRoot->nwChild, x, y, d/2);
	else if (x > d/2 && y <= d/2)
		return get(cRoot->neChild, x-d/2, y, d/2);
	else if (x <= d/2 && y >d/2)
		return get(cRoot->swChild, x, y-d/2, d/2);
	else
		return get(cRoot->seChild, x-d/2, y-d/2, d/2);
	return RGBAPixel();
}

 /**
  * Returns the underlying PNG object represented by the Quadtree.
  * If the current Quadtree is "empty" (i.e., it was created by the default constructor) then the returned PNG should be the one which is created by the default PNG constructor. This function effectively "decompresses" the Quadtree. A Quadtree object, in memory, may take up less space than the underlying bitmap image, but we cannot simply look at the Quadtree and tell what image it represents. By converting the Quadtree back into a bitmap image, we lose the compression, but gain the ability to view the image directly.
  * @return The decompressed PNG image this Quadtree represents
  */
PNG Quadtree::decompress()const
{
	if (root == NULL || size == 0)
		return PNG();

	PNG ret = PNG(size, size);
	decompressHelper(root, 0, 0, size, ret);
	return ret;
}

void Quadtree::decompressHelper(QuadtreeNode *cRoot, int x, int y, int d, PNG &canvas)const
{
	if (cRoot->neChild == NULL || cRoot->nwChild == NULL || cRoot->seChild == NULL || cRoot->swChild == NULL)
	{
		for (int i = x; i < x + d; i++)
		{
			for (int j = y; j < y + d; j++)
			{
				*(canvas(i, j)) = cRoot->element;
			}
		}
		return;
	}
	else
	{
		decompressHelper(cRoot->nwChild, x, y, d/2, canvas);
		decompressHelper(cRoot->neChild, x+d/2, y, d/2, canvas);
		decompressHelper(cRoot->swChild, x, y + d/2, d/2, canvas);
		decompressHelper(cRoot->seChild, x+d/2, y+d/2, d/2, canvas);
	}
}

 /**
  * Rotates the Quadtree object's underlying image clockwise by 90 degrees.
  * (Note that this should be done using pointer manipulation, not by attempting to swap the element fields of QuadtreeNodes. Trust us; it's easier this way.)
  */
void Quadtree::clockwiseRotate()
{
	rotate(root);
}

void Quadtree::rotate(QuadtreeNode *cRoot)const
{
	if (cRoot == NULL)
		return;

	QuadtreeNode *nw = cRoot->nwChild;
	QuadtreeNode *ne = cRoot->neChild;
	QuadtreeNode *sw = cRoot->swChild;
	QuadtreeNode *se = cRoot->seChild;

	rotate(nw);
	rotate(ne);
	rotate(sw);
	rotate(se);

	cRoot->nwChild = sw;
	cRoot->neChild = nw;
	cRoot->swChild = se;
	cRoot->seChild = ne;
}
 /**
  * Compresses the image this Quadtree represents.
  * If the color values of the leaves of a subquadtree don't vary by much, we might as well represent the entire subtree by, say, the average color value of those leaves. We may use this information to effectively "compress" the image, by strategically trimming the Quadtree.
  * Consider a node n and the subtree, Tn rooted at n, and let avg denote the component-wise average color value of all the leaves of Tn. Component-wise average means that every internal node in the tree calculates its value by averaging its immediate children. This implies that the average must be calculated in a "bottom-up" manner.
  * Due to rounding errors, using the component-wise average is not equivalent to using the true average of all leaves in a subtree. If a node n is pruned, the children of n and the subtrees for which they are the roots are removed from the Quadtree. Node n is pruned if the color value of no leaf in Tn, differs from avg by more than tolerance. (Note: for all average calculations, just truncate the value to integer.)
  * We define the "difference" between two colors, (r1,g1,b1) and (r2,g2,b2), to be (r2−r1)^2+(g2−g1)^2+(b2−b1)^2.
  * To be more complete, if the tolerance condition is met at a node n, then the block of the underlying image which n represents contains only pixels which are "nearly" the same color. For each such node n, we remove from the Quadtree all four children of n, and their respective subtrees (an operation we call a pruning). This means that all of the leaves that were deleted, corresponding to pixels whose colors were similar, are now replaced by a single leaf with color equal to the average color over that square region.
  * The prune function, given a tolerance value, prunes the Quadtree as extensively as possible. (Note, however, that we do not want the prune function to do an "iterative" prune. It is conceivable that by pruning some mid-level node n, an ancestor p of n then becomes prunable, due to the fact that the prune changed the leaves descended from p. Your prune function should evaluate the prunability of each node based on the presence of all nodes, and then delete the subtrees based at nodes deemed prunable.)
  * Note: You should start pruning from the root of the Quadtree.
  * @param tolerance	The integer tolerance between two nodes that determines whether the subtree can be pruned.
  */
void Quadtree::prune(int tolerance)
{
	pruneHelper(root, tolerance);
}

void Quadtree::pruneHelper(QuadtreeNode *temp, int tolerance)const
{
	static int count = 0;
	if (temp == NULL)
		return;
	if (prunable(temp, tolerance, temp))
	{
		clear(temp->nwChild);
		clear(temp->neChild);
		clear(temp->swChild);
		clear(temp->seChild);

		temp->nwChild = NULL;
		temp->neChild = NULL;
		temp->swChild = NULL;
		temp->seChild = NULL;
		return;
	}

	
	pruneHelper(temp->nwChild, tolerance);
	pruneHelper(temp->neChild, tolerance);
	pruneHelper(temp->swChild, tolerance);
	pruneHelper(temp->seChild, tolerance);

}

bool Quadtree::prunable(QuadtreeNode *temp, int tolerance, QuadtreeNode *avg)const
{
	if (temp == NULL)
		return true;
	if (temp->nwChild != NULL)
		return (prunable(temp->nwChild, tolerance, avg) && prunable(temp->neChild, tolerance, avg) && prunable(temp->swChild, tolerance, avg) && prunable(temp->seChild, tolerance, avg));

	RGBAPixel px1 = temp->element;
	RGBAPixel px2 = avg->element;
	int dif = 0;
	dif += (px1.red - px2.red)*(px1.red - px2.red);
	dif += (px1.green - px2.green)*(px1.green - px2.green);
	dif += (px1.blue - px2.blue)*(px1.blue - px2.blue);
	if (dif > tolerance)
		return false;

	return true;
}

 /**
  * This function is similar to prune; however, it does not actually prune the Quadtree.
  * Rather, it returns a count of the total number of leaves the Quadtree would have if it were pruned as in the prune function.
  * @param tolerance	The integer tolerance between two nodes that determines whether the subtree can be pruned.
  * @return How many leaves this Quadtree would have if it were pruned with the given tolerance.
  */
int Quadtree::pruneSize(int tolerance)const
{
	return pruneSizeHelper(root, tolerance);
}

int Quadtree::pruneSizeHelper(QuadtreeNode *node, int tolerance)const
{
	if (node == NULL)
		return 0;
	if (node->nwChild == NULL || prunable(node, tolerance, node))
		return 1;
	return (pruneSizeHelper(node->nwChild, tolerance) + pruneSizeHelper(node->neChild, tolerance) + pruneSizeHelper(node->swChild, tolerance) + pruneSizeHelper(node->seChild, tolerance));
}

 /**
  * Calculates and returns the minimum tolerance necessary to guarantee that upon pruning the tree, no more than numLeaves leaves remain in the Quadtree.
  * Essentially, this function is an inverse of pruneSize; for any Quadtree object theTree, and for any positive integer tolerance it should be true that
  * theTree.pruneSize(theTree.idealPrune(numLeaves)) <= numLeaves 
  * Once you understand what this function is supposed to do, you will probably notice that there is an "obvious" implementation. This is probably not the implementation you want to use! There is a fast way to implement this function, and a slow way; you will need to find the fast way. (If you doubt that it makes a significant difference, the tests in the given main.cpp should convince you.)
  * @param numLeaves	The number of leaves you want to remain in the tree after prune is called.
  * @return The minimum tolerance needed to guarantee that there are no more than numLeaves remaining in the tree.
  * Note: The "obvious" implementation involves a sort of linear search over all possible tolerances. What if you tried a binary search instead?
  */
int Quadtree::idealPrune(int numLeaves)const
{
	int max = 3 * 255 * 255;
	int min = 0;
	int tolerance = (max + min)/2;
	int curLeaves = 0;
	int lastVal = max;

	while (tolerance > min && tolerance < max)
	{
		curLeaves = pruneSize(tolerance);

		if (curLeaves > numLeaves)
			min = tolerance;
		else
		{
			lastVal = tolerance;
			max = tolerance;
		}
		tolerance = (max + min)/2;
	}
	if (lastVal == 1)
		lastVal = 0;
	return lastVal;
}

