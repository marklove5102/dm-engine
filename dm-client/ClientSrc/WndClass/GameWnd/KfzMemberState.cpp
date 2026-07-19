#include "KfzMemberState.h"
#include "GameControl/GameControl.h"

#define KFZ_MEMBERSTATE_LINE_COUNT 10

INIT_WND_POSX(CKfzMemberState,POS_AUTO,POS_AUTO)

CKfzMemberState::CKfzMemberState(void)
{
	m_iIndex = 16055;
	m_iPages = 1;
}

CKfzMemberState::~CKfzMemberState(void)
{
}

void CKfzMemberState::OnCreate()
{
	////for test
	//char strTemp[128]={0};
	//for (int i = 0;i < 50; i++)
	//{
	//	_KfzMemberState state;
	//	state.strName = itoa(i,strTemp,10);
	//	state.byState = 0;
	//	state.strMerName = "12345671234567";
	//	state.byMerState = 0;
	//	m_VMemberStateList.push_back(state);
	//}
	////

	//列表控件
	m_pGrid = new CCtrlGrid();
	AddControl(m_pGrid);
	m_pGrid->Create(this,34,176,418,327,KFZ_MEMBERSTATE_LINE_COUNT,15.10f);
	m_pGrid->AddUpButton(367,3,51,52,53);
	m_pGrid->AddDownButton(367,135,56,57,58);
	m_pGrid->AddScroll(367,21,16,113,54);
	m_pGrid->SetDrawOffXY(0,4);

	m_pGrid->SetLinesPerPage(KFZ_MEMBERSTATE_LINE_COUNT);
	m_pGrid->PushColumn(110);
	m_pGrid->PushColumn(80);
	m_pGrid->PushColumn(100);
	m_pGrid->PushColumn(50);
	
	//m_pGrid->SetTotalCount(m_VMemberStateList.size());

	m_pOkBtn = new CCtrlButton();
	AddControl(m_pOkBtn);
	m_pOkBtn->CreateEx(this,121,394,16056,16057,16058);

	m_pCancelBtn = new CCtrlButton();
	AddControl(m_pCancelBtn);
	m_pCancelBtn->CreateEx(this,272,394,16016,16017,16018);

	SetCloseButton(420,57);
	SetMask(16055);
}	

void CKfzMemberState::Draw(void)
{	
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	size_t iNum = m_VMemberStateList.size();
	int pos = m_pGrid->GetScrollPos();
	int iLine = 0;

	for(size_t i = 0; iLine < KFZ_MEMBERSTATE_LINE_COUNT && i + pos < iNum; i ++)
	{
		m_pGrid->DrawGrid(iLine,0,m_VMemberStateList[iLine+pos].strName.c_str(),false,0xFFFFFF00);
		if(m_VMemberStateList[iLine+pos].byState == 0)
			m_pGrid->DrawGrid(iLine,1,"未到场",false,0xFF666666);
		else
			m_pGrid->DrawGrid(iLine,1,"已到场",false,0xFFFFFF00);

		m_pGrid->DrawGrid(iLine,2,m_VMemberStateList[iLine+pos].strMerName.c_str(),false,0xFFFFFF00);
		if(m_VMemberStateList[iLine+pos].byMerState == 0)
			m_pGrid->DrawGrid(iLine,3,"未到场",false,0xFF666666);
		else if(m_VMemberStateList[iLine+pos].byMerState == 1)
			m_pGrid->DrawGrid(iLine,3,"已到场",false,0xFFFFFF00);
		else
			m_pGrid->DrawGrid(iLine,3,"没有元神",false,0xFFFFFF00);

		iLine ++;
	}

	g_pFont->DrawText(m_iScreenX + 180,m_iScreenY + 87,"参赛人员状态",0xFFFFFF00);
	g_pFont->DrawText(m_iScreenX + 46,m_iScreenY + 350,"必须所有参赛成员及其元神已在本图集中,否则未出现在本地图的成",0xFFFFFF00);	
	g_pFont->DrawText(m_iScreenX + 46,m_iScreenY + 364,"员或元神将无法记录状态",0xFFFFFF00);	
}

bool CKfzMemberState::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch(dwMsg)
	{
	case MSG_CTRL_BUTTON_CLICK:
		{
			if(pControl == m_pOkBtn)
			{
				g_pGameControl->SEND_Kfz_MemberState(1);
				CloseWindow();
				return true;
			}
			else if(pControl == m_pCancelBtn)
			{
				CloseWindow();
				return true;
			}
		}
		break;
	case MSG_CTRL_KFZ_MEMBER_STATE_WND:
		{
			OnGetMemberStateList((char *)pControl,dwData);
			return true;
		}
		break;

	default:
		break;
	}

	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}


bool CKfzMemberState::OnWheel(int iWheel)
{
	return m_pGrid->OnWheel(iWheel);
}
//成员名字(14字节)+成员状态(1字节,0表示不在场,1表示在场)+元神名字(14字节)+元神状态(1字节,0表示不在场,1表示在场,2表示没有元神)
void CKfzMemberState::OnGetMemberStateList(const char *msg,int iLen)
{
	for(int i = 0; i <= iLen - 30;)
	{
		_KfzMemberState memberState;
		memberState.strName.assign(msg + i,14);
		i += 14;
		memberState.byState = msg[i];
		i += 1;
		memberState.strMerName.assign(msg + i,14);
		i += 14;
		memberState.byMerState = msg[i];
		i += 1;

		m_VMemberStateList.push_back(memberState);
	}

	m_pGrid->SetTotalCount(m_VMemberStateList.size());
}
