// fill.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "iostream"
#include "vector"
#include "set"
#include "string"
#include "queue"

#pragma warning (disable : 4996);

using namespace std;

struct coord{
	int i;
	int j;
};


FILE *OpenFile(string const &strFileName, bool const  &ForRead)
{
	double fileSize;
	FILE *file;

	const double MAX_FILE_SIZE = 2147483648;

	if (ForRead)
	{
		file = fopen(strFileName.c_str(), "r");
	}
	else
	{
		file = fopen(strFileName.c_str(), "w");
	}

	if (!file)
	{
		cout << "File didn't open";
		return NULL;
	}

	if (ForRead)
	{
		fseek(file, 0, SEEK_END);
		fileSize = ftell(file);
		fseek(file, 0, SEEK_SET);

		if (fileSize > MAX_FILE_SIZE)
		{
			cout << "Too big file";
			return NULL;
		}
	}
	return file;
}

vector<vector<char>> ReadFromFile(FILE *fileWithPicture)
{
	vector<vector<char>> vectField;
	char charSign;
	int index = 0;

	while (!feof(fileWithPicture))
	{
		vectField.resize(index + 1);
		charSign = getc(fileWithPicture);
		while ((charSign != '\n') && (!feof(fileWithPicture)))
		{
			vectField.at(index).push_back(charSign);
			charSign = getc(fileWithPicture);
		}
		++index;
	}
	return vectField;
}

queue<coord> GetCoorinatsOfStart(vector<vector<char>> const &vectField)
{ 
	const char START_SIGN = 'O';
	queue<coord> queueStartCoordinates;
	coord coordStart;

	for (size_t i = 0; i < vectField.size(); ++i)
	{
		for (size_t j = 0; j < vectField.at(i).size(); ++j)
		{
			if (vectField.at(i).at(j) == START_SIGN)
			{
				coordStart.i = i;
				coordStart.j = j;
				queueStartCoordinates.push(coordStart);
			}
		}
	}
	return queueStartCoordinates;
}

vector<vector<char>> FillField(vector<vector<char>> vectField, set<char> const &setSigns,
	int i, int j, int intLength, int intHeight)
{
	const char DOT_SIGN = '.';
	const char START_SIGN = 'O';

	if (vectField.at(i).at(j) != START_SIGN)
	{
		vectField.at(i).at(j) = DOT_SIGN;
	}

	if ((j + 1 < intLength) && (setSigns.find(vectField.at(i).at(j + 1)) == setSigns.end()))
	{
		vectField = FillField(vectField, setSigns, i, j + 1, intLength, intHeight);
	}
	if ((i + 1 < intHeight) && (setSigns.find(vectField.at(i + 1).at(j)) == setSigns.end()))
	{
		vectField = FillField(vectField, setSigns, i + 1, j, intLength, intHeight);
	}
	if ((j - 1 > -1) && (setSigns.find(vectField.at(i).at(j - 1)) == setSigns.end()))
	{
		vectField = FillField(vectField, setSigns, i, j - 1, intLength, intHeight);
	}
	if ((i - 1 > -1) && (setSigns.find(vectField.at(i - 1).at(j)) == setSigns.end()))
	{
		vectField = FillField(vectField, setSigns, i - 1, j, intLength, intHeight);
	}
	return vectField;
}

void WriteIntoFile(FILE *fileOutput, vector<vector<char>> const &vectField)
{
	for (auto vectLine : vectField)
	{
		for (auto charElem : vectLine)
		{
			putc(charElem, fileOutput);
		}
		putc('\n', fileOutput);
	}
}


int _tmain(int argc, _TCHAR* argv[])
{
	const int MAX_SIZE_OF_PICTURE = 100;
	set<char> setSigns{ '#', 'O', '.' };

	string strFileInputName = argv[1];
	string strFileOutputName = argv[2];

	FILE *fileInput = OpenFile(strFileInputName, true);
	if (!fileInput) return 1;

	FILE *fileOutput = OpenFile(strFileOutputName, false);
	if (!fileOutput)
	{
		fclose(fileInput);
		return 1;
	}

	vector<vector<char>> vectField = ReadFromFile(fileInput);
	fclose(fileInput);

	int intHeight = vectField.size();
	int intLength = vectField.at(0).size();

	if ((intHeight > MAX_SIZE_OF_PICTURE) || (intLength > MAX_SIZE_OF_PICTURE))
	{
		fclose(fileOutput);
		return 1;
	}
	
	queue<coord> queueStartCoordinates = GetCoorinatsOfStart(vectField);
	coord tagStart;

	while (!queueStartCoordinates.empty())
	{
		tagStart = queueStartCoordinates.front();
		queueStartCoordinates.pop();
		vectField = FillField(vectField, setSigns, tagStart.i, tagStart.j, intLength, intHeight);
	}

	WriteIntoFile(fileOutput, vectField);
	fclose(fileOutput);

	return 0;
}

