#include "ElixirWnd.h"
#include "Global/Global.h"
#include "GameData/GameData.h"
#include "GameData/PetData.h"
#include "GameControl/GameControl.h"
#include "GameData/MsgBoxMgr.h"
INIT_WND_POSX(CElixirWnd,POS_AUTO,POS_AUTO)
CElixirWnd::CElixirWnd(void)
{
	for (int i = 0;i < 4;i++)
	{
		m_iCount[i] = 0;
	}
	m_iIndex = 16875;

	m_iAlignType = XAL_TOPLEFT;

	g_pGameData->DelPromptInfo(0);
}

CElixirWnd::~CElixirWnd(void)
{
	//1 4 10 30 
}

void CElixirWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	//绘制
	char* pName[4] = {
		"仙灵丹(小)",
		"仙灵丹(中)",
		"仙灵丹(大)",
		"仙灵丹(特大)"
	};
	
	int iTimes[4] = {1,4,10,30};

	char ctemp[128] = {0};
	//int iNeedNum = GetNeedCount();
	LPTexture pTex = NULL;
	Elixir& elixir = g_pGameData->GetElixir();
	
	for (int i = 0;i<4;i++)
	{
		m_pGrid->DrawGrid(i,0,pName[i],TRUE);

		sprintf(ctemp,"%d元宝",elixir.wPrice[i]);
		m_pGrid->DrawGrid(i,1,ctemp,TRUE);

		sprintf(ctemp,"%d倍",elixir.wTimes[i]);
		m_pGrid->DrawGrid(i,2,ctemp,TRUE);

		sprintf(ctemp,"%d",elixir.wTimes[i] * m_iCount[i] * elixir.dwExp[i]);
		m_pGrid->DrawGrid(i,3,ctemp,TRUE);

		sprintf(ctemp,"%d",GetNeedCount(i));
		m_pGrid->DrawGrid(i,4,ctemp,TRUE);

		sprintf(ctemp,"%d",m_iCount[i]);
		m_pGrid->DrawGrid(i,5,ctemp,TRUE);

		pTex = g_pTexMgr->GetTex(PACKAGE_items,5110 + i,EP_UI);
		if (pTex) g_pGfx->DrawTextureNL(m_iScreenX + 31,m_iScreenY + 340 + i * 40,pTex);
	}

	pTex = g_pTexMgr->GetTex(PACKAGE_bgitems,241,EP_UI);
	if (pTex) g_pGfx->DrawTextureNL(m_iScreenX + 55,m_iScreenY + 111,pTex);
	
	sprintf(ctemp,"你在离线时自动修炼了%d个小时。你可以使用仙灵丹将此段时间修",elixir.dwLeftTm / 60);
	g_pFont->DrawText(m_iScreenX + 260,m_iScreenY + 118,ctemp,0xFFFFCF63,FONT_DEFAULT,FONTSIZE_MIDDLE,0);
	g_pFont->DrawText(m_iScreenX + 260,m_iScreenY + 143,"炼的经验释放到放出的绑定灵兽上。(每日最多只可累积8小时，总共",0xFFFFCF63,FONT_DEFAULT,FONTSIZE_MIDDLE,0);
	g_pFont->DrawText(m_iScreenX + 260,m_iScreenY + 167,"只可累积300小时)使用不同的仙灵丹，灵兽获得的经验加成也不同。",0xFFFFCF63,FONT_DEFAULT,FONTSIZE_MIDDLE,0);
}

int CElixirWnd::GetNeedCount(int i)
{
	Elixir& elixir = g_pGameData->GetElixir();
	if (elixir.wHour[i] != 0)
	{
		return elixir.dwLeftTm / (elixir.wHour[i] * 60);
	}
	
	return 0;
}

bool CElixirWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	if (dwMsg == MSG_CTRL_BUTTON_CLICK)
	{
		char* pName[4] = {
			"仙灵丹(小)",
			"仙灵丹(中)",
			"仙灵丹(大)",
			"仙灵丹(特大)"
		};

		for (int i = 0;i<4;i++)
		{
			if (pControl == m_pBtnBuy[i])
			{
				if(g_PetData.HasPetFollowStatus())
					 g_pGameControl->Send_Elixir_Buy_Apply(i + 1,m_iCount[i]);
				else g_MsgBoxMgr.PopSimpleAlert("请先释放出你需要增加经验的灵兽，购买后的仙灵丹将直接增加该灵兽经验");
				//QuickBuy(pName[i],m_iCount[i],0);
			}
			else if (pControl == m_pBtnAdd[i])
			{//增加				
				if (m_iCount[i] < GetNeedCount(i))
					m_iCount[i]++;
			}
			else if (pControl == m_pBtnDec[i])
			{//减少				
				if (m_iCount[i] > 0)
					m_iCount[i]--;
			}
		}
	}

	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CElixirWnd::OnCreate()
{
	SetCloseButton(708,10);
	m_pGrid = new CCtrlGrid;
	m_pGrid->Create(this,75,333,600,494,4,40);
	AddControl(m_pGrid);
	m_pGrid->PushColumn(103);
	m_pGrid->PushColumn(92);
	m_pGrid->PushColumn(83);
	m_pGrid->PushColumn(85);
	m_pGrid->PushColumn(76);
	m_pGrid->PushColumn(86);

	for(int i = 0;i<4;i++)
	{
		m_pBtnBuy[i] = new CCtrlButton;
		m_pBtnBuy[i]->CreateEx(this,617,347 + i * 40,16884,16885,16886);
		AddControl(m_pBtnBuy[i]);

		m_pBtnAdd[i] = new CCtrlButton;
		m_pBtnAdd[i]->CreateEx(this,526,349 + i * 40,16876,16877,16878);
		AddControl(m_pBtnAdd[i]);

		m_pBtnDec[i] = new CCtrlButton;
		m_pBtnDec[i]->CreateEx(this,577,349 + i * 40,16880,16881,16882);
		AddControl(m_pBtnDec[i]);
	}
}
