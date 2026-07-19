
#include "woolstorrecvokwnd.h"
#include "GameControl/GameControl.h"
#include "BaseClass/Control/CtrlButton.h"
#include "BaseClass/Control/CtrlMultiEdit.h"
#include "BaseClass/Control/CtrlEdit.h"
#include "GameData/WooolStoreData.h"
#include "GameData/DirtyWords.h"

INIT_WND_POSX(CWoolStorRecvOKWnd,POS_AUTO,POS_AUTO)

CWoolStorRecvOKWnd::CWoolStorRecvOKWnd(void)
{
	m_pOK = NULL ;
	m_iPos = 0;
	SetMode(true);

	for(int j=0;j<10;j++)
	{
		if(!g_WooolStoreMgr.GetRecvData(j)->m_strItemID.empty())
		{
			m_iPos = j;
			break;
		}
	}
	m_iIndex = 10421;

	//m_strRecvWords;	   //留言
	//m_strItemName;       //物品名称
	//m_strOriginUser;     //发送人
	m_strItemName = g_WooolStoreMgr.GetRecvData(m_iPos)->m_strItemName;
	m_strOriginUser = g_WooolStoreMgr.GetRecvData(m_iPos)->m_strOriginUser;
	m_strRecvWords = g_WooolStoreMgr.GetRecvData(m_iPos)->m_strSendWords;
	g_DirtyWords.ClearWords_All(m_strRecvWords);
}

CWoolStorRecvOKWnd::~CWoolStorRecvOKWnd(void)
{
	g_WooolStoreMgr.GetRecvData(m_iPos)->clear();//清掉缓存中当前接收物品的信息
}

void CWoolStorRecvOKWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	if( !m_strItemName.empty() && !m_strOriginUser.empty())
	{
		LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,20903,EP_UI);
		g_pGfx->DrawTextureNL(m_iScreenX + 40, m_iScreenY + 15, pTex);
		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,20904,EP_UI);
		g_pGfx->DrawTextureNL(m_iScreenX + 25, m_iScreenY + 79, pTex);

		g_pFont->DrawText(m_iScreenX+ 30,m_iScreenY + 134,"此道具为玩家赠送",COLOR_BTN_PRESS,FONT_YAHEI);
		g_pFont->DrawText(m_iScreenX+ 134,m_iScreenY + 19,m_strItemName.c_str(),COLOR_BTN_MOUSEON,FONT_YAHEI);  //
		g_pFont->DrawText(m_iScreenX+ 134,m_iScreenY + 36,m_strOriginUser.c_str(),COLOR_BTN_MOUSEON,FONT_YAHEI);//发送人
		g_pFont->DrawText(m_iScreenX+ 134,m_iScreenY + 54,"给你的礼物",COLOR_BTN_MOUSEON);//发送人
		//DrawText2(m_iScreenX+ 33,m_iScreenY + 88); //留言

		LPTexture pItemTex = g_pTexMgr->GetTexImm(PACKAGE_ptitems,atoi(g_WooolStoreMgr.GetRecvData(0)->m_strItemLooks.c_str()),EP_UI );
		g_pGfx->DrawTextureNL(m_iScreenX + 43, m_iScreenY + 19, pItemTex); 
		return;
	}
	else
	{
		CloseWindow();//关闭当前窗口
	}
}


//void CWoolStorRecvOKWnd::DrawText2(int iX,int iY)
//{
//	char * m_sMsg = const_cast<char *>(m_strRecvWords.c_str() );
//    if(m_sMsg == NULL)
//		return;
//
//	int StrLen = (int)strlen(m_sMsg);
//	char *p = m_sMsg;
//	char Tmp[128];
//	int ascii = 0,Chars = 0;
//	int iLine = 0;
//	int m_iLineLen = 28;
//    while(p<(m_sMsg+StrLen))
//	{
//		ascii=0;
//		Chars=0;
//		memset(Tmp,0,128);
//		//计算本行的字符数
//		int LeftLen = (int)(StrLen-(p-m_sMsg));     //剩余的字符长度
//        int CurLen;
//		if( LeftLen > m_iLineLen) 
//			CurLen = m_iLineLen;
//		else 
//			CurLen = LeftLen;
//		while( Chars < CurLen && p < m_sMsg + StrLen)
//		{
//		    //计算英文字符数量(0=双数 1=单数)
//			Tmp[Chars]=*p;//正常字符copy			
//			p++;
//			Chars++;
//			if((unsigned char)Tmp[Chars-1]==10)                    //如果是回车键就结束换行
//			{
//                 //这里回车键 不处理为换行
//				//Tmp[Chars-1]=0;
//				//goto _NEXT;
//			}			
//			if( (unsigned char)Tmp[Chars-1]>128 )		         //同样的中英文处理
//			{				
//                ascii=1-ascii;
//			}
//		}
//		//如果Tmp[chars-1]是汉字的最后半个.
//		if( ascii == 1 && p< m_sMsg + StrLen )
//		{
//			p--;					//留到下一行
//			Tmp[Chars-1]=0;         //去掉最后一个
//		}
//		g_pFont->DrawText(iX,iY + 16 * iLine,Tmp,0xFFFFFFFF);
//		iLine++;
//	}
//}

bool CWoolStorRecvOKWnd::Msg(DWORD dwMsg,DWORD dwData,CControl* pControl)
{
	if(dwMsg == MSG_CTRL_BUTTON_CLICK)
	{
		if(pControl == m_pOK)
		{
			SetMode(false);
			//通知服务端可以发送物品过来
			g_pGameControl->SEND_PT_Store_Info(8);
			//g_pControl->Msg(MSG_CTRL_WOOOLSTORE_RECV_INFO_WND,1,NULL);
			CloseWindow();//关闭当前窗口 并 向服务端发送接收消息			
			return true;
		}
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CWoolStorRecvOKWnd::OnCreate(void)
{

	
	//确定按钮
	m_pOK = new CCtrlButton();
	AddControl(m_pOK);
	m_pOK->CreateEx(this,140,127,165,166,167);	
	m_pOK->SetText("收下礼物",COLOR_BTN_NORMAL,COLOR_BTN_MOUSEON,COLOR_BTN_PRESS,COLOR_BTN_DISABLE,FONTSIZE_MIDDLE);

	// 详细说明框
	m_pSendText = new CCtrlMultiEdit(52,26,2,false,14);
	AddControl(m_pSendText);
	m_pSendText->Create(this,FONTSIZE_MIDDLE,28,81,196,40);
	m_pSendText->SetText(m_strRecvWords.c_str(),FONT_YAHEI,FONTSIZE_MIDDLE,0,0,0,0xffffffff,0xffffffff,0x00000000);

}

