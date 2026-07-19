#pragma once

#include "Global/Global.h"
#include "../Good.h"

class CGlyph
{
public:
	CGlyph();
	virtual ~CGlyph();

	int size()     { return (int)m_show.size();  }  // 字节数
	const char * c_str(){ return m_show.c_str();}	// 显示用

	const char * getBuf(){ return m_buffer.c_str();}	// 网络传输封装用
	int getBufLength(){ return (int)m_buffer.size(); }

	virtual CGood* getGood() {  return NULL; }
	virtual bool NeedDraw(DWORD&,DWORD,bool&){return false;}
	virtual CGlyph* clone(){ return NULL; }
	virtual const char* getCommand(){ return NULL; }

protected:
	std::string  m_show; //要显示的文字
	std::string  m_buffer; //内部数据
};

class CChar : public CGlyph
{
public:
	CChar(const char * str,int iLen);
	virtual ~CChar();
	virtual CGlyph* clone();
	virtual bool NeedDraw(DWORD&,DWORD,bool&){ return false; }
};

class CObjectLink :	public CGlyph
{
public:
	CObjectLink(const char* buffer,int iLen);
	CObjectLink(CGood& good); //
	virtual ~CObjectLink();
	virtual CGlyph* clone();

	virtual bool NeedDraw(DWORD&,DWORD,bool&);
	virtual CGood* getGood();
private:
	CGood   m_good;
};


class CPTLink : public CGlyph
{
public:
	CPTLink(const char * str,int iLen);
	virtual ~CPTLink();
	virtual CGlyph* clone();

	virtual bool NeedDraw(DWORD&,DWORD,bool&);
	virtual const char* getCommand();
private:
	std::string m_command; //命令
	DWORD m_dwColor;
};