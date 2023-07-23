#pragma once

#include <iostream>
#include <sys/stat.h>
#include <sys/time.h>
#include <errno.h>
#include <string.h>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdarg.h>
#include <map>

#define LOG_NOTHING 0
#define LOG_EVERYTHING 3
#define LOG_WARNINGS_ERRORS 2
#define LOG_ERRORS_ONLY 1

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_EVERYTHING
#endif

#define UPLOAD_DIR "upload/"

#define COL_RED "\x1B[31m"
#define COL_YELLOW "\x1B[33m"
#define COL_GREY "\x1B[90m"
#define COL_DEFAULT "\033[0m"

#define PRINTVAR(x) std::cerr << #x <<": [" << x << "]"  << std::endl
#define PRINT(x) std::cerr << x  << std::endl
#define PRINTVAR_LOG(x) std::cerr<< COL_GREY << #x <<": [" << x << "]" << COL_DEFAULT  << std::endl

#define PRINT_LOG(...)
#define PRINT_WARNING(...)
#define PRINT_ERROR(...)

// #define PRINT_LOG(...) do { \
//     if(LOG_LEVEL >= LOG_EVERYTHING){ \
//     std::cerr << COL_GREY ; \
//     printvar_internal(__VA_ARGS__); \
//     std::cerr << COL_DEFAULT << std::endl; \
//     } \
// } while (false)


// #define PRINT_WARNING(...) do { \
//     if(LOG_LEVEL >= LOG_WARNINGS_ERRORS){ \
//     std::cerr << COL_YELLOW ; \
//     printvar_internal(__VA_ARGS__); \
//     std::cerr << COL_DEFAULT << std::endl; \
//     } \
// } while (false)

// #define PRINT_ERROR(...) do { \
//     if(LOG_LEVEL >= LOG_ERRORS_ONLY){ \
//     std::cerr << COL_RED ; \
//     printvar_internal(__VA_ARGS__); \
//     std::cerr << COL_DEFAULT << std::endl; \
//     } \
// } while (false)

// Helper function for printing multiple variables
// template<typename T>
// void printvar_internal(const T& t) {
//     std::cerr << t;
// }

// template<typename T, typename... Args>
// void printvar_internal(const T& t, Args... args) {
//     std::cerr << t << " ";
//     printvar_internal(args...);
// }


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
std::string getTypeFromExtension( std::string path, const std::map<std::string, std::string>& types);
long	getUnixTime();
