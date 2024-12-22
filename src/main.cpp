#include <iostream>
#include "../include/Block.h"
#include "../include/Blog.h"
#include "../include/Book.h"
#include "../include/Error.h"
#include "../include/Finance.h"
#include "../include/Tokenscanner.h"
#include "../include/User.h"

bool processline(std::string line , Book bookmanage , User usermanage , Finance financemanage , Blog blogmanage);

int main() {
    Book BookManage; //管理图书信息及相关操作的类
    User UserManage; //管理账户系统
    Finance FinanceManage; //管理财务系统
    Blog BlogManage; //管理日志系统
    std::string command;
    while(true) {
        try {
            if(!getline(std::cin , command)) {
                command = "quit";
            }
            if(!processline(command , BookManage , UserManage , FinanceManage,BlogManage)) {
                break;
            }
        }catch(InvalidExpression &error) {
            std::cout << error.what();
        }
    }

    return 0;
}

bool processline(std::string line,Book bookmanage,User usermanage,Finance financemanage,Blog blogmanage) {

}
