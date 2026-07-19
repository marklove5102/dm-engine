#pragma once

#include "Global/Global.h"
#include "BaseClass/TiXml/xmlobj.h"

//一些UI中用到的基本数据结构的定义

//注：控件之间的继承只能在同类控件间继承，比如Button只能从Button控件继承，
//    不能从Control控件继承，虽然在schema中Button是从Control扩展而来
//    先从XML文件中解析得到窗口的元数据,只有真的接收到了创建窗口的消息才实际创建

//schema中定义好的所有的合法元素
enum XML_VALID_ELEMENT
{
	XVE_UNKONWN  = 0           ,//非法
	XVE_BASE = 1			   ,//所有元素的基类
	XVE_OFFSET                 ,//OffSet
	XVE_COLOR                  ,//Color
	XVE_INSET	               ,//Inset
	XVE_SIZE                   ,//Size
	XVE_TEXTURE                ,//Texture
	XVE_FONT                   ,//Font
	XVE_FONTSTRING             ,//FontString
	XVE_SCRIPTS                ,//Scripts
	XVE_XCONTROL               ,//XControl
	XVE_XPAGE                  ,//XPage
	XVE_XWINDOW                ,//XWindow
	XVE_XSTATIC                ,//XStatic
	XVE_XBUTTON                ,//XButton
	XVE_XLABEL                 ,//XLabel
	XVE_XRADIO				   ,//XRadio
	XVE_XRADIOGROUP            ,//XRadioGroup
	XVE_XEDIT                  ,//XEdit
	XVE_XAREA                  ,//XArea
	XVE_XTIP                   ,//XTip
	XVE_XCANVAS                ,//XCanvas
	XVE_XSCROLL                ,//XScroll
	XVE_XMENU                  ,//XMenu
	XVE_XCOMBOBOX              ,//XComboBox
    XVE_XGOODGRID               ,//XGoodGrid
	XVE_XUI                    ,//XUI
   

	XVE_NUMS,                    //总数
};

//ALINEMENT 对齐方式
enum XALINEMENT
{
	XAL_NONE = -1,
	XAL_TOPLEFT = 0,
	XAL_TOP,
	XAL_TOPRIGHT,
	XAL_RIGHT,
	XAL_BOTTOMRIGHT,
	XAL_BOTTOM,
	XAL_BOTTOMLEFT,
	XAL_LEFT,
	XAL_CENTER,
	XAL_NUMS,   //总数
};

enum XML_SCRIPT_EVENT
{
	XSE_OnLoad = 0,
	XSE_OnSizeChanged,
	XSE_OnEvent,
	XSE_OnUpdate,
	XSE_OnClose,
	XSE_OnEnter,
	XSE_OnLeave,
	XSE_OnMouseDown,
	XSE_OnMouseUp,
	XSE_OnMouseWheel,
	XSE_OnMouseMove,
	XSE_OnDragStart,
	XSE_OnDragStop,
	XSE_OnReceiveDrag,
	XSE_OnClick,
	XSE_OnDoubleClick,
	XSE_OnEnterPressed,
	XSE_OnEscapePressed,
	XSE_OnSpacePressed,
	XSE_OnTabPressed,
	XSE_OnTextChanged,
	XSE_OnCursorChanged,
	XSE_OnEditFocusGained,
	XSE_OnEditFocusLost,
	XSE_OnHorizontalScroll,
	XSE_OnVerticalScroll,
	XSE_OnScrollRangeChanged,
	XSE_OnChar,
	XSE_OnKeyDown,
	XSE_OnKeyUp,
	XSE_NUMS
};

enum E_BUTTON_TEX
{
	BTEX_NONE   = -1,
	BTEX_NORMAL = 0,
	BTEX_HIGHLIGHT,
	BTEX_PUSHED,
	BTEX_DISABLED,
	BTEX_EFFECT,
	BTEX_NUMS
};


//颜色类型
class CXmlColor : public CXmlObj
{
	DERIVE(CXmlColor, CXmlObj)
public:
	DWORD GetColor();
protected:
	DECLARE_VAR(string,Value);
};

//字体
class CXmlFont : public CXmlObj
{
	DERIVE(CXmlFont, CXmlObj)
protected:
	DECLARE_VAR(string, Name)
	DECLARE_VAR(int, Size)
};

//偏移
class CXmlOffset : public CXmlObj
{
	DERIVE(CXmlOffset,CXmlObj)
protected:
	DECLARE_VAR(int, X)
	DECLARE_VAR(int, Y)
	DECLARE_VAR(string, Align)	     //对齐方式
};

//大小
class CXmlSize : public CXmlObj
{
	DERIVE(CXmlSize, CXmlObj)
protected:
	DECLARE_VAR(int, Width)
	DECLARE_VAR(int, Height)
};

//矩形框
class CXmlInset : public CXmlObj
{
	DERIVE(CXmlInset,CXmlObj)
protected:
	DECLARE_VAR(int, Left)
	DECLARE_VAR(int, Top)
	DECLARE_VAR(int, Right)
	DECLARE_VAR(int, Bottom)
};

//纹理
class CXmlTexture : public CXmlObj
{
	DERIVE(CXmlTexture,CXmlObj)
public:
	virtual void Clear();
	virtual bool InitChildElement(const char* pValue);

	void GetOffset(int& x,int& y);
protected:
	DECLARE_VAR(int,Index)
	CXmlOffset m_xmlOffset;
	CXmlColor  m_xmlColor;
};


//单行文本
class CXmlFontString : public CXmlObj
{
	DERIVE(CXmlFontString, CXmlObj)
public:
	virtual void Clear();
	virtual bool InitChildElement(const char* pValue);
    bool IsCenter();
    CXmlColor& GetXmlColor(){ return m_xmlColor; }
	CXmlFont& GetXmlFont(){ return m_xmlFont; }
protected:
	DECLARE_VAR(string,Text);
    DECLARE_VAR(int,Center);
	CXmlColor m_xmlColor;    
	CXmlFont m_xmlFont;
};

//事件脚本单项
class CXmlScriptEntry : public CXmlObj
{
	DERIVE(CXmlScriptEntry,CXmlObj)
public:
	virtual void Clear();
	virtual bool InitChildText(const char* pText);

	string  m_sScript; //脚本
};

//事件脚本列表
class CXmlScripts : public CXmlObj
{
	DERIVE(CXmlScripts,CXmlObj)
public:
	virtual void Clear();
	virtual bool InitChildElement(const char* pValue);

	string&  GetScript(int i){ return m_xmlScript[i].m_sScript; }
protected:
	CXmlScriptEntry m_xmlScript[XSE_NUMS];
};

//控件对应的脚本文件
class CXmlScriptFile : public CXmlObj
{
    DERIVE(CXmlScriptFile,CXmlObj)
protected:
    DECLARE_VAR(string,File);       //脚本文件名    
};