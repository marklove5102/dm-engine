#include "CommonSelectWnd.h"
#include "GameData/GameDefine.h"
#include "GameData/GameGlobal.h"
#include "GameData/Good.h"
#include "GameData/GameData.h"
#include "BaseClass/Control/GoodGrid.h"
#include "GameControl/GameControl.h"
#include "BaseClass/Audio/Audio.h"
#include "BaseClass/Control/CtrlMsgBox.h"
#include "GameData/MsgBoxMgr.h"

INIT_WND_POSX(CCommonSelectWnd,POS_AUTO,POS_AUTO)

CCommonSelectWnd::CCommonSelectWnd(void)
{
	m_iIndex = 38;
	m_bModel = true;
	m_strWndInfo.clear();
	m_dwType = HIWORD(sm_dwWindowType);
	m_dwTemp = LOWORD(sm_dwWindowType);
}

CCommonSelectWnd::~CCommonSelectWnd(void)
{
}

void CCommonSelectWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	if (m_dwType == 1)
	{
		//g_pFont->DrawText(GetScreenX() + 30,GetScreenY() + 25,"你要将圣灵精华附加在哪类装备上？",0xFFFFFFFF);
		g_pFont->DrawText(GetScreenX() + 30,GetScreenY() + 25,"你是否要使用10个圣殿精华激活手中武器的隐藏属性？",0xFFFFFFFF);
	}
	else if (m_dwType == 2)
	{
		//g_pFont->DrawText(GetScreenX() + 30,GetScreenY() + 25,"你要将圣灵宝石升级在哪类装备上？",0xFFFFFFFF);
		g_pFont->DrawText(GetScreenX() + 30,GetScreenY() + 25,"你确定将圣殿宝石用于升级武器吗？",0xFFFFFFFF);
	}
}

void CCommonSelectWnd::OnCreate(void)
{
	//SetCloseButton(398,15,80);
	if (m_dwType == 1 || m_dwType == 2)
	{
		m_pArmUpdate = new CCtrlButton();
		AddControl(m_pArmUpdate);
		m_pArmUpdate->CreateEx(this,115,139,90,91,92,93);
		//m_pArmUpdate->SetText("武 器", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);
		m_pArmUpdate->SetText("确 定", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);

		m_pSheildUpdate = new CCtrlButton();
		AddControl(m_pSheildUpdate);
		m_pSheildUpdate->CreateEx(this,232,139,90,91,92,93);
		//m_pSheildUpdate->SetText("盾 牌", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);
		m_pSheildUpdate->SetText("取 消", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);

		//unsigned long ulID = SELF.GetEquipGood(ITEM_POS_WEAPON).GetID();//武器
		//unsigned long ulIDDeng = SELF.GetEquipGood(ITEM_POS_SHIELD).GetID();//盾

		//m_pArmUpdate->SetEnable((ulID != 0));
		//m_pSheildUpdate->SetEnable((ulIDDeng != 0));
	}
}

bool CCommonSelectWnd::Msg(DWORD dwMsg, DWORD dwData, CControl * pControl)
{
	switch(dwMsg)
	{
	case MSG_CTRL_BUTTON_CLICK:
		{
			if (pControl == m_pSheildUpdate)
			{
				CloseWindow();
				return true;
			}

			if (m_dwType == 1 || m_dwType == 2)
			{
				CGood *pGood = NULL;
				WORD wType;
				if (pControl == m_pArmUpdate)
				{
					wType = 1;
					pGood = &(SELF.GetEquipGood(ITEM_POS_WEAPON));//武器
				}
				//else if (pControl == m_pSheildUpdate)
				//{
				//	wType = 2;
				//	pGood = &(SELF.GetEquipGood(ITEM_POS_SHIELD));//盾
				//}

				if (pGood)
				{
					if (m_dwType == 1)
					{
						int iLevel = 0;
						if (wType == 1)
							iLevel = SELF.GetEquipGood(ITEM_POS_WEAPON).GetAttachSkillLevel();
						else if (wType == 2)
							iLevel = SELF.GetEquipGood(ITEM_POS_SHIELD).GetAttachSkillLevel();
						if(iLevel <= 1)
						{
							g_pControl->PopupWindow(MSG_CTRL_CHARWND,OPER_CREATE);
							if(wType == 1)
							{
								g_pControl->MsgToWnd(MSG_CTRL_CHARWND,MSG_CTRL_EQUIPMENT_EFFECT,1);
							}
							else if(wType == 2)
							{
								g_pControl->MsgToWnd(MSG_CTRL_CHARWND,MSG_CTRL_EQUIPMENT_EFFECT,3);
							}
							CGoodGrid::GetDropGoodFrom().DropGood.SetPos(0);											    
							g_pGameControl->SEND_AddObjToObj(SELF.GetPackageGood(m_dwTemp),*pGood);
							g_pAudio->Play(EAT_OTHER,200,g_pAudio->GetRand()++);
						}
						else
						{
							//char ctemp[128]="您手中的武器或盾牌已经附加了较高威力的技能，使用圣灵精华更换附加技能类型，会使这些威力一起消失，您确认您要进行更换操作么？";
							char ctemp[128]="您手中的武器已经附加了较高威力的技能，使用圣殿精华更换附加技能类型，会使这些威力一起消失，您确认您要进行更换操作么？";
							DWORD dwData = MAKELONG(m_dwTemp,wType);
							g_MsgBoxMgr.PopSimpleComfirm(ctemp,MSG_CTRL_UPLEVEL_ARM,dwData);
						}
					}
					else if (m_dwType == 2)
					{
						g_pControl->MsgToWnd("PackageWnd",MSG_CTRL_SPIRT_UPGRATE_WEAPON,MAKELONG(m_dwTemp,wType),NULL);
					}
				}
				else
				{
					//char ctemp[128]="你可升级的武器或盾牌不在你手上吧？";
					g_MsgBoxMgr.PopSimpleAlert("你可升级的武器不在你手上吧？");
				}

			}
			CloseWindow();
			return true;
		}
		break;
	default:
		break;
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}
