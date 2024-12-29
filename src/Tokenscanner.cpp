//
// Created by zhuyi on 2024/12/20.
//
#include "../include/Tokenscanner.h"

void Tokenscanner::TokenSeparator(const std::string input, std::vector<std::string> &result) {
    std::string a;
    for(int i = 0 ; i <= input.size() ; i++) {
        if(i == input.size() || input[i] == ' ') {
            //以空格作为分割单位
            if(!a.empty()) {
                result.push_back(a);
                a = "";
                continue;
            }
        }else {
            a+= input[i];
        }
    }
}

//对于购买数量Quantity 交易笔数
//最大长度: 10位 正整数
int Tokenscanner::StringToIntegerQuant(const std::string &s) {
    if(s.empty() || s.size() > 10) {
        throw InvalidExpression();
    }
    for(int i = 0 ; i < s.size() ; i++) {
        if(s[i] >= '0' && s[i] <= '9') {
            continue;
        }else {
            throw InvalidExpression();
        }
    }
    long long int num = std::stoi(s);
    if(num == 0) {
        throw InvalidExpression();
    }else if(num >  2147483647) {
        throw InvalidExpression();
    }else {
        int res = std::stoi(s);
        return res;
    }
}

//交易总额 Count
//最大长度 : 10位 可以为0
int Tokenscanner::StringToIntegerCount(const std::string &s) {
    if(s.empty() ||s.size() > 10) {
        throw InvalidExpression();
    }
    for(int i = 0 ; i < s.size() ; i++) {
        if(s[i] >= '0' && s[i] <= '9') {
            continue;
        }else {
            throw InvalidExpression();
        }
    }
    long long int num = std::stoi(s);
   if(num >  2147483647) {
        throw InvalidExpression();
   }else {
       int res = std::stoi(s);
       return res;
   }

}

//对于图书单价Price 交易总额TotalCost 最大长度: 13位
//精度固定为小数点后2位
double Tokenscanner::StringToDouble(const std::string &s) {
    if(s.empty() || s.size() > 13) {
        throw InvalidExpression();
    }
    for(int i = 0 ; i < s.size() ; i ++) {
        if((s[i] >= '0' && s[i] <= '9')|| s[i] == '.') {
            continue;
        }
        throw InvalidExpression();
    }
    double res = std::stod(s);
    return res;
}

//检查UserID Password CurrentPassword NewPassword字符集是否合法
//数字，字母，下划线；
//30
void Tokenscanner::checkIDPassword(const std::string &s) {
    if(s.empty() || s.size() > 30) {
        throw InvalidExpression();
    }
    for(int i  = 0 ; i < s.size() ; i ++) {
        if(s[i] >= '0' && s[i] <= '9') {
            continue;
        }else if(s[i] >= 'a'&& s[i] <= 'z') {
            continue;
        }else if(s[i] >= 'A'&& s[i] <= 'Z') {
            continue;
        }else if(s[i] == '_') {
            continue;
        }
        throw InvalidExpression();
    }
}

//检查Username
//除不可见字符以外 ASCII 字符；
//30
void Tokenscanner::checkUsername(const std::string &s) {
    if(s.empty() ||s.size() > 30) {
        throw InvalidExpression();
    }
    for(int i = 0 ; i < s.size() ; i++) {
        if(s[i] == '\x01' || s[i] == '\x02' || s[i] == '\x03' || s[i] == '\x04' || s[i] == '\x05' || s[i] == '\x06' || s[i] == '\a' || s[i] == '\b' || s[i] == '\t' || s[i] == '\n' || s[i] == '\v' || s[i] == '\f' || s[i] == '\r' ) {
            throw InvalidExpression();
        }
    }
}

//检查Privilege
//数字
//只允许出现 0 1 3 7
void Tokenscanner::checkPrivilege(const std::string &s) {
    if(s.size() > 1) {
        throw InvalidExpression();
    }
    if(s[0] != '0' && s[0] != '1' && s[0] != '3' && s[0] != '7') {
        throw InvalidExpression();
    }
}

//检查ISBN
//除不可见字符以外 ASCII 字符；
// 20
void Tokenscanner::checkISBN(const std::string &s) {
    if(s.size() > 20) {
        throw InvalidExpression();
    }
    for(int i = 0 ; i < s.size() ; i++) {
        if(s[i] == '\x01' || s[i] == '\x02' || s[i] == '\x03' || s[i] == '\x04' || s[i] == '\x05' || s[i] == '\x06' || s[i] == '\a' || s[i] == '\b' || s[i] == '\t' || s[i] == '\n' || s[i] == '\v' || s[i] == '\f' || s[i] == '\r' ) {
            throw InvalidExpression();
        }
    }
}

//检查Bookname Author
//除不可见字符和英文双引号以外 ASCII 字符
// 60
void Tokenscanner::checkBooknameAuthor(const std::string &s) {
    if(s.size() > 60) {
        throw InvalidExpression();
    }
    for(int i = 0 ; i < s.size() ; i++) {
        if(s[i] == '"') {
            throw InvalidExpression();
        }else if(s[i] == '\x01' || s[i] == '\x02' || s[i] == '\x03' || s[i] == '\x04' || s[i] == '\x05' || s[i] == '\x06' || s[i] == '\a' || s[i] == '\b' || s[i] == '\t' || s[i] == '\n' || s[i] == '\v' || s[i] == '\f' || s[i] == '\r' ) {
            throw InvalidExpression();
        }
    }
}

//检查Keyword
//除不可见字符和英文双引号以外 ASCII 字符；
// 60
//[keyword] 内容以 | 为分隔可以出现多段信息。
//如 math|magic|quantum 表示该图书具有三个无顺序的关键词
//math, magic 和 quantum，每段信息长度至少为 1。
void Tokenscanner::checkKeyWordSingle(const std::string &s) {
    if(s.size() > 60) {
        throw InvalidExpression();
    }
    for(int i = 0 ; i < s.size() ; i ++) {
        if(s[i] == '"') {
            throw InvalidExpression();
        }else if(s[i] == '|') {
            throw InvalidExpression();
        }else if(s[i] == '\x01' || s[i] == '\x02' || s[i] == '\x03' || s[i] == '\x04' || s[i] == '\x05' || s[i] == '\x06' || s[i] == '\a' || s[i] == '\b' || s[i] == '\t' || s[i] == '\n' || s[i] == '\v' || s[i] == '\f' || s[i] == '\r' ) {
            throw InvalidExpression();
        }
    }
}

//检查show指令的情况
//对show指令进行切片
//附加参数内容为空则操作失败；
//无附加参数时，所有图书均满足要求；
//[Keyword] 中出现多个关键词则操作失败。
std::string Tokenscanner::cutShow(const std::string &s) {
    std::string res;
    if(s.empty()) {
        res = "";
        return res;
    }
    if(s[0] != '-') {
        throw InvalidExpression();
    }
    //图书信息
    if(s.size() < 7) {  //附加参数为空的情况
        throw InvalidExpression();//显然不会满足
    }
    std::string whatinfo1 = s.substr(1, 4);
    if(whatinfo1 == "ISBN") {
        if(s[5] != '=') {
            throw InvalidExpression();
        }
        std::string s1 = s.substr(6 , s.size() - 6);
        checkISBN(s1);
        res = s1;
        return res;
    }
    if(s.size() == 7 || s.size() == 8) {
        throw InvalidExpression();
    }//到这个位置还没有被return显然已经无法满足剩下3个变量的判断了
    if(whatinfo1 == "name") {
        if(s[5] != '=') {
            throw InvalidExpression();
        }
        if(s[6] != '"') {
            throw InvalidExpression();
        }
        if(s[s.size() - 1] != '"') {
            throw InvalidExpression();
        }
        std::string s1 = s.substr(7 , s.size() - 8);
        checkBooknameAuthor(s1);
        res = s1;
        return res;
    }
    if(s.size() == 9 || s.size() == 10) {
        throw InvalidExpression();
    }
    //接下来剩下的字符串长度都大于10
    std::string whatinfo2 = s.substr(1, 6);
    if(whatinfo2 == "author") {
        if(s[7] != '=') {
            throw InvalidExpression();
        }
        if(s[8] != '"') {
            throw InvalidExpression();
        }
        if(s[s.size() - 1] != '"') {
            throw InvalidExpression();
        }
        std::string s1 = s.substr(9 , s.size() - 10);
        checkBooknameAuthor(s1);
        res = s1;
        return res;
    }
    if(s.size() == 11) {
        throw InvalidExpression();
    }
    std::string whatinfo3 = s.substr(1 , 7);
    if(whatinfo3 == "keyword") {
        if(s[8] != '=') {
            throw InvalidExpression();
        }
        if(s[9] != '"') {
            throw InvalidExpression();
        }
        if(s[s.size() - 1] != '"') {
            throw InvalidExpression();
        }
        std::string s1 = s.substr(10 , s.size() - 11);
        checkKeyWordSingle(s1);
        res = s1;
        return s1;
    }
    throw InvalidExpression();
}

//检查modify 指令的情况
//附加参数内容为空则操作失败；
//[keyword] 包含重复信息段则操作失败。
//有重复附加参数为非法指令；
std::string Tokenscanner::cutModify(const std::string &s) {
    std::string res;
    if(s.empty()) {
        res = "";
        return res;
    }
    if(s[0] != '-') {
        throw InvalidExpression();
    }
    //图书信息
    if(s.size() < 7) {  //附加参数为空的情况
        throw InvalidExpression();//显然不会满足
    }
    std::string whatinfo1 = s.substr(1, 4);
    if(whatinfo1 == "ISBN") {
        if(s[5] != '=') {
            throw InvalidExpression();
        }
        std::string s1 = s.substr(6 , s.size() - 6);
        checkISBN(s1);
        res = s1;
        return res;
    }
    if(s.size() == 7 ) {
        throw InvalidExpression();
    }//到这个位置还没有被return显然已经无法满足剩下3个变量的判断了
    std::string whatinfo4 = s.substr(1 ,5);
    if(whatinfo4 == "price") {
        if(s[6] != '=') {
            throw InvalidExpression();
        }
        std::string s1 = s.substr(7 , s.size() - 7);
        return s1;
    }
    if(s.size() == 8 ) {
        throw InvalidExpression();
    }
    if(whatinfo1 == "name" ) {
        if(s[5] != '=') {
            throw InvalidExpression();
        }
        if(s[6] != '"') {
            throw InvalidExpression();
        }
        if(s[s.size() - 1] != '"') {
            throw InvalidExpression();
        }
        std::string s1 = s.substr(7 , s.size() - 8);
        checkBooknameAuthor(s1);
        res = s1;
        return res;
    }
    if(s.size() == 9 || s.size() == 10) {
        throw InvalidExpression();
    }
    //接下来剩下的字符串长度都大于10
    std::string whatinfo2 = s.substr(1, 6);
    if(whatinfo2 == "author") {
        if(s[7] != '=') {
            throw InvalidExpression();
        }
        if(s[8] != '"') {
            throw InvalidExpression();
        }
        if(s[s.size() - 1] != '"') {
            throw InvalidExpression();
        }
        std::string s1 = s.substr(9 , s.size() - 10);
        checkBooknameAuthor(s1);
        res = s1;
        return res;
    }
    if(s.size() == 11) {
        throw InvalidExpression();
    }
    std::string whatinfo3 = s.substr(1 , 7);
    if(whatinfo3 == "keyword") {
        if(s[8] != '=') {
            throw InvalidExpression();
        }
        if(s[9] != '"') {
            throw InvalidExpression();
        }
        if(s[s.size() - 1] != '"') {
            throw InvalidExpression();
        }
        std::string s1 = s.substr(10 , s.size() - 11);
        checkKeywordAll(s1);
        res = s1;
        return s1;
    }
    throw InvalidExpression();
}

void Tokenscanner::checkKeywordAll(const std::string &s) {
    if(s.empty() ||s.size() > 60) {
        throw InvalidExpression();
    }
}

