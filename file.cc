#include <fstream>
#include <iostream>
#include <sstream>

#include "file.h"

std::map<std::string, int> BaseNode::reg_data_type_;

template <typename T>
std::string GenStrByType(int idx) {
    std::ostringstream oss;
    if (typeid(T) == typeid(DataBool)) {
        oss << (idx % 2);
    } else if (typeid(T) == typeid(DataInt)) {
        oss << -(3200 + idx);
    } else if (typeid(T) == typeid(DataInt64)) {
        oss << -(6400 + idx);
    } else if (typeid(T) == typeid(DataUint32)) {
        oss << (3200 + idx);
    } else if (typeid(T) == typeid(DataUint64)) {
        oss << (6400 + idx);
    } else if (typeid(T) == typeid(DataFloat)) {
        oss << (3200.32 + idx);
    } else if (typeid(T) == typeid(DataDouble)) {
        oss << (6400.64 + idx);
    } else if (typeid(T) == typeid(DataString)) {
        oss << (555 + idx);
    } else {
        oss << (777 + idx);
    }
    return oss.str();
}

template <typename C, typename FS>
int DataIoMain(C &src, C &dst, FS &fs, const std::string &save_file) {
    fs.open(save_file.c_str(), std::ios::out | std::ios::trunc);
    src.WriteStream(fs);
    fs.close();

    fs.open(save_file.c_str(), std::ios::in);
    dst.ReadStream(fs);
    fs.close();

    std::cout << typeid(C).name() << " size:{" << dst.Size() << "}" << std::endl;
    return 0;
}

template <typename V, typename D, typename FS>
int DataVectorMain(V &src, V &dst, FS &fs, size_t map_size, const std::string &save_file) {
    for (int idx = 0; idx < map_size; ++idx) {
        std::string str = GenStrByType<D>(idx);
        src.PushBack(D(str));
    }

    DataIoMain<V, FS>(src, dst, fs, save_file);
    return 0;
}

template <typename M, typename D, typename FS>
int DataMapMain(M &src, M &dst, FS &fs, size_t map_size, const std::string &save_file) {
    for (int idx = 0; idx < map_size; ++idx) {
        std::string str = GenStrByType<D>(idx);
        src.Insert(D(str), D(str));
    }

    DataIoMain<M, FS>(src, dst, fs, save_file);
    return 0;
}

#define ContainerMain(top, c, t, v)                                            \
    {                                                                          \
        size_t _size = v;                                                      \
        std::string save_file = std::to_string(_size) + #c + #t;               \
        std::fstream fs;                                                       \
        Data##c##t _src, *_dst = new Data##c##t;                               \
        Data##c##Main<Data##c##t, Data##t>(_src, *_dst, fs, _size, save_file); \
        top.Add(_dst);                                                         \
    }

#define Main(top, c)                          \
    {                                             \
        ContainerMain(top, c, Bool, data_size);   \
        ContainerMain(top, c, Int, data_size);    \
        ContainerMain(top, c, Int32, data_size);  \
        ContainerMain(top, c, Int64, data_size);  \
        ContainerMain(top, c, Uint32, data_size); \
        ContainerMain(top, c, Uint64, data_size); \
        ContainerMain(top, c, Float, data_size);  \
        ContainerMain(top, c, Double, data_size); \
        ContainerMain(top, c, String, data_size); \
    }

int main(int argc, char **argv) {
    int data_size = 2;
    StructNode x;

    Main(x, Vector);
    Main(x, Map);

    std::cout << "-----------" << std::endl << x.ToStr(1) << "==============" << std::endl;

    return 0;
}
