/*
 * $Id: test.cpp 10 2012-01-17 11:20:56Z  $
 */
#include <iostream>
#include <stdio.h>
#include <signal.h>
#include <list>
#include "db4q.h"

using std::cout;
using std::cerr;
using std::endl;


static int run(int argc, char** argv)
{
	cout << "INT8	" << sizeof(DB4Q::INT8) << endl;
	cout << "INT16	" << sizeof(DB4Q::INT16) << endl;
	cout << "INT32	" << sizeof(DB4Q::INT32) << endl;
	cout << "INT64	" << sizeof(DB4Q::INT64) << endl;
	cout << "long	" << sizeof(long) << endl;

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
