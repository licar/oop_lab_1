// SquareFunction.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "iostream"
#include <cmath>

using namespace std;

void FindingSolveOfEquation(int const &A, int const &B, int const &C)
{
	int discriminant = pow(B, 2) - 4 * A * C;

	if (discriminant < 0)
	{
		cout << "There is no real root";
		return;
	}

	cout.precision(4);

	double x1 = (-B + pow(discriminant, 0.5)) / (2 * A);
	cout << "x1 = " << fixed << x1 << endl;

	if (discriminant != 0)
	{
		double x2 = (-B - pow(discriminant, 0.5)) / (2 * A);
		cout << "x2 = " << fixed << x2 << endl;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	int A, B, C;

	A = atoi(argv[1]);
	B = atoi(argv[2]);
	C = atoi(argv[3]);

	if (A == 0)
	{
		cout << "It's not quadratic equation";
		return 1;
	}

	FindingSolveOfEquation(A, B, C);

	return 0;
}

