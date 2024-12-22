//
// Created by zhuyi on 2024/12/20.
//

#ifndef ERROR_H
#define ERROR_H
#include <exception>
#include <string>

class InvalidExpression :public std::exception {
public:
    const char* what() const noexcept override;
};

#endif //ERROR_H
