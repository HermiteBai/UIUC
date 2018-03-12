#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>

#include "2048.h"

#define USAGE "Usage: ./2048 <size>"

bool death = 0;
int** battlefield = NULL;
int size = 0;
int Score = 0;
int blank;
int flag = 0;
int recentlyAdd = 0;

int main(int argc, char const *argv[])
{
	system("clear");
	if (argc != 2)
	{
		puts(USAGE);
		exit(1);
	}
	system("clear");
	printf("Guide: Use w/s/a/d to control, q to quit\n");
	printf("<Click any key to continue>\n");
	char known = getchar();
	system("clear");
	start:
	recentlyAdd = 0;
	system("stty cooked");
	battlefield = init(atoi(argv[1]));
	addhelper(atoi(argv[1]));
	blank--;
	print();
	printf("Score: %d, Blank Left: %d\n", Score, blank);
	run();
	clean();
	char restart;
	printf("Do you want to restart the game?(y/n)\n");
	system("stty raw");
	restart = getchar();

	if (restart == 'y')
	{
		system("clear");
		Score = 0;
		goto start;
	}
	system("clear");
	system("stty cooked");
	return 0;
}

int** init(int scale)
{
	int** ret = (int**)calloc(scale, sizeof(int*));
	for (int i = 0; i < scale; i++)
	{
		ret[i] = (int*)calloc(scale, sizeof(int));
	}
	size = scale;
	blank = size * size;
	return ret;
}

void run()
{
	char operation;
	system("stty raw");
	while ((operation = getchar()) != EOF)
	{
		system("stty cooked");
		int death = 0;
		int success = 0;
		switch (operation)
		{
			case 'w':
				success = up();
				break;
			case 'a':
				success = left();
				break;
			case 'd':
				success = right();
				break;
			case 's':
				success = down();
				break;
			case 'q':
				system("clear");
				printf("Quit Game 2048 with Score: %d\n", Score);
				return;
			default:
				continue;
		}
		if (success)
		{
			system("clear");
			printf("Congratulations! You got score: %d\n", Score);
			return;
		}

		if (flag)
		{
			death = addnew();
			flag = 0;
			if (death)
			{
				system("clear");
				printf("GAME OVER with Score: %d\n", Score);
				return;
			}
		}
		flag = 0;
		print();
		printf("Score: %d, Blank Left: %d\n", Score, blank);
		system("stty raw");
	}
}

int up()
{
	for (int i = 1; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (battlefield[i][j] == 0)
			{
				continue;
			}
			else
			{
				flag = 1;
				while (i >= 1 && (battlefield[i - 1][j] == 0 || battlefield[i][j] == battlefield[i - 1][j]))
				{
					if (battlefield[i - 1][j] == 0)
					{
						battlefield[i - 1][j] = battlefield[i][j];
						battlefield[i][j] = 0;
					}
					else
					{
						Score += battlefield[i - 1][j];
						battlefield[i - 1][j] *= 2;
						battlefield[i][j] = 0;
						blank++;
						if (battlefield[i - 1][j] == 2048)
						{
							print();
							return 1;
						}
					}
					i--;
					print();
				}
			}
		}
	}
	return 0;
}

int left()
{
	for (int i = 1; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (battlefield[j][i] == 0)
			{
				continue;
			}
			else
			{
				flag = 1;
				while (i >= 1 && (battlefield[j][i - 1] == 0 || battlefield[j][i] == battlefield[j][i - 1]))
				{
					if (battlefield[j][i - 1] == 0)
					{
						battlefield[j][i - 1] = battlefield[j][i];
						battlefield[j][i] = 0;
					}
					else
					{
						Score += battlefield[j][i - 1];
						battlefield[j][i - 1] *= 2;
						battlefield[j][i] = 0;
						blank++;
						if (battlefield[j][i - 1] == 2048)
						{
							print();
							return 1;
						}
					}
					i--;
					print();
				}
			}
		}
	}
	return 0;
}

int right()
{
	for (int i = size - 2; i >= 0; i--)
	{
		for (int j = 0; j < size; j++)
		{
			if (battlefield[j][i] == 0)
			{
				continue;
			}
			else
			{
				flag = 1;
				while (i <= size - 2 && (battlefield[j][i + 1] == 0 || battlefield[j][i] == battlefield[j][i + 1]))
				{
					if (battlefield[j][i + 1] == 0)
					{
						battlefield[j][i + 1] = battlefield[j][i];
						battlefield[j][i] = 0;
					}
					else
					{
						Score += battlefield[j][i + 1];
						battlefield[j][i + 1] *= 2;
						battlefield[j][i] = 0;
						blank++;
						if (battlefield[j][i + 1] == 2048)
						{
							print();
							return 1;
						}
					}
					i++;
					print();
				}
			}
		}
	}
	return 0;
}

int down()
{
	for (int i = size - 2; i >= 0; i--)
	{
		for (int j = 0; j < size; j++)
		{
			if (battlefield[i][j] == 0)
			{
				continue;
			}
			else
			{
				flag = 1;
				while (i <= size - 2 && (battlefield[i + 1][j] == 0 || battlefield[i][j] == battlefield[i + 1][j]))
				{
					if (battlefield[i + 1][j] == 0)
					{
						battlefield[i + 1][j] = battlefield[i][j];
						battlefield[i][j] = 0;
					}
					else
					{
						Score += battlefield[i + 1][j];
						battlefield[i + 1][j] *= 2;
						battlefield[i][j] = 0;
						blank++;
						if (battlefield[i + 1][j] == 2048)
						{
							print();
							return 1;
						}
					}
					i++;
					print();
				}
			}
		}
	}
	return 0;
}

void print()
{
	system("clear");
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
			printf("+------");
		printf("+\n");
		for (int j = 0; j < size; j++)
		{
			color(battlefield[i][j]);
			//printf("| %4d ", battlefield[i][j]);
		}
		printf("|\n");
	}
	//printf(" ");
	for (int i = 0; i < size; i++)
		printf("+------");
	printf("+\n");
	usleep(40000);
}

void addhelper()
{
	srand(time(0));
	int x = (rand() % size);
	int y = (rand() % size);
	while (battlefield[y][x] != 0)
	{
		x = (rand() % size);
		y = (rand() % size);
	}
	battlefield[y][x] = ((rand() % 10 == 9) ? 4 : 2);
	recentlyAdd = battlefield[y][x];
}

int addnew()
{
	if (flag == 0)
		return 0;
	if (blank <= 0)
		return 1;
	else
	{
		addhelper();
		blank--;
		return 0;
	}	
}

void clean()
{
	for (int i = 0; i < size; i++)
	{
		free(battlefield[i]);
	}
	free(battlefield);
}

void color(int num)
{
	switch(num)
	{
		case 0:
			printf("|\33[37m      \33[0m");
			break;
		case 2:
			printf("|\33[37m %4d \33[0m", num);
			break;
		case 4:
			printf("|\33[37m %4d \33[0m", num);
			break;
		case 8:
			printf("|\33[33m %4d \33[0m", num);
			break;
		case 16:
			printf("|\33[32m %4d \33[0m", num);
			break;
		case 32:
			printf("|\33[36m %4d \33[0m", num);
			break;
		case 64:
			printf("|\33[34m %4d \33[0m", num);
			break;
		case 128:
			printf("|\33[34m %4d \33[0m", num);
			break;
		case 256:
			printf("|\33[34m %4d \33[0m", num);
			break;
		case 512:
			printf("|\33[34m %4d \33[0m", num);
			break;
		case 1024:
			printf("|\33[35m %4d \33[0m", num);
			break;
		case 2048:
			printf("|\33[31m %4d \33[0m", num);
			break;
		default:
			break;

	}
}

char nextCommand()
{

	int primary = setPrimary();
	if (primary == -1)
		return 'q';
	if (recentlyAdd == 4)
		return 's';
	if (recentlyAdd == 0)
		return 's';
	return 'd';
}

int isFull(int col)
{
	for (int i = 0; i < size; i++)
	{
		if (battlefield[i][col] == 0)
			return 0;
	}
	return 1;
}

int setPrimary()
{
	for (int i = size - 1; i >= 0; i--)
	{
		if (!isFull(i))
			return i;
	}
	return -1;
}






