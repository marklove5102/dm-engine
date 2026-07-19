#include "GameWnd.h"
#include "GameData/GameData.h"
#include "GameMap/GameMap.h"
#include "Global/Interface/AudioInterface.h"
#include "BaseClass/Control/GoodGrid.h"
#include "BaseClass/Control/ParserTip.h"
#include "GameControl/GameControl.h"
#include "GameAI/AIAutoMgr.h"
#include "GameAI/AutoKillMonsterMgr.h"
#include "GameData/MsgBoxMgr.h"
#include "GameData/TradeData.h"
#include "GameData/WooolStoreData.h"
#include "GameData/TalkMgr.h"
#include "BaseClass/Control/CtrlMenuWnd.h"
#include "GameClient/WidgetManager.h"
#include "GameData/OtherData.h"
#include "GameData/NpcData.h"
#include "GameData/LoginData.h"
#include "GameData/TaskData.h"
#include "GameData/ItemCfgMgr.h"
#include "BaseClass/Control/CtrlTreeView.h"
#include "GameAI/AIPickCfgMgr.h"
#include "GameAI/AIConfigMgr.h"
#include "GameClient/ReplayManager.h"

#ifdef _DEBUG
#include "global/new.h"
#define new DEBUG_NEW
#endif

DTI_IMPLEMENT(CGameWnd, CCtrlWindow)
CGameWnd::CGameWnd(void)
{
	m_iControlMode		= CTRL_MODE_GAMEWND;
	m_bNoMove			= TRUE;					// 表示此窗口不能被移动
	m_bNoChangeLevel	= TRUE;					// 表示此窗口始终在底部，不会改变Level
	//m_bCreated          = false;
	m_bDisableEscape    = true;

	m_bDrawTalkServer = false;
	m_byLocalAreaType = 0;
	m_dwShowPiaoHongTime = 0;
	m_bStartDefend = false;
	m_dwDefenceTime = 0;
	m_bStartAI = false;
	m_sName = "GameWnd";


	g_pGfx->SetRenderIGW(true);

	//m_iRunHorsePosX = 0;//(g_pGfx->GetWidth() - pTex->GetWidth()) / 2;
	//m_iRunHorsePosY = 0;//80;

	//m_iOldRHPX = 0;
	//m_iOldRHPY = 0;
	//m_bOnMoveRHP = false;
	m_pRunHorseLamp = NULL;
	m_pBtnBing = NULL;
}

CGameWnd::~CGameWnd(void)
{
	g_pGfx->SetRenderIGW(false);
}


bool CGameWnd::Create(CControl * pParent,int iX,int iY,int iW,int iH)
{
	return CControl::Create(pParent,iX,iY,iW,iH,CTRL_STYLE_BACKMODE_NODRAW);
}

void CGameWnd::OnCreate()
{
	//更改攻击方式
	m_pAttackType = new CCtrlButton();
	m_pAttackType->SetAlignType(XAL_TOPRIGHT);
	AddControl(m_pAttackType);
	m_pAttackType->CreateEx(this,-158,0,13900,13901,13902);
	m_pAttackType->SetShow(false);
	m_pAttackType->SetEnable(false);

	//系统公告
	m_pServerMsg = new CCtrlButton();
	m_pServerMsg->SetAlignType(XAL_TOPRIGHT);
	AddControl(m_pServerMsg);
	m_pServerMsg->CreateEx(this,-214,0,13936,13937,13938);
	m_pServerMsg->SetTips("查看系统信息");

	//左边的聊天窗口
	int iSystemTalky = g_pGfx->GetHeight() - 310;

	//m_pSystemTalkView = new CTalkViewer();
	//AddControl(m_pSystemTalkView);
	//m_pSystemTalkView->SetLineHeight(13);
	//LPTexture pTex = g_pTexMgr->LoadTex(PACKAGE_INTERFACE,13941);
	//m_pSystemTalkView->Create(this,6,iSystemTalky,170,98,CTRL_STYLE_BACKMODE_TEX|CTRL_STYLE_SELFDELETE_BACKTEX,-1,pTex);
	//m_pSystemTalkView->SetDrawOffXY(19,3);
	//m_pSystemTalkView->AddScroll(1,18,16,59,54);
	//m_pSystemTalkView->AddUpButton(1,1,51,52,53);
	//m_pSystemTalkView->AddDownButton(1,78,56,57,58);
	//m_pSystemTalkView->SetTalkType(&g_TalkMgr.GetSystemTalk());
	//m_pSystemTalkView->SetEnable(false);
	//m_pSystemTalkView->SetShow(false);

	////锁定玩家相关消息
	//m_pPlayerMsg_Lock = new CCtrlButton();
	//AddControl(m_pPlayerMsg_Lock);
	//m_pPlayerMsg_Lock->CreateEx(this,5,iSystemTalky + 96,13946,13947,13948);
	//m_pPlayerMsg_Lock->SetTips("锁定窗口");

	////隐藏玩家相关消息，把他放到聊天栏显示
	//m_pPlayerMsg_Hide = new CCtrlButton();
	//AddControl(m_pPlayerMsg_Hide);
	//m_pPlayerMsg_Hide->CreateEx(this,23,iSystemTalky + 96,14344,14345,14346);
	//m_pPlayerMsg_Hide->SetTips("显示窗口");


	//上面中间的系统公告
	m_pServerTalkView = new CTalkViewer();
	m_pServerTalkView->SetAlignType(XAL_TOPRIGHT);
	AddControl(m_pServerTalkView);
	m_pServerTalkView->SetLineHeight(15);

	m_pServerTalkView->Create(this, -238,0,478,148,CTRL_STYLE_BACKMODE_TEX|CTRL_STYLE_SELFDELETE_BACKTEX,-1,MAKELONG(13950,PACKAGE_INTERFACE));
	//m_pServerTalkView->SetDrawOffXY(19,3);
	//m_pServerTalkView->AddScroll(478-18,20,16,109,54);
	//m_pServerTalkView->AddUpButton(478-18,3,51,52,53);
	//m_pServerTalkView->AddDownButton(478-18,148-18,56,57,58);
	m_pServerTalkView->AddScrollEx(0,0,18,148,true,-1,XAL_TOPRIGHT);
	m_pServerTalkView->SetTalkType(&g_TalkMgr.GetServerTalk());
	m_pServerTalkView->SetEnable(false);
	m_pServerTalkView->SetShow(false);

	m_pPromptInfo = new CCtrlButton();
	AddControl(m_pPromptInfo);
	if (g_pGfx->GetWidth() <= 800)
		m_pPromptInfo->CreateEx(this,g_pGfx->GetWidth()/2 - 23,g_pGfx->GetHeight() - 180,16501,16501,16501);
	else 
		m_pPromptInfo->CreateEx(this,g_pGfx->GetWidth()/2 - 20,g_pGfx->GetHeight() - 150,16501,16501,16501);

	m_pPromptInfo->SetShow(false);
	m_pPromptInfo->SetEnable(false);

	m_pInstantHint = new CCtrlButton();
	AddControl(m_pInstantHint);
	m_pInstantHint->SetAlignType(XAL_BOTTOMLEFT);
    m_pInstantHint->CreateEx(this,10,-260,17613,17614,17612);//有新手提示信息
	m_pInstantHint->SetTips("显示游戏温馨提示");


	m_pBtnAutoKill = new CCtrlButton();
	AddControl(m_pBtnAutoKill);
	m_pBtnAutoKill->SetAlignType(XAL_BOTTOM);
	m_pBtnAutoKill->CreateEx(this,-130,-90,165,166,167,168);
	if (g_pGameData->IsAutoKillMonster())
		m_pBtnAutoKill->SetText("取消自动打怪", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);
	else
		m_pBtnAutoKill->SetText("自动打怪", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);

	m_pBtnAutoKill->SetShow(g_AICfgMgr.IsAutoKillShow());


	if (g_Login.GetLoginInExpType() == 1)
	{
		m_pBtnBindPT = new CCtrlButton();
		AddControl(m_pBtnBindPT);
		m_pBtnBindPT->SetAlignType(XAL_BOTTOM);
		m_pBtnBindPT->CreateEx(this,0,-130,23302,23303,23304,23304);
		m_pBtnBindPT->SetText("保存角色", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, 18,0,FONT_YAHEI);
	}
	else
	{
		m_pBtnBindPT = NULL;
	}


	if (g_hParentWnd)
	{
		m_pWidowBtn = new CCtrlButton();
		AddControl(m_pWidowBtn);
		m_pWidowBtn->SetAlignType(XAL_BOTTOM);
		m_pWidowBtn->CreateEx(this,128,-90,165,166,167,168);
		m_pWidowBtn->SetText("窗口模式", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);

		DisplayMode eDisMode = g_pGfx->GetDisMode();
		m_pWidowBtn->SetShow((eDisMode == DM_FULL_FALSE || eDisMode == DM_FULL_TRUE));
		m_pWidowBtn->SetEnable((eDisMode == DM_FULL_FALSE || eDisMode == DM_FULL_TRUE));
	}
	else
	{
		m_pWidowBtn = NULL;
	}


	m_pBtnQiYu = new CCtrlButton();
	m_pBtnQiYu->CreateEx(this,0,246,22403,22403,22403,22403);
	m_pBtnQiYu->SetFlashTexID(MAKELONG(22425,PACKAGE_INTERFACE));
	m_pBtnQiYu->SetMask(22403);
	AddControl(m_pBtnQiYu);
	m_pBtnQiYu->SetShow(false);
	m_pBtnQiYu->SetEnable(false);


	m_pBtnNoticeIE = new CCtrlButton();
	m_pBtnNoticeIE->CreateEx(this,g_pGfx->GetWidth()/2 - 25,g_pGfx->GetHeight() - 190,22603,22604,22605);
	AddControl(m_pBtnNoticeIE);
	m_pBtnNoticeIE->SetShow(false);
	m_pBtnNoticeIE->SetEnable(false);


	//12宫兵种控制
	m_pBtnBing = new CCtrlButton();
	m_pBtnBing->SetAlignType(XAL_TOPRIGHT);
	AddControl(m_pBtnBing);
	m_pBtnBing->CreateEx(this,-250,0,23004,23005,23006);
	m_pBtnBing->SetShow(true);
	m_pBtnBing->SetEnable(true);



	//int iX = g_pGfx->GetWidth() - 195;
	int iY = 260;
	m_pTaskTrackViewer = new CTaskTracker;
	m_pTaskTrackViewer->SetAlignType(XAL_TOPRIGHT);
	m_pTaskTrackViewer->Create(this,-3,iY,200,0);
	AddControl(m_pTaskTrackViewer);
	m_pTaskTrackViewer->SetShow(false);

	//小地图
	g_pControl->PopupWindow(MSG_CTRL_HANDYMAP_WND,OPER_RECREATE);
	g_pControl->PopupWindow(MSG_CTRL_SHORTCUTKEY_WND,OPER_RECREATE);
	g_Timer.SetTimer(MSG_CTRL_GAMEWND,1,1 * 30 * 1000,0xFFFF);

	ResetControlPos();
}

//bool CGameWnd::ReSize(int iW,int iH)
//{
//	bool bRtn = CCtrlWindow::ReSize(iW,iH);
//
//	if(m_bCreated)
//	{
//		//更改攻击方式
//		m_pAttackType->Move(iW - 198,0);
//		//系统公告
//		m_pServerMsg->Move(iW - 219,0);
//		//左边的聊天窗口
//		int iSystemTalky = iH - 310;
//		//m_pSystemTalkView->Move(6,iSystemTalky);
//		////锁定玩家相关消息
//		//m_pPlayerMsg_Lock->Move(5,iSystemTalky + 96);
//		////隐藏玩家相关消息，把他放到聊天栏显示
//		//m_pPlayerMsg_Hide->Move(23,iSystemTalky + 96);
//		g_AIAutoMgr.SetDrawInfoXY(40,iSystemTalky + 99);
//		//上面中间的系统公告
//		m_pServerTalkView->Move(iW - 696,0);
//	}
//
//	return bRtn;
//}

void CGameWnd::OnTimer()
{
	VTimerActivityInfo& vTmActInfo = g_PromptInfoMgr.GetTimerActivityInfo();
	VTimerActivityInfo::iterator it = vTmActInfo.begin();

	struct tm *newtime = NULL;
	__time32_t t_time;
	_time32(&t_time);
	t_time += g_dwServerTime;
	newtime = _localtime32(&t_time);
	if(!newtime)
		return;

	float fTm = newtime->tm_hour + (float)newtime->tm_min/(float)100;
	
	while (it != vTmActInfo.end())
	{
		if (fTm>= it->fStartTm && fTm <= it->fEndTm)
		{
			if (!it->bRuned)
			{
				it->bRuned = true;
				//弹出相应的消息
				HorseRunToDead hr;
				hr.dwColor = COLOR_TEXT_SBU_TITLE;
				hr.eFontType = FONT_YAHEI;
				hr.iFontSize = 12;
				hr.wLastTimes = 1;
				hr.strText = it->strContent;

				g_pGameData->AddHorseRunToDead(hr);
				this->Msg(MSG_CTRL_RUNHORSELAMP_WND,OPER_CREATE);
				
				return;
			}
		}
		else
		{
			it->bRuned = false;
		}

		it++;
	}
}

bool CGameWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch(dwMsg)
	{
	case MSG_WND_TIMER:
		{
			OnTimer();
		}
		break;
	case MSG_CTRL_AI_START:
		{
			m_bStartAI =  (dwData != 0);
			break;
		}
	case MSG_CTRL_UPDATE_ATTACKMODE://刷新攻击模式按钮的图片
		{
			char* pAttackModeList[7] = {"善恶攻击", "全体攻击", "和平攻击", "编组攻击", "行会攻击", "师徒攻击", "夫妻攻击"};
			int iType = SELF.GetReserveData(plyAttackType);
			//m_pAttackType->ReloadTex(13900 + iType * 4,13901 + iType * 4,13902 + iType * 4,13903 + iType * 4);
			m_pAttackType->SetText(pAttackModeList[iType], 0xffdc783c, 0xffe6aa5a, 0xff965032, COLOR_BTN_DISABLE, 12);
			m_pAttackType->SetTextOff(-2, 0);
			m_pAttackType->SetShow(true);
			m_pAttackType->SetEnable(true);
			return true;
		}
		break;
	//case MSG_CTRL_UPDATE_HIDESELFMSG_BTN://刷新隐藏系统消息的按钮状态
	//	{
	//		if(g_TalkMgr.IsShowSystemLeft())
	//		{
	//			m_pPlayerMsg_Hide->ReloadTex(14347,14348,14349);
	//			m_pSystemTalkView->SetEnable(true);
	//			m_pSystemTalkView->SetShow(true);
	//			m_pPlayerMsg_Hide->SetTips("隐藏窗口");
	//		}
	//		else
	//		{
	//			m_pPlayerMsg_Hide->ReloadTex(14344,14345,14346);
	//			m_pSystemTalkView->SetEnable(false);
	//			m_pSystemTalkView->SetShow(false);
	//			m_pPlayerMsg_Hide->SetTips("显示窗口");
	//		}
	//		return true;
	//	}
	//	break;
	case MSG_CTRL_UPDATE_INSTANCE_INFO:
		{
			if (g_pGameData->HasCareForInfo())				
				 m_pInstantHint->ReloadTex(17613,17614,17612);//有新手提示信息
			else m_pInstantHint->ReloadTex(17611,17614,17612);//没有新手提示信息				
		}
		break;
	case MSG_CTRL_UPDATE_TRACK_INFO:
		{
			//更新追踪信息
			WORD wID = LOWORD(dwData);
			WORD wType = HIWORD(dwData);
			if (wType == 0) //增加
			{
				m_pTaskTrackViewer->AddTaskTracker(wID);
				m_pTaskTrackViewer->SetShow(true);

				//if (!g_pGameData->HasPaoPaoStatus(EP_First_AutoFindPath))
				//{
				//	m_pTaskTrackViewer->Msg(MSG_CTRL_POPUP_ARROWTIP,EP_First_AutoFindPath,NULL);
				//}

			}
			else if (wType == 1)//删除
			{
				m_pTaskTrackViewer->DelTaskTracker(wID);

				int iNum = m_pTaskTrackViewer->GetTaskTrackedNum();
				if (iNum == 0) m_pTaskTrackViewer->SetShow(false);
				else m_pTaskTrackViewer->SetShow(true);
			}
			else if (wType == 2)
			{
				g_TaskData.SetTracked(LOWORD(dwData),false);
				g_pControl->MsgToWnd("TaskWnd",MSG_CTRL_TASK_WND,LOWORD(dwData));
				int iNum = m_pTaskTrackViewer->GetTaskTrackedNum();
				if (iNum == 0) m_pTaskTrackViewer->SetShow(false);
			}
			else if (wType == 4)
			{
				m_pTaskTrackViewer->UpdateTaskTracker(wID);
			}
			else if (wType == 5)
			{
				if(m_pTaskTrackViewer->GetTaskTrackedNum() >= MAX_TASKTRACK_COUNT)
				{
					CTreeViewNode* pTreeNode = (CTreeViewNode*)pControl;
					if (!pTreeNode->IsChecked())
					{
						g_MsgBoxMgr.PopSimpleAlert("当前无法追踪更多的任务！");
						return false;
					}
				}

				return true;
			}
			else if (wType == 3)
			{
				TaskTracker* pTracker = (TaskTracker*)pControl;
				if (pTracker)
				{
					const string& strCommand = pTracker->TrackView->GetCommand();
					g_pGameControl->DealGotoCommand(strCommand);

					if (g_pGameData->HasPaoPaoStatus(EP_First_AutoFindPath) && !g_pGameData->HasPaoPaoStatus_Closed(EP_First_AutoFindPath))
					{
						m_pTaskTrackViewer->Msg(MSG_REMOVE_ARROWTIP_CONTROL,EP_First_AutoFindPath,NULL);
					}

					return true;
				}
			}
			else if (wType == 6)
			{
				int iNum = m_pTaskTrackViewer->GetTaskTrackedNum();
				if (iNum < MAX_TASKTRACK_COUNT)
				{
					return true;
				}

				return false;
			}
			else if (wType == 7)
			{
				if (m_pTaskTrackViewer)
				{
					bool bShow = (!m_pTaskTrackViewer->IsShow() && m_pTaskTrackViewer->GetTaskTrackedNum() > 0);
					m_pTaskTrackViewer->SetShow(bShow);
				}
				return true;
			}
			else if(wType == 9)//  [4/27/2010 dujun]
			{
				m_pTaskTrackViewer->ClearTaskTracker();
				m_pTaskTrackViewer->SetShow(false);

				return true;
			}
			else if (wType == 8)
			{
				m_pTaskTrackViewer->ClearTaskTracker(); //  [4/29/2010 dujun]

				int iTastCount = 0;
				MTaskList& mMainTask = g_TaskData.GetMainTaskList();
				MTaskList::iterator it = mMainTask.begin();

				while(it != mMainTask.end())
				{
					if (iTastCount >= MAX_TASKTRACK_COUNT)
						break;

					VTask& vTask = it->second.vTask;
					VTask::iterator itTemp = vTask.begin();
					while (itTemp != vTask.end())
					{
						m_pTaskTrackViewer->AddTaskTracker(itTemp->wTaskID);
						m_pTaskTrackViewer->SetShow(true);
						itTemp->bTrack = true;
						
						iTastCount++;
						itTemp++;
					}

					it++;
				}

				if (iTastCount < MAX_TASKTRACK_COUNT)
				{
					MTaskList& mBranchTask = g_TaskData.GetBranchTaskList();
					MTaskList::iterator it = mBranchTask.begin();

					while (it != mBranchTask.end())
					{
						if (iTastCount >= MAX_TASKTRACK_COUNT)
							break;

						VTask& vTask = it->second.vTask;
						VTask::iterator itTemp = vTask.begin();
						while (itTemp != vTask.end())
						{
							m_pTaskTrackViewer->AddTaskTracker(itTemp->wTaskID);
							m_pTaskTrackViewer->SetShow(true);
							itTemp->bTrack = true;

							iTastCount++;
							itTemp++;
						}

						it++;
					}
				}
			}

			if (wType != 3) m_pTaskTrackViewer->ReSize(200,m_pTaskTrackViewer->GetHeightEx() + 2);
		}
		break;
	case MSG_CTRL_BUTTON_CLICK:
		if(pControl == m_pAttackType)
		{
			OnChangeAttackType();
			return true;
		}
		//else if(pControl == m_pPlayerMsg_Hide)
		//{
		//	OnHidePlayerMsg();
		//	return true;
		//}
		//else if(pControl == m_pPlayerMsg_Lock)
		//{
		//	bool bPlayerMsgAutoWheel = !m_pSystemTalkView->IsAutoWheel();
		//	m_pSystemTalkView->SetAutoWheel(bPlayerMsgAutoWheel);

		//	if(bPlayerMsgAutoWheel)
		//	{
		//		m_pPlayerMsg_Lock->ReloadTex(13946,13947,13948);
		//		m_pPlayerMsg_Lock->SetTips("锁定窗口");
		//	}
		//	else
		//	{
		//		m_pPlayerMsg_Lock->ReloadTex(13942,13943,13944);
		//		m_pPlayerMsg_Lock->SetTips("解除锁定");
		//	}

		//	return true;
		//}
		else if(pControl == m_pServerMsg)
		{
			m_bDrawTalkServer = !m_bDrawTalkServer;

			m_pServerTalkView->SetShow(m_bDrawTalkServer);
			m_pServerTalkView->SetEnable(m_bDrawTalkServer);
			if(m_bDrawTalkServer)
			{
				int iPos = g_TalkMgr.GetServerTalk().size() - m_pServerTalkView->GetLineCount();
				if(iPos < 0)
					iPos = 0;

				m_pServerTalkView->SetDisLine(iPos);
			}

			return true;
		}
		else if (pControl == m_pPromptInfo)
		{
			PromptInfo* pPromptInfo = g_pGameData->GetPromptInfoFirst();

			if (pPromptInfo && pPromptInfo->eMsgWnd != MSG_GAMEWNDS_MESSAGE_BEGIN)
			{
				g_pControl->PopupWindow(pPromptInfo->eMsgWnd,OPER_RECREATE,0);

				pPromptInfo = g_pGameData->GetPromptInfoFirst();
				if (pPromptInfo)
				{
					m_pPromptInfo->SetEnable(false);
					m_pPromptInfo->SetShow(false);
				}
			}
			else g_pGameData->DelPromptInfo();

			return true;
		}
		else if (pControl == m_pInstantHint)
		{
			if(g_pControl->FindWindowByName("FairyGuide") != NULL)
			{
				g_pControl->MsgToWnd("FairyGuide",MSG_CTRL_UPDATE_PROMPT_INFO);
				Msg(MSG_CTRL_UPDATE_INSTANCE_INFO,0,0);
				return true;
			}

			if (g_pGameData->HasCareForInfo())
			{
				g_pControl->PopupWindow(MSG_CTRL_FAIRYGUIDE_WND,OPER_CREATE,0);
				Msg(MSG_CTRL_UPDATE_INSTANCE_INFO,0,0);				
			}

		}
		else if (pControl == m_pBtnAutoKill)
		{
			if (g_pGameData->IsAutoKillMonster())
			{
				g_AutoKillMonsterMgr.SetEnabled(!g_pGameData->IsAutoKillMonster());
			}
			else
			{
				g_MsgBoxMgr.PopOkCancelBox("为了您的安全，请进行相关设置后再使用自动打怪功能，以防止不必要的损失。\n注意：包裹满后请及时清理，否则将无法继续拾取道具。",
					MSG_CTRL_AUTOKILL, 0);				
			}

			if (g_pGameData->IsAutoKillMonster())
				m_pBtnAutoKill->SetText("取消自动打怪", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);
			else
				m_pBtnAutoKill->SetText("自动打怪", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);

			
			return true;
		}
		else if (pControl == m_pWidowBtn)
		{
			ChangeWindowSize(g_pGfx->GetWidth(),g_pGfx->GetHeight(),DM_WINDOWED);
		}
		else if (pControl == m_pBtnQiYu)
		{
			m_pBtnQiYu->SetFlashTexID(MAKELONG(22425,PACKAGE_INTERFACE));

			if (g_OtherData.IsCanClickQiYu())
			{
				g_pGameControl->SEND_Request_Adventrue();
			}
			return true;
		}
		else if (pControl == m_pBtnNoticeIE)
		{
			if (!g_pGameData->GetNoticeIEURL().empty())
			{
				g_pControl->PopupWindow(MSG_CTRL_NOTICEIE_WND,OPER_RECREATE);
			}

			g_pGameData->SetNoticeIEURL("");
			m_pBtnNoticeIE->SetEnable(false);
			m_pBtnNoticeIE->SetShow(false);

			g_pGameData->SetNoticeIEState(0);
			return true;
		}
		else if (pControl == m_pBtnBing)
		{			
			g_pControl->PopupWindow(MSG_CTRL_DEFEND12CTRL_WND,OPER_CREATE);	
			m_pBtnBing->SetEnable(false);
			m_pBtnBing->SetShow(false);
			return true;
		}
		else if (pControl == m_pBtnBindPT)
		{
			if (g_Login.GetLoginInExpType() == 1)
			{
				g_pControl->PopupWindow(MSG_CTRL_BINDPTWND_WND,OPER_CREATE);
				return true;
			}
			return true;
		}
		break;
	case MSG_CTRL_POP_MENU:
		{
			if(dwData >= POP_MENU_ITEM_ATTACK_TYPE_SE && dwData <= POP_MENU_ITEM_ATTACK_TYPE_WH)
			{
				int iType = (int)(dwData - POP_MENU_ITEM_ATTACK_TYPE_SE);
				if(iType != SELF.GetReserveData(plyAttackType))
				{
					char strTemp[56]={0};
					sprintf(strTemp,"@AttackMode %d",iType);
					g_pGameControl->SEND_Guild_Ally(strTemp);
				}
			}

			return true;
		}
		break;
	case MSG_CTRL_LOCATE_AREA:
		{
			m_byLocalAreaType = (BYTE)dwData;
		}
		break;
	case MSG_CTRL_TALKVIEW_LCLICK:
		OnClickSeverMsg();
		break;
	case MSG_CTRL_RUNHORSELAMP_WND:
		{
			if (dwData == OPER_CLOSE)
			{
				RemoveControl((CControl**)(&m_pRunHorseLamp));
				m_pRunHorseLamp = NULL;
			}
			else if (dwData == OPER_CREATE && !m_pRunHorseLamp)
			{
				m_pRunHorseLamp = new CRunHorseLampWnd;
				AddControl(m_pRunHorseLamp);
				m_pRunHorseLamp->Create(this);
			}
		}
		break;
	case MSG_CTRL_DEFEND12GONGBTN:
		m_pBtnBing->SetEnable(dwData != 0);
		m_pBtnBing->SetShow(dwData != 0);
		break;
	case MSG_CTRL_AUTOKILLBTN:
		{
			if (g_pGameData->IsAutoKillMonster())
				m_pBtnAutoKill->SetText("取消自动打怪", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);
			else
				m_pBtnAutoKill->SetText("自动打怪", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);
		}
		break;
	case MSG_ARROWTIP_CLICK_CLOSEBTN:
		{
			int iSelfX = 0,iSelfY = 0;
			SELF.GetScrXY(iSelfX,iSelfY);

			if ((int)dwData == EP_FirstInGmae)
			{
				AddArrowTip(EP_FirstInGmae_Step2,iSelfX + 84,iSelfY + 52,XAL_TOPLEFT,false,XAL_TOPLEFT);

				if (!g_pGameData->HasPaoPaoStatus(EP_First_AutoFindPath))
				{
					m_pTaskTrackViewer->Msg(MSG_CTRL_POPUP_ARROWTIP,EP_First_AutoFindPath,NULL);
				}
			}
			else if ((int)dwData == EP_FirstInGmae_Step2)
			{
				AddArrowTip(EP_FirstInGmae_Step3,iSelfX + 84,iSelfY - 52,XAL_TOPLEFT,false,XAL_BOTTOMLEFT);
			}
			else if ((int)dwData == EP_FirstInGmae_Step3)
			{
				AddArrowTip(EP_FirstInGmae_Step4,iSelfX + 84,iSelfY + 52,XAL_TOPLEFT,false,XAL_TOPLEFT);
			}
		}
		break;
	default:
		break;
	}

	return CCtrlWindow::Msg(dwMsg,dwData,pControl);
}

void CGameWnd::OnEscape(void)
{
	////在游戏窗口按esc,把焦点给最上层窗口，并且调用该窗口的OnEscape();关闭该窗口
	//g_pControl->SetTopWindowFocus();
	//CControl* pFocusControl = g_pControl->GetFocusCtrl();
	//if(pFocusControl)
	//{
	//	int iMode = pFocusControl->GetControlMode();
	//	if(iMode == CTRL_MODE_WINDOW || iMode == CTRL_MODE_MESSAGEBOX)
	//	{
	//		CCtrlWindow *pWnd = dynamic_cast<CCtrlWindow *>(pFocusControl);
	//		if(pWnd)
	//		{
	//			pWnd->OnEscape();
	//		}
	//	}
	//}

	g_pControl->CloseTopWindow(); // 关闭顶层窗口
}

bool CGameWnd::OnLeftButtonDown(int iX,int iY)
{
	if (g_pGameData->HasPaoPaoStatus(EP_FirstInGmae_Step2) && !g_pGameData->HasPaoPaoStatus_Closed(EP_FirstInGmae_Step2))
	{
		this->Msg(MSG_REMOVE_ARROWTIP_CONTROL,EP_FirstInGmae_Step2);
	}

	if (g_pGameData->HasPaoPaoStatus(EP_FirstInGmae) && !g_pGameData->HasPaoPaoStatus_Closed(EP_FirstInGmae))
	{
		this->Msg(MSG_REMOVE_ARROWTIP_CONTROL,EP_FirstInGmae);
	}

	//鼠标第一次点击
	g_pGameControl->Send_Player_Prompt_Status(22,1);

	stGoodFrom &GoodFrom = CGoodGrid::GetDropGoodFrom();

 	if(GoodFrom.DropGood.GetID() != 0)
 	{
		int iCallMonsterType = 0;

		if (GoodFrom.DropGood.GetStdMode() == 49 && GoodFrom.DropGood.GetShape() == 55)//封元印
			iCallMonsterType = 1;
		else if (GoodFrom.DropGood.GetStdMode() == 3 && GoodFrom.DropGood.GetShape() == 216)//箭塔
			iCallMonsterType = 2;

		if (iCallMonsterType > 0)
		{
			bool bSucess = false;

			if((iCallMonsterType == 1 && strnicmp(g_pGameMap->GetMapName(),"wxxg",4) == 0) ||//五行玄关之中
			   (iCallMonsterType == 2 && strnicmp(g_pGameMap->GetRealMapName(),"RTS0001",7) == 0)//RTS0001副本
				)
			{
				int x,y;			
				m_GameMgr.GetMouseTile(x,y);
				CSimpleCharacterNode* pChar = g_pGameData->FindSimpleCharacter(g_pControl->GetMouseOnID());
				if((pChar == NULL || pChar->IsDead()))
				{
					if (iCallMonsterType == 1)
					{
						bSucess = g_OtherData.IsSummonPoint(MAKELONG(x,y));					
					}
					else if (iCallMonsterType == 2)
					{
						bSucess = g_OtherData.IsRtsSummonPoint(x,y);					
					}
				}

				UCHAR cDir = m_GameMgr.GetPlayerDir(iX,iY);
				g_pGameControl->SEND_Player_Turn(char(cDir));
				SELF.SetDir(cDir);

				//增加一个举手的动作魔法
				//增加一个魔法
				if(bSucess)
				{
					SELF.InitAction(ACTION_MAGIC);				
					SAction& ActionNow = SELF.GetAction();                

					ActionNow.iAimX = x;
					ActionNow.iAimY = y;
					ActionNow.iData = MAGICID_CALL_MONSTER_WXXG;
					ActionNow.uData = GoodFrom.DropGood.GetID();


					char cTemp[12] = {0};		
					*(DWORD*)(cTemp) = SELF.GetID();
					*(WORD*)(cTemp + 10) = 91;
					g_pGameControl->MSG_Item_Effect_Switch(cTemp,12);		
				}			
			}

			if(bSucess)
			{
				CGood * pGood = SELF.GetUsingTemp().Add(GoodFrom.DropGood);
				if(pGood)
				{
					pGood->SetPos(GoodFrom.iWndPos);
					CGoodGrid::ClearGoodFrom();
				}
				return true;
			}
			else
			{
				if (iCallMonsterType == 1)
				{
					g_TalkMgr.PushSysTalk("使用封元印必须处于离火封魔阵之中，点击拖动至召唤法阵");            
				}
			}		
		}


		//1.908 商城道具	黄榜大旗
		if(GoodFrom.DropGood.GetID() && GoodFrom.DropGood.GetStdMode() == 37 && GoodFrom.DropGood.GetShape() == 40)
		{
			CSimpleCharacter* pChar = g_pGameData->FindSimpleCharacter(g_pControl->GetMouseOnID());
			if(pChar==NULL)
				return true;

			if(pChar->GetLooks().Char.wShape == 172)	//172，代表 棋座
			{
				g_pControl->PopupWindow(MSG_CTRL_YELLOW_FLAG_WND,OPER_CREATE,GoodFrom.DropGood.GetID());	//新建
				g_pControl->Msg(MSG_CTRL_YELLOW_FLAG_WND,MAKELONG(pChar->GetX(),pChar->GetY()));
			}
			else
			{
				g_MsgBoxMgr.PopSimpleAlert("请把黄榜大旗放在旗座上。");
				return true;
			}

			int i = SELF.PackageGood().FindGoodPos(0);
			if(i >= 0 && i < MAX_PACKAGE_ELEMENT)
			{
				SELF.GetPackageGood(i) = GoodFrom.DropGood;
			}

			CGoodGrid::ClearGoodFrom();
			return true;
		}


		//丢物品
		if(g_pGameData->GetDropMoneyFrom() != EDMYF_NONE)
			return true;
	}



	if(strnicmp(g_pGameMap->GetMapName(),"wxxg",4) == 0)   //五行玄关之中
	{
		int iMouseX,iMouseY;
		m_GameMgr.GetMouseTile(iMouseX,iMouseY);
		if(g_OtherData.IsSummonPoint(MAKELONG(iMouseX,iMouseY)))
		{
			CSimpleCharacterNode* pChar = g_pGameData->FindSimpleCharacter(g_pControl->GetMouseOnID());
			if(pChar == NULL || pChar->IsDead())
			{
				g_MsgBoxMgr.PopSimpleAlert("\n\n     此为召唤法阵，拖动封元印至此处可以召唤出封元印中的怪物");                	
				return true;
			}
		}
	}



	m_GameMgr.OnLeftButtonDown(iX,iY);

	return CCtrlWindow::OnLeftButtonDown(iX,iY);
}

bool CGameWnd::OnLeftButtonUp(int iX,int iY)
{
	m_bClick = false;
	stGoodFrom &GoodFrom = CGoodGrid::GetDropGoodFrom();
	//从包裹或腰带上往外扔东西
	if(GoodFrom.DropGood.GetID() != 0 && (GoodFrom.eFromWnd == Package_Wnd||GoodFrom.eFromWnd == Panel_Wnd))
	{
		if(g_pControl->FindWindowByName("TradeWnd") != NULL)
			return true;

		//  [2/22/2010 dujun]
		DWORD dwGoodType = g_PickCfgMgr.GetShowType(GoodFrom.DropGood.GetName());		
		if (dwGoodType & (ITEMSHOWTYPE_BEST | ITEMSHOWTYPE_EXPENSIVE))
		{
			char buff[128];
			sprintf_s(buff, 128, "是否确认丢弃%s?",GoodFrom.DropGood.GetName());
			g_MsgBoxMgr.PopOkCancelBox(buff,MSG_CTRL_REJECTOBJECT,GoodFrom.DropGood.GetID());
		}
		else
		{
			g_pGameControl->SEND_Package_Object_Reject(GoodFrom.DropGood);
		}
		
		CGoodGrid::ReleaseDrop();
		return true;
	}

	if(g_pGameData->GetDropMoneyFrom() != EDMYF_NONE)
	{
		if(g_TradeData.GetTradeFlag())
			return true;

		g_MsgBoxMgr.PopEditBox("你想放下多少钱?",MSG_CTRL_DROPMONEY,0,NULL,true);
		g_pGameData->SetDropMoneyFrom(EDMYF_NONE);
		return true;
	}

	SkillShortCut& lSkillShortCut = g_pGameData->GetSkillShortCut();
	if(lSkillShortCut.iMagicID != 0)	
	{		
		if(g_pGameData->GetSkillShortCut().iShortCut != -1)
		{
			/*if (lSkillShortCut.iMagicID == MAGICID_FAZHEN_CARRYON)
			{
				g_pGameControl->SEND_YIHUO_NAME_SHORTCUT(0,SELF.GetYIHUOINFO().strName);
				SELF.SetYiHuoShortCutKey(0);
			}
			else*/
			{
				g_pGameControl->SEND_ShortCut_Key_Change(g_pGameData->GetSkillShortCut().iMagicID,0);
				CMagicData * pMagic = SELF.FindMagic(g_pGameData->GetSkillShortCut().iMagicID);
				if(pMagic)
				{
					pMagic->SetShortCutKey(0);
				}
			}			
		}

		if(g_pGameData->GetSkillShortCut().iShortCutEx != -1)
		{
			/*if (lSkillShortCut.iMagicID == MAGICID_FAZHEN_CARRYON)
			{
				SELF.SetYiHuoShortCutKeyEx(0);
				g_AICfgMgr.SaveShortCutConfig();
			}
			else*/
			{
				CMagicData * pMagic = SELF.FindMagic(g_pGameData->GetSkillShortCut().iMagicID);
				if(pMagic)
				{
					pMagic->SetShortCutKeyEx(0);
					g_AICfgMgr.SaveShortCutConfig();
				}
			}			
		}

		g_pGameData->SetSkillShortCut(0,0,-1,-1);
		return true;
	}
	else if (lSkillShortCut.pConSkill)
	{
		if(g_pGameData->GetSkillShortCut().iShortCut != -1)
		{
			if (lSkillShortCut.pConSkill)
			{
				lSkillShortCut.pConSkill->dwKey = 0;
				g_AICfgMgr.SaveShortCutConfig();
			}			
		}

		if(g_pGameData->GetSkillShortCut().iShortCutEx != -1)
		{
			if (lSkillShortCut.pConSkill)
			{
				lSkillShortCut.pConSkill->dwKey = 0;
				g_AICfgMgr.SaveShortCutConfig();
			}
		}

		g_pGameData->SetSkillShortCut(0,0,-1,-1);
		return true;
	}

	//清掉快捷栏鼠标
	CQuickItem& qitem = g_WooolStoreMgr.GetDropQuickItem();
	if(qitem.strItemID.size() > 0)
	{
		qitem.clear();
		return true;
	}

	m_GameMgr.OnLeftButtonUp(iX,iY);
	return CCtrlWindow::OnLeftButtonUp(iX,iY);
}

bool CGameWnd::OnLeftButtonDClick(int iX,int iY)
{
	m_GameMgr.OnLeftButtonDClick(iX,iY);
	return CCtrlWindow::OnLeftButtonDClick(iX,iY);
}

bool CGameWnd::OnRightButtonDown(int iX,int iY)
{
	if (g_pGameData->HasPaoPaoStatus(EP_FirstInGmae_Step4) && !g_pGameData->HasPaoPaoStatus_Closed(EP_FirstInGmae_Step4))
	{
		this->Msg(MSG_REMOVE_ARROWTIP_CONTROL,EP_FirstInGmae_Step4,NULL);
	}
	
	if (g_pGameData->HasPaoPaoStatus(EP_FirstInGmae_Step3) && !g_pGameData->HasPaoPaoStatus_Closed(EP_FirstInGmae_Step3))
	{
		this->Msg(MSG_REMOVE_ARROWTIP_CONTROL,EP_FirstInGmae_Step3,NULL);
	}


	m_GameMgr.OnRightButtonDown(iX,iY);
	return CCtrlWindow::OnRightButtonDown(iX,iY);
}

bool CGameWnd::OnRightButtonUp(int iX,int iY)
{
	m_GameMgr.OnRightButtonUp(iX,iY);
	return CCtrlWindow::OnRightButtonUp(iX,iY);
}

bool CGameWnd::OnRightButtonDClick(int iX,int iY)
{
	m_GameMgr.OnRightButtonDClick(iX,iY);
	return CCtrlWindow::OnRightButtonDClick(iX,iY);
}

bool CGameWnd::OnMiddleButtonDown(int iX,int iY)
{
	m_GameMgr.OnMiddleButtonDown(iX,iY);
	return CCtrlWindow::OnMiddleButtonDown(iX,iY);
}


bool CGameWnd::OnMouseMove(int iX,int iY)
{
	CParserTip *pTip = g_pControl->GetTipWnd();
	pTip->SetShow(false);//为了清除其它窗口留下的tips鼠标进入了GameWnd统统清除tips
	m_GameMgr.OnMouseMove(iX,iY);

	const int cliStartX = 10;
	const int cliStartY = 25;

	int liStartX = cliStartX;

	std::vector<ConSkillBuff>& lBuffs = g_AIMgr.GetConSkillBuffs();
	if(iX > cliStartX && iY > cliStartY && iX < cliStartX + lBuffs.size() * 32 && iY < cliStartY+29)
	{
		CParserTip *pTip = g_pControl->GetTipWnd();
		if (pTip)
		{
			pTip->Clear();

			for (int i = 0; i < lBuffs.size(); ++i)
			{
				ConSkillBuff& lConSkillBuff = lBuffs[i];
				if (lConSkillBuff.bActive && lConSkillBuff.nTotalTime > 0)
				{
					if(iX > liStartX && iY > cliStartY && iX < liStartX+29 && iY < cliStartY+29)
					{
						if (lConSkillBuff.bActive && lConSkillBuff.nTotalTime > 0)
						{
							pTip->AddText(lConSkillBuff.nName.c_str());

							int x = m_iScreenX + iX + 10;
							int y = m_iScreenY + iY + 10;
							pTip->AdjustXY(x,y);
							pTip->Move(x,y);
							pTip->SetShow(true);
							return true;
						}
					}
					liStartX += 32;
				}
			}
		}		
	}
	

 	if(strnicmp(g_pGameMap->GetMapName(),"wxxg",4) == 0)
 	{
 
 		if(iX > 0 && iY > 125 && iX < 220 && iY < 160)
 		{
 			pTip->Clear();
 			pTip->AddText("你共需防守6轮");
 			pTip->Move(iX + 10,iY + 10);
 			pTip->SetShow(true);
 			return true;
 		}
 		else if(iX > 0 && iY > 165 && iX < 220 && iY < 200)
 		{
 			pTip->Clear();
 			pTip->AddText("如果逃跑怪物超过20个，防守将宣告失败");
 			pTip->Move(iX + 10,iY + 10);
 			pTip->SetShow(true);
 			return true;
 		}
 		else if(iX > 0 && iY > 205 && iX < 220 && iY < 240)
 		{
 			pTip->Clear();
 			pTip->AddText("召唤封元印中的怪物需要消耗封元神力，而消灭逃跑怪物可以累积封元神力。");
 			pTip->Move(iX + 10,iY + 10);
 			pTip->SetShow(true);
 			return true;
 		}
 		else
 		{
 			pTip->SetShow(false);
 		}
 	}
 	else if(g_OtherData.GetCenserDurTime() > 0 && iX > 4 && iY > g_pGfx->GetHeight() - 10 - 190 && iX < 4 + 11 && iY < g_pGfx->GetHeight() - 10)
	{
		pTip->Clear();

		int iLeft = (g_OtherData.GetCenserDurTime() - (GetTickCount() - g_OtherData.GetCenserStartTime())) / 1000 / 60 + 1;

		char str[256] = "";
		sprintf(str,"剩余时间： %d分钟",iLeft);
		pTip->AddText(str);		

		int x = m_iScreenX + iX + 10;
		int y = m_iScreenY + iY + 10;		
		pTip->AdjustXY(x,y);
		pTip->Move(x,y);
		pTip->SetShow(true);

		return true;
	}
 	else if (strnicmp(g_pGameMap->GetMapName(),"maze",4) == 0)
	{
		CCharacterAttr* pChar = g_pGameData->FindSimpleCharacter(g_pControl->GetMouseOnID());
		if (pChar)
		{
			if (pChar->GetRaceNo() == 208)
			{	
				CParserTip *pTip = g_pControl->GetTipWnd();
				pTip->Clear();
				pTip->AddText("使用迷仙路引飞向机关，可以打开机关");
				pTip->Move(iX,iY + 20);
				pTip->SetShow(true);
				return true;
			}
		}
	}

	return CCtrlWindow::OnMouseMove(iX,iY);
}

//结束特殊的魔法操作
void CGameWnd::OnKillFocus()
{
}

bool CGameWnd::OnKeyUp(WORD wState,UCHAR cKey)
{
	if(m_GameMgr.OnKeyUp(wState,cKey))
		return true;
	return CCtrlWindow::OnKeyUp(wState,cKey);
}

bool CGameWnd::OnKeyDown(WORD wState,UCHAR cKey)
{
	switch(cKey)
	{
	case VK_ESCAPE:
		OnEscape();
		return true;
	default:
		break;
	}

	if(m_GameMgr.OnKeyDown(wState,cKey))
		return true;

	return CCtrlWindow::OnKeyDown(wState,cKey);
}

//系统公告中如果出现商城道具，点击以后跳到相应的商城页面 如:"测试：商城里有御兽天衣(包月)出售: <御兽天衣(包月)/@@openstore/50/20/1424>"
void CGameWnd::OnClickSeverMsg()
{
	//openstore/大类/小类/ItemID
	std::string strCommand;

// 	strCommand = m_pSystemTalkView->GetCommand();
// 	if(strCommand.length() <= 5)
// 		return;
// 
// 	//返回点击的字符串的头
// 	std::string strLine;
// 	if(g_TalkMgr.IsShowSystemLeft())
// 	{
// 		if(strncmp(strCommand.c_str(),"@@c2c",5) == 0)
// 		{
// 			if(g_PetBoothData.IsPaimaiClosed()) //判断是否关闭了拍卖行
// 				return;
// 
// 			g_WooolStoreMgr.SetWooolStorePage(WOOOLSTORE_PAIMAI);
// 			g_WooolStoreMgr.SetPaiMaiStoreType(5);
// 
// 			strLine = strCommand.substr(5,strCommand.size() - 1);
// 			g_PetBoothData.SetRedirectPage(strLine.c_str());
// 			g_WidgetMgr.LoginPaiMai();
// 			g_pControl->PopupWindow(MSG_CTRL_WOOOL_STORE_WND,OPER_CREATE,3);
// 			return;
// 		}
// 		else if(strncmp(strCommand.c_str(),"@@buy",5) == 0)
// 		{
// 			CQuickItem qitem;
// 			strLine = strCommand.substr(5,strCommand.size() - 1);
// 			int pos = strLine.find_first_of('|');
// 			if(pos > 0)
// 			{
// 				qitem.strName = strLine.substr(0,pos);
// 				qitem.strItemID = strLine.substr(pos+1,strLine.size() - pos - 1);
// 			}
// 			else
// 			{
// 				qitem.strItemID = strLine;
// 			}
// 			g_WooolStoreMgr.BuyQuickItem(qitem,!qitem.strName.empty());
// 			return;
// 		}
// 		return;
// 	}

	strCommand = m_pServerTalkView->GetCommand();
	if(strCommand.length() <= 5)
		return;

	if(strncmp(strCommand.c_str(),"@@openstore",11) == 0)
	{
		int iPos1 = strCommand.find("/",0);
		int iPos2 = strCommand.find("/",iPos1+1);
		if(iPos1 < 0 || iPos2 < 0)
			return;

		std::string strMainGroup = strCommand.substr(iPos1+1,iPos2-iPos1-1);
		int iMainGroup = atoi(strMainGroup.c_str());

		iPos1 = strCommand.find("/",iPos2+1);
		if(iPos1 < 0)
			return;

		std::string strSubGroup = strCommand.substr(iPos2+1,iPos1-iPos2-1);
		int iSubGroup = atoi(strSubGroup.c_str());

		int id = atoi(strCommand.substr(iPos1+1,strCommand.length()-iPos1-1).c_str());

		COpenStorePara  &OpenStorePara = g_WooolStoreMgr.GetOpenStorePara();
		OpenStorePara.iMainGroup = iMainGroup/10-1;
		OpenStorePara.iSubGroup = iSubGroup/10-1;
		OpenStorePara.iIndex = 0;
		OpenStorePara.id = id;


		//如果有这个商品数据打开商城对应的页面，如果没有则发协议请求，结果返回时会自动打开对应的商城页面
		int iIndex = g_WooolStoreMgr.GetItemIndexByID(OpenStorePara.iMainGroup, OpenStorePara.iSubGroup,OpenStorePara.id);

		if(iIndex < 0)
		{	
			char szTemp[256] = {0};
			if(!g_WooolStoreMgr.GetSubGroupByIndex(OpenStorePara.iMainGroup, OpenStorePara.iSubGroup))
			{
				sprintf(szTemp, "%d", (OpenStorePara.iMainGroup+1)*10);
				g_pGameControl->SEND_PT_Store_Info(2, szTemp, strlen(szTemp));
				Sleep(2);
			}

			sprintf(szTemp, "%d", ((OpenStorePara.iMainGroup + 1) * 1000 + (OpenStorePara.iSubGroup + 1) * 10));
			g_pGameControl->SEND_PT_Store_Info(3, szTemp, strlen(szTemp));
		}
		else
		{
			OpenStorePara.iIndex = iIndex;
			g_pControl->PopupWindow(MSG_CTRL_WOOOL_STORE_WND,OPER_CREATE,7);
		}
	}
}

void CGameWnd::Draw()
{
	TRY_BEGIN

#ifdef ENABLE_REPLAY
	//播放结束了以后不处理update及DoLoop
	if (!g_ReplayManager.IsInReplay() || !g_ReplayManager.IsReplayEnd())
#endif
	{
		if(!SELF.IsDead() && m_bStartAI)
			g_AIAutoMgr.DoLoop();  //辅助工具处理

		m_GameMgr.Update(g_pControl->GetFocusCtrl() == this);
	}

	if(!g_pGfx->IsNoDraw())
	{
		g_pControl->SetMouseOnID(0);
		g_pControl->SetMouseOnType(0);
		g_pControl->SetMouseOnHeight(0);
	}


	m_SceneMgr.Draw(g_pControl->GetTipOwnerWnd() == this); //绘制场景
	TRY_END

		TRY_BEGIN
		CCtrlWindow::Draw();

	TRY_END

		TRY_BEGIN

		DrawConSkillBuff();	// 绘制技能连击buff
		DrawQiYu();
		DrawNoticeIE();
		Draw8DunProgress();

		g_AIAutoMgr.ExtraDraw();//辅助工具绘制
	//绘制滚动淡出的系统公告
	if(!m_bDrawTalkServer)
		DrawScrollMsg();

	/////////////精英勋章飘红提醒
	DrawClientPiaoHong();

	////五行玄关提示
	DrawWXXGTips();
	DrawWXXGTimeOut();
	//铁血战歌,rts副本
	DrawTXZGTips();
	//世界杯
	DrawTSJB();

	//千里传音
	DWORD dwTelTime = g_OtherData.GetTelTime();
	if(dwTelTime>0 && GetTickCount() - dwTelTime < 120000)
	{
		WORD wTelColor = 0;
		const char* strTelStr = g_OtherData.GetTelStr(wTelColor);
		if(strTelStr && strlen(strTelStr) > 0)
		{
			int iTelX = g_pGfx->GetWidth() / 2 - FONTSIZE_DEFAULT*strlen(strTelStr)/4;
			int iTelY = g_pGfx->GetHeight() - 200 - FONTSIZE_DEFAULT - 2;
			g_pFont->DrawText(m_iScreenX+iTelX,m_iScreenY+iTelY,strTelStr,0xFF00FF00,FONT_DEFAULT,FONTSIZE_DEFAULT,DTF_BlackFrame);
		}
	}


	switch(m_byLocalAreaType)			// 所在的城区
	{
	case 1:				// 战斗
		g_pFont->DrawText(4,4,"战斗",0xFFFFFF00);
		break;
	case 4:				// 攻城区
		g_pFont->DrawText(4,4,"攻城区",0xFFFFFFFF);
		break;
	case 8:
		g_pFont->DrawText(4,4,"安全",0xFF00FF00);
		break;
	default:
		break;
	}

	DrawFlyText();
	DrawTimeOut();
	DrawPromptInfo();

	m_pInstantHint->SetShow(g_pGameData->HasCareForInfo());
	m_pInstantHint->SetEnable(g_pGameData->HasCareForInfo());
	m_pBtnAutoKill->SetShow(g_AICfgMgr.IsAutoKillShow());
	m_pBtnAutoKill->SetEnable(g_AICfgMgr.IsAutoKillShow());
	if (m_pBtnBindPT)
	{
		m_pBtnBindPT->SetShow(g_Login.GetLoginInExpType() == 1);
		m_pBtnBindPT->SetEnable(g_Login.GetLoginInExpType() == 1);
	}

	if (g_hParentWnd && m_pWidowBtn)
	{
		DisplayMode eDisMode = g_pGfx->GetDisMode();
		m_pWidowBtn->SetShow((eDisMode == DM_FULL_FALSE || eDisMode == DM_FULL_TRUE));
		m_pWidowBtn->SetEnable((eDisMode == DM_FULL_FALSE || eDisMode == DM_FULL_TRUE));
	}

	TRY_END
}

bool CGameWnd::GetDropedObject(int iTileX,int iTileY)
{
	CSimpleGoodNode * pGood = MapArray.GetSimpleGoodHead(iTileX,iTileY);
	if( !pGood ) return false;

	int x,y;
	SELF.GetPlayerXY(x,y);
	if(iTileX == x && iTileY == y)
	{
		g_pGameControl->SEND_Object_Pickup(x,y);
	}

	return true;
}

void CGameWnd::OnChangeAttackType()
{
	CCtrlMenuWnd* pMenuWnd = g_pControl->GetMenuWnd();
	pMenuWnd->Clear();

	S_POP_MENU_DATA & PopMenuData = pMenuWnd->GetData();
	PopMenuData.fItemHeight = 15.0f;
	PopMenuData.iX = m_pAttackType->GetScreenX()+1;
	PopMenuData.iY = m_pAttackType->GetScreenY() + m_pAttackType->GetHeight() + 1;
	PopMenuData.iLeftOffX = PopMenuData.iRightOffX = 3;
	PopMenuData.iTopOffY = PopMenuData.iBottomOffY = 2;
	PopMenuData.fItemHeight = 18.0f;
	PopMenuData.iFontType = FONT_YAHEI;
	PopMenuData.dwItemTextColor = 0xffb46428;
	PopMenuData.dwHoverTextColor = 0xffffe650;
	PopMenuData.dwSelectColor = 0xff1e6464;

	pMenuWnd->AddMenuItem(POP_MENU_ITEM_ATTACK_TYPE_SE,"善恶攻击");
	pMenuWnd->AddMenuItem(POP_MENU_ITEM_ATTACK_TYPE_ALL,"全体攻击");
	pMenuWnd->AddMenuItem(POP_MENU_ITEM_ATTACK_TYPE_PEACE,"和平攻击");
	pMenuWnd->AddMenuItem(POP_MENU_ITEM_ATTACK_TYPE_GROUP,"编组攻击");
	pMenuWnd->AddMenuItem(POP_MENU_ITEM_ATTACK_TYPE_GUILD,"行会攻击");
	pMenuWnd->AddMenuItem(POP_MENU_ITEM_ATTACK_TYPE_ST,"师徒攻击");
	pMenuWnd->AddMenuItem(POP_MENU_ITEM_ATTACK_TYPE_WH,"夫妻攻击");

	m_bClick = false;
	m_bRBClick = false;//否则MenuWnd获得不到焦点
	pMenuWnd->SetCaller(this);
	pMenuWnd->ShowMenu();
	g_pControl->GetTipWnd()->SetShow(false);
}

// void CGameWnd::OnHidePlayerMsg()
// {
// 	bool bShowSystemMsgLeft = !g_TalkMgr.IsShowSystemLeft();
// 	g_TalkMgr.SetShowSystemLeft(bShowSystemMsgLeft);
// 
// 	if(bShowSystemMsgLeft)
// 	{
// 		m_pPlayerMsg_Hide->ReloadTex(14347,14348,14349);
// 		m_pSystemTalkView->SetEnable(true);
// 		m_pSystemTalkView->SetShow(true);
// 		m_pPlayerMsg_Hide->SetTips("隐藏窗口");
// 
// 		g_TalkMgr.RemoveSysTalkToLeft();
// 	}
// 	else//把自己相关的内容中的最后一屏导入到聊天档里
// 	{
// 		m_pPlayerMsg_Hide->ReloadTex(14344,14345,14346);
// 		m_pSystemTalkView->SetEnable(false);
// 		m_pSystemTalkView->SetShow(false);
// 		m_pPlayerMsg_Hide->SetTips("显示窗口");
// 
// 		g_TalkMgr.RemoveSysTalkToPanel();
// 	}    
// }

void CGameWnd::DrawScrollMsg()
{
	TalkType1 &mulSerTalk = g_TalkMgr.GetMultiServerTalk();
	if(mulSerTalk.size() == 0)
		return;

	int i,j;
	int x = 340;
	if ((g_pGfx->GetWidth() == 1024))
	{
		x = 467;
	}
	else if (g_pGfx->GetWidth() == 1280)
	{
		x = 600;
	}

	int y = 70;
	int iSize = mulSerTalk.size();
	int iMaxDrawLines = 5;//最多绘制几行
	int iDrewLineCount = 0;//已经绘制了的行数

	for(i = iSize - 1; i >= 0 && iDrewLineCount < iMaxDrawLines; i--)
	{
		CMultiLine *pMultiLine = mulSerTalk.at(i);
		if(!pMultiLine)
			break;

		int iLines = pMultiLine->getLineCount();
		int iAlpha = pMultiLine->getAlpha();
		if(i == 0 || iDrewLineCount + iLines >= iMaxDrawLines)//只有第一行MultiLine渐隐
			iAlpha --;
		else
			iAlpha = 400;

		pMultiLine->setAlpha(iAlpha);

		if(iAlpha <= 10)
			break;
		else if(iAlpha > 255)
			iAlpha = 255;

		for(j = iLines - 1;j >= 0 && iDrewLineCount < iMaxDrawLines;j--)
		{
			CStringLine *pLine = pMultiLine->getLine(j);
			if(!pLine)
				continue;

			DWORD dwColor = ((((BYTE)iAlpha) << 24) | 0x00FFFFFF) & (pLine->getColor() | 0xFF000000);
			pLine->setColor(dwColor);
			pLine->setFlag(pLine->getFlag() | DTF_Center | DTF_BlackFrame);
			pLine->Draw(x,y);

			iDrewLineCount ++;
			y -= 16;
		}
	}


	///////////////超过了绘制行数或是iAlpha <= 10的行删除它//////
	if(i >= 0)
	{
		for(int k = 0; k <= i; k++)
		{
			SAFE_DELETE(mulSerTalk.at(k));
		}

		mulSerTalk.erase(mulSerTalk.begin(),mulSerTalk.begin() + i + 1);
	}
}


void CGameWnd::DrawClientPiaoHong()
{
	CGood& good = SELF.GetEquipGood(ITEM_POS_MEDAL);

	if(good.GetID() != 0 && good.GetLooks() == 8020)
	{

		char cTemp[16] = {0};		
		for(int i = 0;i<10;i++)
			cTemp[i] = good.GetResvEx(i);

		if(cTemp[0])
		{
			DWORD dwTemp = *((DWORD*)(cTemp + 5));
			__time32_t t_time;
			_time32(&t_time);
			t_time += g_dwServerTime;
			int iTime = (int)((dwTemp - t_time)/60);

			if(iTime > 0 && iTime < 60)
			{
				if(GetTickCount() - m_dwShowPiaoHongTime >= 300000)//5分钟绘制一次
				{
					m_dwShowPiaoHongTime = GetTickCount();
					_OtherMsg MsgTemp;
					char strTemp[128]={0};
					sprintf(strTemp,"历练任务还有%d分钟结束，请尽快到皇宫史官处评定你的称号",iTime);
					MsgTemp.strMsg.assign(strTemp);
					MsgTemp.wColor = 0xFFFF;
					g_OtherData.GetOtherMsg2().push_back(MsgTemp);
				}
			}
		}
	}
}
 void CGameWnd::DrawWXXGTips()
 {
 	if(strnicmp(g_pGameMap->GetMapName(),"wxxg",4) == 0)
 	{
 		WUXINGFLAG& flag = g_OtherData.GetWuXingFlag();
 		if(GetTickCount() - flag.dwStartTime < 16000)	//显示提示
 		{
 			int dwPassTick = GetTickCount() - flag.dwStartTime;
 
 			int iStartPos = 0;
 			int iEndPos = 0;
 			if(!g_pGfx->IsBig())
 			{
 				iStartPos = 550;
 				iEndPos = 190;
 			}
 			else
 			{
 				iStartPos = 700;
 				iEndPos = 300;
 			}
 
 			DWORD dwTimeDev = 0;
 			if(dwPassTick < 8000)
 			{				
 				dwTimeDev = 8000 - dwPassTick;
 
 			}
 			else
 			{
 				dwTimeDev = dwPassTick - 8000;								
 			}
 
 			BYTE  byTemp = (BYTE)((1.0 - ((float)dwTimeDev / 8000.0) ) * 0xFF);	
 			DWORD dwTemp = (byTemp<<24);
 			dwTemp |= 0x00FFFFFF;
 			dwTemp &= 0xFFFFFF00;	
 			g_pFont->DrawText(iEndPos,150,"妖魔即将冲破封印力量的束缚，请勇士尽快召唤出封元印中的怪物协助防守",dwTemp,FONT_DEFAULT,FONTSIZE_MIDDLE);
 		}
 
 		//绘制玩家信息(轮数，召唤灵力数)
 
 
 		LPTexture pTex1 = NULL,pTex2 = NULL,pTex3 = NULL;
 
 		//轮数
 		DWORD dwRound = flag.dwCurrentRound;
 
 		bool bFlash = false;
 		DWORD dwPassTick = GetTickCount() - flag.dwRoundChangeTm;
 
 		if(dwPassTick < 5000 && dwPassTick % 600 < 300)
 		{
 			bFlash = true;		//数据改变时的闪烁效果
 		}
 
 		pTex1 = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15535,EP_UI);
 		if(pTex1)
 		{
 			g_pGfx->DrawTextureNL(0,125,pTex1);
 			if(bFlash)
 			{
 				pTex1 = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15550,EP_UI);
 				if(pTex1)
 				{
 					g_pGfx->SetRenderMode(RM_ADD1);
 					g_pGfx->DrawTextureNL(0 - 10,125 - 7,pTex1);
 					g_pGfx->SetRenderMode();
 				}
 			}
 		}
 
 
 
 		if(dwRound >= 0 && dwRound <= 9)
 		{
 			pTex2 = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15538 + dwRound,EP_UI);
 			if(pTex2)
 			{
 				g_pGfx->DrawTextureNL(180,133,pTex2);
 
 				if(bFlash)
 				{
 					pTex2 = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15553 + dwRound,EP_UI);
 					if(pTex2)
 					{
 						g_pGfx->SetRenderMode(RM_ADD1);
 						g_pGfx->DrawTextureNL(180 - 8,133 - 6,pTex2);
 						g_pGfx->SetRenderMode();
 					}
 				}
 			}
 		}
 		else if(dwRound > 9)
 		{
 			pTex2 = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15538 + dwRound / 10,EP_UI);
 			pTex3 = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15538 + dwRound % 10,EP_UI);
 			if(pTex2 && pTex3)
 			{
 				g_pGfx->DrawTextureNL(180,132,pTex2);
 				g_pGfx->DrawTextureNL(195,132,pTex3);
 
 				if(bFlash)
 				{
 					pTex2 = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15553 + dwRound / 10,EP_UI);
 					pTex3 = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15553 + dwRound % 10,EP_UI);
 					if(pTex2 && pTex3)
 					{
 						g_pGfx->SetRenderMode(RM_ADD1);
 						g_pGfx->DrawTextureNL(180 - 8,132 - 6,pTex2);
 						g_pGfx->DrawTextureNL(195 - 8,132 - 6,pTex3);
 						g_pGfx->SetRenderMode();
 					}
 				}
 			}
 		}
 
 		//逃跑怪物
 		DWORD dwMonster = flag.dwEscapeMon;
 
 		dwPassTick = GetTickCount() - flag.dwEscapeMonTm;
 		bFlash = false;
 		if(dwPassTick < 5000 && dwPassTick % 600 < 300)
 		{
 			bFlash = true;		//数据改变时的闪烁效果
 		}
 
 		pTex1 = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15536,EP_UI);
 		if(pTex1)
 		{
 			g_pGfx->DrawTextureNL(0,165,pTex1);
 
 			if(bFlash)
 			{
 				pTex1 = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15551,EP_UI);
 				if(pTex1)
 				{
 					g_pGfx->SetRenderMode(RM_ADD1);
 					g_pGfx->DrawTextureNL(0 - 8 ,165 - 7,pTex1);
 					g_pGfx->SetRenderMode();
 				}
 			}
 		}
 
 
 
 		if(dwMonster >= 0 && dwMonster <= 9)
 		{
 			pTex2 = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15538 + dwMonster,EP_UI);
 			if(pTex2)
 			{
 				g_pGfx->DrawTextureNL(180,171,pTex2);
 
 				if(bFlash)
 				{
 					pTex2 = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15553 + dwMonster,EP_UI);
 					if(pTex2)
 					{
 						g_pGfx->SetRenderMode(RM_ADD1);
 						g_pGfx->DrawTextureNL(180 - 8,171 - 6,pTex2);
 						g_pGfx->SetRenderMode();
 					}
 				}
 			}
 		}
 		else if(dwMonster > 9)
 		{
 			pTex2 = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15538 + dwMonster / 10,EP_UI);
 			pTex3 = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15538 + dwMonster % 10,EP_UI);
 			if(pTex2 && pTex3)
 			{
 				g_pGfx->DrawTextureNL(180,171,pTex2);
 				g_pGfx->DrawTextureNL(195,171,pTex3);
 
 				if(bFlash)
 				{
 					pTex2 = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15553 + dwMonster / 10,EP_UI);
 					pTex3 = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15553 + dwMonster % 10,EP_UI);
 					if(pTex2 && pTex3)
 					{
 						g_pGfx->SetRenderMode(RM_ADD1);
 						g_pGfx->DrawTextureNL(180 - 8,171 - 6,pTex2);
 						g_pGfx->DrawTextureNL(195 - 8,171 - 6,pTex3);
 						g_pGfx->SetRenderMode();
 					}
 				}
 			}
 		}
 
 		//封元神力
 		DWORD dwGuard = flag.dwGuarderNum;
 		bFlash = false;
 		dwPassTick = GetTickCount() - flag.dwGuardChangeTm;
 		if(dwPassTick < 5000 && dwPassTick % 600 < 300)
 		{
 			bFlash = true;		//数据改变时的闪烁效果
 		}
 
 		pTex1 = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15537,EP_UI);
 		if(pTex1)
 		{
 			g_pGfx->DrawTextureNL(0,200,pTex1);
 
 			if(bFlash)
 			{
 				pTex1 = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15552,EP_UI);
 				if(pTex1)
 				{
 					g_pGfx->SetRenderMode(RM_ADD1);
 					g_pGfx->DrawTextureNL(0 - 12,200 - 9,pTex1);
 					g_pGfx->SetRenderMode();
 				}
 			}
 		}		
 
 		if(dwGuard >= 0 && dwGuard <= 9)
 		{
 			pTex2 = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15538 + dwGuard,EP_UI);
 			if(pTex2)
 			{
 				g_pGfx->DrawTextureNL(180,206,pTex2);
 
 				if(bFlash)
 				{
 					pTex2 = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15553 + dwGuard,EP_UI);
 					if(pTex2)
 					{
 						g_pGfx->SetRenderMode(RM_ADD1);
 						g_pGfx->DrawTextureNL(180 - 8,206 - 6,pTex2);
 						g_pGfx->SetRenderMode();
 					}
 				}
 			}
 		}
 		else if(dwGuard > 9)
 		{
 			pTex2 = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15538 + dwGuard / 10,EP_UI);
 			pTex3 = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15538 + dwGuard % 10,EP_UI);
 			if(pTex2 && pTex3)
 			{
 				g_pGfx->DrawTextureNL(180,206,pTex2);
 				g_pGfx->DrawTextureNL(195,206,pTex3);
 
 				if(bFlash)
 				{
 					pTex2 = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15553 + dwGuard / 10,EP_UI);
 					pTex3 = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15553 + dwGuard % 10,EP_UI);
 					if(pTex2 && pTex3)
 					{
 						g_pGfx->SetRenderMode(RM_ADD1);
 						g_pGfx->DrawTextureNL(180 - 8,206 - 6,pTex2);
 						g_pGfx->DrawTextureNL(195 - 8,206 - 6,pTex3);
 						g_pGfx->SetRenderMode();
 					}
 				}
 			}
 		}        
 	}
 }

void CGameWnd::DrawTimeOut()
{
	VTimeOut &vTimeOut = g_OtherData.GetTimeOut();
	int iType1 = 0,iType2 = 0;
	for (int i = 0; i < vTimeOut.size(); i++)
	{
		_TimeOut & timeout = vTimeOut[i];
		if(timeout.dwStartTime <= 0)
			return;

		DWORD dwTickCount = GetTickCount();
		if(dwTickCount - timeout.dwStartTime < timeout.dwLastTime)
		{
			DWORD dwLeaveTick = timeout.dwLastTime + timeout.dwStartTime - dwTickCount;
			int iLeaveSecond = dwLeaveTick / 1000 + 1;

			if((timeout.iType >= 1 && timeout.iType < 10000) || (timeout.iType >= 20001 && timeout.iType < 30000) || (timeout.iType >= 40001 && timeout.iType < 50000))
			{
				float fMinSize = 0.4f, fMaxSize = 1.5f;
				int iX = 0;
				int iY = 0;
				//_timeout中的monsterid =0 代表玩家,中心显示
				if (timeout.dwMonsterID == 0)
				{
					int iWidth = g_pGfx->GetWidth();
					if (iWidth == 800)
					{
						iX = 305;
						iY = 197;
					}
					else if (iWidth == 1024)
					{
						iX = 430;
						iY = 257;
					}
					else if (iWidth == 1280)
					{
						iX = 430;
						iY = 554;
					}
					else
					{
						iX = iWidth / 2 - 100;
						iY = g_pGfx->GetHeight() / 2 - 50;
					}

				} 
				else
				{
					CCharacterAttr *pChar = g_pGameData->FindCharacterByID(timeout.dwMonsterID);
					if (pChar)
					{
						pChar->GetTopXY(iX,iY);
						iX -= 100;
						iY -= 15;
					}
					else
					{
						timeout.dwStartTime = 1;
						timeout.dwLastTime = 0;
						break;
					}
				}
				

				float fDura = (float)(dwLeaveTick % 1000) / 1000;
				const float fCurSize = fMaxSize * fDura + fMinSize * (1.0f-fDura);
				DWORD dwColor = ((DWORD)(0xFF*fDura)<<24) + 0xFFFFFF;

				//读秒音效
				if(abs(timeout.iLastMusicSecond - iLeaveSecond) >= 1 && iLeaveSecond != 1)
				{
					int iSelfX,iSelfY;
					SELF.GetXY(iSelfX,iSelfY);
					g_pAudio->PlayEx(EAT_OTHER,807, g_pAudio->GetRand()++,iSelfX,iSelfX,iSelfY,iSelfY,true);
					timeout.iLastMusicSecond = iLeaveSecond;
				}

				int iStartIdx = 12810;
				int iSize = 80;
				if (timeout.iType >= 40001 && timeout.iType < 50000)
				{
					iX += 45;
					iY += 10;
					iStartIdx = 17160;
					iSize = 25;
					dwColor = -1;
				}

				int iInterval = (int)(iSize*fCurSize);
				int iLeft = iSize-iInterval;


				if( iLeaveSecond < 10)	//显示1位数的倒计时
				{
					LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,iStartIdx+iLeaveSecond,EP_UI);
					if(pTex)
						g_pGfx->DrawTextureNL_Trans(iX+iSize/2+iLeft/2,iY, pTex,fCurSize,fCurSize,0,dwColor);
				}
				else	//显示2位数的倒计时
				{
					LPTexture pTex1 = g_pTexMgr->GetTex(PACKAGE_INTERFACE,iStartIdx+iLeaveSecond/10,EP_UI);
					LPTexture pTex2 = g_pTexMgr->GetTex(PACKAGE_INTERFACE,iStartIdx+iLeaveSecond%10,EP_UI);

					if(pTex1 && pTex2)
					{
						g_pGfx->DrawTextureNL_Trans(iX+iLeft,iY, pTex1,fCurSize,fCurSize,0,dwColor);
						g_pGfx->DrawTextureNL_Trans(iX+iLeft+iInterval,iY, pTex2,fCurSize,fCurSize,0,dwColor);
					}
				}
			}

			if((timeout.iType >= 10001 && timeout.iType < 20000) || (timeout.iType >= 30001 && timeout.iType < 40000))
			{
				char strTemp[1024]={0},strTemp2[256]={0};;
				int iMin = iLeaveSecond/60;
				int iX = 10,iY = 10;

				if(timeout.bShowSecond)
				{
					sprintf(strTemp2,"%d分%d秒",iMin,iLeaveSecond%60);
				}
				else
				{
					sprintf(strTemp2,"%d分钟",(iMin<=0?1:iMin));
				}

				sprintf(strTemp,timeout.strMsg.c_str(),strTemp2);

				if (timeout.iType == 30010)//提示下次领奖时间
				{
					iX = 40;
					//if(g_EutUiType == EUT_CLASSIC)
					//	iY = g_pGfx->GetHeight() - 240;
					//else
						iY = g_pGfx->GetHeight() - 330;

					if (dwLeaveTick % 1000 < 500)
					{
						LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_ptitems,3001,EP_UI);
						if(pTex)
							g_pGfx->DrawTextureNL(iX,iY - 18, pTex);
					}

					g_pFont->DrawText(iX + 47,iY + FONTSIZE_MIDDLE * i,strTemp,0xFFFFFF00,FONT_DEFAULT,FONTSIZE_MIDDLE,DTF_BlackFrame);
					iType2 ++;
				}
				else if (timeout.iType == 30015)//提示世界杯比赛计时
				{
					iX = 40;
					iY = g_pGfx->GetHeight() - 240;

					/*if (dwLeaveTick % 1000 < 500)
					{
						LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_ptitems,3001);
						if(pTex)
							g_pGfx->DrawTextureNL(iX,iY - 18, pTex);
					}*/
					g_pFont->DrawText(iX + 47,iY + FONTSIZE_MIDDLE * i,"世界杯计时:",0xFFFFFF00,FONT_DEFAULT,FONTSIZE_MIDDLE,DTF_BlackFrame);
					g_pFont->DrawText(iX + 47,iY + FONTSIZE_MIDDLE * i,strTemp,0xFFFFFF00,FONT_DEFAULT,FONTSIZE_MIDDLE,DTF_BlackFrame);
					iType2 ++;
				}
				else
				{
					g_pFont->DrawText(iX,iY + FONTSIZE_MIDDLE * iType1,strTemp,0xFFFFFF00,FONT_DEFAULT,FONTSIZE_MIDDLE,DTF_BlackFrame);
					iType1 ++;
				}		
			}
		}
		else
		{
			if(timeout.iType == 1)
			{
				m_bStartDefend = true;
				m_dwDefenceTime = GetTickCount();
			}

			vTimeOut.erase(vTimeOut.begin() + i);
			i--;
		}
	}
}

 void CGameWnd::DrawWXXGTimeOut()
 {
 	if(strnicmp(g_pGameMap->GetMapName(),"wxxg",4) == 0)
 	{
 		WUXINGFLAG& flag = g_OtherData.GetWuXingFlag();
 
 		float fMinSize = 0.4f, fMaxSize = 1.5f;
 
 		int iX = g_pGfx->IsBig()?430:305;
 		int iY = g_pGfx->IsBig()?360:300;
 
 		DWORD dwDelayCount = 2000;	//持续显示的时间
 
 		if(m_bStartDefend)
 		{
 			DWORD dwLeftTick = dwDelayCount + m_dwDefenceTime - GetTickCount();
 
 			if(GetTickCount() - m_dwDefenceTime > dwDelayCount)	//显示“开始防守”四个字的时间
 			{
 				m_bStartDefend = false;
 				m_dwDefenceTime = 0;
 			}
 
 			float fDura = 0;
 			if(dwLeftTick > dwDelayCount - 500)
 			{
 				fDura = (float)((dwLeftTick - (dwDelayCount - 500)) % 1000) / 500;
 			}
 			else
 			{
 				fDura = 0;
 			}
 			float size = (fMaxSize * (1.0f - fDura) + fMinSize * fDura) * 0.5f;
 			DWORD dwColor = ((DWORD)(0xFF * (1 - fDura))<<24) + 0xFFFFFF;
 			LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15191,EP_UI);
 			if(pTex)
 			{
 				int x = iX - (int)(322 * size) + 125;
 				g_pGfx->DrawTextureNL_Trans(x,iY - 100, pTex,size,size,0,dwColor);					
 			}
 		}
 
 		_WuXingResult& result = g_NPC.GetWuXingResult();
 		if(result.bGameOver)
 		{
 			if(GetTickCount() - result.dwOverTime > 4 * dwDelayCount)	//结果显示的时间
 			{
 				result.bGameOver = false;
 				if(result.bySuccess == 0)
 				{
 					g_pAudio->Play(EAT_OTHER,906,g_pAudio->GetRand()++);
 					if(result.strPrize.size() != 0)
 					{
 						g_pControl->PopupWindow(MSG_CTRL_WUXING_PRIZE_WND,OPER_RECREATE,1);
 					}
 					else
 					{
 						g_pControl->PopupWindow(MSG_CTRL_WUXING_PRIZE_WND,OPER_RECREATE,2);
 					}
 				}
 			}
 			else if(((GetTickCount() - result.dwOverTime) < ( 4 * dwDelayCount ) ) && ((GetTickCount() - result.dwOverTime) > (3 * dwDelayCount)))	//6 - 8秒
 			{
 				DWORD dwLeftTick = 4 * dwDelayCount + result.dwOverTime - GetTickCount();
 
 				float fDura = 0;
 				if(dwLeftTick > dwDelayCount - 500)
 				{
 					fDura = (float)((dwLeftTick - (dwDelayCount - 500)) % 1000) / 500;
 				}
 				else
 				{
 					fDura = 0;
 				}
 				float size = (fMaxSize * (1.0f - fDura) + fMinSize * fDura) * 0.5f;
 				DWORD dwColor = ((DWORD)(0xFF * (1 - fDura))<<24) + 0xFFFFFF;
 				LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15192 + result.bySuccess,EP_UI);
 				LPTexture pTex1 = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15571,EP_UI);
 				if(pTex && pTex1)
 				{
 					int x = iX - (int)(322 * size * 0.8) + 125;
 					g_pGfx->DrawTextureNL_Trans(x,iY, pTex,size * 0.8f,size * 0.8f,0,dwColor);	
 					x = iX - (int)(280 * size) + 115;
 					g_pGfx->DrawTextureNL_Trans(x,iY - (int)(pTex->GetHeight0() * size * 0.8f), pTex1,size,size,0,dwColor);
 				}			
 			}
 
 		}
 
 		if(result.bRoundOver)		//画每回合结束提示
 		{	
 			fMinSize = 1.0f, fMaxSize = 2.0f;
 			if(GetTickCount() - result.dwRoundOverTm > 3 * dwDelayCount)	//结果显示的时间
 			{
 				result.bRoundOver = false;	
 				if(g_OtherData.GetWuXingFlag().dwCurrentRound < 6 && !result.bGameOver)		//未结束
 				{
 					g_OtherData.GetWuXingFlag().dwCurrentRound++;
 					g_OtherData.GetWuXingFlag().dwRoundChangeTm = GetTickCount();
 				}
 			}
 			else if(GetTickCount() - result.dwRoundOverTm < 2 * dwDelayCount && GetTickCount() - result.dwRoundOverTm > dwDelayCount)	//提示防守结束
 			{
 				DWORD dwLeftTick = 2 * dwDelayCount + result.dwRoundOverTm - GetTickCount();
 
 				float fDura = 0;
 				if(dwLeftTick > dwDelayCount - 1500)
 				{
 					fDura = 1.0f;
 				}
 				else
 				{
 					fDura = (float)(dwLeftTick) / 500;
 				}
 				float size = (fMaxSize * fDura + fMinSize * (1.0f - fDura)) * 0.5f;
 				DWORD dwColor = ((DWORD)(0xFF * (fDura))<<24) + 0xFFFFFF;		
 
 				LPTexture pTex1 = NULL, pTex2 = NULL, pTex3 = NULL;
 
 				pTex1 = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15508,EP_UI);
 
 				if(result.dwRound >= 10)
 				{
 					pTex2 = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15510,EP_UI);
 					pTex3 = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15511,EP_UI);
 
 					if(pTex1 && pTex2 && pTex3)
 					{
 						int x = iX - (int)(300 * size) + 50;
 						g_pGfx->DrawTextureNL_Trans(x,iY - 100, pTex1,size,size,0,dwColor);
 						x = iX - (int)(200 * size) + 100;
 						g_pGfx->DrawTextureNL_Trans(x,iY - 100, pTex3,size,size,0,dwColor);
 						x = iX - (int)(160 * size) + 100;
 						g_pGfx->DrawTextureNL_Trans(x,iY - 100, pTex2,size,size,0,dwColor);
 					}                    				
 				}
 				else 
 				{
 					pTex2 = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15510 + result.dwRound,EP_UI);
 					if(pTex1 && pTex2)
 					{
 						int x = iX - (int)(300 * size) + 50;
 						g_pGfx->DrawTextureNL_Trans(x,iY - 100, pTex1,size,size,0,dwColor);
 						x = iX - (int)(180 * size) + 100;
 						g_pGfx->DrawTextureNL_Trans(x,iY - 100, pTex2,size,size,0,dwColor);
 					}                    
 				}
 			}
 			else if(GetTickCount() - result.dwRoundOverTm > 2 * dwDelayCount)		//提示获得多少经验
 			{
 				fMinSize = 1.0f, fMaxSize = 2.0f;
 				DWORD dwLeftTick = 3 * dwDelayCount + result.dwRoundOverTm - GetTickCount();
 
 				float fDura = 0;
 				if(dwLeftTick > dwDelayCount - 500)
 				{
 					fDura = (float)((dwLeftTick - (dwDelayCount - 500)) % 1000) / 500;
 				}
 				else
 				{
 					fDura = 0;
 				}
 				float size = (fMaxSize * (1.0f - fDura) + fMinSize * fDura) * 0.5f;
 				DWORD dwColor = ((DWORD)(0xFF * (1 - fDura))<<24) + 0xFFFFFF;
 
 				LPTexture pTex1 = NULL, pTex2 = NULL, pTex3 = NULL, pTex4 = NULL;
 				pTex1 = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15509,EP_UI);				
 				pTex2 = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15526 + result.bySuccess,EP_UI);
 				if((result.dwRoundExp >= 0) && (result.dwRoundExp < 10))
 				{
 					pTex3 = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15510 + result.dwRoundExp,EP_UI);
 					if(pTex1 && pTex2 && pTex3)
 					{
 						int x = iX - (int)(320 * size) + 120;
 						g_pGfx->DrawTextureNL_Trans(x,iY - 70 - (int)(pTex1->GetHeight0() * size), pTex2,size,size,0,dwColor);
 						if(result.dwRoundExp > 0)		//经验值为0不绘制
 						{
 							x = iX - (int)(250 * size) + 50;
 							g_pGfx->DrawTextureNL_Trans(x,iY - 70, pTex1,size,size,0,dwColor);
 							x = iX - (int)(50 * size) + 100;
 							g_pGfx->DrawTextureNL_Trans(x,iY - 70, pTex3,size,size,0,dwColor);	
 						}
 					}                    
 				}
 				else if(result.dwRoundExp >= 10)
 				{
 					pTex3 = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15510 + result.dwRoundExp / 10,EP_UI);
 					pTex4 = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15510 + result.dwRoundExp % 10,EP_UI);
 					if(pTex1 && pTex2 && pTex3 && pTex4)
 					{
 						int x = iX - (int)(320 * size) + 120;
 						g_pGfx->DrawTextureNL_Trans(x,iY - 70 - (int)(pTex1->GetHeight0() * size), pTex2,size,size,0,dwColor);
 						x = iX - (int)(250 * size) + 50;
 						g_pGfx->DrawTextureNL_Trans(x,iY - 70, pTex1,size,size,0,dwColor);
 						x = iX - (int)(60 * size) + 100;
 						g_pGfx->DrawTextureNL_Trans(x,iY - 70, pTex3,size,size,0,dwColor);	
 						x = iX - (int)(20 * size) + 100;
 						g_pGfx->DrawTextureNL_Trans(x,iY - 70, pTex4,size,size,0,dwColor);	
 					}                    
 				}		
 
 			}
 		}
 	}
 }

void CGameWnd::DrawFlyText()
{
	//在天绝魔域服务器一直滚动提示相关信息
	if(g_Login.IsTianJueMoYu())
	{
		static const char * strZmhMsg[7] = {
			"将行会旗帜插在祭祀王台上,并保护旗帜半小时不被砍到,就能获得胜利",
			"胜利后的行会将获得大量天绝金的奖励",
			"整场战斗将持续2小时",
			"天绝魔域上各种怪物死亡后都会掉落天绝金",
			"混世魔尊那里可以补给战斗所需的药品",
			"密室引路人将带你进入魔域密室,在密室中可以无限获取天绝金",
			"如果己方三块生命石碑被摧毁,一旦死亡后就必须等待180秒才能进入战场",
		};

		int iLen[7] = {440,238,140,286,244,370,468};

		static int iZmhMsgItem = 0;
		static int iZmhMsgDx = g_pGfx->GetWidth() - 150;

		iZmhMsgDx -= 1;

		if(iZmhMsgDx < -iLen[iZmhMsgItem])
		{
			iZmhMsgItem++;
			iZmhMsgDx = g_pGfx->GetWidth() - 150;

			if(iZmhMsgItem >= 7)
			{
				iZmhMsgItem = 0;
			}
		}
		else
		{
			g_pFont->DrawText(iZmhMsgDx,51,strZmhMsg[iZmhMsgItem],0xFF00FF00,FONT_DEFAULT,FONTSIZE_MIDDLE);
		}
	}
}

void CGameWnd::DrawPromptInfo()
{
	if (!g_pGameData) return;

	PromptInfo* pPromptInfo = g_pGameData->GetPromptInfoFirst();
	if (pPromptInfo && pPromptInfo->wLooks)
	{

		if (pPromptInfo->dwTmStart == 0)
		{
			pPromptInfo->dwTmStart = GetTickCount();
		}
		else
		{
			DWORD dwTm = GetTickCount();
			if (dwTm >= pPromptInfo->dwTmStart + pPromptInfo->dwTime)
			{
				g_pGameData->DelPromptInfo();
				return;
			}
		}

		m_pPromptInfo->SetShow(true);
		m_pPromptInfo->SetEnable(true);

		LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,pPromptInfo->wLooks,EP_UI);
		if (g_pGfx->GetWidth() <= 800)
			g_pGfx->DrawTextureNL(m_iScreenX + g_pGfx->GetWidth()/2 - 13,m_iScreenY + g_pGfx->GetHeight() - 172,pTex);
		else 
			g_pGfx->DrawTextureNL(m_iScreenX + g_pGfx->GetWidth()/2 - 10,m_iScreenY + g_pGfx->GetHeight() - 142,pTex);
	}
	else
	{
		m_pPromptInfo->SetShow(false);
		m_pPromptInfo->SetEnable(false);
	}
}

void CGameWnd::DrawTXZGTips()
{
	sTXZG &txzg = g_OtherData.GetTXZG();
	if(txzg.bStarted)
	{
		char szTemp[128] = {0};

		if (txzg.byType == 1)
		{
			char szTemp[128] = {0};
			//g_pFont->DrawText(200,50,"当前资源:",0xFFFFFF00,FONT_YAHEI,28);
			//sprintf(szTemp,"红方 %u / %u蓝方",txzg.wCurRes_Red,txzg.wCurRes_Blue);
			//g_pFont->DrawText(300,50,szTemp,0xFFFFFF00,FONT_YAHEI,28);
			//sprintf(szTemp,"个人贡献: X %u",txzg.wContribute);		
			//g_pFont->DrawText(30,200,szTemp,0xFFFFFF00,FONT_YAHEI,28,DTF_BlackFrame,0,0xFFFF0000);


			int iX = g_pGfx->GetWidth() / 2;//中心
			iX -= 150;

			g_pGfx->DrawTextureNL(iX,15, g_pTexMgr->GetTex(PACKAGE_INTERFACE,9327,EP_UI));
			iX += 110;
			g_pGfx->DrawTextureNL(iX,15 + 3, g_pTexMgr->GetTex(PACKAGE_INTERFACE,9331,EP_UI));

			sprintf(szTemp,"%d",txzg.wCurRes_Red);
			char *p = szTemp;
			iX += 50;

			while(*p)
			{
				g_pGfx->DrawTextureNL(iX,15 + 2, g_pTexMgr->GetTex(PACKAGE_INTERFACE,9332 + *p - '0',EP_UI));
				iX += 14;
				p ++;
			}

			g_pGfx->DrawTextureNL(iX,15 + 1, g_pTexMgr->GetTex(PACKAGE_INTERFACE,9329,EP_UI));
			iX += 20;

			sprintf(szTemp,"%d",txzg.wCurRes_Blue);
			p = szTemp;
			while(*p)
			{
				g_pGfx->DrawTextureNL(iX,15 + 2, g_pTexMgr->GetTex(PACKAGE_INTERFACE,9344 + *p - '0',EP_UI));
				iX += 14;
				p ++;
			}

			iX += 10;
			g_pGfx->DrawTextureNL(iX,15 + 3, g_pTexMgr->GetTex(PACKAGE_INTERFACE,9343,EP_UI));

			iX = 10;//中心

			g_pGfx->DrawTextureNL(iX,150, g_pTexMgr->GetTex(PACKAGE_INTERFACE,9328,EP_UI));
			iX += 100;
			g_pGfx->DrawTextureNL(iX,150 + 3, g_pTexMgr->GetTex(PACKAGE_INTERFACE,9355,EP_UI));
			iX += 25;
			g_pGfx->DrawTextureNL(iX,150 - 1, g_pTexMgr->GetTex(PACKAGE_INTERFACE,9356,EP_UI));
			iX += 25;

			sprintf(szTemp,"%d",txzg.wContribute);
			p = szTemp;
			while(*p)
			{
				g_pGfx->DrawTextureNL(iX,150 - 1, g_pTexMgr->GetTex(PACKAGE_INTERFACE,9357 + *p - '0',EP_UI));
				iX += 16;
				p ++;
			}
		}
		else if (txzg.byType == 2)
		{
			int iX = g_pGfx->GetWidth() / 2;//中心
			iX -= 270;

			g_pGfx->DrawTextureNL(iX,15, g_pTexMgr->GetTex(PACKAGE_INTERFACE,9368,EP_UI));//雇佣兵击杀数量的文字
			iX += 230;
			g_pGfx->DrawTextureNL(iX,15, g_pTexMgr->GetTex(PACKAGE_INTERFACE,txzg.bySide==1?9374:9373,EP_UI));//红方
			iX += 50;
			g_pGfx->DrawTextureNL(iX,15, g_pTexMgr->GetTex(PACKAGE_INTERFACE,9375,EP_UI));//我方
			iX += 80;

			sprintf(szTemp,"%d",txzg.bySide==1?txzg.wKillMonsterNum_Blue:txzg.wKillMonsterNum_Red);//红方击怪数
			char *p = szTemp;

			while(*p)
			{
				g_pGfx->DrawTextureNL(iX,15 + 2, g_pTexMgr->GetTex(PACKAGE_INTERFACE,9332 + *p - '0',EP_UI));
				iX += 14;
				p ++;
			}

			g_pGfx->DrawTextureNL(iX,15 + 1, g_pTexMgr->GetTex(PACKAGE_INTERFACE,9329,EP_UI));
			iX += 20;

			sprintf(szTemp,"%d",txzg.bySide==1?txzg.wKillMonsterNum_Red:txzg.wKillMonsterNum_Blue);//蓝方击怪数
			p = szTemp;
			while(*p)
			{
				g_pGfx->DrawTextureNL(iX,15 + 2, g_pTexMgr->GetTex(PACKAGE_INTERFACE,9344 + *p - '0',EP_UI));
				iX += 14;
				p ++;
			}

			iX += 2;
			g_pGfx->DrawTextureNL(iX,15, g_pTexMgr->GetTex(PACKAGE_INTERFACE,txzg.bySide==1?9373:9374,EP_UI));//蓝方
			iX += 50;
			g_pGfx->DrawTextureNL(iX,15, g_pTexMgr->GetTex(PACKAGE_INTERFACE,9376,EP_UI));//敌方


			iX = 10;
			g_pGfx->DrawTextureNL(iX,150, g_pTexMgr->GetTex(PACKAGE_INTERFACE,9327,EP_UI));//当前资源
			iX = 110;
			g_pGfx->DrawTextureNL(iX,150, g_pTexMgr->GetTex(PACKAGE_INTERFACE,9373,EP_UI));//红方
			iX += 50;
			sprintf(szTemp,"%d",txzg.wCurRes_Red);//红方当前资源
			p = szTemp;
			while(*p)
			{
				g_pGfx->DrawTextureNL(iX,150 + 2, g_pTexMgr->GetTex(PACKAGE_INTERFACE,9332 + *p - '0',EP_UI));
				iX += 14;
				p ++;
			}

			g_pGfx->DrawTextureNL(iX,150 + 1, g_pTexMgr->GetTex(PACKAGE_INTERFACE,9329,EP_UI));
			iX += 20;

			g_pGfx->DrawTextureNL(iX,150, g_pTexMgr->GetTex(PACKAGE_INTERFACE,9374,EP_UI));//蓝方
			iX += 50;
			sprintf(szTemp,"%d",txzg.wCurRes_Blue);//蓝方当前资源
			p = szTemp;
			while(*p)
			{
				g_pGfx->DrawTextureNL(iX,150 + 2, g_pTexMgr->GetTex(PACKAGE_INTERFACE,9344 + *p - '0',EP_UI));
				iX += 16;
				p ++;
			}

			iX = 10;
			g_pGfx->DrawTextureNL(iX,180, g_pTexMgr->GetTex(PACKAGE_INTERFACE,9369,EP_UI));//中立资源归属方
			iX += 185;
			if (txzg.byResSide == 1)
			{
				g_pGfx->DrawTextureNL(iX,180, g_pTexMgr->GetTex(PACKAGE_INTERFACE,9373,EP_UI));//红方
			}
			else if (txzg.byResSide == 2)
			{
				g_pGfx->DrawTextureNL(iX,180, g_pTexMgr->GetTex(PACKAGE_INTERFACE,9374,EP_UI));//蓝方
			}

		}

	}
}

void CGameWnd::DrawTSJB()
{
	
	char szTemp[128] = {0};
	sTSJB &tsjb = g_OtherData.GetTSJB();
	if (tsjb.bStarted)
	{
		sprintf(szTemp,"%u",tsjb.FirstTeamScore);//红方击怪数
		char *p = szTemp;

		int iX = g_pGfx->GetWidth() / 2;//中心
		while(*p)
		{
			g_pGfx->DrawTextureNL(iX,55 + 2, g_pTexMgr->GetTex(PACKAGE_INTERFACE,9332 + *p - '0',EP_UI));
			iX += 14;
			p ++;
		}

		g_pGfx->DrawTextureNL(iX,55 + 1, g_pTexMgr->GetTex(PACKAGE_INTERFACE,9329,EP_UI));

		iX += 20;

		sprintf(szTemp,"%u",tsjb.SecondTeamScore);//蓝方击怪数
		p = szTemp;
		while(*p)
		{
			g_pGfx->DrawTextureNL(iX,55 + 2, g_pTexMgr->GetTex(PACKAGE_INTERFACE,9344 + *p - '0',EP_UI));
			iX += 14;
			p ++;
		}

	}
	
}

void CGameWnd::ResetControlPos()
{
	m_iWidth = g_pGfx->GetWidth();
	m_iHeight = g_pGfx->GetHeight();

	m_pInstantHint->SetOriginalOffX(0);
	m_pInstantHint->SetOriginalOffY(-314);

	m_pBtnNoticeIE->SetOriginalOffX(g_pGfx->GetWidth()/2 - 25);
	m_pBtnNoticeIE->SetOriginalOffY(g_pGfx->GetHeight() - 190);

	m_pBtnAutoKill->SetOriginalOffX((int)(-130*g_ScaleRate.fx));
	m_pBtnAutoKill->SetOriginalOffY((int)(-90*g_ScaleRate.fy));
	if(m_pWidowBtn)
	{
		m_pWidowBtn->SetOriginalOffX((int)(128*g_ScaleRate.fx));
		m_pWidowBtn->SetOriginalOffY((int)(-90*g_ScaleRate.fy));
	}
	if (m_pBtnBindPT)
	{		
		m_pBtnBindPT->SetOriginalOffY((int)(-130*g_ScaleRate.fy));
	}

	CControl *p = m_pControls;
	while(p) 
	{
		p->RelativeMove();
		p = p->GetControlNext(); 
	}
}

void CGameWnd::DrawConSkillBuff()
{
	g_AIMgr.UpdateConSkillBuff();

	const int cliStartX = 10;
	const int cliStartY = 25;

	int liStartX = cliStartX;
	char strtemp[32] = {0};

	LPTexture pMask = g_pTexMgr->GetTex(PACKAGE_INTERFACE,18072,EP_UI);
	LPTexture pTex = 0;

	std::vector<ConSkillBuff>& lBuffs = g_AIMgr.GetConSkillBuffs();
	for (int i = 0; i < lBuffs.size(); ++i)
	{
		ConSkillBuff& lConSkillBuff = lBuffs[i];
		if (lConSkillBuff.bActive && lConSkillBuff.nTotalTime > 0)
		{
			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,lConSkillBuff.nIcon,EP_UI);
			if (pTex)
				g_pGfx->DrawTextureNL(m_iScreenX+liStartX, m_iScreenY+cliStartY, pTex);			

			float arc = 360.0f;
			if (lConSkillBuff.nNowTime - lConSkillBuff.nStartTime < lConSkillBuff.nTotalTime)
			{
				arc = (lConSkillBuff.nNowTime - lConSkillBuff.nStartTime) * 360.0f / lConSkillBuff.nTotalTime;
				arc = max(0, min(arc, 360.0f));
			}
			if (arc < 360.0f)
				g_pGfx->DrawArcTexture(m_iScreenX + liStartX + 15,m_iScreenY + cliStartY + 15,pMask,arc,360.0f,0xad1e1e1e);

			int iShowTime = lConSkillBuff.nNowTime - lConSkillBuff.nStartTime;
			iShowTime = lConSkillBuff.nTotalTime - iShowTime;
			if (iShowTime < 0)
				iShowTime = 0;
			iShowTime = (int)ceil(iShowTime * 0.001f);

			sprintf(strtemp, "%ds", iShowTime);
			// 秒数
			g_pFont->DrawText(m_iScreenX+liStartX + 13,m_iScreenY+cliStartY + 16, strtemp, 0xFFFFFFFF, FONT_DEFAULT, FONTSIZE_DEFAULT,DTF_BlackFrame);

			// 亮框
			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,21589,EP_UI);
			if (pTex)
			{
				g_pGfx->SetRenderMode(RM_ADD2);
				g_pGfx->DrawTextureNL(m_iScreenX+liStartX-1,m_iScreenY+cliStartY,pTex);
				g_pGfx->SetRenderMode();
			}

			liStartX += 32;
		}
	}
}


void CGameWnd::DrawQiYu()
{
	if (!g_OtherData.IsHaveQiYu())
	{
		m_pBtnQiYu->SetShow(false);
		m_pBtnQiYu->SetEnable(false);
		return;
	}
	int iQiYu = g_OtherData.GetQiYuState();	
	if (iQiYu == 0)
	{
		m_pBtnQiYu->SetShow(false);
		m_pBtnQiYu->SetEnable(false);

		if (g_OtherData.GetQiYuFrame() >= 41)
		{
			if (g_OtherData.GetQiYuState() != 2)
			{
				g_OtherData.SetQiYuState(1);
				m_pBtnQiYu->SetShow(true);
				m_pBtnQiYu->SetEnable(true);
			}
		}

		LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22403,EP_UI);
		if (pTex)
			g_pGfx->DrawTextureNL(m_iScreenX - 82 + g_OtherData.GetQiYuFrame()*2, m_iScreenY+247, pTex);

		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22425,EP_UI);
		if (pTex)
		{
			g_pGfx->SetRenderMode(RM_ADD2);
			g_pGfx->DrawTextureNL(m_iScreenX - 82 + g_OtherData.GetQiYuFrame()*2 + 1, m_iScreenY+247 + 1, pTex);
			g_pGfx->SetRenderMode();
		}

		g_OtherData.SetQiYuFrame(g_OtherData.GetQiYuFrame() + 1);
		
	}
	else if (iQiYu == 1)
	{
		if (g_OtherData.GetQiYuShow())
		{
			m_pBtnQiYu->SetShow(true);
			m_pBtnQiYu->SetEnable(true);
		}
		else
		{
			m_pBtnQiYu->SetShow(false);
			m_pBtnQiYu->SetEnable(false);
		}
	}
	else if (iQiYu == 2)
	{
		if (g_OtherData.GetQiYuShow())
		{
			m_pBtnQiYu->SetShow(true);
			m_pBtnQiYu->SetEnable(true);
		}
		else
		{
			m_pBtnQiYu->SetShow(false);
			m_pBtnQiYu->SetEnable(false);
		}
	}	
}

void CGameWnd::DrawNoticeIE()
{
	if (g_pGameData->GetNoticeIEURL().empty())
	{
		g_pGameData->SetNoticeIEState(0);
		m_pBtnNoticeIE->SetShow(false);
		m_pBtnNoticeIE->SetEnable(false);
		return;
	}


	m_pBtnNoticeIE->SetShow(true);
	m_pBtnNoticeIE->SetEnable(true);

	int iNoticeIEState = g_pGameData->GetNoticeIEState();
	if (iNoticeIEState == 1)
	{
		int posx = g_pGameData->GetNoticeFrame()*5;
		if (posx >= g_pGfx->GetWidth()/2 - 25)
		{			
			m_pBtnNoticeIE->Move(g_pGfx->GetWidth()/2 - 25, g_pGfx->GetHeight() - 190);

			g_pGameData->SetNoticeIEState(0);
		}
		else
		{
			m_pBtnNoticeIE->Move(posx, g_pGfx->GetHeight() - 190);
		}		

		g_pGameData->SetNoticeFrame(g_pGameData->GetNoticeFrame() + 1);
	}
}


void CGameWnd::Draw8DunProgress()
{
	//法师才有，
	if (SELF.GetCareer() == 1)
	{
		if (SELF.Get8DunMaxProgress() > 0)
		{
			int iWidth = g_pGfx->GetWidth();
			int iHeight = g_pGfx->GetHeight();

			LPTexture pTex = NULL;
			//强袭烈焰盾攻击激活
			if (SELF.Is8DunShan())
				pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17290,EP_UI);
			else
				pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,(114 - 95) * 2 + 17200,EP_UI);

			LPTexture tech = g_pTexMgr->GetTexImm(PACKAGE_INTERFACE,4,EP_UI);		//技能框
			LPTexture tiao = g_pTexMgr->GetTex(PACKAGE_INTERFACE,5,EP_UI);		//框
			LPTexture jindu = g_pTexMgr->GetTex(PACKAGE_INTERFACE,6,EP_UI);		//进度
			LPTexture point = g_pTexMgr->GetTex(PACKAGE_INTERFACE,7,EP_UI);		//亮点

			if (jindu &&tiao)
			{
				int ox = 0;
				int oy = 0;

				if(g_EutUiType == EUT_CLASSIC)
				{
					ox = iWidth / 2 + 60;
					oy = iHeight - 112;
				}
				else
				{
					ox = iWidth / 2 + 130;
					oy = iHeight - 112;
				}

				g_pGfx->DrawTextureNL(ox - 40,oy - 10,tech);
				g_pGfx->DrawTextureNL(ox - 37,oy - 7,pTex);
								
				g_pGfx->DrawTextureNL(ox,oy,tiao);

				int i8dunright = SELF.Get8DunCurProgress() * jindu->GetWidth() / SELF.Get8DunMaxProgress();
				if (SELF.Is8DunShan())
				{
					DWORD dwColor = (g_pGfx->GetFrameCount() / 10 % 2) == 0 ? -1 : 0x90FFFFFF;
					g_pGfx->DrawPartTexture(ox,oy,jindu,0,0,i8dunright,-1,dwColor);
				}
				else
					g_pGfx->DrawPartTexture(ox,oy,jindu,0,0,i8dunright,-1);

				if (point && SELF.Get8DunCurProgress() > 0 && SELF.Get8DunCurProgress() < SELF.Get8DunMaxProgress())
					g_pGfx->DrawTextureNL(ox + i8dunright - point->GetWidth()/2,oy - 2,point);
			}			
		}
	}	
}

