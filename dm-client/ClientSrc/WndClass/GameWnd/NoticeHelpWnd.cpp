#include "noticehelpwnd.h"
#include <Shellapi.h>
#include "Global/Interface/StreamInterface.h"


INIT_WND_POSX(CNoticeHelpWnd,POS_VARIABLE,POS_VARIABLE)

CNoticeHelpWnd::CNoticeHelpWnd(void)
{
	vecStr.push_back("为了游戏的乐趣与公平性，请勿使用非官方公布的软件和游戏");
	vecStr.push_back("版本。");
	vecStr.push_back("游戏操作简介：");
	vecStr.push_back("按~键，捡取脚下物品。");
	vecStr.push_back("按ALT+C，将选中玩家组入队伍中。");
	vecStr.push_back("再次按ALT+C，将玩家从队伍中踢出。");
	vecStr.push_back("按ALT+鼠标右键，将玩家姓名放入对话框。");
	vecStr.push_back("更多信息请关注：");
	string strUrl = g_pStreamMgr->GetWebsite("Website","");
	str.append(strUrl);

	m_iIndex = 4859;
	m_iPages = 1;
	
	m_iAlignType = XAL_TOPLEFT;
	m_iOffX = (g_pGfx->GetWidth() > 800)?300:187;
	m_iOffY = 50;

}

CNoticeHelpWnd::~CNoticeHelpWnd(void)
{
}

void CNoticeHelpWnd::OnCreate()
{
	SetCloseButton(391,13);
}

void CNoticeHelpWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	int iX = 41,iY = 68;
	for(int i=0;i<vecStr.size();i++)
	{
		if(!vecStr[i].empty())
			g_pFont->DrawText(m_iScreenX+iX,m_iScreenY+iY,vecStr[i].c_str(),0xFFFFFFFF,FONT_DEFAULT,FONTSIZE_DEFAULT,DTF_BlackFrame);

		iY+=16;
	}

	if(!str.empty())
		g_pFont->DrawText(m_iScreenX+iX+100,m_iScreenY+180,str.c_str(),0xFFFFFF00,FONT_DEFAULT,FONTSIZE_DEFAULT,DTF_UnderLine|DTF_BlackFrame);

}

bool CNoticeHelpWnd::OnLeftButtonDown(int iX,int iY)
{
	if(iX >= 100 && iX <= 100 + 8*str.size() && iY >= 178 && iY <= 196)
	{
		::ShellExecute(NULL,"open","iexplore.exe",str.c_str(),NULL,SW_SHOW);
	}

	return CCtrlWindowX::OnLeftButtonDown(iX,iY);
}