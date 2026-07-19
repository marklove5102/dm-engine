#include "ComposeEquipWnd.h"
#include "BaseClass/Control/GoodGrid.h"
#include "GameData/GameData.h"
#include "GameControl/GameControl.h"
#include "GameData/NpcData.h"
#include "GameData/TalkMgr.h"
#include "GameData/MsgBoxMgr.h"
#include "Baseclass/Control/ParserTip.h"
#include "GameAI/AIGoodMgr.h"
#include "GameData/MagicDefine.h"


INIT_WND_POSX(CComposeEquipWnd,POS_AUTO,POS_AUTO)


CComposeEquipWnd::CComposeEquipWnd(void)
{
	m_pOK = 0;
	m_pCancle = 0;

	m_iType = sm_dwWindowType;
	m_iMsg = -1;

	m_iIndex = 22535;
	

	m_iUpFrame = 10000;
	m_GoodFrame = 10000;
	m_iRotFrame = -1;

	m_dwLianHunGoodid = 0;
	m_wShowIconLooks = 0;

	//预读	
	g_pTexMgr->PreLoad(PACKAGE_INTERFACE,22315,22331,EP_MAGIC);
	g_pTexMgr->PreLoad(PACKAGE_INTERFACE,22335,22351,EP_MAGIC);
	g_pTexMgr->PreLoad(PACKAGE_INTERFACE,22540,22557,EP_MAGIC);
}


CComposeEquipWnd::~CComposeEquipWnd(void)
{
	if(g_NPC.GetECGoodLeft().GetID()!=0)
	{
		SELF.PackageGood().BackToArray(g_NPC.GetECGoodLeft());
		g_NPC.GetECGoodLeft().SetID(0);
	}

	if(g_NPC.GetECGoodRight().GetID()!=0)
	{
		SELF.PackageGood().BackToArray(g_NPC.GetECGoodRight());
		g_NPC.GetECGoodRight().SetID(0);
	}
}

void CComposeEquipWnd::OnCreate()
{
	SetCloseButton(332,35, 80);

	m_pOK = new CCtrlButton;
	m_pOK->CreateEx(this,85,412,90,91,92,93);
	m_pOK->SetText("确定",COLOR_BTN_NORMAL,COLOR_BTN_MOUSEON,COLOR_BTN_PRESS,COLOR_BTN_DISABLE,FONTSIZE_DEFAULT,DTF_BlackFrame);
	AddControl(m_pOK);

	m_pCancle = new CCtrlButton;
	m_pCancle->CreateEx(this,240,412,90,91,92,93);
	m_pCancle->SetText("取 消",COLOR_BTN_NORMAL,COLOR_BTN_MOUSEON,COLOR_BTN_PRESS,COLOR_BTN_DISABLE,FONTSIZE_DEFAULT,DTF_BlackFrame);
	AddControl(m_pCancle);

	m_dwLianHunGoodid = 0;
}


bool CComposeEquipWnd::Msg(DWORD dwMsg, DWORD dwData, CControl *pControl)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_COMPOSEEQUIP_WND:
		{
			if (dwData == 1)
			{
				if (m_iType == 2)
				{
					CGood* pGood = (CGood*)pControl;
					m_dwLianHunGoodid = pGood->GetID();
					m_LianHunName = pGood->GetName();
					m_iHunLevel = pGood->GetAC();
				}	
				return true;
			}

			WORD wResult = HIWORD(dwData);
			WORD wType = LOWORD(dwData);

			/*		
			类型29,30
			0：成功
			1：非法物品
			2：物品错误，数量不足
			3：钱不够
			*/

			m_iMsg = wResult;
			

			if (wResult == 0)
			{
				m_iUpFrame = 0;

				if (m_iType == 2)
				{
					m_dwLianHunGoodid = 0;					

					// 升级成功后,需要重新点物品打开界面
					m_pOK->SetEnable(false);
				}

				if(g_NPC.GetECGoodLeft().GetID()!=0)
				{
					m_wShowIconLooks = g_NPC.GetECGoodLeft().GetLooks();
					SELF.PackageGood().BackToArray(g_NPC.GetECGoodLeft());
					g_NPC.GetECGoodLeft().SetID(0);
				}
			}
			else
			{
				switch (wResult)
				{
				case 1:
					g_MsgBoxMgr.PopSimpleAlert("放入的物品无法进行升级");
					break;
				case 2:
					g_MsgBoxMgr.PopSimpleAlert("放入的物品不正确或数量不足");
					break;
				case 3:
					g_MsgBoxMgr.PopSimpleAlert("金币不足或包裹已满");
					break;					
				}
			}		

			return true;
		}
		break;
	case MSG_CTRL_BUTTON_CLICK:
		{
			if (pControl == m_pOK)
			{
				if (m_iType == 1)
				{
					if (g_NPC.GetECGoodLeft().GetID() != 0 && g_NPC.GetECGoodRight().GetID() != 0)
					{
						m_iUpFrame = 10000;
						m_GoodFrame = 10000;
						m_iRotFrame = -1;

						g_pGameControl->SEND_Equip_Compose(g_NPC.GetECGoodRight().GetID(), g_NPC.GetECGoodLeft().GetID());
					}
				}
				else if (m_iType == 2)
				{
					if (g_NPC.GetECGoodLeft().GetID() != 0 && m_dwLianHunGoodid != 0)
					{
						m_iUpFrame = 10000;
						m_GoodFrame = 10000;
						m_iRotFrame = -1;

						m_wShowIconLooks = g_NPC.GetECGoodLeft().GetLooks();
						g_pGameControl->SEND_Szzp_Compose(g_NPC.GetECGoodLeft().GetID(), m_dwLianHunGoodid);
					}					
				}
				

				return true;
			}
			else if (pControl == m_pCancle)
			{
				OnClickCloseButton();
				return true;
			}
		}
		break;
	default:
		break;
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}


void CComposeEquipWnd::Draw()
{	
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	if (m_iType == 1)
	{
		g_pFont->DrawText(m_iScreenX + 165,m_iScreenY + 37,"炼魂炉",0xFF550F0F,FONT_YAHEI,16,DTF_BlackFrame,0,0xFFE0920C);		
	}
	else if (m_iType == 2)
	{
		g_pFont->DrawText(m_iScreenX + 132,m_iScreenY + 37,m_LianHunName.c_str(),0xFF550F0F,FONT_YAHEI,16,DTF_BlackFrame,0,0xFFE0920C);		
	}
	

	g_pFont->DrawText(m_iScreenX + 115,m_iScreenY + 382,"需要花费:",COLOR_TEXT_NORMAL,FONT_DEFAULT,FONTSIZE_SMALL,DTF_BlackFrame);
	g_pFont->DrawText(m_iScreenX + 180,m_iScreenY + 383,"1,000",COLOR_TEXT_NORMAL);

	LPTexture pTex = NULL;

	if (m_iRotFrame >= 0)
	{			
		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22307,EP_UI);
		if(pTex)
		{
			g_pGfx->DrawTextureFX(m_iScreenX + 199,m_iScreenY + 205,pTex,0xFFFFFFFF,0,0,0.0f,0.0f,m_iRotFrame * 8.0f);
		}			

		++m_iRotFrame;

		if (m_iRotFrame == 360/8)
		{
			m_iRotFrame = -1;
			m_GoodFrame = 0;
		}
	}
	pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22536,EP_UI);
	if(pTex)
	{
		g_pGfx->DrawTextureNL(m_iScreenX + 174 ,m_iScreenY + 183,pTex);			
	}
	

	if(g_NPC.GetECGoodLeft().GetID()!=0)
		CGoodGrid::DrawOneGood(m_iScreenX+178+20, m_iScreenY+186+20 + 132, g_NPC.GetECGoodLeft());
	

	if(g_NPC.GetECGoodRight().GetID()!=0)
		CGoodGrid::DrawOneGood(m_iScreenX+178+20, m_iScreenY+186+20, g_NPC.GetECGoodRight());

	

	if (m_iType == 2)
	{
		if (m_iMsg == 0 && m_wShowIconLooks != 0)
		{
			pTex = g_pTexMgr->GetTex(PACKAGE_items,m_wShowIconLooks,EP_UI);
			g_pGfx->DrawTextureNL(m_iScreenX+183,m_iScreenY+190,pTex);
		}
		else if (g_NPC.GetECGoodLeft().GetID()!=0 && g_NPC.GetECGoodLeft().GetMAC() < 7)
		{
			pTex = g_pTexMgr->GetTex(PACKAGE_items,g_NPC.GetECGoodLeft().GetLooks(),EP_UI);
			g_pGfx->DrawTextureNL(m_iScreenX+183,m_iScreenY+190,pTex);
		}
	}	

	if (m_GoodFrame < 64 + 80)
	{
		if(m_GoodFrame < 64)
		{
			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22315+m_GoodFrame / 4,EP_UI);
			if(pTex)
			{
				g_pGfx->SetRenderMode(RM_ADD2);
				g_pGfx->DrawTextureNL(m_iScreenX + 53,m_iScreenY + 44,pTex);
				g_pGfx->SetRenderMode();
			}
		}		

		++m_GoodFrame;

		if (m_GoodFrame == 64 + 80)
		{
			m_wShowIconLooks = 0;

			if (m_iType == 2)
			{
				CloseWindow();
			}
		}
	}

	if (m_iUpFrame < 51)
	{
		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22540+m_iUpFrame / 3,EP_UI);
		if(pTex)
		{			
			g_pGfx->DrawTextureNL(m_iScreenX + 72,m_iScreenY + 105,pTex);			
		}

		++m_iUpFrame;
		if (m_iUpFrame == 51)
		{
			m_iRotFrame = 0;
		}
	}
}

bool CComposeEquipWnd::OnMouseMove(int iX, int iY)
{
	CParserTip* pTip = g_pControl->GetTipWnd();
	if(!pTip)
		return true;

	pTip->Clear();
	pTip->SetShow(false);
	pTip->GetSelfEquipTips()->SetShow(false);

	if(iX > 178 && iX < 215 && iY > 186+132 && iY < 224+132)
	{		
		if(g_NPC.GetECGoodLeft().GetID() != 0)
		{
			pTip->Parse(g_NPC.GetECGoodLeft());			
		}
		else
		{
			if (m_iType == 1)
			{
				pTip->AddText("请放入强化所需材料！");
			}
			else if (m_iType == 2)
			{
				switch (m_iHunLevel)
				{
				case 1:
					pTip->AddText("请放入圣战级别之魂！");
					break;
				case 2:
					pTip->AddText("请放入神武级别之魂！");
					break;
				case 3:
					pTip->AddText("请放入战神级别之魂！");
					break;
				}
			}
			else
			{
				return CCtrlWindowX::OnMouseMove(iX,iY);
			}
		}

		int x = m_iScreenX + iX + 10;
		int y = m_iScreenY + iY + 10;
		pTip->AdjustXY(x,y);
		pTip->Move(x,y);
		pTip->SetShow(true);
		return true;
	}
	else if (m_iType == 2 && iX > 183 && iX < 183+37 && iY > 190 && iY < 190+37)
	{
		if(g_NPC.GetECGoodLeft().GetID()!=0 && g_NPC.GetECGoodLeft().GetMAC() < 7)
		{
			std::string strGoodName = g_NPC.GetECGoodLeft().GetName();
			size_t pos = strGoodName.find_first_of("(");
			if (pos != std::string::npos)
			{
				strGoodName = strGoodName.substr(0, pos);
				switch (g_NPC.GetECGoodLeft().GetMAC())
				{
				case 1:
					strGoodName = "将获得" + strGoodName + "(二阶)";
					break;
				case 2:
					strGoodName = "将获得" + strGoodName + "(三阶)";
					break;
				case 3:
					strGoodName = "将获得" + strGoodName + "(四阶)";
					break;
				case 4:
					strGoodName = "将获得" + strGoodName + "(五阶)";
					break;
				case 5:
					strGoodName = "将获得" + strGoodName + "(六阶)";
					break;
				case 6:
					strGoodName = "将获得" + strGoodName + "(七阶)";
					break;
				}					
			}
			pTip->AddText(strGoodName.c_str(), -1, -1);

		}
		else
		{
			return CCtrlWindowX::OnMouseMove(iX,iY);
		}

		int x = m_iScreenX + iX + 10;
		int y = m_iScreenY + iY + 10;
		pTip->AdjustXY(x,y);
		pTip->Move(x,y);
		pTip->SetShow(true);
		return true;
	}
	else if(m_iType == 1 && iX > 183 && iX < 183+37 && iY > 190 && iY < 190+37)
	{		
		if(g_NPC.GetECGoodRight().GetID() != 0)
		{
			pTip->Parse(g_NPC.GetECGoodRight());
		}
		else
		{
			if (m_iType == 1)
			{
				pTip->AddText("请放入需要强化的装备");
			}		
			else
			{
				return CCtrlWindowX::OnMouseMove(iX,iY);
			}
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

bool CComposeEquipWnd::OnLeftButtonUp(int iX,int iY)
{
	stGoodFrom GoodFrom = CGoodGrid::GetDropGoodFrom();

	int iStdMode = GoodFrom.DropGood.GetStdMode();
	int iShape = GoodFrom.DropGood.GetShape();

	if(iX > 178 && iX < 215 && iY > 186+132 && iY < 224+132)
	{
		if (m_iType == 1)
		{
			if (GoodFrom.DropGood.GetID() && (!(iStdMode == 103 && iShape == 1)))
			{
				g_TalkMgr.PushSysTalk("请放入强化所需材料！");
				return true;
			}
		}
		else if (m_iType == 2)
		{
			if ( GoodFrom.DropGood.GetID() && ( !(iStdMode == 103 && iShape == 1 && (GoodFrom.DropGood.GetAC() == m_iHunLevel)) ) )
			{
				switch (m_iHunLevel)
				{
				case 1:
					g_TalkMgr.PushSysTalk("请放入圣战级别之魂！");
					break;
				case 2:
					g_TalkMgr.PushSysTalk("请放入神武级别之魂！");
					break;
				case 3:
					g_TalkMgr.PushSysTalk("请放入战神级别之魂！");
					break;
				}
				
				return true;
			}
		}
		else
		{
			return true;
		}

		CGood tmp = GoodFrom.DropGood;
		GoodFrom.DropGood = g_NPC.GetECGoodLeft();
		g_NPC.GetECGoodLeft() = tmp;
		CGoodGrid::SetDropGoodFrom(GoodFrom);

		m_iMsg = -1;

		return true;
	}
	else if(iX > 183 && iX < 183+37 && iY > 190 && iY < 190+37)
	{
		if (m_iType == 1)
		{
			if (GoodFrom.DropGood.GetID() && (!g_AIGoodMgr.IsEquip(GoodFrom.DropGood)))
			{
				g_TalkMgr.PushSysTalk("请放入需要强化的装备!");
				return true;
			}
		}
		else
		{
			return true;
		}

		CGood tmp = GoodFrom.DropGood;
		GoodFrom.DropGood = g_NPC.GetECGoodRight();
		g_NPC.GetECGoodRight() = tmp;
		CGoodGrid::SetDropGoodFrom(GoodFrom);

		m_iMsg = -1;

		return true;
	}	

	return CCtrlWindowX::OnLeftButtonUp(iX,iY);
}
