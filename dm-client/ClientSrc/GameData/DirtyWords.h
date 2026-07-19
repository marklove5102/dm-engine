#pragma once

#include "Global/Global.h"
#include "ObjectLink/MultiLine.h"
#include "Global/StringUtil.h"

class CDirtyWords
{
public:
	CDirtyWords();
	~CDirtyWords();

	bool ClearWords(const char * strMessage, char * buf);
	bool ClearWords(CMultiLine* strLine);     //替换掉屏蔽字
	bool ClearWords(CStringLine* strLine);
	bool ClearWords(string& strTemp);
	bool HaveDirtyWords(const char * strMessage,DWORD dwForbidType = 0xFFFFFFFF);

	bool ClearWords1(const char * strMessage);	//如果有相应的禁止关键字，整句话屏蔽
	bool ClearWords1(CMultiLine* strLine);   	//如果有相应的禁止关键字，整句话屏蔽
	bool ClearWords1(CStringLine* strLine);
	bool ClearWords_All(string& strTemp);//清除名人等敏感词语,网址,特殊字符

	bool GetName(string& strName,string& str);
    bool ParseNameIsGM(string& strName);
private:
	bool ReadDirtyWordsFile(const char * strFileName,const char * strPwd = "邱玢整理haha");
	bool ReadDirtyWordsFile1(const char * strFileName,const char * strPwd = "邱玢整理haha");
	bool ReadDirtyWordsFile2(const char * strFileName,const char * strPwd = "邱玢整理haha");

	VString		m_vecDirtyWords;
	vector<std::wstring>		m_vecDirtyWords1;

	VString		m_vecDirtyWords_1;
	vector<std::wstring>		m_vecDirtyWords1_1;

	VString		m_vecSpecial;

	wchar_t wstr[10240];
};

extern CDirtyWords g_DirtyWords;