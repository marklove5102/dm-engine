#include "LuxNpcWnd.h"
#include "GameControl/GameControl.h"
#include "Global/Interface/AudioInterface.h"
#include "GameData/NpcData.h"
#include "Baseclass/Control/ParserTip.h"

INIT_WND_POSX(CLuxNpcWnd,POS_AUTO,POS_AUTO)

CLuxNpcWnd::CLuxNpcWnd(void)
{
    m_pMarkViewer = NULL;
    m_iType = sm_dwWindowType;    

    if(m_iType == 2)
    {
        m_iStartFrame = 0;
        m_iTotalFrame = 108;
        m_vPauseFrame[0] = 9;
        m_vPauseFrame[1] = 43;
        m_vPauseFrame[2] = 59;
        m_vPauseFrame[3] = 89;
        m_vPauseFrame[4] = 105;
    }
    else if(m_iType == 6)
    {
        m_iStartFrame = 0;
        m_iTotalFrame = 51;
        m_vPauseFrame[0] = 9;
        m_vPauseFrame[1] = 43;
        m_vPauseFrame[2] = 43;
        m_vPauseFrame[3] = 43;
        m_vPauseFrame[4] = 43;
    }
    else if(m_iType == 7)
    {
        m_iStartFrame = 52;
        m_iTotalFrame = 39;
        m_vPauseFrame[0] = 7;
        m_vPauseFrame[1] = 37;
        m_vPauseFrame[2] = 37;
        m_vPauseFrame[3] = 37;
        m_vPauseFrame[4] = 37;
    }
    else if(m_iType == 8)
    {
        m_iStartFrame = 92;
        m_iTotalFrame = 16;
        m_vPauseFrame[0] = 13;
        m_vPauseFrame[1] = 13;
        m_vPauseFrame[2] = 13;
        m_vPauseFrame[3] = 13;
        m_vPauseFrame[4] = 13;
    }
    else
    {
        m_iStartFrame = 0;
        m_iTotalFrame = 0;
        m_vPauseFrame[0] = 0;
        m_vPauseFrame[1] = 0;
        m_vPauseFrame[2] = 0;
        m_vPauseFrame[3] = 0;
        m_vPauseFrame[4] = 0;
    }

    m_pGoWxxgBtn = NULL;    
    m_pGetPrizeBtn = NULL;
    m_pQuitBtn = NULL;
    m_pReplayBtn = NULL;

	if(m_iType == 11 || m_iType == 13 || m_iType == 15)
	{
		m_iIndex = 17060;
	}
	else if(m_iType == 12 || m_iType == 14 || m_iType == 16)
	{
		m_iIndex = 17061;
	}
	else
	{
		m_iIndex = 15575;
	}
}

CLuxNpcWnd::~CLuxNpcWnd(void)
{
}

void CLuxNpcWnd::OnCreate()
{
    m_wCurFrames = 0;
    m_wCriticalFrames = 0;
    m_iCount = 0;

	if(m_iType >= 11 && m_iType <= 16)
	{
		SetCloseButton(760,14);

		int iRow = g_TagTextMgr.GetNpcText().GetRow();
		if(iRow > 0)
		{
			int iCols = g_TagTextMgr.GetNpcText().GetColumn(iRow - 1);
			if(iCols > 0)
			{
				string strCommand = g_TagTextMgr.GetNpcText().GetCommand(iRow - 1,iCols - 1);
				m_iPercent = StringUtil::toInt(strCommand);
			}
		}
		m_pMarkViewer = new CMarkViewer(16,30);
	}
	else
	{
		SetCloseButton(760,13);
		m_pMarkViewer = new CMarkViewer(16,30);
	}

	AddControl(m_pMarkViewer);
	m_pMarkViewer->Create(this,586,48,0,0);
	m_pMarkViewer->SetTagText(&g_TagTextMgr.GetNpcText());
	m_pMarkViewer->SetDrawType(1);


    if(m_iType == 1)
    {
        m_pGoWxxgBtn = new CCtrlButton();
        AddControl(m_pGoWxxgBtn);
        m_pGoWxxgBtn->CreateEx(this,589,313,15576,15577,15578);

        m_pGetPrizeBtn = new CCtrlButton();
        AddControl(m_pGetPrizeBtn);
        m_pGetPrizeBtn->CreateEx(this,589,367,15584,15585,15586);        
    }
    else if(m_iType == 9)
    {
        m_pGoWxxgBtn = new CCtrlButton();
        AddControl(m_pGoWxxgBtn);
        m_pGoWxxgBtn->CreateEx(this,589,313,15576,15577,15578);

        m_pQuitBtn = new CCtrlButton();
        AddControl(m_pQuitBtn);
        m_pQuitBtn->CreateEx(this,589,367,15855,15856,15857); 
    }

    if(m_iType == 2 || m_iType == 6 || m_iType == 7 || m_iType == 8)
    {
        m_pReplayBtn = new CCtrlButton();
        AddControl(m_pReplayBtn);
        m_pReplayBtn->CreateEx(this,80,388,15838,15839,15840);
        m_pReplayBtn->SetShow(false);
    }
}

void CLuxNpcWnd::OnDraw()
{
    LPTexture pTex1 = NULL;

    switch(m_iType)
    {	
    case 2:		
    case 6:
    case 7:
    case 8:	    
        pTex1 = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15596,EP_UI);
        break;
    }

    if(pTex1 == NULL && (m_iType == 2 || m_iType == 6 || m_iType == 7 || m_iType == 8))	//大图片没有读取以小图片替代
    {        
        pTex1 = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15597,EP_UI);
        if(pTex1)
        {
            g_pGfx->DrawTextureNL(m_iScreenX + 9,m_iScreenY + 14,pTex1);
        }
        return;        
    }

    if(pTex1 && (m_iType == 2 || m_iType == 6 || m_iType == 7 || m_iType == 8))
    {
        int iSpeed = 9;

        pTex1->EnableSysAnim(false);

        if(m_wCurFrames == m_iTotalFrame * iSpeed)
        {
            m_pReplayBtn->SetShow(true);
            m_iCount += 3;
            if(m_iCount < 10)
            {
                m_pReplayBtn->Move(80,388 - m_iCount);
            }
            else if(m_iCount < 30)
            {
                m_pReplayBtn->Move(80,378 + (m_iCount - 10) / 2);
            }	

            pTex1->SetCurFrame(m_iStartFrame);
        }
        else
        {
            pTex1->SetCurFrame(m_iStartFrame + m_wCurFrames / iSpeed);
        }


        g_pGfx->DrawTextureNL(m_iScreenX + 9,m_iScreenY + 14,pTex1);

        if(m_wCurFrames == m_vPauseFrame[0] * iSpeed || m_wCurFrames == m_vPauseFrame[1] * iSpeed || m_wCurFrames == m_vPauseFrame[2] * iSpeed || m_wCurFrames == m_vPauseFrame[3] * iSpeed || m_wCurFrames == m_vPauseFrame[4] * iSpeed) //暂停的几帧
        {
            m_wCriticalFrames = 60;
            m_wCurFrames++;
        }
        else if(m_wCriticalFrames > 0)
        {
            m_wCriticalFrames--;
        }
        else if(m_wCurFrames < m_iTotalFrame * iSpeed)
        {
            m_wCurFrames++;
        }
    }
}

void CLuxNpcWnd::Draw()
{
    CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

    LPTexture pTex = NULL;	

    switch(m_iType)
    {
    case 1:
    case 9:
        pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15588,EP_UI);
        break;
    case 2:		//没有底图		
        break;		
    case 3:
        pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15589,EP_UI);
        break;
    case 4://封元对决房间		
        break;
    case 5://封元对决排行榜        
        break;
    case 6://		
    case 7:
    case 8:	    
        break;
	case 13:
	case 14:
		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17077 + m_iType - 13,EP_UI);
		break;
	case 15:
	case 16:
		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17065,EP_UI);
		break;
    }

    if(pTex)
    {
        g_pGfx->DrawTextureNL(m_iScreenX + 6,m_iScreenY + 17,pTex);
    }

	if(m_iType >= 11 && m_iType <= 16)
	{
		int iTop = int(float((100 - m_iPercent)) * 420 / 100);
		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17062 + m_iType%2,EP_UI);
		g_pGfx->DrawPartTexture(m_iScreenX,m_iScreenY,pTex,0,iTop,-1,-1);
	}
}

void CLuxNpcWnd::OnClickCloseButton()
{
    g_pGameControl->SEND_Exchange_Menu_Click(g_NPC.GetID(),"@exit");
    g_pControl->PopupWindow(MSG_CTRL_LUXNPCWND,OPER_CLOSE);
}

bool CLuxNpcWnd::OnLeftButtonDown(int iX, int iY)
{
    m_bClick = true;
    return true;
}

bool CLuxNpcWnd::OnLeftButtonUp(int iX, int iY)
{
    m_bClick = false;

    string strCommand = m_pMarkViewer->GetCommand();

    if(!strCommand.empty())
    {
        g_pAudio->Play(EAT_OTHER,5,g_pAudio->GetRand()++);

        if((int)strCommand.size() > 0)
        {
            g_pGameControl->SEND_Exchange_Menu_Click(g_NPC.GetID(),strCommand.c_str());

            if(strCommand == "@exit")
                g_pControl->PopupWindow(MSG_CTRL_LUXNPCWND,OPER_CLOSE);

            return true;
        }
    }

    return CCtrlWindowX::OnLeftButtonUp(iX,iY);
}

bool CLuxNpcWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
    switch( dwMsg )
    {
    case MSG_CTRL_BUTTON_CLICK:
        {
            if(m_iType == 1 || m_iType == 9)
            {
                if(pControl == m_pGoWxxgBtn)
                {
                    g_pGameControl->SEND_Exchange_Menu_Click(g_NPC.GetID(),"@_wxxg_main");
                    return true;
                }
                else if(pControl == m_pGetPrizeBtn)
                {
                    g_pGameControl->SEND_Exchange_Menu_Click(g_NPC.GetID(),"@_blsfenyin_main");
                    return true;
                }	
                else if(pControl == m_pQuitBtn)
                {
                    g_pGameControl->SEND_Exchange_Menu_Click(g_NPC.GetID(),"@exit");
                    return true;
                }
            }
            else if(m_iType == 2 || m_iType == 6 || m_iType == 7 || m_iType == 8)
            {
                if(pControl == m_pReplayBtn)
                {
                    m_pReplayBtn->SetShow(false);
                    m_wCurFrames = 0;
                    m_iCount = 0;
                    return true;
                }
            }
        }
        break;
    }

    return  CCtrlWindow::Msg(dwMsg,dwData,pControl);
}

bool CLuxNpcWnd::OnMouseMove(int iX,int iY)
{
	if(m_iType >= 11 && m_iType <= 16)
	{
		CParserTip* pTip = g_pControl->GetTipWnd();

		if(iX >= 13 && iX <= 46 && iY >= 19 && iY <= 435)
		{
			pTip->Clear();
			char strTemp[24]={0};
			sprintf(strTemp,"%d%%",m_iPercent);
			pTip->AddText(strTemp);

			int x = m_iScreenX + iX + 10;
			int y = m_iScreenY + iY + 10;
			pTip->AdjustXY(x,y);
			pTip->Move(x,y);
			pTip->SetShow(true);
			return true;
		}
	}

	return CCtrlWindowX::OnMouseMove(iX,iY);
}