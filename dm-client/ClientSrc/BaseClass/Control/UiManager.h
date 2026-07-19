#pragma once

#include "XmlBase.h"
#include "XmlControl.h"
#include "Global/StringUtil.h"


class CCtrlWindowX;
//------------------------------//
//			窗口名字和路径		//
//------------------------------//
class CXmlUiPath  : public CXmlObj
{
	DERIVE(CXmlUiPath,CXmlObj)
protected:
	DECLARE_VAR(string, Path)
};

class CXmlUiEntry : public CXmlObj
{
	DERIVE(CXmlUiEntry,CXmlObj)
public:
	virtual void Clear();
	virtual bool InitChildElement(const char* pValue);
	void         GetAllPath(VString& path);
protected:
	DECLARE_VAR(string, Name)
	DECLARE_VAR(string, Path)
	vector<CXmlUiPath>  m_xmlPaths;
};

//------------------------------//
//			UI管理器			//
//------------------------------//
class CUiManager : public CXmlObj
{
	DERIVE(CUiManager, CXmlObj)
	typedef std::map<string, CXmlWindow*> MXmlWindow;
	typedef std::map<string, CXmlUiEntry> MXmlUiEntry;

public:
	CUiManager();
	~CUiManager();
	virtual bool LoadFile(const char*);
	virtual void Clear();
	virtual bool InitChildElement(const char* pValue);

	CXmlWindow*	 GetXmlWindow(const string& name,int iPos = 0); //获得Xml窗口对象
protected:
	MXmlWindow  m_wins;//所有从xml文件解析过来的xmlwindow，启动时导入全部数据
	MXmlUiEntry m_entrys;
};