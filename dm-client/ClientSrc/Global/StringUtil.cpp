#include "StringUtil.h"
#include <algorithm>
#include <stdarg.h>

//-----------------------------------------------------------------------
const string StringUtil::BLANK = string("");

void StringUtil::replace(string& str,const string& chars,char to)
{
	for(size_t ii = 0;ii < chars.size();ii++)
	{
		std::replace(str.begin(),str.end(),chars[ii],to);
	}
}

void StringUtil::replace2(string& str,const string& from,const string to)
{
	size_t i = 0;
	while((i = str.find(from)) != string::npos)
	{
		str.replace(i,from.size(),to.c_str());
	}
}

void StringUtil::replace_all(string& str,const string& old_value,const string& new_value)   
{   
	string::size_type  pos(0);   
	while((pos = str.find(old_value,pos)) != string::npos)
	{   
		str.replace(pos,old_value.length(),new_value);  
	}   
}  
//-----------------------------------------------------------------------
void StringUtil::trim(string& str, bool left, bool right)
{
	static const string delims = " \t\r";
	if(right)
		str.erase(str.find_last_not_of(delims)+1); // trim right
	if(left)
		str.erase(0, str.find_first_not_of(delims)); // trim left
}

void StringUtil::trim(char* str)
{
	std::string buf;
	buf.assign(str);
	static const std::string delims = " \t\r\n";
	buf.erase(buf.find_last_not_of(delims)+1); // trim right
	buf.erase(0, buf.find_first_not_of(delims)); // trim left

	strcpy(str,buf.c_str());
}

void StringUtil::erasechar(string& str,char c)
{
	size_t pos = 0,st = 0;	
	while (true)
	{
		pos = str.find_first_of(c, st);
		if (pos != string::npos)
		{
			str.erase(pos,1);
		}
		else break;
	}
}

//-----------------------------------------------------------------------
VString StringUtil::split( const string& str, const string& delims, unsigned int maxSplits)
{
	// static unsigned dl;
	VString ret;
	unsigned int numSplits = 0;

	// Use STL methods 
	size_t start, pos;
	start = 0;
	do 
	{
		pos = str.find_first_of(delims, start);
		if (pos == start)
		{
			// Do nothing
			start = pos + 1;
		}
		else if (pos == string::npos || (maxSplits && numSplits == maxSplits))
		{
			// Copy the rest of the string
			ret.push_back( str.substr(start) );
			break;
		}
		else
		{
			// Copy up to delimiter
			ret.push_back( str.substr(start, pos - start) );
			start = pos + 1;
		}
		// parse up to next real data
		start = str.find_first_not_of(delims, start);
		++numSplits;

	} while (pos != string::npos);



	return ret;
}

//-----------------------------------------------------------------------
void StringUtil::toLowerCase(string& str)
{
	std::transform(
		str.begin(),
		str.end(),
		str.begin(),
		tolower);
}

//-----------------------------------------------------------------------
void StringUtil::toUpperCase(string& str) 
{
	std::transform(
		str.begin(),
		str.end(),
		str.begin(),
		toupper);
}
//-----------------------------------------------------------------------
float StringUtil::toReal(const string& str)
{
	return (float)atof(str.c_str());
}
//-----------------------------------------------------------------------
bool StringUtil::startsWith(const string& str, const string& pattern, bool lowerCase)
{
	size_t thisLen = str.length();
	size_t patternLen = pattern.length();
	if (thisLen < patternLen || patternLen == 0)
		return false;

	string startOfThis = str.substr(0, patternLen);
	if (lowerCase)
		StringUtil::toLowerCase(startOfThis);

	return (startOfThis == pattern);
}
//-----------------------------------------------------------------------
bool StringUtil::endsWith(const string& str, const string& pattern, bool lowerCase)
{
	size_t thisLen = str.length();
	size_t patternLen = pattern.length();
	if (thisLen < patternLen || patternLen == 0)
		return false;

	string endOfThis = str.substr(thisLen - patternLen, patternLen);
	if (lowerCase)
		StringUtil::toLowerCase(endOfThis);

	return (endOfThis == pattern);
}
//-----------------------------------------------------------------------
string StringUtil::standardisePath(const string& init)
{
	string path = init;

	std::replace(path.begin(), path.end(), '\\', '/' );
	if( path[path.length() - 1] != '/' )
		path += '/';

	return path;
}
//-----------------------------------------------------------------------
void StringUtil::splitFilename(const string& qualifiedName, 
							   string& outBasename, string& outPath)
{
	string path = qualifiedName;
	// Replace \ with / first
	std::replace( path.begin(), path.end(), '\\', '/' );
	// split based on final /
	size_t i = path.find_last_of('/');

	if (i == string::npos)
	{
		outPath = "";
		outBasename = qualifiedName;
	}
	else
	{
		outBasename = path.substr(i+1, path.size() - i - 1);
		outPath = path.substr(0, i+1);
	}

}
//-----------------------------------------------------------------------
bool StringUtil::match(const string& str, const string& pattern, bool caseSensitive)
{
	string tmpStr = str;
	string tmpPattern = pattern;
	if (!caseSensitive)
	{
		StringUtil::toLowerCase(tmpStr);
		StringUtil::toLowerCase(tmpPattern);
	}

	string::const_iterator strIt = tmpStr.begin();
	string::const_iterator patIt = tmpPattern.begin();
	string::const_iterator lastWildCardIt = tmpPattern.end();
	while (strIt != tmpStr.end() && patIt != tmpPattern.end())
	{
		if (*patIt == '*')
		{
			lastWildCardIt = patIt;
			// Skip over looking for next character
			++patIt;
			if (patIt == tmpPattern.end())
			{
				// Skip right to the end since * matches the entire rest of the string
				strIt = tmpStr.end();
			}
			else
			{
				// scan until we find next pattern character
				while(strIt != tmpStr.end() && *strIt != *patIt)
					++strIt;
			}
		}
		else
		{
			if (*patIt != *strIt)
			{
				if (lastWildCardIt != tmpPattern.end())
				{
					// The last wildcard can match this incorrect sequence
					// rewind pattern to wildcard and keep searching
					patIt = lastWildCardIt;
					lastWildCardIt = tmpPattern.end();
				}
				else
				{
					// no wildwards left
					return false;
				}
			}
			else
			{
				++patIt;
				++strIt;
			}
		}

	}
	// If we reached the end of both the pattern and the string, we succeeded
	if (patIt == tmpPattern.end() && strIt == tmpStr.end())
	{
		return true;
	}
	else
	{
		return false;
	}

}

string StringUtil::format(char* fmt,...)
{
	string str;
	char tmp[1024] = {0};
	va_list argptr;

	va_start(argptr,fmt);
	vsprintf_s(tmp,fmt,argptr);
	va_end(argptr);

	str.assign(tmp);

	return tmp;
}

int StringUtil::toInt(const string& val)
{
	return atoi(val.c_str());
}

void StringUtil::AutoCut(string& str,int nMaxLength)
{
	if(nMaxLength < 1)
		return;

	if((int)str.size() <= nMaxLength)
		return;

	LPCTSTR ptr = CharNext(str.c_str()+nMaxLength);
	int iSize = (int)(ptr - str.c_str());
	str = str.substr(0,iSize);
}

void StringUtil::AutoCut(char *str, int nMaxLength)
{
	if(nMaxLength >= (int)strlen(str))
		return;
	str[nMaxLength] = 0;

	BYTE *p = (BYTE *)str;
	bool bAsc = false;
	for(int i = 0; i< nMaxLength; i++)
	{
		if(i == nMaxLength - 1 && bAsc)
		{
			p[i] = 0;
			return;
		}
		if(!bAsc && p[i] >= 128)
			bAsc = true;
		else
			bAsc = false;
	}
}

bool StringUtil::isSpace(string& str)
{
	string delims = " \t\r\n";
	return (string::npos == str.find_first_not_of(delims));
}

string StringUtil::GetCommerInt(int iInput, int iCommer, int iBase)
{
	char temp[128] = {0};
	char cBuf[128] = {0};
	ltoa(iInput,temp,iBase);
	int  iLen = (int)strlen(temp);

	int i = 0,j = 0;
	for(; i < iLen;)
	{
		if(i != 0 && (iLen - i)%3 == 0)
		{
			cBuf[j++] = ',';
		}
		cBuf[j++] = temp[i++];
	}
	cBuf[j] = 0;
	return cBuf;
}

//------------------------------------------------------------------------------
string StringUtil::toStr(const string& str,int &iStart,char cEndChar/* = '/'*/)
{
	string tmpStr;

	if(iStart >= (int)str.size())
		return tmpStr;

	int iPos = str.find_first_of(cEndChar,iStart);
	if(iPos == -1)
		iPos = (int)str.size();

	tmpStr = str.substr(iStart,iPos - iStart);
	iStart = iPos + 1;
	return tmpStr;
}

int StringUtil::toInt(const string& str,int &iStart,char cEndChar/* = '/'*/)
{
	string tmpStr;

	if(iStart >= (int)str.size())
		return 0;

	int iPos = str.find_first_of(cEndChar,iStart);
	if(iPos == -1)
		iPos = (int)str.size();

	tmpStr = str.substr(iStart,iPos - iStart);
	iStart = iPos + 1;
	return atoi(tmpStr.c_str());
}

UINT StringUtil::toUInt(const string& str,int &iStart,char cEndChar/* = '/'*/)
{
	string tmpStr;

	if(iStart >= (int)str.size())
		return 0;

	int iPos = str.find_first_of(cEndChar,iStart);
	if(iPos == -1)
		iPos = (int)str.size();

	tmpStr = str.substr(iStart,iPos - iStart);
	iStart = iPos + 1;

	UINT uInfo = 0;
	sscanf(tmpStr.c_str(),"%u",&uInfo);
	return uInfo;
}

float StringUtil::toFloat(const string& str,int &iStart,char cEndChar/* = '/'*/)
{
	string tmpStr;

	if(iStart >= (int)str.size())
		return 0.0f;

	int iPos = str.find_first_of(cEndChar,iStart);
	if(iPos == -1)
		iPos = (int)str.size();

	tmpStr = str.substr(iStart,iPos - iStart);
	iStart = iPos + 1;

	float fInfo = 0.0f;
	sscanf(tmpStr.c_str(),"%f",&fInfo);
	return fInfo;
}

bool StringUtil::hasInvalidChar(string& str)
{
	const char * p = str.c_str();
	while((*p) != 0)
	{
		BYTE ch = (BYTE)(*p++);
		if(ch > 0x80)//是汉字
		{
			//  GBK：区码 81H～FEH，位码：40H～FEH 
			//  GB：区码 A1H～F7H，位码：A1H～FEH
			//目前使用GBK字库
			if(ch < 0x81 || ch > 0xFE)
				return true;//区码不合法

			ch = (BYTE)(*p++); //
			if(ch < 0x40 || ch > 0xFE)//位码不合法
				return true;
		}
		else
		{
			if(!isdigit(ch) && ch != '_' && !isalpha(ch)) //有特殊字符
				return true;
		}
	}
	return false;
}

BYTE StringUtil::toHex(const BYTE &x)
{
	return x > 9 ? x + 55: x + 48;
}

string StringUtil::UrlEncoding(const string &sIn )
{
	std::string sOut;
	for( int ix = 0; ix < sIn.size(); ix++ )
	{
		BYTE buf[4];
		memset( buf, 0, 4 );
		if( isalnum( (BYTE)sIn[ix] ) )
		{
			buf[0] = sIn[ix];
		}
		else if ( isspace( (BYTE)sIn[ix] ) )
		{
			buf[0] = '+';
		}
		else
		{
			buf[0] = '%';
			buf[1] = toHex( (BYTE)sIn[ix] >> 4 );
			buf[2] = toHex( (BYTE)sIn[ix] % 16);
		}
		sOut += (char *)buf;
	}
	return sOut;
}

string StringUtil::Gb2312ToUtf8(const string &sIn )
{
	if (sIn.size() <= 0 || sIn.size() >= 1024)
		return "";

	WCHAR wcUniCode[2 * 1024] = {0};
	MultiByteToWideChar(CP_ACP,0,sIn.c_str(),-1,wcUniCode,2 * 1024);

	char wcUtf8[4 * 1024] = {0};
	WideCharToMultiByte(CP_UTF8,0,wcUniCode,-1,wcUtf8,4 * 1024,NULL,NULL);

	return wcUtf8;
}
