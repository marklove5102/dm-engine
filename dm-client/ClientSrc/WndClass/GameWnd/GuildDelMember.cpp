#include "GuildDelMember.h"
#include "GameData/GameData.h"
#include "GameControl/GameControl.h"
#include "GameData/OtherData.h"
#include "GameData/GuildData.h"
#include "GameData/TalkMgr.h"
#include "RelationWnd.h"
#include "GameData/MsgBoxMgr.h"
#define SCREEN_ROWS 8

INIT_WND_POSX(CGuildDelMember,POS_AUTO,POS_AUTO)

CGuildDelMember::CGuildDelMember(void)
{
	m_pDel = 0;//m_pUp = m_pDown = 0;
	m_start = 0;
	m_select = -1;
	m_iType = LOWORD(sm_dwWindowType);
	m_wData = HIWORD(sm_dwWindowType);

	m_iIndex = 13548;
	m_iPages = 1;
	
	m_iAlignType = XAL_TOPLEFT;

}

CGuildDelMember::~CGuildDelMember(void)
{
}

void CGuildDelMember::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	int iStart = m_pGrid->GetScrollPos();
	int iEnd = iStart + SCREEN_ROWS;
	
	if (m_iType != 3 && m_iType != 4)
	{
		iEnd = (iEnd < m_players.size()?iEnd:m_players.size());
		for(int i = iStart;i< iEnd;i++)
		{
			if(i == m_select)
				g_pFont->DrawText(m_iScreenX + 20, m_iScreenY + 81 + (i-iStart) * 21 + 4,m_players[i].c_str(),0xFFFFFFFF);
			else
				g_pFont->DrawText(m_iScreenX + 20, m_iScreenY + 81 + (i-iStart) * 21 + 4,m_players[i].c_str(),0xFF888888);
		}
	}
	else if (m_iType == 3 || m_iType == 4)
	{
		int iOnlineTotal = g_GuildData.GetGuildOnlineTael().size();
		std::vector<CGuildData::sGuildTael>& vguildTael = g_GuildData.GetGuildOnlineTael();
	
		g_pFont->DrawText(m_iScreenX + 17,m_iScreenY + 82,"行会成员          行会银两",0xFFE6C800,FONT_YAHEI);
		for(size_t i = 0,iLine = 0; iLine < 7 && i + iStart < iOnlineTotal; i ++,iLine++)
		{
			CGuildData::sGuildTael& guildTael = vguildTael[i];
			char ctemp[128]={0};
			sprintf(ctemp,"%-20s%d",guildTael.strName.c_str(),guildTael.dwTael);
			DWORD dwColor = 0xFFFFFFFF;
			if (i == m_select)
			{
				dwColor = 0xFF00FFFF;
			}
			// 				g_pFont->DrawText(m_iScreenX + 20, m_iScreenY + 94 + (i-iStart) * 21 + 6,ctemp,dwColor);
			m_pGrid->DrawGrid(iLine,0,ctemp,false,dwColor);
		}
	}

	std::string strTemp,strTemp1;
	if(m_iType == 0)
	{
		strTemp = "请输入要删除成员的玩家名字?";
		strTemp1 = "删除成员";
	}		
	else if(m_iType == 1)
	{
		strTemp = "请输入要禁言的玩家名字?";
		strTemp1 = "行会禁言";
	}
	else if (m_iType == 3)
	{
		strTemp = "请输入要任命的玩家名字?";
		strTemp1 = "行会任命";
	}
	else if (m_iType == 4)
	{
		strTemp = "请输入分配的数目并选择分配的成员";
		strTemp1 = "银两分配";
	}

	g_pFont->DrawText(m_iScreenX + 77,m_iScreenY+6,strTemp1.c_str(),COLOR_TEXT_NORMAL,FONT_YAHEI,16,DTF_BlackFrame|DTF_Bold);
	g_pFont->DrawText(m_iScreenX + 17,m_iScreenY+30,strTemp.c_str(),0xFFE6C800,FONT_YAHEI);
}

bool CGuildDelMember::OnLeftButtonDown(int iX,int iY)
{
	if(InWhichArea(iX,iY) >=0 )
	{		
		return true;
	}
	else
	{
		return CCtrlWindowX::OnLeftButtonDown(iX,iY);
	}
}

bool CGuildDelMember::OnWheel(int iWheel)
{
	return CCtrlWindowX::OnWheel(iWheel);
}

bool CGuildDelMember::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch(dwMsg)
	{
	case MSG_CTRL_BUTTON_CLICK:
		{
			if(pControl== (CControl*)m_pDel)
			{
				if(m_iType == 0)
					OnDelGuildMember();
				else if(m_iType == 1)
					OnForbid();
				else if (m_iType == 3)
					OnAppoint();
				else if (m_iType == 4)
					OnAssginTael();
				return true;
			}
			else if(pControl == (CControl*)m_pCancel)
			{
				g_pControl->Msg(MSG_GUILDDELMEMBER_WND,OPER_CLOSE);
				return true;
			}
			break;
		}
	case MSG_CTRL_GRID_SELECT_LINE_COL:
		{
			int iSelLine = LOWORD(dwData);
			if (iSelLine < 0) break;

			if (m_iType != 3 && m_iType != 4)
			{
				if(iSelLine < m_players.size())
				{
					m_pEdit->SetText(m_players[iSelLine].c_str());
					m_select = iSelLine;
				}
			}
			else
			{
				if (m_iType == 3)
				{
					std::vector<CGuildData::sGuildTael>& vguildTael = g_GuildData.GetGuildOnlineTael();
					if (iSelLine < vguildTael.size())
					{
						m_pEdit->SetText(vguildTael[iSelLine].strName.c_str());					
						m_select = iSelLine;
					}
					else
					{
						g_TalkMgr.PushSysTalk("不在线成员不能被任命！");
					}				
				}
			}		
		}
		break;
	default:
		break;
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CGuildDelMember::OnCreate()
{
// 	//test
// 	for(int i = 0; i < 22; i ++)
// 		g_GuildData.GetGuildOnlineTael().push_back(g_GuildData.GetGuildOnlineTael()[0]);
//     //end test

	m_pEdit = new CCtrlEdit();
	AddControl(m_pEdit);
	m_pEdit->CreateEx(this,15,49,175,23,0,12,12,12);
	m_pEdit->SetFont(FONT_YAHEI,FONTSIZE_DEFAULT);
	m_pEdit->SetMaxLength(14);
	//m_pEdit->Create(this,FONTSIZE_DEFAULT,0xFFFFFFFF,17 ,49,210,16);

	//m_pDel = new CCtrlButton();
	//AddControl(m_pDel);
	//m_pDel->CreateEx(this,53,264,13506,13507,13508);

	//m_pCancel = new CCtrlButton();
	//AddControl(m_pCancel);
	//m_pCancel->CreateEx(this,144,264,13510,13511,13512);

	m_pDel = new CCtrlButton();
	AddControl(m_pDel);
	m_pDel->CreateEx(this,23,260,90,91,92);
	m_pDel->SetText("确 定",COLOR_BTN_NORMAL,COLOR_BTN_MOUSEON,COLOR_BTN_PRESS);

	m_pCancel = new CCtrlButton();
	AddControl(m_pCancel);
	m_pCancel->CreateEx(this,114,260,90,91,92);
	m_pCancel->SetText("取 消",COLOR_BTN_NORMAL,COLOR_BTN_MOUSEON,COLOR_BTN_PRESS);


	//m_pUp = new CCtrlButton();
	//AddControl(m_pUp);
	//m_pUp->CreateEx(this,204,99,51,52,53);

	//m_pDown = new CCtrlButton();
	//AddControl(m_pDown);
	//m_pDown->CreateEx(this,204,235,56,57,58);

	m_pGrid = new CCtrlGrid;
	if (m_iType == 3 || m_iType == 4)
	{
		m_pGrid->CreateEx(this,12,82,200,252,7,21/*,17665*/);
		m_pGrid->SetLinesPerPage(7);
		m_pGrid->SetDrawOffXY(0,21);
	}
	else
	{
		m_pGrid->CreateEx(this,12,82,200,252,8,21,17665);
		m_pGrid->SetDrawOffXY(0,2);
	}
	AddControl(m_pGrid);
	m_pGrid->PushColumn(110);
	m_pGrid->PushColumn(71);

	SetCloseButton(187,2,80);

	int iTotalNum = 0;
	if(m_iType == 0 || m_iType == 1)
	{
		std::vector<CGuildData::sTileNode>* pNodeList = NULL;
		for (int i = 0;i<2;i++)
		{
			if (i ==0)	pNodeList = &(g_GuildData.GetGuildMemberOfficer());
			else		pNodeList = &(g_GuildData.GetGuildMemberNormal());

			for (int j = 0;j< pNodeList->size();j++)
			{
				CGuildData::sTileNode& sNode = (*pNodeList)[j];
				for (int k = 0;k< sNode.vMember.size();k++)
				{
					CGuildData::_NewMember& newMem = sNode.vMember[k];
					if (i == 0)
					{//官员	
						if (newMem.strName.length() > 0)
							m_players.push_back(newMem.strName);

						if (newMem.strAssist.length() > 0)
							m_players.push_back(newMem.strAssist);
					}
					else
					{//不是官员							
						m_players.push_back(newMem.strName);
					}
				}
			}
		}
		iTotalNum = m_players.size();
	}
	else if(m_iType == 2)
	{
		//将所有的好友此处加进来
		for(int i = 0;i< g_OtherData.GetRelationVector().size();i++)
		{
			int iii = g_OtherData.GetRelationVector()[i].iRelType & RT_FRIEND;

			if((g_OtherData.GetRelationVector()[i].iRelType & RT_FRIEND) != 0)
				m_players.push_back(g_OtherData.GetRelationVector()[i].strName.c_str());
		}
	}

	if (m_iType == 3 || m_iType == 4)
	{
		iTotalNum = g_GuildData.GetGuildOnlineTael().size();
	}

	//for(int i = 0; i < 30;i++)
	//{
	//	char cTemp[123]={0};
	//	sprintf(cTemp,"%d",i);
	//	m_players.push_back(cTemp);
	//}

	m_pGrid->SetTotalCount(iTotalNum);

} 

void CGuildDelMember::OnDelGuildMember()
{
	if(strlen(m_pEdit->GetText()) <= 0) return;
    
    int iRet = strcmp(m_pEdit->GetText(),SELF.GetName());
    if(m_players.size() > 1 && iRet == 0)
    {
        g_TalkMgr.PushSysTalk("退出行会必须先清除其余所有行会成员");
        return;
    }

	g_GuildData.SetGuildDelName(m_pEdit->GetText());
	g_pGameControl->SEND_Guild_Delete_Member(m_pEdit->GetText());
	for(int i = 0;i< m_players.size();i++)
	{
		if(m_players[i].compare(m_pEdit->GetText()) == 0)
		{
			std::vector<std::string>::iterator Iter = m_players.begin();
			Iter +=i;
			m_players.erase(Iter);
			m_pGrid->SetTotalCount(m_players.size());
		}
	}
	
	//if(iRet == 0)[传奇世界不用告诉了，有可能删除不了]
	//{
	//	g_GuildData.Clear();
	//	g_pControl->Msg(MSG_CTRL_RELATION_WND,15);
	//}
}

void CGuildDelMember::OnAssginTael()
{
	//分配行会银两
	string strTemp = m_pEdit->GetText();
	if (strTemp.length() <= 0)
	{
		return;
	}

	int istart = 0;
	DWORD dwTael = StringUtil::toUInt(strTemp,istart);	
	strTemp.clear();
	std::vector<CGuildData::sGuildTael>& vguildTael = g_GuildData.GetGuildOnlineTael();
	if (m_select < vguildTael.size())
	{
		strTemp = vguildTael[m_select].strName;
	}
	else
	{
		if (m_select - vguildTael.size() < m_players.size())
		{
			strTemp = m_players[m_select - vguildTael.size()];
		}
	}

	if (strTemp.length() > 0)
	{
		g_pGameControl->SEND_Guild_Assign_Tael(dwTael,strTemp.c_str());
	}	
}

void CGuildDelMember::OnAppoint()
{
	string strName = m_pEdit->GetText();

	std::vector<CGuildData::sGuildTael>& vguildTael = g_GuildData.GetGuildOnlineTael();
	if (m_select < vguildTael.size() && strName.length() > 0)
	{
		DWORD dwTael = vguildTael[m_select].dwTael;
		std::map<int ,CGuildData::sOfficer>& OffMap = g_GuildData.GetGuildOfficerMap();
		std::map<int ,CGuildData::sOfficer>::iterator it = OffMap.find(m_wData);
		if (it != OffMap.end())
		{
			if (it->second.dwTael <= dwTael)
			{
				//任命和解雇
				char ctemp[256]={0};
				sprintf(ctemp,"%s/%s",strName.c_str(),CRelationWnd::GetAppointName().c_str());
				g_pGameControl->SEND_Guild_Appoint_Dismiss(ctemp,m_wData,true);
			}
			else
			{
				g_MsgBoxMgr.PopSimpleAlert("被任命的成员银两不够，不能任命！");
			}
		}
	}
}

void CGuildDelMember::OnForbid()
{
	string strName = "";
	if((m_select >= 0 ) && (m_select < m_players.size()))
	{
		strName = m_players[m_select].c_str();
		if(strName.empty())	return;			

		//添加到列表中
		if(g_GuildData.GetFindStr().size()>=50)
			g_GuildData.GetFindStr().erase(g_GuildData.GetFindStr().begin());

		g_GuildData.GetFindStr().push_back(strName);

		string str = "@禁止行会聊天  ";
		str.append(strName.c_str());
		g_pGameControl->SEND_Message_Send(str.c_str(),str.size());
	}
	else
	{
		strName = m_pEdit->GetText();
		if (strName.size() > 0)
		{
			string str = "@禁止行会聊天  ";
			str.append(strName.c_str());
			g_pGameControl->SEND_Message_Send(str.c_str(),str.size());
		}
	}
}

int CGuildDelMember::InWhichArea(int iX,int iY)
{
	if(iX > 15 && iX < 201 && iY >= 97 && iY <= 249)
	{
		int iIndex = (iY - 94) / 20;
		if (m_iType != 3 && m_iType != 4)
		{
			m_select = iIndex;
			if(m_select + m_pGrid->GetScrollPos() < m_players.size())
			{
				m_pEdit->SetText(m_players[m_select + m_pGrid->GetScrollPos()].c_str());
				m_select = m_select + m_pGrid->GetScrollPos();
				return m_select;
			}
		}
		else
		{
			m_select = iIndex;
			if (m_iType == 3)
			{
				std::vector<CGuildData::sGuildTael>& vguildTael = g_GuildData.GetGuildOnlineTael();
				if (iIndex + m_pGrid->GetScrollPos() < vguildTael.size())
				{
					m_pEdit->SetText(vguildTael[iIndex + m_pGrid->GetScrollPos()].strName.c_str());					
				}
				else
				{
					g_TalkMgr.PushSysTalk("不在线成员不能被任命！");
				}				
			}
			m_select = m_select + m_pGrid->GetScrollPos();
			return m_select;
		}
	}
	m_select = -1;
	return m_select;
}