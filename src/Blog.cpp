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

}
