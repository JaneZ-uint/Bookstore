cmake_minimum_required(VERSION 3.28)
project(Bookstore_2024)

set(CMAKE_CXX_STANDARD 20)

add_executable(code src/main.cpp
        include/Block.h
        include/Book.h
        src/Book.cpp
        include/User.h
        include/Finance.h
        include/Error.h
        include/Tokenscanner.h
        src/Blog.cpp
        include/Blog.h
        src/Finance.cpp
        src/Error.cpp
        src/User.cpp
        src/Tokenscanner.cpp
        include/MemoryRiver.h
)
