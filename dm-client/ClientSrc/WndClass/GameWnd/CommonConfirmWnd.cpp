#include "CommonConfirmWnd.h"
#include "GameControl/GameControl.h"
#include "GameData/GameData.h"
#include "GameData/GameGlobal.h"
#include "GameData/OtherData.h"

INIT_WND_POSX(CCommonConfirmWnd,POS_VARIABLE,POS_VARIABLE)


CCommonConfirmWnd::CCommonConfirmWnd(void)
{
	std::vector<S_CommonConfirm> & VCommonConfirm = g_OtherData.GetCommonConfirmVector();

	if(VCommonConfirm.size() > 0)
	{
		S_CommonConfirm & sCommonConfirm = VCommonConfirm.front();
		if(sCommonConfirm.dwLastTime != 0)//==0不自动关闭
			m_dwLifeTime = GetTickCount() + sCommonConfirm.dwLastTime; 
	}
	else
		m_dwLifeTime = GetTickCount() + 20; 

	m_iIndex = 30;
	m_iPages = 1;
	
	m_iAlignType = XAL_TOPLEFT;
	m_iOffX = 10;
	m_iOffY = 320;

}	

CCommonConfirmWnd::~CCommonConfirmWnd(void)
{
}

void CCommonConfirmWnd::OnCreate()
{
	m_pBtnOk = new CCtrlButton();
	AddControl(m_pBtnOk);
	m_pBtnOk->CreateEx(this,30,105,90,91,92);
	m_pBtnOk->SetText("同 意", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE, 0, FONT_YAHEI);

	m_pBtnCancel = new CCtrlButton();
	AddControl(m_pBtnCancel);
	m_pBtnCancel->CreateEx(this,130,105,90,91,92);
	m_pBtnCancel->SetText("拒 绝", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE, 0, FONT_YAHEI);

	std::vector<S_CommonConfirm> & VCommonConfirm = g_OtherData.GetCommonConfirmVector();
	if(VCommonConfirm.size() > 0)
	{
		S_CommonConfirm & sCommonConfirm = g_OtherData.GetCommonConfirmVector().front();
		m_TagText.Parse(sCommonConfirm.strMsg,0,"\\",26);
		if(sCommonConfirm.iType == 3)
		{
			m_TagText.ReplaceAll("＜","<");
			m_TagText.ReplaceAll("＞",">");
		}
	}

	m_pMarkViewer = new CMarkViewer(16,2);
	AddControl(m_pMarkViewer);
	m_pMarkViewer->Create(this,22,17,20,100);
	m_pMarkViewer->SetTagText(&m_TagText);
	m_pMarkViewer->SetFont(FONT_YAHEI, FONTSIZE_DEFAULT);
	m_pMarkViewer->SetDefaultColor(COLOR_TEXT_NORMAL);
}	

void CCommonConfirmWnd::Draw(void)
{	
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	/*std::vector<S_CommonConfirm> & VCommonConfirm = g_OtherData.GetCommonConfirmVector();
	if(VCommonConfirm.size() <= 0)
		return;

	S_CommonConfirm & sCommonConfirm = VCommonConfirm.front();

	std::vector<std::string>   vDrawStr;
	CutByUnicode((char*)sCommonConfirm.strMsg.c_str(),vDrawStr,26);

	for(size_t i = 0,j = 0; i < vDrawStr.size(); i++,j += 14)
	{
		g_pFont->DrawText(m_iScreenX+ 22,m_iScreenY + 17+j,vDrawStr[i].c_str(),-1,FONTSIZE_DEFAULT);
	}*/

	int iSeconds = (m_dwLifeTime - GetTickCount())/1000;
	char strTemp[12];
	sprintf(strTemp,"%d",iSeconds);
	g_pFont->DrawText(m_iScreenX+ 115,m_iScreenY + 53,strTemp,COLOR_TEXT_NORMAL, FONT_YAHEI);

}

void CCommonConfirmWnd::OnClickCloseButton()
{
	vector<S_CommonConfirm>::iterator it = g_OtherData.GetCommonConfirmVector().begin();
	if(it != g_OtherData.GetCommonConfirmVector().end())
		g_OtherData.GetCommonConfirmVector().erase(it);

	if(g_OtherData.GetCommonConfirmVector().size() > 0)//还有没有处理的请求再
	{
		S_CommonConfirm & sCommonConfirm = g_OtherData.GetCommonConfirmVector().front();
		if(sCommonConfirm.dwLastTime != 0)//==0不自动关闭
			m_dwLifeTime = GetTickCount() + sCommonConfirm.dwLastTime; 

		m_TagText.Parse(g_OtherData.GetCommonConfirmVector().front().strMsg,0,"\\",26);
		if(sCommonConfirm.iType == 3)
		{
			m_TagText.ReplaceAll("＜","<");
			m_TagText.ReplaceAll("＞",">");
		}

		return;
	}

	CloseWindow();
}	

bool CCommonConfirmWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch(dwMsg)
	{
	case MSG_CTRL_BUTTON_CLICK:
		{
			std::vector<S_CommonConfirm> & VCommonConfirm = g_OtherData.GetCommonConfirmVector();
			if(VCommonConfirm.size() <= 0)
			{
				SetLifeTime(GetTickCount() + 20);
				return true;
			}
			S_CommonConfirm & sCommonConfirm = VCommonConfirm.front();

			if(pControl == m_pBtnOk )
			{
				switch(sCommonConfirm.iType)
				{
				case 1://请求拜师
					g_pGameControl->SEND_Master_Prentice_Request(sCommonConfirm.dwRequestPersonID,3,0);
					break;

				case 2://请求收徒
					g_pGameControl->SEND_Master_Prentice_Request(sCommonConfirm.dwRequestPersonID,4,0);
					break;

				case 3://请求交易
					g_pGameControl->SEND_Player_Trade(1,sCommonConfirm.dwRequestPersonID,0);
					break;
				case 4://请求加为好友
					{
						CSimpleCharacterNode * p = g_pGameData->FindSimpleCharacter(sCommonConfirm.dwRequestPersonID);
						if(p)
							g_pGameControl->SEND_Answer_Friend_Request(p->GetName(),true);
					}
					break;
				case 5://请求加入行会
					g_pGameControl->SEND_Guild_Reply_Invited(true);
					break;
				case 6://接受官职
					g_pGameControl->SEND_Guild_Off_Appoint_Confirm(true);
					break;
				case 7://通用的响应其它玩家的请求
					g_pGameControl->SEND_General_Reply_Confirm(true,1);//1为宗派
					break;
				case 8://请求组队
					{
						CSimpleCharacterNode * p = g_pGameData->FindSimpleCharacter(sCommonConfirm.dwRequestPersonID);
						if(p)
							g_pGameControl->SEND_Answer_Group_Request(p->GetName(),true);
					}
					break;
				case 9://请求好友结义
					{
						CSimpleCharacterNode * p = g_pGameData->FindSimpleCharacter(sCommonConfirm.dwRequestPersonID);
						if(p)
							g_pGameControl->SEND_Answer_Swear_Request(p->GetName(),true);
					}
					break;
				default:
					break;

				}

			}
			else if(pControl == m_pBtnCancel )
			{
				switch(sCommonConfirm.iType)
				{
				case 1://请求拜师
					g_pGameControl->SEND_Master_Prentice_Request(sCommonConfirm.dwRequestPersonID,3,1);
					break;

				case 2://请求收徒
					g_pGameControl->SEND_Master_Prentice_Request(sCommonConfirm.dwRequestPersonID,4,1);
					break;

				case 3://请求交易
					g_pGameControl->SEND_Player_Trade(1,sCommonConfirm.dwRequestPersonID,1);
					break;
				case 4://请求加为好友
					{
						CSimpleCharacterNode * p = g_pGameData->FindSimpleCharacter(sCommonConfirm.dwRequestPersonID);
						if(p)
							g_pGameControl->SEND_Answer_Friend_Request(p->GetName(),false);
					}
					break;
				case 5://请求加入行会
					g_pGameControl->SEND_Guild_Reply_Invited(false);
					break;
				case 6://接受官职
					g_pGameControl->SEND_Guild_Off_Appoint_Confirm(false);
					break;
				case 7://通用的响应其它玩家的请求
					g_pGameControl->SEND_General_Reply_Confirm(false,1);//1为宗派
					break;
				case 8://请求组队
					{
						CSimpleCharacterNode * p = g_pGameData->FindSimpleCharacter(sCommonConfirm.dwRequestPersonID);
						if(p)
							g_pGameControl->SEND_Answer_Group_Request(p->GetName(),false);
					}
					break;
				case 9://请求好友结义
					{
						CSimpleCharacterNode * p = g_pGameData->FindSimpleCharacter(sCommonConfirm.dwRequestPersonID);
						if(p)
							g_pGameControl->SEND_Answer_Swear_Request(p->GetName(),false);
					}
					break;
				default:
					break;
				}
			}
			SetLifeTime(GetTickCount() + 20);
			return true;
		}
		break;

	default:
		break;
	}

	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}
