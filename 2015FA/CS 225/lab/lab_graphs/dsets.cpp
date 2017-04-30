/* Your code here! */
#include "dsets.h"
using namespace std;

/**
 * Creates n unconnected root nodes at the end of the vector.
 * @param num	The number of nodes to create
 */
void DisjointSets::DisjointSets::addelements(int num)
{
	for (int i = 0; i < num; i++)
	{
		roots.push_back(-1);
	}
}

/**
 * This function should compress paths and works as described in lecture.
 * @return the index of the root of the up-tree in which the parameter element resides.
 */
int DisjointSets::find(int elem)
{
	if (roots[elem] < 0)
		return elem;
	else
		return find(roots[elem]);
}

/**
 * union two sets by size
 * @param a, b index
 */
void DisjointSets::setunion(int a, int b)
{
	int set1 = find(a);
	int set2 = find(b);
	if (set1 == set2)
		return;
	int size = roots[set1]+roots[set2];
	if (roots[set1] <= roots[set2])
	{
		roots[set2] = set1;
		roots[set1] = size;
	}
	else
	{
		roots[set1] = set2;
		roots[set2] = size;
	}
}

void DisjointSets::erase()
{
	roots.erase(roots.begin(), roots.end());
}

