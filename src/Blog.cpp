//
// Created by zhuyi on 2024/12/20.
//
#include "../include/Blog.h"

Blog::Blog() {
    blog.initialise("blogFile");
    blog.clear();
}

void Blog::WriteBlog(Information& info) {
    int total;
    blog.get_info(total , 1);
    total ++;
    blog.write_info(total , 1);
    blog.write(info , 4 + (total - 1)*sizeof(Information) , 1);
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
    int total ;
    blog.get_info(total , 1);
    int num = 0 ;
    num = total / 2000 + 1;
    std::cout<<"------Welcome to JaneZ's Bookstore------"<<std::endl;
    std::cout<<"------Here is the Employee Report ------"<<std::endl;
    for(int i = 0 ; i < num ; i ++) {
        Information Info[2000];
        blog.read(Info[0] , 4 + i * 2000 * sizeof(Information) , 2000);
        int e = (i == num-1) ? total / 2000 : 2000;
        for(int i = 0 ; i < e ; i ++ ) {
            std::cout<<i + 1 <<". ";
            std::cout<<Info[i].UserID<<" "<<Info[i].behave<<std::endl;
        }
    }
}

void Blog::ReadAll(Finance & FinanceManage,User &UserManage) {
    if(UserManage.LogStack.empty()) {
        throw InvalidExpression();
    }
    if(UserManage.LogStack.back().level != 7) {
        throw InvalidExpression();
    }
    int total ;
    blog.get_info(total , 1);
    int num = 0 ;
    num = total / 2000 + 1;
    //std::cout<<"------Welcome to JaneZ's Bookstore------"<<std::endl;
    std::cout<<"------Here is the Log Report      ------"<<std::endl;
    std::cout<<"------First Part: Finance Report  ------"<<std::endl;
    FinanceManage.ReportFinance(UserManage);
    std::cout<<"------Second Part: Employee Report------"<<std::endl;
    ReadWorker(UserManage);
}
