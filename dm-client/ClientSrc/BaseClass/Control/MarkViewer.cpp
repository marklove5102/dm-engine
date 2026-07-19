#include "MarkViewer.h"
#include "GameData/GameGlobal.h"
#include "GameControl/GameControl.h"
#include "baseclass/control/ParserTip.h"
#include "GameData/NpcData.h"


DTI_IMPLEMENT(CMarkViewer, CControl)

CMarkViewer::CMarkViewer(float fLineHeight,int iLineCount,int iOffX,int iOffY,bool bNewVersion)
{
    m_fLineHeight = fLineHeight;
    m_iLineCount = iLineCount;
    m_iDisLine = 0;
    m_iDrawType = 0;
	m_pUpButton = NULL;
	m_pDownButton = NULL;
	m_pScroll = NULL;
	m_iDrawOffX = iOffX;
	m_iDrawOffY = iOffY;
	m_iFontSize = FONTSIZE_DEFAULT;
	m_bNewVersion = bNewVersion;
    m_bClick = false;
    m_dwDefaultColor = COLOR_DEFAULT;
	m_iScrollStep = 0;

    m_TagText = NULL;
}

CMarkViewer::~CMarkViewer(void)
{
}

void CMarkViewer::SetTagText(CTagText* tag)
{
    m_TagText = tag;

	if(m_TagText)
	{
		if(m_pScroll)
		{
			m_pScroll->SetRange(m_TagText->GetRow() -1);
		}
	}
	else
	{
		if(m_pScroll)
		{
			m_pScroll->SetRange(0);
		}
	}
}

void CMarkViewer::SetDisLine(int iDisLine)
{
    m_iDisLine = iDisLine;
	if(m_pScroll)
		m_pScroll->SetPos(iDisLine);
}

void CMarkViewer::AddUpButton(int iX,int iY,WORD wTex,WORD wMTex,WORD wCTex)
{
	if(m_pUpButton)
		RemoveControl((CControl**)&m_pUpButton);

	m_pUpButton = new CCtrlButton();
	AddControl(m_pUpButton);
	m_pUpButton->CreateEx(this,iX,iY,wTex,wMTex,wCTex);
}

void CMarkViewer::AddDownButton(int iX,int iY,WORD wTex,WORD wMTex,WORD wCTex)
{
	if(m_pDownButton)
		RemoveControl((CControl**)&m_pDownButton);

	m_pDownButton = new CCtrlButton();
	AddControl(m_pDownButton);
	m_pDownButton->CreateEx(this,iX,iY,wTex,wMTex,wCTex);
}

void CMarkViewer::AddScroll(int iX,int iY,int iW,int iH,int iScrollIdx)
{
	if(m_pScroll)
		RemoveControl((CControl**)&m_pScroll);

	m_pScroll = new CCtrlScroll();
	AddControl(m_pScroll);
	m_pScroll->Create(this,iX,iY,iW,iH,iScrollIdx);

	if(m_TagText)
		m_pScroll->SetRange(m_TagText->GetRow());
}

void CMarkViewer::AddScrollEx(int iX,int iY,int iW,int iH)
{
	if(m_pScroll)
		RemoveControl((CControl**)&m_pScroll);

	m_pScroll = new CCtrlScroll();
	AddControl(m_pScroll);
	m_pScroll->CreateEx(this,iX,iY,iW,iH);

	if(m_TagText)
		m_pScroll->SetRange(m_TagText->GetRow());
}

void CMarkViewer::SetDefaultColor(DWORD dwColor)
{
    m_dwDefaultColor = dwColor;
}

void CMarkViewer::OnCreate(void)
{
	if (m_bNewVersion && !m_pScroll)
	{
		m_pScroll = new CCtrlScroll();
		AddControl(m_pScroll);		
		m_pScroll->CreateEx(this,m_iWidth - 14,0,14,m_iHeight);
	}
}

void CMarkViewer::Draw()
{
	CControlContainer::Draw();

    if(!m_TagText)
    {
        return;
    }

    if(m_iDrawType == 1)
    {
        DrawEx();
        return;
    }

    m_strCommand.clear();
	if (m_pScroll)
	{
		m_iDisLine = m_pScroll->GetPos();
	}

	if (m_iScrollStep > 0)
	{
		m_iDisLine -= m_iDisLine % m_iScrollStep;
	}

    int iRows = m_TagText->GetRow();
    if(m_iLineCount > 0 && m_iDisLine + m_iLineCount < iRows)
    {
        iRows = m_iDisLine + m_iLineCount;
    }
    
    int mouseX,mouseY;
    g_pInput->GetMousePos(mouseX,mouseY);
    
    int iFontSize = 12;
    
    int iOffY = 0;  //因为绘制图片及偏移导致的y位置增加   

    for(int i = m_iDisLine; i < iRows; i++)  //row
    {
        int iPrintX = m_iScreenX + m_iDrawOffX;
        int iPrintY = m_iScreenY + m_iDrawOffY + iOffY;
        int iCols = m_TagText->GetColumn(i);
        int iMaxHeightInRow = 0;
        

        for(int j = 0; j < iCols; j++ )     //col
        {
            DWORD dwColor = 0xFFFFFF00;                  //显示文字的颜色
			_Tag* pTag = m_TagText->GetAt(i,j);
           
            if(!pTag)
            {
                continue;
            }

			iPrintX = iPrintX + pTag->iOffX;
			int iDrawY = iPrintY + pTag->iOffY;

			if(m_iLineCount > 0 && iDrawY + iFontSize > m_iScreenY + m_iDrawOffY + (int)(m_fLineHeight * m_iLineCount))
			{
				continue;
			}

			pTag->iDrawX = iPrintX;
			pTag->iDrawY = iDrawY;

			iMaxHeightInRow = max(iMaxHeightInRow,(int)m_fLineHeight + pTag->iOffY);

            int len = g_pFont->GetLen(m_TagText->GetString(i,j), NULL, m_iFontSize);
            
            if(m_TagText->IsCommand(i,j))       //将要显示的是链接
            {
                if(g_pInput->IsLeftButton())
                {
                    if(mouseX >= iPrintX && mouseX <= iPrintX + len 
                    && mouseY >= iDrawY && mouseY <= iDrawY + iFontSize) 
                    {
                        //点中了此链接,设成红色,并保存命令
                        dwColor = 0xFFFF0000;
                        m_strCommand = m_TagText->GetCommand(i,j);
                    }                   
                }

				if (strlen(m_TagText->GetString(i,j)) > 0)
				{
					if(IsFadeIn())
					{
						dwColor &= m_dwBackColor;
					}

					g_pFont->DrawText(iPrintX,iDrawY,m_TagText->GetString(i,j),dwColor,m_iFont,m_iFontSize,DTF_UnderLine);
				}                
            }
            else if(m_TagText->IsGoodName(i,j))     //Tag含有物品名字，将物品显示出来，并且可以获得物品属性
            {
                CGood* pGood = NULL;

                const char* strGoodName = m_TagText->GetTagGoodName(i,j);
                if(strGoodName)
                {
                    map<std::string,CGood>::iterator itr = CTagText::sm_GoodMap.find(strGoodName);
                    if(itr != CTagText::sm_GoodMap.end())
                    {
                        pGood = &(itr->second);
                    }
                }

                if(pGood)
                {
                    iPrintX += 2;
                    DWORD dwPicID = pGood->GetLooks();
                    LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_items,(WORD)dwPicID,EP_UI);
                    if(pTex)
                    {
                        //如果鼠标在图片上就设置tips内容
                        int iMouseX,iMouseY;
                        g_pInput->GetMousePos(iMouseX,iMouseY);

                        if(iMouseX >= iPrintX && iMouseX < iPrintX + pTex->GetWidth0() + 2
                            && iMouseY >= iDrawY && iMouseY < min(iDrawY + pTex->GetHeight0(),m_iScreenY + (int)(m_fLineHeight * m_iLineCount)))
                        {
							CParserTip *pTip = g_pControl->GetTipWnd();
                            pTip->Clear();
                            pTip->Parse(*pGood);
                            pTip->Move(iMouseX + 10,iMouseY + 10);
                            pTip->SetShow(true);
                        }

                        int iPicHeight = pTex->GetHeight0();
                        iMaxHeightInRow = max(iPicHeight + pTag->iOffY,iMaxHeightInRow);  //获得这行的最大高度
                        DrawGoodLooks(iPrintX,iDrawY,pTex);
                        iPrintX += pTex->GetWidth0() + 2;

						dwColor = 0xFFFFFFFF;
						if(IsFadeIn())
						{
							dwColor &= m_dwBackColor;
						}
                        g_pFont->DrawText(iPrintX,iDrawY,pGood->GetName(),dwColor,m_iFont,m_iFontSize);   //显示物品名称
                        iPrintX += g_pFont->GetLen(pGood->GetName(), NULL, m_iFontSize) + 2; 

                    }                                     
                }
            }
            else if(m_TagText->IsGoodLooks(i,j))
            {
                LPTexture pTex = g_pTexMgr->GetTexFromID(m_TagText->GetTagGoodLooks(i,j),EP_UI);
                if(pTex)
                {
                    iPrintX += 2;
                    int iPicHeight = pTex->GetHeight();
                    iMaxHeightInRow = max(iPicHeight + pTag->iOffY,iMaxHeightInRow);  //获得这行的最大高度
                    DrawGoodLooks(iPrintX,iDrawY,pTex);
                    iPrintX += pTex->GetWidth0() + 2;
                }
            }
            else                    //显示的是普通文字
            {
                dwColor = m_TagText->GetColor(i,j);
                if(dwColor == COLOR_DEFAULT)
                {
                    dwColor = m_dwDefaultColor;
                }

				if(IsFadeIn())
				{
					dwColor &= m_dwBackColor;
				}
				
                g_pFont->DrawText(iPrintX,iDrawY,m_TagText->GetString(i,j),dwColor,m_iFont,m_iFontSize);
            }
            iPrintX += len;
        }
        iOffY += iMaxHeightInRow;
    }
}

void CMarkViewer::DrawEx()
{
	CControlContainer::Draw();

	m_strCommand.clear();
	if (m_pScroll)
	{
		m_iDisLine = m_pScroll->GetPos();
	}

    int iPrintCommandY = m_iScreenY + 237;

    int iRows = m_TagText->GetRow();
    if(m_iLineCount > 0 && m_iDisLine + m_iLineCount < iRows)
    {
        iRows = m_iDisLine + m_iLineCount;
    }

    int mouseX,mouseY;
    g_pInput->GetMousePos(mouseX,mouseY);


    for(int i = m_iDisLine; i < iRows; i++)  //row
    {
        int iPrintX = m_iScreenX + m_iDrawOffX;
        int iPrintY = m_iScreenY + m_iDrawOffY + (int)(m_fLineHeight * (i - m_iDisLine));
		int iCols = m_TagText->GetColumn(i);
        int iMaxHeightInRow = (int)m_fLineHeight;

        if(m_iLineCount > 0 && iPrintY + m_iFontSize > m_iScreenY + (int)(m_fLineHeight * m_iLineCount))
        {
            break;
        }

        bool bHasCommand = false;
        for(int j = 0; j < iCols; j++ )     //col
        {
            DWORD dwColor = 0xFFFFFF00;                  //显示文字的颜色
			_Tag* pTag = m_TagText->GetAt(i,j);

            if(!pTag)
            {
                continue;
            }

			pTag->iDrawX = iPrintX;
			pTag->iDrawY = iPrintY;
            int len = g_pFont->GetLen(m_TagText->GetString(i,j));

            if(m_TagText->IsCommand(i,j))       //将要显示的是链接
            {
                iPrintY = iPrintCommandY;	
                bHasCommand = true;	

                if(g_pInput->IsLeftButton())
                {
                    if(mouseX >= iPrintX && mouseX <= iPrintX + len 
                        && mouseY >= iPrintY && mouseY <= iPrintY + m_iFontSize) 
                    {
                        //点中了此链接,设成红色,并保存命令
                        dwColor = 0xFFFF0000;
                        m_strCommand = m_TagText->GetCommand(i,j);
                    }                   
                }

                g_pFont->DrawText(iPrintX,iPrintY,m_TagText->GetString(i,j),dwColor,m_iFont,m_iFontSize,DTF_UnderLine);
            }
            else                    //显示的是普通文字
            {
                dwColor = m_TagText->GetColor(i,j);
                if(dwColor == COLOR_DEFAULT)
                {
                    dwColor = m_dwDefaultColor;
                }

                g_pFont->DrawText(iPrintX,iPrintY,m_TagText->GetString(i,j),dwColor);                
            }
            iPrintX += len;
        }

        if(bHasCommand)
        {
            iPrintCommandY += 14;
        }
        
    }
}

void CMarkViewer::DrawGoodLooks(int iPrintX,int iPrintY,LPTexture pTex)
{
    if(iPrintY + pTex->GetHeight() > m_iScreenY + (int)(m_fLineHeight * m_iLineCount))
    {        
        RECT rc;
        rc.left = 0;
        rc.top = 0;
        rc.right = pTex->GetWidth0();
        rc.bottom = (int)(m_fLineHeight * m_iLineCount) + m_iScreenY - iPrintY;
        g_pGfx->DrawTextureFX(iPrintX,iPrintY,pTex,-1,&rc);        
    }
    else
    {
        g_pGfx->DrawTextureNL(iPrintX,iPrintY,pTex);        
    }
}

bool CMarkViewer::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_BUTTON_CLICK:
		{
			if(pControl == m_pUpButton)
			{
				if(m_pScroll)
					m_pScroll->GoDown();

				return true;
			}
			else if(pControl == m_pDownButton)
			{
				if(m_pScroll)
					m_pScroll->GoUp();

				return true;
			}
			break;
		}
	case MSG_INPUT_WHEEL:	// 只有有Focus的控件会收到这个消息
		return OnWheel( (int)LOWORD( dwData ));
		break;
	default:
		break;
	}

	if(IsSonsMsg(dwMsg,dwData,pControl))
		return true;
	
    return false;           //不让此控件影响父控件接受消息
}

bool CMarkViewer::OnWheel(int iWheel)
{
	if(m_pScroll)
		return m_pScroll->OnWheel(iWheel);

	return false;
}

void  CMarkViewer::SetScrollPos(int i)
{
	if(m_pScroll && i >= 0 && i < m_pScroll->GetRange())
	{
		m_pScroll->SetPos(i);
		m_iDisLine = i;
	}
}

void CMarkViewer::DoCommand()
{
    if(m_strCommand.empty())
    {
        return;
    }

    if(m_strCommand.substr(0,2) == "@@")
    {
    }
    else
    {
        g_pGameControl->SEND_Exchange_Menu_Click(g_NPC.GetID(),m_strCommand.c_str());        
    }
}

void CMarkViewer::SetScrollStep(int iScrollLines)
{
	m_iScrollStep = iScrollLines;
}

int CMarkViewer::GetScrollStep()                
{
	return m_iScrollStep;
}

void CMarkViewer::SetScrollShow(bool show)
{
	if(m_pScroll)
	{
		m_pScroll->SetShow(show);
	}
}