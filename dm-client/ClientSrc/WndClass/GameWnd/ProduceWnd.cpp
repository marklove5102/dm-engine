#include "ProduceWnd.h"
#include "GameControl/GameControl.h"
#include "BaseClass/Control/ParserTip.h"
#include "baseclass/Control/GoodGrid.h"
#include "GameData/NpcData.h"
#include "GameControl/GameControl.h"
#include "GameData/GameData.h"
#include "GameData/MsgBoxMgr.h"
#include "Global/Interface/AudioInterface.h"
#include "GameData/NpcData.h"
#include "GameData/MagicDefine.h"
INIT_WND_POSX(CProduceWnd,POS_AUTO,POS_AUTO)

#define FrameSpeed 3
#define TotalFrame FrameSpeed * 60
CProduceWnd::CProduceWnd(void)
{
	m_Good.SetID(0);
	m_iType = sm_dwWindowType;//1是炼制2是铸造
	m_iProbabity = -1;
	m_iFrame = 0;
	m_bStartAin = false;
	m_bFine = false;
	m_iIndex = 17683;
	m_iFlag = 0;
	m_bDrawSucess = false;
	m_bDrawFoundSucess = false;
	m_bDrawFoundFail = false;
}

CProduceWnd::~CProduceWnd(void)
{//归还包裹	
	g_NPC.RetToPkgFromOperateGood(m_Good.GetID());
}

void CProduceWnd::OnCreate()
{
	SetCloseButton(497,10,80);

	m_pOkBtn = new CCtrlButton;
	AddControl(m_pOkBtn);
	

	m_pOkBtn->CreateEx(this,37,300,95,96,97,98);
	string str = "制 炼";
	if (m_iType == 2) str = "铸 造";
	m_pOkBtn->SetText(str.c_str(),COLOR_BTN_NORMAL,COLOR_BTN_MOUSEON,COLOR_BTN_PRESS,COLOR_BTN_DISABLE);	
	m_pOkBtn->SetFont(FONT_YAHEI,12);

	m_pMarkViewer = new CMarkViewer(16,4);
	AddControl(m_pMarkViewer);

	m_pMarkViewer->Create(this,115,242,388,79);
	m_pMarkViewer->SetTagText(&m_Tag);
	m_pMarkViewer->SetFont(FONT_YAHEI,12);
	if (m_iType == 1)
		 str = "<color=yellow 请放入需要制炼的配方，并在包裹中准备好制炼该物品所需的材料。>\\<color=yellow 每次制炼需要精力值，精力值可通过精力药水获得。>";
	else str = "<color=yellow 请放入需要铸造的配方，并在包裹中准备好铸造该装备所需的材料。>\\<color=yellow 每次铸造需要精力值，精力值可通过精力药水获得。>";
	
	m_Tag.Parse(str);
}

void CProduceWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}


	LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,(m_iType == 1)?17684:17685,EP_UI);
	if (pTex) g_pGfx->DrawTextureNL(m_iScreenX + 24,m_iScreenY + 46,pTex);

	string str = "制  炼";
	if (m_iType == 2) str = "铸  造";

	g_pFont->DrawText(m_iScreenX + 244,m_iScreenY + 17,str.c_str(),0xffff9434,FONT_DEFAULT,16,DTF_Bold|DTF_BlackFrame);

	CGoodGrid::DrawOneGood(m_iScreenX + 66,m_iScreenY + 264,m_Good);

	pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17341,EP_UI);
	if (pTex) 
	{
		g_pGfx->SetRenderMode(RM_ADD2);
		g_pGfx->DrawTextureNL(m_iScreenX + 33,m_iScreenY + 233,pTex);
		g_pGfx->SetRenderMode();
	}

	//绘制动画
	if (m_bStartAin)
	{
		WORD wID = m_iType == 2?17336:8601;
		if (m_iType == 2)
		{
			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,wID,EP_UI);
			if (pTex)
			{
				//开始绘制动画
				pTex->EnableSysAnim(false);
				pTex->SetCurFrame(m_iFrame / FrameSpeed);
				g_pGfx->DrawTextureNL(m_iScreenX - 53,m_iScreenY - 30,pTex);
			}	

			if(m_iFrame == 13 * FrameSpeed || m_iFrame == 21 * FrameSpeed || m_iFrame == 29 * FrameSpeed || m_iFrame == 37 * FrameSpeed || m_iFrame == 47 * FrameSpeed)
			{
				g_pAudio->Play(EAT_OTHER,915,g_pAudio->GetRand()++);
			}

			m_iFrame++;
			if (m_iFrame == 60 * FrameSpeed)
			{
				m_pOkBtn->SetEnable(true);
				m_bStartAin = false;
				if (m_Good.GetID() != 0) g_pGameControl->Send_Produce_Info(m_Good.GetID(),(m_iType == 1)?0:1);
				m_iFrame = 0;
			}
		}
		else
		{
			if (m_bFine)
			{
				pTex = g_pTexMgr->GetTex(PACKAGE_npc1,8601,EP_UI);
				if (pTex) g_pGfx->DrawTextureNL(m_iScreenX + 72,m_iScreenY - 16,pTex);

				pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17686,EP_UI);
				if (pTex) 
				{
					g_pGfx->DrawTextureNL(m_iScreenX + 200,m_iScreenY + 20,pTex);

					if (pTex->GetFrameNow() >= 14)
						m_bFine = false;
				}
			}
			else
			{
				pTex = g_pTexMgr->GetTex(PACKAGE_npc1,wID,EP_UI);
				if (pTex) g_pGfx->DrawTextureNL(m_iScreenX + 72,m_iScreenY - 16,pTex);

				static int isFrame = 0;
				int iCurFrame = isFrame / (FrameSpeed * 2);
				iCurFrame = iCurFrame % 10;
				if (iCurFrame > 5) iCurFrame = 10 - iCurFrame + 1;
				pTex = g_pTexMgr->GetTex(PACKAGE_npc1,8411 + iCurFrame,EP_UI);

				if (pTex) g_pGfx->DrawTextureNL(m_iScreenX + 200,m_iScreenY + 20,pTex);

				isFrame++;


				iCurFrame = m_iFrame / FrameSpeed;
				if (iCurFrame >= 8) iCurFrame = iCurFrame % 8;
				pTex = g_pTexMgr->GetTex(PACKAGE_npc1,wID + iCurFrame,EP_UI);
				if (pTex) g_pGfx->DrawTextureNL(m_iScreenX + 72,m_iScreenY - 16,pTex);

				m_iFrame++;
				if (m_iFrame == 60 * FrameSpeed)
				{
					m_pOkBtn->SetEnable(true);
					m_bStartAin = false;
					if (m_Good.GetID() != 0) g_pGameControl->Send_Produce_Info(m_Good.GetID(),(m_iType == 1)?0:1);
					m_iFrame = 0;
				}
			}
		}
	}
	else
	{
		WORD wID = m_iType == 2?17335:8601;
		if (m_iType == 2)
		{
			if (m_bDrawFoundFail || m_bDrawFoundSucess)
			{
				pTex = NULL;
				if (m_bDrawFoundSucess)
					pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17687,EP_UI);
				else if (m_bDrawFoundFail)
					pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17688,EP_UI);

				if (pTex)
				{
					pTex->EnableSysAnim(false);
					int iCurFrame = m_iFrame / 6;
					if (iCurFrame >= 9) iCurFrame = iCurFrame % 9;
					pTex->SetCurFrame(iCurFrame);
					g_pGfx->DrawTextureNL(m_iScreenX + 272,m_iScreenY + 36,pTex);
					m_iFrame++;
					if (m_iFrame >= 8 * 6)
					{
						m_iFrame = 0;
						pTex->SetCurFrame(0);
						m_bDrawFoundSucess = false;
						m_bDrawFoundFail = false;
					}
				}
			}
			else
			{
				pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,wID,EP_UI);
				if (pTex) g_pGfx->DrawTextureNL(m_iScreenX + 272,m_iScreenY + 36,pTex);
			}
		}
		else
		{
			pTex = g_pTexMgr->GetTex(PACKAGE_npc1,wID,EP_UI);
			if (pTex) g_pGfx->DrawTextureNL(m_iScreenX + 72,m_iScreenY - 16,pTex);

			static int iFrame = 0;
			int iCurFrame = iFrame / (FrameSpeed * 2);
			iCurFrame = iCurFrame % 10;
			if (iCurFrame > 5) iCurFrame = 10 - iCurFrame + 1;
			pTex = g_pTexMgr->GetTex(PACKAGE_npc1,8411 + iCurFrame,EP_UI);

			if (pTex) g_pGfx->DrawTextureNL(m_iScreenX + 200,m_iScreenY + 20,pTex);

			iFrame++;
		}
	}
	
	if (!m_bDrawSucess) return;


	if (m_iFlag == 1)
	{
		//成功
		if (m_iType == 2)
		{
			//铸造		
			g_pFont->DrawText(m_iScreenX + 164,m_iScreenY + 87,"铸造成功",0xffff9434,FONT_DEFAULT,16,DTF_BlackFrame);
		}
		else if (m_iType == 1)
		{
			//制炼
			g_pFont->DrawText(m_iScreenX + 164,m_iScreenY + 87,"制炼成功",0xffff9434,FONT_DEFAULT,16,DTF_BlackFrame);
		}
	}
	else if (m_iFlag == 2)
	{
		//失败
		if (m_iType == 2)
		{
			//铸造
			g_pFont->DrawText(m_iScreenX + 164,m_iScreenY + 87,"铸造失败",0xffff9434,FONT_DEFAULT,16,DTF_BlackFrame);
		}
		else if (m_iType == 1)
		{
			//制炼
			g_pFont->DrawText(m_iScreenX + 164,m_iScreenY + 87,"制炼失败",0xffff9434,FONT_DEFAULT,16,DTF_BlackFrame);
		}
	}
}

bool CProduceWnd::OnLeftButtonUp(int iX, int iY)
{
	if(iX > 50 && iX < 80 && iY > 245 && iY < 280)
	{
		if(CGoodGrid::GetDropGoodFrom().eFromWnd != Package_Wnd && CGoodGrid::GetDropGoodFrom().eFromWnd != Panel_Wnd)
		{
			g_MsgBoxMgr.PopSimpleAlert("请放入包裹栏中物品");
			return true;
		}

		stGoodFrom& GoodFrom = CGoodGrid::GetDropGoodFrom();
		g_NPC.AddOperateGood(GoodFrom.DropGood);
		CGood gootemp = m_Good;
		m_Good = GoodFrom.DropGood;
		GoodFrom.DropGood = gootemp;

		g_NPC.DelOperateGood(GoodFrom.DropGood.GetID());

		UpdateDescription(m_Good);
		
		return true;
	}

	return CCtrlWindowX::OnLeftButtonUp(iX,iY);
}

bool CProduceWnd::OnMouseMove(int iX, int iY)
{
	if(iX > 50 && iX < 80 && iY > 245 && iY < 280)
	{
		CParserTip *pTip = g_pControl->GetTipWnd();
		pTip->Clear();
		if(m_Good.GetID())
		{				
			pTip->Parse(m_Good);			
		}
		else
		{
			if (m_iType == 2)
				 pTip->AddText("请放入你需要铸造的配方！");
			else pTip->AddText("请放入你需要制炼的配方！");
		}

		int x = m_iScreenX + iX + 10;
		int y = m_iScreenY + iY + 10;
		pTip->AdjustXY(x,y);
		pTip->Move(x,y);
		pTip->SetShow(true);
		return true;
	}

	return CCtrlWindowX::OnMouseMove(iX,iY);
}

bool CProduceWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch (dwMsg)
	{
	case MSG_CTRL_CLEAN_PROCUDE_GOOD_INFO:
		{
			if (dwData == 0)
			{
				const char* pName = m_Good.GetName();
				const char* pSubName = strstr(pName,"配方");
				if (!pSubName) pSubName = strstr(pName,"图纸");

				if (pSubName)
				{
					string strName;
					strName.assign(pName,(int)(pSubName - pName));
					char ctemp[256] = {0};
					m_iFlag = 1;
					if (m_iType == 1)
						sprintf(ctemp,"你成功制炼出了%s。每次制炼成功，都会有概率的增加相应的技能经验",strName.c_str());
					else sprintf(ctemp,"你成功铸造出了%s。每次铸造成功，都会有概率的增加相应的技能经验",strName.c_str());

					g_MsgBoxMgr.PopSimpleAlert(ctemp);
				}
			}
			else m_iFlag = 2;

			if (dwData == 0 || dwData == 13)
			{
				if (m_iType == 2)
				{
					m_bDrawFoundSucess = false;
					m_bDrawFoundFail = false;

					if (dwData == 0) m_bDrawFoundSucess = true;
					else if (dwData == 13) m_bDrawFoundFail = true;
				}

				m_Good.SetID(0);
				UpdateDescription(m_Good);
			}

			m_bDrawSucess = true;
		}
		break;
	case MSG_CTRL_BUTTON_CLICK:
		if (pControl && pControl == m_pOkBtn)
		{
			m_iFlag = 0;
			if (m_Good.GetID() != 0)
			{
				m_bStartAin = true;
				m_bFine = true;
				m_iFrame = 0;
				m_pOkBtn->SetEnable(false);
			}
			else 
			{
				if (m_iType == 1)
					 g_MsgBoxMgr.PopSimpleAlert("请放入你要制炼物品的配方！");
				else g_MsgBoxMgr.PopSimpleAlert("请放入你要铸造物品的配方！");
			}
			return true;
		}
	default:
		break;
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CProduceWnd::UpdateDescription(CGood& good)
{
	m_Tag.Clear();

	m_bDrawSucess = false;
	string str;
	if (good.GetID() == 0) 
	{
		if (m_iType == 1)
			str = "<color=yellow 请放入需要制炼的配方，并在包裹中准备好制炼该物品所需的材料。>\\<color=yellow 每次制炼需要精力值，精力值可通过精力药水获得。>";
		else str = "<color=yellow 请放入需要铸造的配方，并在包裹中准备好铸造该装备所需的材料。>\\<color=yellow 每次铸造需要精力值，精力值可通过精力药水获得。>";
		m_Tag.Parse(str);
		return;
	}

	string strAddTips = g_ItemCfgMgr.ParseGoodsTips(good.GetName());
	
	//if (strAddTips.length() <= 0)
	//	return;
	
	int iMagicID = MAGICID_FOUNDSKILL;
	if(m_iType == 1) iMagicID = MAGICID_SMELTSKILL;

	CMagicData* pMagicData = SELF.GetProduceMagicEx(iMagicID);
	if (pMagicData)
	{
		if (m_iType == 2)
			 m_iProbabity = g_ItemCfgMgr.ParseRefFoundInfo("FOUND",good.GetName(),pMagicData->GetMagicLevel());
		else m_iProbabity = g_ItemCfgMgr.ParseRefFoundInfo("REFINE",good.GetName(),pMagicData->GetMagicLevel());
	}

	char ctemp[128] = {0};
	if (m_iProbabity == -1)
	{
		if (m_iType == 2)
			 sprintf(ctemp,"此物品不可铸造！");
		else sprintf(ctemp,"此物品不可制炼！");
		str = ctemp;
		m_pOkBtn->SetEnable(false);
	}
	else
	{
		sprintf(ctemp,"/成功概率：%d%%",m_iProbabity);
		strAddTips += ctemp;

		string astr[8] = {"初级","中级","高级","专家","大师","宗师","权威","王者"};
		int iLevel = pMagicData->GetMagicLevel();
		if (iLevel < 0) iLevel = 0;
		if (iLevel > 7) iLevel = 7;
		
		if (m_iType == 2)
			sprintf(ctemp,"你当前铸造等级为：%s，",astr[iLevel].c_str());
		else sprintf(ctemp,"你当前制炼等级为：%s，",astr[iLevel].c_str());
		str = ctemp;
		str += strAddTips;

		m_pOkBtn->SetEnable(true);
	}


	m_Tag.Parse(str,0,"/",64,0xFFFFFF00);
}