#pragma once

#include <iostream>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

#define PRINTVAR(x) std::cerr << #x <<": " << x  << std::endl
#define PRINT(x) std::cerr << x  << std::endl
#define PRINT_MORE(x) std::cerr << "'" << x << "'" << std::endl


enum AllowedMethods{
    METH_NONE   = 0,
    METH_GET    = 1,
    METH_POST   = 2,
    METH_DELETE = 4,
    METH_ALL    = 7
};


int ValidatePath(const std::string& path);
int getFileSize(const std::string &path);
std::string MethodEnumToString(int val);
std::string saveDateTime();
