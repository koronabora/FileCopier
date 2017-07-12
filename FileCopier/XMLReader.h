#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "rapidxml.hpp"

using namespace std;
using namespace rapidxml;

#define XML_READ_BUFF

struct FileData
{
	wstring srcPath;
	wstring fileName;
	wstring copyToPath;
};

class XMLReader
{
public:
	static XMLReader* i();
	static vector<FileData> readAllFileNames(wstring xmlFile);

private:
	XMLReader();
	~XMLReader();	
	//wchar 
};