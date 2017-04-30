/**
 * @file maptiles.cpp
 * Code for the maptiles function.
 */
	 			
#include <iostream>
#include <map>
#include "maptiles.h"

using namespace std;

MosaicCanvas * mapTiles(SourceImage const & theSource, vector<TileImage> const & theTiles)
{
    /**
     * @todo Implement this function!
     */

	int row = theSource.getRows();
	int col = theSource.getColumns();

	MosaicCanvas* ret = new MosaicCanvas(row, col);
	vector<Point<3> > pointS;
	map<Point<3>, TileImage> pointToPixel;
	RGBAPixel color;
	Point<3> point;
	
	for (size_t i = 0;i < theTiles.size(); i++)
	{
		color = theTiles[i].getAverageColor();
		point = Point<3>(color.red, color.green, color.blue);
		pointS.push_back(point);
		pointToPixel[point] = theTiles[i];
	}

	KDTree<3> tree(pointS);
	for (int x = 0; x < row; x++)
	{
		for (int y = 0; y < col; y++)
		{
			color = theSource.getRegionColor(x,y);
			point = Point<3>(color.red, color.green, color.blue);
			point = tree.findNearestNeighbor(point);
			ret->setTile(x, y, pointToPixel[point]);
		}
	}
	return ret;
}
