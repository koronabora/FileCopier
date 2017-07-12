#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <io.h> 
#include <fcntl.h>
#include <algorithm>
#include <locale>
#include <codecvt>
#include "XMLReader.h"

using namespace std;

#define DEF_XML_FILE L"config.xml"
#define BLOCK_SIZE 4096LL

#ifdef _MSC_VER
#define PATH_SEP L"\\"
#else
#define PATH_SEP L"//"
#endif

bool copyFile(FileData dat)
{
	bool res = true;
	wstring src = dat.srcPath;
	src.append(PATH_SEP);
	src.append(dat.fileName);
	wstring dst = dat.copyToPath;
	dst.append(PATH_SEP);
	dst.append(dat.fileName);
	wcout << src << " -> " << dst << endl;

	wifstream in;
	in.open(src, ios::in | ios::binary);
	wofstream out;
	out.open(dst, ios::out | ios::binary);
	
	if (!in)
	{
		wcout << "Could not open file " << src << " for reading!" << endl;
		res = false;
	}
	else if (!out)
	{
		wcout << "Could not open file " << dst << " for writing!" << endl;
		res = false;
	}
	else
	{
		// get file size
		in.seekg(0, ios::end);
		streampos length = in.tellg();
		in.seekg(0);

		// read by peaces
		wchar_t buffer[BLOCK_SIZE];
		streampos curPos = 0;
		while (1)
		{
			if (!in.is_open() || in.eof() || !out.is_open())
				break;
			long long ss = min(BLOCK_SIZE, length.seekpos() - curPos.seekpos());
#ifdef _DEBUG
			wcout << "Block size = " << ss << " " << in.eof() << endl;
#endif
			in.read(buffer, ss);
			out.write(buffer, ss);
			curPos = in.tellg();
			if (ss < BLOCK_SIZE)
				in.read(buffer, 1); // don't forget eof symbol
		}
		if (!in.eof())
		{
			wcout << "Something went wrong while reading input file." << endl;
			res = false;
		}
		in.close();
		out.close();
	}
	return res;
}

int main(int argc, char **argv)
{
	_setmode(_fileno(stdout), _O_U16TEXT);

	wstring xmlFile(DEF_XML_FILE);
	if (argc > 1)
		if (argv != nullptr && argv[1] != nullptr)
		{
			wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
			xmlFile = converter.from_bytes(argv[1]);
		}

	wcout << "Welcome to file copy utiliy!" << endl;
	wcout << "Input settings file: " << xmlFile << endl;

	vector<FileData> d = XMLReader::readAllFileNames(xmlFile);
	wcout << "Starting to copy " << d.size() << " files: " << endl;
	int count = 0;
	for (FileData dat : d)
		if (copyFile(dat))
			count++;
	wcout << "Suscessfully copied " << count << " files!" << endl;

	wcout << "Have a nice day!" << endl;

#ifdef _MSC_VER
	system("pause");
#endif

	return 0;
}