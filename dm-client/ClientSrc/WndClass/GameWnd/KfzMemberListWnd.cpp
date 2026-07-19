#include "KfzMemberListWnd.h"
#include "GameControl/GameControl.h"
#include "GameData/GameData.h"
#include "GameData/TalkMgr.h"


INIT_WND_POSX(CKfzMemberListWnd,POS_AUTO,POS_AUTO)

CKfzMemberListWnd::CKfzMemberListWnd(void)
{
	m_iIndex = 16000;
	m_iPages = 1;
}

CKfzMemberListWnd::~CKfzMemberListWnd(void)
{

}

void CKfzMemberListWnd::OnCreate()
{
	////for test
	//_KfzGuild &KfzGuild = g_OtherData.GetKfzGuildData();
	//vector<_KfzMember> & VMemberList = KfzGuild.VKfzMemberList;
	//KfzGuild.strGuildName = "testguild";
	//VMemberList.clear();
	//char strTemp[128]={0};
	//for (int i = 0;i < 50; i++)
	//{
	//	VMemberList.push_back(_KfzMember(itoa(i,strTemp,10)));
	//}
	//VMemberList.push_back(_KfzMember("test005005"));
	////end for test

	//列表控件
	m_pGrid = new CCtrlGrid();
	AddControl(m_pGrid);
	m_pGrid->Create(this,35,144,270,328,PERSONS_KFZ_LINE_COUNT+2,15.50f);
	m_pGrid->AddUpButton(218,0,51,52,53);
	m_pGrid->AddDownButton(218,168,56,57,58);
	m_pGrid->AddScroll(218,18,16,149,54);
	m_pGrid->SetDrawOffXY(0,1);
	m_pGrid->SetSelBackColor(0);
	m_pGrid->SetSelTextColor(0xFFFF0000);

	m_pGrid->SetLinesPerPage(PERSONS_KFZ_LINE_COUNT+2);
	m_pGrid->PushColumn(130);
	m_pGrid->PushColumn(110);
	m_pGrid->SetTotalCount(g_OtherData.GetKfzGuildData().VKfzMemberList.size());

	//移动
	m_pClose = new CCtrlButton();
	AddControl(m_pClose);
	m_pClose->CreateEx(this,119,395,16001,16002,16003);

	SetCloseButton(270,55);
	SetMask(16000);
}	

void CKfzMemberListWnd::Draw(void)
{	
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	_KfzGuild &KfzGuild = g_OtherData.GetKfzGuildData();
	vector<_KfzMember> & VMemberList = KfzGuild.VKfzMemberList;
	size_t iNum = VMemberList.size();
	int pos = m_pGrid->GetScrollPos();
	int iLine = 0;

	char strTemp[128]={0};
	sprintf(strTemp,"本服\"%s\"行会跨服争霸成员列表",KfzGuild.strGuildName.c_str());

	g_pFont->DrawText(m_iScreenX + 158,m_iScreenY + 82,strTemp,0xFFFFFF00,FONT_DEFAULT,12,DTF_Center);

	for(size_t i = 0; iLine < PERSONS_KFZ_LINE_COUNT+2 && i + pos < iNum; i ++,iLine ++)
	{
		_KfzMember & member = VMemberList[iLine+pos];
		m_pGrid->DrawGrid(iLine,0,member.strName.c_str());
		if(member.byState == 0)
		{
			if(strcmp(SELF.GetName(),member.strName.c_str()) == 0)
			{
				int iSelLine = m_pGrid->GetSelLine();
				int iCol = m_pGrid->GetSelCol();

				if(m_pGrid->IsClick() && iSelLine == i + pos && iCol == 1)
					m_pGrid->DrawGrid(iLine,1,"我来报到",false,0xFFFF0000,DTF_UnderLine);
				else
					m_pGrid->DrawGrid(iLine,1,"我来报到",false,0xFFFFFF00,DTF_UnderLine);
			}
			else
			{
				m_pGrid->DrawGrid(iLine,1,"未报到");
			}
		}
		else if(member.byState == 2)
		{
			m_pGrid->DrawGrid(iLine,1,"已报到");
		}
		else if(member.byState == 3)
		{
			m_pGrid->DrawGrid(iLine,1,"已报到");
		}

	}

	g_pFont->DrawText(m_iScreenX + 32,m_iScreenY + 348,"上面的勇士将代表本区本服参加跨服争霸，请",0xFFFFFF00);
	g_pFont->DrawText(m_iScreenX + 32,m_iScreenY + 361,"还未报到跨服争霸帐号的勇士尽快来报到，否",0xFFFFFF00);
	g_pFont->DrawText(m_iScreenX + 32,m_iScreenY + 376,"则将失去比赛的资格。",0xFFFFFF00);
}

bool CKfzMemberListWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch(dwMsg)
	{
	case MSG_CTRL_GRID_SELECT_LINE_COL:
		{
			int iSelLine = LOWORD(dwData);
			int iCol = HIWORD(dwData);
			int pos = m_pGrid->GetScrollPos();

			vector<_KfzMember> & VMemberList = g_OtherData.GetKfzGuildData().VKfzMemberList;
			if(iCol == 1 && iSelLine >= 0 && iSelLine < VMemberList.size())
			{
				if(VMemberList[iSelLine].byState == 0 && strcmp(SELF.GetName(),VMemberList[iSelLine].strName.c_str()) == 0)
				{
					g_pGameControl->SEND_Kfz_Activate();
					g_TalkMgr.PushSysTalk("正在报到,请稍候");
				}
			}

			return true;
		}
		break;
	case MSG_CTRL_GRID_DBCLICK:
		{
			return true;
		}
		break;
	case MSG_CTRL_BUTTON_CLICK:
		{
			if(pControl == m_pClose)
			{
				CloseWindow();
				return true;
			}
		}
		break;
	case MSG_CTRL_KFZ_MEMBER_LIST_WND:
		{
			if(dwData == 1)
			{
				m_pGrid->SetTotalCount(g_OtherData.GetKfzGuildData().VKfzMemberList.size());
			}
		}

	default:
		break;
	}

	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

bool CKfzMemberListWnd::OnMouseMove(int iX,int iY)
{
	//int iHotLine = m_pGrid->GetHotLine();
	//CParserTip *pTip = g_pControl->GetTipWnd();

	//if(m_pGrid->IsMouseOn(m_iScreenX + iX,m_iScreenY + iY) && iHotLine >= 0 && iHotLine < PERSONS_LINE_COUNT && iHotLine < m_VNpc.size())
	//{
	//	pTip->Clear();
	//	int x = m_iScreenX + iX + 10;
	//	int y = m_iScreenY + iY + 10;
	//	pTip->AdjustXY(x,y);
	//	pTip->Move(x,y);
	//	pTip->SetShow(true);

	//	if(m_iCurPage == 1)
	//	{
	//		if(m_VNpc[iHotLine].strDesp.length() > 0)
	//			pTip->AddText(m_VNpc[iHotLine].strDesp.c_str(),0xFFFFFF00);

	//		pTip->AddText("双击该NPC,自动寻路到该NPC处",0xFFFFFF00);
	//	}
	//	else if(g_pControl->GetMenuWnd()->GetCaller() != this)
	//		pTip->AddText("点击右键弹出操作菜单",0xFFFFFF00);
	//	else
	//		pTip->SetShow(false);

	//	return true;
	//}

	return CCtrlWindowX::OnMouseMove(iX,iY);
}


bool CKfzMemberListWnd::OnWheel(int iWheel)
{
	return m_pGrid->OnWheel(iWheel);
}