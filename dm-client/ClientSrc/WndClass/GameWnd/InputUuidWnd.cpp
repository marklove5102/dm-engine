#include "inputuuidwnd.h"
#include "GameControl/GameControl.h"
#include "GameData/GameData.h"
#include "Global/Interface/AudioInterface.h"
#include "GameData/MsgBoxMgr.h"
#include <time.h>

#define COLOR_BTN_NORMAL_SKB	0xFFB47D3D
#define COLOR_BTN_MOUSEON_SKB	0xFFFF6600
#define COLOR_BTN_PRESS_SKB		0xFF5AA0A0
#define COLOR_BTN_DISABLE_SKB	0xFF646464


INIT_WND_POSX(CInputUuidWnd,POS_VARIABLE,POS_VARIABLE)

CInputUuidWnd::CInputUuidWnd(void)
{
	m_pOKButton		= NULL;
	m_pEdit			= NULL;
	m_iLineLen 		= 22;
	m_sMsg			= NULL;
	m_pCurEdit		= NULL;
	m_iType			= sm_dwWindowType;
	m_iFocusPos	    = 0;
	//m_bSmallMatrix  = false;
	m_iOpenPlace    = 0;

// 	//离线托管后上线时自己的状态已经是接管过来的托管过来的状态了,如果要输入密保,应该能输入
// 	if (g_TrusteeshipData.IsTrusteeship() && !g_TrusteeshipData.IsCaptain())
// 	{
// 		m_bModel = true;
// 	}

	for(int i = 0;i< 10;i++)
	{
		m_aiDitital[i] = i;
	}

	for(int i = 0;i < 3;i++)
	{
		m_aiRowNum[i] = m_aiColNum[i] = 0;
	}

	m_iControlMode = CTRL_MODE_WINDOW;

	if(m_iType == 0)
	{
		m_iIndex = 14100;
		m_iOffX = (g_pControl->GetWidth() - 210)/2;
	    m_iOffY = (g_pControl->GetHeight() - 306)/2;
	}	    
	else if(m_iType == 1)	//大安全卡
	{
		m_iIndex = 15009;
		m_iOffX = (g_pControl->GetWidth() - 400)/2;
	    m_iOffY = (g_pControl->GetHeight() - 450)/2;
	}
	else if(m_iType == 2)	//小安全卡
	{		
		m_iIndex = 20958;
		m_iOffX = (g_pControl->GetWidth() - 452)/2;
		m_iOffY = (g_pControl->GetHeight() - 464)/2;
		//m_bSmallMatrix = true;
	}
        
	m_iPages = 1;
	
	m_iAlignType = XAL_TOPLEFT;	
}

CInputUuidWnd::~CInputUuidWnd(void)
{
	SAFE_DELETE(m_sMsg);
}

void CInputUuidWnd::OnCreate()
{
	if(m_iType == 0)
	{
		m_pOKButton = new CCtrlButton();
		m_pOKButton->CreateEx(this,82,215,14101,14102,14103);
		AddControl(m_pOKButton);
		m_pOKButton->SetText("确 定", COLOR_BTN_NORMAL, COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE);

		m_pEdit = new CCtrlEdit();
		m_pEdit->Create(this,FONTSIZE_DEFAULT,0xFFFFFFFF,52,89,100,20);
		m_pEdit->SetMaxLength(10);	
		m_pEdit->SetFont(FONT_YAHEI);
		m_pEdit->SetColor(COLOR_TEXT_NORMAL);
		AddControl(m_pEdit);

		//计算数字按钮的位置
		int iTemp =0,i1 = 0,i2 = 0;
		for(int i = 0;i< 6;i++)
		{
			i1 = rand()%10;
			i2 = rand()%10;
			iTemp = m_aiDitital[i1];
			m_aiDitital[i1] = m_aiDitital[i2];
			m_aiDitital[i2] = iTemp;
		}
		//下面开始计算位置
		int iPic = 0,iX=0,iY=0;
		char buff[3];
		for(int i = 1; i< 10; i++)
		{
			iPic = 1652;
			m_aDigitalBtn[i] = new CCtrlButton();
			AddControl(m_aDigitalBtn[i]);
			iTemp = i%3;
			if(iTemp == 0) iTemp = 2;			
			else iTemp -= 1;
			iX = iTemp*35 + 46;
			iY = ((i - 1)/3)*30 + 125;
			itoa(m_aiDitital[i], buff, 10);
			m_aDigitalBtn[i]->CreateEx(this,iX,iY,iPic,iPic + 1,iPic+2);
			m_aDigitalBtn[i]->SetText(buff);
			m_aDigitalBtn[i]->SetTextOff(-3, 0);
		}
		//最后一个
		iPic = 1652;
		m_aDigitalBtn[0] = new CCtrlButton();
		iX = 46;
		iY = 215;
		m_aDigitalBtn[0]->CreateEx(this,iX,iY,iPic,iPic + 1,iPic+2);
		AddControl(m_aDigitalBtn[0]);
		itoa(m_aiDitital[0], buff, 10);
		m_aDigitalBtn[0]->SetText(buff);
		m_aDigitalBtn[0]->SetTextOff(-3, 0);

		SetCloseButton(156,15, 66);
	}
	else if(m_iType == 1)
	{
		m_pOKButton = new CCtrlButton();
		m_pOKButton->CreateEx(this,121,414,59,60,61,62);
		AddControl(m_pOKButton);
		m_pOKButton->SetEnable(false);
		m_pOKButton->SetText("确 认");

		m_pCancelButton = new CCtrlButton();
		m_pCancelButton->CreateEx(this,252,414,59,60,61,62);
		AddControl(m_pCancelButton);
		m_pCancelButton->SetText("关  闭");

		m_pEdit = new CCtrlEdit();
		m_pEdit->Create(this,FONTSIZE_DEFAULT,COLOR_TEXT_MAIN_TITLE,379,181,27,21);
		m_pEdit->SetDigital(true);
		m_pEdit->SetMaxLength(2);
		AddControl(m_pEdit);

		m_pEdit1 = new CCtrlEdit();
		m_pEdit1->Create(this,FONTSIZE_DEFAULT,COLOR_TEXT_MAIN_TITLE,379,212,27,21);
		m_pEdit1->SetDigital(true);
		m_pEdit1->SetMaxLength(2);
		AddControl(m_pEdit1);

		m_pEdit2 = new CCtrlEdit();
		m_pEdit2->Create(this,FONTSIZE_DEFAULT,COLOR_TEXT_MAIN_TITLE,379,242,27,21);
		m_pEdit2->SetDigital(true);
		m_pEdit2->SetMaxLength(2);
		AddControl(m_pEdit2);

		//计算数字按钮的位置
		int iTemp =0,i1 = 0,i2 = 0;
		for(int i = 0;i< 6;i++)
		{
			i1 = rand()%10;
			i2 = rand()%10;
			iTemp = m_aiDitital[i1];
			m_aiDitital[i1] = m_aiDitital[i2];
			m_aiDitital[i2] = iTemp;
		}
		//下面开始计算位置
		int iPic = 0,iX=0,iY=0;
		for(int i = 0; i < 10; i++)
		{
			m_aDigitalBtn[i] = new CCtrlButton();
			AddControl(m_aDigitalBtn[i]);
			char c[2] = "";
			c[0] = '0' + m_aiDitital[i];			
			m_aDigitalBtn[i]->SetText(c,COLOR_BTN_NORMAL_SKB,COLOR_BTN_MOUSEON_SKB,COLOR_BTN_PRESS_SKB,COLOR_BTN_DISABLE_SKB,12,DTF_BlackFrame,FONT_YAHEI,false,-3,-1);
			m_aDigitalBtn[i]->SetFont(FONT_YAHEI);

			iX = (i % 5) * 25 + 272;
			iY = 309;
			if(i >= 5)
			{
				iY += 25;
			}

			m_aDigitalBtn[i]->CreateEx(this,iX,iY,1652,1653,1654);
		}
		m_pCurEdit = m_pEdit;		
	}
	else if(m_iType == 2)
	{	
		m_pOKButton = new CCtrlButton();
		m_pOKButton->CreateEx(this,121,414,59,60,61,62);
		AddControl(m_pOKButton);
		m_pOKButton->SetEnable(false);
		m_pOKButton->SetText("确 认");

		m_pCancelButton = new CCtrlButton();
		m_pCancelButton->CreateEx(this,252,414,59,60,61,62);
		AddControl(m_pCancelButton);
		m_pCancelButton->SetText("关  闭");

		m_pEdit = new CCtrlEdit();
		m_pEdit->Create(this,FONTSIZE_DEFAULT,COLOR_TEXT_MAIN_TITLE,356,186,27,21);
		m_pEdit->SetDigital(true);
		m_pEdit->SetMaxLength(2);
		AddControl(m_pEdit);

		m_pEdit1 = new CCtrlEdit();
		m_pEdit1->Create(this,FONTSIZE_DEFAULT,COLOR_TEXT_MAIN_TITLE,356,217,27,21);
		m_pEdit1->SetDigital(true);
		m_pEdit1->SetMaxLength(2);
		AddControl(m_pEdit1);

		m_pEdit2 = new CCtrlEdit();
		m_pEdit2->Create(this,FONTSIZE_DEFAULT,COLOR_TEXT_MAIN_TITLE,356,247,27,21);
		m_pEdit2->SetDigital(true);
		m_pEdit2->SetMaxLength(2);
		AddControl(m_pEdit2);

		//计算数字按钮的位置
		int iTemp =0,i1 = 0,i2 = 0;
		for(int i = 0;i< 6;i++)
		{
			i1 = rand()%10;
			i2 = rand()%10;
			iTemp = m_aiDitital[i1];
			m_aiDitital[i1] = m_aiDitital[i2];
			m_aiDitital[i2] = iTemp;
		}
		//下面开始计算位置
		int iPic = 0,iX=0,iY=0;
		for(int i = 0; i < 10; i++)
		{
			m_aDigitalBtn[i] = new CCtrlButton();
			AddControl(m_aDigitalBtn[i]);
			char c[2] = "";
			c[0] = '0' + m_aiDitital[i];			
			m_aDigitalBtn[i]->SetText(c,COLOR_BTN_NORMAL_SKB,COLOR_BTN_MOUSEON_SKB,COLOR_BTN_PRESS_SKB,COLOR_BTN_DISABLE_SKB,12,DTF_BlackFrame,FONT_YAHEI,false,-3,-1);
			m_aDigitalBtn[i]->SetFont(FONT_YAHEI);

			iX = (i % 5) * 25 + 250;
			iY = 321;
			if(i >= 5)
			{
				iY += 25;
			}

			m_aDigitalBtn[i]->CreateEx(this,iX,iY,1652,1653,1654);
		}
		m_pCurEdit = m_pEdit;		
	}
}

void CInputUuidWnd::OnClickCloseButton()
{
	CloseWindow();
	g_pControl->Msg(MSG_CTRL_ENABLELOGINWND,0);
}

void CInputUuidWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	if(m_iType == 0)
	{		
		g_pFont->DrawText(48+m_iScreenX, 23+m_iScreenY, "请输入密宝", COLOR_TEXT_MAIN_TITLE, FONT_YAHEI, FONTSIZE_BIG);
	    DrawText();
	}
	else if(m_iType == 1 || m_iType == 2)
	{
		CControl *pFocusCtrl = g_pControl->GetFocusCtrl();
		if (pFocusCtrl == m_pEdit || pFocusCtrl == m_pEdit1 || pFocusCtrl == m_pEdit2)
		{
			m_pCurEdit = (CCtrlEdit *)pFocusCtrl;
		}

		if(strlen(m_pEdit->GetText()) !=2 || strlen(m_pEdit1->GetText()) != 2 || strlen(m_pEdit2->GetText()) != 2)
		{
			m_pOKButton->SetEnable(false);
		}
		else
		{
			m_pOKButton->SetEnable(true);
		}

		g_pFont->DrawText(m_iScreenX + 173,m_iScreenY + 23,"盛大安全卡验证",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG);

		g_pFont->DrawText(m_iScreenX + 60,m_iScreenY + 60,"安全提醒：如果连续5次输入错误，您的帐号在5分钟内将会被",COLOR_TEXT_SBU_TITLE,FONT_YAHEI);
		g_pFont->DrawText(m_iScreenX + 95,m_iScreenY + 80,"暂时锁定，请不要再次尝试输入，以防帐号被盗。",COLOR_TEXT_SBU_TITLE,FONT_YAHEI);

		if(m_iType == 1)
		{
			g_pFont->DrawText(m_iScreenX + 42,m_iScreenY + 118,"根据下图提示找到安全卡上对应坐标位置",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI);
			
			g_pFont->DrawText(m_iScreenX + 265,m_iScreenY + 144,"请依次输入盛大安全卡3个",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI);
			g_pFont->DrawText(m_iScreenX + 265,m_iScreenY + 164,"坐标位置上对应的2位数字",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI);

			g_pFont->DrawText(m_iScreenX + 254 + 23,m_iScreenY + 186,"坐标1",COLOR_TEXT_SBU_TITLE,FONT_YAHEI);
			g_pFont->DrawText(m_iScreenX + 314 + 23,m_iScreenY + 186,m_PosArray[0],COLOR_TEXT_SBU_TITLE,FONT_YAHEI);

			g_pFont->DrawText(m_iScreenX + 254 + 23,m_iScreenY + 215,"坐标2",COLOR_TEXT_SBU_TITLE,FONT_YAHEI);
			g_pFont->DrawText(m_iScreenX + 314 + 23,m_iScreenY + 215,m_PosArray[1],COLOR_TEXT_SBU_TITLE,FONT_YAHEI);

			g_pFont->DrawText(m_iScreenX + 254 + 23,m_iScreenY + 245,"坐标3",COLOR_TEXT_SBU_TITLE,FONT_YAHEI);
			g_pFont->DrawText(m_iScreenX + 314 + 23,m_iScreenY + 245,m_PosArray[2],COLOR_TEXT_SBU_TITLE,FONT_YAHEI);

			g_pFont->DrawText(m_iScreenX + 260 + 23,m_iScreenY + 288,"点击输入数字密码：",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI);

			LPTexture pRowTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15040,EP_UI);
			LPTexture pColTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15041,EP_UI);
			LPTexture pSelectTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15046,EP_UI);		
			int iX = m_iScreenX + 60 + m_aiColNum[m_iFocusPos] * 25;
			int iY = m_iScreenY + 151 + m_aiRowNum[m_iFocusPos] * 16;		
			g_pGfx->DrawTextureNL(m_iScreenX + 60,iY,pRowTex);
			g_pGfx->DrawTextureNL(iX,m_iScreenY + 151,pColTex);
			g_pGfx->DrawTextureNL(iX,iY,pSelectTex);

			char c[4] = "";

			for(int i = 0;i < 8;i++)
			{
				c[0] = 'A' + i;
				g_pFont->DrawText(m_iScreenX + 68 + 25 * i,m_iScreenY + 134,c,COLOR_TEXT_SBU_TITLE,FONT_YAHEI,FONTSIZE_MIDDLE);
			}

			for(int i = 0;i < 9;i++)
			{
				sprintf(c,"%d",i + 1);
				g_pFont->DrawText(m_iScreenX + 45,m_iScreenY + 150 + 16 * i ,c,COLOR_TEXT_SBU_TITLE,FONT_YAHEI,FONTSIZE_MIDDLE);
			}

			for(int i = 9;i < 15;i++)
			{
				sprintf(c,"%d",i + 1);
				g_pFont->DrawText(m_iScreenX + 41,m_iScreenY + 150 + 16 * i ,c,COLOR_TEXT_SBU_TITLE,FONT_YAHEI,FONTSIZE_MIDDLE);
			}

			for(int i = 0;i < 8;i++)
			{
				for(int j = 0;j < 9;j++)
				{
					sprintf(c,"%c%d",'A' + i,j + 1);
					g_pFont->DrawText(m_iScreenX + 64 + 25 * i,m_iScreenY + 150 + 16 * j ,c,0xff000000,FONT_YAHEI,FONTSIZE_MIDDLE);
				}

				for(int j = 9;j < 15;j++)
				{
					sprintf(c,"%c%d",'A' + i,j + 1);
					g_pFont->DrawText(m_iScreenX + 61 + 25 * i,m_iScreenY + 150 + 16 * j ,c,0xff000000,FONT_YAHEI,FONTSIZE_MIDDLE);
				}
			}			
		}
		else if(m_iType == 2)
		{
			g_pFont->DrawText(m_iScreenX + 73,m_iScreenY + 146,"请根据下图提示找到您",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI);
			g_pFont->DrawText(m_iScreenX + 73,m_iScreenY + 166,"安全卡上对应坐标位置",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI);

			g_pFont->DrawText(m_iScreenX + 242,m_iScreenY + 146,"请依次输入盛大安全卡3个",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI);
			g_pFont->DrawText(m_iScreenX + 242,m_iScreenY + 166,"坐标位置上对应的2位数字",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI);

			g_pFont->DrawText(m_iScreenX + 254,m_iScreenY + 191,"坐标1",COLOR_TEXT_SBU_TITLE,FONT_YAHEI);
			g_pFont->DrawText(m_iScreenX + 314,m_iScreenY + 191,m_PosArray[0],COLOR_TEXT_SBU_TITLE,FONT_YAHEI);

			g_pFont->DrawText(m_iScreenX + 254,m_iScreenY + 220,"坐标2",COLOR_TEXT_SBU_TITLE,FONT_YAHEI);
			g_pFont->DrawText(m_iScreenX + 314,m_iScreenY + 220,m_PosArray[1],COLOR_TEXT_SBU_TITLE,FONT_YAHEI);

			g_pFont->DrawText(m_iScreenX + 254,m_iScreenY + 250,"坐标3",COLOR_TEXT_SBU_TITLE,FONT_YAHEI);
			g_pFont->DrawText(m_iScreenX + 314,m_iScreenY + 250,m_PosArray[2],COLOR_TEXT_SBU_TITLE,FONT_YAHEI);

			g_pFont->DrawText(m_iScreenX + 260,m_iScreenY + 290,"点击输入数字密码：",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI);

			LPTexture pRowTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,20959,EP_UI);
			LPTexture pColTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,20960,EP_UI);
			LPTexture pSelectTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,20961,EP_UI);

			int iX = m_iScreenX + 86 + m_aiColNum[m_iFocusPos] * 24;
			int iY = m_iScreenY + 209 + m_aiRowNum[m_iFocusPos] * 21;		
			g_pGfx->DrawTextureNL(m_iScreenX + 86,iY,pRowTex);
			g_pGfx->DrawTextureNL(iX,m_iScreenY + 209,pColTex);
			g_pGfx->DrawTextureNL(iX,iY,pSelectTex);

			char c[3] = "";

			for(int i = 0;i < 5;i++)
			{
				c[0] = 'A' + i;
				g_pFont->DrawText(m_iScreenX + 93 + 25 * i,m_iScreenY + 190 ,c,COLOR_TEXT_SBU_TITLE,FONT_YAHEI,FONTSIZE_MIDDLE);
			}

			for(int i = 0;i < 8;i++)
			{
				c[0] = '1' + i;
				g_pFont->DrawText(m_iScreenX + 70,m_iScreenY + 209 + 21 * i ,c,COLOR_TEXT_SBU_TITLE,FONT_YAHEI,FONTSIZE_MIDDLE);
			}

			for(int i = 0;i < 5;i++)
			{
				for(int j = 0;j < 8;j++)
				{
					c[0] = 'A' + i;
					c[1] = '1' + j;
					g_pFont->DrawText(m_iScreenX + 89 + 25 * i,m_iScreenY + 210 + 21 * j ,c,0xff000000,FONT_YAHEI,FONTSIZE_MIDDLE);
				}
			}				
		}
	}
}

bool CInputUuidWnd::Msg(DWORD dwMsg,DWORD dwData,CControl* pControl)
{
	switch (dwMsg)
	{
	case MSG_CTRL_EDIT_ENTRY:
		{
			SendDPWD();
			return true;
		}
	case MSG_CTRL_BUTTON_CLICK:
		{
			for(int i = 0;i<10;i++)
			{
				if(pControl == m_aDigitalBtn[i])
				{
					int iPos = m_aiDitital[i];
					char ctemp[10]={0};
					sprintf(ctemp,"%d",iPos);
                    if(m_iType == 0)
					    m_pEdit->InsertText(ctemp);
					else if(m_iType == 1 || m_iType == 2)
					{
						if (m_pCurEdit)
						{
							m_pCurEdit->InsertText(ctemp);
						}
					}
					return true;
				}
			}
			
			if(pControl == m_pOKButton)
			{
				SendDPWD();
				g_pAudio->Play(EAT_OTHER,3,g_pAudio->GetRand()++);
				return true;
			}

			if(pControl == m_pCancelButton)
			{
				CloseWindow();
				g_pControl->Msg(MSG_CTRL_ENABLELOGINWND,0);
				return true;
			}
		}
		break;
	case MSG_CTRL_INPUT_MIBAO_WND:
		{
			if(dwData == 1)
			{
				ShowText((const char *)pControl);
			}			
			else if (dwData == 3)
			{
				m_iOpenPlace = 1;
			}
			else if(dwData == 4)
			{
				if (int((__int64)pControl) > 0)
				{
					m_pEdit->SetMaxLength(int((__int64)pControl)); 
				}
			}
			
			return true;
		}
	case MSG_CTRL_ON_SET_FOCUS:
		{
			if(m_iType == 1 || m_iType == 2)
			{
				if(pControl == m_pEdit)
				{
					m_iFocusPos = 0;
				}
				else if(pControl == m_pEdit1)
				{
					m_iFocusPos = 1;
				}
				else if(pControl == m_pEdit2)
				{
					m_iFocusPos = 2;
				}
			}	

		    return true;
		}
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

bool CInputUuidWnd::OnLeftButtonDown(int iX,int iY)
{
	if(m_iType == 1)
	{
		if(iX > 265 && iX < 372 && iY > 183 && iY < 200)
		{
			m_pCurEdit = m_pEdit;
			m_pCurEdit->SetFocus();
		}
		else if(iX > 265 && iX < 372 && iY > 213 && iY < 231)
		{
			m_pCurEdit = m_pEdit1;
			m_pCurEdit->SetFocus();
		}
		else if(iX > 265 && iX < 372 && iY > 242 && iY < 261)
		{
			m_pCurEdit = m_pEdit2;
			m_pCurEdit->SetFocus();
		}
	}
	else if(m_iType == 2)
	{
		if(iX > 243 && iX < 350 && iY > 188 && iY < 206)
		{
			m_pCurEdit = m_pEdit;
			m_pCurEdit->SetFocus();
		}
		else if(iX > 243 && iX < 350 && iY > 218 && iY < 236)
		{
			m_pCurEdit = m_pEdit1;
			m_pCurEdit->SetFocus();
		}
		else if(iX > 243 && iX < 350 && iY > 248 && iY < 266)
		{
			m_pCurEdit = m_pEdit2;
			m_pCurEdit->SetFocus();
		}
	}

	return CCtrlWindowX::OnLeftButtonDown(iX,iY);
}

void CInputUuidWnd::GetEditText(char * sEditText)
{
	if(m_iType == 0)
	{
		if(m_pEdit == NULL || sEditText == NULL) return;		
		strcpy(sEditText,m_pEdit->GetText());
	}
	else if(m_iType == 1 || m_iType == 2)
	{
		sprintf(sEditText,"%s:%s|%s:%s|%s:%s",m_PosArray[0],m_pEdit->GetText(),m_PosArray[1],m_pEdit1->GetText(),m_PosArray[2],m_pEdit2->GetText());
	}
}

bool CInputUuidWnd::SetFocus(void)
{
	if(m_iType == 0)
	{
		if(m_pEdit) return m_pEdit->SetFocus();		
	}
	else if(m_iType == 1 || m_iType == 2)
	{
		if(m_pCurEdit) return m_pCurEdit->SetFocus();		
	}
	return CControl::SetFocus();
}

void CInputUuidWnd::ShowText(const char * sMsg)
{
	if(m_iType == 0)
	{
		if(sMsg == NULL) return;
		int n = (int)strlen(sMsg);
		SAFE_DELETE_ARRAY(m_sMsg);
		m_sMsg = new char[n + 4];
		strcpy(m_sMsg,sMsg);
	}
	else if(m_iType == 1 || m_iType == 2)
	{
		if(sMsg == NULL) return;
		int iStart = 0;

        string strTmp = StringUtil::toStr(sMsg, iStart);
		strcpy(m_PosArray[0], strTmp.c_str());
		strTmp = StringUtil::toStr(sMsg, iStart);
		strcpy(m_PosArray[1], strTmp.c_str());
		strTmp = StringUtil::toStr(sMsg, iStart);
		strcpy(m_PosArray[2], strTmp.c_str());

		char col;
		int row;

		sscanf(m_PosArray[0],"%c%d",&col,&row);
		m_aiColNum[0] = col - 'A';
		m_aiRowNum[0] = row - 1;

		sscanf(m_PosArray[1],"%c%d",&col,&row);
		m_aiColNum[1] = col - 'A';
		m_aiRowNum[1] = row - 1;

		sscanf(m_PosArray[2],"%c%d",&col,&row);
		m_aiColNum[2] = col - 'A';
		m_aiRowNum[2] = row - 1;
	}	
}

void CInputUuidWnd::DrawText(void)
{
	if(m_sMsg == NULL)
		return;
	int StrLen = (int)strlen(m_sMsg);
	char *p = m_sMsg;
	char Tmp[128];
	int ascii = 0,Chars = 0;
	int iLine = 0;

	while(p<(m_sMsg+StrLen))
	{
		ascii=0;
		Chars=0;
		memset(Tmp,0,128);
		//计算本行的字符数
		int LeftLen = (int)(StrLen-(p-m_sMsg));     //剩余的字符长度
		int CurLen;
		if( LeftLen > m_iLineLen) 
			CurLen = m_iLineLen;
		else 
			CurLen = LeftLen;
		while( Chars < CurLen && p < m_sMsg + StrLen)
		{
			//计算英文字符数量(0=双数 1=单数)
			Tmp[Chars]=*p;//正常字符copy			
			p++;
			Chars++;
			if((unsigned char)Tmp[Chars-1]==10)                    //如果是回车键就结束换行
			{
				Tmp[Chars-1]=0;
				goto _NEXT;
			}			
			if( (unsigned char)Tmp[Chars-1]>128 )		         //同样的中英文处理
			{				
				ascii=1-ascii;
			}
		}
		//如果Tmp[chars-1]是汉字的最后半个.
		if( ascii == 1 && p< m_sMsg + StrLen )
		{
			p--;					//留到下一行
			Tmp[Chars-1]=0;         //去掉最后一个
		}
_NEXT:
		g_pFont->DrawText(m_iScreenX + 32,m_iScreenY + 53 + 16 * iLine,Tmp,0xFFFFFF66, FONT_YAHEI);
		iLine++;
	}
		
}

void CInputUuidWnd::SendDPWD(void)
{
	char str[1024] = "";
	if(m_iType == 0)
	{
		GetEditText(str);
		if(strlen(str) == 0)
			return;

		//g_pGameControl->SEND_Input_DPWD(str);

		g_pControl->PopupWindow(MSG_CTRL_INPUT_MIBAO_LATER_WND,OPER_CLOSE);				
	}
	else if(m_iType == 1 || m_iType == 2)
	{
		if(strlen(m_pEdit->GetText()) !=2 || strlen(m_pEdit1->GetText()) != 2 || strlen(m_pEdit2->GetText()) != 2)
		{
			g_MsgBoxMgr.PopSimpleAlert("密码输入不完全，请检查。");
			return;
		}

		GetEditText(str);
		//g_pGameControl->SEND_Input_DPWD(str,1);
	}

	if (m_iOpenPlace == 0)
	{
		g_pGameControl->SEND_Challenge_Ack(str);
	}
	else
	{
		g_pGameControl->SEND_VerifyStoreEkey_Ack(str);
	}


	CloseWindow();
}
