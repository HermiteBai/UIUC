/* Your code here! */
#include "maze.h"
#include <vector>
//#include <pair>
#include <stack>
using std::vector;
//using std::pair;
using std::stack;

//Makes a new SquareMaze of the given height and width.
void SquareMaze::makeMaze(int width, int height)
{
	if (width < 0 || height < 0)
		return;
	this->width = width;
	this->height = height;
	
	if (!squares.empty())
	{
		for (int i = 0; i < width * height; i++)
		{
			maze.erase();
			squares.erase(squares.begin(), squares.end());
		}
	}


	maze.addelements(width * height);

	for (int i = 0; i < width * height; i++)
	{
		squares.push_back(square());
	}

	vector<int> randomArr;
	for (int i = 0; i < width * height * 2; i++)
	{
		randomArr.push_back(i);	
	}

	srand(time(NULL));
	std::random_shuffle(randomArr.begin(), randomArr.end());
	
	for (vector<int>::iterator itr = randomArr.begin(); itr != randomArr.end(); itr++)
	{
	//1.generate random number within the range
		int randomNum = *itr % squares.size();
	//2.determine whether to tear it's right or down wall
		int randomDir = *itr / squares.size();
	//4.if is removable, remove it by union two seperated disjoint sets
	//  vice versa
		if ((randomDir == 0 && (randomNum + 1) % width != 0) || (randomDir == 1 && randomNum < (height - 1) * width))
		{
			if (!isConnected(randomNum, randomDir == 0 ? (randomNum + 1) : (randomNum + width)))
			{
				maze.setunion(randomNum, randomDir == 0 ? (randomNum + 1) : (randomNum + width));
				if (randomDir == 0)
					squares[randomNum].right = false;
				else
					squares[randomNum].down = false;
			}
		}
	}
}
//This uses your representation of the maze to determine whether it is possible to travel in the given direction from the square at coordinates (x,y).
//
//For example, after makeMaze(2,2), the possible input coordinates will be (0,0), (0,1), (1,0), and (1,1).
//
//dir = 0 represents a rightward step (+1 to the x coordinate)
//dir = 1 represents a downward step (+1 to the y coordinate)
//dir = 2 represents a leftward step (-1 to the x coordinate)
//dir = 3 represents an upward step (-1 to the y coordinate)
bool SquareMaze::canTravel(int x, int y, int dir)const
{
	if (x < 0 || y < 0 || x >= width || y >= height)
		return false;
	if (dir == 0)
	{
		if (squares[y * width + x].right == false && x < width - 1)
			return true;
		else
			return false;
	}
	else if (dir == 1)
	{
		if (squares[y * width + x].down == false && y < height - 1)
			return true;
		else
			return false;
	}
	else if (dir == 2)
	{
		if (x >= 1 && squares[y * width + x - 1].right == false)
			return true;
		else
			return false;
	}
	else
	{
		if (y >= 1 && squares[y * width + x - width].down == false)
			return true;
		else
			return false;
	}
}
//Sets whether or not the specified wall exists.
//
//This function should be fast (constant time). You can assume that in grading we will not make your maze a non-tree and then call one of the other member functions. setWall should not prevent cycles from occurring, but should simply set a wall to be present or not present. Our tests will call setWall to copy a specific maze into your implementation
void SquareMaze::setWall(int x, int y, int dir, bool exists)
{
	if (dir == 0)
		squares[y * width + x].right = exists;
	else if (dir == 1)
		squares[y * width + x].down = exists;
	else
		return;
}

//Solves this SquareMaze.
//
//For each square on the bottom row (maximum y coordinate), there is a distance from the origin (i.e. the top-left cell), which is defined as the length (measured as a number of steps) of the only path through the maze from the origin to that square.
//
//Select the square in the bottom row with the largest distance from the origin as the destination of the maze. solveMaze() returns the winning path from the origin to the destination as a vector of integers, where each integer represents the direction of a step, using the same encoding as in canTravel().
//
//If multiple paths of maximum length exist, use the one with the destination cell that has the smallest x value.
//
//Hint: this function should run in time linear in the number of cells in the maze.
vector<int> SquareMaze::solveMaze()
{
	vector<size_t > sizes;

	for (int i = 0; i < width; i++)
	{
		int dest = width * (height-1) + i;
		sizes.push_back(solutionSize(0, dest));
	}

	int max = 0;
	for (int i = 1; i < width; i++)
	{
		if (sizes[i] > sizes[max])
			max = i;
	}
	return DFSShell(0, width * (height-1) + max);
}

//Helper function to solve the maze
vector<int> SquareMaze::DFSShell(int curr, int dest)
{
	vector<int> solution;
	DFS(curr, dest, solution);
	return solution;
}

size_t SquareMaze::solutionSize(int curr, int dest)
{
	return DFSShell(curr, dest).size();
}

//Draws the maze without the solution.
//
//First, create a new PNG. Set the dimensions of the PNG to (width*10+1,height*10+1). where height and width were the arguments to makeMaze. Blacken the entire topmost row and leftmost column of pixels, except the entrance (1,0) through (9,0). For each square in the maze, call its maze coordinates (x,y). If the right wall exists, then blacken the pixels with coordinates ((x+1)*10,y*10+k) for k from 0 to 10. If the bottom wall exists, then blacken the pixels with coordinates (x*10+k, (y+1)*10) for k from 0 to 10.
//
//The resulting PNG will look like the sample image, except there will be no exit from the maze and the red line will be missing.
PNG* SquareMaze::drawMaze()const
{
	PNG *canvas = new PNG(width * 10 + 1, height * 10 + 1);

	//blacken edges
	for (int i = 0; i < 10*height; i++)
	{
		(*canvas)(0, i)->red = 0;
		(*canvas)(0, i)->green = 0;
		(*canvas)(0, i)->blue = 0;
	}


	for (int i = 10; i < 10*width; i++)
	{
		(*canvas)(i, 0)->red = 0;
		(*canvas)(i, 0)->green = 0;
		(*canvas)(i, 0)->blue = 0;
	}

	//blacken squares
	for (size_t i = 0; i < squares.size(); i++)	
	{
		if (squares[i].right)
		{
			for (int j = 0; j <= 10; j++)
			{
				int x = (i % width + 1)*10;
				int y = (i / width) * 10 + j;
				(*canvas)(x, y)->red = 0;
				(*canvas)(x, y)->green = 0;
				(*canvas)(x, y)->blue = 0;
			}
		}
		if (squares[i].down)
		{
			for (int j = 0; j <= 10; j++)
			{
				int y = (i / width + 1)*10;
				int x = (i % width) * 10 + j;
				(*canvas)(x, y)->red = 0;
				(*canvas)(x, y)->green = 0;
				(*canvas)(x, y)->blue = 0;
			}
		}
	}
	for (int i = 1; i < 10; i++)
	{
		(*canvas)(i, 0)->red = 255;
		(*canvas)(i, 0)->green = 255;
		(*canvas)(i, 0)->blue = 255;
	}
	return canvas;
}

//This function calls drawMaze, then solveMaze; it modifies the PNG from drawMaze to show the solution vector and the exit.
//
//Start at pixel (5,5). Each direction in the solution vector corresponds to a trail of 11 red pixels in the given direction. If the first step is downward, color pixels (5,5) through (5,15) red. (Red is 255,0,0 in RGB). Then if the second step is right, color pixels (5,15) through (15,15) red. Then if the third step is up, color pixels (15,15) through (15,5) red. Continue in this manner until you get to the end of the solution vector, so that your output looks analogous the above picture.
//
//Make the exit by undoing the bottom wall of the destination square: call the destination maze coordinates (x,y), and whiten the pixels with coordinates (x*10+k, (y+1)*10) for k from 1 to 9.
PNG* SquareMaze::drawMazeWithSolution()
{
	PNG* canvas = drawMaze();
//solution...
	vector<int> solution = solveMaze();
	int currX = 5;
	int currY = 5;

	for (size_t i = 0; i < solution.size(); i++)
	{	
		if (solution[i] == 0)
		{
			for (int j = 0; j <= 10; j++)
			{
				(*canvas)(currX, currY)->red = 255;
				(*canvas)(currX, currY)->green = 0;
				(*canvas)(currX, currY)->blue = 0;
				currX++;
			}
			currX--;
		}
		else if (solution[i] == 1)
		{
			for (int j = 0; j <= 10; j++)
			{
				(*canvas)(currX, currY)->red = 255;
				(*canvas)(currX, currY)->green = 0;
				(*canvas)(currX, currY)->blue = 0;
				currY++;
			}
			currY--;
		}
		else if (solution[i] == 2)
		{
			for (int j = 0; j <= 10; j++)
			{
				(*canvas)(currX, currY)->red = 255;
				(*canvas)(currX, currY)->green = 0;
				(*canvas)(currX, currY)->blue = 0;
				currX--;
			}
			currX++;
		}
		else
		{
			for (int j = 0; j <= 10; j++)
			{
				(*canvas)(currX, currY)->red = 255;
				(*canvas)(currX, currY)->green = 0;
				(*canvas)(currX, currY)->blue = 0;
				currY--;
			}
			currY++;
		}
	}

	currX /= 10;

	for (int j = 1; j < 10; j++)
	{
		(*canvas)(currX*10 + j, 10*height)->red = 255;
		(*canvas)(currX*10 + j, 10*height)->green = 255;
		(*canvas)(currX*10 + j, 10*height)->blue = 255;
	}

	return canvas;
}


bool SquareMaze::isConnected(int num1, int num2)
{
	return (maze.find(num1) == maze.find(num2));
}

//1. maybe just push the number of the nodes in the vector of solution?
//2. got to figure out why access validDir would cause an heap buffer overflow
//Another helper funtion to solve the maze by using DFS
bool SquareMaze::DFS(int curr, int dest, vector<int> &solution)
{
	//   3
	// 2   0
	//   1
	if (curr == dest)
		return true;
	int x = curr % width;
	int y = curr / width;
	for (int dir = 0; dir < 4; dir++)
	{
		if (canTravel(x, y, dir))
		{
			solution.push_back(dir);
			if (dir == 0)
			{
				setWall(x, y, 0, true);
				if (DFS(curr+1, dest, solution))
				{
					setWall(x, y, 0, false);
					return true;
				}
				solution.pop_back();	
				setWall(x, y, 0, false);
			}
			else if (dir == 1)
			{
				setWall(x, y, 1, true);
				if (DFS(curr + width, dest, solution))
				{
					setWall(x, y, 1, false);
					return true;
				}
				solution.pop_back();
				setWall(x, y, 1, false);
			}
			else if (dir == 2)
			{
				setWall(x-1, y, 0, true);
				if (DFS(curr-1, dest, solution))
				{
					setWall(x-1, y, 0, false);
					return true;
				}
				solution.pop_back();
				setWall(x-1, y, 0, false);
			}
			else
			{
				setWall(x, y-1, 1, true);
				if (DFS(curr-width, dest, solution))
				{
					setWall(x, y-1, 1, false);
					return true;
				}
				solution.pop_back();
				setWall(x, y-1, 1, false);
			}
		}
	}
	return false;
}

