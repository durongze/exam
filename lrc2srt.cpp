#pragma warning(disable: 4996)	// To disable deprecation, use _CRT_SECURE_NO_WARNINGS.

#include <windows.h>
#include <string>		// std::string
#include <iostream>		// std::cout
#include <vector>		// std::vector<LrcLine>
#include <algorithm>	// std::sort
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
#include <direct.h>		// _getcwd()
#include <Stringapiset.h>	// MultiByteToWideChar , 

const int MAX_SIZE = 1000;

struct LrcLine
{
	LrcLine()
	{
		millisecond = 0;
		content = ar = al = ti = au = length = by = re = ve = TEXT("");
	}

	int	millisecond;// 가사, 파일의 시작을 기준으로 오프셋 밀리 시간을 표시
	std::wstring	content;	// 행의 내용
	std::wstring	ar;	//[ar:가사 아티스트]
	std::wstring	al;	//[al:노래의 앨범]
	std::wstring	ti;	//[ti:가사(노래) 제목]
	std::wstring	au;	//[au:가사 작성자]
	std::wstring	length;	//[length:음악의 길이]
	std::wstring	by;	//[by:LRC파일의 작성자]
	std::wstring	re;	//[re:LRC를 작성한 플레이어나 편집기]
	std::wstring	ve;	//[ve:프로그렘 버전]
};
bool operator< (LrcLine& l, LrcLine& r)
{
	return l.millisecond < r.millisecond;
}

typedef std::vector<LrcLine>  LrcData;


// string to wstring
std::wstring s2ws(const std::string& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
	std::wstring r(len, L'\0');
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, &r[0], len);
	return r;
}

// wstring to string
std::string ws2s(const std::wstring& s)
{
	int len;
	int slength = (int)s.length() + 1;
	len = WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, 0, 0, 0, 0);
	std::string r(len, '\0');
	WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, &r[0], len, 0, 0);
	return r;
}

void format_srt_time(TCHAR* output, int bufferSize, int millisecond)
{
	// 시, 분, 초, 밀리초
	// 분수는 3개의 0으로 채워진 숫자 (00 : 00 : 00,000)
	int milli = millisecond % 1000;
	int secs = millisecond / 1000;
	int mins = secs / 60;
	secs = secs % 60;
	_stprintf_s(output, bufferSize, TEXT("00:%0.2d:%02d,%0.3d"),
		mins, secs, milli);
}
//bool save_lrc_as_srt(LrcData &lrc_data)
bool save_lrc_as_srt(const char* src_file_name, LrcData& lrc_data)
{
	//wchar_t code;
	//AnsiString s = "한";
	//MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, s.c_str(), 2, &code, 1);
	//ShowMessage(AnsiString().sprintf("%X", code));
	FILE* srt_file = fopen(src_file_name, "w+, ccs=UTF-8");	// 읽고 쓰기 위해 빈 파일을 엽니다. 지정한 파일이 있으면 이 파일의 내용은 삭제됩니다.
	if (srt_file == NULL) {
		return false;
	}

	TCHAR time1[64];
	TCHAR time2[64];

	for (unsigned int i = 0; i < lrc_data.size(); i++) {
		int t1 = lrc_data[i].millisecond;
		int t2 = t1;
		if (i + 1 < lrc_data.size()) {
			int t3 = lrc_data[i + 1].millisecond - 50;
			if (t3 - t1 > 50) { t2 = t3; }
		}
		format_srt_time(time1, 64, t1);
		format_srt_time(time2, 64, t2);

		//printf(
		//fprintf(srt_file,
		_ftprintf(srt_file,
			TEXT("%d\n")
			TEXT("%s --> %s\n")
			TEXT("%s\n\n"),
			i + 1,
			time1, time2,
			lrc_data[i].content.c_str());

		_tprintf(
			TEXT("%d\n")
			TEXT("%s --> %s\n")
			TEXT("%s\n\n"),
			i + 1,
			time1, time2,
			lrc_data[i].content.c_str());
	}

	fclose(srt_file);

	return true;
}

bool loadLrc(const char* lrc_filename, LrcData& lrc_data)
{
	// MultiByteToWideChar utf-8 to utf-16
	// WideCharToMultiByte utf-16 to utf-8
	// iconv 같은 라이브러리도 고려

	// 유니코드 및 멀티바이트 스트림 I/O에서 텍스트 모드 및 이진 모드를 사용하는 방법에 대한 자세한 내용은 
	// Text and Binary Mode File I/O 및 Unicode Stream I/O in Text and Binary Modes를 참조하세요.
	// https://msdn.microsoft.com/ko-kr/library/ktss1a9b.aspx
	// https://msdn.microsoft.com/ko-kr/library/c4cy2b8e.aspx

	//ccs = UTF - 8 을 사용해서 열고 prinf대신 wprintf를 사용해야함

	bool ret = false;
	FILE* lrc_file = fopen(lrc_filename, "rt+, ccs=UTF-8");	//읽고 쓰기 위해 엽니다. 파일이 있어야 합니다.
	if (lrc_file == NULL)
	{
		return ret;
	}

	TCHAR content[512] = { '\0', }; // 가사 내용 캐싱
	TCHAR ar[128] = { '\0', };		//[ar:가사 아티스트]
	TCHAR al[128] = { '\0', };		//[al:노래의 앨범]
	TCHAR ti[128] = { '\0', };		//[ti:가사(노래) 제목]
	TCHAR au[128] = { '\0', };		//[au:가사 작성자]
	TCHAR length[128] = { '\0', };	//[length:음악의 길이]
	TCHAR by[128] = { '\0', };		//[by:LRC파일의 작성자]
	TCHAR re[128] = { '\0', };		//[re:LRC를 작성한 플레이어나 편집기]
	TCHAR ve[128] = { '\0', };		//[ve:프로그렘 버전]

	int read_simple_items = 0;
	int read_enhanced_item = 0;
	int num_simple_time_label = 0;		// simple format time label 개수
	int num_enhanced_time_label = 0;	// enhanced format time label 개수
	int minute = 0;
	int second = 0;		// 시간 태그는 [분:초.x/100초]
	int second2 = 0;


	//while (fgetws(content, _countof(content), lrc_file) != NULL)	// multi-byte 
	while (_fgetts(content, _countof(content), lrc_file) != NULL)	//unicode
	{
		//printf("%s", content);
		LrcLine line;
		//memset(&line, 0, sizeof(struct LrcLine));
		if (_stscanf(content, TEXT("[ti:%[^]\n\t]"), ti) > 0)
		{
			_tprintf(TEXT("[ti:%s]\n"), ti);
			line.ti = ti;
		}
		else if (_stscanf(content, TEXT("[ar:%[^]\n\t]"), ar) > 0)
		{
			_tprintf(TEXT("[ar:%s]\n"), ar);
			line.ar = ar;
		}
		else if (read_simple_items = _stscanf(content, TEXT("[%d:%d.%d]"), &minute, &second, &second2))
		{
			if (read_simple_items)
			{
				//content = "[00:12.85]<00:12.85>Wae <00:13.04>ba<00:13.24>bo<00:13.43>gach<00:13.63>yi <00:13.82>nal <00:14.21>mot <00:14.41>yit<00:14.60>go<00:14.99>\n"
				if (read_simple_items == EOF)
				{
					perror("read simple item failed!");
					ret = false;
					break;
				}

				// 심플 타임 스탬프 [mm:ss.xx] 읽기 성공. 
				line.millisecond = static_cast<int>((minute * 60 + second + second2 * 0.01) * 1000);

				num_simple_time_label++;

				TCHAR simple_time_stamp[128] = { '\0', };
				int time_m = 0;
				int time_s = 0;
				int time_s2 = 0;
				_stprintf_s(simple_time_stamp, _countof(simple_time_stamp), TEXT("[%02d:%02d.%02d]"), time_m, time_s, time_s2);

				// 강화 LRC 포맷 타임스탬프 <mm:ss.xx>.
				std::wstring strContent, strSimpleTimeStamp, strResult;
				strContent = content;
				strSimpleTimeStamp = simple_time_stamp;
				strContent.find(strSimpleTimeStamp);
				strResult = strContent.substr(strSimpleTimeStamp.length());

				std::wstring element;
				TCHAR etc_content[512] = { '\0', };
				memcpy(etc_content, strResult.c_str(), strResult.length() * sizeof(TCHAR));
				TCHAR temp2[64] = { '\0' };
				TCHAR* token = _tcstok(etc_content, TEXT("<"));
				while (token != NULL)
				{
					if (_tcsstr(token, TEXT("\n")))
					{
						element.append(token);
						std::wcout << TEXT("Line end reached.") << std::endl;
					}
					else
					{
						memcpy(temp2, token, _countof(temp2));
						TCHAR* pStr = _tcsstr(temp2, TEXT(">"));
						TCHAR* pStrTest = pStr + 1;
						if (pStr)
						{
							std::wcout << TEXT("test : ") << (pStr + 1) << std::endl;
							element.append(pStr + 1);
						}

					}
					token = _tcstok(NULL, TEXT("<"));
				}

				line.content = element;
				lrc_data.push_back(line);
			}
		}
	}

	fclose(lrc_file);

	return ret;
}

int lrc2srt(std::string lrcFile, std::string srtFile)
{
	setlocale(LC_CTYPE, "");

	LrcData lrc_data;
	{
		//load_lrc_file(argv[1], lrc_data);
		//save_lrc_as_srt(lrc_data);			
		loadLrc(lrcFile.c_str(), lrc_data);
		save_lrc_as_srt(srtFile.c_str(), lrc_data);
		return 0;
	}
	
	{
		std::string lrcPath("\\10223-k.lrc");
		std::string srtPath("\\10223-k.srt");

		char lrcDir[1024] = { '\0', };
		char srtDir[1024] = { '\0', };

		char curDir[1024] = { '\0', };
		char* buffer;

		if ((buffer = _getcwd(NULL, 0)) == NULL)
			perror("_getcwd error");
		else
		{
			printf("%s \nLength: %d\n", buffer, strnlen(buffer, 1024));

			memcpy(curDir, buffer, sizeof(curDir));
			printf("%s\n", curDir);

			free(buffer);

			memcpy(lrcDir, curDir, sizeof(lrcDir));
			memcpy(srtDir, curDir, sizeof(srtDir));

			strcat_s(lrcDir, sizeof(lrcDir), lrcPath.c_str());
			strcat_s(srtDir, sizeof(srtDir), srtPath.c_str());


			loadLrc(lrcDir, lrc_data);
			save_lrc_as_srt(srtDir, lrc_data);

			/* // 한글인지 아닌지
			FILE *fp = fopen(lrcDir, "r, ccs=UTF-8");
			if (fp == NULL)
			{
				perror("save::fopen faild");
				return false;
			}
			TCHAR inp[MAX_SIZE] = { '\0', };
			bool HANGUL = false;
			while (fgetws(inp, MAX_SIZE, fp))
			{
				wprintf(L"%s", inp);
				for (int i = 0; i < wcslen(inp); i++) {
					if ((inp[i] & 0x80) == 0x80)	//한글인지 확인
					{
						HANGUL = true;
					}
					if (!HANGUL)	//아니면 그냥 출력
					{
						wprintf(L"%c  ", inp[i]);
					}
					else
					{
						//한글이면 2byte 출력
						wprintf(L"%c%c", inp[i], inp[i + 1]);
						i++;
					}
					HANGUL = false;
				}
				wprintf(L"\n");
			}
			fclose(fp);
			*/
		}
	}
	return 0;
}