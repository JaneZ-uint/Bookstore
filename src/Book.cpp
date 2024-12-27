//
// Created by zhuyi on 2024/12/20.
//
#include "../include/Book.h"
#include <iomanip>
#include <ranges>

//重载运算符
bool BookInfo::operator==(const BookInfo &other) const {
    if(strcmp(ISBN , other.ISBN) == 0) {
        return true;
    }
    return false;
}

bool BookInfo::operator<(const BookInfo &other) const {
    if(strcmp(ISBN , other.ISBN) < 0) {
        return true;
    }
    return false;
}

bool BookInfo::operator<=(const BookInfo &other) const {
    if(strcmp(ISBN , other.ISBN) == 0) {
        return true;
    }
    if(strcmp(ISBN , other.ISBN) < 0) {
        return true;
    }
    return false;
}

bool BookInfo::operator>(const BookInfo &other) const {
    if(strcmp(ISBN , other.ISBN) > 0) {
        return true;
    }
    return false;
}

bool BookInfo::operator>=(const BookInfo &other) const {
    if(strcmp(ISBN , other.ISBN) == 0) {
        return true;
    }
    if(strcmp(ISBN , other.ISBN) > 0) {
        return true;
    }
    return false;
}

void Book::showInfo(const char *isbn, const char *bookname, const char *Author, const char *singlekeyword, User &UserManage, Blog &blog) {
    if(UserManage.LogStack.empty()) {
        throw InvalidExpression();
    }
    //找到当前操作的人
    UserInfo nowUser = UserManage.LogStack.back();
    if(nowUser.level == 0) {
        throw InvalidExpression();
    }
    if(isbn != nullptr) {
        std::vector<BookInfo> result = Book_ISBN.find(isbn);
        if(result.empty()) {
            std::cout<<'\n';
            return;
        }
        for(auto it : result) {
            std::cout<<it.ISBN<<'\t'<<it.BookName<<'\t'<<it.Author<<'\t'<<it.KeyWord<<'\t'<<it.Price<<'\t'<<it.Quantity<<'\n';
        }
        return;
    }
    if(bookname != nullptr) {
        std::vector<BookInfo> result = Book_Name.find(bookname);
        if(result.empty()) {
            std::cout<<'\n';
            return;
        }
        for(auto it : result) {
            std::cout<<it.ISBN<<'\t'<<it.BookName<<'\t'<<it.Author<<'\t'<<it.KeyWord<<'\t'<<it.Price<<'\t'<<it.Quantity<<'\n';
        }
        return;
    }
    if(Author != nullptr) {
        std::vector<BookInfo> result = Book_Author.find(Author);
        if(result.empty()) {
            std::cout<<'\n';
            return ;
        }
        for(auto it : result) {
            std::cout<<it.ISBN<<'\t'<<it.BookName<<'\t'<<it.Author<<'\t'<<it.KeyWord<<'\t'<<it.Price<<'\t'<<it.Quantity<<'\n';
        }
        return;
    }
    if(singlekeyword != nullptr) {
        std::vector<BookInfo> result = Book_keyword.find(singlekeyword);
        if(result.empty()) {
            std::cout<<'\n';
            return;
        }
        for(auto it : result) {
            std::cout<<it.ISBN<<'\t'<<it.BookName<<'\t'<<it.Author<<'\t'<<it.KeyWord<<'\t'<<it.Price<<'\t'<<it.Quantity<<'\n';
        }
        return;
    }

    //TODO 完成日志记录
}

void Book::showeverything(User &UserManage, Blog &blog) {
    if(UserManage.LogStack.empty()) {
        throw InvalidExpression();
    }
    UserInfo nowUser = UserManage.LogStack.back();
    if(nowUser.level == 0) {
        throw InvalidExpression();
    }
    std::vector<BookInfo> result = Book_ISBN.find_all();
    for(auto &it: result) {
        std::cout<<it.ISBN<<'\t'<<it.BookName<<'\t'<<it.Author<<'\t'<<it.KeyWord<<'\t'<<it.Price<<'\t'<<it.Quantity<<'\n';
    }

    //TODO 完成日志记录
}

void Book::Shopping(const char *isbn, const int QUANT, User &UserManage, Finance &money, Blog &blog) {
    if(UserManage.LogStack.empty()) {
        throw InvalidExpression();
    }
    UserInfo nowUser = UserManage.LogStack.back();
    if(nowUser.level == 0) {
        throw InvalidExpression();
    }
    std::vector<BookInfo> result = Book_ISBN.find(isbn);
    BookInfo Purchase = result[0];
    if(result.empty()) {
        throw InvalidExpression();
    }
    if(QUANT <= 0) {
        throw InvalidExpression();
    }
    if(QUANT > Purchase.Quantity) {
        throw InvalidExpression();
    }
    //四个块状链表都删除这个块进行信息更新
    Book_ISBN.remove(isbn , Purchase);
    if(strcmp(Purchase.BookName , "\0") != 0) {
        //说明当前isbn号对应的书存在书名
        Book_Name.remove(Purchase.BookName , Purchase);
    }
    if(strcmp(Purchase.Author , "'\0") != 0) {
        Book_Author.remove(Purchase.Author, Purchase);
    }
    if(strcmp(Purchase.KeyWord , "'\0") != 0) {
        Book_keyword.remove(Purchase.KeyWord, Purchase);
    }
    Purchase.Quantity -= QUANT; //更新库存数量
    double PRICE = Purchase.Price ;
    double COST = PRICE * QUANT ;
    //更新总交易系统
    int total;
    total = money.FinanceCount();
    total ++;
    money.FinanceReport.write_info(total , 1);
    FinanceInfo INFORMATION(PRICE , Purchase.Importprice , QUANT , true, nowUser.UserID,isbn ,COST);
    money.FinanceReport.write(INFORMATION , 4 + (total - 1)*sizeof(FinanceInfo));

    //——————————————————
    std::cout<<std::fixed<<std::setprecision(2)<<COST<<'\n';
    //更新块
    Book_ISBN.insert(isbn , Purchase);
    if(strcmp(Purchase.BookName , "\0") != 0) {
        //说明当前isbn号对应的书存在书名
        Book_Name.insert(Purchase.BookName , Purchase);
    }
    if(strcmp(Purchase.Author , "'\0") != 0) {
        Book_Author.insert(Purchase.Author , Purchase);
    }
    if(strcmp(Purchase.KeyWord , "'\0") != 0) {
        Book_keyword.insert(isbn , Purchase);
    }

    //TODO 完成日志记录
}

void Book::Select_Book(const char *isbn, User &UserManage, Blog &blog , const char* command) {
    if(UserManage.LogStack.empty()) {
        throw InvalidExpression();
    }
    UserInfo nowUser = UserManage.LogStack.back();
    if(nowUser.level < 3) {
        throw InvalidExpression();
    }
    UserManage.LogStack.pop_back();
    std::vector<BookInfo> result = Book_ISBN.find(isbn);
    if(result.empty()) {
        //如果不存在这个isbn 则创建仅拥有isbn信息的新图书
        BookInfo info1(isbn);
        //写入块状链表中
        Book_ISBN.insert(isbn ,info1);
        //注意：只写isbn号就够了
        selected = info1;
    }else {
        selected = result[0];
    }
    nowUser.selected = true; //说明当前用户选择了图书
    strcpy(nowUser.selectedISBN , isbn);
    UserManage.LogStack.push_back(nowUser);
    //TODO 完成日志记录
}


//切割多个关键词情形
std::vector<std::string> Book::SplitKeywords(const char *allkeyword) {
    std::vector<std::string> result;
    std::string s;
    for(int i = 0 ; i <= strlen(allkeyword); i++) {
        if(allkeyword[i] == '|' || i == strlen(allkeyword)) {
            for(auto &it : result) {
                if(s == it) {
                    throw InvalidExpression();
                }
            }
            Tokenscanner::checkKeyWordSingle(s);
            result.push_back(s);
            s = "";
            continue;
        }
        s += allkeyword[i];
    }
    return result;
}


void Book::modify(const char *isbn, const char *bookname,const char *Author,const char *all_keywords,double price, User &UserManage, Blog &blog, const char *command) {
    //传入参数时， 直接把不要修改的部分赋成空指针
    //对于修改这一操作 需要把4个文件里的BookInfo全部修改
    if(UserManage.LogStack.empty()) {
        throw InvalidExpression();
    }
    UserInfo nowUser = UserManage.LogStack.back();
    if(nowUser.level < 3) {
        throw InvalidExpression();
    }
    if(!nowUser.selected) {
        throw InvalidExpression();
    }
    //先统一更新选中图书的信息 重新建一个ModifiedBook
    //或许我还需要在ISBN->bookInfo中找到对应的BookINFO
    //满足条件的只有一个
    std::vector<BookInfo> result = Book_ISBN.find(nowUser.selectedISBN);
    BookInfo MODIFIED = result[0];
    Book_ISBN.remove(nowUser.selectedISBN , result[0]);
    //如果存在相应的只写信息 就在相应的文件中删除
    //等等 这样真的合理吗 好像不是很合理啊 我似乎应该把修改了什么信息放在函数的形式参数中
    //但不管我又没有修改名字作者或是关键词 我都需要先把他们从块状链表中移除
    if(strcmp(result[0].BookName , "\0") != 0) {
        Book_Name.remove(result[0].BookName, result[0]);
    }
    if(strcmp(result[0].Author , "\0") != 0) {
        Book_Author.remove(result[0].Author , result[0]);
    }
    //关键词删除先放后面
    if(strcmp(isbn , "nullptr") != 0) {
        //说明会对isbn号进行修改
        if(strcmp(isbn , nowUser.selectedISBN) == 0) {
            throw InvalidExpression();
        }
        //修改isbn号
        strcpy(MODIFIED.ISBN , isbn);
    }
    if(strcmp(bookname , "nullptr") != 0) {
        //对书名进行修改
        strcpy(MODIFIED.BookName , bookname);
    }
    if(strcmp(Author , "nullptr") != 0) {
        //对作者进行修改
        strcpy(MODIFIED.Author , Author);
    }
    if(price != -1) {
        MODIFIED.Price = price;
    }
    if(strcmp(all_keywords ,  "nullptr") != 0) {
        //需要判断keyword是否包含重复信息段
        //修改后的所有关键词
        std::vector<std::string> Result = SplitKeywords(all_keywords);
        if(strcmp(MODIFIED.KeyWord , "\0") == 0) {
            strcpy(MODIFIED.KeyWord , all_keywords); //更新MODIFIED
            for(auto &it:Result) {
                char a[71] = {'\0'};
                strcpy(a , it.c_str());
                Book_keyword.insert(a , MODIFIED);
            }
        }else {
            //修改前的关键词
            std::vector<std::string> selectedKEYWORD = SplitKeywords(MODIFIED.KeyWord);
            for(auto &it : selectedKEYWORD) {
                //先从string转化为char数组
                char a[71] = {'\0'};
                strcpy(a, it.c_str());
                Book_keyword.remove(a, result[0]);
            }
            //现在再更新
            strcpy(MODIFIED.KeyWord , all_keywords);
            for(auto &it: Result) {
                char a[71] = {'\0'};
                strcpy(a , it.c_str());
                Book_keyword.insert(a , MODIFIED);
            }
        }
    }
    Book_ISBN.insert(MODIFIED.ISBN , MODIFIED);
    if(strcmp(MODIFIED.BookName , "\0") != 0) {
        Book_Name.insert(MODIFIED.BookName , MODIFIED);
    }
    if(strcmp(MODIFIED.Author , "\0") != 0) {
        Book_Author.insert(MODIFIED.Author , MODIFIED);
    }

    //TODO 完成日志部分记录
}

void Book::ImportBook(const int QUANT, double COST, User &UserManage, Finance &money, Blog &blog, const char *command) {
    if(UserManage.LogStack.empty()) {
        throw InvalidExpression();
    }
    UserInfo nowUser = UserManage.LogStack.back();
    BookInfo tmp  = selected;
    if(nowUser.level < 3) {
        throw InvalidExpression();
    }
    if(!nowUser.selected) {
        throw InvalidExpression();
    }
    if(QUANT <= 0) {
        throw InvalidExpression();
    }
    if(COST <= 0) {
        throw InvalidExpression();
    }
    selected.Quantity += QUANT;
    double cost = COST / QUANT;

    FinanceInfo INFORMATION(selected.Price , cost , QUANT , false ,nowUser.UserID ,nowUser.selectedISBN,COST);
    int totalCount ;
    totalCount++;
    money.FinanceReport.write_info(totalCount , 1);
    money.FinanceReport.write(INFORMATION,sizeof(FinanceInfo)*(totalCount - 1) + 4);
    selected.Importprice = cost;
    Book_ISBN.remove(selected.ISBN , tmp);
    Book_ISBN.insert(selected.ISBN, selected);
    if(strcmp(selected.BookName , "\0") != 0) {
        Book_Name.remove(selected.ISBN , tmp);
        Book_Name.insert(selected.ISBN , selected);
    }
    if(strcmp(selected.Author , "\0") != 0) {
        Book_Name.remove(selected.Author , tmp);
        Book_Name.insert(selected.Author , selected);
    }
    if(strcmp(selected.KeyWord, "\0") != 0) {
        Book_Name.remove(selected.KeyWord , tmp);
        Book_Name.insert(selected.KeyWord , selected);
    }

    //TODO 完成日志记录
}
