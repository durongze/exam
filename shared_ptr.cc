#include <iostream>
#include <memory>
#include <fstream>

class My{
	public:
		void Dump(std::ostream &os) { os << __FUNCTION__ << __LINE__ << std::endl; }
};

template<typename T>
class SharedPtr {
	public:
		SharedPtr(std::shared_ptr<T> d):data(d){}
		std::shared_ptr<T> operator->() { return data; }
	private:
		std::shared_ptr<T> data;
};

int main(int argc, char** argv) {
	std::shared_ptr<My> p(new My);
	SharedPtr<My> sp(p);
	sp->Dump(std::cout);
	return 0;
}
