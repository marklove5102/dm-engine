#include "namewnd.h"
#include "GameControl/GameControl.h"
#include "GameData/TalkName.h"

INIT_WND_POSX(CNameWnd,POS_VARIABLE,POS_VARIABLE)

CNameWnd::CNameWnd()
{
	m_iType = sm_dwWindowType;
	if(m_iType == 1)
		m_iCount = g_TalkName.GetSize();
	else if(m_iType == 2)
		m_iCount = g_TalkName.GetLWNameSize();
	else
		m_iCount = 0;

	m_iStart = 0;
	m_iCurrent =0;
	m_byFirstChar = 0;
	m_iControlMode = CTRL_MODE_WINDOW;

	m_iPages = 1;
	
	m_iIndex = 162;

	m_iAlignType = XAL_BOTTOMLEFT;
	m_iOffX = 189;
	m_iOffY = -40;
}

CNameWnd::~CNameWnd(void)
{
}

void CNameWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	int x = m_iScreenX + 5;
	int y = m_iScreenY + 154;
	int width = 115;

	for(int i = 0; i<MAX_ROWS && i + m_iStart < m_iCount ;i++)
	{
		char * pName = NULL;
		if(m_iType == 1)
			pName =(char *)g_TalkName.GetNameList(i+m_iStart).c_str();
		else if(m_iType == 2)
			pName = g_TalkName.GetLWName(i + m_iStart);		

		if(i == m_iCurrent)
		{
			g_pGfx->DrawRectangleWithFrame(0x0,0xFFFFFFFF, x, y - i*16  ,x+width, y-(i-1)*16 - 2 ,0);
			g_pFont->DrawText(x, y - i*16 ,pName,0xFFFF0000);
		}
		else
			g_pFont->DrawText(x, y - i*16, pName,0xFF00FF00);
	}

}	

bool CNameWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl )
{
	return  CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CNameWnd::OnCreate()
{
}

bool CNameWnd::OnKeyDown(WORD wState,UCHAR cKey)
{
	char name[40]={0};
	switch(cKey)
	{
	case VK_DOWN:
		{
			if(m_iCurrent>0)
				m_iCurrent--;
			else if(m_iCurrent ==0)
			{
				if(m_iStart>0)
				{
					m_iStart -= MAX_ROWS  ;     //一页的行数-1
					m_iCurrent = MAX_ROWS -1;
				}
			}
			return true;
		}
	case VK_UP:
		{
			if(m_iCurrent+m_iStart < m_iCount-1)
			{
				if(m_iCurrent==MAX_ROWS -1)
				{
					m_iStart+=MAX_ROWS;
					m_iCurrent = 0;
				}
				else
					m_iCurrent++;	

			}
			return true;
		}
	case VK_RETURN:
		{

			if(m_iType == 1)
				sprintf(name,"/%s ",g_TalkName.GetNameList(m_iCurrent + m_iStart).c_str());

			else if(m_iType == 2)
			{
				char *p = g_TalkName.GetLWName(m_iCurrent + m_iStart);
				if(p[0] == '1')
					sprintf(name,"#%s ",p);
				else
					sprintf(name,"##%s ",p);
			}
			g_pControl->Msg(MSG_CTRL_INSERT_TEXT,0,(CControl *)name);
			CloseWindow();
			return true;
		}
	} 

	return CCtrlWindowX::OnKeyDown(wState,cKey);	
}
