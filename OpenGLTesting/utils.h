#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <streambuf>

using namespace std;

string readFileToString(const char * filePath) {
	ifstream inFile;
	inFile.open(filePath);
	stringstream strStream;
	strStream << inFile.rdbuf();
	string str = strStream.str();

	cout << "Read from file " << filePath << ":\n" << str << "\n" << endl;

	return str;
}

#define glCheckError() glCheckError_(__FILE__, __LINE__) 
GLenum glCheckError_(const char *file, int line)
{
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		std::string error;
		switch (errorCode)
		{
		case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
		case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
		case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		}
		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
	}
	return errorCode;
}
