#pragma once

#include "XmlBase.h"

class CControl;

//-----------------------------------------------------------------------------
//基本控件
class CXmlControl : public CXmlObj
{
	DERIVE(CXmlControl,CXmlObj)
public:
	CXmlControl();
    virtual ~CXmlControl(){}
	virtual void Clear();
	virtual bool InitChildText(const char* pText);
	virtual bool InitChildElement(const char* pValue);

	//函数
	XML_VALID_ELEMENT GetXmlMode(){ return m_xmlMode; }
	void GetOffset(int& x,int& y);
	const string& GetAlign(){ return m_xmlOffset.GetAlign(); }
	void GetSize(int& w,int & h);
	string& GetScript(int i){ return m_xmlScripts.GetScript(i); }
    
	CXmlTexture& GetBackTexture(){ return m_xmlBackTexure;}   //获得背景图片
	const char*  GetTips(){ return m_xmlTipFont.GetText().c_str(); }

protected:
	DECLARE_VAR(string, Name)        //名字
	DECLARE_VAR(string, Inherits)	 //继承的对象名
	DECLARE_VAR(int,    Hidden)      //是否隐藏
	DECLARE_VAR(int,    Mask)        //掩码编号

	CXmlTexture m_xmlBackTexure;     //背景
	CXmlOffset  m_xmlOffset;         //相对父控件的位置
	CXmlSize    m_xmlSize;           //大小
	CXmlScripts m_xmlScripts;        //对应的脚本
	CXmlFontString m_xmlTipFont;     //TIPS
	string      m_sComment;          //注释
	XML_VALID_ELEMENT   m_xmlMode;   //Xml节点类型
};

typedef vector<CXmlControl*> VXmlControl; //Xml控件列表

//-----------------------------------------------------------------------------
//按钮控件
class CXmlButton : public CXmlControl
{
	DERIVE(CXmlButton, CXmlControl)
public:
	CXmlButton();
	virtual void Clear();
	virtual bool InitChildElement(const char* pValue);

	CXmlTexture& GetTexture(int i){ return m_xmlTextures[i];}
	CXmlFontString& GetFontString(){return m_xmlButtonText;}

protected:
	DECLARE_VAR(bool,Switch)
	DECLARE_VAR(int,Index)
	CXmlTexture    m_xmlTextures[BTEX_NUMS];
    CXmlFontString m_xmlButtonText;

};
//-----------------------------------------------------------------------------
//Label控件
class CXmlLabel : public CXmlControl
{
	DERIVE(CXmlLabel, CXmlControl)
public:
	CXmlLabel();
	virtual void Clear();
	virtual bool InitChildElement(const char* pValue);

	CXmlTexture& GetTexture(int i){ return m_xmlTextures[i];}
	CXmlFontString& GetFontString(){return m_xmlLabelText;}

protected:
	DECLARE_VAR(int,Index)
	CXmlTexture    m_xmlTextures[BTEX_NUMS];
	CXmlFontString m_xmlLabelText;

};
//-----------------------------------------------------------------------------
//单选按钮
class CXmlRadio : public CXmlControl
{
	DERIVE(CXmlRadio, CXmlControl)
public:
	CXmlRadio();
	virtual void Clear();
	virtual bool InitChildElement(const char* pValue);

	enum E_RADIO_TEX
	{
		RTEX_NORMAL = 0,
		RTEX_CHECKED,
		RTEX_NUMS
	};

	CXmlTexture& GetTexture(int i){ return m_xmlTextures[i];}
protected:
	DECLARE_VAR(int,Checked)
	CXmlTexture    m_xmlTextures[RTEX_NUMS];
};

typedef vector<CXmlRadio*> VXmlRadio;

//-----------------------------------------------------------------------------
//单选按钮组
class CXmlRadioGroup : public CXmlControl
{
	DERIVE(CXmlRadioGroup, CXmlControl)
public:
	CXmlRadioGroup();
	virtual void Clear();
	virtual bool InitChildElement(const char* pValue);

	inline VXmlRadio& GetRadios(){ return m_xmlRadios; }
protected:
	VXmlRadio m_xmlRadios;
};

//-----------------------------------------------------------------------------
//属性页面
class CXmlPage : public CXmlControl
{
	DERIVE(CXmlPage, CXmlControl)
public:
	CXmlPage();
	virtual void Clear();
	virtual bool InitChildElement(const char* pValue);
	VXmlControl& GetChildren(){ return m_xmlCtrls; }
	CXmlControl* FindChild(const char* name);
protected:
	VXmlControl  m_xmlCtrls; //控件

};

typedef vector<CXmlPage*> VXmlPage;

//-----------------------------------------------------------------------------
//窗口控件
class CXmlWindow : public CXmlPage
{
	DERIVE(CXmlWindow,CXmlPage)
public:
	CXmlWindow();
	virtual ~CXmlWindow();
	virtual void Clear();
	virtual bool InitChildElement(const char* pValue);
	VXmlPage& GetPages(){  return m_xmlPages; }
	CXmlWindow* GetSibling(){ return m_pSibling; }
	void AddSibling(CXmlWindow* win);

	const string & GetScriptFileContent();

protected:
	DECLARE_VAR(bool, NoMovable)
	DECLARE_VAR(bool, NoChangeLevel)
	DECLARE_VAR(bool, NoFocus)
	DECLARE_VAR(bool, SavePos)//是否保存上一次窗口的位置

	VXmlPage  m_xmlPages; //属性页列表
	CXmlWindow* m_pSibling; //兄弟窗口

	CXmlScriptFile m_xmlScriptFile;
	string m_strScriptFileContent;
};

//-----------------------------------------------------------------------------
//编辑框
class CXmlEdit : public CXmlControl
{
	DERIVE(CXmlEdit,CXmlControl)
public:
	CXmlEdit();
	virtual void Clear();
	virtual bool InitChildElement(const char* pValue);
    CXmlColor& GetXmlColor(){ return m_xmlColor; }
protected:
    CXmlColor m_xmlColor;
    
};

//-----------------------------------------------------------------------------
//滚动条
class CXmlScroll : public CXmlControl
{
	DERIVE(CXmlScroll,CXmlControl)
public:
	CXmlScroll();
	virtual void Clear();
	virtual bool InitChildElement(const char* pValue);
	CXmlTexture& GetScroll(){ return m_xmlScroll; }
protected:
	CXmlTexture  m_xmlScroll;
};

//Menu Item
class CXmlMenuItem :public CXmlControl
{
    DERIVE(CXmlMenuItem,CXmlControl)
public:
    CXmlMenuItem();
    virtual void Clear();
    virtual bool InitChildElement(const char* pText);
protected:
    DECLARE_VAR(string, Text)
    DECLARE_VAR(string, Tip)
};

//-----------------------------------------------------------------------------
//菜单
typedef vector<CXmlMenuItem> VXmlMenuItems;

class CXmlMenu : public CXmlControl
{
	DERIVE(CXmlMenu,CXmlControl)
public:
	CXmlMenu();
	virtual void Clear();
	virtual bool InitChildElement(const char* pValue);
    VXmlMenuItems& GetXmlMenuItems(){ return m_xmlMenuItems; }
protected:
    VXmlMenuItems m_xmlMenuItems;
};

//-----------------------------------------------------------------------------
//TIPS控件
typedef vector<CXmlFontString> VXmlFontString;

class CXmlTip : public CXmlControl
{
	DERIVE(CXmlTip,CXmlControl)
public:
	CXmlTip();
    ~CXmlTip();
	virtual void Clear();
	virtual bool InitChildElement(const char* pValue);
    VXmlFontString& GetLines(){ return m_xmlLines; }
protected:
    DECLARE_VAR(int, Center)
    DECLARE_VAR(int, Border)
    VXmlFontString	 m_xmlLines;
};



