#include "sw.h"
#include <iostream>
#include <fstream>
#include <sstream>

#ifdef _WIN32
#include <Windows.h>
#endif


#define PACE 1


WordFilter* WordFilter::pInstace = NULL;


WordNode::WordNode(string character) {
	if (character.size() == PACE)
	{
		m_character.assign(character);
	}
	else
		cout << "error" << endl;
}

WordNode* WordNode::findChild(string& nextCharacter) {
	_TreeMapIterator TreeMapIt = m_map.find(nextCharacter);
	if (TreeMapIt == m_map.end())
	{
		return NULL;
	}
	else
	{
		return &TreeMapIt->second;
	}
}
WordNode* WordNode::insertChild(string& nextCharacter) {
	if (!findChild(nextCharacter))
	{
		m_map.insert(pair<string, WordNode>(nextCharacter, WordNode(nextCharacter)));
		return &(m_map.find(nextCharacter)->second);
	}
	return NULL;
}






WordNode* WordTree::insert(string& keyword) {
	return insert(&m_emptyRoot, keyword);
}
WordNode* WordTree::insert(const char* keyword) {
	string wordstr(keyword);
	return insert(wordstr);
}
WordNode* WordTree::find(string& keyword) {
	return find(&m_emptyRoot, keyword);
}

WordNode* WordTree::insert(WordNode* parent, string& keyword) {
	if (keyword.size() == 0)
	{
		return NULL;
	}
	string firstChar = keyword.substr(0, PACE);
	WordNode* firstNode = parent->findChild(firstChar);
	if (firstNode == NULL)
	{
		return insertBranch(parent, keyword);
	}
	string restChar = keyword.substr(PACE, keyword.size());
	return insert(firstNode, restChar);
}
WordNode* WordTree::insertBranch(WordNode* parent, string& keyword) {
	string firstChar = keyword.substr(0, PACE);
	WordNode* firstNode = parent->insertChild(firstChar);
	if (firstNode != NULL)
	{
		string restChar = keyword.substr(PACE, keyword.size());
		if (!restChar.empty())
		{
			return insertBranch(firstNode, restChar);
		}
	}
	return NULL;
}
WordNode* WordTree::find(WordNode* parent, string& keyword) {
	string firstChar = keyword.substr(0, PACE);
	WordNode* firstNode = parent->findChild(firstChar);
	if (firstNode == NULL)
	{
		count = 0;
		return NULL;
	}
	string restChar = keyword.substr(PACE, keyword.size());
	if (firstNode->m_map.empty())
	{
		return firstNode;
	}
	if (keyword.size() == PACE)
	{
		return NULL;
	}
	count++;
	return find(firstNode, restChar);
}




WordFilter* WordFilter::sharedInstace() {
	if (pInstace)
	{
		return pInstace;
	}
	pInstace = new WordFilter;
	return pInstace;
}
void WordFilter::release() {
	if (pInstace)
	{
		delete pInstace;
	}
	pInstace = NULL;
}


void WordFilter::load(const char* filepath) {
	ifstream infile(filepath, ios::in);

	if (!infile)
	{
		cout << "open file error" << endl;
	}
	else
	{
		cout << "open file succeed" << endl;
		string read;
		while (getline(infile, read))
		{
//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//			string s;
//			s = read.substr(0, read.length() - 1);
//			m_tree.insert(s);
//#endif
			m_tree.insert(read);
		}
	}

	infile.close();
}

void WordFilter::load(const std::string& content) {
	std::stringstream ss(content);

	string read;
	while (getline(ss, read))
	{
		//#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
		//			string s;
		//			s = read.substr(0, read.length() - 1);
		//			m_tree.insert(s);
		//#endif
		// Trim
		if (read.size() > 0) {
			if (read[read.size() - 1] == '\n') {
				read.resize(read.size() - 1);
			}
		}
		if (read.size() > 0) {
			if (read[read.size() - 1] == '\r') {
				read.resize(read.size() - 1);
			}
		}
		m_tree.insert(read);
	}
}

#ifdef _WIN32
bool WordFilter::loadFromRes(int nResID, const char* type) {
	HRSRC src = FindResource(::GetModuleHandle(NULL), MAKEINTRESOURCE(nResID), type);
	if (NULL == src) {
		return false;
	}

	HGLOBAL hg = LoadResource(::GetModuleHandle(NULL), src);
	if (NULL == hg) {
		return false;
	}

	LPVOID data = LockResource(hg);
	if (NULL == data) {
		return false;
	}

	DWORD size = SizeofResource(::GetModuleHandle(NULL), src);
	if (0 == size) {
		return false;
	}

	std::string content((const char *)data, size);
	load(content);

	FreeResource(src);

	return true;
}
#endif

void WordFilter::censor(string& source) {
	int lenght = source.size();
	for (int i = 0; i < lenght; i += 1)
	{
		string substring = source.substr(i, lenght - i);
		if (m_tree.find(substring) != NULL)
		{
			source.replace(i, (m_tree.count + 1), "**");
			lenght = source.size();
			cout << "source = " << source << endl;
		}
	}
}
