//
// Created by zhuyi on 2024/12/20.
//

#ifndef BOOK_H
#define BOOK_H
#include <iostream>
#include <fstream>
#include "Block.h"
#include "Blog.h"
#include "Error.h"
#include "Finance.h"
#include "Tokenscanner.h"
#include "User.h"

//存储所有图书相关信息
class BookInfo {
    friend class Book;
    friend class User;
    friend class Block<500 , 71 , 71>;
    friend class FinanceInfo;
private:
    char ISBN[71] = {'\0'};
    char BookName[71] = {'\0'};
    char Author[71] = {'\0'};
    char KeyWord[71] = {'\0'};
    int Quantity = 0;
    double Price = 0;
    double Totalcost = 0;
    bool hasindex = false; //判断当前图书是否具有关键字
public:
    BookInfo() = default;
    ~BookInfo() = default;
};


//Book系统中最重要最核心的类
class Book {
    friend class Finance;
    friend class User;
    friend class Blog;
private:
    Block<500 , 71  ,71> Book_ISBN; //ISBN->ISBN映射
    Block<500 , 71 , 71> Book_Name; //name->ISBN映射
    Block<500 , 71 , 71> Book_Author; //Author->ISBN映射
    Block<500 , 71 , 71> Block_keyword;  //keyword->ISBN映射

public:
    //当前选中的图书的全部信息
    BookInfo selected;
    Book():Book_ISBN("ISBN1", "ISBN2"), Book_Name("name1","name2"),Book_Author("author1","author2"),Block_keyword("keyword1","keyword2") {}
    ~Book();

    //检索图书
    void showInfo(const char* isbn , const char* bookname , const char* Author, const char* singlekeyword,User& UserManage, Blog& blog , const char* command);

    //special situation
    void showeverything(User& UserManage, Blog& blog , const char* command);

    //购买图书
    void Shopping(const char* isbn , const int QUANT ,User& UserManage, Finance& money , Blog& blog , const char* command);

    //选择图书
    void Select_Book(const char* isbn,User& UserManage, Blog& blog , const char* command);

    //修改图书信息
    void modify(const char* isbn , const char* bookname , const char* Author, const char* all_keywords , double price ,User& UserManage, Blog& blog , const char* command);

    //图书进货
    void ImportBook(const int QUANT , double COST ,User& UserManage,Finance& money, Blog& blog , const char* command);
};



#endif //BOOK_H
