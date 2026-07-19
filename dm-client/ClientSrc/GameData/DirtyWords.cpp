
#include "DirtyWords.h"
#include "Global/Global.h"
#include "Global/DebugTry.h"
#include <io.h>
#include <algorithm>

CDirtyWords g_DirtyWords;

bool CDirtyWords::GetName(string& strName,string& str)
{
	if(str.size() == 0)
		return false;

	int iBeginPos = 0;
	if(str[0] == '/')
	{
		iBeginPos = str.find(' ');
		if(iBeginPos < 0)
			iBeginPos = str.size();
		else
			iBeginPos++;
	}
	else if(str[0] == '(')
	{
		iBeginPos = str.find(':');

		if(iBeginPos > 0)
			iBeginPos++;
	}
	else if(str[0] == '=')
	{
		iBeginPos = str.find(':');
		if(iBeginPos > 0)
			iBeginPos++;
	}
	else if(str.size() > 3 && str[0] == '<' && str[3] == '>')
	{
		iBeginPos = str.find("=>");
		if(iBeginPos > 0)
			iBeginPos += 2;
	}
	else
	{
		iBeginPos = str.find(':');
		if(iBeginPos < 0)
			iBeginPos = str.find(' ');

		if(iBeginPos > 0)
			iBeginPos++;
	}

	if(iBeginPos == -1)
	{
		iBeginPos = 0;
		return false;
	}
	strName = str.substr(0,iBeginPos);
	str = str.substr(iBeginPos,str.size() - iBeginPos);

	return true;
}

//判断是否GM01 --- GM09 的留言
bool CDirtyWords::ParseNameIsGM(string& strName)
{
	if(strName.find("gm") == string::npos && strName.find("GM") == string::npos)
		return false;

	int iBeginPos = 0;
	int iEndPos = 0;
    if(strName.size() > 7 && strName[0] == '<' && strName[3] == '>')
	{

	    iBeginPos = 4;
		iEndPos = strName.find("=>");
		if(iEndPos < 0)
			iEndPos = strName.size();
	}
	else if(strName[0]=='(' && strName[2]==')')
	{
		iBeginPos =3;
		iEndPos =strName.find(':');
		if(iEndPos<0)
			iEndPos = strName.size();
	}
	else
	{
		iBeginPos = 0;
		iEndPos = strName.find(':');
		if(iEndPos < 0)
			iEndPos = strName.size();
	}

	if((iEndPos-iBeginPos) != 4)
		return false;

	if(strnicmp(strName.c_str()+iBeginPos,"gm0",3) == 0 && isdigit(strName[iBeginPos+3]))
		return true;

	return false;
}


CDirtyWords::CDirtyWords()
{
	string strPath = GetGameDataDir();
	ReadDirtyWordsFile((strPath + "\\config\\dirtywords.dat").c_str());
	ReadDirtyWordsFile1((strPath + "\\config\\dirtywords1.dat").c_str());
	ReadDirtyWordsFile2((strPath + "\\config\\dirtywords2.dat").c_str());
}

CDirtyWords::~CDirtyWords()
{

}

bool CDirtyWords::ReadDirtyWordsFile(const char * strFileName,const char * strPwd)
{
    FILE *hSource      = NULL;
    INT eof = 0;
	DWORD key = 0x1001011;
	string bbb;

    PBYTE pbBuffer = NULL;
    DWORD dwBufferLen;

    if((hSource = fopen(strFileName,"rb")) == NULL) 
	{
		return false;
	}

	dwBufferLen = filelength(fileno(hSource));

	pbBuffer = new BYTE[dwBufferLen];
	fread(pbBuffer,dwBufferLen,1,hSource);
	fclose(hSource);

	for(size_t i = 0; i < dwBufferLen; i++)
	{
		pbBuffer[i] = pbBuffer[i] ^ (BYTE)key;
	}
	bbb.assign((const char *)pbBuffer,dwBufferLen);

	size_t begin = 0;
	size_t end = 0;
	while(1)
	{
		end = bbb.find("\r\n",begin);
		if( end == string::npos )
			break;
		string tmp = bbb.substr(begin,end - begin);
		if(tmp.size() > 0)
            m_vecDirtyWords.push_back(tmp);
		begin = end + 2;
	}
	delete pbBuffer;
	std::locale loc;
	wchar_t wstr[512] = {0};
	for(UINT i = 0; i <  m_vecDirtyWords.size(); i++)
	{
		std::transform(m_vecDirtyWords[i].begin(), m_vecDirtyWords[i].end(), m_vecDirtyWords[i].begin(), tolower);
		int b = MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,&(*m_vecDirtyWords[i].begin()),
			m_vecDirtyWords[i].size(),wstr,512);
		wstr[b] = 0;

		m_vecDirtyWords1.push_back(wstr);
	}
	return true;

}

bool CDirtyWords::ClearWords(string& strTemp)
{
	if(strTemp[0] == '@' || strTemp.substr(0,2) == "&@")
		return false;

	std::string strName;
	GetName(strName,strTemp);
	//////////////////////////////////////
	//GM01 -- GM09 的消息不过滤
	if( ParseNameIsGM(strName) )
		return false;
	//std::string strMyName;
	//strMyName = g_pGameData->GetSelfCharacter().GetName();
	//if(ParseNameIsGM(strMyName))
	//	return false;
	//--------------------------
	std::string strTemp1= strTemp;
	std::wstring wstrTemp;
	std::locale loc;

    //std::tolower(strTemp1.c_str(),loc);
	std::transform(strTemp1.begin(), strTemp1.end(), strTemp1.begin(), tolower);

	int b = MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,strTemp1.c_str(),
		strTemp1.size(),wstr,10240);

	wstr[b] = 0;

	wstrTemp = wstr;
	std::wstring wstrTemp11;


	for(size_t i = 0 ; i < m_vecDirtyWords.size() ; i ++)
	{
		size_t begin = 0;
		size_t end = 0;
		//wstrTemp11.clear();

		while(1)
		{
			end = wstrTemp.find(m_vecDirtyWords1[i],begin);
			if( end == std::string::npos )
				break;

			char tmp[1024] = {0};
			//替换字符数
			int iLen = WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,m_vecDirtyWords1[i].c_str(),
				m_vecDirtyWords1[i].size(),tmp,1024,NULL,NULL);
			//开始位置
			int iPos = WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,wstrTemp.c_str(),
				end,tmp,1024,NULL,NULL);
			
			strTemp.replace(iPos,iLen,iLen,'*');
			begin = end + m_vecDirtyWords1[i].size();





			//wstrTemp.replace(end,m_vecDirtyWords1[i].size(),m_vecDirtyWords1[i].size(),'*');
			//wstrTemp11 = wstrTemp.substr(0,end + m_vecDirtyWords1[i].size());

			//char tmp[10240] = {0};
			//int j = WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,wstrTemp11.c_str(),
			//	wstrTemp11.size(),tmp,10240,NULL,NULL);

			//strTemp.replace(j - m_vecDirtyWords[i].size(),m_vecDirtyWords[i].size(),m_vecDirtyWords[i].size(),'*');
			//begin = end + m_vecDirtyWords1[i].size();
		}
	}
	strTemp = strName + strTemp;
	return true;
}

bool CDirtyWords::ClearWords(const char * strMessage, char * buf)
{
	string strTemp = strMessage;
	this->ClearWords(strTemp);
	strcpy(buf,strTemp.c_str());
	return true;
}

bool CDirtyWords::ClearWords1(const char * strMessage)
{
	string strTemp = strMessage;

	if(strTemp[0] == '@' || strTemp.substr(0,2) == "&@")
		return false;

	string strName;
	GetName(strName,strTemp);

	if( ParseNameIsGM(strName) )
		return false;
 	string strTemp1= strTemp;
	std::wstring wstrTemp;
	std::locale loc;

	for(int i = 0; i < (int)m_vecSpecial.size(); i++)
	{
		int npos = strTemp1.find(m_vecSpecial[i]);
		while(npos != string::npos)
		{
            strTemp1.erase(npos,m_vecSpecial[i].size());
			npos = strTemp1.find(m_vecSpecial[i]);
		}
	}

	//std::tolower(strTemp1.c_str(), loc );
	std::transform(strTemp1.begin(), strTemp1.end(), strTemp1.begin(), tolower);


	int b = MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,strTemp1.c_str(),
			strTemp1.size(),wstr,10240);

	wstr[b] = 0;
    
	wstrTemp = wstr;

	for(size_t i = 0 ; i < m_vecDirtyWords_1.size() ; i ++)
	{
		if( wstrTemp.find(m_vecDirtyWords1_1[i]) != string::npos )
		{
			return true;
		}
	}
	return false;
}

bool CDirtyWords::HaveDirtyWords(const char * strMessage,DWORD dwForbidType)
{
	if (!strMessage)
	{
		return false;
	}

	string strTemp1= strMessage;
	std::wstring wstrTemp;
	std::locale loc;

	// *," [等特殊符号
	if (dwForbidType & 0x00000001)
	{
		for(int i = 0; i < (int)m_vecSpecial.size(); i++)
		{
			int npos = strTemp1.find(m_vecSpecial[i]);
			if(npos != string::npos)
			{
				return true;
			}
		}
	}

	std::transform(strTemp1.begin(), strTemp1.end(), strTemp1.begin(), tolower);
	int b = MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,strTemp1.c_str(),strTemp1.size(),wstr,10240);
	wstr[b] = 0;
	wstrTemp = wstr;

	//www,网址,挂机等
	if (dwForbidType & 0x00000002)
	{
		for(size_t i = 0 ; i < m_vecDirtyWords_1.size() ; i ++)
		{
			if( wstrTemp.find(m_vecDirtyWords1_1[i]) != string::npos )
			{
				return true;
			}
		}
	}

	//名人等脏话
	if (dwForbidType & 0x00000004)
	{
		for(size_t i = 0 ; i < m_vecDirtyWords.size() ; i ++)
		{
			size_t pos = wstrTemp.find(m_vecDirtyWords1[i],0);
			if(pos != std::string::npos )
				return true;
		}
	}

	return false;
}

//黄榜大旗中清除网址等字符
bool CDirtyWords::ClearWords_All(string& strTemp)
{
	std::string strTemp1= strTemp;
	std::wstring wstrTemp;
	std::locale loc;


	std::transform(strTemp1.begin(), strTemp1.end(), strTemp1.begin(), tolower);
	int b = MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,strTemp1.c_str(),
		strTemp1.size(),wstr,10240);

	wstr[b] = 0;

	wstrTemp = wstr;
	//网址等
	for(size_t i = 0 ; i < m_vecDirtyWords_1.size() ; i ++)
	{
		size_t begin = 0;
		size_t end = 0;

		while(1)
		{
			end = wstrTemp.find(m_vecDirtyWords1_1[i],begin);
			if( end == std::string::npos )
				break;

			char tmp[1024] = {0};
			//替换字符数
			int iLen = WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,m_vecDirtyWords1_1[i].c_str(),
				m_vecDirtyWords1_1[i].size(),tmp,1024,NULL,NULL);
			//开始位置
			int iPos = WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,wstrTemp.c_str(),
				end,tmp,1024,NULL,NULL);

			strTemp.replace(iPos,iLen,iLen,'*');
			begin = end + m_vecDirtyWords1_1[i].size();
		}
	}
	//特殊字符
	for(int i = 0; i < (int)m_vecSpecial.size(); i++)
	{
		int npos = strTemp1.find(m_vecSpecial[i]);
		while(npos != string::npos)
		{
			strTemp1.erase(npos,m_vecSpecial[i].size());
			npos = strTemp1.find(m_vecSpecial[i]);
		}
	}
	//名人,敏感词语
	for(size_t i = 0 ; i < m_vecDirtyWords.size() ; i ++)
	{
		size_t begin = 0;
		size_t end = 0;

		while(1)
		{
			end = wstrTemp.find(m_vecDirtyWords1[i],begin);
			if( end == std::string::npos )
				break;

			char tmp[1024] = {0};
			//替换字符数
			int iLen = WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,m_vecDirtyWords1[i].c_str(),
				m_vecDirtyWords1[i].size(),tmp,1024,NULL,NULL);
			//开始位置
			int iPos = WideCharToMultiByte(CP_ACP,WC_COMPOSITECHECK,wstrTemp.c_str(),
				end,tmp,1024,NULL,NULL);

			strTemp.replace(iPos,iLen,iLen,'*');
			begin = end + m_vecDirtyWords1[i].size();
		}
	}


	return true;
}

bool CDirtyWords::ReadDirtyWordsFile1(const char * strFileName,const char * strPwd)
{
    FILE *hSource      = NULL;
    INT eof = 0;
	DWORD key = 0x1001011;
	string bbb;
    PBYTE pbBuffer = NULL;
    DWORD dwBufferLen;
    if((hSource = fopen(strFileName,"rb")) == NULL) 
	{
		return false;
	}

	dwBufferLen = filelength(fileno(hSource));

	pbBuffer = new BYTE[dwBufferLen];
	fread(pbBuffer,dwBufferLen,1,hSource);
	fclose(hSource);

	for(size_t i = 0; i < dwBufferLen; i++)
	{
		pbBuffer[i] = pbBuffer[i] ^ (BYTE)key;
	}
	bbb.assign((const char *)pbBuffer,dwBufferLen);
	size_t begin = 0;
	size_t end = 0;
	while(1)
	{
		end = bbb.find("\r\n",begin);
		if( end == string::npos )
			break;
		string tmp = bbb.substr(begin,end - begin);
		if(tmp.size() > 0)
            m_vecDirtyWords_1.push_back(tmp);
		begin = end + 2;
	}
	delete pbBuffer;
	std::locale loc;
	wchar_t wstr[512] = {0};
	for(UINT i = 0; i <  m_vecDirtyWords_1.size(); i++)
	{
		//std::tolower(m_vecDirtyWords_1[i].c_str(),loc);
		std::transform(m_vecDirtyWords_1[i].begin(), m_vecDirtyWords_1[i].end(), m_vecDirtyWords_1[i].begin(), tolower);
		int b = MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,&(*m_vecDirtyWords_1[i].begin()),
			m_vecDirtyWords_1[i].size(),wstr,512);
		wstr[b] = 0;

		m_vecDirtyWords1_1.push_back(wstr);
	}
	return true;

}


bool CDirtyWords::ReadDirtyWordsFile2(const char * strFileName,const char * strPwd)
{
    FILE *hSource      = NULL;
    INT eof = 0;
	DWORD key = 0x1001011;
	string bbb;
    PBYTE pbBuffer = NULL;
    DWORD dwBufferLen;
    if((hSource = fopen(strFileName,"rb")) == NULL) 
	{
		return false;
	}

	dwBufferLen = filelength(fileno(hSource));

	pbBuffer = new BYTE[dwBufferLen];
	fread(pbBuffer,dwBufferLen,1,hSource);
	fclose(hSource);

	for(size_t i = 0; i < dwBufferLen; i++)
	{
		pbBuffer[i] = pbBuffer[i] ^ (BYTE)key;
	}
	bbb.assign((const char *)pbBuffer,dwBufferLen);
	size_t begin = 0;
	size_t end = 0;
	while(1)
	{
		end = bbb.find("\r\n",begin);
		if( end == string::npos )
			break;
		string tmp = bbb.substr(begin,end - begin);
		if(tmp.size() > 0)
            m_vecSpecial.push_back(tmp);
		begin = end + 2;
	}
	delete pbBuffer;
	return true;
}

bool CDirtyWords::ClearWords1(CMultiLine* strLine)
{
	if(strLine == NULL) return false;

	if(this->ClearWords1(strLine->c_str()))
		return true;

	return false;
}

bool CDirtyWords::ClearWords1(CStringLine* strLine)
{
	if(strLine == NULL) return false;

	if(this->ClearWords1(strLine->c_str()))
		return true;

	return false;
}

bool CDirtyWords::ClearWords(CMultiLine* strLine)
{
	if(strLine == NULL)
		return false;

	string strTemp = strLine->c_str();
	if(this->ClearWords(strTemp))
	{
		strLine->clean(strTemp.c_str());  //改变原来的内容
	}
	return true;
}

bool CDirtyWords::ClearWords(CStringLine* strLine)
{
	if(strLine == NULL)
		return false;

	string strTemp = strLine->c_str();
	if(this->ClearWords(strTemp))
	{
		strLine->clean(strTemp.c_str());  //改变原来的内容
	}
	return true;
}