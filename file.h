#ifndef _FILE_H_
#define _FILE_H_

#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <vector>

template <typename S, typename D>
size_t ReadBaseDataFromStream(S &s, D &d) {
    s >> d;
    return 0;
}

template <typename S, typename D>
size_t WriteBaseDataToStream(S &s, const D &d) {
    s << d;
    return 0;
}

template <typename S, typename D>
size_t ReadVectorFromStream(S &s, std::vector<D> &d_vector) {
    size_t size = 0;
    size_t vector_size;
    s >> vector_size;
    d_vector.clear();
    d_vector.resize(vector_size);
    for (auto iter = d_vector.begin(); iter != d_vector.end(); ++iter) {
        s >> *iter;
        size += iter->Size();
    }
    return size;
}

template <typename S, typename D>
size_t WriteVectorToStream(S &s, const std::vector<D> &d_vector) {
    size_t size = 0;
    s << d_vector.size() << std::endl;
    for (auto iter = d_vector.cbegin(); iter != d_vector.cend(); ++iter) {
        s << *iter;
        size += iter->Size();
    }
    return size;
}

template <typename D>
size_t VectorSize(const std::vector<D> &d_vector) {
    size_t size = 0;
    for (auto iter = d_vector.cbegin(); iter != d_vector.cend(); ++iter) {
        size += iter->Size();
    }
    return size;
}

template <typename S, typename K, typename D>
size_t ReadMapFromStream(S &s, std::map<K, D> &d_map) {
    size_t size = 0;
    size_t map_size = 0;
    size_t k_size, d_size;
    K k;
    D d;
    s >> map_size;
    d_map.clear();
    for (size_t iter = 0; iter < map_size; ++iter) {
        s >> k;
        s >> d;
        d_map.insert(std::make_pair(k, d));
        size += k.Size() + d.Size();
    }
    return size;
}

template <typename S, typename K, typename D>
size_t WriteMapToStream(S &s, const std::map<K, D> &d_map) {
    size_t size = 0;
    s << d_map.size() << std::endl;
    for (auto iter = d_map.cbegin(); iter != d_map.cend(); ++iter) {
        s << iter->first;
        s << iter->second;
        size += iter->first.Size() + iter->second.Size();
    }
    return size;
}

template <typename K, typename D>
size_t MapSize(const std::map<K, D> &d_map) {
    size_t size = 0;
    for (auto iter = d_map.cbegin(); iter != d_map.cend(); ++iter) {
        size += iter->first.Size() + iter->second.Size();
    }
    return size;
}

template <class T>
T MakeData(const std::string &str) {
    std::istringstream iss(str);
    T data;
    iss >> data;
    return data;
}

template <class T>
std::string ToString(const T &data) {
    std::ostringstream oss;
    oss << data;
    return oss.str();
}

inline std::string GenPrefixByLevel(int level) {
    std::string prefix;
    for (int i = 0; i < level; ++i) {
        prefix += "..";
    }
    return prefix;
}

// template <typename D, typename Ss = std::fstream>
class BaseNode {
    using Ss = std::fstream;
  public:
    BaseNode() {}
    virtual ~BaseNode() {}
    virtual size_t ReadStream(Ss &s) = 0;
    virtual size_t WriteStream(Ss &s) = 0;
    virtual size_t Size() const = 0;
    virtual std::string ToStr(int level) const = 0;
    virtual const std::string GetDataType() const = 0;
  protected:
    static void RegisterDataType(const std::string &type_name) {
        reg_data_type_.insert(std::make_pair(type_name, reg_data_type_.size()));
    }
    static bool IsValidDataType(const std::string &type_name) { 
        return reg_data_type_.find(type_name) != reg_data_type_.end(); 
    }
    static std::map<std::string, int> reg_data_type_;
};

template <typename D, typename S = std::fstream>
class BaseData : virtual public BaseNode {
  public:
    BaseData() : data_size_(sizeof(D)) {
        BaseNode::RegisterDataType(typeid(D).name());
    }
    BaseData(std::string &d) : data_size_(sizeof(D)) {
        data_ = MakeData<D>(d);
        if (!IsSysType()) {
            data_size_ = d.length();
        }
        BaseNode::RegisterDataType(typeid(D).name());
    }
    BaseData(const D &d, size_t s = sizeof(D)) : data_(d), data_size_(s) {
        BaseNode::RegisterDataType(typeid(D).name());
    }

    virtual ~BaseData() {}

    bool IsSysType() const {
        return (typeid(data_) == typeid(bool) || typeid(data_) == typeid(int) || typeid(data_) == typeid(size_t) ||
                typeid(data_) == typeid(uint32_t) || typeid(data_) == typeid(uint64_t) ||
                typeid(data_) == typeid(float) || typeid(data_) == typeid(double));
    }

    virtual size_t ReadStream(S &s) {
        ReadBaseDataFromStream<S, D>(s, data_);
        return Size();
    }

    virtual size_t WriteStream(S &s) {
        WriteBaseDataToStream<S, D>(s, data_);
        return Size();
    }

    virtual size_t Size() const { return data_size_; }

    virtual std::string ToStr(int level) const {
        std::string prefix = GenPrefixByLevel(level);
        return prefix + ToString<D>(data_) + "\n";
    }

    template <typename OS>
    friend OS &operator<<(OS &s, const BaseData<D> &d) {
        s << d.Size() << std::endl;
        s << typeid(d.data_).name() << std::endl;
        if (d.IsSysType()) {
            s << d.data_ << std::endl;
            // s.write(reinterpret_cast<const char *>(&d.data_), sizeof(D));
        } else {
            s << d.data_ << std::endl;
        }
        return s;
    }
    template <typename IS>
    friend IS &operator>>(IS &s, BaseData<D> &d) {
        std::string type_name;
        s >> d.data_size_;
        s >> type_name;
        if (d.IsSysType()) {
            s >> d.data_;
            // s.read(reinterpret_cast<char *>(&d.data_), sizeof(D));
        } else {
            s >> d.data_;
        }
        return s;
    }
    bool operator<(const BaseData<D> &other) const { return data_ < other.data_; }

    const D &Value() const { return data_; }
    virtual const std::string GetDataType() const { return typeid(D).name(); }
  private:
    D data_;
    size_t data_size_;
};

// template <typename S = std::fstream>
class StructNode : virtual public BaseNode {
    using Ss = std::fstream;

  public:
    StructNode() {}
    virtual ~StructNode() {}
    virtual size_t ReadStream(Ss &s) {
        size_t size = 0;
        for (auto iter = child_.begin(); iter != child_.end(); ++iter) {
            size += (*iter)->ReadStream(s);
        }
        return size;
    }
    virtual size_t WriteStream(Ss &s) {
        size_t size = 0;
        for (auto iter = child_.begin(); iter != child_.end(); ++iter) {
            size += (*iter)->WriteStream(s);
        }
        return size;
    }
    virtual size_t Size() const {
        size_t size = 0;
        for (auto iter = child_.cbegin(); iter != child_.cend(); ++iter) {
            size += (*iter)->Size();
        }
        return size;
    }
    virtual std::string ToStr(int level) const {
        int idx = 0;
        std::ostringstream oss;
        std::string prefix = GenPrefixByLevel(level);

        oss << prefix << "ChildSize:" << child_.size() << "\n";
        for (auto iter = child_.cbegin(); iter != child_.cend(); ++iter) {
            oss << prefix << " child[" << (idx++) << "]:" << (*iter)->ToStr(level + 1) << "\n";
        }
        return oss.str();
    };

    int Add(BaseNode *child) {
        child_.push_back(child);
        return 0;
    }
    const std::vector<BaseNode*> &Value() const { return child_; }
    virtual const std::string GetDataType() const {return typeid(std::vector<BaseNode*>).name(); }
  private:
    std::vector<BaseNode*> child_;
};

template <typename D, typename S = std::fstream>
class DataVector : virtual public BaseNode {
  public:
    DataVector() {}
    virtual ~DataVector() {}
    // template <typename S>
    virtual size_t ReadStream(S &s) { return ReadVectorFromStream<S, D>(s, data_); }
    // template <typename S>
    virtual size_t WriteStream(S &s) { return WriteVectorToStream<S, D>(s, data_); }
    int PushBack(const D &d) {
        data_.push_back(d);
        return 0;
    }
    virtual size_t Size() const { return VectorSize<D>(data_); }
    virtual std::string ToStr(int level) const {
        std::string prefix = GenPrefixByLevel(level);
        std::stringstream s;
        WriteVectorToStream<std::stringstream, D>(s, data_);
        return prefix + s.str() + "\n";
    }

    void Dump(std::ostream &s = std::cout, const std::string &title = " ") {
        s << title << " | " << typeid(D).name() << std::endl;
        WriteVectorToStream<std::ostream, D>(s, data_);
        return;
    }
    const std::vector<D> &Value() const { return data_; }
    virtual const std::string GetDataType() const {return typeid(std::vector<D>).name(); }
  private:
    std::vector<D> data_;
};

template <typename K, typename D, typename S = std::fstream>
class DataMap : virtual public BaseNode {
  public:
    DataMap() {}
    virtual ~DataMap() {}
    // template <typename S>
    virtual size_t ReadStream(S &s) { return ReadMapFromStream<S, K, D>(s, data_); }
    // template <typename S>
    virtual size_t WriteStream(S &s) { return WriteMapToStream<S, K, D>(s, data_); }
    int Insert(const K &k, const D &d) {
        data_.insert(std::make_pair(k, d));
        return 0;
    }
    void Dump(std::ostream &s = std::cout, const std::string &title = " ") {
        s << title << " | " << typeid(K).name() << " | " << typeid(D).name() << std::endl;
        WriteMapToStream<std::ostream, K, D>(s, data_);
        return;
    }
    virtual size_t Size() const { return MapSize<K, D>(data_); }
    virtual std::string ToStr(int level) const {
        std::string prefix = GenPrefixByLevel(level);
        std::stringstream s;
        WriteMapToStream<std::stringstream, K, D>(s, data_);
        return prefix + s.str() + +"\n";
    }
    const std::map<K, D> &Value() const { return data_; }
    virtual const std::string GetDataType() const {return typeid(std::map<K, D>).name(); }
  private:
    std::map<K, D> data_;
};

using DataBool = BaseData<bool>;
using DataInt = BaseData<int>;
using DataInt8 = BaseData<int8_t>;
using DataInt16 = BaseData<int16_t>;
using DataInt32 = BaseData<int32_t>;
using DataInt64 = BaseData<int64_t>;
using DataUint8 = BaseData<uint8_t>;
using DataUint16 = BaseData<uint16_t>;
using DataUint32 = BaseData<uint32_t>;
using DataUint64 = BaseData<uint64_t>;
using DataFloat = BaseData<float>;
using DataDouble = BaseData<double>;
using DataString = BaseData<std::string>;

using DataVectorBool = DataVector<DataBool>;
using DataVectorInt = DataVector<DataInt>;
using DataVectorInt8 = DataVector<DataInt8>;
using DataVectorInt16 = DataVector<DataInt16>;
using DataVectorInt32 = DataVector<DataInt32>;
using DataVectorInt64 = DataVector<DataInt64>;
using DataVectorUint8 = DataVector<DataUint8>;
using DataVectorUint16 = DataVector<DataUint16>;
using DataVectorUint32 = DataVector<DataUint32>;
using DataVectorUint64 = DataVector<DataUint64>;
using DataVectorFloat = DataVector<DataFloat>;
using DataVectorDouble = DataVector<DataDouble>;
using DataVectorString = DataVector<DataString>;

using DataMapBool = DataMap<DataBool, DataBool>;
using DataMapInt = DataMap<DataInt, DataInt>;
using DataMapInt8 = DataMap<DataInt8, DataInt8>;
using DataMapInt16 = DataMap<DataInt16, DataInt16>;
using DataMapInt32 = DataMap<DataInt32, DataInt32>;
using DataMapInt64 = DataMap<DataInt64, DataInt64>;
using DataMapUint8 = DataMap<DataUint8, DataUint8>;
using DataMapUint16 = DataMap<DataUint16, DataUint16>;
using DataMapUint32 = DataMap<DataUint32, DataUint32>;
using DataMapUint64 = DataMap<DataUint64, DataUint64>;
using DataMapFloat = DataMap<DataFloat, DataFloat>;
using DataMapDouble = DataMap<DataDouble, DataDouble>;
using DataMapString = DataMap<DataString, DataString>;

#endif
