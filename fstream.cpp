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

int GenTimeByLine(int Secs, int Lines, int line, int& min, int& sec)
{
	float fSecs = (float)Secs / Lines * line;
	min = fSecs / 60;
	sec = ((int)fSecs) % 60;
	return 0;
}

int GenLrcTable(std::map<int, std::vector<std::string> >& allData, std::map<int, std::vector<std::string> >& data, int Secs)
{
	int idxRow;
	std::map<int, std::vector<std::string> >::iterator iterAllData;
	for (iterAllData = allData.begin(), idxRow = 0; allData.end() != iterAllData; ++iterAllData, ++idxRow)
	{
		std::string time;
		int min = 0;
		int sec = 0;
		GenTimeByLine(Secs, allData.size(), idxRow, min, sec);
		char buf[3] = { 0 };
		snprintf(buf, sizeof(buf), "%02d", min);
		time += "[" + std::string(buf) + ":";
		snprintf(buf, sizeof(buf), "%02d", sec);
		time += std::string(buf) + "]";
		iterAllData->second.insert(iterAllData->second.begin(), time);
		data[idxRow] = iterAllData->second;
	}
	return 0;
}

int GenTxtByLrc(const std::string& fileTxt, const std::string& fileLrc)
{
	std::map<int, std::vector<std::string> > data;
	ParseFile(fileLrc, data, ']');

	std::ofstream fs(fileTxt, std::ios::out | std::ios::trunc);
	std::map<int, std::vector<std::string> >::iterator iterData;
	for (iterData = data.begin(); iterData != data.end(); ++iterData) {
		std::vector<std::string>::iterator iterWord;
		for (iterWord = iterData->second.begin(), ++iterWord; iterWord != iterData->second.end(); ++iterWord)
		{
			fs << *iterWord << " ";
		}
		fs << std::endl;
	}
	return 0;
}

int WriteLrcTable(std::map<int, std::vector<std::string> >& data, std::ofstream& fs)
{
	std::map<int, std::vector<std::string> >::iterator iterData;
	for (iterData = data.begin(); iterData != data.end(); ++iterData)
	{
		std::vector<std::string>::iterator iterWord;
		for (iterWord = iterData->second.begin(); iterWord != iterData->second.end(); ++iterWord)
		{
			fs << *iterWord << " ";
		}
		fs << std::endl;
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
			allCol.push_back(element);
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
		if (allCol.size() != 0) {
			allData[idxRow] = allCol;
		} else {
			--idxRow;
		}
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

int mainGetMaxSubArr()
{
	int idx = 0;
	int cnt = 0;
	int number = 0;
	std::vector<int> arr;

	std::cin >> cnt;

	for (idx = 0; idx < cnt; ++idx)
	{
		std::cin >> number;
		arr.push_back(number);
	}

	std::cout << getMaxSubArr(arr) << std::endl;
	return 0;
}

int GenTxtByLrc(const std::string& fileTxt, const std::string& fileLrc)
{
	char oper = ']';
	std::map<int, std::vector<std::string> > data;
	ParseFile(fileLrc, data, oper);

	std::ofstream fs(fileTxt, std::ios::out | std::ios::trunc);
	std::map<int, std::vector<std::string> >::iterator iterData;
	for (iterData = data.begin(); iterData != data.end(); ++iterData) {
		std::vector<std::string>::iterator iterWord;
		for (iterWord = iterData->second.begin(), ++iterWord; iterWord != iterData->second.end(); ++iterWord)
		{
			fs << *iterWord << oper;
		}
		fs << std::endl;
	}
	return 0;
}

int WriteLrcTable(std::map<int, std::vector<std::string> >& data, std::ofstream& fs)
{
	std::map<int, std::vector<std::string> >::iterator iterData;
	for (iterData = data.begin(); iterData != data.end(); ++iterData)
	{
		std::vector<std::string>::iterator iterWord;
		for (iterWord = iterData->second.begin(); iterWord != iterData->second.end(); ++iterWord)
		{
			fs << *iterWord << " ";
		}
		fs << std::endl;
	}
	return 0;
}
#ifdef _WIN32
#include <io.h>
int ReadFileByDir(std::string dirName, std::string fileExt, std::vector<std::string>& files)
{
	struct _finddata_t s_file;
	std::string str = dirName + "\\*" + fileExt;
	bool withDir = false;
	intptr_t h_file = _findfirst(str.c_str(), &s_file);
	if (h_file != static_cast<intptr_t>(-1.0)) {
		do {
			if (withDir) {
				files.push_back(dirName + "\\" + s_file.name);
			} else {
				files.push_back(std::string(s_file.name));
			}
		} while (_findnext(h_file, &s_file) == 0);
	}
	_findclose(h_file);
	return 0;
}
#endif
wchar_t * char2wchar(const char* cchar)
{
	wchar_t *m_wchar;
	int len = MultiByteToWideChar(CP_ACP, 0, cchar, strlen(cchar), NULL, 0);
	m_wchar = new wchar_t[len + 1];
	MultiByteToWideChar(CP_ACP, 0, cchar, strlen(cchar), m_wchar, len);
	m_wchar[len] = '\0';
	return m_wchar;
}
char * wchar2char(const wchar_t* wchar)
{
	char * m_char;
	int len = WideCharToMultiByte(CP_ACP, 0, wchar, wcslen(wchar), NULL, 0, NULL, NULL);
	m_char = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, wchar, wcslen(wchar), m_char, len, NULL, NULL);
	m_char[len] = '\0';
	return m_char;
}

#include <mmSystem.h>
#pragma comment(lib, "winmm.lib")

MCI_OPEN_PARMS m_mciOpen;  //打开参数
MCI_PLAY_PARMS m_mciPlay;  //播放参数

bool PlayMP3(const char * szFileName)
{
	// PlaySound(szFileName, NULL, SND_FILENAME | SND_ASYNC|SND_LOOP);
	m_mciOpen.lpstrDeviceType = TEXT("mpegvideo"); //要操作的文件类型
	m_mciOpen.lpstrElementName = char2wchar(szFileName); //要操作的文件路径
	MCIERROR mcierror = mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD)&m_mciOpen); //打开文件命令
	if (mcierror) {
		return false;
	}
	else {
		m_mciPlay.dwCallback = (DWORD)NULL;
		m_mciPlay.dwFrom = (DWORD)0; //播放起始位置ms为单位
		mciSendCommand(m_mciOpen.wDeviceID, MCI_PLAY, MCI_NOTIFY | MCI_FROM, (DWORD)(LPVOID)&m_mciPlay);
	}

	delete[] m_mciOpen.lpstrElementName;
	return true;
}

int GetMp3TimeLen(const char *file)
{
	long lLength = 0;
	wchar_t sPosition[MAX_PATH] = { 0 };
	wchar_t *pCmd = NULL;
	std::string cmd;
	
	cmd = "open ";
	cmd += file;
	cmd += " alias song";
	pCmd = char2wchar(cmd.c_str());
	mciSendString(pCmd, NULL, 0, NULL);
	delete[] pCmd;

	cmd = "status ";
	cmd += file;
	cmd += " length";
	pCmd = char2wchar(cmd.c_str());
	mciSendString(pCmd, sPosition, 255, 0);
	delete[] pCmd;

	std::wcout << sPosition << std::endl;
	char *pPosition = wchar2char(sPosition);
	lLength = strtol(pPosition, NULL, 10) / 1000;
	std::cout << lLength << std::endl;
	// mciSendString(TEXT("play song wait"), NULL, 0, NULL);
	mciSendString(TEXT("close song"), NULL, 0, NULL);
	delete[] pPosition;

	return lLength;
}

int ShowItem(std::map<int, std::vector<std::string> >& item)
{
	std::string itemKey = "filename";
	std::map<int, std::vector<std::string> >::iterator iterData;
	for (iterData = item.begin(); iterData != item.end(); ++iterData) {
		std::vector<std::string>::iterator iterWord;
		iterWord = iterData->second.begin();
		if (*iterWord == itemKey) {
			++iterWord;
			std::cout << *iterWord << std::endl;
		}
	}
	return 0;
}

int ShowElement(std::vector<std::map<int, std::vector<std::string> > >& elements)
{
	std::vector<std::map<int, std::vector<std::string> > >::iterator iterItem;
	for (iterItem = elements.begin(); iterItem != elements.end(); ++iterItem)
	{
		ShowItem(*iterItem);
	}
	return 0;
}

int GetItemValueByKey(std::map<int, std::vector<std::string> >& item, std::string itemKey, std::string& itemValue)
{
	std::map<int, std::vector<std::string> >::iterator iterData;
	for (iterData = item.begin(); iterData != item.end(); ++iterData) {
		std::vector<std::string>::iterator iterWord;
		iterWord = iterData->second.begin();
		if (*iterWord == itemKey) {
			++iterWord;
			itemValue = *iterWord ;
		}
	}
	return 0;
}

int ParseElement(std::map<int, std::vector<std::string> >& data, std::vector<std::map<int, std::vector<std::string> > >& elements)
{
	std::string elementKeyStart = "[FORMAT]";
	std::string elementKeyEnd = "[/FORMAT]";

	std::map<int, std::vector<std::string> > item;
	std::map<int, std::vector<std::string> >::iterator iterData;
	for (iterData = data.begin(); iterData != data.end(); ++iterData)
	{
		std::vector<std::string>::iterator iterWord = iterData->second.begin();
		if (*iterWord == elementKeyStart) {
			item.clear();
		} else if (*iterWord == elementKeyEnd) {
			elements.push_back(item);
		} else {
			item.insert(*iterData);
		}
	}
	return 0;
}

int GetVideoTimeLen(char *iniInfo, std::string videoFile)
{
	char opt = '=';
	std::string duration;
	std::map<int, std::vector<std::string> > data;
	std::vector<std::map<int, std::vector<std::string> > > elements;

	ParseFile(iniInfo, data, opt);
	ParseElement(data, elements);
	ShowElement(elements);
	std::vector<std::map<int, std::vector<std::string> > >::iterator iterItem;

	for (iterItem = elements.begin(); iterItem != elements.end(); ++iterItem)
	{
		std::map<int, std::vector<std::string> >::iterator iterLine = iterItem->begin();
		std::vector<std::string>::iterator iterWord = iterLine->second.begin();
		if (++iterWord != iterLine->second.end() && videoFile == *iterWord) {
			GetItemValueByKey(*iterItem, videoFile, duration);
			char* nptr = NULL;
			return std::strtold(duration.c_str(), &nptr);
		}
	}
	return 0;
}
