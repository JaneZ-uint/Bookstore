#include "../include/User.h"
#include <cstring>
//
// Created by zhuyi on 2024/12/20.
//

//重载运算符 用于块状链表
bool UserInfo::operator==(const UserInfo &other) const {
    if(level == other.level && strcmp(UserName  ,other.UserName) && strcmp(Password  ,other.Password)) {
        return true;
    }
    return false;
}

bool UserInfo::operator<(const UserInfo &other) const {
    if(level < other.level) {
        return true;
    }else if(level > other.level) {
        return false;
    }else {
        if(strcmp(UserName , other.UserName) < 0) {
            return true;
        }else if(strcmp(UserName , other.UserName) > 0) {
            return false;
        }else {
            if(strcmp(Password , other.Password) < 0) {
                return true;
            }else {
                return false;
            }
        }
    }
}

bool UserInfo::operator<=(const UserInfo &other) const {
    if(level == other.level && strcmp(UserName  ,other.UserName) && strcmp(Password  ,other.Password)) {
        return true;
    }
    if(level < other.level) {
        return true;
    }else if(level > other.level) {
        return false;
    }else {
        if(strcmp(UserName , other.UserName) < 0) {
            return true;
        }else if(strcmp(UserName , other.UserName) > 0) {
            return false;
        }else {
            if(strcmp(Password , other.Password) < 0) {
                return true;
            }else {
                return false;
            }
        }
    }
}

bool UserInfo::operator>(const UserInfo &other) const {
    if(level > other.level) {
        return true;
    }else if(level < other.level) {
        return false;
    }else {
        if(strcmp(UserName , other.UserName) > 0) {
            return true;
        }else if(strcmp(UserName , other.UserName) < 0) {
            return false;
        }else {
            if(strcmp(Password , other.Password) > 0) {
                return true;
            }else {
                return false;
            }
        }
    }
}

bool UserInfo::operator>=(const UserInfo &other) const {
    if(level == other.level && strcmp(UserName  ,other.UserName) && strcmp(Password  ,other.Password)) {
        return true;
    }

    if(level > other.level) {
        return true;
    }else if(level < other.level) {
        return false;
    }else {
        if(strcmp(UserName , other.UserName) > 0) {
            return true;
        }else if(strcmp(UserName , other.UserName) < 0) {
            return false;
        }else {
            if(strcmp(Password , other.Password) > 0) {
                return true;
            }else {
                return false;
            }
        }
    }
}


User::User():UserBlock("User1", "User2"){
    const char* username = "JaneZ";
    const char* password = "20241224";
    const char* userid = "root";
    const int userlevel = 7;
    UserInfo owner(userid , username , password , userlevel);
    UserBlock.insert(userid,owner);
    //插入店主
}

User::~User() = default;

//登录账户操作
void User::su(const char *User_ID, const char *password, Blog &blog) {
    std::vector<UserInfo> result = UserBlock.find(User_ID);
    if(result.empty()) {
        throw InvalidExpression();
    }
    UserInfo suUser = result[0];
    UserBlock.remove(User_ID , suUser);
    if(password == nullptr && currentlevel <= suUser.level) {
        throw InvalidExpression();
    }
    if(password != nullptr && strcmp(password , suUser.Password) != 0) {
        throw InvalidExpression();
    }
    //更新当前账户系统的用户
    currentlevel = suUser.level;

    std::string ID;
    ID.assign(User_ID);
    //当前账户处于登录状态
    Log_Map[ID]++;

    //处理选择情况
    suUser.selected = false;
    char isbn[71] = {'\0'};
    strcpy(suUser.selectedISBN , isbn);

    //更新登录栈
    LogStack.push_back(suUser);

    //更新块状链表
    UserBlock.insert(User_ID , suUser);

    //TODO 写入日志部分
}

//注销账户 logout
void User::logout(Book &book , Blog &blog) {
    if(LogStack.empty()) {
        throw InvalidExpression();
    }
    //游客状态不存在
    if(currentlevel == 0) {
        throw InvalidExpression();
    }
    //从登录栈中取出最后一个元素
    UserInfo currentLogouter = LogStack.back();
    std::string ID;
    ID.assign(currentLogouter.UserID);
    Log_Map[ID]--;
    LogStack.pop_back();

    if(LogStack.empty()) {
        currentlevel = 0;

        //当前选中图书的置空
        BookInfo cur;
        book.selected = cur;
    }else {
        //更新当前账户
        UserInfo newUser = LogStack.back();
        currentlevel = newUser.level;

        //TODO 补充选择图书相关内容
    }

    //TODO 完成日志部分
}

void User::Register(const char *User_ID, const char *password, const char *user_name, Blog &blog) {
    //新创建账户
    UserInfo newUser(User_ID,user_name , password , 1);
    if(!UserBlock.insert(User_ID , newUser)) {
        throw InvalidExpression();
    }
    //TODO 完成日志部分
}

void User::RevisePassword(const char *User_ID, const char *currentPassword, const char *newPassword, Blog &blog, const char *command) {
    if(currentlevel == 0) {
        throw InvalidExpression();
    }
    if(currentPassword == nullptr && currentlevel != 7) {
        throw InvalidExpression();
    }
    std::vector<UserInfo> result = UserBlock.find(User_ID);
    if(result.empty()) {
        throw InvalidExpression();
    }
    UserInfo nowUser = result[0];
    if(currentPassword != nullptr && strcmp(currentPassword , nowUser.Password) != 0) {
        throw InvalidExpression();
    }
    UserBlock.remove(User_ID , nowUser);
    strcpy(nowUser.Password , newPassword);
    UserBlock.insert(User_ID , nowUser);
    //TODO 完成日志部分
}

//创建账户
void User::UserAdd(const char *User_ID, const char *password, const int LEVEL, const char *user_name, Blog &blog, const char *command) {
    if(currentlevel < 3) {
        throw InvalidExpression();
    }
    if(LEVEL >= currentlevel) {
        throw InvalidExpression();
    }
    if(LEVEL == 7) {
        throw InvalidExpression();
    }
    UserInfo addUser(User_ID , user_name , password , LEVEL);
    std::vector<UserInfo> result = UserBlock.find(User_ID );
    if(!result.empty()) {
        throw InvalidExpression();
    }else {
        UserBlock.insert(User_ID , addUser);
    }

    //TODO 完成日志部分
}

void User::Delete(const char *User_ID, Blog &blog) {
    if(currentlevel < 7) {
        throw InvalidExpression();
    }
    std::vector<UserInfo> result = UserBlock.find(User_ID);
    if(result.empty()) {
        throw InvalidExpression();
    }
    std::string ID;
    ID.assign(User_ID);
    if(Log_Map[ID] > 0) {
        throw InvalidExpression();
    }
    UserBlock.remove(User_ID , result[0]);
    //TODO 完成日志部分
}
