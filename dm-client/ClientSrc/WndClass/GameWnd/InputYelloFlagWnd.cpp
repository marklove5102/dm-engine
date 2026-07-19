#include "inputyelloflagwnd.h"
#include "Gamedata/GameData.h"
#include "GameData/MsgBoxMgr.h"
#include "GameControl/GameControl.h"
#include "GameData/DirtyWords.h"

INIT_WND_POSX(CInputYelloFlagWnd,POS_AUTO,POS_AUTO)

CInputYelloFlagWnd::CInputYelloFlagWnd(void)
{
	m_pOKButton=NULL;
	m_pCancelButton=NULL;
	m_pTitle=NULL;
	m_pInfo=NULL;
	m_iFlagX = m_iFlagY = 0;
	m_id = sm_dwWindowType;

	m_iIndex = 13481;
	m_iPages = 1;
	
	m_iAlignType = XAL_TOPLEFT;

}

CInputYelloFlagWnd::~CInputYelloFlagWnd(void)
{
}

void CInputYelloFlagWnd::OnCreate()
{
	m_pTitle = new CCtrlEdit();
	AddControl(m_pTitle);
	m_pTitle->CreateEx(this,129,32,302,18);
	m_pTitle->SetText("",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI);
	m_pTitle->SetMaxLength(20);

	m_pTitle->SetFocus();

	m_pInfo = new CCtrlMultiEdit(160,40,4);
	AddControl(m_pInfo);
	m_pInfo->Create(this,12,39,75,301 - 12,128 - 75);
	m_pInfo->SetText("",FONT_YAHEI,FONTSIZE_SMALL,0,0,0,COLOR_TEXT_NORMAL);

	m_pOKButton = new CCtrlButton();
	AddControl(m_pOKButton);
	m_pOKButton->CreateEx(this, 63,146, 95, 96, 97, 98);
	m_pOKButton->SetText("确定", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);

	m_pCancelButton = new CCtrlButton();
	AddControl(m_pCancelButton);
	m_pCancelButton->CreateEx(this, 203,146, 95, 96, 97, 98);
	m_pCancelButton->SetText("取消", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);

}

void CInputYelloFlagWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	g_pFont->DrawText(m_iScreenX + 38,m_iScreenY + 35,"设置旗帜名称:",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL);
	g_pFont->DrawText(m_iScreenX + 38,m_iScreenY + 55,"设置留言内容:",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL);

}

bool CInputYelloFlagWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	if( m_pOKButton && dwMsg == MSG_CTRL_BUTTON_CLICK && pControl==(CControl *)m_pOKButton )
	{
		OnClickOKButton();
		return true;
	}
	else if( m_pCancelButton && dwMsg == MSG_CTRL_BUTTON_CLICK && pControl==(CControl *)m_pCancelButton )
	{
		OnClickCancelButton();
		return true;
	}
	else if(dwMsg == MSG_CTRL_YELLOW_FLAG_WND)
	{
		m_iFlagX = LOWORD(dwData);
		m_iFlagY = HIWORD(dwData);
	}

	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CInputYelloFlagWnd::OnClickCancelButton()
{
	CloseWindow();
}

void CInputYelloFlagWnd::OnClickOKButton()
{
	m_strTitle = m_pTitle->GetText();
	if(m_strTitle == "")
	{
		m_pTitle->SetFocus();
		return;
	}
	m_strInfo = m_pInfo->GetText();
	if(m_strInfo == "")
	{
		m_pInfo->SetFocus();
		return;
	}
	char *p = (char*)m_strTitle.c_str();
	int n=strlen(p);
	char *p1;
	int i=0,j=0;
	while(j<n)
	{
		p1 = CharNext(p);
		if(p1==NULL)
			break;
		i = (int)(p1 - p);

		if(i==1)
		{
			g_MsgBoxMgr.PopSimpleAlert("对不起，旗帜名称只能输入汉字和中文标点符号，请重新输入。");
			return;
		}

		j +=i;
		p=p1;
	}

	g_DirtyWords.ClearWords(m_strTitle);
	g_DirtyWords.ClearWords(m_strInfo);
	string str;
	str += m_strTitle;
	char temp[1];
	temp[0]='\0';
	str += string(temp,1);
	str += m_strInfo;


	g_pGameControl->SEND_Input_YelloFlag(m_id,str,m_iFlagX,m_iFlagY,1);
	CloseWindow();

}
