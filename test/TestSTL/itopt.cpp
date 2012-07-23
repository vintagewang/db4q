/*
 * $Id: itopt.cpp 1 2012-01-17 02:31:57Z  $
 */
#include <iostream>
#include <stdio.h>
#include <signal.h>
#include <list>

using std::cout;
using std::cerr;
using std::endl;

template< class T2 = std::list<T> >
inline T& LWPR_LIST_AT(T2& s, size_t index)
{
	assert(index < s.size());

	T2::iterator it = s.begin();

	for(size_t i = 0; i < index; i++, it++);
	return *it;
}

static int run(int argc, char** argv)
{
	std::list <int> nums;

	nums.push_back(1);
	nums.push_back(2);
	nums.push_back(4);
	nums.push_back(6);
	nums.push_back(8);

	//cout << LWPR_LIST_AT(nums, 1) << endl;
	sort

	return 0;
}

int main(int argc, char** argv)
{
	try
	{
		return run(argc, argv);
	}
	catch(...)
	{
		cerr << "Unknow exception" << endl;
	}

	return -1;
}
