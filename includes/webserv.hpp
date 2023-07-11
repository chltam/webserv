#pragma once

#include <iostream>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

#define PRINT(x) std::cerr << #x <<": " << x  << std::endl

int ValidatePath(const std::string& path);
