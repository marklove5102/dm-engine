#include "NpcBookWnd.h"
#include "GameControl/GameControl.h"
#include "Global/Interface/AudioInterface.h"
#include "GameData/GameData.h"
#include "GameData/NpcData.h"
#include "GameClient/SDOAInterface.h"

INIT_WND_POSX(CNpcBookWnd,POS_VARIABLE,POS_VARIABLE)

CNpcBookWnd::CNpcBookWnd(void)
{
	m_iCurPage = sm_dwWindowType;
	m_pMarkViewer = NULL;

	if(m_iCurPage < 0 || m_iCurPage > 1)
		m_iCurPage = 0;

	m_iIndex = 11525+m_iCurPage;
	m_iPages = 1;
	
	m_iAlignType = XAL_TOP;
	m_iOffX = 0;
	m_iOffY = 40;

	g_pControl->PopupWindow(MSG_CTRL_NPCWND,OPER_CLOSE);
    g_pControl->PopupWindow(MSG_CTRL_LUXNPCWND,OPER_CLOSE);
	g_pControl->PopupWindow(MSG_CTRL_NPCBUYWND,OPER_CLOSE);
	g_pControl->PopupWindow(MSG_CTRL_NPCSALEWND,OPER_CLOSE);
	g_pControl->PopupWindow(MSG_CTRL_NPC_QUICK_WND,OPER_CLOSE);
	g_pControl->PopupWindow(MSG_CTRL_NPCBOARDWND,OPER_CLOSE);
	g_pControl->Msg(MSG_CTRL_YUANBAO_WND,OPER_CLOSE);
	g_pControl->Msg(MSG_CTRL_RELATION_WND,OPER_CUSTOM);

	g_pControl->PopupWindow(MSG_CTRL_ADVENTURE_WND, OPER_CLOSE);
}

CNpcBookWnd::~CNpcBookWnd(void)
{
}

void CNpcBookWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	if(SELF.GetLevel() > 22)
	{
		if(m_iCurPage == 0)
		{
			LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,11524,EP_UI);
			if(pTex)
			{
				g_pGfx->DrawTextureNL(m_iScreenX+284,m_iScreenY+67,pTex);
			}
		}
		else
		{
			LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,11523,EP_UI);
			if(pTex)
			{
				g_pGfx->DrawTextureNL(m_iScreenX+282,m_iScreenY+63,pTex);
			}
		}
	}

	m_pMarkViewer->SetDisLine(m_pScroll->GetPos()); //设置开始行

	int iRow = 0;
	if(m_iCurPage == 0)
		iRow = g_TagTextMgr.GetBookData().GetRow();
	else
		iRow = g_TagTextMgr.GetGuideData().GetRow();

	m_pScroll->SetRange(iRow - 1);

}

void CNpcBookWnd::OnCreate()
{
	SetCloseButton(468,4); 


	m_pScroll = new CCtrlScroll();
	AddControl(m_pScroll);
	m_pScroll->Create(this,447,85,17,177,4684);

	m_pScroll->SetPos(0);
	m_pScroll->SetRange(0);

	//数据显示框
	m_pMarkViewer = new CMarkViewer(16,9);
	AddControl(m_pMarkViewer);
	m_pMarkViewer->Create(this,120,100,320,159);
	if(m_iCurPage == 0)
		m_pMarkViewer->SetTagText(&g_TagTextMgr.GetBookData());
	else
		m_pMarkViewer->SetTagText(&g_TagTextMgr.GetGuideData());

	SetMask(11525);
}

bool CNpcBookWnd::OnLeftButtonUp(int iX, int iY)
{
	string strCommand = m_pMarkViewer->GetCommand();

	if(!strCommand.empty())
	{
		//up 音乐
		g_pAudio->Play(EAT_OTHER,5,g_pAudio->GetRand()++);

		if(strCommand.substr(0,2) == "@@")
		{
			g_NPC.SetLastCommand(strCommand);

			if(strCommand == "@@REALINFO")
			{
				g_pControl->Msg(MSG_CTRL_USERINFO_WND,OPER_CREATE);
			}
			else if(strCommand == "@@RAINBOW")
			{
				//AP不要彩虹帮助,里面有广告,彩虹无法去掉,平台不接受
				if (g_strChannelName.empty())
				{
					StartRainBowSpirt();
				}
			}
			else if(strCommand == "@@STORE")
			{
				g_pControl->PopupWindow(MSG_CTRL_WOOOL_STORE_WND,OPER_CREATE,2);
			}

			return true;
		}
		else if((int)strCommand.size() > 0)
		{
			if(strCommand == "@exit")
			{
				CloseWindow();
				return true;
			}

			g_pGameControl->SEND_Exchange_Menu_Click(g_NPC.GetID(),strCommand.c_str());
			return true;
		}
		return true;
	}

	return CCtrlWindowX::OnLeftButtonUp(iX,iY);
}

bool CNpcBookWnd::OnLeftButtonDown(int iX, int iY)
{
	if(iX > 186 && iY > 64 && iX < 281 && iY < 84)
	{
		if(m_iCurPage == 1)
		{
			SwitchToPage(0);
		}
		return true;
	}
	else if(iX > 283 && iY > 64 && iX < 375 && iY < 84)
	{
		if(m_iCurPage == 0)
		{
			SwitchToPage(1);
		}
		return true;
	}

	return CCtrlWindowX::OnLeftButtonDown(iX,iY);
}

void CNpcBookWnd::SwitchToPage(int iPage)
{
	if(iPage < 0 || iPage > 1)
		iPage = 0;

	if(iPage == m_iCurPage)
	{
		m_pScroll->SetPos(0);
		return;
	}

	m_iCurPage = iPage;
	SetPageTex(0,PACKAGE_INTERFACE,11525+m_iCurPage);

	m_pScroll->SetPos(0);

	if(m_iCurPage == 0)
		m_pMarkViewer->SetTagText(&g_TagTextMgr.GetBookData());
	else
		m_pMarkViewer->SetTagText(&g_TagTextMgr.GetGuideData());

}

bool CNpcBookWnd::OnWheel(int iWheel)
{
	if(m_pScroll->GetRange() == 0)
		return true;

	short iWheelSize = (short)iWheel;
	iWheelSize /= WHEEL_DELTA;

	int iPos = m_pScroll->GetPos();
	iPos -= iWheelSize;

	if(iPos > m_pScroll->GetRange())
		iPos = m_pScroll->GetRange();
	if(iPos < 0)
		iPos = 0;
	m_pScroll->SetPos(iPos);

	return true;
}

void CNpcBookWnd::UpdateScroll()
{
	if(m_pScroll)
		m_pScroll->SetPos(0);
}

bool CNpcBookWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_NPCBOOK_WND:
		if(dwData == 1 || dwData == 2)
		{
			m_pScroll->SetPos(0);
			return true;
		}

	default:
		break;
	}

	return  CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}