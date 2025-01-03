//
// Created by zhuyi on 2024/12/25.
//

#ifndef MEMORYRIVER_H
#define MEMORYRIVER_H
#include <fstream>
#include "Finance.h"
#include <unistd.h>

using std::string;
using std::fstream;
using std::ifstream;
using std::ofstream;

template<class T, int info_len = 2>
class MemoryRiver {
    friend class Finance;
    friend class Blog;
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
        file.open(file_name, std::ios::in|std::ios::out);
        if(!file) {
            file.open(file_name, std::ios::out);
            int tmp = 0;
            for (int i = 0; i < info_len; ++i)
                file.write(reinterpret_cast<char *>(&tmp), sizeof(int));

        }
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
        file.open(file_name, std::ios::in | std::ios::out);
        file.seekp(0);
        file.write(reinterpret_cast<char *>(&tmp), sizeof(int));
        file.seekg(0);
        int x;
        file.read(reinterpret_cast<char*> (&x), sizeof(int));
        file.close();
    }

    void write(T &t,  int index , int size = 1) {
        /* your code here */
        file.open(file_name , std::ios::in | std::ios::out);
        file.seekp(index);
        file.write(reinterpret_cast<char*>(&t),sizeofT*size);
        file.close();
    }

    void read(T &t,  int index, int size) {
        /* your code here */
        file.open(file_name);
        file.seekg(index);
        file.read(reinterpret_cast<char*>(&t) , sizeofT*size);
        file.close();
    }

    void clear() {
        file.open(file_name);
        file.clear();
        file.close();
    }

};


#endif //MEMORYRIVER_H
