#include "SealWnd.h"
#include "Global/Global.h"
#include "Global/GlobalMsg.h"
#include "GameData/GameData.h"
#include "GameData/ItemCfgMgr.h"

INIT_WND_POSX(CSealWnd,POS_AUTO,POS_AUTO)

CSealWnd::CSealWnd(void)
{
	m_pCloseButton = NULL;
	ZeroMemory(m_szName,64);
	m_iCardNum = 0;
	m_iRarity = 0;	
	m_iType = 0;
	m_bFlashCard = false;

	m_iIndex = 15170;
	m_iPages = 1;
	
}

CSealWnd::~CSealWnd(void)
{
}

void CSealWnd::OnCreate()
{
	SetMask(15170,249,339);
	m_pCloseButton = new CCtrlButton();
	AddControl(m_pCloseButton);
	m_pCloseButton->CreateEx(this,198,28,15171,15172,15173);		
}

bool CSealWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch (dwMsg)
	{
	case MSG_CTRL_BUTTON_CLICK:
		if(pControl == m_pCloseButton)
		{	
			CloseWindow();
			return true;
		}

		break;
	case MSG_CTRL_SEAL_WND:
		if(dwData == 1)
		{
            CGood* pGood = (CGood*)pControl;
			GetCardInfo(pGood);
			/*if(pGood)
		        pGood->SetID(0)*/;
			return true;
		}
		else if(dwData == 2)
		{
			DWORD dwValue = (DWORD)((__int64)pControl);
			int iType = LOWORD(dwValue);
			int iNum = HIWORD(dwValue);
			GetCardInfo(iType,iNum);
			return true;
		}

		break;
	default:
		break;
	}

	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CSealWnd::GetCardInfo(int iType,int iNum)
{
	m_iType = iType;

	if(iNum > 10)
	{
		iNum -= 10;
		m_bFlashCard = true;
	}
	else
	{
		m_bFlashCard = false;
	}
	m_iCardNum = iNum;

    if(m_iType == 0)
    {
        switch(m_iCardNum)
        {
        case 1:
        case 2:
        case 3:
        case 4:
            m_iRarity = 1;
            break;
        case 5:
        case 6:
        case 7:
            m_iRarity = 2;
            break;
        case 8:
        case 9:
            m_iRarity = 3;
            break;
        case 10:
            m_iRarity = 4;
            break;
        }
    }
    else if(m_iType == 2)
    {
        switch(m_iCardNum)
        {
        case 1:
        case 2:
        case 3:
            m_iRarity = 4;
            break;
        }
    }

	char szKey[16] = "";
	char szDescription[512] = "";
	sprintf(szKey,"%d",m_iType * 20 + m_iCardNum);
	g_ItemCfgMgr.ParseCardInfo("CardName",szKey,m_szName,NULL);
}

void CSealWnd::GetCardInfo(CGood * pGood)
{
	if(pGood == NULL)
	{
		return;
	}

	if(pGood->GetID() != 0 && pGood->GetStdMode() == 49 && pGood->GetShape() == 55)		//封元印
	{
		m_iType = pGood->GetAC();
		m_iCardNum = pGood->GetAC2();

		if(pGood->GetMAC() == 1)
		{
			m_bFlashCard = true;
		}
		else
		{
			m_bFlashCard = false;
		}

		m_iRarity = pGood->GetRarity();

		char szKey[16] = "";
		char szDescription[512] = "";
		sprintf(szKey,"%d",m_iType * 20 + m_iCardNum);
		g_ItemCfgMgr.ParseCardInfo("CardName",szKey,m_szName,NULL);
	}	
}

void CSealWnd::Draw()
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	LPTexture pTex = NULL;

    //画圈
    pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15169,EP_UI);
    if(pTex)
    {
        g_pGfx->DrawTextureNL(m_iScreenX + 96,m_iScreenY + 192,pTex);
    }

    //画星
    pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15174,EP_UI);
    if(pTex)
    {
        for(int i = 0;i < m_iRarity;i++)
        {
            g_pGfx->DrawTextureNL(m_iScreenX + 211 - 17 * i,m_iScreenY + 193,pTex);
        }
    }

    //画卡号
    char temp[] = "壹贰叁肆伍陆柒捌玖拾";
    char szCardNum[3] = "";
    if(m_iCardNum > 0 && m_iCardNum < 11)
    {
        memcpy(szCardNum,temp + (m_iCardNum - 1) * 2,2);
    }
    g_pFont->DrawText(m_iScreenX + 101,m_iScreenY + 194,szCardNum,0xffc2ab14);

    //画卡片的所属类别
    char szTypeName[32] = "";
    if(m_iType == 0)
    {
        strcpy(szTypeName,"铁血魔城");
    }
    g_pFont->DrawText(m_iScreenX + 37,m_iScreenY + 194,szTypeName,0xffc2ab14);

    //显示卡片描述和卡片名称
    g_pFont->DrawText(m_iScreenX + 120 - strlen(m_szName) * 4,m_iScreenY + 23,m_szName,0xffc2ab14,FONT_LISHU,18);

	if(m_bFlashCard)
	{
		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15179,EP_UI);
		if(pTex)
		{
			g_pGfx->SetRenderMode(RM_ADD1);
			g_pGfx->DrawTextureNL(m_iScreenX,m_iScreenY + 46,pTex);
			g_pGfx->SetRenderMode();
		}
	}

	//画怪物图片
	pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15200 + m_iType * 20 + m_iCardNum - 1,EP_UI);
	if(pTex)
	{
		g_pGfx->DrawTextureNL(m_iScreenX,m_iScreenY,pTex);
	}	
}

