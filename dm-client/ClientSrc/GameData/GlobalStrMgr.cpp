#include "GlobalStrMgr.h"
#include "Global/Global.h"
#include <fstream>

CGlobalStrMgr g_StrMgr;

CGlobalStrMgr::CGlobalStrMgr(void)
{
	char strPath[256] = {0};
	sprintf_s(strPath,"%s\\config\\GlobalStrings.ini",GetGameDataDir());

	std::ifstream fin(strPath);
	if(!fin.is_open())
		return;

	m_GlobalStr.clear();

	VString strvect;
	while(!fin.eof())
	{
		string inbuf;
		getline(fin, inbuf,'\n');
		strvect.push_back(inbuf);
	}

	if(strvect.empty())
		return;

	for(VString::iterator it = strvect.begin();it != strvect.end();it++)
	{
		int first = it->find('\"');
		int last = it->rfind('\"');
		if( first != string::npos && last != string::npos && first != last+1)
		{
			m_GlobalStr.push_back(it->substr(first+1,last-first-1));			
		}
	}

	fin.close();
}

CGlobalStrMgr::~CGlobalStrMgr(void)
{
}
