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

class Blog;
class User;
class Finance;

//存储所有图书相关信息
class BookInfo {
    friend class Book;
    friend class User;
    friend class Block<500 , 71 , BookInfo>;
    friend class FinanceInfo;
private:
    char ISBN[71] = {'\0'};
    char BookName[71] = {'\0'};
    char Author[71] = {'\0'};
    char KeyWord[71] = {'\0'};
    int Quantity = 0;
    double Price = 0; //售价
    double Importprice = 0; // 进价
    bool hasindex = false; //判断当前图书是否具有关键字
public:
    BookInfo() = default;
    BookInfo(const char* isbn) {
        strcpy(ISBN, isbn);
    }

    //重载运算符

    bool operator==(const BookInfo &) const;

    bool operator<(const BookInfo &)const;

    bool operator<=(const BookInfo &)const;

    bool operator>(const BookInfo &)const;

    bool operator>=(const BookInfo& )const;
};


//Book系统中最重要最核心的类
class Book {
    friend class Finance;
    friend class User;
    friend class Blog;
private:
    //表面上看映射到BookInfo 实际上直接映射的是 ISBN(好吧好像也不完全
    Block<500 , 71  ,BookInfo> Book_ISBN; //ISBN->ISBN映射
    Block<500 , 71 , BookInfo> Book_Name; //name->ISBN映射
    Block<500 , 71 , BookInfo> Book_Author; //Author->ISBN映射
    Block<500 , 71 , BookInfo> Book_keyword;  //keyword->ISBN映射

public:
    //当前选中的图书的全部信息
    //根据登录栈中当前最后一个账户是否选择 选择了什么来决定
    //logout select 等操作时不断更新这个selected
    BookInfo selected;
    Book():Book_ISBN("ISBN1.txt", "ISBN2.txt"), Book_Name("name1.txt","name2.txt"),Book_Author("author1.txt","author2.txt"),Book_keyword("keyword1.txt","keyword2.txt") {}
    ~Book();

    //检索图书
    void showInfo(const char* isbn , const char* bookname , const char* Author, const char* singlekeyword,User& UserManage, Blog& blog);

    //special situation
    void showeverything(User& UserManage, Blog& blog );

    //购买图书
    void Shopping(const char* isbn , const int QUANT ,User& UserManage, Finance& money , Blog& blog);

    //选择图书
    void Select_Book(const char* isbn,User& UserManage, Blog& blog,const char* command);

    //分割多个关键词
    std::vector<std::string> SplitKeywords(const char* allkeyword);

    //修改图书信息
    void modify(const char* isbn , const char* bookname ,const char* Author, const char* all_keywords , double price ,User& UserManage, Blog& blog , const char* command);

    //图书进货
    void ImportBook(const int QUANT , double COST ,User& UserManage,Finance& money, Blog& blog , const char* command);
};



#endif //BOOK_H
