#ifdef _MSC_VER
#include <Windows.h>
#endif
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstdlib>

void SetCmdRed()
{
#ifdef _MSC_VER
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
#else
  printf("\033[31m");
#endif
}

void SetCmdGreen()
{
#ifdef _MSC_VER
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);
#else
  printf("\033[32m");
#endif
}

void SetCmdBlue()
{
#ifdef _MSC_VER
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE);
#else
  printf("\033[33m");
#endif
}

void SetCmdDefault()
{
#ifdef _MSC_VER
  SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
#else
  printf("\033[0m");
#endif
}

int args_main (int argc, char** argv, std::ostream &os)
{
  int i = 0;
  SetCmdGreen();
  os << "++++++++++" << __DATE__ << __FILE__ << __TIME__ << ":" << __FUNCTION__ << ":" << std::to_string(__LINE__) << "++++++++++" << std::endl;
  SetCmdDefault();
  os << argv[0] << " argc : " << std::to_string(argc) << ", argv : " << std::endl;
  for (; i < argc; ++i) {
    os << "argv[" << std::to_string(i) << "]:" <<argv[i] << std::endl;
  }
  os << argv[0] << " argc : " << std::to_string(argc) << ", argv : " << std::endl;
  os << "-----------------------------------------------------" << std::endl;
  return 0;
}

int gen_script(int argc, char** argv, std::ostream &os)
{
  int i = 0;
  SetCmdGreen();
  os << "##########" << __DATE__ << __FILE__ << __TIME__ << ":" << __FUNCTION__ << ":" << std::to_string(__LINE__) << "##########" << std::endl;
  SetCmdDefault();
  for (; i < argc; ++i) {
    os << argv[i] << " ";
  }
  os << std::endl;
  return 0;
}

int log_main (int argc, char** argv)
{
  std::string test_args = "test_args_log.txt";
  std::fstream fs(test_args.c_str(), std::ios::out | std::ios::app);
  args_main(argc, argv, std::cout);
  args_main(argc, argv, fs);
  return 0;
}

int script_main (int argc, char** argv)
{
  std::string test_args = "test_args_log.sh";
  std::fstream fs(test_args.c_str(), std::ios::out | std::ios::app);
  gen_script(argc, argv, std::cout);
  gen_script(argc, argv, fs);
  return 0;
}

int exec_main (int argc, char** argv)
{
  int i = 0;
  std::stringstream str_os;
  str_os << "origin_";
  for (; i < argc; ++i) {
    str_os << argv[i] << " ";
  }
  system(str_os.str().c_str());
  return 0;
}

int main (int argc, char** argv)
{
  log_main(argc, argv);
  script_main(argc, argv);
  exec_main(argc, argv);
  return 0;
}
