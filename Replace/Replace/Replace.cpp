// replace.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "iostream"
#include <string>
#include <stdio.h>
#include "windows.h"
#include <conio.h>

#pragma warning (disable : 4996)

using namespace std;

FILE *OpenFile(string const &strFileName, bool const &ForRead)
{
	const double MAX_FILE_SIZE = 2147483648;

	double fileSize;
	FILE *file;

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
			fclose(file);
			return NULL;
		}
	}
	return file;
}


string SearchInFileStringForFinding(FILE *fileInput,
	string const &strFind, char &charSign)
{
	string strBuffer;
	int index = 0;

	do
	{
		strBuffer.push_back(charSign);
		charSign = getc(fileInput);
		++index;
	} while ((!feof(fileInput)) && 
		(strFind.length() != strBuffer.length())
		&& (charSign == strFind.at(index)));
	
	return strBuffer;

}

void ProcessStrings(FILE *fileInput, FILE *fileOutput, 
	string const &strFind, string const &strReplace)
{
	string strBuffer;

	char charSign = getc(fileInput);
	while (!feof(fileInput))
	{
		if (charSign == strFind.at(0))
		{
			strBuffer = SearchInFileStringForFinding(fileInput, strFind, charSign);

			if (strBuffer.length() == strFind.length())
			{
				fputs(strReplace.c_str(), fileOutput);
			}
			else
			{
				fputs(strBuffer.c_str(), fileOutput);
			}
			strBuffer.clear();
		}
		else
		{
			fputc(charSign, fileOutput);
			charSign = fgetc(fileInput);
		}
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	string fileInputName = argv[1];
	string fileOutputName = argv[2];
	string strSearch = argv[3];
	string strReplace = argv[4];

	FILE *fileInput = OpenFile(fileInputName, true);
	if (!fileInput) return 1; 

	FILE *fileOutput = OpenFile(fileOutputName, false);
	if (!fileOutput)
	{
		fclose(fileInput);
		return 1;
	}

	ProcessStrings(fileInput, fileOutput, strSearch, strReplace);

	fclose(fileInput);
	fclose(fileOutput);

	return 0;
}


