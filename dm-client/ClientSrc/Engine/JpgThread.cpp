#include "stdafx.h"
#include "GraphicD3D.h"
#include "Global/include/ijl.h"
#include "Global/Interface/Callbackinterface.h"
#include "FontD3D.h"

//#pragma comment(lib,"ijl15l.lib")

// 存储jpg线程相关的定义
CGraphicD3D::JpgSaveInfo	CGraphicD3D::m_JSI;
HANDLE						CGraphicD3D::m_hThread		= NULL;		// 线程句柄
BOOL						CGraphicD3D::m_bSaving		= FALSE;	// 是否正在保存
BOOL						CGraphicD3D::m_bExited		= FALSE;	// 是否结束线程
BYTE*						CGraphicD3D::m_pThreadBuf	= NULL;		// 线程buffer

// 保存为jpg
BOOL CGraphicD3D::SaveSnapShot(LPCTSTR sJpgFile,LPRECT pRect)
{
	//w
	// 查看表面是否正常
	if(!m_pBackSurf || !m_pMemSurf)
		return FALSE;

	// 查看线程是否已经创建
	if(!m_hThread)
	{
		DWORD dwThreadID;
		m_hThread = CreateThread(NULL,0,ThreadSaveJpg,NULL,CREATE_SUSPENDED,&dwThreadID);
		if(!m_hThread)
			return FALSE;
	}

	// 上次保存没结束，不允许本次保存
	if(m_bSaving)
	{
		ResumeThread(m_hThread);
		return FALSE;
	}

	// 解除锁定
	if(m_bMemSurf)
	{
		m_pMemSurf->UnlockRect();
		m_bMemSurf = FALSE;
	}

	this->BeginDraw();
	//DrawRectangleWithFrame(0xFF000000,0x40000000,0,0,160,60);

	if (g_pCallBack)
	{
		char temp[256] = {0};
		// 版本号
		sprintf(temp,"V%s - Build%04d",g_pCallBack->GetVersion(),g_pCallBack->GetBuildID());
		g_pEFont->DrawText(2,2,temp,0xFFFFFFFF,FONT_DEFAULT,FONTSIZE_DEFAULT,DTF_BlackFrame);
		// 组名，角色名
		/*sprintf(temp,"%s  %s",g_Login.GetGroupName(),g_OtherData.GetCharName());
		g_pEFont->DrawText(2,16,temp,0xFFFFFFFF,FONTSIZE_DEFAULT,DTF_BlackFrame);*/
		// 日期
		/*sprintf(temp,"日  期:  %s",g_pGlobalParam->GetTimer()->GetDateTime("%Y/%m/%d"));
		g_pEFont->DrawText(2,30,temp,0xFFFFFFFF,FONTSIZE_DEFAULT,DTF_BlackFrame);*/
		// 时间
		/*sprintf(temp,"时  间:  %s",g_pGlobalParam->GetTimer()->GetDateTime("%H:%M:%S"));
		g_pEFont->DrawText(2,44,temp,0xFFFFFFFF,FONTSIZE_DEFAULT,DTF_BlackFrame);*/
	}

	this->EndDraw();

	// 保存背景到后备缓冲中
	hr = m_p3dDev->GetRenderTargetData(m_pBackSurf,m_pMemSurf);

	if(hr != D3D_OK)
		return FALSE;

	// 设置保存的参数
	m_JSI.b32Bits = (m_d3dpp.BackBufferFormat == D3DFMT_X8R8G8B8) ? TRUE : FALSE;
	strcpy(m_JSI.sJpgFile,sJpgFile);
	if(pRect)
		memcpy(&m_JSI.rc,pRect,sizeof(RECT));
	else
	{
		m_JSI.rc.left = m_JSI.rc.top = 0;
		m_JSI.rc.right	= m_iWidth;
		m_JSI.rc.bottom	= m_iHeight;
	}
	m_pMemSurf->LockRect(&m_JSI.lr,pRect,D3DLOCK_READONLY);
	m_bMemSurf = TRUE;

	ResumeThread(m_hThread);
	return TRUE;
}

// 线程保存jpg函数
DWORD WINAPI CGraphicD3D::ThreadSaveJpg(LPVOID lpData)
{
	while(TRUE)
	{
		if(m_bExited)
			break;
		if(m_JSI.sJpgFile[0] != 0 && m_pThreadBuf)
		{
			m_bSaving = TRUE;

			JPEG_CORE_PROPERTIES image;
			if(ijlInit(&image) == IJL_OK)
			{
				struct R8G8B8
				{
					BYTE r,g,b;
				};
				struct A8R8G8B8
				{
					BYTE b;
					BYTE g;
					BYTE r;
					BYTE a;
				};
				struct A0R5G6B5
				{
					WORD b:5;
					WORD g:6;
					WORD r:5;
				};

				int width	= m_JSI.rc.right - m_JSI.rc.left;
				int height	= m_JSI.rc.bottom - m_JSI.rc.top;

				R8G8B8 *pData = (R8G8B8 *)m_pThreadBuf;
				R8G8B8 *pDest = pData;
				if(m_JSI.b32Bits)		// 32位色
				{
					A8R8G8B8 *pSrc = (A8R8G8B8 *)m_JSI.lr.pBits;
					for(int j = 0;j < height;j++)
					{
						for(int i = 0;i < width;i++)
						{
							pDest->r = pSrc->r;
							pDest->g = pSrc->g;
							pDest->b = pSrc->b;

							pSrc++;
							pDest++;
						}
						BYTE *p = (BYTE *)pSrc;
						p += (m_JSI.lr.Pitch - width * sizeof(A8R8G8B8));
						pSrc = (A8R8G8B8 *)p;
					}
				}
				else					// 16位色
				{
					A0R5G6B5 *pSrc = (A0R5G6B5 *)m_JSI.lr.pBits;
					for(int j = 0;j < height;j++)
					{
						for(int i = 0;i < width;i++)
						{
							pDest->r = ((BYTE)pSrc->r)<<3;
							pDest->g = ((BYTE)pSrc->g)<<2;
							pDest->b = ((BYTE)pSrc->b)<<3;

							pSrc++;
							pDest++;
						}
						BYTE *p = (BYTE *)pSrc;
						p += (m_JSI.lr.Pitch - width * sizeof(A0R5G6B5));
						pSrc = (A0R5G6B5 *)p;
					}
				}

				// 原图设置
				image.DIBBytes			= (BYTE *)pData;
				image.DIBColor			= IJL_RGB;
				image.DIBWidth			= width;
				image.DIBHeight			= height;
				image.DIBChannels		= 3;

				// 保存后jpg设置
				image.JPGFile			= m_JSI.sJpgFile;
				image.JPGColor			= IJL_YCBCR;
				image.JPGWidth			= width;
				image.JPGHeight			= height;
				image.JPGChannels		= 3;
				image.JPGSubsampling	= IJL_411;
				image.jquality			= 95;

				ijlWrite(&image,IJL_JFILE_WRITEWHOLEIMAGE);
				ijlFree(&image);
			}
			m_JSI.sJpgFile[0] = 0;
		}

		if(m_bExited)
			break;

		m_bSaving = FALSE;

		SuspendThread(m_hThread);
	}

	m_bSaving = FALSE;

	return 0;
}