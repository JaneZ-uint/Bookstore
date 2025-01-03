//
// Created by zhuyi on 2024/12/20.
//
#include "../include/Finance.h"
#include "../include/Finance.h"

#include <iomanip>

Finance::Finance() {
    FinanceReport.initialise("Financefile.txt");
}

Finance::~Finance()=default;

int Finance::FinanceCount() {
    int count;
    FinanceReport.get_info(count,1);
    return count;
}

void Finance::ShowFinance(const int count, User &UserManage) {
    if(UserManage.currentlevel != 7) {
        throw InvalidExpression();
    }
    int total = FinanceCount();
    if(count > FinanceCount()) {
        throw InvalidExpression();
    }
    if(count == 0) {
        std::cout<<'\n';
        return;
    }
    double income = 0.00;
    double outcome = 0.00;
    auto *target = new FinanceInfo[count];
    FinanceReport.read(target[0] , 4 + sizeof(FinanceInfo)*(total - count),count);
    for(int i = 0 ;i < count ; i++) {
        if(target[i].state) {
            income += target[i].money;
        }else {
            outcome += target[i].money;
        }
    }
    std::cout<<"+ "<<std::fixed<<std::setprecision(2)<<income<<" "<<"- "<<std::fixed<<std::setprecision(2)<<outcome<<'\n';
    delete []target;
}

void Finance::ReportFinance(User &UserManage) {
    //TODO 补全财务报告部分
    if(UserManage.LogStack.empty()) {
        throw InvalidExpression();
    }
    if(UserManage.LogStack.back().level != 7) {
        throw InvalidExpression();
    }
    std::cout<<"------Welcome to JaneZ's Bookstore------"<<std::endl;
    std::cout<<"------Here is the Finance Report  ------"<<std::endl;
    std::cout<<"The Number of Transaction is "<< FinanceCount()<<std::endl;
    auto *Info = new FinanceInfo[FinanceCount()];
    FinanceReport.read(Info[0] , 4 , FinanceCount());
    for(int i = 0 ; i < FinanceCount() ; i ++) {
        std::cout<<i+1 << ". ";
        if(Info[i].state) {
            std::cout<< "INCOME: "<<std::fixed<<std::setprecision(2)<<Info[i].money<<std::endl;
            std::cout<< "By： "<<Info[i].UserID<<std::endl;
        }else {
            std::cout<<"Expense: "<<std::fixed<<std::setprecision(2)<<Info[i].money<<std::endl;
            std::cout<< "By： "<<Info[i].UserID<<std::endl;
        }
    }
    delete []Info;
}
