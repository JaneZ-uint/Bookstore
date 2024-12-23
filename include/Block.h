//
// Created by zhuyi on 2024/12/20.
//

#ifndef BLOCK_H
#define BLOCK_H
#include<iostream>
#include<algorithm>
#include<fstream>
#include "Blog.h"
#include "Book.h"
#include "Error.h"
#include "Tokenscanner.h"
#include "User.h"

const int Block_size = 4000;  //需要调整，根据实际情况以及我又没有de出来那个bug
const int Key_size = 71; // 键的长度固定为 71 字节

// 定义模板参数
template <typename ValueType, int BlockSize = Block_size, int KeySize = Key_size>
class Block {
private:
    using arr = std::array<std::pair<char[KeySize + 1], ValueType>, BlockSize>; // 键为 char[65]

    struct head {
        std::pair<char[KeySize + 1], ValueType> maxele;
        std::pair<char[KeySize + 1], ValueType> minele;
        int size = 0;
        int indexnum = 0;
    };

    struct node {
        head info;
        node* next;
        node(head info) : info(info), next(nullptr) {}
    };

    std::string file1_name;
    std::fstream file1;
    std::string file2_name;
    std::fstream file2;
    node* Head;
    node* tair;

    // 比较函数
    bool larger(const std::pair<char[KeySize + 1], ValueType>& a, const std::pair<char[KeySize + 1], ValueType>& b) {
        int cmp = strcmp(a.first, b.first);
        if (cmp > 0) {
            return true;
        } else if (cmp < 0) {
            return false;
        } else {
            return a.second > b.second;
        }
    }

    bool largerequal(const std::pair<char[KeySize + 1], ValueType>& a, const std::pair<char[KeySize + 1], ValueType>& b) {
        int cmp = strcmp(a.first, b.first);
        if (cmp > 0) {
            return true;
        } else if (cmp == 0) {
            return a.second >= b.second;
        }
        return false;
    }

    bool smaller(const std::pair<char[KeySize + 1], ValueType>& a, const std::pair<char[KeySize + 1], ValueType>& b) {
        int cmp = strcmp(a.first, b.first);
        if (cmp < 0) {
            return true;
        } else if (cmp > 0) {
            return false;
        } else {
            return a.second < b.second;
        }
    }

    bool smallerequal(const std::pair<char[KeySize + 1], ValueType>& a, const std::pair<char[KeySize + 1], ValueType>& b) {
        int cmp = strcmp(a.first, b.first);
        if (cmp < 0) {
            return true;
        } else if (cmp == 0) {
            return a.second <= b.second;
        }
        return false;
    }

    bool equal(const std::pair<char[KeySize + 1], ValueType>& a, const std::pair<char[KeySize + 1], ValueType>& b) {
        return strcmp(a.first, b.first) == 0 && a.second == b.second;
    }

    // 分块函数
    void splitBlock(node* current, const std::pair<char[KeySize + 1], ValueType>& tmp, arr& a, int i) {
        head new_headnode; // 创建新的头节点
        arr a0;
        if (i == BlockSize) {
            // 说明来到了整个块状链表的最后一个位置
            strncpy(a0[0].first, tmp.first, KeySize);
            a0[0].first[KeySize] = '\0'; // 确保字符串以 '\0' 结尾
            a0[0].second = tmp.second;
            new_headnode.size = 1;
            file2.seekp(0, std::ios::end);
        } else {
            // 正常情况下的分块操作
            for (int j = i; j < current->info.size; j++) {
                strncpy(a0[j - i].first, a[j].first, KeySize);
                a0[j - i].first[KeySize] = '\0';
                a0[j - i].second = a[j].second;
            }
            for (int j = i; j < current->info.size; j++) {
                strncpy(a[j].first, "\0", KeySize);
                a[j].first[KeySize] = '\0';
                a[j].second = 0;
            }
            strncpy(a[i].first, tmp.first, KeySize);
            a[i].first[KeySize] = '\0';
            a[i].second = tmp.second;
            current->info.size = i + 1;
            file2.seekp(current->info.indexnum * sizeof(arr));
            file2.write(reinterpret_cast<char*>(&a), sizeof(a));
            file2.seekp(0, std::ios::end);
            new_headnode.size = BlockSize - i;
        }
        new_headnode.indexnum = file2.tellp() / sizeof(arr);
        // 将新块写入file2中
        file2.write(reinterpret_cast<char*>(&a0), sizeof(arr));
        current->info.size = std::min(i + 1, BlockSize);
        strncpy(current->info.minele.first, a[0].first, KeySize);
        current->info.minele.first[KeySize] = '\0';
        current->info.minele.second = a[0].second;
        strncpy(current->info.maxele.first, a[current->info.size - 1].first, KeySize);
        current->info.maxele.first[KeySize] = '\0';
        current->info.maxele.second = a[current->info.size - 1].second;
        strncpy(new_headnode.minele.first, a0[0].first, KeySize);
        new_headnode.minele.first[KeySize] = '\0';
        new_headnode.minele.second = a0[0].second;
        strncpy(new_headnode.maxele.first, a0[new_headnode.size - 1].first, KeySize);
        new_headnode.maxele.first[KeySize] = '\0';
        new_headnode.maxele.second = a0[new_headnode.size - 1].second;

        node* p = new node(new_headnode);
        p->next = current->next;
        current->next = p;
    }

public:
    Block() = default;
    Block(const std::string& file1name, const std::string& file2name) :
        file1_name(file1name), file2_name(file2name) {
        file1.open(file1name, std::ios::binary | std::ios::in | std::ios::out);
        file2.open(file2name, std::ios::binary | std::ios::in | std::ios::out);
        if (!file1) {
            file1.open(file1name, std::ios::binary | std::ios::out);
        }
        if (!file2) {
            file2.open(file2name, std::ios::binary | std::ios::out);
        }

        head tmp;
        Head = new node(tmp);
        tair = Head;
        while (true) {
            file1.read(reinterpret_cast<char*>(&tmp), sizeof(head));
            if (file1.eof()) {
                break;
            }
            tair->next = new node(tmp);
            tair = tair->next;
        }
        file1.close();
        file2.close();
    }

    ~Block() {
        file1.open(file1_name, std::ios::binary | std::ios::in | std::ios::out);
        node* current = Head->next;
        delete Head;
        while (current != nullptr) {
            file1.write(reinterpret_cast<char*>(&current->info), sizeof(head));
            node* next = current->next;
            delete current;
            current = next;
        }
    }

    void insert(const char* key, const ValueType& value) {
        file2.open(file2_name, std::ios::in | std::ios::out | std::ios::binary);
        node* current = Head->next;
        std::pair<char[KeySize + 1], ValueType> tmp;
        strncpy(tmp.first, key, KeySize);
        tmp.first[KeySize] = '\0'; // 确保字符串以 '\0' 结尾
        tmp.second = value;
        arr a;
        while (current != nullptr) {
            if (smallerequal(tmp, current->info.maxele) || current->next == nullptr) {
                file2.seekg(current->info.indexnum * sizeof(arr));
                file2.read(reinterpret_cast<char*>(&a), sizeof(arr));
                int i;
                // 顺序查找版的 lowerbound
                for (i = 0; i < current->info.size; i++) {
                    if (largerequal(a[i], tmp)) {
                        break;
                    }
                }
                if (equal(a[i], tmp)) {
                    file2.close();
                    return;
                } else {
                    // 如果块还未满
                    if (current->info.size < BlockSize) {
                        for (int j = current->info.size - 1; j >= i; j--) {
                            strncpy(a[j + 1].first, a[j].first, KeySize);
                            a[j + 1].first[KeySize] = '\0';
                            a[j + 1].second = a[j].second;
                        }
                        strncpy(a[i].first, key, KeySize);
                        a[i].first[KeySize] = '\0';
                        a[i].second = value;
                        file2.seekp(current->info.indexnum * sizeof(arr));
                        file2.write(reinterpret_cast<char*>(&a), sizeof(arr));
                        strncpy(current->info.minele.first, a[0].first, KeySize);
                        current->info.minele.first[KeySize] = '\0';
                        current->info.minele.second = a[0].second;
                        strncpy(current->info.maxele.first, a[current->info.size].first, KeySize);
                        current->info.maxele.first[KeySize] = '\0';
                        current->info.maxele.second = a[current->info.size].second;
                        current->info.size++;
                    } else {
                        // 如果块已经满了，进行裂块操作
                        splitBlock(current, tmp, a, i);
                    }
                }
                file2.close();
                return;
            }
            current = current->next;
        }
        strncpy(a[0].first, tmp.first, 65);
        a[0].second = tmp.second;
        file2.seekp(0);
        file2.write(reinterpret_cast<char *>(&a), sizeof(arr));
        head n;
        strncpy(n.maxele.first, tmp.first, 65);
        n.maxele.second = tmp.second;
        strncpy(n.minele.first, tmp.first, 65);
        n.minele.second = tmp.second;
        n.size = 1;
        tair = new node(n);
        Head -> next = tair;
        file2.close();
    }

    // delete操作
    void remove(const char* key, const ValueType& value) {
        file2.open(file2_name, std::ios::in | std::ios::out | std::ios::binary);
        node* current = Head;
        std::pair<char[KeySize + 1], ValueType> tmp;
        strncpy(tmp.first, key, KeySize);
        tmp.first[KeySize] = '\0';
        tmp.second = value;
        while (current->next != nullptr) {
            // 如果在这个块中
            if (smallerequal(current->next->info.minele, tmp) && largerequal(current->next->info.maxele, tmp)) {
                arr a;
                file2.seekg(current->next->info.indexnum * sizeof(arr));
                file2.read(reinterpret_cast<char*>(&a), sizeof(arr));
                for (int i = 0; i <= current->next->info.size; i++) {
                    if (equal(a[i], tmp)) {
                        // 如果当前块中只有一个元素 需要删除空块
                        if (current->next->info.size == 1) {
                            node* t = current->next;
                            current->next = t->next;
                            delete t;
                        } else {
                            for (int j = i + 1; j < current->next->info.size; j++) {
                                strncpy(a[j - 1].first, a[j].first, KeySize);
                                a[j - 1].first[KeySize] = '\0';
                                a[j - 1].second = a[j].second;
                            }
                            current->next->info.size--;
                            // 更新最小元素
                            strncpy(current->next->info.minele.first, a[0].first, KeySize);
                            current->next->info.minele.first[KeySize] = '\0';
                            current->next->info.minele.second = a[0].second;
                            // 更新最大元素
                            strncpy(current->next->info.maxele.first, a[current->next->info.size - 1].first, KeySize);
                            current->next->info.maxele.first[KeySize] = '\0';
                            current->next->info.maxele.second = a[current->next->info.size - 1].second;
                            // 尾置元素 清零
                            strncpy(a[current->next->info.size].first, "\0", KeySize);
                            a[current->next->info.size].first[KeySize] = '\0';
                            a[current->next->info.size].second = 0;
                            file2.seekp(current->next->info.indexnum * sizeof(arr));
                            file2.write(reinterpret_cast<char*>(&a), sizeof(arr));
                        }
                        file2.close();
                        return;
                    }
                    if (larger(a[i], tmp)) {
                        file2.close();
                        return;
                    }
                }
            }
            current = current->next;
        }
        file2.close();
    }

    // find操作
    void find(const char* key) {
        file2.open(file2_name, std::ios::binary | std::ios::in | std::ios::out);
        node* current = Head->next;
        char tmp[KeySize + 1] = {'\0'};
        strncpy(tmp, key, KeySize);
        tmp[KeySize] = '\0';
        std::vector<ValueType> result;
        while (current != nullptr) {
            // 如果有在当前块中的可能性
            if (strcmp(current->info.minele.first, tmp) <= 0 && strcmp(current->info.maxele.first, tmp) >= 0) {
                arr a;
                file2.seekg(current->info.indexnum * sizeof(arr));
                file2.read(reinterpret_cast<char*>(&a), sizeof(arr));
                for (int i = 0; i < current->info.size; i++) {
                    if (strcmp(a[i].first, tmp) == 0) {
                        result.push_back(a[i].second);
                    } else if (strcmp(a[i].first, tmp) > 0) {
                        break;
                    }
                }
            } else if (strcmp(current->info.minele.first, tmp) > 0) {
                break;
            }
            current = current->next;
        }
        if (result.empty()) {
            std::cout << "null";
        } else {
            std::sort(result.begin(), result.end());
            for (auto it : result) {
                std::cout << it << " ";
            }
        }
        std::cout << std::endl;
        file2.close();
    }
};

#endif //BLOCK_H
