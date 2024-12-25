//
// Created by zhuyi on 2024/12/25.
//

#ifndef MEMORYRIVER_H
#define MEMORYRIVER_H
#include <fstream>

using std::string;
using std::fstream;
using std::ifstream;
using std::ofstream;

template<class T, int info_len = 2>
class MemoryRiver {
private:
    /* your code here */
    fstream file;
    string file_name;
    int sizeofT = sizeof(T);
public:
    MemoryRiver() = default;

    MemoryRiver(const string& file_name) : file_name(file_name) {}

    void initialise(string FN = "") {
        if (FN != "") file_name = FN;
        file.open(file_name, std::ios::out);
        int tmp = 0;
        for (int i = 0; i < info_len; ++i)
            file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
    }

    //读出第n个int的值赋给tmp，1_base
    void get_info(int &tmp, int n) {
        if (n > info_len) return;
        /* your code here */
        file.open(file_name);
        file.seekg((n - 1) * sizeof(int));
        file.read(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.close();
    }

    //将tmp写入第n个int的位置，1_base
    void write_info(int tmp, int n) {
        if (n > info_len) return;
        /* your code here */
        file.open(file_name, std::fstream::in | std::fstream::out);
        file.seekp(0);
        file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.seekg(0);
        int x;
        file.read(reinterpret_cast<char*> (&x), sizeof(int));
        file.close();
        return;
    }

    void clear() {
        file.open(file_name);
        file.clear();
        file.close();
    }
};


#endif //MEMORYRIVER_H
