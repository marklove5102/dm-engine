#include "XmlControl.h"
#include "Control.h"
#include "XmlUser.h"
#include "Global/Interface/StreamInterface.h"
#include "Global/StringUtil.h"

//-----------------------------------------------------------------------------
//基本控件
BEGIN_VAR(CXmlControl, CXmlObj)
INIT_VAR(string, Name,"")
INIT_VAR(string, Inherits,"")	 //继承的对象名
INIT_VAR(int,    Hidden, 0)      //是否隐藏
INIT_VAR(int,    Mask,0)        //掩码编号
END_VAR(CXmlControl, CXmlObj)

CXmlControl::CXmlControl()
{
	m_xmlMode = XVE_XCONTROL;
}

void CXmlControl::Clear()
{
	SUPERCLASS::Clear();
	m_xmlBackTexure.Clear();
	m_xmlOffset.Clear();
	m_xmlSize.Clear();
	m_xmlScripts.Clear();
	m_sComment.clear();
	m_xmlTipFont.Clear();
}

bool CXmlControl::InitChildText(const char* pText)
{
	if(SUPERCLASS::InitChildText(pText))
		return true;
	assert(pText);
	m_sComment	= pText;
	return true;
}

bool CXmlControl::InitChildElement(const char* pValue)
{
	if(SUPERCLASS::InitChildElement(pValue))
		return true;

	assert(pValue);
	if(stricmp(pValue,"BackTexture") == 0)
	{
		m_xmlBackTexure.Create();
		return true;
	}
	if(stricmp(pValue, "Offset") == 0)
	{
		m_xmlOffset.Create();
		return true;
	}
	else if(stricmp(pValue,"Size") == 0)
	{
		m_xmlSize.Create();
		return true;
	}
	else if(stricmp(pValue,"Tips") == 0)
	{
		m_xmlTipFont.Create();
		return true;
	}
	else if(stricmp(pValue,"Scripts") == 0)
	{
		m_xmlScripts.Create();
		return true;
	}

	return false;
}

void CXmlControl::GetOffset(int& x,int& y)
{
	x = m_xmlOffset.GetX();
	y = m_xmlOffset.GetY();
}

void CXmlControl::GetSize(int& w,int & h)
{
	w = m_xmlSize.GetWidth();
	h = m_xmlSize.GetHeight();
}

//-----------------------------------------------------------------------------
//按钮控件
BEGIN_VAR(CXmlButton, CXmlControl)
INIT_VAR(bool,Switch,false)
INIT_VAR(int,Index,0)
END_VAR(CXmlButton, CXmlControl)

CXmlButton::CXmlButton()
{
	m_xmlMode = XVE_XBUTTON;
	Clear();
}

void CXmlButton::Clear()
{
	SUPERCLASS::Clear();

	for(int ii = 0;ii < BTEX_NUMS;ii++)
	{
		m_xmlTextures[ii].Clear();
	}
	m_xmlTipFont.Clear();
}

bool CXmlButton::InitChildElement(const char* pValue)
{
	if(SUPERCLASS::InitChildElement(pValue))
		return true;

	assert(pValue);
	if(stricmp(pValue,"NormalTexture") == 0)
	{
		m_xmlTextures[BTEX_NORMAL].Create();
		return true;
	}
	else if(stricmp(pValue,"HighlightTexture") == 0)
	{
		m_xmlTextures[BTEX_HIGHLIGHT].Create();
		return true;
	}
	else if(stricmp(pValue,"PushedTexture") == 0)
	{
		m_xmlTextures[BTEX_PUSHED].Create();
		return true;
	}
	else if(stricmp(pValue,"DisableTexture") == 0)
	{
		m_xmlTextures[BTEX_DISABLED].Create();
		return true;
	}
    else if(stricmp(pValue,"EffectTexture") == 0)
    {
        m_xmlTextures[BTEX_EFFECT].Create();
        return true;
    }
    else if(stricmp(pValue,"ButtonText") == 0)
    {
        m_xmlButtonText.Create();
        return true;
    }

	return false;
}

//-----------------------------------------------------------------------------
//Label控件
BEGIN_VAR(CXmlLabel, CXmlControl)
INIT_VAR(int,Index,0)
END_VAR(CXmlLabel, CXmlControl)

CXmlLabel::CXmlLabel()
{
	m_xmlMode = XVE_XLABEL;
	Clear();
}

void CXmlLabel::Clear()
{
	SUPERCLASS::Clear();

	for(int ii = 0;ii < BTEX_NUMS;ii++)
	{
		m_xmlTextures[ii].Clear();
	}
	m_xmlTipFont.Clear();
}

bool CXmlLabel::InitChildElement(const char* pValue)
{
	if(SUPERCLASS::InitChildElement(pValue))
		return true;

	assert(pValue);
	if(stricmp(pValue,"NormalTexture") == 0)
	{
		m_xmlTextures[BTEX_NORMAL].Create();
		return true;
	}
	else if(stricmp(pValue,"HighlightTexture") == 0)
	{
		m_xmlTextures[BTEX_HIGHLIGHT].Create();
		return true;
	}
	else if(stricmp(pValue,"PushedTexture") == 0)
	{
		m_xmlTextures[BTEX_PUSHED].Create();
		return true;
	}
	else if(stricmp(pValue,"DisableTexture") == 0)
	{
		m_xmlTextures[BTEX_DISABLED].Create();
		return true;
	}
	else if(stricmp(pValue,"EffectTexture") == 0)
	{
		m_xmlTextures[BTEX_EFFECT].Create();
		return true;
	}
	else if(stricmp(pValue,"LabelText") == 0)
	{
		m_xmlLabelText.Create();
		return true;
	}

	return false;
}

//-----------------------------------------------------------------------------
//单选按钮
BEGIN_VAR(CXmlRadio,CXmlControl)
INIT_VAR(int,Checked,0)
END_VAR(CXmlRadio,CXmlControl)

CXmlRadio::CXmlRadio()
{
	m_xmlMode = XVE_XRADIO;
	Clear();
}

void CXmlRadio::Clear()
{
	SUPERCLASS::Clear();

	for(int ii = 0;ii < RTEX_NUMS;ii++)
	{
		m_xmlTextures[ii].Clear();
	}
}

bool CXmlRadio::InitChildElement(const char* pValue)
{
	if(SUPERCLASS::InitChildElement(pValue))
		return true;

	assert(pValue);
	if(stricmp(pValue,"NormalTexture") == 0)
	{
		m_xmlTextures[RTEX_NORMAL].Create();
		return true;
	}
	else if(stricmp(pValue,"CheckedTexture") == 0)
	{
		m_xmlTextures[RTEX_CHECKED].Create();
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
//单选按钮组
BEGIN_VAR(CXmlRadioGroup,CXmlControl)
END_VAR(CXmlRadioGroup,CXmlControl)

CXmlRadioGroup::CXmlRadioGroup()
{
	m_xmlMode = XVE_XRADIOGROUP;
	Clear();
}

void CXmlRadioGroup::Clear()
{
	SUPERCLASS::Clear();

	while(!m_xmlRadios.empty())
	{
		CXmlRadio* radio = m_xmlRadios.back();
		radio->Clear();
		SAFE_DELETE(radio);
		m_xmlRadios.pop_back();
	}
}

bool CXmlRadioGroup::InitChildElement(const char* pValue)
{
	if(SUPERCLASS::InitChildElement(pValue))
		return true;

	assert(pValue);
	if(stricmp(pValue,"Radio") == 0)
	{
		CXmlRadio* radio = new CXmlRadio();
		radio->Create();
		m_xmlRadios.push_back(radio);
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
//属性页面
BEGIN_VAR(CXmlPage,CXmlControl)
END_VAR(CXmlPage,CXmlControl)

CXmlPage::CXmlPage()
{
	m_xmlMode = XVE_XPAGE;
}

void CXmlPage::Clear()
{
	SUPERCLASS::Clear();

	while(!m_xmlCtrls.empty())
	{
		CXmlControl* ctrl = m_xmlCtrls.back();
		ctrl->Clear();
		SAFE_DELETE(ctrl);
		m_xmlCtrls.pop_back();
	}
}

bool CXmlPage::InitChildElement(const char* pValue)
{
	if(SUPERCLASS::InitChildElement(pValue))
		return true;

	assert(pValue);
	if(stricmp(pValue,"Button") == 0)
	{
		CXmlButton* btn = new CXmlButton();
		btn->Create();
		m_xmlCtrls.push_back(btn);
		return true;
	}
	if(stricmp(pValue,"Label") == 0)
	{
		CXmlLabel* label = new CXmlLabel();
		label->Create();
		m_xmlCtrls.push_back(label);
		return true;
	}
	else if(stricmp(pValue,"CheckButton") == 0)
	{
		CXmlRadio* radio = new CXmlRadio();
		radio->Create();
		m_xmlCtrls.push_back(radio);
		return true;
	}
	else if(stricmp(pValue,"RadioGroup") == 0)
	{
		CXmlRadioGroup* radiogrp = new CXmlRadioGroup();
		radiogrp->Create();
		m_xmlCtrls.push_back(radiogrp);
		return true;
	}
    else if(stricmp(pValue,"Edit") == 0)
    {
        CXmlEdit* edit = new CXmlEdit();
        edit->Create();
        m_xmlCtrls.push_back(edit);
        return true;
    }
    else if(stricmp(pValue,"Tip") == 0)
    {
        CXmlTip* tip = new CXmlTip();
        tip->Create();
        m_xmlCtrls.push_back(tip);
        return true;
    }
    else if(stricmp(pValue,"Menu") == 0)
    {
        CXmlMenu* menu = new CXmlMenu();
        menu->Create();
        m_xmlCtrls.push_back(menu);
        return true;
    }
	else if(stricmp(pValue,"Scroll") == 0)
	{
		CXmlScroll* scroll = new CXmlScroll();
		scroll->Create();
		m_xmlCtrls.push_back(scroll);
		return true;
	}
	else if(stricmp(pValue,"Control") == 0)
	{
		CXmlControl* ctrl = new CXmlControl();
		ctrl->Create();
		m_xmlCtrls.push_back(ctrl);
	}
    else if(stricmp(pValue,"GoodGrid") == 0)
    {
        CXmlControl* ctrl = new CXmlGoodGrid();
        ctrl->Create();
        m_xmlCtrls.push_back(ctrl);
    }

	return false;
}

CXmlControl* CXmlPage::FindChild(const char* name)
{
	VXmlControl::iterator itr;
	for(itr = m_xmlCtrls.begin();itr != m_xmlCtrls.end();itr++)
	{
		CXmlControl* ctrl = *itr;
		if(!ctrl)
			continue;

		if(ctrl->GetName().compare(name) == 0)
			return ctrl;
	}
	return NULL;
}

//-----------------------------------------------------------------------------
//窗口
BEGIN_VAR(CXmlWindow, CXmlControl)
INIT_VAR(bool, NoMovable,false)
INIT_VAR(bool, NoChangeLevel,false)
INIT_VAR(bool, NoFocus,false)
INIT_VAR(bool, SavePos,true)
END_VAR(CXmlWindow, CXmlControl)

CXmlWindow::CXmlWindow()
{
	m_xmlMode = XVE_XWINDOW;
	m_pSibling = NULL;
	Clear();
}

CXmlWindow::~CXmlWindow()
{
	SAFE_DELETE(m_pSibling);
	Clear();
}

void CXmlWindow::Clear()
{
	SUPERCLASS::Clear();
	while(!m_xmlPages.empty())
	{
		CXmlPage* page = m_xmlPages.back();
		page->Clear();
		SAFE_DELETE(page);
		m_xmlPages.pop_back();
	}

	m_strScriptFileContent.clear();
}

bool CXmlWindow::InitChildElement(const char* pValue)
{
	if(SUPERCLASS::InitChildElement(pValue))
		return true;

	assert(pValue);
	if(stricmp(pValue,"Page") == 0)
	{
		CXmlPage* page = new CXmlPage();
		page->Create();
		m_xmlPages.push_back(page);
		return true;
	}
	else if(stricmp(pValue,"Script") == 0)//需要载入的脚本文件
	{
		m_xmlScriptFile.Create();
		return true;
	}	

    return false;
}

const string & CXmlWindow::GetScriptFileContent()
{
	if(!(m_strScriptFileContent.empty()))
		return m_strScriptFileContent;

	if(!m_xmlScriptFile.GetFile().empty())
	{
	    string filename = StringUtil::format("ui\\%s",m_xmlScriptFile.GetFile().c_str());
		DataStreamInterface* stream = g_pStreamMgr->OpenDataFile(filename.c_str(),false,true,false,EP_UI);
		if (!stream)
		{
			stream = g_pStreamMgr->OpenDataFile(filename.c_str(),false,false,false,EP_UI);
		}

		if(!stream)
			return m_strScriptFileContent;

	    string buf;
	    while(!stream->eof())
	    {
	        buf += stream->getLine();
	        buf += "\n";
	    }        
	    m_strScriptFileContent += buf;
		SAFE_DELETE(stream);
	}

	return m_strScriptFileContent;

}

void CXmlWindow::AddSibling(CXmlWindow* win)
{
	if(m_pSibling == NULL)
	{
		m_pSibling = win;
		return;
	}

	//加入尾部
	CXmlWindow* p = m_pSibling;
	while(p->m_pSibling)
	{
		p = p->m_pSibling;
	}
	p->m_pSibling = win;
}

//-----------------------------------------------------------------------------
//编辑框
BEGIN_VAR(CXmlEdit,CXmlControl)
END_VAR(CXmlEdit,CXmlControl)

CXmlEdit::CXmlEdit()
{
	m_xmlMode = XVE_XEDIT;
}

void CXmlEdit::Clear()
{
	SUPERCLASS::Clear();    
}

bool CXmlEdit::InitChildElement(const char* pValue)
{
	if(SUPERCLASS::InitChildElement(pValue))
		return true;

    assert(pValue);
        
    if(stricmp(pValue,"Color") == 0)
    {
        m_xmlColor.Create();
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------
//滚动条
BEGIN_VAR(CXmlScroll,CXmlControl)
END_VAR(CXmlScroll,CXmlControl)

CXmlScroll::CXmlScroll()
{
	m_xmlMode = XVE_XSCROLL;
}

void CXmlScroll::Clear()
{
	SUPERCLASS::Clear();
	m_xmlScroll.Clear();
}

bool CXmlScroll::InitChildElement(const char* pValue)
{
	if(SUPERCLASS::InitChildElement(pValue))
		return true;

	if(stricmp(pValue,"ScrollTexture") == 0)
	{
		m_xmlScroll.Create();
		return true;
	}
	return false;
}

//Menu Item
BEGIN_VAR(CXmlMenuItem,CXmlControl)
INIT_VAR(string, Text,"")
INIT_VAR(string, Tip,"")
END_VAR(CXmlMenuItem,CXmlControl)
CXmlMenuItem::CXmlMenuItem()
{
    m_xmlMode = XVE_BASE;
}

void CXmlMenuItem::Clear()
{
    SUPERCLASS::Clear();    
}

bool CXmlMenuItem::InitChildElement(const char* pValue)
{
    if(SUPERCLASS::InitChildElement(pValue))
        return true;

    return false;
}

//-----------------------------------------------------------------------------
//菜单

BEGIN_VAR(CXmlMenu,CXmlControl)
END_VAR(CXmlMenu,CXmlControl)

CXmlMenu::CXmlMenu()
{
	m_xmlMode = XVE_XMENU;
}

void CXmlMenu::Clear()
{
	SUPERCLASS::Clear();
    m_xmlMenuItems.clear();
}

bool CXmlMenu::InitChildElement(const char* pValue)
{
	if(SUPERCLASS::InitChildElement(pValue))
		return true;
    
    assert(pValue);

    if(stricmp(pValue,"MenuItem") == 0)
    {
        CXmlMenuItem menuItem;
        menuItem.Create();
        m_xmlMenuItems.push_back(menuItem);
        return true;        
    }

	return false;
}

//-----------------------------------------------------------------------------
//TIPS控件
BEGIN_VAR(CXmlTip,CXmlControl)
INIT_VAR(int, Center,0)
INIT_VAR(int, Border,0)
END_VAR(CXmlTip,CXmlControl)

CXmlTip::CXmlTip()
{
	m_xmlMode = XVE_XTIP;
}

CXmlTip::~CXmlTip()
{    
}

void CXmlTip::Clear()
{
	SUPERCLASS::Clear();
    m_xmlLines.clear(); 
}

bool CXmlTip::InitChildElement(const char* pValue)
{
	if(SUPERCLASS::InitChildElement(pValue))
		return true;

    assert(pValue);

    if(stricmp(pValue,"Line") == 0)
    {
        CXmlFontString line;
        line.Create();
        m_xmlLines.push_back(line);
        return true;        
    }

	return false;
}
