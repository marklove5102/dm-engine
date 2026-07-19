#include "AdventureWnd.h"
#include "Global/Interface/AudioInterface.h"
#include "GameControl/GameControl.h"
#include "GameData/NpcData.h"
#include "GameData/MsgBoxMgr.h"
#include "Baseclass/Control/ParserTip.h"
#include "GameAI/AIGoodMgr.h"

#include <cmath>

INIT_WND_POSX(CAdventureWnd,POS_AUTO,POS_AUTO)



// RotItem
void RotItem::Start()
{	
	m_fPos = 0;	
	m_iRotState = 2;
	m_iRotNum = 0;	
}
void RotItem::Stop()
{
	m_iRotState = 3;
}

void RotItem::GetFinalClip(int& out_iFinalTop, int& out_iFinalBottom, int iClipTop, int iClipBottom, int source_offy, int source_h)
{	
	iClipTop = iClipTop - source_offy;
	iClipBottom = iClipBottom - source_offy;

	out_iFinalTop = 0;
	out_iFinalBottom = 0;

	if (0 < iClipBottom && source_h > iClipTop)
	{
		out_iFinalTop = 0 > iClipTop ? 0 : iClipTop;
		out_iFinalBottom = source_h < iClipBottom ? source_h : iClipBottom;
	}
	else
	{
		out_iFinalTop = 0;
		out_iFinalBottom = 0;
	}
}

void RotItem::DrawClipTex(int x, int y, LPTexture pTex, int iClipTop, int iClipBottom)
{
	if (pTex == NULL)
		return;	

	int iFinalTop = 0;
	int iFinalBottom = 0;

	GetFinalClip(iFinalTop, iFinalBottom, iClipTop, iClipBottom, pTex->GetOffY(), pTex->GetHeight());

	if (iFinalBottom > iFinalTop)
	{
		g_pGfx->DrawPartTexture(x,y,pTex,0,iFinalTop,-1,iFinalBottom);
	}
}



void RotItem::Draw(int x, int y)
{

}



// RotPic
void RotPic::DrawOne(int x, int y, int iPosY, int index)
{
	if (index < 0 || index >= m_itemCount)
		return;

	LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22410 + index,EP_UI);	
	if (pTex)
	{
		int iTempPosY = iPosY;
		DrawClipTex(x,y + iTempPosY,pTex,-iTempPosY,m_iMaxH - iTempPosY);		
	}
}


void RotPic::Draw(int x, int y)
{
	if (m_iRotState == 2 || m_iRotState == 3)
	{
		if (m_iRotNum < 0 || m_iRotNum >= m_itemCount)
		{
			m_iRotState = 0;
			return;
		}

		int iUpIndex = m_iRotNum+1;
		if (iUpIndex >= m_itemCount)
			iUpIndex = 0;	
		DrawOne(x, y, (int)m_fPos - m_iDis, iUpIndex);

		DrawOne(x, y, (int)m_fPos, m_iRotNum);

		int iDownIndex = m_iRotNum-1;
		if (iDownIndex < 0)
			iDownIndex = m_itemCount-1;
		DrawOne(x, y, (int)m_fPos + m_iDis, iDownIndex);

		if (m_iRotState == 3)
		{			
			if (m_iStopDelay > 0)
			{
				--m_iStopDelay;

				m_fRotSpeed -= 0.1f;
				if (m_fRotSpeed < 8.0f)
				{
					m_fRotSpeed = 8.0f;
				}
			}
			else
			{
				m_iRotState = 0;
				if (m_bStopSend)
					g_pGameControl->SEND_Request_Give_Adventrue_Reward();
			}
		}


		m_fPos += m_fRotSpeed;
		int iPos = (int)m_fPos;

		if (iPos >= m_fStopPos)
		{
			m_fPos -= m_iDis;
			m_iRotNum++;
			if (m_iRotNum >= m_itemCount)
				m_iRotNum = 0;
		}
	}	
	else if (m_iRotState == 0)
	{
		if (m_iRotTarget >= 0 && m_iRotTarget < m_itemCount)
		{
			DrawOne(x, y, (int)m_fStopPos, m_iRotTarget);
		}
	}	
}


// RotGood
void RotGood::DrawGoodEffect(const char *strName,int iX,int iY,int dwClientLooks, int iClipTop, int iClipBottom)
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
	bEffect = g_ItemCfgMgr.ParseItemEffect(strName,strTempLooks,sItemEffect);

	if(bEffect)
	{
		WORD wID = sItemEffect.iTextureID;

		LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_stateitem,wID,EP_UI);
		if(pTex)
		{
			int iFinalTop = 0;
			int iFinalBottom = 0;

			GetFinalClip(iFinalTop, iFinalBottom, iClipTop, iClipBottom, pTex->GetOffY() + sItemEffect.iOffsetY, pTex->GetHeight());

			if (iFinalBottom > iFinalTop)
			{
				g_pGfx->SetRenderMode(RenderMode(sItemEffect.iRenderMode));

				g_pGfx->DrawPartTexture(iX + sItemEffect.iOffsetX,iY + sItemEffect.iOffsetY,pTex,0,iFinalTop,-1,iFinalBottom);

				g_pGfx->SetRenderMode();
			}
		}
	}
}

void RotGood::DrawGood(int x, int y, int iPosY, int index)
{
	if (index < 0 || index >= m_vItems.size())
		return;

	const QiYuRotItem& item = m_vItems[index];
	LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_items,item.looks,EP_UI);
	if (pTex)
	{
		int iTempPosY = iPosY;
		DrawClipTex(x,y + iTempPosY,pTex,-iTempPosY,m_iMaxH - iTempPosY);

		iTempPosY = iPosY+16;
		DrawGoodEffect("item", x + 16,y + iTempPosY, item.looks, -iTempPosY,m_iMaxH - iTempPosY);

		iTempPosY = iPosY+11;
		int iTexH = 12;
		int iFinalTop = 0;
		int iFinalBottom = 0;
		GetFinalClip(iFinalTop, iFinalBottom, -iTempPosY + 2,m_iMaxH - iTempPosY - 2, 0, iTexH);
		if (iFinalBottom > iFinalTop)
		{
			if (iFinalTop > 0)
			{
				g_pFont->DrawTextPart(x + 37,y + iTempPosY,item.szName,0xFF00FF00,iFinalTop);
			}
			else if (iFinalBottom < iTexH)
			{
				g_pFont->DrawTextPart(x + 37,y + iTempPosY,item.szName,0xFF00FF00,iFinalBottom-iTexH);
			}
			else if (iFinalTop == 0 && iFinalBottom == iTexH)
			{
				g_pFont->DrawText(x + 37,y + iTempPosY,item.szName,0xFF00FF00);
			}
		}
	}
}

void RotGood::Draw(int x, int y)
{
	if (m_iRotState == 2 || m_iRotState == 3)
	{
		int itemCount = (int)m_vItems.size();

		if (m_iRotNum < 0 || m_iRotNum >= itemCount)
		{
			m_iRotState = 0;

			if (m_bStopSend)
				g_pGameControl->SEND_Request_Give_Adventrue_Reward();
			return;
		}

		const QiYuRotItem& item = m_vItems[m_iRotNum];

		int iUpIndex = m_iRotNum+1;
		if (iUpIndex >= itemCount)
			iUpIndex = 0;	
		DrawGood(x, y, (int)m_fPos - m_iDis, iUpIndex);

		DrawGood(x, y, (int)m_fPos, m_iRotNum);

		int iDownIndex = m_iRotNum-1;
		if (iDownIndex < 0)
			iDownIndex = itemCount-1;	
		DrawGood(x, y, (int)m_fPos + m_iDis, iDownIndex);

		if (m_iRotState == 3)
		{			
			m_fRotSpeed -= 0.04f;
			if (m_fRotSpeed < 2.0f)
			{
				m_fRotSpeed = 2.0f;
			}
		}
		
		m_fPos += m_fRotSpeed;
		int iPos = (int)m_fPos;

		if (iPos >= m_fStopPos)
		{
			if (m_iRotState == 3 && item.looks == m_iRotTarget)
			{
				m_iRotState = 0;

				if (m_bStopSend)
					g_pGameControl->SEND_Request_Give_Adventrue_Reward();
			}
			else
			{
				m_fPos -= m_iDis;
				m_iRotNum++;
				if (m_iRotNum >= itemCount)
					m_iRotNum = 0;	
			}
		}
	}	
	else if (m_iRotState == 0)
	{
		if (m_iRotNum >= 0 && m_iRotNum < m_vItems.size())
		{
			DrawGood(x, y, (int)m_fStopPos, m_iRotNum);
		}
	}
}




// CAdventureWnd
CAdventureWnd::CAdventureWnd(void)
{
	m_iType = 2;

	m_iIndex = 22401;

	m_iState = 0;
	m_iFrame = 0;
	m_iRewardType = 0;
	m_iRewardNum = 0;
	m_iRewardLooks = 0;

	m_pOK = NULL;
	m_pMKVDes1 = NULL;
	m_pMKVDes2 = NULL;

	g_OtherData.SetQiYuShow(false);

	g_pTexMgr->GetTex(PACKAGE_INTERFACE,22404,EP_UI);
	g_pTexMgr->PreLoad(PACKAGE_INTERFACE,22430,22454,EP_UI);
	g_pTexMgr->PreLoad(PACKAGE_INTERFACE,22460,22484,EP_UI);	
}


CAdventureWnd::~CAdventureWnd(void)
{
	g_OtherData.SetQiYuShow(true);
}

void CAdventureWnd::OnCreate()
{
	SetCloseButton(393,184, 80);

	m_pOK = new CCtrlButton;
	m_pOK->CreateEx(this,165,375,22405,22406,22407,22408);
	m_pOK->SetText("停 止",COLOR_BTN_NORMAL,COLOR_BTN_MOUSEON,COLOR_BTN_PRESS,COLOR_BTN_DISABLE,FONTSIZE_DEFAULT,DTF_BlackFrame);
	AddControl(m_pOK);
	m_pOK->SetShow(false);
	m_pOK->SetEnable(false);


	m_pMKVDes1 = new CMarkViewer(16,8,0,0,false);
	AddControl(m_pMKVDes1);
	m_pMKVDes1->Create(this,118,155,192,128);
	m_pMKVDes1->AddUpButton(70,-45,22490,22491,22492);
	m_pMKVDes1->AddDownButton(70,200,22494,22495,22496);
	m_pMKVDes1->AddScroll(518,22,13,150,0);
	m_pMKVDes1->SetScrollShow(false);
	m_pMKVDes1->SetFont(FONT_LISHU,FONTSIZE_BIG);
	m_pMKVDes1->SetShow(false);
	m_pMKVDes1->SetEnable(false);


	m_pMKVDes2 = new CMarkViewer(16,10,0,0,false);
	AddControl(m_pMKVDes2);
	m_pMKVDes2->Create(this,118,155,192,162);
	m_pMKVDes2->AddUpButton(70,-45,22490,22491,22492);
	m_pMKVDes2->AddDownButton(70,200,22494,22495,22496);
	m_pMKVDes2->AddScroll(518,22,13,150,0);
	m_pMKVDes2->SetScrollShow(false);
	m_pMKVDes2->SetFont(FONT_LISHU,FONTSIZE_BIG);
	m_pMKVDes2->SetShow(false);	
	m_pMKVDes2->SetEnable(false);

	SetMask(22400);

	m_iState = 0;
	m_iFrame = 0;

	Move(0, 50);	
}

bool CAdventureWnd::Msg(DWORD dwMsg, DWORD dwData, CControl *pControl)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_ADVENTURE_WND:
		{
			if (dwData == 10)
			{
				if (m_iType == 1)
				{
					m_iRewardType = 1;
					m_iRewardNum = (int)(ULONG_PTR)pControl;
					for (int i = 0; i < M_ROTNUMCOUNT; ++i)
					{
						m_picRot[i].SetRotSpeed(12.0f+0.3f*i);
						m_picRot[i].SetStopPos(2);
						m_picRot[i].SetDis(37+42);
						m_picRot[i].Start();
						m_picRot[i].m_fPos = -40.0f+11.0f*i;
					}

					m_pOK->SetShow(true);
					m_pOK->SetEnable(true);
				}
				
				return true;
			}
			else if (dwData == 12)
			{
				if (m_iType == 1)
				{
					m_iRewardType = 3;
					m_iRewardNum = (int)(ULONG_PTR)pControl;
					for (int i = 0; i < M_ROTNUMCOUNT - 1; ++i)
					{
						m_picRot[i].SetRotSpeed(12.0f+0.3f*i);
						m_picRot[i].SetStopPos(2);
						m_picRot[i].SetDis(37+42);
						m_picRot[i].Start();
						m_picRot[i].m_fPos = -40.0f+11.0f*i;
					}

					m_pOK->SetShow(true);
					m_pOK->SetEnable(true);
				}

				return true;
			}
			else if (dwData == 11)
			{
				if (m_iType == 1)
				{
					m_iRewardType = 2;
					m_iRewardLooks = (int)(ULONG_PTR)pControl;

					m_itemRot.m_vItems = g_OtherData.GetQiYuRotItems();
					m_itemRot.SetRotSpeed(4.0f);
					m_itemRot.SetStopPos(5);
					m_itemRot.SetDis(32+42);
					m_itemRot.Start();

					m_pOK->SetShow(true);
					m_pOK->SetEnable(true);
				}
				return true;
			}			
			else if (dwData == 21)//完成
			{
				m_iType = 1;
				SetPageTex(0,PACKAGE_INTERFACE,22400);

				if (m_pMKVDes2)
				{
					m_pMKVDes2->SetTagText(NULL);
					m_pMKVDes2->SetShow(false);
					m_pMKVDes2->SetEnable(false);
				}
				if (m_pMKVDes1)
				{
					m_pMKVDes1->SetTagText(&g_TagTextMgr.GetQiYuText());
					m_pMKVDes1->SetShow(true);
					m_pMKVDes1->SetEnable(true);
				}
				
				m_pOK->SetShow(true);
				m_pOK->SetEnable(true);

				return true;
			}
			else if (dwData == 22)//未完成
			{
				m_iType = 2;
				SetPageTex(0,PACKAGE_INTERFACE,22401);

				if (m_pMKVDes1)
				{
					m_pMKVDes1->SetTagText(NULL);
					m_pMKVDes1->SetShow(false);
					m_pMKVDes1->SetEnable(false);
				}

				if (m_pMKVDes2)
				{
					m_pMKVDes2->SetTagText(&g_TagTextMgr.GetQiYuText());
					m_pMKVDes2->SetShow(true);
					m_pMKVDes2->SetEnable(true);
				}				
				
				m_pOK->SetShow(false);
				m_pOK->SetEnable(false);

				return true;
			}
		}
		break;
	case MSG_CTRL_BUTTON_CLICK:
		{
			if (m_pOK == pControl)
			{
				if (m_iType == 1)
				{
					if (m_iRewardType == 1)
					{
						int iRewardNum = m_iRewardNum;
						for (int i = 0; i < M_ROTNUMCOUNT; ++i)
						{
							m_picRot[i].SetStopSend(i == 0);	// 最后一个停止的时候发送消息
							m_picRot[i].SetRotTarget((iRewardNum / (int)(pow(10.0f, M_ROTNUMCOUNT-i-1))) % 10);
							m_picRot[i].SetStopDelay(25*(M_ROTNUMCOUNT - i));
							m_picRot[i].Stop();
						}
					}
					else if (m_iRewardType == 3)
					{
						int iRewardNum = m_iRewardNum / 10000;
						int iNumCount = M_ROTNUMCOUNT - 1;
						for (int i = 0; i < iNumCount; ++i)
						{
							m_picRot[i].SetStopSend(i == 0);	// 最后一个停止的时候发送消息
							m_picRot[i].SetRotTarget((iRewardNum / (int)(pow(10.0f, iNumCount-i-1))) % 10);
							m_picRot[i].SetStopDelay(25*(iNumCount - i));
							m_picRot[i].Stop();
						}
					}
					else if (m_iRewardType == 2)
					{
						m_itemRot.SetStopSend(true);	// 停止的时候发送消息
						m_itemRot.SetRotTarget(m_iRewardLooks);
						m_itemRot.Stop();
					}
					m_pOK->SetEnable(false);
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


void CAdventureWnd::Draw()
{
	if (m_iState == 0)
	{
		// 界面出现效果

		const int ciTJPosX = m_iScreenX + 210;
		const int ciTJPosY = m_iScreenY + 237;
		const float cfTJScale = 4.7f;

		int iTJPosX = 0;
		int iTJPosY = 0;
		float fScale = 1.0f;
		const int frames = 30;
		if (m_iFrame < frames)
		{
			iTJPosX = 41 + (ciTJPosX-41)*m_iFrame/frames;
			iTJPosY = 287 + (ciTJPosY-287)*m_iFrame/frames;
			fScale = 1.0f + (cfTJScale-1.0f)*m_iFrame/frames;
		}
		else
		{
			m_iState = 1;
			m_iFrame = 0;
			iTJPosX = ciTJPosX;
			iTJPosY = ciTJPosY;
			fScale = cfTJScale;			
		}
		++m_iFrame;
		

		LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22404,EP_UI);
		if (pTex)
			g_pGfx->DrawTextureFX(iTJPosX,iTJPosY,pTex,-1,NULL,&POS(fScale,fScale),0.0f,0.0f,m_iFrame * 12 * 2.0f);
	}
	else
	{		
		CCtrlWindowX::Draw();
		//背景图片还没有下载下来,其它的东西不画
		if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
		{
			return;
		}

		if (m_iType == 1)
		{			
			if (m_iRewardType == 1)
			{
				// 刷钱
				for (int i = 0; i < M_ROTNUMCOUNT; ++i)
				{
					m_picRot[i].Draw(m_iScreenX+136+20*i, m_iScreenY+296);
				}				
			}
			else if (m_iRewardType == 3)
			{
				// 刷经验
				for (int i = 0; i < M_ROTNUMCOUNT - 1; ++i)
				{
					m_picRot[i].Draw(m_iScreenX+130+20*i, m_iScreenY+296);
				}

				LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22420,EP_UI);
				if (pTex)
					g_pGfx->DrawTextureNL(m_iScreenX+252, m_iScreenY+296, pTex);
			}
			else if (m_iRewardType == 2)
			{
				// 刷道具
				m_itemRot.Draw(m_iScreenX+140, m_iScreenY+296);
			}			
		}


		if (m_iFrame < 24 * 4)
		{
			LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22430 + m_iFrame/4,EP_UI);
			if (pTex)
				g_pGfx->DrawTextureNL(m_iScreenX - 88, m_iScreenY - 55, pTex);

			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22460 + m_iFrame/4,EP_UI);
			if (pTex)
			{
				g_pGfx->SetRenderMode(RM_ADD2);
				g_pGfx->DrawTextureNL(m_iScreenX - 88, m_iScreenY - 55, pTex);
				g_pGfx->SetRenderMode();
			}

			++m_iFrame;

			if (m_iFrame >= 24 * 4)
			{
				if (m_pMKVDes1)
				{
					m_pMKVDes1->SetFadeIn(0x03);
					m_pMKVDes1->SetBackTexByID(0x00FFFFFF,0);
				}
				if (m_pMKVDes2)
				{
					m_pMKVDes2->SetFadeIn(0x03);
					m_pMKVDes2->SetBackTexByID(0x00FFFFFF,0);
				}
			}
			else
			{
				if (m_pMKVDes1)
				{
					m_pMKVDes1->SetShow(false);
					m_pMKVDes1->SetEnable(false);
				}

				if (m_pMKVDes2)
				{
					m_pMKVDes2->SetShow(false);
					m_pMKVDes2->SetEnable(false);
				}
			}
		}
		else
		{
			if (m_pMKVDes1)
			{
				m_pMKVDes1->SetShow(m_iType == 1);
				m_pMKVDes1->SetEnable(m_iType == 1);
			}

			if (m_pMKVDes2)
			{
				m_pMKVDes2->SetShow(m_iType == 2);
				m_pMKVDes2->SetEnable(m_iType == 2);
			}
		}
	}
}

bool CAdventureWnd::OnLeftButtonUp(int iX, int iY)
{
	m_bClick = false;

	string strCommand;

	if (m_iType == 1 && m_pMKVDes1)
	{
		strCommand = m_pMKVDes1->GetCommand();
	}
	else if (m_iType == 2 && m_pMKVDes2)
	{
		strCommand = m_pMKVDes2->GetCommand();
	}

	if(!strCommand.empty())
	{
		//up 音乐
		g_pAudio->Play(EAT_OTHER,5,g_pAudio->GetRand()++);
		
		if(strCommand.substr(0,2) == "@@")
		{
			g_NPC.SetRepairWindow(false);
			g_NPC.SetLastCommand(strCommand);

			if(strCommand.substr(2,4)=="goto")
			{
				g_pGameControl->DealGotoNpcCommand(strCommand);
			}
			else if(strCommand.substr(2,7) == "goplace")
			{
				g_pGameControl->DealGoPlaceCommand(strCommand);
			}

			return true;
		}
		else if((int)strCommand.size() > 0)
		{
			g_pGameControl->SEND_Exchange_Menu_Click(g_NPC.GetID(),strCommand.c_str());
			return true;
		}
	}

	return CCtrlWindowX::OnLeftButtonUp(iX,iY);
}

bool CAdventureWnd::OnLeftButtonDown(int iX, int iY)
{
	m_bClick = true;
	return CCtrlWindowX::OnLeftButtonDown(iX,iY);
}