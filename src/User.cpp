#include "../include/User.h"
#include <cstring>
//
// Created by zhuyi on 2024/12/20.
//

//重载运算符 用于块状链表
bool UserInfo::operator==(const UserInfo &other) const {
    if(level == other.level && strcmp(UserName  ,other.UserName) == 0 && strcmp(Password  ,other.Password) == 0) {
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
    if(level == other.level && strcmp(UserName  ,other.UserName)  == 0 && strcmp(Password  ,other.Password) == 0) {
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
    if(level == other.level && strcmp(UserName  ,other.UserName) == 0 && strcmp(Password  ,other.Password) == 0) {
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


User::User():UserBlock("User1.txt", "User2.txt"){
    const char* username = "JaneZ";
    const char* password = "sjtu";
    const char* userid = "root";
    const int userlevel = 7;
    UserInfo owner(userid , username , password , userlevel);
    UserBlock.insert(userid,owner);
    //插入店主
}

User::~User(){
    UserBlock.ELIMINATE();
}

//登录账户操作
void User::su(const char *User_ID, const char *password, Blog &blog) {
    std::vector<UserInfo> result = UserBlock.find(User_ID);
    if(result.empty()) {
        throw InvalidExpression();
    }
    UserInfo suUser = result[0];
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

    //更新登录栈
    LogStack.push_back(suUser);

    Information content(User_ID , "su" , suUser.level);
    blog.WriteBlog(content);
}

//注销账户 logout
void User::logout(Book &book , Blog &blog) {
    if(LogStack.empty()) {
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
        if(newUser.selected) {
            std::vector<BookInfo> nextone = book.Book_ISBN.find(newUser.selectedISBN);
            if(!nextone.empty()){
                book.selected = nextone[0];
            }
        }
    }

    Information content(currentLogouter.UserID ,"logout" , currentLogouter.level);
}

void User::Register(const char *User_ID, const char *password, const char *user_name, Blog &blog) {
    //新创建账户
    UserInfo newUser(User_ID,user_name , password , 1);
    bool flag = UserBlock.insert(User_ID , newUser);
    if(!flag) {
        throw InvalidExpression();
    }
    Information content(User_ID , "register" , 0);
    blog.WriteBlog(content);
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
    Information content(LogStack.back().UserID , "revise password" , currentlevel);
    blog.WriteBlog(content);
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

    Information content(LogStack.back().UserID , "add user" , currentlevel);
    blog.WriteBlog(content);
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
    Information content(LogStack.back().UserID , "delete" , 7);
    blog.WriteBlog(content);
}

void User::Eliminate() {
    Log_Map.clear();
}
