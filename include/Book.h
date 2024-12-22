//
// Created by zhuyi on 2024/12/20.
//

#ifndef BOOK_H
#define BOOK_H
#include "Block.h"
#include "Blog.h"
#include "Book.h"
#include "Error.h"
#include "Finance.h"
#include "Tokenscanner.h"
#include "User.h"

//Book系统中最重要最核心的类
class Book {

};


//存储所有图书相关信息
class BookInfo {
private:
    // 首先是一些需要用于类模板Block实现的成员变量
    int size = 0;  //当前块的大小
    int indexnum = 0; //当前块在块状链表中的位置
    int next = 0; // 下一个块在块状链表中的位置

    char ISBN[21] = {'\0'};
    char BookName[61] = {'\0'};
    char Author[61] = {'\0'};
    char KeyWord[61] = {'\0'};
    int Quantity = 0;
    double Price = 0;
    double Totalcost = 0;
    bool hasindex = false; //判断当前图书是否具有关键字
public:

};


//存储各种关键字到某一本书ISBN号关系的映射
class MAP {
    friend class Block<MAP>;
    friend class Book;
private:
    // 首先是一些需要用于类模板Block实现的成员变量
    int size = 0;
    int indexnum = 0;
    int next = 0;
    //关键字都是char[61]字符数组
    char index[61] = {'\0'};
    char ISBN[21] = {'\0'};

public:
    MAP() = default;
};



#endif //BOOK_H
