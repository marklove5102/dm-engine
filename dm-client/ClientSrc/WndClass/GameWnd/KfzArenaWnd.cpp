#include "KfzArenaWnd.h"
#include "GameControl/GameControl.h"


INIT_WND_POSX(CKfzArenaWnd,POS_AUTO,POS_AUTO)

CKfzArenaWnd::CKfzArenaWnd(void)
{
	m_iIndex = 16067;
	m_iPages = 1;

	m_iTotalNum = m_iStart = m_iEnd = 0;
}

CKfzArenaWnd::~CKfzArenaWnd(void)
{
}

void CKfzArenaWnd::OnCreate()
{
	//列表控件
	m_pGrid = new CCtrlGrid();
	AddControl(m_pGrid);
 	m_pGrid->Create(this,34,150,502,381,KFZ_LEITAI_LINE_COUNT,19.8f);
	//m_pGrid->AddUpButton(160,3,51,52,53);
	//m_pGrid->AddDownButton(160,254,56,57,58);
	//m_pGrid->AddScroll(160,21,16,232,54);
	m_pGrid->SetDrawOffXY(0,2);

	m_pGrid->SetLinesPerPage(KFZ_LEITAI_LINE_COUNT);
	m_pGrid->PushColumn(45);
	m_pGrid->PushColumn(110);
	m_pGrid->PushColumn(140);
	m_pGrid->PushColumn(75);
	m_pGrid->PushColumn(40);
	m_pGrid->PushColumn(70);
	//m_pGrid->SetTotalCount(m_VMemberList.size());


	m_pPreBtn = new CCtrlButton();
	AddControl(m_pPreBtn);
	m_pPreBtn->CreateEx(this,167,397,13386,13387,13388,13389);
	m_pNextBtn = new CCtrlButton();
	AddControl(m_pNextBtn);
	m_pNextBtn->CreateEx(this,310,397,13390,13391,13392,13393);

	SetCloseButton(502,56);
	SetMask(16067);
}	

void CKfzArenaWnd::Draw(void)
{	
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	size_t iNum = m_VKfzLeiTai.size();
	int iLine = 0;

	char strTemp[128]={0};
	int iSelLine = m_pGrid->GetSelLine();
	int iCol = m_pGrid->GetSelCol();
	for(size_t i = 0; iLine < KFZ_LEITAI_LINE_COUNT && i < iNum; i ++)
	{
		sprintf(strTemp,"%d",m_VKfzLeiTai[iLine].iNo);
		m_pGrid->DrawGrid(iLine,0,strTemp);
		string strGuildName = m_VKfzLeiTai[iLine].strGuildName;
		if(strGuildName.length() > 16)
			strGuildName = strGuildName.substr(0,14) + "...";

		m_pGrid->DrawGrid(iLine,1,strGuildName.c_str());
		m_pGrid->DrawGrid(iLine,2,m_VKfzLeiTai[iLine].strAreaServer.c_str());
		sprintf(strTemp,"%u",m_VKfzLeiTai[iLine].dwDeposit);
		m_pGrid->DrawGrid(iLine,3,strTemp);
		sprintf(strTemp,"%d",m_VKfzLeiTai[iLine].iLeaverMins);
		m_pGrid->DrawGrid(iLine,4,strTemp);

		if(m_pGrid->IsClick() && iSelLine == i && iCol == 5)
			m_pGrid->DrawGrid(iLine,5,"我要攻擂",false,0xFFFF0000,DTF_UnderLine);
		else
			m_pGrid->DrawGrid(iLine,5,"我要攻擂",false,0xFFFFFF00,DTF_UnderLine);

		iLine ++;
	}
}

bool CKfzArenaWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch(dwMsg)
	{
	case MSG_CTRL_GRID_SELECT_LINE_COL:
		{
			int iSelLine = LOWORD(dwData);
			int iCol = HIWORD(dwData);

			if(iCol == 5 && iSelLine >= 0 && iSelLine < m_VKfzLeiTai.size())
			{
				g_pGameControl->SEND_Kfz_Arena_Join(m_VKfzLeiTai[iSelLine].strMapName.c_str());
				CloseWindow();
			}

			return true;
		}
		break;
	case MSG_CTRL_BUTTON_CLICK:
		{
			if(pControl == m_pPreBtn)
			{
				if(m_iStart <= 0)
					return true;

				int iStart = m_iStart - KFZ_LEITAI_LINE_COUNT;
				int iEnd = m_iStart - 1;
				if(iStart < 0)
					iStart = 0;

				if(iStart <=  iEnd)
				{
					g_pGameControl->SEND_Request_Kfz_ArenaList(iStart,iEnd);
				}				

				return true;
			}
			else if(pControl == m_pNextBtn)
			{
				if(m_iEnd >= m_iTotalNum)
					return true;

				int iEnd = m_iEnd + KFZ_LEITAI_LINE_COUNT;
				int iStart = m_iEnd + 1;
				if(iEnd > m_iTotalNum - 1)
					iEnd = m_iTotalNum - 1;

				if(iStart <=  iEnd)
				{
					g_pGameControl->SEND_Request_Kfz_ArenaList(iStart,iEnd);
				}				

				return true;
			}
		}
		break;
	case MSG_CTRL_KFZ_ARENA_WND:
		{
			if(dwData == 1)
			{
				if(pControl)
				{
					OnGetMemberList((char *)pControl);
				}

				return true;
			}
		}
		break;

	default:
		break;
	}

	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CKfzArenaWnd::OnGetMemberList(const char* msg)
{
	if(!msg)
		return;

	m_VKfzLeiTai.clear();

	m_iTotalNum = Conv_WORD(msg + 6);
	m_iStart = Conv_WORD(msg + 8);
	m_iEnd = Conv_WORD(msg + 10);

	int iNum = m_iEnd - m_iStart + 1;
	_KfzLeiTai kfzLeiTai;
	int iLen = 12;
	for(int i = 0; i < iNum; i++)
	{
		kfzLeiTai.iNo = Conv_WORD(msg + iLen);
		iLen += 2;
		kfzLeiTai.strGuildName.assign(msg + iLen,32);
		iLen += 32;
		kfzLeiTai.strAreaServer.assign(msg + iLen,22);
		iLen += 22;
		kfzLeiTai.dwDeposit = Conv_DWORD(msg + iLen);
		iLen += 4;
		kfzLeiTai.iLeaverMins = Conv_BYTE(msg + iLen);
		iLen += 1;
		kfzLeiTai.strMapName.assign(msg + iLen,15);
		iLen += 15;

		m_VKfzLeiTai.push_back(kfzLeiTai);
	}

	if(m_iStart <= 0)
	{
		m_pPreBtn->SetEnable(false);
	}
	else
	{
		m_pPreBtn->SetEnable(true);
	}

	if(m_iEnd >= m_iTotalNum - 1)
	{
		m_pNextBtn->SetEnable(false);
	}
	else
	{
		m_pNextBtn->SetEnable(true);
	}

	m_pGrid->SetTotalCount(m_VKfzLeiTai.size());
}