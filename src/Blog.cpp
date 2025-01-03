//
// Created by zhuyi on 2024/12/20.
//
#include "../include/Blog.h"

Blog::Blog() {
    blog.initialise("blogFile.txt");
}

int Blog::BlogCount() {
    int total;
    blog.get_info(total , 1);
    return total;
}

void Blog::ReadWorker(User &UserManage) {
    if(UserManage.LogStack.empty()) {
        //登录栈为空时
        throw InvalidExpression();
    }
    //核查权限问题
    if(UserManage.LogStack.back().level != 7) {
        throw InvalidExpression();
    }

    std::cout<<"------Welcome to JaneZ's Bookstore------"<<std::endl;
    std::cout<<"------Here is the Employee Report ------"<<std::endl;
    auto* Info = new Information[BlogCount()]; // 动态分配数组
    blog.read(Info[0], 4, BlogCount()); // 从文件偏移量4开始读取total条记录
    for(int i = 0 ; i < BlogCount() ; i ++) {
        std::cout<<i+1 <<". ";
        std::cout<<Info[i].UserID<<" "<<Info[i].behave<<std::endl;
    }
    delete []Info;
}

void Blog::ReadAll(Finance & FinanceManage,User &UserManage) {
    if(UserManage.LogStack.empty()) {
        throw InvalidExpression();
    }
    if(UserManage.LogStack.back().level != 7) {
        throw InvalidExpression();
    }

    std::cout<<"------Here is the Log Report      ------"<<std::endl;
    std::cout<<"------First Part: Finance Report  ------"<<std::endl;
    FinanceManage.ReportFinance(UserManage);
    std::cout<<"------Second Part: Employee Report------"<<std::endl;
    ReadWorker(UserManage);
}
