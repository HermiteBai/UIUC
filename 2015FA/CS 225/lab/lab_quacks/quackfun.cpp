/**
 * @file quackfun.cpp
 * This is where you will implement the required functions for the
 *  stacks and queues portion of the lab.
 */

#include <iostream>
using namespace std;

/**
 * Sums items in a stack.
 * @param s A stack holding values to sum.
 * @return The sum of all the elements in the stack, leaving the original
 *  stack in the same state (unchanged).
 *
 * @note You may modify the stack as long as you restore it to its original
 *  values.
 * @note You may use only two local variables of type T in your function.
 *  Note that this function is templatized on the stack's type, so stacks of
 *  objects overloading the + operator can be summed.
 * @note We are using the Standard Template Library (STL) stack in this
 *  problem. Its pop function works a bit differently from the stack we
 *  built. Try searching for "stl stack" to learn how to use it.
 * @hint Think recursively!
 */
template <typename T>
T QuackFun::sum(stack<T> & s)
{
    // Your code here
	T temp = s.top();
	T sumNum = temp;
	if (s.size() == 1)
	{
		s.pop();
		s.push(temp);
    	return sumNum; 
	}
	else
	{
		s.pop();
		sumNum += sum(s);
		s.push(temp);
	}
	return sumNum;
}

/**
 * Reverses even sized blocks of items in the queue. Blocks start at size
 * one and increase for each subsequent block.
 * @param q A queue of items to be scrambled
 *
 * @note Any "leftover" numbers should be handled as if their block was complete.
 * @note We are using the Standard Template Library (STL) queue in this
 *  problem. Its pop function works a bit differently from the stack we
 *  built. Try searching for "stl stack" to learn how to use it.
 * @hint You'll want to make a local stack variable.
 */
template <typename T>
void QuackFun::scramble(queue<T> & q)
{
    stack<T> s;
	int n = 1;
	int temp = q.front();
    // Your code here
	q.push(q.front());
	q.pop();
	n++;
	while (q.front() != temp)
	{
		if (n % 2 == 1)
		{
			for (int i = 0; i < n && q.front() != temp; i++)
			{
				q.push(q.front());
				q.pop();
			}	
			//cout << "check:" << n << endl;
		}
		else
		{
			for (int i = 0; i < n && q.front() != temp; i++)
			{
				s.push(q.front());
				q.pop();
			}
			for (int i = 0; i < n && s.size() > 0; i++)
			{
				q.push(s.top());
				s.pop();
			}
			//cout << "check:" << n << endl;
		}
		n++;
	}
}

/**
 * @return true if the parameter stack and queue contain only elements of exactly
 *  the same values in exactly the same order; false, otherwise.
 *
 * @note You may assume the stack and queue contain the same number of items!
 * @note There are restrictions for writing this function.
 * - Your function may not use any loops
 * - In your function you may only declare ONE local boolean variable to use in your return statement,
 *      and you may only declare TWO local variables of parametrized type T to use however you wish.
 *   No other local variables can be used.
 * - After execution of verifySame, the stack and queue must be unchanged. Be sure to comment your code VERY well.
 */
template <typename T>
bool QuackFun::verifySame(stack<T> & s, queue<T> & q)
{
	bool isSame = true;
	if (s.size() == 0)
		return true;
	T temp = s.top();
	s.pop();
	isSame = verifySame(s, q);
	T temp2 = q.front();
	q.pop();
	isSame = isSame & (temp == temp2);
	q.push(temp2);
	s.push(temp);
	return isSame;
}

