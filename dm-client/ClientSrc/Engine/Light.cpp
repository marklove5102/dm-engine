#include "stdafx.h"
#include "Light.h"
#include "TexManager.h"


CLight::CLight(void)
{
	m_bEnable				= TRUE;
	m_dwSetColor			= 0;
	m_dwLightColor.col  	= 0xFFFFFFFF;
	m_iLightCount			= 1;

	m_fScaleX				= 0.0f;
	m_fScaleY				= 0.0f;
}

CLight::~CLight(void)
{
}

void CLight::SetEnable(BOOL bEnable)
{
	m_bEnable = bEnable;
}

BOOL CLight::IsEnable(void)
{
	return m_bEnable;
}

void CLight::SetLightColor(DWORD dwColor)
{
	m_dwSetColor			= dwColor;
	m_dwLightColor.col  	= dwColor;

	A8R8G8B8 Color;
	Color.col   = dwColor;
	Color.r		= 0xFF - Color.r;
	Color.g		= 0xFF - Color.g;
	Color.b		= 0xFF - Color.b;
	m_vLightMap[0].dwColor = Color.col;
}

DWORD CLight::GetLightColor(void)
{
	return m_dwSetColor;
}

void CLight::CreateLight(void)
{
	m_iLightCount			= 1;

	m_fScaleX				= g_pEGfx->GetWidth() / 256.0f;
	m_fScaleY				= g_pEGfx->GetHeight() / 256.0f;

	// Ö÷¹âÔ´
	m_vLightMap[0].iX		= 0;
	m_vLightMap[0].iY		= 0;
	m_vLightMap[0].fScaleX	= m_fScaleX;
	m_vLightMap[0].fScaleY	= m_fScaleY;

	m_vLightMap[0].dwID		= MAKELONG(4,PACKAGE_effect);
	m_vLightMap[0].dwColor	= 0xFF404040;
	m_vLightMap[0].bCenter  = false;
	m_vLightMap[0].eMode    = RM_ADD2;
}

void CLight::DrawLight(int iX,int iY,DWORD dwID,float fScaleX,float fScaleY,DWORD dwColor)
{
	if(m_iLightCount >= MAX_LIGHTS)
		return;

	m_vLightMap[m_iLightCount].iX		= iX * 256 / g_pEGfx->GetWidth();
	m_vLightMap[m_iLightCount].iY		= iY * 256 / g_pEGfx->GetHeight();
	m_vLightMap[m_iLightCount].dwID		= dwID;
	m_vLightMap[m_iLightCount].fScaleX	= fScaleX * 256.0f / g_pEGfx->GetWidth();
	m_vLightMap[m_iLightCount].fScaleY	= fScaleY * 256.0f / g_pEGfx->GetHeight();
	m_vLightMap[m_iLightCount].dwColor	= dwColor;
	m_vLightMap[m_iLightCount].fRotZ    = 0.0f;
	m_vLightMap[m_iLightCount].eMode    = RM_ADD2;
	m_vLightMap[m_iLightCount].bCenter    = true;

	m_iLightCount++;
}

void CLight::DrawBLight(int iX,int iY,DWORD dwID,float fScaleX,float fScaleY,DWORD dwColor,float fRotZ)
{
	if(m_iLightCount >= MAX_LIGHTS)
		return;

	m_vLightMap[m_iLightCount].iX		= iX * 256 / g_pEGfx->GetWidth();
	m_vLightMap[m_iLightCount].iY		= iY * 256 / g_pEGfx->GetHeight();
	m_vLightMap[m_iLightCount].dwID		= dwID;
	m_vLightMap[m_iLightCount].fScaleX	= fScaleX * 256.0f / g_pEGfx->GetWidth();
	m_vLightMap[m_iLightCount].fScaleY	= fScaleY * 256.0f / g_pEGfx->GetHeight();
	m_vLightMap[m_iLightCount].dwColor	= dwColor;
	m_vLightMap[m_iLightCount].fRotZ    = fRotZ;
	m_vLightMap[m_iLightCount].eMode    = RM_ALPHA;
	m_vLightMap[m_iLightCount].bCenter  = false;

	m_iLightCount++;
}

void CLight::RenderLight(void)
{
	if(g_pEGfx->IsNoDraw() || !m_bEnable)
	{
		m_iLightCount = 1;
		return;
	}

	m_fScaleX				= g_pEGfx->GetWidth() / 256.0f;
	m_fScaleY				= g_pEGfx->GetHeight() / 256.0f;
	m_vLightMap[0].fScaleX	= m_fScaleX;
	m_vLightMap[0].fScaleY	= m_fScaleY;

	LPTexture pTex;
	if(g_pEGfx->m_pLight && g_pEGfx->m_pLightSurf)
	{
		g_pEGfx->m_pSprite->End();
		g_pEGfx->m_p3dDev->SetRenderTarget(0,g_pEGfx->m_pLightSurf);
		g_pEGfx->m_pSprite->Begin();

		g_pEGfx->ClearColor(m_dwLightColor.col);
		g_pEGfx->SetRenderMode(RM_LINEAR);

		pTex = g_pETexMgr->GetTexFromID(m_vLightMap[0].dwID,EP_NORMAL);
		if(pTex)
		{
			g_pEGfx->SetRenderMode(m_vLightMap[0].eMode);
			g_pEGfx->DrawTextureFX(128,128,pTex,m_vLightMap[0].dwColor);
		}

		for(int i = 1;i < m_iLightCount;i++)
		{
			pTex = g_pETexMgr->GetTexFromID(m_vLightMap[i].dwID,EP_NORMAL);
			if(!pTex) continue;

			int iX = m_vLightMap[i].iX;
			int iY = m_vLightMap[i].iY;
			if(m_vLightMap[i].bCenter)
			{
				iX += (int)(pTex->GetCenterX() * m_vLightMap[i].fScaleX);
				iY += (int)(pTex->GetCenterY() * m_vLightMap[i].fScaleY);
			}
			g_pEGfx->SetRenderMode(m_vLightMap[i].eMode);
			g_pEGfx->DrawTextureFX(iX,iY,pTex,m_vLightMap[i].dwColor,NULL,
				&POS(m_vLightMap[i].fScaleX,m_vLightMap[i].fScaleY),0.0f,0.0f,m_vLightMap[i].fRotZ);
		}

		g_pEGfx->SetRenderMode(RM_POINT);
		g_pEGfx->SetRenderMode();

		g_pEGfx->m_pSprite->End();
		g_pEGfx->m_p3dDev->SetRenderTarget(0,g_pEGfx->m_pBackSurf);
		g_pEGfx->m_pSprite->Begin();

		g_pEGfx->SetRenderMode(RM_MODULATE);

		g_pEGfx->SetRenderMode(RM_MODULATE);
		g_pEGfx->m_pSprite->Draw(g_pEGfx->m_pLight,NULL,&D3DXVECTOR2(m_fScaleX,m_fScaleY),NULL,0,NULL,0x66FFFFFF);
		g_pEGfx->SetRenderMode();
	}
	else
	{
		pTex = g_pETexMgr->GetTexFromID(m_vLightMap[0].dwID,EP_NORMAL);
		if(pTex)
		{
			g_pEGfx->SetRenderMode(RM_MODULATE);
			g_pEGfx->SetRenderMode(RM_LINEAR);

			g_pEGfx->DrawTextureFX(m_vLightMap[0].iX,m_vLightMap[0].iY,pTex,m_vLightMap[0].dwColor,NULL,
				&POS(m_vLightMap[0].fScaleX,m_vLightMap[0].fScaleY));

			g_pEGfx->SetRenderMode(RM_POINT);
			g_pEGfx->SetRenderMode();
		}
	}

	m_iLightCount = 1;
}
