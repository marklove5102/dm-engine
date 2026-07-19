#include "ExternMessgeBoxWnd.h"
#include "GameData/NpcData.h"
#include "GameData/MsgBoxMgr.h"
#include "GameData/GameData.h"
#include "GameData/GameGlobal.h"
#include "GameData/DirtyWords.h"
#include "GameData/WooolStoreData.h"

INIT_WND_POSX(CExternMessgeBoxWnd,POS_AUTO,POS_AUTO)

CExternMessgeBoxWnd::CExternMessgeBoxWnd(void)
{
	m_type = sm_dwWindowType;
	m_iLineLen = 48;
	m_sMsg = NULL;
	m_pInfo = NULL;
	ZeroMemory(name,128);

	m_pOK = NULL;
	m_pCancel = NULL;
	m_pLeavWords = NULL;

	m_pUpButton_1 = NULL;
	m_pDownButton_1 = NULL;
	m_pScroll_1 = NULL;
	m_pUpButton_2 = NULL;
	m_pDownButton_2 = NULL;
	m_pScroll_2 = NULL;
	
	m_iPages = 1;
	
    
	if(m_type == 1)
	    m_iIndex = 1003;
	else if(m_type == 2)
		m_iIndex = 1001;
}

CExternMessgeBoxWnd::~CExternMessgeBoxWnd(void)
{
	SAFE_DELETE_ARRAY(m_sMsg);
}

void CExternMessgeBoxWnd::OnCreate()
{
	if(m_type == 1)
	{
		int iDisplayRows = 3;
		m_pInfo = new CCtrlMultiEdit(512,40,3,false);
		AddControl(m_pInfo);
		m_pInfo->Create(this,14,44,60,248,49);
		m_pInfo->SetColor(0xFFB48C5A);
		m_pInfo->SetFont(FONT_YAHEI, FONTSIZE_DEFAULT);

		m_pGetFlag = new CCtrlButton;
		AddControl(m_pGetFlag);
		m_pGetFlag->CreateEx(this,100,114,132,133,134);	
		m_pGetFlag->SetText("购买黄榜大旗",0xffe0b060,0xffff9434,0xff66370b,COLOR_BTN_DISABLE,14,DTF_BlackFrame);
		//m_pGetFlag->SetTextOff(8,7);

		//上下按钮和滚动条
		//m_pUpButton_1 = new CCtrlButton();
		//AddControl(m_pUpButton_1);
		//m_pUpButton_1->CreateEx(this,329,75,51,52,53);

		//m_pDownButton_1 = new CCtrlButton();
		//AddControl(m_pDownButton_1);
		//m_pDownButton_1->CreateEx(this,329,111,56,57,58);

		m_pScroll_1 = new CCtrlScroll();
		AddControl(m_pScroll_1);
		m_pScroll_1->CreateEx(this, 300, 52, 17, 53);	
		m_pInfo->SetScroll(m_pScroll_1);
		
		m_pLeavWords = new CCtrlMultiEdit(20480,40,6,false);
		AddControl(m_pLeavWords);
		m_pLeavWords->Create(this,14,45,150,230,64);
		m_pLeavWords->SetColor(0xFFB48C5A);
		m_pLeavWords->SetFont(FONT_YAHEI, FONTSIZE_DEFAULT);
		

		//设置数据
		string strTmp;
		for(int i = 0;i< g_NPC.GetLeavWords().size();i++)
			strTmp += g_NPC.GetLeavWords().at(i) + "\r\n";

		g_DirtyWords.ClearWords_All(strTmp);
		m_pLeavWords->SetText(strTmp.c_str(),FONT_YAHEI,FONTSIZE_DEFAULT,0,0,0,COLOR_TEXT_NORMAL);


		//m_pUpButton_2 = new CCtrlButton();
		//AddControl(m_pUpButton_2);
		//m_pUpButton_2->CreateEx(this,329,153,51,52,53);

		//m_pDownButton_2 = new CCtrlButton();
		//AddControl(m_pDownButton_2);
		//m_pDownButton_2->CreateEx(this,329,228,56,57,58);

		m_pScroll_2 = new CCtrlScroll();
		AddControl(m_pScroll_2);
		m_pScroll_2->CreateEx(this, 300, 137, 17, 89);

		m_pScroll_2->SetRange(m_pLeavWords->GetLines() - m_pLeavWords->GetDisRows() );
		m_pScroll_2->SetPos(m_pLeavWords->GetDisLine());
		m_pLeavWords->SetFocus();

		m_pLeavWords->SetScroll(m_pScroll_2);

		//留言按钮
		m_pOK = new CCtrlButton();
		if(m_pOK)
		{
			AddControl(m_pOK);
			m_pOK->CreateEx(this,75,235,90,91,92);
			m_pOK->SetText("我要留言",0xFFF0AF64 ,0xFFFAC896, 0xFF5AA0A0, 0xFF646464, FONTSIZE_MIDDLE, 0, FONT_YAHEI);
		}
		//离开按钮
		m_pCancel = new CCtrlButton();
		if(m_pCancel)
		{
			AddControl(m_pCancel);
			m_pCancel->CreateEx(this, 196,235,90,91,92);
			m_pCancel->SetText("离  开", 0xFFF0AF64 ,0xFFFAC896, 0xFF5AA0A0, 0xFF646464, FONTSIZE_MIDDLE, 0, FONT_YAHEI);
		}
	}
	else if(m_type == 2)
	{
		m_pLeavWords = new CCtrlMultiEdit(10240,40,6,false);
		AddControl(m_pLeavWords);
		m_pLeavWords->Create(this,16,56,120,248,87);
		//设置数据
		string strTmp;
		for(int i = 0;i< g_NPC.GetLeavWords().size();i++)
			strTmp += g_NPC.GetLeavWords().at(i) + "\r\n";
		m_pLeavWords->SetText(strTmp.c_str()),FONT_YAHEI,FONTSIZE_DEFAULT,0,0,0,COLOR_TEXT_NORMAL;

		m_pUpButton_2 = new CCtrlButton();
		AddControl(m_pUpButton_2);
		m_pUpButton_2->CreateEx(this,309,120,51,52,53);

		m_pDownButton_2 = new CCtrlButton();
		AddControl(m_pDownButton_2);
		m_pDownButton_2->CreateEx(this,309,196,56,57,58);

		m_pScroll_2 = new CCtrlScroll();
		AddControl(m_pScroll_2);
		m_pScroll_2->Create(this, 309, 138, 17, 56, 54);

		m_pScroll_2->SetRange(m_pLeavWords->GetLines() - m_pLeavWords->GetDisRows() );
		m_pScroll_2->SetPos(m_pLeavWords->GetDisLine());
		m_pLeavWords->SetFocus();

		//留言按钮
		m_pOK = new CCtrlButton();
		if(m_pOK)
		{
			AddControl(m_pOK);
			m_pOK->CreateEx(this,95,219,1008,1009,1010);
		}
		//离开按钮
		m_pCancel = new CCtrlButton();
		if(m_pCancel)
		{
			AddControl(m_pCancel);
			m_pCancel->CreateEx(this, 212,219,1011,1012,1013);
		}
	}
}

void CExternMessgeBoxWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	m_pInfo->SetDisLine(m_pScroll_1->GetPos());
	m_pLeavWords->SetDisLine(m_pScroll_2->GetPos());

	if(m_type == 1)
	{
		if(strcmp(name,"") == 0)
			g_pFont->DrawText(m_iScreenX + 38,m_iScreenY + 30,"系统公告",0xFFB46428, FONT_YAHEI, FONTSIZE_MIDDLE);
		else
		{
			g_pFont->DrawText(m_iScreenX + 52,m_iScreenY + 28,"英雄",0xFFFFFFFF, FONT_YAHEI);
			g_pFont->DrawText(m_iScreenX + 76,m_iScreenY +28,name,0xFF00FF00, FONT_YAHEI,FONTSIZE_DEFAULT,DTF_UnderLine);
			g_pFont->DrawText(m_iScreenX + 76+strlen(name)*6,m_iScreenY + 28,"留言",0xFFFFFFFF, FONT_YAHEI);
			g_pFont->DrawText(m_iScreenX + 176,m_iScreenY + 19,"此处为非官方留言区",0xFF00FF00, FONT_YAHEI);
			g_pFont->DrawText(m_iScreenX + 206,m_iScreenY + 35,"谨防受骗",0xFFFF0000, FONT_YAHEI);

		}
		g_pFont->DrawText(m_iScreenX + 38,m_iScreenY + 116,"留言板",0xFFB46428, FONT_YAHEI, FONTSIZE_MIDDLE);
	}
	if(m_type == 2)
		DrawText();
}

bool CExternMessgeBoxWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	WORD w1 = HIWORD(dwData);
	WORD w2 = LOWORD(dwData);
	int x = (short)w2;
	int y = (short)w1;

	if(dwMsg == MSG_CTRL_EXTERN_MESSAGEBOX_WND)
	{
		ShowText((const char*)pControl);
		return true;
	}
	if( MSG_INPUT_LEFTBT_UP == dwMsg )
	{
		if(m_type == 1)
		{
			if(IsInNameArea(x,y))
			{
				std::string str;
				str.assign(name);
				str += " ";

				g_pControl->Msg(MSG_CTRL_INSERT_TEXT,1,(CControl *)str.c_str());
			}
		}
	}
	if( MSG_CTRL_BUTTON_CLICK == dwMsg)
	{
		if(pControl == m_pOK)
		{
			//先判断留言条件
			int maxSize = 20;
			if(m_type == 1)
			{
				maxSize = 50;
			}
			
            if(SELF.GetLevel()<32)
			{
				g_MsgBoxMgr.PopSimpleAlert("32级以上才可以留言。");
				return true;
			}
			if(g_NPC.GetLeavWords().size()>= maxSize)
			{
				g_MsgBoxMgr.PopSimpleAlert("32系统留言超过上限。");
				return true;
			}
			for(int i = 0;i< g_NPC.GetLeavWords().size();i++)
			{
				char strName[128] = {0};
				sprintf(strName,"玩家%s留言",SELF.GetName());
				if(g_NPC.GetLeavWords().at(i).find(strName) != std::string::npos)
				{
					g_MsgBoxMgr.PopSimpleAlert("每个玩家只能留言一次。");
					return true;
				}
			}

			//弹出留言框
			g_pControl->PopupWindow(MSG_CTRL_EX_LEAVWORDS_WND,OPER_CREATE,m_type);
			g_pControl->PopupWindow(MSG_CTRL_EXTERN_MESSAGEBOX_WND, OPER_CLOSE);	
			return true;
		}
		else if(pControl == m_pCancel)
		{
			//关闭当前窗口
			g_pControl->PopupWindow(MSG_CTRL_EXTERN_MESSAGEBOX_WND, OPER_CLOSE);
			return true;
		}
		else if(pControl == m_pUpButton_1)
		{
			if(m_type == 1)
			{
				m_pScroll_1->GoDown();
			}
		}
		else if(pControl == m_pDownButton_1)
		{
			if(m_type == 1)
			{
				m_pScroll_1->GoUp();
			}
		}
		else if(pControl == m_pUpButton_2)
		{
			m_pScroll_2->GoDown();
		}
		else if(pControl == m_pDownButton_2)
		{
			m_pScroll_2->GoUp();
		}	
		else if(pControl == m_pGetFlag)
		{
			g_WooolStoreMgr.SetQuickBuyItem("黄榜大旗(3天)");
			CQuickBuyData &QuickBuyData = g_WooolStoreMgr.GetQuickBuyData();

			if(QuickBuyData.pItem)
			{
				char str[256] = "";
				sprintf(str,"您需要马上购买黄榜大旗吗？单价%d元宝",QuickBuyData.pItem->iPrice);
				QuickBuyData.strMsg = str;
				QuickBuyData.iType = 1;
				QuickBuyData.iUseAfterBuyType = 0;					
				g_pControl->PopupWindow(MSG_CTRL_QUICKBUY_WND,OPER_CREATE);						
			}

			return true;
		}
	}
	else if(MSG_INPUT_LEFTBT_DCLICK == dwMsg)
	{
		if(pControl == m_pLeavWords)
		{
			int i = m_pLeavWords->GetCursorLine(); 
			char strTemp[256]={0};
			m_pLeavWords->GetLineText(strTemp,i);
			char *p1 = strstr(strTemp,"玩家");
			char *p2 = strstr(strTemp,"留言");

			if(p1 != NULL && p2 != NULL && (int)(p2-p1-4)>0 )
			{
				std::string str;
				str.append(p1+4,(int)(p2-p1-4));
				str += " ";

				g_pControl->Msg(MSG_CTRL_INSERT_TEXT,1,(CControl *)str.c_str());
			}
		}
	}

	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CExternMessgeBoxWnd::DrawText(void)
{
	if(m_sMsg == NULL)
		return;

	VString VStr;
	CutByUnicode(m_sMsg,VStr,m_iLineLen);

	for(int ii=0;ii < VStr.size();ii++)
	{
		g_pFont->DrawText(m_iScreenX+44,m_iScreenY+54+16*ii,VStr.at(ii).c_str(),0xFFFFFFFF, FONT_YAHEI);
	}
}

void CExternMessgeBoxWnd::ShowText(const char * sMsg)
{
	if(sMsg == NULL) return;
	int n = (int)strlen(sMsg);
	SAFE_DELETE_ARRAY(m_sMsg);
	m_sMsg = new char[n + 4];
	strcpy(m_sMsg,sMsg);

	if(m_type == 1)
	{
		if(m_pInfo)
		{
			char * temp = strstr(m_sMsg,":\\");
			if(temp==NULL)
			{
				m_pInfo->SetText(m_sMsg,FONT_YAHEI,FONTSIZE_DEFAULT,0,0,0,COLOR_TEXT_NORMAL);
			}
			else
			{
				memcpy(name,m_sMsg,temp - m_sMsg);
				string strContent = temp+2;
				g_DirtyWords.ClearWords_All(strContent);
				m_pInfo->SetText(strContent.c_str());
				m_pInfo->SetFont(FONT_YAHEI);
			}
		}

		m_pScroll_1->SetRange(m_pInfo->GetLines() - m_pInfo->GetDisRows() );
		m_pScroll_1->SetPos(m_pInfo->GetDisLine());
	}
}

void CExternMessgeBoxWnd::SetLineLen(int iLen)
{
	if(iLen >128)
		m_iLineLen = 128;
	else
        m_iLineLen = iLen;
}

bool CExternMessgeBoxWnd::IsInNameArea(int iX,int iY)
{
	if(strcmp(name,"") == 0)
		return false;
	int x = iX- (m_iScreenX + 106);
	int y = iY- (m_iScreenY+60);

	if (x>=0 && x<=int(strlen(name)*6) && y>=0 && y<=12)
		return true;
	return false;
}

bool CExternMessgeBoxWnd::OnWheel(int iWheel)
{
	if(m_type == 1 || m_type == 2)
	{
		return m_pScroll_2->OnWheel(iWheel);
	}

	return CCtrlWindowX::OnWheel(iWheel);
}
