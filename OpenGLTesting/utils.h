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