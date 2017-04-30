/**
 * @file exercies.cpp
 * This file contains the recursion exercise code.
 */

#include "exercises.h"

/**
 * Given a non-negative int n, return the sum of its digits recursively (no loops).
 *
 * @param n The number to sum the digits of
 * @return The sum of its digits
 *
 * @note Note that mod (%) by 10 yields the rightmost digit (126 % 10 is 6),
 *  while divide (/) by 10 removes the rightmost digit (126 / 10 is 12).
 *
 * Example:
 *  sumDigits(126) == 9
 *  sumDigits(49) == 13
 *  sumDigits(12) == 3
 */
int RecursionExercises::sumDigits(int n)
{
	int temp = n % 10;
	if (n - temp == 0)
		return temp;
	else
	{
		n -= temp;
		n /= 10;
		temp += sumDigits(n);
		return temp;
	}
}

/**
 * We have triangle made of blocks. The topmost row has 1 block, the next row down has 2 blocks,
 *  the next row has 3 blocks, and so on. Compute recursively (no loops or multiplication) the
 *  total number of blocks in such a triangle with the given number of rows.
 *
 *  @param rows The number of horizontal rows in the triangle.
 *  @return The total number of blocks in the triangle pyramid.
 */
int RecursionExercises::triangle(int rows)
{
	int temp = rows;
	int sum = rows;
	if (temp == 0)
		return 0;
	if (temp == 1)
		return temp;
	else
	{
		temp--;
		sum += triangle(temp);
	}
    return sum;
}
