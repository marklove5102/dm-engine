#include "BoothNamePrice.h"
#include "GameData/GameData.h"
#include "BaseClass/Control/ParserTip.h"
#include "GameControl/GameControl.h"
#include "GameData/MsgBoxMgr.h"
#include "GameData/TalkMgr.h"
#include <string>


INIT_WND_POSX(CBoothNameWnd,POS_VARIABLE,POS_VARIABLE)

//摊位名字
CBoothNameWnd::CBoothNameWnd()
{
	m_iIndex = 40;
	m_iPages = 1;
	
	m_iAlignType = XAL_TOPLEFT;

	if(g_pGfx->GetWidth() > 800)
	{
		m_iOffX = 320;
		m_iOffY = 320;
	}
	else
	{
		m_iOffX = 210;
		m_iOffY = 250;
	}

}

CBoothNameWnd::~CBoothNameWnd()
{
}

void CBoothNameWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}
	g_pFont->DrawText(m_iScreenX+78,m_iScreenY+68,"输入摊位名字",COLOR_TEXT_NORMAL,FONT_YAHEI);
}

void CBoothNameWnd::OnCreate()
{
	m_pNameEdit = new CCtrlEdit();
	m_pNameEdit->CreateEx(this,155,66,155,18,0, 38,38,38,38);
	AddControl(m_pNameEdit);
	m_pNameEdit->SetMaxLength(52);
	m_pNameEdit->SetFocus();

	m_pNameEdit->SetText(g_BoothData.GetBoothName());
	m_pNameEdit->SetColor(COLOR_TEXT_NORMAL);
	m_pNameEdit->SetFont(FONT_YAHEI);
	m_pNameEdit->SetTextOff(8,0);

	//确定按钮
	m_pOKButton = new CCtrlButton();
	AddControl(m_pOKButton);
	m_pOKButton->CreateEx(this,100,125,90,91,92);
	m_pOKButton->SetText("确定", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE, 0, FONT_YAHEI);

	//取消按钮
	m_pCancelButton = new CCtrlButton();
	AddControl(m_pCancelButton);
	m_pCancelButton->CreateEx(this,220,125,90,91,92);
	m_pCancelButton->SetText("取消", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE, 0, FONT_YAHEI);
}

bool CBoothNameWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_BOOTH_NAME_FOCUS:
		{
			if (m_pNameEdit)
				m_pNameEdit->SetFocus();

			return true;
		}
	case MSG_CTRL_BUTTON_CLICK:
		if(pControl == m_pOKButton)
		{
			char temp[1024] = {0};
			strcpy(temp,m_pNameEdit->GetText());

			if (strlen(temp) ==0)
				return true;

			g_BoothData.SetBoothName(temp);

			CloseWindow();
			return true;
		}
		else if(pControl == m_pCancelButton)
		{
			CloseWindow();
			return true;
		}
	default:
		break;
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

bool CBoothNameWnd::SetFocus()
{
	return m_pNameEdit->SetFocus();
}


INIT_WND_POSX(CBoothPriceWnd,POS_AUTO,POS_AUTO)

//商品价格窗口
CBoothPriceWnd::CBoothPriceWnd()
{
    m_pOKButton = NULL;
	m_pCancelButton = NULL;
	m_iCurPage = 1;

	m_dwMsg  = sm_dwWindowType;

	m_iIndex = 4609;
	m_iPages = 1;
}

CBoothPriceWnd::~CBoothPriceWnd()
{
	
}

void CBoothPriceWnd::OnClickCloseButton()
{
	g_BoothData.ClearReadyGood(); //相对于取消
	CloseWindow();
}

void CBoothPriceWnd::SwitchPage(int i)
{
	if(i == m_iCurPage)
		return;

	if(m_dwMsg == MSG_CTRL_UPLOAD_ADD_GOOD && i == 0)
	{
		g_TalkMgr.PushSysTalk("拍卖物品，暂不开放设置金币价格");
		return;
	}

	SetPageTex(0,PACKAGE_INTERFACE,4608+i);

	m_iCurPage = i;
	m_pCountEdit->Clear();

	if(m_iCurPage == 0)
		m_pCountEdit->SetMaxLength(9);
	else
		m_pCountEdit->SetMaxLength(7);


	this->Msg(MSG_CTRL_BOOTH_PRICE_FOCUS,0,this);
}

void CBoothPriceWnd::OnCreate()
{
	m_pCountEdit = new CCtrlEdit;
	AddControl(m_pCountEdit);
	m_pCountEdit->CreateEx(this,100,74,159,18,0,39,39,39,39);
	m_pCountEdit->SetText("",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_DEFAULT,0,FONT_YAHEI);
	m_pCountEdit->SetDigital(true);
	if (m_iCurPage == 0)
		m_pCountEdit->SetMaxLength(9);
	else
		m_pCountEdit->SetMaxLength(7);

	//确定按钮
	m_pOKButton = new CCtrlButton();
	AddControl(m_pOKButton);
	m_pOKButton->CreateEx(this,78,123,90,91,92);
	m_pOKButton->SetText("确 定",COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE, 0, FONT_YAHEI);

	//取消按钮
	m_pCancelButton = new CCtrlButton();
	AddControl(m_pCancelButton);
	m_pCancelButton->CreateEx(this,168,123,90,91,92);
	m_pCancelButton->SetText("取 消",COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE, 0, FONT_YAHEI);

	SetCloseButton(288,1,80);
}

void CBoothPriceWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}
	g_pFont->DrawText(m_iScreenX+22,m_iScreenY+76,"输入物品价格",COLOR_TEXT_NORMAL, FONT_YAHEI);
	g_pFont->DrawText(122+m_iScreenX, 7+m_iScreenY, "输入价格", COLOR_TEXT_MAIN_TITLE, FONT_YAHEI, FONTSIZE_BIG);

	LPTexture pCurPageTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE, 4639,EP_UI);
	LPTexture pOtherPageTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE, 4638,EP_UI);

	if (m_iCurPage == 0)
	{
		g_pGfx->DrawTextureNL(m_iScreenX+14, m_iScreenY+31, pCurPageTex);
		g_pGfx->DrawTextureNL(m_iScreenX+102, m_iScreenY+31, pOtherPageTex);
		g_pFont->DrawText(m_iScreenX+20, m_iScreenY+32, "使用金币交易", COLOR_TABPAGE_PRESS,	FONT_YAHEI);
		g_pFont->DrawText(m_iScreenX+109, m_iScreenY+32, "使用元宝交易", COLOR_TABPAGE_NORMAL,	FONT_YAHEI);
		g_pFont->DrawText(m_iScreenX+100, m_iScreenY+57, "金额范围：1-150,000,000金币", 0xFFFFFF00,	FONT_YAHEI);
	}
	else if(m_iCurPage == 1)
	{
		g_pGfx->DrawTextureNL(m_iScreenX+14, m_iScreenY+31, pOtherPageTex);
		g_pGfx->DrawTextureNL(m_iScreenX+102, m_iScreenY+31, pCurPageTex);
		g_pFont->DrawText(m_iScreenX+20, m_iScreenY+32, "使用金币交易", COLOR_TABPAGE_NORMAL,	FONT_YAHEI);
		g_pFont->DrawText(m_iScreenX+109, m_iScreenY+32, "使用元宝交易", COLOR_TABPAGE_PRESS,	FONT_YAHEI);
	}
}

bool CBoothPriceWnd::OnLeftButtonUp(int iX, int iY)
{
	if (m_iCurPage == 0 && iX > 102 && iX <190 && iY > 32 && iY <51)
	{
		SwitchPage(1);
		return true;
	}else if( m_iCurPage == 1 && iX > 14 && iX < 92 && iY > 32 && iY < 51)
	{
		SwitchPage(0);
		return true;
	}
	return CCtrlWindowX::OnLeftButtonUp(iX,iY);
}

bool CBoothPriceWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_BOOTH_PRICE_FOCUS:
		{
			if(m_pCountEdit)
				m_pCountEdit->SetFocus();
			return true;

		}
	case MSG_CTRL_BUTTON_CLICK:
		if(pControl == m_pOKButton)
		{
			//参数检查
			if (strlen(m_pCountEdit->GetText()) == 0)
			{
				this->Msg(MSG_CTRL_BOOTH_PRICE_FOCUS,0,this);
				return true;
			}

			long lCount = 0;
			lCount = atol(m_pCountEdit->GetText());
			if (lCount <= 0 || (m_iCurPage == 1 && lCount > 10000000) || (m_iCurPage == 0 && lCount > 150000000))
			{
				this->Msg(MSG_CTRL_BOOTH_PRICE_FOCUS,0,NULL);
				return true;
			}

			char tempMsg[256] = {0};
			
			//租赁摊位消息
			if(m_dwMsg == MSG_CTRL_LEASEBOOTH_ADD_GOOD || m_dwMsg == MSG_CTRL_LEASEBOOTH_FIRSTTIME)//租赁摊位
			{
				
			
				CGood& readyGood = g_OtherBoothData.GetReadyGood();
				CGood* pGood = SELF.PackageGood().FindGood(readyGood.GetID());
				
				if (readyGood.GetID() == 0 || strlen(readyGood.GetName()) == 0 || pGood == NULL)
				{
					g_OtherBoothData.ClearReadyGood();
					CloseWindow();
					return true;
				}

				readyGood = *pGood;
				//得到价格并确认
				if (m_iCurPage == 0)//金币
				{
					readyGood.SetPrice(lCount);
					readyGood.SetPayType(0);
				}
				else//元宝
				{
					readyGood.SetPrice(lCount);
					readyGood.SetPayType(1);
				}
				
				char str[64] = {0};
				ToCommaStr(str,lCount);

				if (m_iCurPage == 0)
				{
					sprintf(tempMsg,"你决定以%s金币的价格出售“%s”物品吗？",str,readyGood.GetName());
				}
				else
				{
					sprintf(tempMsg,"你决定以%s元宝的价格出售“%s”物品吗？",str,readyGood.GetName());
				}
				
				if (m_dwMsg == MSG_CTRL_LEASEBOOTH_ADD_GOOD)
				{
					g_MsgBoxMgr.PopSimpleComfirm(tempMsg,MSG_CTRL_LEASEBOOTH_ADD_CONFIRM,0);
				} 
				else
				{
					g_MsgBoxMgr.PopSimpleComfirm(tempMsg,MSG_CTRL_BOOTH_FIRSTTIME_CONFIRM,0);
				}
				
			}

			//如果物品不在包裹里了,要重新设置价格
			GoodPrice_t& readyPrice = g_BoothData.GetReadyPrice();

			//查看物品是否在包裹中
			CGood* pGood = SELF.PackageGood().FindGood(readyPrice.GetID());

			if (readyPrice.GetID() == 0 || strlen(readyPrice.GetName()) == 0 || pGood == NULL)
			{
				g_BoothData.ClearReadyGood();
				CloseWindow();
				return true;
			}

			//得到价格并确认
			if (m_iCurPage == 0)//金币
				g_BoothData.SetReadyPrice(lCount,0);
			else//元宝
				g_BoothData.SetReadyPrice(0,lCount);

			if(m_dwMsg == MSG_CTRL_UPLOAD_ADD_GOOD)  //拍卖行上传不需要确认
			{
				g_PetBoothData.AddC2CGood();
				CloseWindow();
				return true;
			}
			//原地摆摊和豹子摆摊先进行确认
			char str[64] = {0};
			ToCommaStr(str,lCount);

			if (m_iCurPage == 0)
			{
				sprintf(tempMsg,"你决定以%s金币的价格出售“%s”物品吗？",str,readyPrice.GetName());
			}
			else
			{
				sprintf(tempMsg,"你决定以%s元宝的价格出售“%s”物品吗？",str,readyPrice.GetName());
			}

			if(m_dwMsg == MSG_CTRL_BOOTH_ADD_GOOD)
			{
				g_MsgBoxMgr.PopSimpleComfirm(tempMsg,MSG_CTRL_BOOTH_ADD_CONFIRM,MSG_CTRL_BOOTH_ADD_CANCEL);
			}
			else if(m_dwMsg == MSG_CTRL_FIRM_WND)
			{
				g_MsgBoxMgr.PopSimpleComfirm(tempMsg,MSG_CTRL_SELL_GOOD_VIPWND,MSG_CTRL_BOOTH_ADD_CANCEL);
			}
			else
			{
				g_MsgBoxMgr.PopSimpleComfirm(tempMsg,0,0);
			}

			CloseWindow();
			return true;

		}
		else if(pControl == m_pCancelButton)
		{
			g_BoothData.ClearReadyGood();
			CloseWindow();
			return true;

		}
	default:
		break;
	}

	return  CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

bool CBoothPriceWnd::SetFocus()
{
	return m_pCountEdit->SetFocus();
}
