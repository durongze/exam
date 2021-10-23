#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <vector>
#include <sys/types.h>
#include <unistd.h>

class MemInfo{
	public:
		MemInfo(){}
		~MemInfo(){}
		void ParseString(const std::string &str) {
    		std::stringstream str_stream;
			str_stream << str;
            str_stream >> mem_name >> mem_size >>  mem_unit;
		}
		void Show(std::ostream &os = std::cout) const {
			if (mem_name.find("VmPeak") != -1) {
				os << "--------------peak_virt_mem---------------" << std::endl;
			} else if (mem_name.find("VmSize") != -1) {
				os << "----------------virt_mem------------------" << std::endl;
			} else if (mem_name.find("VmHWM") != -1) {
				os << "--------------peak_phy_mem--------------" << std::endl;
			} else if (mem_name.find("VmRSS") != -1) {
				os << "----------------phy_mem-----------------" << std::endl;
			} else {
				return;
			}
			os << std::right << std::setw(26) << mem_name 
			   << std::right << std::setw(10) << mem_size
               << std::right << std::setw(10) << mem_unit << std::endl;
		}
	private:
    	std::string mem_name;
    	std::string mem_unit;
		int mem_size;
};

int GetMemInfo()
{
    pid_t pid = getpid();
    std::string line;
    std::stringstream proc_stat_file;
	std::vector<MemInfo> mem_info;

    proc_stat_file << "/proc/" << pid << "/status";
	std::cout << proc_stat_file.str() << std::endl;
    std::ifstream fp(proc_stat_file.str(), std::ios::in | std::ios::binary);
    if (!fp) {
        return 1;
    }

    while (std::getline(fp, line)) {
		MemInfo mem;
		mem.ParseString(line);
		mem.Show();
    }
    fp.close();
	return 0;
}

int main(int argc, char** argv) 
{
	std::string line;
	std::vector<char*> vp;
	for (int i = 0; i < 400000; ++i) {
		std::cout << "XXXXXXXXXXX " << i << " XXXXXXXXXXXXX" << std::endl;
		char *p = new char[1024*1024];
		// vp.push_back(p);
		GetMemInfo();
		delete[] p;
	}
	std::cin >> line;
	return 0;
}
