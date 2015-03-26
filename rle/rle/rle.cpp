// rle.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include "iostream"
#include "string"

#pragma warning (disable : 4996);

using namespace std;

FILE *FileOpen(string const &strFileName, bool const &ForRead)
{
	FILE *file;
	double fileSize;

	double MAX_FILE_SIZE = 2147483648;

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

bool CheckIsFreeFile(FILE *fileInput)
{
	fseek(fileInput, 0, SEEK_END);
	int fileSize = ftell(fileInput);
	fseek(fileInput, 0, SEEK_SET);

	return (fileSize == 0);
}

void PackFile(FILE *fileInput, FILE *fileOutput)
{
	auto WriteResult = [](int const &intLetterCount, char 
		const &charBuffer, FILE *fileOutput)
	{
		fputc(intLetterCount, fileOutput);
		fputc(charBuffer, fileOutput);
	};

	const int MAX_COUNT = 255;

	int intLetterCount = 0;
	char charSign = fgetc(fileInput);
	char charBuffer = charSign;

	while (!feof(fileInput))
	{
		if (charSign == charBuffer)
		{
			if (intLetterCount < MAX_COUNT)
			{
				++intLetterCount;
			}
			else
			{
				WriteResult(intLetterCount, charBuffer, fileOutput);
				intLetterCount = 1;
			}
		}
		else
		{
			WriteResult(intLetterCount, charBuffer, fileOutput);
			charBuffer = charSign;
			intLetterCount = 1;
		}
		charSign = fgetc(fileInput);
	}
	WriteResult(intLetterCount, charBuffer, fileOutput);
}


void UnpackFile(FILE *fileInput, FILE *fileOutput)
{
	char charSign;
	int intLetterCount;
	int i;
	
	while (!feof(fileInput))
	{
		intLetterCount = int(getc(fileInput));
		charSign = fgetc(fileInput);

		for (i = 0; i < intLetterCount; ++i)
		{
			fputc(charSign, fileOutput);
		}
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	
	FILE *fileInput;
	FILE *fileOutput;

	string strAction = argv[1];
	string strFileInputName = argv[2];
	string strFileOutputName = argv[3];

	string strPack = "pack";
	string strUnpack = "unpack";

	fileInput = FileOpen(strFileInputName, true);
	if (!fileInput)
	{
		return 1;
	}

	fileOutput = FileOpen(strFileOutputName, false);
	if (!fileOutput)
	{
		fclose(fileInput);
		return 1;
	}

	if (CheckIsFreeFile(fileInput) == true)
	{
		fclose(fileInput);
		fclose(fileOutput);
		return 1;
	}

	if (strAction == strPack) PackFile(fileInput, fileOutput);
	if (strAction == strUnpack) UnpackFile(fileInput, fileOutput);
	
	fclose(fileInput);
	fclose(fileOutput);

	return 0;
}

