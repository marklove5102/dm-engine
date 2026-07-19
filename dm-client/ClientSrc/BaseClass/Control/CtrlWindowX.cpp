#include "CtrlWindowX.h"
#include "UiManager.h"
#include "CtrlButton.h"
#include "CtrlRadio.h"
#include "CtrlEdit.h"
#include "CtrlTip.h"
#include "CtrlMenuWnd.h"
#include "CtrlScroll.h"
#include "GoodGrid.h"
#include "LuaScripts/LuaScript.h"

DTI_IMPLEMENT(CCtrlWindowX,CCtrlWindowS)

CCtrlWindowX::CCtrlWindowX(void):
CCtrlWindowS()
{
	m_iControlMode		= CTRL_MODE_WINDOW;
	m_sName.clear();
	m_pXmlWindow = NULL;
	m_bAutoCloseWhenTrusteeship = true;
}

CCtrlWindowX::~CCtrlWindowX(void)
{
}

bool CCtrlWindowX::Create(CControl* pParent,int iCurPage)
{
	CXmlWindow* win = NULL;
	if(m_pXmlWindow)
		win = m_pXmlWindow;
	else
	{
		if(!m_sName.empty())
			win = g_pUiMgr->GetXmlWindow(m_sName);
	}

	if(win == NULL)
		return CCtrlWindowS::Create(pParent,GetWndSX(),GetWndSY(),iCurPage);

	return CreateXML(pParent,win);
}

bool CCtrlWindowX::CreateXML(CControl* pParent,CXmlWindow* win)
{
	if(win == NULL)
		return false;

	if(pParent == NULL)
		pParent = g_pControl;

	if(!m_bNoMove) //是默认值读Xml,否则以程序设置的为准
		m_bNoMove = win->GetNoMovable();

	if(!m_bNoChangeLevel)//是默认值读Xml,否则以程序设置的为准
		m_bNoChangeLevel = win->GetNoChangeLevel();

	if(m_bNeedSavePos)//是默认值读Xml,否则以程序设置的为准
		m_bNeedSavePos = win->GetSavePos();

	if(m_iIndex == 0)//是默认值读Xml,否则以程序设置的为准
		m_iIndex = win->GetBackTexture().GetIndex();

	if(m_iPages == 1)//是默认值读Xml,否则以程序设置的为准
		m_iPages = win->GetPages().size();

	m_iCurPage = 0;
	if(m_iPages == 0)
		m_iPages = 1;

	m_pXmlWindow = win;
	AssignXML(win);


	//创建窗口
	if(!CCtrlWindowS::Create(pParent,POS_AUTO,POS_AUTO))//POS_AUTO无效，以设置好了的值为准，在CCtrlWindowS::Create中有判断if(m_pXmlControl == NULL)
		return false;

	//创建好了，看这个窗口是否有包含脚本文件，如果有的话那么执行这个文件中的所有脚本，
	//以便该窗口中的控件正确有使用脚本
	string strScriptContent = m_pXmlWindow->GetScriptFileContent();
	if(!(strScriptContent.empty()))
	{
		g_LuaScript.ExecuteString(strScriptContent);
	}

	return true;
}

void CCtrlWindowX::OnCreate()
{
	if(m_pXmlWindow)
	{
		VXmlPage& Pages = m_pXmlWindow->GetPages();
		int iXmlPages = Pages.size();

		if(iXmlPages == 0)
		{
			CreateChild(m_pXmlWindow);
		}
		else if(iXmlPages == 1)
		{
			CreateChild(Pages[0]);
			CreateChild(m_pXmlWindow);
		}
		else
		{
			CreateChild(Pages[m_iCurPage]);
		}
		m_pXmlControl = m_pXmlWindow;
	}

	CCtrlWindowS::OnCreate();
}

bool CCtrlWindowX::CreateChild(CXmlPage* page)
{
	if(!page)
		return false;

	VXmlControl& child = page->GetChildren();
	VXmlControl::iterator itr;
	for(itr = child.begin();itr != child.end();itr++)
	{
		CXmlControl* ctrl = *itr;
		assert(ctrl);

		switch(ctrl->GetXmlMode())
		{
		case XVE_XBUTTON:
			{
				CCtrlButton* btn = new CCtrlButton();
				AddControl(btn);
				btn->CreateXML(this,dynamic_cast<CXmlButton*>(ctrl));
			}
			break;
		case XVE_XRADIO:
			{
				CCtrlRadio* radio = new CCtrlRadio();
				AddControl(radio);
				radio->CreateXML(this,dynamic_cast<CXmlRadio*>(ctrl));
			}
			break;
		case XVE_XRADIOGROUP:
			{
				CXmlRadioGroup* group = dynamic_cast<CXmlRadioGroup*>(ctrl);
				VXmlRadio& radios = group->GetRadios();

				CCtrlRadio* pRootRadio = NULL;
				for(int ii = 0;ii < radios.size();ii++)
				{
					CCtrlRadio* radio = new CCtrlRadio();
					AddControl(radio);
					radio->CreateXML(this,radios.at(ii));

					if(pRootRadio)
						pRootRadio->AddBuddy(radio);
					else
						pRootRadio = radio;
				}
            }
			break;
        case XVE_XEDIT:
            {
                CCtrlEdit* edit = new CCtrlEdit();
                AddControl(edit);
                edit->CreateXML(this,dynamic_cast<CXmlEdit*>(ctrl));
            }
            break;
        case XVE_XTIP:
            //{
            //    CCtrlTip* tip = new CCtrlTip();
            //    AddControl(tip);
            //    tip->CreateXML(this,dynamic_cast<CXmlTip*>(ctrl));
            //}
            break;
        case XVE_XMENU:
        //    {
        //        CCtrlMenuWnd* menu = new CCtrlMenuWnd();
        //        AddControl(menu);
        //        menu->CreateXML(this,dynamic_cast<CXmlMenu*>(ctrl));
        //    }
            break;
		case XVE_XSCROLL:
			{
				CCtrlScroll* scroll = new CCtrlScroll();
				AddControl(scroll);
				scroll->CreateXML(this,dynamic_cast<CXmlScroll*>(ctrl));
			}
			break;
        case XVE_XGOODGRID:
            {
                CGoodGrid* grid = new CGoodGrid();
                AddControl(grid);
                grid->CreateXML(this,dynamic_cast<CXmlGoodGrid*>(ctrl));
            }
            break;
		}
	}
	return true;
}

void CCtrlWindowX::OnClickCloseButton()
{
	CloseWindow();
}

void CCtrlWindowX::CloseWindow(BOOL bOK)
{
	g_pControl->Msg(MSG_CTRL_CLOSE_WINDOW,OPER_CLOSE,this);   
}

CCtrlScroll* CCtrlWindowX::FindScrollByName(const char* name)
{
	CControl* pCtrl = CCtrlWindowS::FindControlByName(name,CTRL_MODE_SCROLL);
	if(pCtrl == NULL)
		return NULL;

	return dynamic_cast<CCtrlScroll*>(pCtrl);
}

CCtrlRadio*  CCtrlWindowX::FindRadioByName(const char* name)
{
	CControl* pCtrl = CCtrlWindowS::FindControlByName(name,CTRL_MODE_RADIO);
	if(pCtrl == NULL)
		return NULL;

	return dynamic_cast<CCtrlRadio*>(pCtrl);
}

CCtrlButton* CCtrlWindowX::FindButtonByName(const char* name)
{
	CControl* pCtrl = CCtrlWindowS::FindControlByName(name,CTRL_MODE_BUTTON);
	if(pCtrl == NULL)
		return NULL;

	return dynamic_cast<CCtrlButton*>(pCtrl);
}

CCtrlEdit*   CCtrlWindowX::FindEditByName(const char* name)
{
	CControl* pCtrl = CCtrlWindowS::FindControlByName(name,CTRL_MODE_EDIT);
	if(pCtrl == NULL)
		return NULL;

	return dynamic_cast<CCtrlEdit*>(pCtrl);
}



