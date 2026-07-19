#include "BlessZhongZhouWnd.h"
#include "BaseClass/Control/GoodGrid.h"
#include "GameData/GameData.h"
#include "GameControl/GameControl.h"
#include "GameData/NpcData.h"
#include "GameData/TalkMgr.h"
#include "GameData/MsgBoxMgr.h"
#include "Baseclass/Control/ParserTip.h"
#include "GameAI/AIGoodMgr.h"


INIT_WND_POSX(CBlessZhongZhouWnd,POS_AUTO,POS_AUTO)

CBlessZhongZhouWnd::CBlessZhongZhouWnd(void)
{
	m_pOK = NULL;
	m_pCancel = NULL;
	m_pCharButton = NULL;
	m_pChangeButton = NULL;

	g_NPC.GetCharGood().SetID(0);
	g_NPC.GetCharChangeGood().SetID(0);

	m_iPages = 1;
	m_iIndex = 20603;
	m_iSelected = -1;
	for(int i=0; i<4; i++)
		m_pChars[i]=NULL;
	m_iType = sm_dwWindowType;
}

const char* CBlessZhongZhouWnd::m_pMtrlList[10] = {"金矿", "银矿", "铜矿", "铁矿", "黑铁矿石", "桃木", "樟木", "柚木", "松木", "桐木"};

CBlessZhongZhouWnd::~CBlessZhongZhouWnd(void)
{
}

void CBlessZhongZhouWnd::OnCreate()
{
	SetCloseButton(283,4, 80);

	m_pOK = new CCtrlButton();
	AddControl(m_pOK);
	m_pOK->CreateEx(this, 50, 195, 90, 91, 92, 93);
	m_pOK->SetText("确 定", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE,0,FONT_YAHEI);

	m_pCancel = new CCtrlButton();
	AddControl(m_pCancel);
	m_pCancel->CreateEx(this, 185, 195, 90, 91, 92, 93);
	m_pCancel->SetText("取 消", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE,0,FONT_YAHEI);

	m_pCharButton = new CCtrlButton();
	AddControl(m_pCharButton);
	m_pCharButton->CreateEx(this, 188, 62, 78, 78, 78, 78);

	m_pChangeButton = new CCtrlButton();
	AddControl(m_pChangeButton);
	m_pChangeButton->CreateEx(this, 92, 62, 78, 78, 78, 78);

	char* pChars[4]={"天", "佑", "中", "州"};

	for(WORD i=0; i<4; i++)
	{
		m_pChars[i] = new CCtrlButton();
		AddControl(m_pChars[i]);
		m_pChars[i]->CreateEx(this, i*60+40, 148, 95, 96, 97, 98);
		
		if (m_iType == 0)
		{
			m_pChars[i]->SetText(pChars[i], COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_DEFAULT,0,FONT_YAHEI);
		}
	}

	rcChar.left = m_pCharButton->GetX();
	rcChar.top = m_pCharButton->GetY();
	rcChar.bottom = rcChar.top+m_pCharButton->GetHeight();
	rcChar.right = rcChar.left+m_pCharButton->GetWidth();

	rcChange.left = m_pChangeButton->GetX();
	rcChange.top = m_pChangeButton->GetY();
	rcChange.bottom = rcChange.top+m_pChangeButton->GetHeight();
	rcChange.right = rcChange.left+m_pChangeButton->GetWidth();
}

int CBlessZhongZhouWnd::GetMaterialType(const char* pName)
{
	int i=0;
	for (i=0; i<10; i++)
	{
		if (strcmp(pName, m_pMtrlList[i])==0)
			return i+1;
	}
	return -1;
}

bool CBlessZhongZhouWnd::Msg(DWORD dwMsg, DWORD dwData, CControl *pControl)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_BUTTON_CLICK:
		{
			if(pControl == m_pOK)
			{
				if (m_iType == 0)//转换字符
				{
					if(g_NPC.GetCharGood().GetID()!=0 && g_NPC.GetCharChangeGood().GetID()!=0 && m_iSelected>-1 && m_iSelected<4)
					{
						if(g_NPC.GetCharGood().GetShape() != (m_iSelected+1))
						{
							g_pGameControl->Send_BlessZhongZhou_Change(g_NPC.GetCharGood().GetID(), g_NPC.GetCharChangeGood().GetID(), m_iSelected);
							OnClickCloseButton();
						}
						else
						{
							g_MsgBoxMgr.PopSimpleAlert("请选择与你放入的道具不一样的字符");
						}
					}
					else
					{
						if(g_NPC.GetCharGood().GetID()==0)
							g_MsgBoxMgr.PopSimpleAlert("请放入字符");
						else if(g_NPC.GetCharChangeGood().GetID()==0)
							g_MsgBoxMgr.PopSimpleAlert("请放入转换乾坤");
						else if(m_iSelected>3 || m_iSelected<0)
							g_MsgBoxMgr.PopSimpleAlert("请选择你希望转换成的字符");
					}
				}
				else//转换材料
				{
					if(g_NPC.GetCharGood().GetID()!=0 && g_NPC.GetCharChangeGood().GetID()!=0 && m_iSelected>-1 && m_iSelected<4)
					{
						int iMtrlType = GetMaterialType(g_NPC.GetCharGood().GetName());
						int iChangeTo = GetMaterialType(m_pChars[m_iSelected]->GetText());
						if(iMtrlType!= -1 && iMtrlType!=iChangeTo)
						{
							g_pGameControl->Send_Change_Material(g_NPC.GetCharGood().GetID(), g_NPC.GetCharChangeGood().GetID(),iMtrlType,  iChangeTo);
							OnClickCloseButton();
						}
						else
						{
							g_MsgBoxMgr.PopSimpleAlert("请选择与你放入的道具不一样的材料");
						}
					}
					else
					{
						if(g_NPC.GetCharGood().GetID()==0)
							g_MsgBoxMgr.PopSimpleAlert("请放入要转换的材料");
						else if(g_NPC.GetCharChangeGood().GetID()==0)
							g_MsgBoxMgr.PopSimpleAlert("请放入转换乾坤");
						else if(m_iSelected>3 || m_iSelected<0)
							g_MsgBoxMgr.PopSimpleAlert("请选择你希望转换成的材料类型");
					}
				}
				return true;
			}

			if(pControl == m_pCancel)
			{
				OnClickCloseButton();
				return true;
			}
			
			if(pControl == m_pCharButton)
			{
				if (m_iType == 0)//转换字符
				{
					stGoodFrom GoodFrom = CGoodGrid::GetDropGoodFrom();
					if(g_AIGoodMgr.IsTianYouZhongZhou(GoodFrom.DropGood) || (GoodFrom.DropGood.GetID()==0 && g_NPC.GetCharGood().GetID()!=0))
					{
						CGood tmp = GoodFrom.DropGood;
						GoodFrom.DropGood = g_NPC.GetCharGood();
						g_NPC.GetCharGood() = tmp;
						CGoodGrid::SetDropGoodFrom(GoodFrom);
					}
					else
					{
						if(GoodFrom.DropGood.GetID() != 0)
							g_TalkMgr.PushSysTalk("请放入“天”、“佑”、“中”、“州”四个字符之一！");
					}
				}
				else//转换材料
				{
					stGoodFrom GoodFrom = CGoodGrid::GetDropGoodFrom();
					int iMtrlType = GetMaterialType(GoodFrom.DropGood.GetName());
					if(iMtrlType!=-1 || (GoodFrom.DropGood.GetID()==0 && g_NPC.GetCharGood().GetID()!=0))
					{
						CGood tmp = GoodFrom.DropGood;
						GoodFrom.DropGood = g_NPC.GetCharGood();
						g_NPC.GetCharGood() = tmp;
						CGoodGrid::SetDropGoodFrom(GoodFrom);
						SetButtonsTextByMtrl(iMtrlType);
					}
					else
					{
						if(GoodFrom.DropGood.GetID() != 0)
							g_TalkMgr.PushSysTalk("请放入矿石或木材！");
					}
				}
				return true;
			}

			if(pControl == m_pChangeButton)
			{
				stGoodFrom GoodFrom = CGoodGrid::GetDropGoodFrom();
				if((GoodFrom.DropGood.GetStdMode()==79 && GoodFrom.DropGood.GetShape()==8)  || (GoodFrom.DropGood.GetID()==0 && g_NPC.GetCharChangeGood().GetID()!=0))
				{
					CGood tmp = GoodFrom.DropGood;
					GoodFrom.DropGood = g_NPC.GetCharChangeGood();
					g_NPC.GetCharChangeGood() = tmp;
					CGoodGrid::SetDropGoodFrom(GoodFrom);
				}
				else
				{
					if(GoodFrom.DropGood.GetID() != 0)
						g_TalkMgr.PushSysTalk("请放入转换乾坤！");
				}
				return true;
			}
			
			for(int i=0; i<4; i++)
			{
				if(m_pChars[i] == pControl)
				{
					if (m_iType !=0 && g_NPC.GetCharGood().GetID()==0)
					{
						return true;
					}

					if (m_iSelected != -1)
					{
						m_pChars[m_iSelected]->SetTex(BTEX_NORMAL, 95);
						m_pChars[m_iSelected]->SetTex(BTEX_HIGHLIGHT, 96);
						m_pChars[m_iSelected]->SetText(m_pChars[m_iSelected]->GetText(), COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_DEFAULT,0,FONT_YAHEI);
					}
					m_pChars[i]->SetTex(BTEX_NORMAL, 97);
					m_pChars[i]->SetTex(BTEX_HIGHLIGHT, 97);
					m_pChars[i]->SetText(m_pChars[i]->GetText(), COLOR_BTN_PRESS, COLOR_BTN_PRESS, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_DEFAULT,0,FONT_YAHEI);
					m_iSelected = i;
					return true;
				}
			}
		}
		break;
	default:
		break;
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CBlessZhongZhouWnd::SetButtonsTextByMtrl(int iMtrlType)
{
	int iPos = 0, iLower, iUpper;
	if(iMtrlType>5)
	{
		iLower = 6;
		iUpper = 10;
	}
	else if(iMtrlType>0)
	{
		iLower = 1;
		iUpper = 5;
	}
	else
	{
		for(int i=0; i<4; i++)
		{
			m_pChars[i]->SetText("", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_DEFAULT,0,FONT_YAHEI);
			m_pChars[i]->SetTex(BTEX_NORMAL, 95);
			m_pChars[i]->SetTex(BTEX_HIGHLIGHT, 96);
		}
		m_iSelected = -1;
		return;
	}

	for(int i=iLower; i<=iUpper; i++)
	{
		if(iMtrlType != i)
		{
			m_pChars[iPos]->SetTex(BTEX_NORMAL, 95);
			m_pChars[iPos]->SetTex(BTEX_HIGHLIGHT, 96);
			m_pChars[iPos++]->SetText(m_pMtrlList[i-1], COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_DEFAULT,0,FONT_YAHEI);
		}
		m_iSelected = -1;
	}
}

void CBlessZhongZhouWnd::Draw()
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	stGoodFrom GoodFrom = CGoodGrid::GetDropGoodFrom();
	if (GoodFrom.DropGood.GetID()!=0)
	{
		int x, y;
		g_pControl->GetMouseXY(x, y);
		POINT pt = {x-m_iScreenX, y-m_iScreenY};

		if(PtInRect(&rcChange, pt))
		{
			if (GoodFrom.DropGood.GetStdMode()!=79 || GoodFrom.DropGood.GetShape()!=8)
				g_pGfx->DrawFillRect(m_pChangeButton->GetScreenX()+4, m_pCharButton->GetScreenY()+4, 32, 32, 0x99FF0000);
			else
				g_pGfx->DrawFillRect(m_pChangeButton->GetScreenX()+4, m_pCharButton->GetScreenY()+4, 32, 32, 0x9900FF00);
		}
		else if (PtInRect(&rcChar, pt))
		{
			if(m_iType == 0)
			{
				if (!g_AIGoodMgr.IsTianYouZhongZhou(GoodFrom.DropGood))
					g_pGfx->DrawFillRect(m_pCharButton->GetScreenX()+4, m_pCharButton->GetScreenY()+4, 32, 32, 0x99FF0000);
				else
					g_pGfx->DrawFillRect(m_pCharButton->GetScreenX()+4, m_pCharButton->GetScreenY()+4, 32, 32, 0x9900FF00);
			}
			else
			{
				if (GetMaterialType(GoodFrom.DropGood.GetName()) == -1)
					g_pGfx->DrawFillRect(m_pCharButton->GetScreenX()+4, m_pCharButton->GetScreenY()+4, 32, 32, 0x99FF0000);
				else
					g_pGfx->DrawFillRect(m_pCharButton->GetScreenX()+4, m_pCharButton->GetScreenY()+4, 32, 32, 0x9900FF00);
			}
		}
	}

	if(g_NPC.GetCharGood().GetID()!=0)
		CGoodGrid::DrawOneGood(m_pCharButton->GetScreenX()+m_pCharButton->GetWidth()/2, m_pCharButton->GetScreenY()+m_pCharButton->GetHeight()/2, g_NPC.GetCharGood());

	if(g_NPC.GetCharChangeGood().GetID()!=0)
		CGoodGrid::DrawOneGood(m_pChangeButton->GetScreenX()+m_pCharButton->GetWidth()/2, m_pChangeButton->GetScreenY()+m_pCharButton->GetHeight()/2, g_NPC.GetCharChangeGood());

	int nWindowX=GetScreenX(), nWindowY=GetScreenY();
	g_pFont->DrawText(125+nWindowX, 11+nWindowY, "转换乾坤", COLOR_TEXT_NORMAL, FONT_YAHEI, FONTSIZE_BIG);
	g_pFont->DrawText(74+nWindowX, 108+nWindowY, "请放入“转换", 0xFFA79F79, FONT_YAHEI);
	g_pFont->DrawText(81+nWindowX, 123+nWindowY, "乾坤”道具", 0xFFA79F79, FONT_YAHEI);
	g_pFont->DrawText(174+nWindowX, 108+nWindowY, "请放入您需要", 0xFFA79F79, FONT_YAHEI);

	if (m_iType == 0)
	{
		g_pFont->DrawText(178+nWindowX, 123+nWindowY, "转换的字符", 0xFFA79F79, FONT_YAHEI);
		g_pFont->DrawText(86+nWindowX, 174+nWindowY, "请选择您希望转换成的字符", 0xFFA79F79, FONT_YAHEI);
	}
	else
	{
		g_pFont->DrawText(178+nWindowX, 123+nWindowY, "转换的材料", 0xFFA79F79, FONT_YAHEI);
		g_pFont->DrawText(86+nWindowX, 174+nWindowY, "请选择您希望转换成的材料", 0xFFA79F79, FONT_YAHEI);
	}
}

void CBlessZhongZhouWnd::OnClickCloseButton()
{
	if(g_NPC.GetCharGood().GetID()!=0)
	{
		SELF.PackageGood().BackToArray(g_NPC.GetCharGood());
		g_NPC.GetCharGood().SetID(0);
	}
	if(g_NPC.GetCharChangeGood().GetID()!=0)
	{
		SELF.PackageGood().BackToArray(g_NPC.GetCharChangeGood());
		g_NPC.GetCharChangeGood().SetID(0);
	}
	CCtrlWindowX::OnClickCloseButton();
}

bool CBlessZhongZhouWnd::OnMouseMove(int iX,int iY)
{
	TRY_BEGIN
		CParserTip* pTip = g_pControl->GetTipWnd();
	if(!pTip)
		return true;

	POINT pt = {iX, iY};

	if(PtInRect(&rcChar, pt) || PtInRect(&rcChange, pt))
	{
		pTip->Clear();
		pTip->SetShow(false);

		if(PtInRect(&rcChar, pt))
		{
			if(g_NPC.GetCharGood().GetID() == 0)
			{
				if(m_iType == 0)
					pTip->AddText("请放入您需要转换成另外一个字的字符。它只能是“天”、“佑”、“中”、“州”中的某一个。");
				else
					pTip->AddText("请放入您需要转换的材料。它只能是矿或木材。");
			}
			else
				pTip->Parse(g_NPC.GetCharGood());
		}
		else if(PtInRect(&rcChange, pt))
		{
			if(g_NPC.GetCharChangeGood().GetID() == 0)
				pTip->AddText("请放入“转换乾坤”道具。您可以从商城中买到它。或者参加活动，部分BOSS也会掉落。");
			else
				pTip->Parse(g_NPC.GetCharChangeGood());
		}

		int x = m_iScreenX + iX + 10;
		int y = m_iScreenY + iY + 10;
		pTip->AdjustXY(x,y);
		pTip->Move(x,y);
		pTip->SetShow(true);
		return true;
	}
	else
	{
		pTip->SetShow(false);
	}
	TRY_END

		return CCtrlWindowX::OnMouseMove(iX,iY);
}