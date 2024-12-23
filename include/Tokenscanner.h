//
// Created by zhuyi on 2024/12/20.
//

#ifndef TOKENSCANNER_H
#define TOKENSCANNER_H
#include <iostream>
#include "Block.h"
#include "Blog.h"
#include "Book.h"
#include "Error.h"
#include "Finance.h"
#include "Tokenscanner.h"
#include "User.h"
#include <string>
#include <sstream>
#include <vector>

class Tokenscanner {
//private:
    //std::string command;
    //std::vector<std::string> Tokens;
public:

    //利用TokenSeparator得到块的切片
    static void TokenSeparator(const std::string input, std::vector<std::string> &result);


    //对于购买数量Quantity 交易笔数
    //最大长度: 10位 正整数
    static int StringToIntegerQuant(const std::string& s);

    //交易总额 Count
    //最大长度 : 10位 可以为0
    static int StringToIntegerCount(const std::string& s);

    //对于图书单价Price 交易总额TotalCost 最大长度: 13位
    //精度固定为小数点后2位
    static double StringToDouble(const std::string& s);

    //检查UserID Password CurrentPassword NewPassword字符集是否合法
    //数字，字母，下划线；
    //30
    static void checkIDPassword(const std::string& s);

    //检查Username
    //除不可见字符以外 ASCII 字符；
    //30
    static void checkUsername(const std::string& s);

    //检查Privilege
    //数字
    //只允许出现 0 1 3 7
    static void checkPrivilege(const std::string& s);

    //检查ISBN
    //除不可见字符以外 ASCII 字符；
    // 20
    static void checkISBN(const std::string& s);

    //检查Bookname Author
    //除不可见字符和英文双引号以外 ASCII 字符
    // 60
    static void checkBooknameAuthor(const std::string& s);

    //检查Keyword
    //除不可见字符和英文双引号以外 ASCII 字符；
    // 60
    //[keyword] 内容以 | 为分隔可以出现多段信息。
    //如 math|magic|quantum 表示该图书具有三个无顺序的关键词
    //math, magic 和 quantum，每段信息长度至少为 1。
    static void checkKeyWordSingle(const std::string& s);

    //检查show指令的情况
    //对show指令进行切片
    //附加参数内容为空则操作失败；
    //无附加参数时，所有图书均满足要求；
    //[Keyword] 中出现多个关键词则操作失败。
    static std::string cutShow(const std::string& s);

    //检查modify 指令的情况
    //附加参数内容为空则操作失败；
    //[keyword] 包含重复信息段则操作失败。
    //有重复附加参数为非法指令；
    static std::string cutModify(const std::string& s);

    //检查多个Keyword的情况
    //[keyword] 内容以 | 为分隔可以出现多段信息。
    //如 math|magic|quantum 表示该图书具有三个无顺序的关键词
    //math, magic 和 quantum，
    //每段信息长度至少为 1。
    //除不可见字符和英文双引号以外 ASCII 字符
    //60
    static void checkKeywordAll(const std::string& s);
};


#endif //TOKENSCANNER_H
