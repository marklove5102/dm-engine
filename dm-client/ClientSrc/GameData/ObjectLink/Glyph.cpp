#include "glyph.h"

CGlyph::CGlyph()
{
	m_show.clear();
	m_buffer.clear();
}

CGlyph::~CGlyph(void)
{
}


CChar::CChar(const char * str,int iLen)
{
	m_show.assign(str,iLen);
	m_buffer = m_show;
}

CChar::~CChar()
{

}

CGlyph* CChar::clone()
{
	return new CChar(m_buffer.c_str(),m_buffer.size());
}

//对象链接
CObjectLink::CObjectLink(const char* buffer,int iLen)
{
	if(iLen <= 0 || iLen > 255)
		return;//有时会出现负值

	m_good.handleBuffer((char*)buffer,iLen);
	m_show = "[";
	m_show += m_good.GetName();
	m_show += "]";

	char temp[256] = {0};
	temp[0] = 0x01;
	temp[1] = (char)iLen;
	memcpy(temp+2,buffer,iLen);
	m_buffer.assign(buffer,iLen);
}

CObjectLink::CObjectLink(CGood& good)
{
	m_good = good;
	m_show = "[";
	m_show.append(good.GetName());
	m_show.append("]");

	char temp[1024] = {0};
	temp[0] = 0x01;
	unsigned int i = 256;
	m_good.getBuffer(temp + 2,i);
	temp[1] = i;
	m_buffer.assign(temp,i + 2);
}

CObjectLink::~CObjectLink()
{

}

CGlyph* CObjectLink::clone()
{
	return new CObjectLink(m_good);
}

CGood* CObjectLink::getGood()
{
	return &m_good;
}

bool CObjectLink::NeedDraw(DWORD& front,DWORD back,bool& bUnderLine)
{
	if(back == 0xFFFFFFFF)
		front = 0xFF006000;
	else if(back == 0xFFF7E700)
		front = 0xFF0000FF;
	else
		front = 0xFF00FF00;

	bUnderLine = true;

	return true;
}

//PT链接

//PT链接
CPTLink::CPTLink(const char * str,int iLen)
{
	m_buffer.assign(str,iLen);
	int pos = m_buffer.find_first_of('/');
	if(pos >= 0)
	{
		m_show = m_buffer.substr(1,pos-1);
		m_command = m_buffer.substr(pos+1,m_buffer.size()-pos-2);
	}
	else
	{
		m_show = m_buffer.substr(1,m_buffer.size()-2);
	}
}

CPTLink::~CPTLink()
{

}

CGlyph* CPTLink::clone()
{
	return new CPTLink(m_buffer.c_str(),m_buffer.size());
}

bool CPTLink::NeedDraw(DWORD& front,DWORD back,bool& bUnderLine)
{
	if(back == 0xFFFFFFFF)
		front = 0x00006000 | (front & 0xFF000000);
	else if(back == 0xFFF7E700)
		front = 0x000000FF | (front & 0xFF000000);
	else
		front = 0x0000FF00 | (front & 0xFF000000);

	bUnderLine = true;
	return true;
}

const char* CPTLink::getCommand()
{
	return m_command.c_str();
}