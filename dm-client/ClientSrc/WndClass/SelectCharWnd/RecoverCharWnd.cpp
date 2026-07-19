#include "recovercharwnd.h"
#include "GameControl/GameControl.h"
#include "GameData/GameData.h"
#include "GameData/GameGlobal.h"
#include "GameData/MsgBoxMgr.h"
#include "GameData/OtherData.h"
#include "GameData/LoginData.h"

#ifdef _DEBUG
#include "global/new.h"
#define new DEBUG_NEW
#endif

INIT_WND_POS(CRecoverCharWnd,POS_AUTO,POS_AUTO)

CRecoverCharWnd::CRecoverCharWnd(void)
{
	m_bModel		= false;
	m_bInheritScaleFromParent = false;
	//	m_bNoMove		= true;

	m_pRecover = NULL;
	m_pCancel = NULL;

	m_nRecoverChar = 0;
	m_iIndex = 20747;
	m_uStyle &= ~CTRL_STYLE_TRANS;

	//g_pGameControl->SEND_Deleted_Char_Info();
}

CRecoverCharWnd::~CRecoverCharWnd(void)
{
}

void CRecoverCharWnd::OnClickCloseButton()
{
	GetParent()->Msg(MSG_CTRL_RECOVERCHARWND,OPER_CLOSE,this);
}

void CRecoverCharWnd::OnCreate()
{
	SetCloseButton(462,22, 66);

	m_pRecover = new CCtrlButton;
	AddControl(m_pRecover);
	m_pRecover->CreateEx(this,145,217,59,60,61,0);
	m_pRecover->SetText("恢 复", 0xFFffb04a ,0xFFffb74f, 0xFFff924a, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE,0, FONT_YAHEI);

	m_pCancel = new CCtrlButton;
	AddControl(m_pCancel);
	m_pCancel->CreateEx(this,277,217, 59,60,61,0);
	m_pCancel->SetText("取 消", 0xFFffb04a ,0xFFffb74f, 0xFFff924a, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE,0, FONT_YAHEI);

	m_pScroll = new CCtrlScroll();
	AddControl(m_pScroll);
	m_pScroll->CreateEx(this,450,82,14,208 - 82);

	m_pScroll->SetRange(g_OtherData.GetDelCharList().size());

}

bool CRecoverCharWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch(dwMsg)
	{
	case MSG_CTRL_BUTTON_CLICK:
		if(pControl == m_pRecover)
		{
			if (g_OtherData.GetUnDelCharSelectIdx() >= 0)
			{
				return true;
			}
			OnRecover();
			return true;
		}
		else if(pControl == m_pCancel)
		{
			GetParent()->Msg(MSG_CTRL_RECOVERCHARWND,OPER_CLOSE,this);
			return true;
		}		
		break;
	case MSG_RECOVER_CHAR_OK://接收消息框消息
		{
			//g_pGameControl->SEND_Restore_Char(m_nRecoverChar);

			if (m_nRecoverChar < 0 || m_nRecoverChar >= g_OtherData.GetDelCharList().size())
			{
				return true;
			}

			g_OtherData.SetUnDelCharSelectIdx(m_nRecoverChar);
			g_pGameControl->SEND_UnDelRole_Req(g_Login.GetLoginID(),g_OtherData.GetDelCharList().at(m_nRecoverChar).szRoleName);

			GetParent()->Msg(MSG_CTRL_RECOVERCHARWND,OPER_CLOSE,this);
			return true;
		}
	default:
		break;
	}	
	return CCtrlWindow::Msg(dwMsg,dwData,pControl);
}


void CRecoverCharWnd::OnDraw()
{
	//删除的角色信息
	VCharInfo& VInfo = g_OtherData.GetDelCharList();

	int nMaxNum = VInfo.size();
	//if (nMaxNum>5)
	//	nMaxNum = 5;
	int iPos = m_pScroll->GetPos();

	for(size_t i = iPos,iRow = 0; i < nMaxNum && iRow < 5;i++,iRow ++)
	{
		_CharInfo& info = VInfo.at(i);

		DWORD dwColor = 0xFF9c9b9b;
		int nBgTex = 20748;
		if(i == m_nRecoverChar)
		{
			dwColor = 0xFFe89251;
			nBgTex = 20749;
		}

		int y = 91 + iRow * 23;
		DrawTexture(27, y-3, nBgTex);
		string strLevel = StringUtil::format("%d",info.wLevel);
		TextOut(80,y,info.szRoleName,dwColor,DTF_Center);
		TextOut(160,y,GetGenderName(info.bySex),dwColor,DTF_Center);
		TextOut(212,y,GetJobName(info.byJob),dwColor,DTF_Center);
		TextOut(264,y,strLevel.c_str(),dwColor,DTF_Center);
		TextOut(373,y,info.szDeleteTime,dwColor,DTF_Center);
	}

	g_pFont->DrawText(200+m_iScreenX, 25+m_iScreenY, "恢 复 角 色", 0xFFFAC896, FONT_YAHEI, FONTSIZE_BIG);
	TextOut(68, 64, "名字         性别     职业     等级            删除时间", 0xFF92442e);
}

bool CRecoverCharWnd::OnRecover()
{
	if(m_nRecoverChar < 0)
		return true;

	//没选中任何已删除角色，直接返回
	VCharInfo& VInfo = g_OtherData.GetDelCharList();
	if(m_nRecoverChar < (int)VInfo.size())
	{
		string str = StringUtil::format("你真的要恢复'%s这个角色吗?",VInfo.at(m_nRecoverChar).szRoleName);
		g_MsgBoxMgr.PopSimpleComfirm(str.c_str(),MSG_RECOVER_CHAR_OK,0);
	}
	return true;
}

bool CRecoverCharWnd::OnLeftButtonDown(int iX, int iY)
{
	m_nRecoverChar = InWhichArea(iX, iY);

	if(m_nRecoverChar >= 0)
		return true;

	return CCtrlWindow::OnLeftButtonDown(iX,iY);
}

int CRecoverCharWnd::InWhichArea(int iX, int iY)
{
	int nWhichArea = -1;

	if(iX > 26 && iX < 466 && iY > 88 && iY < 208)
	{
		if(((iY-88)%23) < 20)
			nWhichArea = (iY - 88) / 23;
	}

	nWhichArea += m_pScroll->GetPos();
	return nWhichArea;
}

bool CRecoverCharWnd::OnWheel(int iWheel)
{
	return m_pScroll->OnWheel(iWheel);
}