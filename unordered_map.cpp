#include <map>
#include <iostream>
#include <unordered_map>
#include <sstream>

template<typename D = std::string>
class DbType {
	public:
		DbType(){}
		DbType(const D& d):m_d(d){}
		DbType(const DbType &other) { m_d = other.m_d;}
		~DbType(){}
		D &Value(){ return m_d; }
		const D &Value() const { return m_d; }
		DbType &operator=(const DbType &other) { m_d = other.m_d; return *this;}
		bool operator==(const DbType &other) const { return m_d == other.m_d; }
		void Dump() const { std::cout << m_d << std::endl;}
	private:
		D m_d;
};

template<typename K>
struct DbHash {
	std::size_t operator()(const K& k) const {
		auto val = k.Value();
		return std::hash<decltype(val)>{}(val); 
	}
};

template<typename K, typename V, typename H = DbHash<K> >
class UnorderedMap {
	public:
		UnorderedMap(){}
		V &operator[](const K& k){return m_map[k];}
		void Dump() {
			for (auto &e : m_map) {
				e.first.Dump();
				e.second.Dump();
			}
		}
	private:
		std::unordered_map<K, V, H> m_map;
};

int main() {
	UnorderedMap< DbType<std::string>, DbType<std::string> > m;
	for (int i = 0; i < 10; ++i) {
		m[DbType<std::string>(std::to_string(i))] = DbType<std::string>(std::to_string(i));
	}
	m.Dump();
	return 0;	
}
