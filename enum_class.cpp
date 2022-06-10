#include <iostream>
#include <string>
#include <sstream>
#include <stdarg.h>
#include <vector>
#include <map>

#define ENUM_TO_STR(...) #__VA_ARGS__
#define ENUM_VAL(N, V) N = V
#define NUMARGS(...)  (sizeof((int[]){0, ##__VA_ARGS__})/sizeof(int)-1)

void InitEnumStrArr(int argc, const char *argv[], const std::string &str, std::vector<std::string> &str_arr) {
	std::string argv_str = str;
	if (argv == nullptr || std::string(argv[0]) != str) {
		std::cout << argv[0] << " vs " << str << std::endl;
        return ;
	}
	str_arr.resize(argc);
    for(int i = 0; i < argc; i++)  
    {  
		str_arr[i] = std::string(argv_str.substr(0, argv_str.find(",")));
        argv[i] = str_arr[i].c_str();
		argv_str = argv_str.substr(argv_str.find(",") + 1);
		std::cout << "argv[" << i << "]:" << argv[i] << std::endl;
    }  
}

void ProcEnumStrArr(const std::string &str, std::map<int, std::string> &str_arr) {
	std::string argv_str = str;
	int argc = 0;
	int pos = 0;
	for(argc = 0; (pos = argv_str.find(",", pos + 3)) != -1; argc++){
	}
    for(int i = 0; i < argc + 1; i++)  
    {  
		std::string cur_str(argv_str.substr(0, argv_str.find(",")));
		int key = atoi(cur_str.substr(cur_str.find("=") + 1, cur_str.length()).c_str());
		str_arr[key] = cur_str.substr(0, cur_str.find("="));
		argv_str = argv_str.substr(argv_str.find(",") + 1);
		std::cout << "str_arr[" << key << "]:" << str_arr[key] << std::endl;
    }  
}

#define EnumExt(Name, ...)   \
		const char* Name##StrArr[NUMARGS(__VA_ARGS__)] = {   \
			#__VA_ARGS__                                     \
		};                                                   \
		std::string Name##ToStr(Name type) {                 \
            static std::vector<std::string> Name##Arr;       \
			if (Name##Arr.size() <= 0) { InitEnumStrArr( NUMARGS(__VA_ARGS__), Name##StrArr, #__VA_ARGS__, Name##Arr); }   \
			if ( type >= NUMARGS(__VA_ARGS__) || Name##StrArr[type] == nullptr ) return "";        \
			return std::string(Name##StrArr[type]);          \
		}
		
#define EnumProc(Name, ...)   \
		std::string Name##ToStr(Name type) {                 \
        	static std::map<int, std::string> Name##Arr;     \
			if (Name##Arr.size() <= 0) { ProcEnumStrArr(#__VA_ARGS__, Name##Arr); }   \
			return (Name##Arr[type]);                        \
		} 					  \

#define EnumBase(Name, ...)    \
		enum Name{             \
			__VA_ARGS__        \
		};                     \
		                                         
#define EnumClass(Name, ...)        \
		EnumBase(Name, __VA_ARGS__) \
		EnumExt(Name, __VA_ARGS__)  \

#define EnumFunc(Name, ...)         \
		EnumBase(Name, __VA_ARGS__) \
		EnumProc(Name, __VA_ARGS__) \

EnumFunc(NumberType, Int32 = 32, Int64 = 64, UInt32 = 3232, UInt64 = 6464, Double = 6400, Float = 3200);

//EnumClass(NumberType, Int32, Int64, UInt32, UInt64, Double, Float);

int main(int argc, char** argv)
{
	std::cout << NumberTypeToStr(Int32) << std::endl;
	std::cout << NumberTypeToStr(Int64) << std::endl;
	std::cout << NumberTypeToStr(UInt32) << std::endl;
	std::cout << NumberTypeToStr(UInt64) << std::endl;
	std::cout << NumberTypeToStr(Double) << std::endl;
	std::cout << NumberTypeToStr(Float) << std::endl;
	return 0;
}
