#include "XMLReader.h"

XMLReader* XMLReader::i()
{
	static XMLReader instance;
	return &instance;
}

XMLReader::XMLReader()
{}

XMLReader::~XMLReader()
{}

vector<FileData> XMLReader::readAllFileNames(wstring xmlFile)
{
	vector<FileData> res;
	wstring text;
	wifstream file;

	file.open(xmlFile, ios::in);
	if (!file)
	{
		wcout << "Cannot read input xml file: " << xmlFile << endl;
	}
	else
	{
		// read all data
		wstring buf;
		while (1)
		{
			if (file.eof() || !file.is_open())
				break;
			file >> buf;
			text.append(buf);
		}
		if (!file.eof())
			wcout << "An error occured while reading input xml file: " << xmlFile << endl;
		else
		{
			// parse xml
			xml_document<wchar_t> doc;
			wchar_t *xbuf = new wchar_t[text.size() + 1];
			for (long long i = 0; i < text.size(); i++) xbuf[i] = text[i];
			xbuf[text.size()] = L'\0';
			doc.parse<0>(xbuf);
			// read data from xml
			xml_node<wchar_t> *pRoot = doc.first_node(L"Files");
			// With the xml example above this is the <document/> node
			if (pRoot != nullptr)
				for (xml_node<wchar_t> *pNode = pRoot->first_node(L"File"); pNode; pNode = pNode->next_sibling())
				{
					if (pNode != nullptr)
					{
						FileData dat;
						dat.fileName = pNode->first_node(L"FileName")->value();
						dat.srcPath = pNode->first_node(L"SrcPath")->value();
						dat.copyToPath = pNode->first_node(L"CopyToPath")->value();
						res.push_back(dat);
					}
				}
			else
				wcout << "An error occured while parsing input xml file: " << xmlFile << endl;
		}
	}
	return res;
}