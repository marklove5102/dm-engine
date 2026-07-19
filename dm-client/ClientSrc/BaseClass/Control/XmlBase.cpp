#include "XmlBase.h"
#include <assert.h>

//颜色
BEGIN_VAR(CXmlColor, CXmlObj)
INIT_VAR(string, Value, "")
END_VAR(CXmlColor, CXmlObj)

DWORD CXmlColor::GetColor()
{
	string str = this->GetValue();
	DWORD color = 0xFFFFFFFF;
	if(str.empty())
		return color;

	if(stricmp(str.c_str(),"red") == 0)
		color = 0xFFFF0000;
	else if(stricmp(str.c_str(),"green") == 0)
		color = 0xFF00FF00;
	else if(stricmp(str.c_str(),"ggreen") == 0)
		color = 0xFF87C23A;
	else if(stricmp(str.c_str(),"blue") == 0)
		color = 0xFF0000FF;
	else if(stricmp(str.c_str(),"black") == 0)
		color = 0xFF000000;
	else if(stricmp(str.c_str(),"yellow") == 0)
		color = 0xFFFFFF00;
	else if(stricmp(str.c_str(),"orange") == 0)
		color = 0xFFFF8000;
	else if(stricmp(str.c_str(),"purple") == 0)
		color = 0xFFFF00FF;
	else if(stricmp(str.c_str(),"gray") == 0)
		color = 0xFFC0C0C0;
	else if(stricmp(str.c_str(),"white") == 0)
		color = 0xFFFFFFFF;
	else
	{
		color = strtoul(str.c_str(),NULL,16);
		color |= 0xFF000000;
	}
	return color;
}

//字体
BEGIN_VAR(CXmlFont, CXmlObj)
INIT_VAR(string, Name,"")
INIT_VAR(int, Size, 12)
END_VAR(CXmlFont, CXmlObj)

//偏移
BEGIN_VAR(CXmlOffset, CXmlObj)
INIT_VAR(int, X, 0)
INIT_VAR(int, Y, 0)
INIT_VAR(string, Align, "")
END_VAR(CXmlOffset, CXmlObj)

//大小
BEGIN_VAR(CXmlSize, CXmlObj)
INIT_VAR(int, Width, 0)
INIT_VAR(int, Height, 0)
END_VAR(CXmlSize, CXmlObj)

//矩形框
BEGIN_VAR(CXmlInset,CXmlObj)
INIT_VAR(int, Left,0)
INIT_VAR(int, Top, 0)
INIT_VAR(int, Right, 0)
INIT_VAR(int, Bottom, 0)
END_VAR(CXmlInset,CXmlObj)

//纹理
BEGIN_VAR(CXmlTexture,CXmlObj)
INIT_VAR(int,Index,0)
END_VAR(CXmlTexture,CXmlObj)

void CXmlTexture::GetOffset(int& x,int& y)
{
	x = m_xmlOffset.GetX();
	y = m_xmlOffset.GetY();
}

void CXmlTexture::Clear()
{
	SUPERCLASS::Clear();
	m_xmlColor.Clear();
	SetIndex(0);
}

bool CXmlTexture::InitChildElement(const char* pValue)
{
	if(SUPERCLASS::InitChildElement(pValue))
		return true;

	assert(pValue);
	if(stricmp(pValue,"Color") == 0)
	{
		m_xmlColor.Create();
		return true;
	}
	else if(stricmp(pValue,"Offset") == 0)
	{
		m_xmlOffset.Create();
		return true;
	}
	return false;
}

//单行文本
BEGIN_VAR(CXmlFontString, CXmlObj)
INIT_VAR(string,Text,"")
INIT_VAR(int,Center,1)
END_VAR(CXmlFontString,CXmlObj)

void CXmlFontString::Clear()
{
	SUPERCLASS::Clear();
	m_xmlColor.Clear();
}

bool CXmlFontString::InitChildElement(const char* pValue)
{
	if(SUPERCLASS::InitChildElement(pValue))
		return true;

	assert(pValue);
	if(stricmp(pValue,"Color") == 0)
	{
		m_xmlColor.Create();
		return true;
	}
	else if(stricmp(pValue,"Font") == 0)
	{
		m_xmlFont.Create();
		return true;
	}

	return false;
}

//事件脚本单项
BEGIN_VAR(CXmlScriptEntry,CXmlObj)
END_VAR(CXmlScriptEntry,CXmlObj)

void CXmlScriptEntry::Clear()
{
	SUPERCLASS::Clear();
	m_sScript.clear();
}

bool CXmlScriptEntry::InitChildText(const char* pText)
{
	assert(pText);
	m_sScript = pText;
	return true;
}

//事件脚本列表
BEGIN_VAR(CXmlScripts,CXmlObj)
END_VAR(CXmlScripts,CXmlObj)

void CXmlScripts::Clear()
{
	SUPERCLASS::Clear();
	for(int ii = 0;ii < XSE_NUMS; ii++)
	{
		m_xmlScript[ii].Clear();
	}
}

bool CXmlScripts::InitChildElement(const char* pValue)
{
	if(SUPERCLASS::InitChildElement(pValue))
		return true;

	assert(pValue);

	static char* SCRIPT_STR[] = 
	{
		"OnLoad",
		"OnSizeChanged",
		"OnEvent",
		"OnUpdate",
		"OnClose",
		"OnEnter",
		"OnLeave",
		"OnMouseDown",
		"OnMouseUp",
		"OnMouseWheel",
		"OnMouseMove",
		"OnDragStart",
		"OnDragStop",
		"OnReceiveDrag",
		"OnClick",
		"OnDoubleClick",
		"OnEnterPressed",
		"OnEscapePressed",
		"OnSpacePressed",
		"OnTabPressed",
		"OnTextChanged",
		"OnCursorChanged",
		"OnEditFocusGained",
		"OnEditFocusLost",
		"OnHorizontalScroll",
		"OnVerticalScroll",
		"OnScrollRangeChanged",
		"OnChar",
		"OnKeyDown",
		"OnKeyUp",
	};

	for(int ii = 0; ii < XSE_NUMS;ii++)
	{
		if(stricmp(pValue,SCRIPT_STR[ii]) == 0)
		{
			m_xmlScript[ii].Create();
			return true;
		}
	}
	return false;
}

//单行文本
BEGIN_VAR(CXmlScriptFile, CXmlObj)
INIT_VAR(string,File,"")
END_VAR(CXmlScriptFile,CXmlObj)

