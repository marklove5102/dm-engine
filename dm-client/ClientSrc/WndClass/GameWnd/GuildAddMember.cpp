#include "GuildAddMember.h"
#include "GameData/GameData.h"
#include "GameControl/GameControl.h"
#include "GameAI/AIBossCfgMgr.h"
#include "GameMap/GameMap.h"
#include "GameData/TalkMgr.h"

#define SCREEN_ROWS 8

INIT_WND_POSX(CGuildAddMember,POS_AUTO,POS_AUTO)


CGuildAddMember::CGuildAddMember()
{
	m_pAdd = 0;//m_pUp = m_pDown = 0;
	m_select = -1;

	m_iType = sm_dwWindowType;
	m_iIndex = 13548;
	m_iPages = 1;
	
	m_iAlignType = XAL_TOPLEFT;

}

void CGuildAddMember::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	int iStart = m_pGrid->GetScrollPos();
	int iEnd = iStart + SCREEN_ROWS;
	iEnd = (iEnd < m_players.size()?iEnd:m_players.size());
	for(int i = iStart;i< iEnd;i++)
	{
		if(i == m_select)
			g_pFont->DrawText(m_iScreenX + 20, m_iScreenY + 81 + (i - iStart) * 21 + 4,m_players[i].c_str(),0xFFFFFFFF,FONT_YAHEI);
		else
			g_pFont->DrawText(m_iScreenX + 20, m_iScreenY + 81 + (i - iStart) * 21 + 4,m_players[i].c_str(),0xFF888888,FONT_YAHEI);
	}
	std::string strTemp,strTemp1;
	if(m_iType == 0)
	{
		strTemp1 = "增加成员";
		strTemp = "请输入想要加入行会的玩家名字?";
	}
	else if(m_iType == 1)
	{
		strTemp1 = "增加好友";
		strTemp = "请输入要加为好友的玩家名字?";
	}
	else if(m_iType == 2)
	{
		strTemp1 = "添加黑名单";
		strTemp = "请输入要加入黑名单的玩家名字?";
	}
	else if (m_iType == 3)
	{
		strTemp1 = "加入宗族";
		strTemp = "清输入要加入宗族的玩家名字?";
	}

	g_pFont->DrawText(m_iScreenX + 77,m_iScreenY+6,strTemp1.c_str(),COLOR_TEXT_NORMAL,FONT_YAHEI,16,DTF_BlackFrame|DTF_Bold);
	g_pFont->DrawText(m_iScreenX + 17,m_iScreenY+30,strTemp.c_str(),0xFFE6C800,FONT_YAHEI);
}

bool CGuildAddMember::OnLeftButtonDown(int iX,int iY)
{
	return CCtrlWindowX::OnLeftButtonDown(iX,iY);
}

bool CGuildAddMember::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch(dwMsg)
	{
	case MSG_CTRL_BUTTON_CLICK:
		{
			if(pControl== (CControl*)m_pAdd)
			{
				if(m_iType == 0)		OnAddGuildMember();
				else if(m_iType == 1)	OnAddFriend();
				else if(m_iType == 2)	OnAddBlkList();
				else if (m_iType == 3)  OnPhyleAdd();

				return true;
			}
			else if(pControl == (CControl*)m_pCancel)
			{
				g_pControl->Msg(MSG_GUILDADDMEMBER_WND,OPER_CLOSE);
				return true;
			}
			break;
		}
	case MSG_CTRL_GRID_SELECT_LINE_COL:
		{
			int iSelLine = LOWORD(dwData);

			if (iSelLine < 0 || iSelLine >= m_players.size()) break;

			m_pEdit->SetText(m_players[iSelLine].c_str());
		}
	default:
		break;
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CGuildAddMember::OnCreate()
{
	m_pEdit = new CCtrlEdit();
	AddControl(m_pEdit);
	m_pEdit->CreateEx(this,15,49,175,23,0,12,12,12);
	m_pEdit->SetFont(FONT_YAHEI,FONTSIZE_DEFAULT);
	m_pEdit->SetMaxLength(14);

	m_pAdd = new CCtrlButton();
	AddControl(m_pAdd);
	m_pAdd->CreateEx(this,23,260,90,91,92);
	m_pAdd->SetText("确 定",COLOR_BTN_NORMAL,COLOR_BTN_MOUSEON,COLOR_BTN_PRESS);

	m_pCancel = new CCtrlButton();
	AddControl(m_pCancel);
	m_pCancel->CreateEx(this,114,260,90,91,92);
	m_pCancel->SetText("取 消",COLOR_BTN_NORMAL,COLOR_BTN_MOUSEON,COLOR_BTN_PRESS);

	m_pGrid = new CCtrlGrid;
	m_pGrid->CreateEx(this,12,83,200,252,8,21,17665);
	AddControl(m_pGrid);
	m_pGrid->SetDrawOffXY(0,3);
	m_pGrid->PushColumn(110);

	SetCloseButton(187,2,80);

	CSimpleCharacterNode * pChar = NULL;
	int iSelfX,iSelfY;
	int i,j;
	int nBx,nBy,nEx,nEy;
	int nMapW = g_pGameMap->GetWidth();
	int nMapH = g_pGameMap->GetHeight();

	SELF.GetXY(iSelfX, iSelfY);
	nBx = max(0, iSelfX-5);
	nEx = min(iSelfX+5, nMapW);
	nBy = max(0, iSelfY-5);
	nEy = min(iSelfY+5, nMapH);

	for( i=nBx; i<nEx; i++ )
	{
		for( j=nBy; j<nEy; j++ )
		{
			pChar = MapArray.GetSimpleCharacterHead( i, j );
			while( pChar )
			{
				if(pChar->IsHuman())
				{
					m_players.push_back(pChar->GetName());
				}
				pChar = pChar->m_MapNext;
			}
		}
	}

	////test
	//for(int i = 0; i < 30;i++)
	//{
	//	char cTemp[123]={0};
	//	sprintf(cTemp,"%d",i);
	//	m_players.push_back(cTemp);
	//}
	////end test

	m_pGrid->SetTotalCount(m_players.size());
} 

void CGuildAddMember::OnAddGuildMember()
{
	if(strlen(m_pEdit->GetText()) <= 0)
		return;

	g_pGameControl->SEND_Guild_Add_Member(m_pEdit->GetText());

}

void CGuildAddMember::OnPhyleAdd()
{
	if(strlen(m_pEdit->GetText()) <= 0) 
		return;

	g_pGameControl->SEND_Phyle_Operate(m_pEdit->GetText(),0);
}

void CGuildAddMember::OnAddFriend()
{
	if(strlen(m_pEdit->GetText()) <= 0) 
		return;

	g_pGameControl->SEND_Add_Friend(m_pEdit->GetText());
}

void CGuildAddMember::OnAddBlkList()
{
	if(strlen(m_pEdit->GetText()) <= 0) return;
	if(g_BossCfgMgr.AddBlackName(const_cast<char*>(m_pEdit->GetText())))
	{
		g_pControl->Msg(MSG_CTRL_RELATION_WND,12);
	}
}

int CGuildAddMember::InWhichArea(int iX,int iY)
{
	return 0;
}

bool CGuildAddMember::OnWheel(int iWheel)
{
	return CCtrlWindowX::OnWheel(iWheel);
	//m_pScroll->OnWheel(iWheel);
	//return true;
}