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
#include "MemoryRiver.h"

//财务相关信息
class FinanceInfo {
    friend class Finance;
    friend class Book;
private:
    double price = 0.00; // 图书价格
    double cost = 0.00; //图书消费
    int sum = 0;  //交易总笔数
    bool state; // + or -
    char UserID[71] = {'\0'}; //进行操作的人员
    char ISBN[71] = {'\0'}; //被操作图书的isbn号
    double money; //交易金额
public:
    FinanceInfo():money(0.00) ,state(true){}
    explicit FinanceInfo(double Price, double Cost ,int Sum , bool Status , const char* User_ID, const char* isbn):price(Price),cost(Cost),sum(Sum), state(Status) {
        strcpy(UserID , User_ID);
        strcpy(ISBN , isbn);
    }
};
//与财务相关操作
class Finance {
    friend class Book;
    friend class FinanceInfo;
private:
    //财务报告  直接写在文件里
    MemoryRiver<FinanceInfo , 1> FinanceReport;
public:
    Finance();
    ~Finance();

    //获得当前交易笔数总数
    int FinanceCount();

    //财务记录查询
    void ShowFinance(int count ,User& UserManage);

    //生成财务记录报告
    void ReportFinance(User& UserManage);
};



#endif //FINANCE_H
