#include <iostream>
#include <string>
#include <vector>
#include <memory>

class File {
	public:
		File(const std::string &name):m_name(name){}
	public:
		virtual void Show(std::ostream &os, int depth) const {
			for (int i = 0; i < depth; ++i) {
				os << "\t"; 
			}
			os << m_name << std::endl;
		}
	protected:
		std::string m_name;
};
using FilePtr=std::shared_ptr<File>;

class Dir : public File {
    public:
		Dir(const std::string &name):File(name){}
	public:
		void Insert(const FilePtr file) {
			m_sub.push_back(file);
		}
	public:
		virtual void Show(std::ostream &os, int depth) const {
            File::Show(os, depth);
			for (auto iter = m_sub.cbegin(); iter != m_sub.cend(); ++iter) {
				(*iter)->Show(os, depth + 1);
			}
		}
	private:
		std::vector<FilePtr> m_sub;
};
using DirPtr=std::shared_ptr<Dir>;

DirPtr CreateLevel(DirPtr &dir, int childNum, const std::string &name)
{
    DirPtr sub;
	for (int j = 0; j < childNum; j++) {
		sub = std::make_shared<Dir>(name + "-" + std::to_string(j));
		dir->Insert(sub);
	}
	return sub;
}

int main (int argc, char *argv[])
{
	int i = 0;
	DirPtr parent;
	DirPtr top = std::make_shared<Dir>("Top");
	for (i = 0, parent = top; i < 10; i++) {
		parent = CreateLevel(parent, 3, std::to_string(i));
	}
	top->Show(std::cout, 0);
	return 0;
}
