// invert.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "iostream"
#include "sstream"
#include "vector"
#include "algorithm"
#include <windows.h>

#pragma warning(disable : 4996)

using namespace std;

FILE *OpenFile(string const &strFileName)
{
	const double MAX_FILE_SIZE = 2147483648;
	FILE *file = fopen(strFileName.c_str(), "r");

	if (!file)
	{
		cout << "File didn't open";
		return NULL;
	}

	fseek(file, 0, SEEK_END);
	if (ftell(file) > MAX_FILE_SIZE)
	{
		cout << "Too big size of file";
		return NULL;
	}
	fseek(file, 0, SEEK_SET);

	return file;
}

vector<vector<double>> ReadMatrixFromFile(FILE *fileInput)
{
	vector<vector<double>> vectMatrix(3);
	string strBuffer;
	stringstream stream;
	
	double number;
	char charSign;
	size_t index = -1;

	while (!feof(fileInput))
	{
		++index;
		do
		{
			charSign = getc(fileInput);
			if (charSign != ' ' && charSign != '\n' &&
				!feof(fileInput))
			{
				strBuffer.push_back(charSign);
			}
			else
			{
				stream << strBuffer;
				stream >> number;

				vectMatrix.at(index).push_back(number);

				strBuffer.clear();
				stream.clear();
			}
		} while (!feof(fileInput) && charSign != '\n');
	}
	return vectMatrix;
}

double FindDeterminantThreeByThree(vector<vector<double>> const &vectMatrix)
{
	return vectMatrix.at(0).at(0) * vectMatrix.at(1).at(1) * vectMatrix.at(2).at(2) +
		vectMatrix.at(0).at(1) * vectMatrix.at(1).at(2) * vectMatrix.at(2).at(0) +
		vectMatrix.at(0).at(2) * vectMatrix.at(1).at(0) * vectMatrix.at(2).at(1) -
		vectMatrix.at(0).at(2) * vectMatrix.at(1).at(1) * vectMatrix.at(2).at(0) -
		vectMatrix.at(0).at(0) * vectMatrix.at(1).at(2) * vectMatrix.at(2).at(1) -
		vectMatrix.at(0).at(1) * vectMatrix.at(1).at(0) * vectMatrix.at(2).at(2);
}

double FindDeterminantTwoByTwo(vector<vector<double>> const &vectMatrix)
{
	return vectMatrix.at(0).at(0) * vectMatrix.at(1).at(1) -
		vectMatrix.at(0).at(1) * vectMatrix.at(1).at(0);
}

vector<vector<double>> FillMatrixMinor(vector<vector<double>> const &vectMatrix)
{
	vector<vector<double>> vectMatrixTwoByTwo;
	vector<vector<double>> vectMatrixMinor;
	vector<double> vectStrMatrixTwoByTwo;
	vector<double> vectStrMatrixMinor;
	
	for (size_t i = 0; i < vectMatrix.size(); ++i)
	{
		for (size_t j = 0; j < vectMatrix.at(i).size(); ++j)
		{
			for (size_t k = 0; k < vectMatrix.size(); ++k)
			{
				for (size_t n = 0; n < vectMatrix.at(k).size(); ++n)
				{
					if (i != k)
					{
						if (j != n) vectStrMatrixTwoByTwo.push_back(vectMatrix.at(k).at(n));
					}
				}
				
				if (vectStrMatrixTwoByTwo.size() != 0)
				{
					vectMatrixTwoByTwo.push_back(vectStrMatrixTwoByTwo);
					vectStrMatrixTwoByTwo.clear();
				}
			}
			vectStrMatrixMinor.push_back(FindDeterminantTwoByTwo(vectMatrixTwoByTwo));
			vectMatrixTwoByTwo.clear();
		}
		vectMatrixMinor.push_back(vectStrMatrixMinor);
		vectStrMatrixMinor.clear();
	}
	return vectMatrixMinor;
}

vector<vector<double>> ChangeSigns(vector<vector<double>> &vectMatrix)
{
	vectMatrix.at(0).at(1) = -vectMatrix.at(0).at(1);
	vectMatrix.at(1).at(0) = -vectMatrix.at(1).at(0);
	vectMatrix.at(1).at(2) = -vectMatrix.at(1).at(2);
	vectMatrix.at(2).at(1) = -vectMatrix.at(2).at(1);
	return vectMatrix;
}

vector<vector<double>> MatrixTransposition(vector<vector<double>> const &vectMatrix)
{ 
	vector<vector<double>> vectMatrixTrans(3);
	for (size_t i = 0; i < vectMatrix.size(); ++i)
	{
		for (size_t j = 0; j < vectMatrix.at(i).size(); ++j)
		{
			vectMatrixTrans.at(j).push_back(vectMatrix.at(i).at(j));
		}
	}
	return vectMatrixTrans;
}

void PrintResultMatrix(vector<vector<double>> const &vectMatrix)
{
	cout.precision(4);

	for (auto MatrixLine : vectMatrix)
	{
		for (auto MatrixElem : MatrixLine)
		{
			cout << fixed << MatrixElem << ' ';
		}
		cout << endl;
	}
}

vector<vector<double>> ResultMatrix(vector<vector<double>> vectMatrix, double const &Determinant)
{
	double fraction = 1.0 / Determinant;

	for (size_t i = 0; i != vectMatrix.size(); ++i)
	{
		for (size_t j = 0; j != vectMatrix.at(i).size(); ++j)
		{
			vectMatrix.at(i).at(j) *= fraction;
		}
	}
	return vectMatrix;
}

int _tmain(int argc, char *argv[])
{
	string strFileInputName = argv[1];
	FILE *fileMatrix = OpenFile(strFileInputName);
	if (!fileMatrix) return 1;

	vector<vector<double>> vectMatrix = ReadMatrixFromFile(fileMatrix);
	fclose(fileMatrix);

	double determinant = FindDeterminantThreeByThree(vectMatrix);

	vectMatrix = FillMatrixMinor(vectMatrix);
	vectMatrix = ChangeSigns(vectMatrix);
	vectMatrix = MatrixTransposition(vectMatrix);
	vectMatrix = ResultMatrix(vectMatrix, determinant);
	PrintResultMatrix(vectMatrix);

	return 0;
}

