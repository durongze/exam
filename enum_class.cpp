#include <iostream>
#include <string>
#include <sstream>
#include <stdarg.h>
#include <vector>

#define ENUM_TO_STR(...) #__VA_ARGS__
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

#define EnumClass(Name, ...)   \
		enum Name{             \
			__VA_ARGS__        \
		};                     \
		const char* Name##StrArr[NUMARGS(__VA_ARGS__)] = {  \
			#__VA_ARGS__                                     \
		};                                                   \
		std::string Name##ToStr(Name type) {        \
            static std::vector<std::string> Name##Arr;\
			if (Name##Arr.size() <= 0) { InitEnumStrArr( NUMARGS(__VA_ARGS__), Name##StrArr, #__VA_ARGS__, Name##Arr); }      \
			if ( type >= NUMARGS(__VA_ARGS__) || Name##StrArr[type] == nullptr ) return "";              \
			return std::string(Name##StrArr[type]); \
		}                                           
		                                         
EnumClass(NumberType, Int32, Int64, UInt32, UInt64, Double, Float);

int main(int argc, char** argv)
{
	for (int i = NumberType::Int32; i < NumberType::Float; ++i) {
		std::cout << NumberTypeToStr((NumberType)i) << std::endl;
	}
	return 0;
}
