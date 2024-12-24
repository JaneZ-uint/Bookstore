//
// Created by zhuyi on 2024/12/20.
//

#ifndef FINANCE_H
#define FINANCE_H

#include <iostream>
#include "Block.h"
#include "Blog.h"
#include "Book.h"
#include "Error.h"
#include "Tokenscanner.h"
#include "User.h"


//财务相关信息
class FInanceInfo {
private:
    double price = 0.00; // 图书价格
    double cost = 0.00; //图书消费
    int sum = 0;  //交易总笔数
    bool state; // + or -
    char UserID[71] = {'\0'}; //进行操作的人员
    char ISBN[71] = {'\0'}; //被操作图书的isbn号
public:
    explicit FInanceInfo(double Price, double Cost ,int Sum , bool Status , const char* User_ID, const char* isbn):price(Price),cost(Cost),sum(Sum), state(Status) {
        strcpy(UserID , User_ID);
        strcpy(ISBN , isbn);
    }
};
//与财务相关操作
class Finance {
    friend class Book;
private:

public:
    Finance();
    ~Finance();

    //财务记录查询
    void ShowFinance(const int count ,User& UserManage);

    //生成财务记录报告
    void ReportFinance(User& UserManage);
};



#endif //FINANCE_H
