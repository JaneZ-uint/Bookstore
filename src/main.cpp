#include <iostream>
#include "../include/Block.h"
#include "../include/Blog.h"
#include "../include/Book.h"
#include "../include/Error.h"
#include "../include/Finance.h"
#include "../include/Tokenscanner.h"
#include "../include/User.h"

bool processline(std::string line , Book &bookmanage , User &usermanage , Finance &financemanage , Blog &blogmanage);

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

bool processline(std::string line,Book &bookmanage,User &usermanage,Finance &financemanage,Blog &blogmanage) {
    std::vector<std::string> Parse;
    Tokenscanner::TokenSeparator(line , Parse); //得到指令切片
    //如果为空行，直接返回
    if(Parse.empty()) {
        return true;
    }
    std::string command1 = Parse[0];
    if(command1 == "quit" || command1 == "exit") {
        usermanage.LogStack.clear();
        usermanage.Eliminate();
        return false;
    }
    //su [UserID] ([Password])?
    if(command1 == "su") {
        if(Parse.size() == 1 && Parse.size() > 3) {
            throw InvalidExpression();
        }
        Tokenscanner::checkIDPassword(Parse[1]);
        //检查ID是否合法
        char ID[71] = {'\0'};
        strcpy(ID , Parse[1].c_str());
        if(Parse.size() == 2) {
            usermanage.su(ID , nullptr , blogmanage);
        }else if(Parse.size() == 3) {
            //需要密码的普通用户
            Tokenscanner::checkIDPassword(Parse[2]);
            char PASS[71] = {'\0'};
            strcpy(PASS , Parse[2].c_str());
            usermanage.su(ID, PASS , blogmanage);
        }else {
            throw InvalidExpression();
        }
    }else if(command1 == "logout") {
        if(Parse.size() != 1) {
            throw InvalidExpression();
        }
        usermanage.logout(bookmanage , blogmanage);
    }else if(command1 == "register") {
        //register [UserID] [Password] [Username]
        if(Parse.size() != 4) {
            throw InvalidExpression();
        }
        Tokenscanner::checkIDPassword(Parse[1]);
        Tokenscanner::checkIDPassword(Parse[2]);
        Tokenscanner::checkUsername(Parse[3]);
        char ID[71] = {'\0'};
        strcpy(ID , Parse[1].c_str());
        char PASS[71] = {'\0'};
        strcpy(PASS , Parse[2].c_str());
        char NAME[71] = {'\0'};
        strcpy(NAME , Parse[3].c_str());
        usermanage.Register(ID , PASS , NAME , blogmanage);
    }else if(command1 == "passwd") {
        //passwd [UserID] ([CurrentPassword])? [NewPassword]
        if(Parse.size() != 3 && Parse.size() != 4) {
            throw InvalidExpression();
        }
        Tokenscanner::checkIDPassword(Parse[1]);
        char ID[71] = {'\0'};
        strcpy(ID , Parse[1].c_str());
        if(Parse.size() == 3) {
            //尊贵的店长 不需要密码
            Tokenscanner::checkIDPassword(Parse[2]);
            char PASS[71] = {'\0'};
            strcpy(PASS , Parse[2].c_str());
            std::string s1 = "change the password of ";
            std::string s2 = " to ";
            std::string s = s1 + Parse[1] + s2 + Parse[2];
            char com[100] = {'\0'};
            strcpy(com , s.c_str());
            usermanage.RevisePassword(ID , nullptr ,PASS ,blogmanage , com );
        }else {
            //对应需要原密码的情况
            Tokenscanner::checkIDPassword(Parse[2]);
            char curPass[71] = {'\0'};
            strcpy(curPass , Parse[2].c_str());
            Tokenscanner::checkIDPassword(Parse[3]);
            char PASS[71] = {'\0'};
            strcpy(PASS , Parse[3].c_str());
            std::string s1 = "change the password of ";
            std::string s2 = " to ";
            std::string s = s1 + Parse[1] + s2 + Parse[3];
            char com[100] = {'\0'};
            strcpy(com , s.c_str());
            usermanage.RevisePassword(ID, curPass , PASS ,blogmanage,com);
        }
    }else if(command1 == "useradd") {
        //useradd [UserID] [Password] [Privilege] [Username]
        if(Parse.size() != 5) {
            throw InvalidExpression();
        }
        Tokenscanner::checkIDPassword(Parse[1]);
        char ID[71] = {'\0'};
        strcpy(ID , Parse[1].c_str());
        Tokenscanner::checkIDPassword(Parse[2]);
        char PASS[71] = {'\0'};
        strcpy(PASS , Parse[2].c_str());
        Tokenscanner::checkPrivilege(Parse[3]);
        int privilege = std::stoi(Parse[3]);
        Tokenscanner::checkUsername(Parse[4]);
        char NAME[71] = {'\0'};
        strcpy(NAME , Parse[4].c_str());
        char com[100] = {'\0'};
        std::string s1 = "add user ";
        std::string s = s1 + Parse[1];
        strcpy(com , s.c_str());
        usermanage.UserAdd(ID , PASS , privilege ,NAME ,blogmanage , com );
    }else if(command1 == "delete") {
        //delete [UserID]
        if(Parse.size() != 2) {
            throw InvalidExpression();
        }
        Tokenscanner::checkIDPassword(Parse[1]);
        char ID[71] = {'\0'};
        strcpy(ID , Parse[1].c_str());
        std::string s1 = "delete user ";
        std::string s = s1 + Parse[1];
        char com[100] = {'\0'};
        strcpy(com , s.c_str());
        usermanage.Delete(ID , blogmanage);
    }else if(command1 == "show") {
        //show (-ISBN=[ISBN] | -name="[BookName]" | -author="[Author]" | -keyword="[Keyword]")?
        //show finance ([Count])?
        if(Parse.size() > 3) {
            throw InvalidExpression();
        }
        if(Parse.size() > 2 ) {
            //进行show finance操作
            if(Parse[1] != "finance") {
                throw InvalidExpression();
            }
            int num = Tokenscanner::StringToIntegerCount(Parse[2]);
            financemanage.ShowFinance(num , usermanage);
        }
        if(Parse.size() == 2) {
            if(Parse[1] == "finance") {
                int total = financemanage.FinanceCount();
                financemanage.ShowFinance(total , usermanage);
            }else {
                //理论上通过cutshow的操作会得到一个正确的类型参数
                std::string result = Tokenscanner::cutShow(Parse[1]);
                if(Parse[1][1] == 'I') {
                    //说明返回的是ISBN
                    char isbn[71] = {'\0'};
                    strcpy(isbn , result.c_str());
                    bookmanage.showInfo(isbn , nullptr, nullptr , nullptr , usermanage , blogmanage );
                }else if(Parse[1][1] == 'n') {
                    //说明返回的是bookname
                    char BKname[71] = {'\0'};
                    strcpy(BKname , result.c_str());
                    bookmanage.showInfo(nullptr , BKname , nullptr , nullptr , usermanage , blogmanage);
                }else if(Parse[1][1] == 'a') {
                    char AUthor[71] = {'\0'};
                    strcpy(AUthor , result.c_str());
                    bookmanage.showInfo(nullptr , nullptr , AUthor, nullptr , usermanage , blogmanage);
                }else if(Parse[1][1] == 'k') {
                    char SINGLEkeyword[71] = {'\0'};
                    strcpy(SINGLEkeyword , result.c_str());
                    bookmanage.showInfo(nullptr , nullptr , nullptr , SINGLEkeyword, usermanage , blogmanage);
                }
            }
        }
        if(Parse.size() == 1) {
            bookmanage.showeverything(usermanage , blogmanage);
        }
    }else if(command1 == "buy") {
        if(Parse.size() != 3) {
            throw InvalidExpression();
        }
        Tokenscanner::checkISBN(Parse[1]);
        char ISBN[71] = {'\0'};
        strcpy(ISBN , Parse[1].c_str());
        int QUANT = Tokenscanner::StringToIntegerQuant(Parse[2]);
        bookmanage.Shopping(ISBN , QUANT , usermanage , financemanage ,blogmanage);
    }else if(command1 == "select") {
        if(Parse.size() != 2) {
            throw InvalidExpression();
        }
        Tokenscanner::checkISBN(Parse[1]);
        char ISBN[71] = {'\0'};
        strcpy(ISBN , Parse[1].c_str());
        std::string s1 = "select ";
        std::string s = s1 + Parse[1];
        bookmanage.Select_Book(ISBN , usermanage , blogmanage , s.c_str());
    }else if(command1 == "modify") {
        //modify (-ISBN=[ISBN] | -name="[BookName]" | -author="[Author]" | -keyword="[Keyword]" | -price=[Price])+
        if(Parse.size() > 6) {
            throw InvalidExpression();
        }
        if(Parse.size() == 1) {
            throw InvalidExpression();
        }
        bool isI = false;
        bool isn = false;
        bool isa = false;
        bool isk = false;
        bool isp = false;
        char ISBN[71] = {'\0'};
        char NAME[71] = {'\0'};
        char AUTHOR[71] = {'\0'};
        char KEYWORD[71] = {'\0'};

        double PRIce = -1;
        for(int i = 1; i < Parse.size() ; i++) {
            std::string s = Tokenscanner::cutModify(Parse[i]);
            if(Parse[i][1] == 'I') {
                if(isI) {
                    throw InvalidExpression();
                }
                isI = true;
                //可以进行修改ISBN的操作了
                strcpy(ISBN , s.c_str());
            }else if(Parse[i][1] == 'n') {
                if(isn) {
                    throw InvalidExpression();
                }
                isn = true;
                //可以进行修改name的操作了
                strcpy(NAME , s.c_str());
            }else if(Parse[i][1] == 'a') {
                if(isa) {
                    throw InvalidExpression();
                }
                isa = true;
                //可以进行修改作者名的操作了

                strcpy(AUTHOR  ,s.c_str());
            }else if(Parse[i][1] == 'k') {
                if(isk) {
                    throw InvalidExpression();
                }
                isk = true;
                //可以进行修改关键词的操作了
                strcpy(KEYWORD, s.c_str());
            }else if(Parse[i][1] == 'p') {
                if(isp) {
                    throw InvalidExpression();
                }
                isp = true;
                //可以进行修改价格的操作了
                double price = Tokenscanner::StringToDouble(s);
                PRIce = price;
            }else {
                throw InvalidExpression();
            }
        }
        if(!isI) {
            strcpy(ISBN , "nullptr");
        }
        if(!isn) {
            strcpy(NAME , "nullptr");
        }
        if(!isa) {
            strcpy(AUTHOR , "nullptr");
        }
        if(!isk) {
            strcpy(KEYWORD , "nullptr");
        }
        bookmanage.modify(ISBN , NAME, AUTHOR, KEYWORD ,PRIce , usermanage, blogmanage ,line.c_str() );
    }else if(command1 == "import") {
        if(Parse.size() != 3) {
            throw InvalidExpression();
        }
        int QUANT = Tokenscanner::StringToIntegerQuant(Parse[1]);
        double totalcost = Tokenscanner::StringToDouble(Parse[2]);
        std::string s1;
        s1.assign(usermanage.LogStack.back().selectedISBN);
        std::string s = line + s1;
        bookmanage.ImportBook(QUANT , totalcost , usermanage , financemanage , blogmanage , s.c_str());
    }else if(command1 == "report") {
        if(Parse.size() != 2) {
            throw InvalidExpression();
        }
        if(Parse[1] == "finance") {
            financemanage.ReportFinance(usermanage);
        }else if(Parse[1] == "employee") {
            blogmanage.ReadWorker(usermanage);
        }else {
            throw InvalidExpression();
        }
    }else if(command1 == "log") {
        if(Parse.size() != 1) {
            throw InvalidExpression();
        }
        blogmanage.ReadAll(financemanage , usermanage);
    }else {
        throw InvalidExpression();
    }
    return true;
}
