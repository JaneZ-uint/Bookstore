//
// Created by zhuyi on 2024/12/20.
//

#ifndef USER_H
#define USER_H
#include <iostream>
#include "Block.h"
#include "Blog.h"
#include "Book.h"
#include "Error.h"
#include "Finance.h"
#include "Tokenscanner.h"
#include <cstring>
#include <string>

//记录用户信息
class UserInfo {
    friend class User;
    friend class Block<500,71 , 71>;
    friend class Book;
    friend class BookInfo;
    friend class Finance;
    friend class Blog;
private:
    char UserID[71] = {'\0'}; //ID
    char UserName[71] = {'\0'}; //用户名
    char Password[71] = {'\0'};  //密码
    char SpecialInfo[71] = {'\0'}; //UserID对映的各种个人信息
    int level  = 0;  //权限等级

public:
    UserInfo() = default;
    //构造函数
    UserInfo(const char* User_ID ,const char* User_name , const char* password , int LEVEL ) {
        std::strncpy(UserID , User_ID , 71);
        std::strncpy(UserName , User_name , 71);
        std::strncpy(Password , password , 71);
        level = LEVEL;
    }
};


//实现账户的多个操作
class User {
    friend class Blog;
    friend class Book;
    friend class Finance;
private:
    int currentlevel = 0; // 当前用户权限
    Block<500 , 71 , 71> UserBlock; //存储当前系统内所有账户信息

public:

    //维护登录栈
    std::vector<UserInfo> LogStack;
    User();
    ~User();

    //涉及到账户有关操作的成员函数声明
    //登录账户
    void su(const char* User_ID , const char* password , Blog& blog);

    //注销账户
    void logout(Blog& blog );

    //注册账户
    void Register(const char* User_ID , const char* password , const char* user_name , Blog& blog);

    //修改密码
    void RevisePassword(const char* User_ID , const char* currentPassword, const char* newPassword , Blog& blog , const char* command);

    //创建账户
    void UserAdd(const char* User_ID, const char* password , const int LEVEL , const char* user_name , Blog& blog, const char* command);

    //删除账户
    void Delete(const char* User_ID , Blog& blog);

};

#endif //USER_H
