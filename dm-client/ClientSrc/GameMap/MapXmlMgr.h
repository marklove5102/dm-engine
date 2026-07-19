#pragma once

#include "Global/Global.h"
#include "BaseClass/TiXml/xmlobj.h"
#include <vector>

class CMapXmlEntry : public CXmlObj
{
	DERIVE(CMapXmlEntry,CXmlObj)
public:
	DECLARE_VAR(string,Src);
	DECLARE_VAR(string,Dst);
	DECLARE_VAR(string,MinMap);
};

class CMapXmlMgr : public CXmlObj
{
	DERIVE(CMapXmlEntry,CXmlObj)
public:
	CMapXmlMgr(void);
	~CMapXmlMgr(void);
	virtual bool InitChildElement(const char* pValue);
	const char * GetRealMap(const char * strMapName);
	const char * GetLocalMap(const char * strMapName);

private:
	vector<CMapXmlEntry> m_map;
};

extern CMapXmlMgr g_MapXmlMgr;
