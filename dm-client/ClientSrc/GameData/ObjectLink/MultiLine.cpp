#include "StringLine.h"
#include "multiline.h"
#include "../GameDefine.h"
#include "Global/Interface/FontInterface.h"

CMultiLine::CMultiLine(DWORD color,DWORD back,DWORD dwType,int limit,int iFontType,int iFontSize,bool bIgnoreewLine)
{
	m_iLimit = limit;
	m_dwColor = color;
	m_dwBack = back;

	m_dwType = dwType;
	m_iAlpha = 500;
	m_show.clear();
	m_dwFlag = 0;
	m_iFontType = iFontType;
	m_iFontSize = iFontSize;
	m_bIgnoreewLine = bIgnoreewLine;
}

CMultiLine::~CMultiLine(void)
{
	this->clear();
}

void CMultiLine::clear()
{
	for(size_t i = 0; i < m_lines.size();i++)
	{
		delete m_lines[i];
	}
	m_lines.clear();
}

void CMultiLine::handleBuffer(const char* buf,int iLen)
{
	if(iLen <= 0)
		return;

	char* pBuf  = (char*)buf;
	char* pNext = pBuf;

	CStringLine * line = new CStringLine(m_dwColor,m_dwBack,m_iFontType,m_iFontSize,m_dwFlag);
	m_lines.push_back(line); //加入一行

	int i = 0;
	while(i < iLen)
	{
		char c = *pBuf; //当前字符

		if(c == 0x01)
		{
			int iBufSize = (unsigned char)(pBuf[1]);
			pNext = pBuf+iBufSize+2;

			if(pNext <= buf + iLen)
			{
				CObjectLink * link = new CObjectLink(pBuf+2,iBufSize);
				if(strlen(link->getGood()->GetName()) > 0)//当名称为空时，说明数据有错
				{
					if(m_iLimit>0 && line->size()+link->size() > m_iLimit) //切字符，生成新的串
					{
						line = new CStringLine(m_dwColor,m_dwBack,m_iFontType,m_iFontSize,m_dwFlag);
						m_lines.push_back(line);
					}
					line->insert(link);
				}
				else
				{
					SAFE_DELETE(link);
				}
			}
		}
		else if((c == '\n' || c == '\r' || c == '\\') && !m_bIgnoreewLine) //回车换行直接生成新的一行
		{
			pNext = CharNext(pBuf);
			if(pNext == pBuf) //已经到文字尾部，结束了
				break;

			if(line->size() > 0) //原来是空行，不再重新加入了，也就是连着的'\n'等只换一次行
			{
				line = new CStringLine(m_dwColor,m_dwBack,m_iFontType,m_iFontSize,m_dwFlag);
				m_lines.push_back(line);
			}
		}
		else
		{
			char* pEnd = NULL;

			//对<生><元神分身>等字符不作为PTLink，即第一个字符是<的情况.
			//对于TALKTYPE_SERVER没有限制
			if(c == '<' && (m_dwType == TALKTYPE_SERVER || (m_dwType == TALKTYPE_SYSTEM &&  pBuf != buf && (m_dwColor != 0xFFFFFFFF || m_dwBack != 0xFFEF6B00)    ) ) ) //m_dwColor == 0xFFFFFFFF && m_dwBack == 0xffef6b00 是传音号角,防止玩家用号角刷
			{
				pEnd = strchr(pBuf,'>'); //查找到对应的>符号

				char* pNew = strchr(pBuf+1,'<');
				if(pNew && pNew < pEnd)  //有不匹配的<>字符,直接输出
				{
					pEnd = NULL;
				}
				else
				{
					pNew = strchr(pBuf+1,'@');
					if(pNew && pNew >= pEnd)  //没有@字符,直接输出
						pEnd = NULL;
				}

				if(pEnd)
				{
					CPTLink* pt = new CPTLink(pBuf,(int)(pEnd - pBuf)+1);

					if(m_iLimit>0 && line->size()+pt->size() > m_iLimit) //切字符，生成新的串
					{
						line = new CStringLine(m_dwColor,m_dwBack,m_iFontType,m_iFontSize,m_dwFlag);
						m_lines.push_back(line);
					}
					line->insert(pt);
					pNext = pEnd + 1;
				}
			}

			if(pEnd == NULL) //没有处理<>表示的特殊含义的内容
			{
				pNext = CharNext(pBuf);
				if(pNext == pBuf) //已经到文字尾部，结束了
					break;

				CChar* ch = new CChar(pBuf,(int)(pNext-pBuf));

				if(m_iLimit>0 && line->size()+ch->size()>m_iLimit) //切字符
				{
					line = new CStringLine(m_dwColor,m_dwBack,m_iFontType,m_iFontSize,m_dwFlag);
					m_lines.push_back(line);
				}
				line->insert(ch);
			}
		}

		i += (int)(pNext-pBuf);
		if(i <= 0)
		{
			break;
		}

		pBuf = pNext;
	}
}

void CMultiLine::clean(const char* str)
{
	//复制数据
	string tmpStr = str;
	CStringLine* line = NULL;
	int pos = 0;

	for(size_t i = 0;i < m_lines.size();i++)
	{
		line = m_lines[i];
		if(line == NULL)
			continue;

		int _size = (int)line->size();

		if(pos + _size > (int)tmpStr.size()) //超出长度了，结束
			break;

		line->clean(tmpStr.substr(pos,_size).c_str());
		pos += _size;
	}
}

const char* CMultiLine::c_str()
{
	m_show.clear();

	CStringLine* line = NULL;

	for(size_t i = 0;i < m_lines.size();i++)
	{
		line = m_lines[i];
		if(line == NULL)
			continue;

		m_show += line->c_str();
	}
	return m_show.c_str();
}

