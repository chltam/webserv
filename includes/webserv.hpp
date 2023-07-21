#pragma once

#include <iostream>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdarg.h>

#define PRINTVAR(x) std::cerr << #x <<": [" << x << "]"  << std::endl
#define PRINT(x) std::cerr << x  << std::endl
#define PRINT_ERROR(x) fprintf(stderr,"\x1B[31m%s\033[0m\n",x)
#define PRINT_WARNING(x) fprintf(stderr,"\x1B[33m%s\033[0m\n",x)

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
int StringToMethodEnum(const std::string& method);
std::string saveDateTime();
std::string getFileContent( const std::string& filename );
std::string getStringFromStatus(int status);
std::string	toString(int number);
