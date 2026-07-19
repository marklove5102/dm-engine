#include "WenPeiWnd.h"
#include "BaseClass/Control/GoodGrid.h"
#include "GameData/GameData.h"
#include "GameControl/GameControl.h"
#include "GameData/NpcData.h"
#include "GameData/OtherData.h"
#include "GameData/TalkMgr.h"
#include "GameData/MsgBoxMgr.h"
#include "Baseclass/Control/ParserTip.h"
#include "GameAI/AIGoodMgr.h"
#include "GameData/MagicDefine.h"
#include "GameData/XmlCfg.h"


INIT_WND_POSX(CWenPeiWnd,POS_AUTO,POS_AUTO)


CWenPeiWnd::CWenPeiWnd(void)
{
	SetCtrlAll0();

	m_iIndex = 22300;

	m_iPages = 1;
	m_iAlignType = XAL_TOPLEFT;

	for (int j = 0; j < 2; ++j)
	{
		for (int i = 0; i < 4; ++i)
		{
			m_iSelectIndex[j][i] = -1;
		}
	}

	m_iSuccessFrame = 10000;


	g_pGameControl->SEND_Exchange_Menu_Click(DWORD(-8),"@main");



	m_iVersion = 2;
	S_TabPage* pPage1 = AddTabPage(0,0,MAKELONG(22300,PACKAGE_INTERFACE),0,36,85,86,87,88,"纹佩包裹",NULL,true,0,0,1);
	AddTabPage(0,0,MAKELONG(22300,PACKAGE_INTERFACE),51,34,17616,17617,17618,17618,"边 纹",pPage1,false,0,0,0,FONT_YAHEI,FONTSIZE_SMALL);
	AddTabPage(0,0,MAKELONG(22300,PACKAGE_INTERFACE),51 + 50 * 1,34,17616,17617,17618,17618,"底 纹",pPage1,false,0,0,0,FONT_YAHEI,FONTSIZE_SMALL);
	AddTabPage(0,0,MAKELONG(22300,PACKAGE_INTERFACE),51 + 50 * 2,34,17616,17617,17618,17618,"主 纹",pPage1,false,0,0,0,FONT_YAHEI,FONTSIZE_SMALL);
	AddTabPage(0,0,MAKELONG(22301,PACKAGE_INTERFACE),51 + 50 * 3,34,17616,17617,17618,17618,"辅饰纹",pPage1,false,0,0,0,FONT_YAHEI,FONTSIZE_SMALL);

	S_TabPage* pPage2 = AddTabPage(0,0,MAKELONG(22300,PACKAGE_INTERFACE),0,36 + 90,85,86,87,88,"纹佩分解",NULL,true,0,0,1);
	AddTabPage(0,0,MAKELONG(22300,PACKAGE_INTERFACE),51,34,17616,17617,17618,17618,"边 纹",pPage2,false,0,0,0,FONT_YAHEI,FONTSIZE_SMALL);
	AddTabPage(0,0,MAKELONG(22300,PACKAGE_INTERFACE),51 + 50 * 1,34,17616,17617,17618,17618,"底 纹",pPage2,false,0,0,0,FONT_YAHEI,FONTSIZE_SMALL);
	AddTabPage(0,0,MAKELONG(22300,PACKAGE_INTERFACE),51 + 50 * 2,34,17616,17617,17618,17618,"主 纹",pPage2,false,0,0,0,FONT_YAHEI,FONTSIZE_SMALL);
	AddTabPage(0,0,MAKELONG(22301,PACKAGE_INTERFACE),51 + 50 * 3,34,17616,17617,17618,17618,"辅饰纹",pPage2,false,0,0,0,FONT_YAHEI,FONTSIZE_SMALL);

	S_TabPage* pPage3 = AddTabPage(0,0,MAKELONG(22302,PACKAGE_INTERFACE),0,36 + 180,85,86,87,88,"纹佩图鉴",NULL,true,0,0,1);
	AddTabPage(0,0,MAKELONG(22302,PACKAGE_INTERFACE),51,34,17616,17617,17618,17618,"边 纹",pPage3,false,0,0,0,FONT_YAHEI,FONTSIZE_SMALL);
	AddTabPage(0,0,MAKELONG(22302,PACKAGE_INTERFACE),51 + 50 * 1,34,17616,17617,17618,17618,"底 纹",pPage3,false,0,0,0,FONT_YAHEI,FONTSIZE_SMALL);
	AddTabPage(0,0,MAKELONG(22302,PACKAGE_INTERFACE),51 + 50 * 2,34,17616,17617,17618,17618,"主 纹",pPage3,false,0,0,0,FONT_YAHEI,FONTSIZE_SMALL);
	AddTabPage(0,0,MAKELONG(22302,PACKAGE_INTERFACE),51 + 50 * 3,34,17616,17617,17618,17618,"辅饰纹",pPage3,false,0,0,0,FONT_YAHEI,FONTSIZE_SMALL);
}


CWenPeiWnd::~CWenPeiWnd(void)
{
	
}

void CWenPeiWnd::SetCtrlAll0()
{
	m_pScroll = 0;
	m_pTiQu = 0;
	m_pHeCheng1 = 0;
	m_pHeCheng = 0;
	m_pFenJie = 0;
	for ( int row = 0; row < PAGE_INFO_ROWS; ++row)
	{
		for ( int col = 0; col < PAGE_INFO_COLS; ++col)
		{
			m_pTaskBtn[row][col] = 0;
		}
	}
}

void CWenPeiWnd::OnCreate()
{
	m_iSuccessFrame = 10000;

	SetCtrlAll0();

	SetCloseButton(438,4, 80);

	int iCurPage = m_TabPage.iCurPage;

	if (iCurPage == PAGE_INDEX_HECHENG)
	{
		m_pHeCheng1 = new CCtrlButton;
		m_pHeCheng1->CreateEx(this,355,270,90,91,92,93);
		m_pHeCheng1->SetText("一键合成",COLOR_BTN_NORMAL,COLOR_BTN_MOUSEON,COLOR_BTN_PRESS,COLOR_BTN_DISABLE,FONTSIZE_DEFAULT,DTF_BlackFrame);
		AddControl(m_pHeCheng1);		

		m_pHeCheng = new CCtrlButton;
		m_pHeCheng->CreateEx(this,355,300,90,91,92,93);
		m_pHeCheng->SetText("合 成",COLOR_BTN_NORMAL,COLOR_BTN_MOUSEON,COLOR_BTN_PRESS,COLOR_BTN_DISABLE,FONTSIZE_DEFAULT,DTF_BlackFrame);
		AddControl(m_pHeCheng);
		m_pHeCheng->SetEnable(false);

		m_pScroll = new CCtrlScroll();
		m_pScroll->CreateEx(this, 434, 65, 17, 157);
		m_pScroll->SetPos(0);	
		m_pScroll->SetStep(1);
		AddControl(m_pScroll);		
	}
	else if(iCurPage == PAGE_INDEX_FENJIE)	
	{
		m_pTiQu = new CCtrlButton;
		m_pTiQu->CreateEx(this,355,270,90,91,92,93);
		m_pTiQu->SetText("提 取",COLOR_BTN_NORMAL,COLOR_BTN_MOUSEON,COLOR_BTN_PRESS,COLOR_BTN_DISABLE,FONTSIZE_DEFAULT,DTF_BlackFrame);
		AddControl(m_pTiQu);
		m_pTiQu->SetEnable(false);

		m_pFenJie = new CCtrlButton;
		m_pFenJie->CreateEx(this,355,300,90,91,92,93);
		m_pFenJie->SetText("分 解",COLOR_BTN_NORMAL,COLOR_BTN_MOUSEON,COLOR_BTN_PRESS,COLOR_BTN_DISABLE,FONTSIZE_DEFAULT,DTF_BlackFrame);
		AddControl(m_pFenJie);
		m_pFenJie->SetEnable(false);

		m_pScroll = new CCtrlScroll();
		m_pScroll->CreateEx(this, 434, 65, 17, 157);
		m_pScroll->SetPos(0);	
		m_pScroll->SetStep(1);
		AddControl(m_pScroll);	
	}
	else if ( iCurPage == PAGE_INDEX_INFO )
	{
		m_pScroll = new CCtrlScroll();
		m_pScroll->CreateEx(this, 434, 46, 17, 280);
		m_pScroll->SetPos(0);
		m_pScroll->SetStep(1);
		AddControl(m_pScroll);

		for ( int row = 0; row < PAGE_INFO_ROWS; ++row)
		{
			for ( int col = 0; col < PAGE_INFO_COLS; ++col)
			{
				m_pTaskBtn[row][col] = new CCtrlButton();
				
				int mvx = PAGE_INFO_START_X + (PAGE_INFO_CELL_W + PAGE_INFO_INTER_W) * col + 78; 
				int mvy = PAGE_INFO_START_Y + (PAGE_INFO_CELL_H + PAGE_INFO_INTER_H) * row + 8;

				m_pTaskBtn[row][col]->CreateEx(this, mvx,mvy, 165,166,167,168);
				m_pTaskBtn[row][col]->SetText("领取任务", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);

				AddControl(m_pTaskBtn[row][col]);
				m_pTaskBtn[row][col]->SetShow(false);
				m_pTaskBtn[row][col]->SetEnable(false);
			}
		}
	}

	CCtrlWindowX::OnCreate();

	UpdateWenPeiInfo();	

	if (!g_pGameData->HasPaoPaoStatus(EP_First_Open_WeiPeiWnd))
	{
		AddArrowTip(EP_First_Level_30,27,36 + 180 + 40,XAL_TOPLEFT,false,XAL_TOPLEFT);
	}

}


bool CWenPeiWnd::Msg(DWORD dwMsg, DWORD dwData, CControl *pControl)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_TABPAGE_TABCHANGE:
		{
			if (pControl)
			{
				S_TabPage * pParentTabpage = (S_TabPage * )pControl;
				if (pParentTabpage == &m_TabPage && dwData == 2)//点了纹配图鉴页签
				{
					this->Msg(MSG_REMOVE_ARROWTIP_CONTROL,EP_First_Open_WeiPeiWnd,NULL);
					return false;
				}
			}
		}
		break;
	case MSG_CTRL_WENPEI_WND:
		{
			if (dwData == 10)
			{
				int subpage = (int)(ULONG_PTR)pControl;
				subpage -= 1;
				if (subpage >= 0 && subpage < 4)
				{
					SwitchToPage(subpage,&(m_TabPage.vSubTabPage[0]),true);
				}
				return true;
			}
			else if (dwData == 9)//update button state
			{
				UpdateWenPeiInfo();
			}
		}
		break;
	case MSG_CTRL_BUTTON_CLICK:
		{
			int iCurPage = m_TabPage.iCurPage;
			if (pControl == m_pHeCheng1)
			{
				if(iCurPage == PAGE_INDEX_HECHENG)
				{
					int id[4] = {-1,-1,-1,-1};

					for (int i = 0; i < 4; ++i)
					{
						VSubWenPei& lSubWenPeis0 = g_OtherData.GetSubWenPei(i);
						if (!lSubWenPeis0.empty())
						{
							SubWenPei& lSubWenPei = lSubWenPeis0[0];
							id[i] = lSubWenPei.id;
						}
						else
						{
							switch (i)
							{
							case 0:
								g_MsgBoxMgr.PopSimpleAlert("没有边纹部件，无法合成圣殿纹佩。");
								break;
							case 1:
								g_MsgBoxMgr.PopSimpleAlert("没有底纹部件，无法合成圣殿纹佩。");
								break;
							case 2:
								g_MsgBoxMgr.PopSimpleAlert("没有主纹部件，无法合成圣殿纹佩。");
								break;
							case 3:
								g_MsgBoxMgr.PopSimpleAlert("没有辅饰纹纹部件，无法合成圣殿纹佩。");
								break;
							}
							return true;
						}

						if (id[0] != -1 && id[1] != -1 && id[2] != -1 && id[3] != -1)
						{
							g_pGameControl->SEND_WenPei_Compound(id[0],id[1],id[2],id[3]);
							m_iSuccessFrame = 0;
						}
					}
				}
				return true;
			}
			else if (pControl == m_pHeCheng)
			{
				if(iCurPage == PAGE_INDEX_HECHENG)
				{
					int id[4] = {-1,-1,-1,-1};

					for (int i = 0; i < 4; ++i)
					{
						VSubWenPei& lSubWenPeis0 = g_OtherData.GetSubWenPei(i);
						if (m_iSelectIndex[0][i] >= 0 && m_iSelectIndex[0][i] < lSubWenPeis0.size())
						{
							SubWenPei& lSubWenPei = lSubWenPeis0[m_iSelectIndex[0][i]];
							id[i] = lSubWenPei.id;
						}
						else
						{
							return true;
						}
					}

					if (id[0] != -1 && id[1] != -1 && id[2] != -1 && id[3] != -1)
					{
						g_pGameControl->SEND_WenPei_Compound(id[0],id[1],id[2],id[3]);
						m_iSuccessFrame = 0;
						for (int i = 0; i < 4; ++i)
						{
							m_iSelectIndex[0][i] = -1;
						}
					}
				}
				return true;
			}
			else if (pControl == m_pFenJie)
			{
				if(iCurPage == PAGE_INDEX_FENJIE)
				{
					int iSubPage = m_TabPage.vSubTabPage[iCurPage].iCurPage;

					if (iSubPage >= 0 && iSubPage < 4)
					{
						VSubWenPei& lSubWenPeis = g_OtherData.GetSubWenPei(iSubPage);

						if (m_iSelectIndex[iCurPage][iSubPage] >= 0 && m_iSelectIndex[iCurPage][iSubPage] < lSubWenPeis.size())
						{
							SubWenPei& lSubWenPei = lSubWenPeis[ m_iSelectIndex[iCurPage][iSubPage] ];
							g_pGameControl->SEND_WenPeiSub_Decompound(iSubPage, lSubWenPei.id);
						}
					}
				}

				return true;
			}
			else if (pControl == m_pTiQu)
			{
				if(iCurPage == PAGE_INDEX_FENJIE)
				{
					int iSubPage = m_TabPage.vSubTabPage[iCurPage].iCurPage;

					if (iSubPage >= 0 && iSubPage < 4)
					{
						VSubWenPei& lSubWenPeis = g_OtherData.GetSubWenPei(iSubPage);

						if (m_iSelectIndex[iCurPage][iSubPage] >= 0 && m_iSelectIndex[iCurPage][iSubPage] < lSubWenPeis.size())
						{
							SubWenPei& lSubWenPei = lSubWenPeis[ m_iSelectIndex[iCurPage][iSubPage] ];
							g_pGameControl->SEND_WenPeiSub_TiQu(iSubPage, lSubWenPei.id);
						}
					}
				}

				return true;
			}

			if(iCurPage == PAGE_INDEX_INFO)
			{
				for ( int row = 0; row < PAGE_INFO_ROWS; ++row)
				{
					for ( int col = 0; col < PAGE_INFO_COLS; ++col)
					{
						if (pControl == m_pTaskBtn[row][col])
						{
							int iSubPage = m_TabPage.vSubTabPage[iCurPage].iCurPage;
							if (iSubPage < 0 || iSubPage >= 4)
							{
								return true;
							}

							VWenPeiTask& lWenPeiTask = g_XmlCfg.GetWenPeiTasks(iSubPage);

							int iPos = m_pScroll->GetPos();
							int iCur = (row + iPos) * PAGE_INFO_COLS + col;

							if (iCur >= lWenPeiTask.size())
							{
								return true;
							}

							char szCommand[256] = {0};
							sprintf(szCommand,"@ArmoryTask&%d",lWenPeiTask[iCur].looks);

							g_pGameControl->SEND_Exchange_Menu_Click(DWORD(-8),szCommand);
							return true;
						}
					}
				}
			}

		}
		break;
	case MSG_CTRL_SCROLL_MOVE:
		{
			if (pControl == m_pScroll)
			{
				UpdateWenPeiInfo();
				return true;
			}
		}
		break;
	default:
		break;
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}


void CWenPeiWnd::UpdateWenPeiInfo()
{
	int iCurPage = m_TabPage.iCurPage;
	if(iCurPage == PAGE_INDEX_INFO)
	{
		int iSubPage = m_TabPage.vSubTabPage[iCurPage].iCurPage;
		if (iSubPage < 0 || iSubPage >= 4)
		{
			return;
		}

		VWenPeiTask& lSubWenPeis = g_XmlCfg.GetWenPeiTasks(iSubPage);
		int iCount = lSubWenPeis.size();

		int iCur = 0;
		int liPos = m_pScroll->GetPos();

		for ( int row = 0; row < PAGE_INFO_ROWS; ++row)
		{
			for ( int col = 0; col < PAGE_INFO_COLS; ++col)
			{
				m_pTaskBtn[row][col]->SetShow(false);
				m_pTaskBtn[row][col]->SetEnable(false);

				iCur = row * PAGE_INFO_COLS + col + liPos * PAGE_INFO_COLS;				

				if (iCur < iCount)
				{
					sWenPeiTask& lInfo = lSubWenPeis[iCur];

					m_pTaskBtn[row][col]->SetShow(true);

					//1:边纹,2:底纹,3:主纹,4:辅纹
					int type = (lInfo.looks - 6000) / 100;					

					if (lInfo.taskstate == 0)
					{
						m_pTaskBtn[row][col]->SetEnable(true);
						
						switch (type)
						{
						case 1:
							m_pTaskBtn[row][col]->SetText("我要边纹", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);
							break;
						case 2:
							m_pTaskBtn[row][col]->SetText("我要底纹", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);
							break;
						case 3:
							m_pTaskBtn[row][col]->SetText("我要主纹", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);
							break;
						case 4:
							m_pTaskBtn[row][col]->SetText("我要辅纹", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);
							break;
						}
					}
					else if (lInfo.taskstate == 1)
					{
						m_pTaskBtn[row][col]->SetEnable(false);
						m_pTaskBtn[row][col]->SetText("进行中", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);
					}
					else if (lInfo.taskstate == 2)
					{
						m_pTaskBtn[row][col]->SetEnable(true);

						switch (type)
						{
						case 1:
							m_pTaskBtn[row][col]->SetText("领取边纹", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);
							break;
						case 2:
							m_pTaskBtn[row][col]->SetText("领取底纹", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);
							break;
						case 3:
							m_pTaskBtn[row][col]->SetText("领取主纹", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);
							break;
						case 4:
							m_pTaskBtn[row][col]->SetText("领取辅纹", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);
							break;
						}
					}
					else if (lInfo.taskstate == 3)
					{
						m_pTaskBtn[row][col]->SetEnable(false);
						m_pTaskBtn[row][col]->SetText("已领取", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);
					}
				}
			}
		}
	}
}



void CWenPeiWnd::Draw()
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	int iCurPage = m_TabPage.iCurPage;

	if (iCurPage == PAGE_INDEX_HECHENG)
		g_pFont->DrawText(m_iScreenX + 210,m_iScreenY + 10,"纹佩包裹",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG,DTF_BlackFrame);
	else if (iCurPage == PAGE_INDEX_FENJIE)
		g_pFont->DrawText(m_iScreenX + 210,m_iScreenY + 10,"纹佩分解",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG,DTF_BlackFrame);
	else if (iCurPage == PAGE_INDEX_INFO)
		g_pFont->DrawText(m_iScreenX + 210,m_iScreenY + 10,"纹佩图鉴",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG,DTF_BlackFrame);

	
	int cols = 0;
	int rows = 0;
	int startx = 0;
	int starty = 0;
	int cellw = 0;
	int cellh = 0;
	int interw = 0;
	int interh = 0;
	int drawtype = 2;

	if (iCurPage == PAGE_INDEX_HECHENG || iCurPage == PAGE_INDEX_FENJIE)
	{
		int iSubPage = m_TabPage.vSubTabPage[iCurPage].iCurPage;

		if (iSubPage < 0 || iSubPage >= 4)
		{
			return;
		}

		VSubWenPei& lSubWenPeis = g_OtherData.GetSubWenPei(iSubPage);
		int iCount = lSubWenPeis.size();		

		//辅纹
		if (iSubPage == 3)
		{
			startx = PAGE_SMALLICON_START_X;
			starty = PAGE_SMALLICON_START_Y;
			cellw = PAGE_SMALLICON_CELL_W;
			cellh = PAGE_SMALLICON_CELL_H;
			interw = PAGE_SMALLICON_INTER_W;
			interh = PAGE_SMALLICON_INTER_H;
			cols = PAGE_SMALLICON_COLS;
			rows = PAGE_SMALLICON_ROWS;
			drawtype = 1;
		}
		else
		{
			startx = PAGE_LARGEICON_START_X;
			starty = PAGE_LARGEICON_START_Y;
			cellw = PAGE_LARGEICON_CELL_W;
			cellh = PAGE_LARGEICON_CELL_H;
			interw = PAGE_LARGEICON_INTER_W;
			interh = PAGE_LARGEICON_INTER_H;
			cols = PAGE_LARGEICON_COLS;
			rows = PAGE_LARGEICON_ROWS;
			drawtype = 2;
		}

		bool fjenabled = false;

		m_pScroll->SetRange((iCount - 1) / cols + 1 - rows);
		int iPos = m_pScroll->GetPos();
		
		int liSelect = m_iSelectIndex[iCurPage][iSubPage] - iPos * cols;
		if(liSelect >= 0 && liSelect < rows*cols)
		{
			int iSelRow = liSelect/cols;
			int iSelCol = liSelect%cols;

			//画选中框
			g_pGfx->DrawFillRect(m_iScreenX + startx + (cellw + interw) * iSelCol,
							m_iScreenY + starty + (cellh + interh) * iSelRow,
							cellw,cellh,0x44FF0000);
		}		

		int iCur = 0;		

		for ( int row = 0; row < rows; ++row)
		{
			for ( int col = 0; col < cols; ++col)
			{
				iCur = row * cols + col + iPos * cols;
				if (iCur >= iCount)
					break;
				
				DrawSubWenPei(m_iScreenX + startx + (cellw + interw) * col, 
						m_iScreenY + starty + (cellh + interh) * row, 
						lSubWenPeis[iCur].looks, NULL,drawtype);

				fjenabled = true;
			}

			if (iCur >= iCount)
				break;
		}

		if (iCurPage == PAGE_INDEX_HECHENG)
		{
			// 合成图		
			sWenPeiInfo * pWenPeiInfo = NULL;
			char str[256] = {0};
			bool enabled = true;

			int iDrawX = 143;
			int iDrawY = 257;

			for (int i = 0; i < 4; ++i)
			{
				int index = g_OtherData.GetSubWenPeiDraw(i);
				VSubWenPei& lSubWenPeis0 = g_OtherData.GetSubWenPei(index);
				if (m_iSelectIndex[0][index] >= 0 && m_iSelectIndex[0][index] < lSubWenPeis0.size())
				{
					SubWenPei& lSubWenPei = lSubWenPeis0[ m_iSelectIndex[0][index] ];
					DrawSubWenPei(m_iScreenX + 53, m_iScreenY + 252, 
						lSubWenPei.looks, NULL,2);


					pWenPeiInfo = g_XmlCfg.GetWenPeiCfg(lSubWenPei.looks);
					if(pWenPeiInfo)
					{
						sprintf(str,"%s:%s",pWenPeiInfo->strType.c_str(),pWenPeiInfo->strValue.c_str());
						g_pFont->DrawText(m_iScreenX + iDrawX,m_iScreenY + iDrawY,str,0xFF9E6D34,FONT_DEFAULT,FONTSIZE_DEFAULT);
						iDrawY += 16;
					}
				}
				else
				{
					enabled = false;
				}
			}

			m_pHeCheng->SetEnable(enabled);


			//合成成功特效
			if (m_iSuccessFrame < 40)
			{		
				LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,4722+m_iSuccessFrame / 4,EP_UI);
				if(pTex)
				{
					g_pGfx->DrawTextureNL(m_iScreenX - 70 - 40,m_iScreenY - 10 + 40,pTex);
				}

				++m_iSuccessFrame;
			}

		}
		else if (iCurPage == PAGE_INDEX_FENJIE)
		{
			m_pFenJie->SetEnable(fjenabled);
			m_pTiQu->SetEnable(fjenabled);


			char str[256] = {0};
			int iDrawX = 143;
			int iDrawY = 257;

			if (iSubPage >= 0 && iSubPage < 4)
			{
				if (m_iSelectIndex[iCurPage][iSubPage] >= 0 && m_iSelectIndex[iCurPage][iSubPage] < lSubWenPeis.size())
				{
					SubWenPei& lSubWenPei = lSubWenPeis[ m_iSelectIndex[iCurPage][iSubPage] ];

					DrawSubWenPei(m_iScreenX + 53, m_iScreenY + 252, lSubWenPei.looks, NULL,2);

					sWenPeiInfo * pWenPeiInfo = g_XmlCfg.GetWenPeiCfg(lSubWenPei.looks);
					if(pWenPeiInfo)
					{
						for ( int irate = 0; irate < 3; ++irate)
						{
							if (pWenPeiInfo->rate[irate] > 0)
							{
								if (irate == 0)
								{
									sprintf(str,"初级纹佩精华:%d%%",pWenPeiInfo->rate[irate]);
								}
								else if (irate == 1)
								{
									sprintf(str,"中级纹佩精华:%d%%",pWenPeiInfo->rate[irate]);
								}
								else if (irate == 2)
								{
									sprintf(str,"高级纹佩精华:%d%%",pWenPeiInfo->rate[irate]);
								}

								g_pFont->DrawText(m_iScreenX + iDrawX,m_iScreenY + iDrawY,str,0xFF9E6D34,FONT_DEFAULT,FONTSIZE_DEFAULT);
								iDrawY += 16;
							}
						}

						if (pWenPeiInfo->shengwang > 0)
						{
							sprintf(str,"分解此纹佩需要消耗%d点声望值",pWenPeiInfo->shengwang);
							g_pFont->DrawText(m_iScreenX + iDrawX,m_iScreenY + iDrawY,str,0xFF9E6D34,FONT_DEFAULT,FONTSIZE_DEFAULT);
							iDrawY += 16;
						}
					}
				}
			}
		}
	}	
	else if ( iCurPage == PAGE_INDEX_INFO )
	{
		int iSubPage = m_TabPage.vSubTabPage[iCurPage].iCurPage;

		if (iSubPage < 0 || iSubPage >= 4)
		{
			return;
		}

		VWenPeiTask& lSubWenPeis = g_XmlCfg.GetWenPeiTasks(iSubPage);		

		int iCount = lSubWenPeis.size();

		m_pScroll->SetRange((iCount - 1) / PAGE_INFO_COLS + 1 - PAGE_INFO_ROWS);
		int iPos = m_pScroll->GetPos();	
		
		int iCur = 0;
		int iDrawX = 0;
		int iDrawY = 0;
		LPTexture pTex = NULL;

		for ( int row = 0; row < PAGE_INFO_ROWS; ++row)
		{
			for ( int col = 0; col < PAGE_INFO_COLS; ++col)
			{
				iCur = row * PAGE_INFO_COLS + col + iPos * PAGE_INFO_COLS;

				if (iCur >= iCount)
					break;
			
				const sWenPeiTask& lInfo = lSubWenPeis[iCur];

				iDrawX = m_iScreenX + PAGE_INFO_START_X + (PAGE_INFO_CELL_W + PAGE_INFO_INTER_W) * col;
				iDrawY = m_iScreenY + PAGE_INFO_START_Y + (PAGE_INFO_CELL_H + PAGE_INFO_INTER_H) * row;
				DrawSubWenPei(iDrawX, iDrawY, lInfo.looks, NULL, 2);				

				if (lInfo.iMaxNum > 0 && lInfo.iFinishNum > 0)
				{
					pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22303,EP_UI);
					if (pTex)
					{
						int iDrawW = (pTex->GetWidth() * lInfo.iFinishNum) / lInfo.iMaxNum;
						g_pGfx->DrawPartTexture(iDrawX + 84,iDrawY + 60,pTex,0,0,iDrawW);
					}
				}
			}

			if (iCur >= iCount)
				break;
		}

	}	
}

bool CWenPeiWnd::OnMouseMove(int iX, int iY)
{
	CParserTip* pTip = g_pControl->GetTipWnd();
	if(!pTip)
		return true;

	pTip->Clear();
	pTip->SetShow(false);

	int iCurPage = m_TabPage.iCurPage;
	if (iCurPage == PAGE_INDEX_HECHENG || iCurPage == PAGE_INDEX_FENJIE)
	{
		int iSubPage = m_TabPage.vSubTabPage[iCurPage].iCurPage;

		if (iSubPage >= 0 && iSubPage < 4)
		{
			int index = GetIndex(iX,iY);
			if(index >= 0)
			{
				VSubWenPei& lSubWenPeis = g_OtherData.GetSubWenPei(iSubPage);				

				int iCount = lSubWenPeis.size();
				if(index < iCount)
				{
					char str[64] = {0};
					SubWenPei& lSub = lSubWenPeis[index];

					sWenPeiInfo* pWenPeiInfo = g_XmlCfg.GetWenPeiCfg(lSub.looks);
					if(pWenPeiInfo)
					{
						pTip->AddText(pWenPeiInfo->name.c_str(),TIPS_GOODNAME,-1,0,-1,FONT_DEFAULT,FONTSIZE_MIDDLE);
						sprintf(str,"数量%d",lSub.nums);
						pTip->AddText(str);

						//配置物品说明
						sTipsCfg * pTipCfg = g_TipsCfg.GetCfgTips(pWenPeiInfo->name.c_str());
						if(pTipCfg)
						{
							pTip->AddCfgTips(pTipCfg);
						}
					}

					int x = m_iScreenX + iX + 10;
					int y = m_iScreenY + iY + 10;
					pTip->AdjustXY(x,y);
					pTip->Move(x,y);
					pTip->SetShow(true);
					return true;
				}
			}
		}
	}
	else if (iCurPage == PAGE_INDEX_INFO)
	{
		int iSubPage = m_TabPage.vSubTabPage[iCurPage].iCurPage;

		if (iSubPage >= 0 && iSubPage < 4)
		{
			int index = GetIndex(iX,iY);
			if(index >= 0)
			{
				VWenPeiTask& lSubWenPeis = g_XmlCfg.GetWenPeiTasks(iSubPage);
				if(index < lSubWenPeis.size())
				{				
					sWenPeiTask& lSub = lSubWenPeis[index];

					//pTip->AddText(lSub->name.c_str(),TIPS_GOODNAME,-1,0,-1,FONT_DEFAULT,FONTSIZE_MIDDLE);				

					pTip->AddText(lSub.content[0].c_str(), 0xFFFFFFFF);
					pTip->AddText(lSub.content[1].c_str(), 0xFFFFCF63);

					int x = m_iScreenX + iX + 10;
					int y = m_iScreenY + iY + 10;
					pTip->AdjustXY(x,y);
					pTip->Move(x,y);
					pTip->SetShow(true);
					return true;
				}
			}

			index = GetTaskProgressIndex(iX,iY);
			if(index >= 0)
			{
				VWenPeiTask& lSubWenPeis = g_XmlCfg.GetWenPeiTasks(iSubPage);
				if(index < lSubWenPeis.size())
				{
					char str[64] = {0};
					sWenPeiTask& lSub = lSubWenPeis[index];

					if (lSub.taskstate == 1)
					{
						sprintf(str,"进度：%d/%d", lSub.iFinishNum, lSub.iMaxNum);
						pTip->AddText(str, 0xFFFFFFFF);

						int x = m_iScreenX + iX + 10;
						int y = m_iScreenY + iY + 10;
						pTip->AdjustXY(x,y);
						pTip->Move(x,y);
						pTip->SetShow(true);
						return true;
					}
				}
			}
		}		
	}

	return CCtrlWindowX::OnMouseMove(iX,iY);
}

bool CWenPeiWnd::OnLeftButtonDown(int iX,int iY)
{	
	int index = GetIndex(iX, iY);
	if (index >= 0)
		return true;
	
	return CCtrlWindowX::OnLeftButtonDown(iX,iY);
}

bool CWenPeiWnd::OnLeftButtonUp(int iX,int iY)
{
	int iCurPage = m_TabPage.iCurPage;
	if (iCurPage == PAGE_INDEX_HECHENG || iCurPage == PAGE_INDEX_FENJIE)
	{
		int iSubPage = m_TabPage.vSubTabPage[iCurPage].iCurPage;

		if (iSubPage >= 0 && iSubPage < 4)
		{
			int index = GetIndex(iX, iY);
			if (index >= 0)
			{
				VSubWenPei& lSubWenPeis = g_OtherData.GetSubWenPei(iSubPage);				

				int iCount = lSubWenPeis.size();
				if(index < iCount)
				{
					m_iSelectIndex[iCurPage][iSubPage] = index;					
				}

				return true;
			}			
		}
	}	

	return CCtrlWindowX::OnLeftButtonUp(iX,iY);
}


int CWenPeiWnd::GetIndex(int iX,int iY)
{
	int cols = 0;
	int rows = 0;
	int startx = 0;
	int starty = 0;
	int cellw = 0;
	int cellh = 0;
	int interw = 0;
	int interh = 0;

	int page = m_TabPage.iCurPage;

	if (page == PAGE_INDEX_HECHENG || page == PAGE_INDEX_FENJIE)
	{
		int subpage = m_TabPage.vSubTabPage[page].iCurPage;
		//辅纹
		if (subpage == 3)
		{
			startx = PAGE_SMALLICON_START_X;
			starty = PAGE_SMALLICON_START_Y;
			cellw = PAGE_SMALLICON_CELL_W;
			cellh = PAGE_SMALLICON_CELL_H;
			interw = PAGE_SMALLICON_INTER_W;
			interh = PAGE_SMALLICON_INTER_H;
			cols = PAGE_SMALLICON_COLS;
			rows = PAGE_SMALLICON_ROWS;
		}
		else
		{
			startx = PAGE_LARGEICON_START_X;
			starty = PAGE_LARGEICON_START_Y;
			cellw = PAGE_LARGEICON_CELL_W;
			cellh = PAGE_LARGEICON_CELL_H;
			interw = PAGE_LARGEICON_INTER_W;
			interh = PAGE_LARGEICON_INTER_H;
			cols = PAGE_LARGEICON_COLS;
			rows = PAGE_LARGEICON_ROWS;
		}

		if(iY <= starty || iY >= starty+rows*(cellh + interh))
			return -1;
		if(iX <= startx || iX >= startx+cols*(cellw+interw))
			return -1;

		int iRow = (iY-starty)/(cellh + interh);
		int iCol = (iX-startx)/(cellw+interw);
		iRow += m_pScroll->GetPos();

		int index = iRow * cols + iCol;
		return index;
	}	
	else if ( page == PAGE_INDEX_INFO )
	{
		if(iY <= PAGE_INFO_START_Y || iY >= PAGE_INFO_START_Y+PAGE_INFO_ROWS*(PAGE_INFO_CELL_H + PAGE_INFO_INTER_H))
			return -1;
		if(iX <= PAGE_INFO_START_X || iX >= PAGE_INFO_START_X+PAGE_INFO_COLS*(PAGE_INFO_CELL_W+PAGE_INFO_INTER_W))
			return -1;

		int iRow = (iY-PAGE_INFO_START_Y)/(PAGE_INFO_CELL_H + PAGE_INFO_INTER_H);
		int iCol = (iX-PAGE_INFO_START_X)/(PAGE_INFO_CELL_W+PAGE_INFO_INTER_W);

		int iOffsetY = (iY-PAGE_INFO_START_Y)%(PAGE_INFO_CELL_H + PAGE_INFO_INTER_H);
		int iOffsetX = (iX-PAGE_INFO_START_X)%(PAGE_INFO_CELL_W+PAGE_INFO_INTER_W);
		
		if (iOffsetX > 72 || iOffsetY > 72)
		{
			return -1;
		}

		iRow += m_pScroll->GetPos();

		int index = iRow * PAGE_INFO_COLS + iCol;
		return index;
	}

	return -1;
}

int CWenPeiWnd::GetTaskProgressIndex(int iX,int iY)
{
	int page = m_TabPage.iCurPage;
	if ( page == PAGE_INDEX_INFO )
	{
		if(iY <= PAGE_INFO_START_Y || iY >= PAGE_INFO_START_Y+PAGE_INFO_ROWS*(PAGE_INFO_CELL_H + PAGE_INFO_INTER_H))
			return -1;
		if(iX <= PAGE_INFO_START_X || iX >= PAGE_INFO_START_X+PAGE_INFO_COLS*(PAGE_INFO_CELL_W+PAGE_INFO_INTER_W))
			return -1;

		int iRow = (iY-PAGE_INFO_START_Y)/(PAGE_INFO_CELL_H + PAGE_INFO_INTER_H);
		int iCol = (iX-PAGE_INFO_START_X)/(PAGE_INFO_CELL_W+PAGE_INFO_INTER_W);

		int iOffsetY = (iY-PAGE_INFO_START_Y)%(PAGE_INFO_CELL_H + PAGE_INFO_INTER_H);
		int iOffsetX = (iX-PAGE_INFO_START_X)%(PAGE_INFO_CELL_W+PAGE_INFO_INTER_W);

		if (iOffsetX > 83 && iOffsetX < 83 + 86 && iOffsetY > 52 && iOffsetY < 52 + 12)
		{
			iRow += m_pScroll->GetPos();

			int index = iRow * PAGE_INFO_COLS + iCol;
			return index;
		}		
	}

	return -1;
}


void CWenPeiWnd::DrawSubWenPei(int x, int y, int looks, LPPOS pScale, int type)
{
	int pack = CGoodGrid::ConvertType2ItemPack(type);	
	LPTexture pTex = g_pTexMgr->GetTex(pack,looks,EP_UI);

	if(pScale)
	{
		g_pGfx->DrawTextureFX(x,y,pTex,-1,NULL,pScale);
		SDrawGoodEffect("item",(int)(x + 16*pScale->fx),(int)(y + 16*pScale->fy),looks,pScale,type);
	}
	else
	{
		g_pGfx->DrawTextureNL(x,y,pTex);
		SDrawGoodEffect("item",x+16,y+16,looks,NULL,type);
	}
}

void CWenPeiWnd::SDrawGoodEffect(const char *strName,int iX,int iY,int dwClientLooks,LPPOS pScale,int iType)
{
	SItemEffect sItemEffect;
	char strTempLooks[32]={0};
	if(dwClientLooks != 0)
	{
		sprintf(strTempLooks,"%d",dwClientLooks);
	}
	else
	{
		return;
	}

	bool bEffect = false;
	if (strcmp(strName, "item") == 0 && iType == 2)
	{
		bEffect = g_ItemCfgMgr.ParseItemEffect("item2",strTempLooks,sItemEffect);
	}
	else
	{
		bEffect = g_ItemCfgMgr.ParseItemEffect(strName,strTempLooks,sItemEffect);
	}

	if(bEffect)
	{
		WORD wID = sItemEffect.iTextureID;		

		LPTexture pTex = g_pTexMgr->GetTex(CGoodGrid::ConvertType2StateItemPack(iType),wID,EP_UI);		

		g_pGfx->SetRenderMode(RenderMode(sItemEffect.iRenderMode));
		if(pTex)
		{
			if(pScale)
				g_pGfx->DrawTextureFX(iX + (int)(sItemEffect.iOffsetX*pScale->fx),iY + (int)(sItemEffect.iOffsetY*pScale->fy),pTex,-1,NULL,pScale);
			else
				g_pGfx->DrawTextureNL(iX + sItemEffect.iOffsetX,iY + sItemEffect.iOffsetY,pTex);
		}
		g_pGfx->SetRenderMode();
	}
}