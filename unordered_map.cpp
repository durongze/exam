#include <map>
#include <iostream>
#include <unordered_map>
#include <sstream>
#include <cstring>
#include <memory>

class DbBase {
	public:
		DbBase(){}
		~DbBase(){}
};
using DbBasePtr = std::shared_ptr<DbBase>;

template<typename D = std::string>
class DbType : public DbBase {
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

using DbInt8=DbType<int8_t>;
using DbInt16=DbType<int16_t>;
using DbInt32=DbType<int32_t>;
using DbInt64=DbType<int64_t>;
using DbUInt8=DbType<uint8_t>;
using DbUInt16=DbType<uint16_t>;
using DbUInt32=DbType<uint32_t>;
using DbUInt64=DbType<uint64_t>;
using DbString=DbType<std::string>;

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
	DbBasePtr p = std::make_shared<DbString>("xxxxxxxxxx");
	std::static_pointer_cast<DbString>(p);
	return 0;	
}
