/* Your code here! */
#ifndef _MAZE_H
#define _MAZE_H
#include "dsets.h"
#include "png.h"
#include <vector>
using std::vector;
class SquareMaze
{
	public:
		void makeMaze(int width, int height);
		bool canTravel(int x, int y, int dir)const;
		void setWall(int x, int y, int dir, bool exists);
		vector<int> solveMaze();
		PNG* drawMaze()const;
		PNG* drawMazeWithSolution();

	private:
		DisjointSets maze;
		int width;
		int height;
		struct square
		{
			bool right;
			bool down;
			square()
			{
				right = true;
				down = true;
			}
		};
		vector<square> squares;
		bool DFS(int curr, int dest, vector<int> &solution);
		vector<int> DFSShell(int curr, int dest);
		bool isConnected(int num1, int num2);
		size_t solutionSize(int curr, int dest);
};

#endif

