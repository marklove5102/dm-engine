#include "KfzResultWnd.h"


INIT_WND_POSX(CKfzResultWnd,POS_AUTO,POS_AUTO)

CKfzResultWnd::CKfzResultWnd(void)
{
	m_iIndex = 15183;
	m_iPages = 1;
	m_iResult = -1;
	m_dwDrawTimes = 0;
	m_bModel = true;
}

CKfzResultWnd::~CKfzResultWnd(void)
{
}

void CKfzResultWnd::OnCreate()
{
	m_pCancel = new CCtrlButton();
	AddControl(m_pCancel);
	m_pCancel->CreateEx(this,177,224,4843,4844,4845);

	m_pMarkViewer = new CMarkViewer(18,15);
	AddControl(m_pMarkViewer);
	m_pMarkViewer->Create(this,90,87,229,81);
	m_pMarkViewer->SetTagText(&m_TagText);

	SetMask(15183);
}	

void CKfzResultWnd::Draw(void)
{	
	if(m_iResult >= 0 && m_iResult <= 3 && m_dwDrawTimes < 150)
	{
		m_dwDrawTimes ++;
		float fMinSize = 0.4f, fMaxSize = 1.0f;
		int iY = (g_pGfx->GetWidth() > 800)?200:150;

		float fDura = (float)m_dwDrawTimes / 100;
		if(fDura > 1.0f)
			fDura = 1.0f;

		const float fCurSize = fMaxSize * fDura + fMinSize * (1.0f-fDura);
		DWORD dwColor = ((DWORD)(0xFF*fDura)<<24) + 0xFFFFFF;

		int iIndex = m_iResult;
		if(m_iResult == 3)
			iIndex = 2;

		LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,16045 + m_iResult,EP_UI);
		if(pTex)
		{
			int iInterval = (int)(150*fCurSize);
			int iLeft = 150-iInterval;
			int iX = (g_pGfx->GetWidth() - pTex->GetWidth0()) / 2;
			g_pGfx->DrawTextureNL_Trans(iX + iLeft,iY, pTex,fCurSize,fCurSize,0,dwColor);
		}
	}
	else
	{
		CCtrlWindowX::Draw();
		//背景图片还没有下载下来,其它的东西不画
		if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
		{
			return;
		}
	}
}

bool CKfzResultWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch(dwMsg)
	{
	case MSG_CTRL_KFZ_RESULT:
		{
			if(pControl)
			{
				char * pChar = (char *)pControl;
				//string str = pChar;
				string str;
				m_iResult = dwData/10000;
				str.assign(pChar,dwData%10000);
				m_TagText.Parse(str);
				return true;
			}
		}
		break;
	case MSG_CTRL_BUTTON_CLICK:
		{
			if(pControl == m_pCancel)
			{
				CloseWindow();
				return true;
			}
		}
		break;
	default:
		break;
	}

	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}