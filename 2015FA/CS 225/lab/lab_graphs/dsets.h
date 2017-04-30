/* Your code here! */
#ifndef _DSET_H
#define _DSET_H
#include <vector>
using std::vector;
class DisjointSets
{
	public:

		void addelements(int num);

		int find(int elem);

		void setunion(int a, int b);

		void erase();


	private:

		vector<int> roots;

};
#endif
