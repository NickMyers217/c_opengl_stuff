#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <streambuf>
#include <glad\glad.h>


#define glCheckError() glCheckError_(__FILE__, __LINE__)
namespace MyUtils {
	std::string readFileToString(const char * filePath);
	GLenum glCheckError_(const char * file, int line);
};
