// Rri.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <Windows.h>

typedef float T;

void SetCmdRed()
{
#ifdef WIN32
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
#else
	printf("\033[31m");
#endif
}

void SetCmdGreen()
{
#ifdef WIN32
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
#else
	printf("\033[32m");
#endif
}

void SetCmdBlue()
{
#ifdef WIN32
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
#else
	printf("\033[33m");
#endif
}

void SetCmdDefault()
{
#ifdef WIN32
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
#else
	printf("\033[0m");
#endif
}

void DumpArray(T* arr, int dstIdx, int srcIdx, int size)
{
	int i;
	static int cnt = 0;
	printf("[%2d] (%2d => %2d) ", ++cnt, srcIdx, dstIdx);
	for (i = 0; i < size; ++i) {
		if (i == dstIdx) {
			SetCmdGreen();
		} else if (i == srcIdx) {
			SetCmdRed();
		} else {
			SetCmdDefault();
		}
		std::cout << std::setw(8) << arr[i] << " ";
	}
	std::cout << std::endl;
}

void QuickSort(T *arr, int left, int right, int size)
{
	int l = left;
	int r = right;
	int p = (left + right) / 2;
	T pval = arr[p];
	while (l < r) {
		for (; l < p && pval >= arr[l]; ++l);
		if (l < p) {
			arr[p] = arr[l];
			DumpArray(arr, p, l, size);
			p = l;
		}
		for (; r > p && pval <= arr[r]; --r);
		if (r > p) {
			arr[p] = arr[r];
			DumpArray(arr, p, r, size);
			p = r;
		}
	}
	arr[p] = pval;
	DumpArray(arr, p, (left + right) / 2, size);
	if (p - left > 1)
		QuickSort(arr, left, p - 1, size);
	if (right - p >1)
		QuickSort(arr, p + 1, right, size);

}

void SelectSort(T* arr, int size)
{
	int i, j;
	for (i = 0; i < size; ++i) {
		for (j = i + 1; j < size; ++j) {
			if (arr[i] > arr[j]) {
				T tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;
				DumpArray(arr, i, j, size);
			}
		}
	}
}

void ShowAllCol(std::vector<std::string>& allCol)
{
	std::vector<std::string>::iterator iterAllCol;
	for (iterAllCol = allCol.begin(); iterAllCol != allCol.end(); ++iterAllCol)
	{
		if (iterAllCol != allCol.begin()) {
			std::cout << "\t";
		}
		std::cout << *iterAllCol;
	}
}

void ShowAllData(std::map<int, std::vector<std::string> >& allData)
{
	std::map<int, std::vector<std::string> >::iterator iterAllData;
	for (iterAllData = allData.begin(); iterAllData != allData.end(); ++iterAllData)
	{
		if (iterAllData != allData.begin()) {
			std::cout << std::endl;
		}
		ShowAllCol(iterAllData->second);
	}
	std::cout << std::endl;
}

int TransLine(std::vector<std::string>& allCol, std::vector<T>& col)
{
	std::vector<std::string>::iterator iterAllCol;
	for (iterAllCol = allCol.begin(); allCol.end() != iterAllCol; ++iterAllCol)
	{
		char *end = NULL;
		col.push_back(std::strtof(iterAllCol->c_str(), &end));
	}
	return 0;
}

int TransTable(std::map<int, std::vector<std::string> >& allData, std::map<int, std::vector<T> >& data)
{
	int idxRow;
	std::map<int, std::vector<std::string> >::iterator iterAllData;
	for (iterAllData = allData.begin(), idxRow = 0; allData.end() != iterAllData; ++iterAllData, ++idxRow)
	{
		std::vector<T> line;
		TransLine(iterAllData->second, line);
		data[idxRow] = line;
	}
	return 0;
}

int ParseLine(const std::string& line, std::vector<std::string>& allCol)
{
	char oper = ' ';
	for (int idxCol = 0; idxCol < line.length(); ++idxCol)
	{
		int idxTab = line.find(oper, idxCol);
		std::string element;
		if (idxTab == -1) {
			element = line.substr(idxCol, line.length() - idxCol);
			break;
		} else {
			element = line.substr(idxCol, idxTab - idxCol);
			idxCol = idxTab;
		}
		allCol.push_back(element);
	}
	return 0;
}

int ParseFile(const std::string& file, std::map<int, std::vector<std::string> >& allData)
{
	std::fstream s(file, std::fstream::in | std::fstream::out);
	int idxRow = 0;
	for (std::string line; std::getline(s, line); ++idxRow) {
		std::vector<std::string> allCol;
		ParseLine(line, allCol);
		allData[idxRow] = allCol;
	}

	return 0;
}

int SortMain(std::map<int, std::vector<T> >& data)
{
	std::map<int, std::vector<T> >::iterator iterData;
	for (iterData = data.begin(); iterData != data.end(); ++iterData)
	{
		QuickSort(&iterData->second.at(0), 0, iterData->second.size() - 1, iterData->second.size());
	}
	return 0;
}

int main(int argc, char* argv[])
{
	std::map<int, std::vector<std::string> > allData;
	std::map<int, std::vector<T> > data;
	ParseFile("test.txt", allData);
	ShowAllData(allData);
	TransTable(allData, data);
	SortMain(data);
    return 0;
}

