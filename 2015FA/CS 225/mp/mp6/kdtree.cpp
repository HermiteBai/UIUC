/**
 * @file kdtree.cpp
 * Implementation of KDTree class.
 */

template<int Dim>
bool KDTree<Dim>::smallerDimVal(const Point<Dim> & first, const Point<Dim> & second, int curDim) const
{
    /**
     * @todo Implement this function!
     */
	if (first[curDim] == second[curDim])
    	return (second > first);
	else
		return (second[curDim] > first[curDim]);
}


template<int Dim>
bool KDTree<Dim>::shouldReplace(const Point<Dim> & target, const Point<Dim> & currentBest, const Point<Dim> & potential) const
{
    /**
     * @todo Implement this function!
     */
	int currDist = 0;
	int poteDist = 0;
	for (int i = 0; i < Dim; i++)
	{
		currDist += (currentBest[i]-target[i])*(currentBest[i]-target[i]);
		poteDist += (potential[i]-target[i])*(potential[i]-target[i]);
	}

	if (currDist == poteDist)
		return (potential < currentBest);
    return (currDist > poteDist);
}

template<int Dim>
KDTree<Dim>::KDTree(const vector< Point<Dim> > & newPoints)
{
    /**
     * @todo Implement this function!
     */
	points = newPoints;
	build(0, (int)(points.size()-1), points, 0);
}

template<int Dim>
void KDTree<Dim>::build(int start, int end, vector<Point<Dim> > & source, int dimension)
{
	if (start > end)
		return;
	points[(start + end)/2] = select(source, start, end, (start+end)/2, dimension);
	build(start, (start+end)/2 - 1, source, (dimension+1)%Dim);
	build((start+end)/2 + 1, end, source, (dimension+1)%Dim);
}

template<int Dim>
Point<Dim> KDTree<Dim>::select(vector< Point<Dim> > & list, int start, int end, int k, int dimension)
{
	while(start < end)
	{
		int pivot = partition(list, start, end, (start + end)/2, dimension);

		if (pivot == k)
			return list[pivot];
		else if (k < pivot)
			end = pivot - 1;
		else
			start = pivot + 1;
	}
	return list[start];
}

template<int Dim>
int KDTree<Dim>::partition(vector< Point<Dim> > & list, int start, int end, int pivot, int dimension)
{
	Point<Dim> pivotNum = list[pivot];
	swap(list[pivot], list[end]);
	int x = start;
	int i = start;
	while (i < end)
	{
		if (smallerDimVal(list[i], pivotNum, dimension))
		{
			swap(list[i], list[x]);
			x++;
		}
		i++;
	}
	swap(list[end], list[x]);
	return x;
}

template<int Dim>
void KDTree<Dim>::swap(Point<Dim> &a, Point<Dim> &b)
{
	Point<Dim> temp = a;
	a = b;
	b = temp;
}

template<int Dim>
Point<Dim> KDTree<Dim>::findNearestNeighbor(const Point<Dim> & query) const
{
    /**
     * @todo Implement this function!
     */
    return find(0, points.size()-1, query, 0);
}

template<int Dim>
Point<Dim> KDTree<Dim>::find(int start, int end, const Point<Dim> &query, int dimension) const
{
	int mid = (start + end)/2;

	if (start >= end)
		return points[start];

	Point<Dim> currBest;

	if (smallerDimVal(points[mid], query, dimension))
		currBest = find(mid + 1, end, query, (dimension+1)%Dim);
	else 
		currBest = find(start, mid - 1, query, (dimension+1)%Dim);
		
	if (shouldReplace(query, currBest, points[mid]))
		currBest = points[mid];

	int radius = distance(currBest, query);

	int distance = (query[dimension] - points[mid][dimension])*(query[dimension] - points[mid][dimension]);

	if (distance <= radius)
	{
		Point<Dim> temp;

		if (smallerDimVal(points[mid], query, dimension))
			temp = find(start, mid - 1, query, (dimension+1)%Dim);
		else
			temp = find(mid + 1, end, query, (dimension+1)%Dim);

		if (shouldReplace(query, currBest, temp))
			currBest = temp;
	}
	return currBest;
}

template<int Dim>
int KDTree<Dim>::distance(const Point<Dim> &a, const Point<Dim> &b) const
{
	int total = 0;
	for (int x = 0; x < Dim; x++)
	{
		total += (a[x]-b[x])*(a[x]-b[x]);
	}
	return total;
}
