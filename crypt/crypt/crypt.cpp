// crypt.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"

#include "stdafx.h"
#include "iostream"
#include "string"

#pragma warning (disable : 4996);

using namespace std;

char MixingBits(char const &charSign, bool ForCrypt)
{
	char charResult = 0x0;
	if (ForCrypt)
	{
		charResult |= ((charSign & 0x01) << 2);
		charResult |= ((charSign & 0x02) << 2);
		charResult |= ((charSign & 0x04) << 2);
		charResult |= ((charSign & 0x08) << 3);
		charResult |= ((charSign & 0x10) << 3);
		charResult |= ((charSign & 0x20) >> 5);
		charResult |= ((charSign & 0x40) >> 5);
		charResult |= ((charSign & 0x80) >> 2);
	}
	else
	{
		charResult |= ((charSign & 0x01) << 5);
		charResult |= ((charSign & 0x02) << 5);
		charResult |= ((charSign & 0x04) >> 2);
		charResult |= ((charSign & 0x08) >> 2);
		charResult |= ((charSign & 0x10) >> 2);
		charResult |= ((charSign & 0x20) << 2);
		charResult |= ((charSign & 0x40) >> 3);
		charResult |= ((charSign & 0x80) >> 3);
	}
	return charResult;
}

char OperationXOR(char charSign,int const &intKey, bool ForCrypt)
{
	if (!ForCrypt) charSign = MixingBits(charSign, ForCrypt);
	charSign ^= intKey;
	if (ForCrypt) charSign = MixingBits(charSign, ForCrypt);
	return charSign;
}

FILE *FileOpen(string &strFileName, bool ForRead)
{
	FILE *file;
	double fileSize;
	const double MAX_SIZE_OF_FILE = 2147483648;

	if (ForRead) file = fopen(strFileName.c_str(), "r");
	else file = fopen(strFileName.c_str(), "w");

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

		if (fileSize > MAX_SIZE_OF_FILE)
		{
			cout << "Too big file";
			return NULL;
		}
	}

	return file;
}


void ReadAndWrite(FILE *fileInput, FILE * fileOutput, int const &intKey, bool ForCrypt)
{
	char charSign = getc(fileInput);
	while (!feof(fileInput))
	{
		charSign = OperationXOR(charSign, intKey, ForCrypt);
		putc(charSign, fileOutput);
		charSign = getc(fileInput);
	}
}


int _tmain(int argc, _TCHAR* argv[])
{
	const int MAX_VALUE_OF_KEY = 255;
	const string CRYPT = "crypt";

	string strAction = argv[1];
	string strFileInputName = argv[2];
	string strFileOutputName = argv[3];
	short int intKey = atoi(argv[4]);

	if ((intKey > MAX_VALUE_OF_KEY) || (intKey < 0))
	{
		return 1;
	}

	FILE *fileInput = FileOpen(strFileInputName, true);
	if (!fileInput) return 1;

	FILE *fileOutput = FileOpen(strFileOutputName, false);
	if (!fileOutput)
	{
		fclose(fileInput);
		return 1;
	}

	bool ForCrypt = (CRYPT == strAction);

	ReadAndWrite(fileInput, fileOutput, intKey, ForCrypt);
	
	fclose(fileInput);
	fclose(fileOutput);
	return 0;
}

