#include "NpcListWnd.h"
#include "GameData/ConfigData.h"
#include "GameData/OtherData.h"
#include "GameData/NpcData.h"

const int CNpcListWnd::MAX_ROWS = 9;

INIT_WND_POSX(CNpcListWnd,POS_VARIABLE,POS_VARIABLE)

CNpcListWnd::CNpcListWnd()
{
	m_iType = sm_dwWindowType;
	m_iIndex = 4680;
	m_iPages = 1;
	
	m_iAlignType = XAL_TOPLEFT;

	if((g_pGfx->GetWidth() > 800))
	{
		m_iOffX = 284;
		m_iOffY = 180;
	}
	else
	{
		m_iOffX = 172;
		m_iOffY = 100;
	}

}

CNpcListWnd::~CNpcListWnd(void)
{
}

bool CNpcListWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_BUTTON_CLICK:
	default:
		break;
	}

	return  CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CNpcListWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	g_pFont->DrawText(m_iScreenX+197, m_iScreenY+25, "英 雄 榜", COLOR_TEXT_MAIN_TITLE, FONT_YAHEI, FONTSIZE_BIG);

	//底板色
	DWORD dwBkColor = 0x00E7BE8C | ((g_Config.GetTransColor()>>1) & 0xFF000000);

	//列表标题栏
	int iX = m_iScreenX + 35;
	int iY = m_iScreenY + 49;

	std::string str = g_NPC.GetTitleStrData();
	if(!str.empty())
	{
		g_pFont->DrawText(iX,iY,str.c_str(),0xFFFFFF00,FONT_YAHEI,FONTSIZE_DEFAULT,0);
	}

	iY = m_iScreenY + 67;

	int iDisLine = m_pScroll->GetPos();
	int iTotalCount = g_NPC.GetVectorStrCount();
	if(iDisLine + MAX_ROWS < iTotalCount)
		iTotalCount = iDisLine + MAX_ROWS;

	for(int i = iDisLine;i< iTotalCount;i++)
	{
		str = g_NPC.GetVectorStrData().at(i);
		if(!str.empty())
		{
			g_pFont->DrawText(iX,iY,str.c_str(),COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_DEFAULT);
		}
		iY += 16;
	}
}

void CNpcListWnd::OnCreate()
{
	CCtrlWindowX::OnCreate();
/*
	VString& strData = g_NPC.GetVectorStrData();
	VString::iterator it = strData.begin();
	it = strData.insert(it, " 1    asdasd          战士   41      10       100      30000");
	it = strData.insert(it, " 2    asdasd          战士   41      10       100      30000");
	it = strData.insert(it, " 3    asdasd          战士   41      10       100      30000");
	it = strData.insert(it, " 4    asdasd          战士   41      10       100      30000");
	it = strData.insert(it, " 5    asdasd          战士   41      10       100      30000");
	it = strData.insert(it, " 6    asdasd          战士   41      10       100      30000");
	it = strData.insert(it, " 7    asdasd          战士   41      10       100      30000");
	it = strData.insert(it, " 8    asdasd          战士   41      10       100      30000");
	it = strData.insert(it, " 9    asdasd          战士   41      10       100      30000");
	it = strData.insert(it, " 10   asdasd          战士   41      10       100      30000");
	it = strData.insert(it, " 11   asdasd          战士   41      10       100      30000");
	it = strData.insert(it, " 12   asdasd          战士   41      10       100      30000");
	it = strData.insert(it, " 13   asdasd          战士   41      10       100      30000");
	it = strData.insert(it, " 14   asdasd          战士   41      10       100      30000");*/

	m_pScroll = new CCtrlScroll();
	AddControl(m_pScroll);
	m_pScroll->CreateEx(this,425,46,17,180);
	m_pScroll->SetPos(0);
	m_pScroll->SetRange(g_NPC.GetVectorStrCount() - MAX_ROWS);

	SetCloseButton(423, 1, 80);
//	SetMask(4680);
}

bool CNpcListWnd::OnWheel(int iWheel)
{
	if(m_pScroll->GetRange() == 0)
		return true;

	short iWheelSize = (short)iWheel;
	iWheelSize /= WHEEL_DELTA;

	int iPos = m_pScroll->GetPos();
	iPos -= iWheelSize;

	if(iPos > m_pScroll->GetRange())
		iPos = m_pScroll->GetRange();
	if(iPos < 0)
		iPos = 0;
	m_pScroll->SetPos(iPos);

	return true;
}
