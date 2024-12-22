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

class User {

};


class UserInfo {
private:
    int size = 0;
    int indexnum = 0;
    char UserID[31] = {'\0'};
    char UserName[31] = {'\0'};
    char Password[31] = {'\0'};
    int level  = 0;

};

#endif //USER_H
