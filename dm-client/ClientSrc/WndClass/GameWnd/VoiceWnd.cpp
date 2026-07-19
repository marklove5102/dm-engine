#include "VoiceWnd.h"
#include "GameClient/VoiceAdapter.h"
#include "GameData/MsgBoxMgr.h"
#include "GameControl/GameControl.h"
#include "GameData/TalkMgr.h"
#include "GameClient/WidgetManager.h"
#include "GameClient/SDOAInterface.h"

#ifdef _CHAT

INIT_WND_POSX(CVoiceWnd,POS_VARIABLE,POS_VARIABLE)

CVoiceWnd::CVoiceWnd(void)
{
	m_iType = sm_dwWindowType;//type = 0,公共大厅，type = 1,行会大厅

	m_dwClickReturnButtonTime = 0;
	m_pCreatButton = NULL;
	m_pEnterButton = NULL;
	m_pSetupButton = NULL;
	//m_pCharID = NULL;

	m_iIndex  = 13989;

	m_pGrid   = NULL;
	m_iOffX   = m_iOffY = 0;
	m_iAlignType = XAL_RIGHT;

	PostMessage(g_hWnd,MSG_IGW_WIDGET,APPID_VOICE_CHAT,HOSTAPP_OPEN);


	m_iVersion = 2;
	AddTabPage(0,0,MAKELONG(13989,PACKAGE_INTERFACE),35,36,120,121,122,123,"公共大厅",NULL,false,0,0,0,FONT_YAHEI,FONTSIZE_SMALL);
	AddTabPage(0,0,MAKELONG(13989,PACKAGE_INTERFACE),107,36,120,121,122,123,"行会大厅",NULL,false,0,0,1,FONT_YAHEI,FONTSIZE_SMALL);
	if (g_VoiceAdapter.IsInRoom())
	{
		AddTabPage(0,0,MAKELONG(13990,PACKAGE_INTERFACE),180,36,145,146,147,148,g_VoiceAdapter.GetCurRoomName(),NULL,false,0,0,1,FONT_YAHEI,FONTSIZE_SMALL);
	}

	m_TabPage.iCurPage = m_iType;

}

CVoiceWnd::~CVoiceWnd(void)
{

}

void CVoiceWnd::OnClickCloseButton()
{
	PostMessage(g_hWnd,MSG_IGW_WIDGET,APPID_VOICE_CHAT,HOSTAPP_CLOSE);
	CCtrlWindowX::OnClickCloseButton();
}

bool CVoiceWnd::Create(CControl* pParent,int iCurPage)
{
	if(g_VoiceAdapter.GetPort() == 0) 
	{
		if (!g_VoiceAdapter.IsWaitServer())
		{
			g_pGameControl->SEND_Voice_Request_Server_Addr();
			g_VoiceAdapter.SetWaitServer(true);
		}

		return false;
	}

	return CCtrlWindowX::Create(pParent,iCurPage);
}

void CVoiceWnd::OnCreate()
{
	CCtrlWindowX::OnCreate();

	m_iType = m_TabPage.iCurPage;

	if(!g_VoiceAdapter.IsConnected())
	{
		g_VoiceAdapter.Init();
	}
	//if (g_VoiceAdapter.GetConnectStatus())
	//{
	//	g_VoiceAdapter.GetRoomList();
	//}

	SetCloseButton(325,4,80);

	//m_pCharID = new CCtrlEdit;
	//AddControl(m_pCharID);
	//m_pCharID->Create(this,FONTSIZE_DEFAULT,0xFFFFFFFF,318,234);
	//m_pCharID->SetMaxLength(14);

	m_pCreatButton = new CCtrlButton();
	AddControl(m_pCreatButton);
	m_pCreatButton->CreateEx(this, 272,353, 95, 96, 97, 98);
	m_pCreatButton->SetText("创建频道", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);

	m_pSearchButton = new CCtrlButton();
	AddControl(m_pSearchButton);
	m_pSearchButton->SetEnable(true);
	m_pSearchButton->CreateEx(this, 116,353, 95, 96, 97, 98);
	m_pSearchButton->SetText("语聊搜索", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);

	m_pReturnButton = new CCtrlButton();
	AddControl(m_pReturnButton);
	m_pReturnButton->CreateEx(this, 14,353, 132, 133, 134, 135);
	m_pReturnButton->SetText("返回本服频道", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);

	m_pSetupButton = new CCtrlButton();
	AddControl(m_pSetupButton);
	m_pSetupButton->CreateEx(this, 194,353, 95, 96, 97, 98);
	m_pSetupButton->SetText("语音设置", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);

	m_pEnterButton = new CCtrlButton();
	AddControl(m_pEnterButton);
	m_pEnterButton->SetEnable(false);
	m_pEnterButton->CreateEx(this, 156,322, 95, 96, 97, 98);
	m_pEnterButton->SetText("进入频道", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);

	m_pGrid = new CCtrlGrid();
	AddControl(m_pGrid);
	m_pGrid->Create(this,48,105,335,313,10,21.0f);
	m_pGrid->AddScrollEx(271,0,16,208);
	m_pGrid->SetFont(FONT_YAHEI,FONTSIZE_SMALL);
	m_pGrid->SetDrawOffXY(0,2);

	m_pGrid->ClearColumn();
	m_pGrid->PushColumn(150);
	m_pGrid->PushColumn(80);
	m_pGrid->PushColumn(43);

}

void CVoiceWnd::Draw()
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	g_pFont->DrawText(m_iScreenX + 181,m_iScreenY + 11,"语音聊天",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG,DTF_Center);
	g_pFont->DrawText(m_iScreenX + 70,m_iScreenY + 65,"语音频道",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,FONTSIZE_MIDDLE);
	g_pFont->DrawText(m_iScreenX + 190,m_iScreenY + 65,"语音人数",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,FONTSIZE_MIDDLE);
	g_pFont->DrawText(m_iScreenX + 261,m_iScreenY + 65,"语音状态",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,FONTSIZE_MIDDLE);

	m_pSetupButton->SetEnable(g_VoiceAdapter.IsInRoom());

	if(m_pGrid)
	{
		if(g_VoiceAdapter.IsWaiting())
			m_pEnterButton->SetEnable(false);
		else
			m_pEnterButton->SetEnable(m_pGrid->GetSelLine()>=0);


		RoomVector* pRoomList = NULL;
		if (m_iType == 0)
			pRoomList = &(g_VoiceAdapter.GetRoomArray());
		else
			pRoomList = &(g_VoiceAdapter.GetGuildRoomArray());

		int iRooms = (int)pRoomList->size();

		char tmp[256] = {0};
		int xx,yy;

		m_pGrid->SetTotalCount(iRooms);
		for(int i = 0;i < m_pGrid->GetLinesPerPage();i++)
		{
			int pos = i+m_pGrid->GetDisLine();
			if(pos >= iRooms)
				continue;

			IRoom* pRoom = pRoomList->at(pos);
			if(pRoom == NULL)
				continue;

			m_pGrid->DrawGrid(i,0,pRoom->GetRoomName());
			sprintf(tmp,"%d/%d",pRoom->GetUserCount(),pRoom->GetMaxUser());
			m_pGrid->DrawGrid(i,1,tmp);

			if(m_pGrid->GetGridXY(i,2,xx,yy))
			{
				LPTexture pTex = g_pTexMgr->GetTexImm(PACKAGE_INTERFACE,pRoom->HasPassword()?14028:14023,EP_UI);
				if(pTex)
				{
					g_pGfx->DrawTextureNL(xx,yy,pTex);
				}
			}
		}
	}

	//绘制频道名称
	if(g_VoiceAdapter.IsInRoom()) 
	{
		string strRoomName = g_VoiceAdapter.GetCurRoomName();
		if(strRoomName.length() > 16)
		{
			strRoomName = strRoomName.substr(0,12);
			strRoomName += "...";
		}

		g_pFont->DrawText(m_iScreenX + 200,m_iScreenY + 88,strRoomName.c_str(),COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL);
	}

	if (g_VoiceAdapter.GetGameAreaName() && g_VoiceAdapter.GetGameGroupName())
	{
		char tmp[255] = {0};
		sprintf(tmp,"%s/%s",g_VoiceAdapter.GetGameAreaName(),g_VoiceAdapter.GetGameGroupName());
		g_pFont->DrawText(m_iScreenX + 40,m_iScreenY + 88,tmp,COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_SMALL);
	}

}

bool CVoiceWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	if(dwMsg == MSG_CTRL_TABPAGE_TABCHANGE)
	{
		if (dwData == 1)
		{
			RoomVector& RoomList = g_VoiceAdapter.GetGuildRoomArray();
			if (RoomList.size() == 0)
			{
				g_TalkMgr.PushSysTalk("请先加入行会再进入行会语聊大厅！");
				return true;
			}
			else
			{
				return false;//切换页签
			}
		}
		else if (dwData == 2)
		{
			if(!g_VoiceAdapter.IsInRoom()) //不在房间里面
				return true;

			CloseWindow();
			g_pControl->PopupWindow(MSG_CTRL_VOICE_CHAT_WND,OPER_CREATE);
			return true;
		}

		return true;
	}
	else if(dwMsg == MSG_CTRL_BUTTON_CLICK)
	{
		//创建语聊窗口
		if(pControl == m_pCreatButton)
		{
			//发送创建语聊窗消息   
			g_pControl->PopupWindow(MSG_CTRL_VOICE_ADD_WND,OPER_CREATE,m_iType);
			return true;
		}
		else if(pControl == m_pEnterButton) //进入语聊频道
		{
			int iSelLine = m_pGrid->GetSelLine();
			g_VoiceAdapter.EnterRoom(iSelLine,NULL,m_iType);
			return true;
		}
		else if(pControl == m_pSetupButton) //设置选项
		{
			//弹出设置窗口  
			g_pControl->PopupWindow(MSG_CTRL_VOICE_CONFIG_WND,OPER_CREATE);
			return true;
		}
		else if (pControl == m_pSearchButton)
		{
			if (g_VoiceAdapter.IsInRoom())
			{
				g_MsgBoxMgr.PopSimpleAlert("你已经在一个语音频道中，要想登陆其他服务器，请先退出语音频道!");
				return true;
			}
			g_VoiceAdapter.SearchServer();
			m_pSearchButton->SetEnable(false);
			return true;
		}
		else if (pControl == m_pReturnButton)
		{
			if(GetTickCount() - m_dwClickReturnButtonTime < 2000)
			{
				return true;
			}
			m_dwClickReturnButtonTime = GetTickCount();

			if (g_VoiceAdapter.IsInRoom())
			{
				g_MsgBoxMgr.PopSimpleAlert("你已经在频道中，如要返回，请先退出频道！");
				return true;
			}
			if (g_VoiceAdapter.IsInMyGame())
			{
				g_MsgBoxMgr.PopSimpleAlert("你已经在本游戏频道中！");
				return true;
			}

			g_pGameControl->SEND_Voice_Request_Info(0,"",2);
			return true;
		}
	}
	else if(dwMsg == MSG_CTRL_GRID_DBCLICK)
	{
		int iSelLine = LOWORD(dwData);
		m_bClick = false;//防止后面弹出的模式框获得不到焦点
		g_VoiceAdapter.EnterRoom(iSelLine,NULL,m_iType);
		return true;
	}
	else if(dwMsg == MSG_CTRL_VOICE_WND)
	{
		if(dwData == 1)
		{
			m_pSearchButton->SetEnable(true);
		}
		return true;
	}


	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}


#endif