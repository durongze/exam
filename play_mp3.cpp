#ifdef _WIN32
#include <io.h>
int ReadFileByDir(std::string dirName, std::string fileExt, std::vector<std::string>& files)
{
	struct _finddata_t s_file;
	std::string str = dirName + "\\*" + fileExt;
	bool withDir = true;
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
