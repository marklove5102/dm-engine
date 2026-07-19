#include "KfzEditMemberWnd.h"
#include "GameData/GameData.h"
#include "GameControl/GameControl.h"
#include "GameData/GuildData.h"
#include "GameData/MsgBoxMgr.h"


INIT_WND_POSX(CKfzEditMemberWnd,POS_AUTO,POS_AUTO)

CKfzEditMemberWnd::CKfzEditMemberWnd(void)
{
	m_iIndex = 16006;
	m_iPages = 1;
}

CKfzEditMemberWnd::~CKfzEditMemberWnd(void)
{
}

bool CKfzEditMemberWnd::Create(CControl* pParent,int iCurPage)
{
	if(strlen(SELF.GetTitle()) == 0)
	{
		return false;
	}

	return CCtrlWindowX::Create(pParent,iCurPage);
}


void CKfzEditMemberWnd::OnCreate()
{
	////for test
	//vector<_KfzMember> & VSelMemberList = g_OtherData.GetKfzGuildData().VKfzMemberList;
	//m_VMemberList.clear();
	//VSelMemberList.clear();
	//char strTemp[128]={0};
	//for (int i = 0;i < 50; i++)
	//{
	//	m_VMemberList.push_back(_KfzMember(itoa(i,strTemp,10)));
	//	VSelMemberList.push_back(_KfzMember(itoa(i,strTemp,10)));
	//}
	////



	//列表控件
	m_pGrid_Left = new CCtrlGrid();
	AddControl(m_pGrid_Left);
	m_pGrid_Left->Create(this,34,140,210,410,KFZ_EDITMEMBER_LINE_COUNT_LEFT,15.10f);
	m_pGrid_Left->AddUpButton(160,3,51,52,53);
	m_pGrid_Left->AddDownButton(160,254,56,57,58);
	m_pGrid_Left->AddScroll(160,21,16,232,54);
	m_pGrid_Left->SetDrawOffXY(0,4);

	m_pGrid_Left->SetLinesPerPage(KFZ_EDITMEMBER_LINE_COUNT_LEFT);
	m_pGrid_Left->PushColumn(130);
	//m_pGrid_Left->SetTotalCount(m_VMemberList.size());

	//
	m_pGrid_Right = new CCtrlGrid();
	AddControl(m_pGrid_Right);
	m_pGrid_Right->Create(this,380,140,556,410,KFZ_EDITMEMBER_LINE_COUNT_RIGHT,15.10f);
	m_pGrid_Right->AddUpButton(160,3,51,52,53);
	m_pGrid_Right->AddDownButton(160,254,56,57,58);
	m_pGrid_Right->AddScroll(160,20,16,232,54);
	m_pGrid_Right->SetDrawOffXY(0,3);

	m_pGrid_Right->SetLinesPerPage(KFZ_EDITMEMBER_LINE_COUNT_RIGHT);
	m_pGrid_Right->PushColumn(130);
	//m_pGrid_Right->SetTotalCount(VSelMemberList.size());


	m_pAdd = new CCtrlButton();
	AddControl(m_pAdd);
	m_pAdd->CreateEx(this,238,186,16007,16008,16009);
	m_pDel = new CCtrlButton();
	AddControl(m_pDel);
	m_pDel->CreateEx(this,238,267,16010,16011,16012);
	m_pConfirm = new CCtrlButton();
	AddControl(m_pConfirm);
	m_pConfirm->CreateEx(this,230,366,16013,16014,16015);
	m_pCancel = new CCtrlButton();
	AddControl(m_pCancel);
	m_pCancel->CreateEx(this,298,366,16016,16017,16018);

	SetCloseButton(559,84);
	SetMask(16006);



	g_OtherData.GetKfzGuildData().VKfzMemberList.clear();

	if (g_GuildData.GetGuildMemberOfficer().size() == 0)
	{
		g_pGameControl->SEND_Guild_Look_Member_List();//请求行会成员列表
	}
	else
	{
		OnGetNewMemberList();
	}
}	

void CKfzEditMemberWnd::Draw(void)
{	
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	size_t iNum = m_VMemberList.size();
	int pos = m_pGrid_Left->GetScrollPos();
	int iLine = 0;

	for(size_t i = 0; iLine < KFZ_EDITMEMBER_LINE_COUNT_LEFT && i + pos < iNum; i ++)
	{
		m_pGrid_Left->DrawGrid(iLine,0,m_VMemberList[iLine+pos].strName.c_str());
		iLine ++;
	}


	vector<_KfzMember> & VSelMemberList = g_OtherData.GetKfzGuildData().VKfzMemberList;

	iNum = VSelMemberList.size();
	pos = m_pGrid_Right->GetScrollPos();
	iLine = 0;
	for(size_t i = 0; iLine < KFZ_EDITMEMBER_LINE_COUNT_RIGHT && i + pos < iNum; i ++)
	{
		m_pGrid_Right->DrawGrid(iLine,0,VSelMemberList[iLine+pos].strName.c_str());
		iLine ++;
	}

	//g_pFont->DrawText(m_iScreenX + 247,m_iScreenY + 116,"最多选择10位成员",0xFFFFFF00);

}

bool CKfzEditMemberWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch(dwMsg)
	{
	case MSG_CTRL_GRID_RCLICK_LINE_COL:
		{
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
			if(pControl == m_pConfirm)
			{
				OnOk();
				return true;
			}
			else if(pControl == m_pCancel)
			{
				CloseWindow();
				return true;
			}
			else if(pControl == m_pAdd)
			{
				OnAddMember();
				return true;
			}
			else if(pControl == m_pDel)
			{
				OnDelMember();
				return true;
			}

		}
		break;
	case MSG_CTRL_KFZ_EDIT_MEMBER_WND:
		{
			if(dwData == 1)
			{
				OnGetMemberList();
				return true;
			}
			else if(dwData == 2)
			{
				OnGetNewMemberList();
				return true;
			}
		}
		break;

	default:
		break;
	}

	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}


bool CKfzEditMemberWnd::OnWheel(int iWheel)
{
	int iMouseX, iMouseY;
	g_pControl->GetMouseXY(iMouseX, iMouseY);

	if(iMouseX >= m_pGrid_Right->GetScreenX() && iMouseX <= m_pGrid_Right->GetScreenX() + m_pGrid_Right->GetWidth() &&
		iMouseY >= m_pGrid_Right->GetScreenY() && iMouseY <= m_pGrid_Right->GetScreenY() + m_pGrid_Right->GetHeight() )
	{
		return m_pGrid_Right->OnWheel(iWheel);
	}
	else
	{
		return m_pGrid_Left->OnWheel(iWheel);
	}
}

void CKfzEditMemberWnd::OnGetMemberList()
{
	vector<_KfzMember> & VSelMemberList = g_OtherData.GetKfzGuildData().VKfzMemberList;
	//如果自己不在列表中，加进去，因为会长一定要参加
	bool bFindSelf = false;
	for(int i = 0; i < VSelMemberList.size(); i ++)
	{
		if(strcmp(SELF.GetName(),VSelMemberList[i].strName.c_str()) == 0)
		{
			bFindSelf = true;
		}
	}

	if(!bFindSelf)
	{
		VSelMemberList.insert(VSelMemberList.begin(),_KfzMember(SELF.GetName()));
	}


	int iNum = g_GuildData.GetMember().size();
	for(int i = 0; i < iNum; i++)
	{
		int iSize = g_GuildData.GetMember()[i].name.size();

		for(int j = 0; j < iSize; j++)
		{
			string & strName = g_GuildData.GetMember()[i].name[j];
			bool bFind = false;
			//如果已经在参加人员列表中，那么就不在左边待选列表中显示
			for(int k = 0; k < VSelMemberList.size(); k ++)
			{
				if(VSelMemberList[k].strName == strName)
				{
					bFind = true;
					break;
				}
			}

			if(!bFind)
			{
				m_VMemberList.push_back(_KfzMember(strName));
			}
		}
	}

	m_pGrid_Left->SetTotalCount(m_VMemberList.size());
	m_pGrid_Right->SetTotalCount(VSelMemberList.size());
}

void CKfzEditMemberWnd::OnGetNewMemberList()
{
	vector<_KfzMember> & VSelMemberList = g_OtherData.GetKfzGuildData().VKfzMemberList;
	//如果自己不在列表中，加进去，因为会长一定要参加
	bool bFindSelf = false;
	for(int i = 0; i < VSelMemberList.size(); i ++)
	{
		if(strcmp(SELF.GetName(),VSelMemberList[i].strName.c_str()) == 0)
		{
			bFindSelf = true;
		}
	}

	if(!bFindSelf)
	{
		VSelMemberList.insert(VSelMemberList.begin(),_KfzMember(SELF.GetName()));
	}


	int iNum = g_GuildData.GetGuildMemberOfficer().size();
	for(int i = 0; i < iNum; i++)
	{
		int iSize = g_GuildData.GetGuildMemberOfficer()[i].vMember.size();

		for(int j = 0; j < iSize; j++)
		{
			string & strName = g_GuildData.GetGuildMemberOfficer()[i].vMember[j].strName;
			bool bFind = false;

			//如果已经在参加人员列表中，那么就不在左边待选列表中显示
			for(int k = 0; k < VSelMemberList.size(); k ++)
			{
				if(VSelMemberList[k].strName == strName)
				{
					bFind = true;
					break;
				}
			}

			if(!bFind)
			{
				m_VMemberList.push_back(_KfzMember(strName));
			}

			if(g_GuildData.GetGuildMemberOfficer()[i].vMember[j].bAssist)
			{
				bool bFindAssist = false;
				string & strAssName = g_GuildData.GetGuildMemberOfficer()[i].vMember[j].strAssist;
				for(int k = 0; k < VSelMemberList.size(); k ++)
				{
					if(VSelMemberList[k].strName == strAssName)
					{
						bFindAssist = true;
						break;
					}
				}

				if(!bFindAssist && !strAssName.empty())
				{
					m_VMemberList.push_back(_KfzMember(strAssName));
				}
			}
		}
	}

	iNum = g_GuildData.GetGuildMemberNormal().size();
	for(int i = 0; i < iNum; i++)
	{
		int iSize = g_GuildData.GetGuildMemberNormal()[i].vMember.size();

		for(int j = 0; j < iSize; j++)
		{
			string & strName = g_GuildData.GetGuildMemberNormal()[i].vMember[j].strName;
			bool bFind = false;
			//如果已经在参加人员列表中，那么就不在左边待选列表中显示
			for(int k = 0; k < VSelMemberList.size(); k ++)
			{
				if(VSelMemberList[k].strName == strName)
				{
					bFind = true;
				}
			}

			if(!bFind)
			{
				m_VMemberList.push_back(_KfzMember(strName));
			}

			if(g_GuildData.GetGuildMemberNormal()[i].vMember[j].bAssist)
			{
				string & strAssName = g_GuildData.GetGuildMemberNormal()[i].vMember[j].strAssist;
				bool bFindAssist = false;
				for(int k = 0; k < VSelMemberList.size(); k ++)
				{
					if(VSelMemberList[k].strName == strAssName)
					{
						bFindAssist = true;
						break;
					}
				}

				if(!bFindAssist && !strAssName.empty())
				{
					m_VMemberList.push_back(_KfzMember(strAssName));
				}
			}
		}
	}


	m_pGrid_Left->SetTotalCount(m_VMemberList.size());
	m_pGrid_Right->SetTotalCount(VSelMemberList.size());
}

void CKfzEditMemberWnd::OnAddMember()
{
	vector<_KfzMember> & VSelMemberList = g_OtherData.GetKfzGuildData().VKfzMemberList;
	if(VSelMemberList.size() >= 20)
	{
		g_MsgBoxMgr.PopSimpleAlert("最多只能有20位成员参加。");
		return;
	}

	int iSelLine = m_pGrid_Left->GetSelLine();
	if(iSelLine >= 0 && iSelLine < m_VMemberList.size())
	{
		string & strSelName = m_VMemberList[iSelLine].strName;

		for(size_t i = 0; i < VSelMemberList.size(); i++)
		{
			if(VSelMemberList[i].strName == strSelName)
			{
				g_MsgBoxMgr.PopSimpleAlert("该成员已经在参战成员列表中。");
				return;
			}
		}

		VSelMemberList.push_back(m_VMemberList[iSelLine]);
		m_VMemberList.erase(m_VMemberList.begin() + iSelLine);
	}
	else
	{
		g_MsgBoxMgr.PopSimpleAlert("请选择要添加的成员。");
		return;
	}

	m_pGrid_Left->SetTotalCount(m_VMemberList.size());
	m_pGrid_Right->SetTotalCount(VSelMemberList.size());
}

void CKfzEditMemberWnd::OnDelMember()
{
	vector<_KfzMember> & VSelMemberList = g_OtherData.GetKfzGuildData().VKfzMemberList;
	int iSelLine = m_pGrid_Right->GetSelLine();
	if(iSelLine >= 0 && iSelLine < VSelMemberList.size())
	{
		bool bFind = false;
		string & strSelName = VSelMemberList[iSelLine].strName;
		if(strcmp(strSelName.c_str(),SELF.GetName()) == 0)
		{
			g_MsgBoxMgr.PopSimpleAlert("行会会长必须参加。");
			return;
		}

		for(size_t i = 0; i < m_VMemberList.size(); i++)
		{
			if(m_VMemberList[i].strName == strSelName)
			{
				bFind = true;
				break;
			}
		}

		if(!bFind)
		{
			m_VMemberList.push_back(VSelMemberList[iSelLine]);
		}

		VSelMemberList.erase(VSelMemberList.begin() + iSelLine);
	}
	else
	{
		g_MsgBoxMgr.PopSimpleAlert("请选择要踢除的成员。");
		return;
	}

	m_pGrid_Left->SetTotalCount(m_VMemberList.size());
	m_pGrid_Right->SetTotalCount(VSelMemberList.size());
}

void CKfzEditMemberWnd::OnOk()
{
	//vector<_KfzMember> & VSelMemberList = g_OtherData.GetKfzGuildData().VKfzMemberList;
	//if(VSelMemberList.size() != 20)
	//{
	//	g_MsgBoxMgr.PopSimpleAlert("请选择20位成员参加。");
	//	return;
	//}

	g_pGameControl->SEND_Kfz_Confirm_Member();
	CloseWindow();
}