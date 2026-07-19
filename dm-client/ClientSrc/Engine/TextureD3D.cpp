#include "stdafx.h"
#include "TextureD3D.h"
#include "TexManager.h"
#include "FontD3D.h"
#include "GraphicD3D.h"
#include "BaseClass/Compress/Compr.h"
#include "BaseClass/DXTn/DXTn.h"

BYTE s_ComprBuf[NML_POOL_SIZE] = {0};



PixelTable s_Table[TPT_TYPES] = 
{
	{TPT_A8R8G8B8	, 4},
	{TPT_A4R4G4B4	, 2},
	{TPT_A0R5G6B5	, 2},
	{TPT_A8L8		, 2},
	{TPT_DXT1		, 2},
	{TPT_DXT3		, 4},
	{TPT_DXT5		, 4},
};

PoolData    s_OldPool[OLD_POOL_TYPES];
PoolData	s_TexPool[TEX_POOL_TYPES];
PoolData	s_FontPool[FTST_NUMS];
BYTE		s_Buf[NML_POOL_SIZE];



void CTextureD3D::GetNewDiv(int iw,int ih,VInt& vW,VInt& vH)
{
	vW.clear();
	vH.clear();

	int i = 0,iNum,iOdd;
	if((iw - 1) % 128 >= 96 && (ih - 1) % 128 >= 96)	// 128*128切割
	{
		iNum = iw / 128;
		iOdd = iw % 128;
		for(i = 0;i < iNum;i++)
			vW.push_back(128);
		if(iOdd > 0)
			vW.push_back(iOdd);

		iNum = ih/ 128;
		iOdd = ih % 128;
		for(i = 0;i < iNum;i++)
			vH.push_back(128);
		if(iOdd > 0)
			vH.push_back(iOdd);
	}
	else												// 256*064切割
	{
		iNum = iw / 256;
		iOdd = iw % 256;
		for(i = 0;i < iNum;i++)
			vW.push_back(256);

		if(iOdd > 0 && iOdd >= 128)
		{
			iOdd -= 128;
			vW.push_back(128);
		}
		if(iOdd > 0)
			vW.push_back(iOdd);

		iNum = ih / 64;
		iOdd = ih % 64;
		for(i = 0;i < iNum;i++)
			vH.push_back(64);
		if(iOdd > 0)
			vH.push_back(iOdd);
	}
}

CTexBlock::CTexBlock(void)
{
	bCompr	= FALSE;
	ePixelType	= 0;
	wReserved	= 0;
	wWidth		= 0;
	wHeight		= 0;
	pPoolData	= NULL;
	dwSize		= 0;
	pData		= NULL;
}

CTexBlock::~CTexBlock(void)
{
	if(pPoolData)
		pPoolData->Pool.Release(this);
}


LPDIRECT3DTEXTURE9 CTexBlock::GetVideoTex(LPDIRECT3DDEVICE9 pDev)
{
	LPDIRECT3DTEXTURE9 pTex = GetSrc();
	
	//计数加1
	if(pPoolData)
	{
		pPoolData->Pool.MoveFront(pList);


		/*
		pPoolData->iCounter++;

		//如果同屏使用纹理数目较大,应至少保证同屏幕可以同时显示的纹理块数
		//没有创建失败，如果创建失败了不作此处理		
		if(!pPoolData->bCreateFail && pPoolData->iCounter > pPoolData->Pool.GetSrcNum())
		{
			if(pPoolData->iCounter > pPoolData->iMaxSize)
				pPoolData->iMaxSize = pPoolData->iCounter;

			pPoolData->bCanCreate = TRUE;
			pTex = NULL;
		}	
		*/

		if (!pTex && !pPoolData->bCreateFail 
			&& pPoolData->Pool.GetSrcNum() == pPoolData->iMaxSize)
		{
			if (pPoolData->Pool.GetEndIsInCur())
			{
				++pPoolData->iMaxSize;
				pPoolData->bCanCreate = TRUE;
			}
		}
	}
	

	if(!pTex && pPoolData)
	{
		if(pPoolData->bCanCreate && pPoolData->Pool.GetSrcNum() < pPoolData->iMaxSize)
		{
			LPD3DTEX pD3DTex = NULL;
			HRESULT hr = pDev->CreateTexture(pPoolData->iWidth,pPoolData->iHeight,1,0,pPoolData->D3DFormat,pPoolData->PoolType,&pD3DTex,NULL);
			if(hr == D3D_OK && pD3DTex)
			{
				pPoolData->Pool.AddSrc(pD3DTex);
				if(pPoolData->D3DFormat == D3DFMT_DXT3)
					g_pEGfx->GetAlloacMem() += pPoolData->iWidth * pPoolData->iHeight;
				else
					g_pEGfx->GetAlloacMem() += pPoolData->iWidth * pPoolData->iHeight * 2;

				if(g_pEGfx->GetAlloacMem() >= g_pEGfx->GetShouldMem())
					pPoolData->bCanCreate = FALSE;
			}
			else
			{
				pPoolData->bCanCreate = FALSE;
				pPoolData->bCreateFail = TRUE;
			}
		}

		if(pPoolData->Pool.GetSrc(this) > NotEnoughSrc && (pTex = GetSrc()))
		{
			BYTE *pBuf	= pData;
			if(bCompr)
			{
				DWORD dwDestSize = NML_POOL_SIZE;
				Uncompress(s_ComprBuf,&dwDestSize,pData,dwSize);
				pBuf = s_ComprBuf;
			}

			D3DLOCKED_RECT lr = {0,NULL};
			if(pPoolData->PoolType == D3DPOOL_MANAGED)
				pTex->LockRect(0,&lr,NULL,0);
			else if(pPoolData->pMemTex)
				pPoolData->pMemTex->LockRect(0,&lr,NULL,0);

			if(lr.Pitch == 0 || !lr.pBits)
				return NULL;

			if(pPoolData->D3DFormat == D3DFMT_DXT3)
				Convert(TPT_DXT3,(BYTE *)lr.pBits,lr.Pitch,pPoolData->iHeight,ePixelType,pBuf,wWidth,wHeight);
			else if(pPoolData->D3DFormat == D3DFMT_A4R4G4B4)
				Convert(TPT_A4R4G4B4,(BYTE *)lr.pBits,lr.Pitch,pPoolData->iHeight,ePixelType,pBuf,wWidth,wHeight);
			else
				Convert(TPT_A8R8G8B8,(BYTE *)lr.pBits,lr.Pitch,pPoolData->iHeight,ePixelType,pBuf,wWidth,wHeight);

			if(pPoolData->PoolType == D3DPOOL_MANAGED)
				pTex->UnlockRect(0);
			else if(pPoolData->pMemTex)
			{
				pPoolData->pMemTex->UnlockRect(0);
				pDev->UpdateTexture(pPoolData->pMemTex,pTex);
			}
		}
	}
	return pTex;
}

CTextureD3D::CTextureD3D(void)
{
	m_dwOption		= 0;
	m_dwFrames		= 0;
	m_dwSpeed		= 0;

	m_dwID			= 0;
	m_dwPixelSize	= 0;
	m_dwFrameNow	= 0;
	m_bSysAnim		= TRUE;

	m_vFrame.clear();
	m_pCurFrame		= NULL;
}

CTextureD3D::~CTextureD3D(void)
{
	m_vFrame.clear();
}

BOOL CTextureD3D::AddFrame(int w,int h,BYTE* pData,int iPitch)
{
	if(!pData || w == 0 || h == 0)
		return FALSE;

	BYTE* pSrc = pData;

	int x0		= 0;
	int y0		= 0;
	int x1		= w-1;
	int y1		= h-1;

	int iW = x1 - x0 + 1;
	int iH = y1 - y0 + 1;

	//切割
	VInt vW0,vH0;
	GetNewDiv(iW,iH,vW0,vH0);

	CTexFrame	frame;
	frame.wWidth0   = w;
	frame.wHeight0	= h;
	frame.wWidth	= iW;
	frame.wHeight	= iH;
	frame.wCenterX	= 0;
	frame.wCenterY	= 0;
	frame.wOffX		= x0;
	frame.wOffY		= y0;
	frame.wXBlocks	= (WORD)vW0.size();
	frame.wYBlocks	= (WORD)vH0.size();

	if(iPitch == 0)
		iPitch = w * sizeof(A8R8G8B8);

	VInt vW1,vH1;
	GetNewDiv(frame.wWidth,frame.wHeight,vW1,vH1);

	DWORD dwSize = frame.wXBlocks * frame.wYBlocks;
	frame.vBlocks	= new CTexBlock[dwSize];
	if(!frame.vBlocks)
		return FALSE;

	int w0,h0,w1,h1,w2,h2;
	h2 = y0;

	for(int j = 0;j < frame.wYBlocks;j++)
	{
		w2 = x0;
		for(int i = 0;i < frame.wXBlocks;i++)
		{
			CTexBlock& block = frame.vBlocks[frame.wXBlocks * j + i];

			w0 = vW0[i];
			h0 = vH0[j];
			w1 = vW1[i];
			h1 = vH1[j];

			// 取出块像素数据,设置Block的头
			block.bCompr     = FALSE;
			block.ePixelType = TPT_A4R4G4B4;
			block.wReserved  = 0;
			block.wWidth = w0;
			block.wHeight = h0;
			block.dwSize = w0 * h0 * sizeof(A4R4G4B4);

			block.pData	= g_pETexMgr->GetTexMemPool().NewTexMem(block.dwSize);
			if(block.pData)
			{
				for(int n = 0; n < h0;n++)
				{
					A4R4G4B4* dst = ((A4R4G4B4*)block.pData) + n * w1;
					A8R8G8B8* src = ((A8R8G8B8*)(pSrc + (h2 + n) * iPitch)) + w2;

					for(int m = 0; m < w0;m++)
					{
						dst->r = src->r >> 4;
						dst->g = src->g >> 4;
						dst->b = src->b >> 4;
						dst->a = src->a >> 4;
						dst++,src++;
					}
				}
			}
			m_dwPixelSize += block.dwSize;

			//处理PoolData
			if(block.ePixelType == TPT_A4R4G4B4) //处理使用A4R4G4B4的纹理块
			{
				FontTexSizeType ePool = PoolData::GetFontPool(block.wWidth,block.wHeight);
				if(ePool != FTST_NULL)
					block.pPoolData	= &(s_FontPool[ePool]);
				else
					block.pPoolData	= NULL;
			}
			w2 += w0;
		}
		h2 += h0;
	}
	m_vFrame.push_back(frame);
	m_dwFrames = (DWORD)m_vFrame.size();
	m_pCurFrame = &m_vFrame.back();
	return TRUE;
}

BOOL CTextureD3D::CreateFontTex(int iw,int ih,BYTE *pBuf)
{
	if(m_vFrame.size() > 0)
		return TRUE;

	m_dwPixelSize	= 0;
	m_dwFrames		= 1;

	CTexFrame frame;
	memset(&frame,0,sizeof(CTexFrame));

	frame.wWidth0	= iw;
	frame.wHeight0	= ih;
	frame.wWidth	= iw;
	frame.wHeight	= ih;
	frame.wOffX		= 0;
	frame.wOffY		= 0;
	frame.wCenterX	= 0;
	frame.wCenterY	= 0;
	frame.wXBlocks	= (iw + 127) / 128;
	frame.wYBlocks	= 1;
	frame.vBlocks	= new CTexBlock[frame.wXBlocks];
	if(!frame.vBlocks)
		return FALSE;

	int iW = 0;
	for(int i = 0;i < frame.wXBlocks;i++)
	{
		CTexBlock& block = frame.vBlocks[i];

		block.bCompr		= 0;
		block.ePixelType	= TPT_A8R8G8B8;
		block.wReserved		= 0;
		block.wWidth		= (i == (frame.wXBlocks - 1)) ? ((iw - 1) % 128) + 1 : 128;
		block.wHeight		= ih;
		block.dwSize		= block.wWidth * block.wHeight * s_Table[block.ePixelType].dwPerPitchs;
		if(block.wHeight <= 16)
			block.pData		= g_pEFont->m_FixPool16.NewEle();
		else if(block.wHeight <= 32)
			block.pData		= g_pEFont->m_FixPool32.NewEle();
		else if(block.wHeight <= 64)
			block.pData		= g_pEFont->m_FixPool64.NewEle();
		else if(block.wHeight <= 256)
			block.pData		= g_pEFont->m_FixPool256.NewEle();

		if(block.pData)
		{
			for(int h = 0;h < ih;h++)
				memcpy(block.pData + h * block.wWidth * s_Table[block.ePixelType].dwPerPitchs,pBuf + (h * iw + iW) * s_Table[block.ePixelType].dwPerPitchs,block.wWidth * s_Table[block.ePixelType].dwPerPitchs);
		}

		FontTexSizeType ePool = PoolData::GetFontPool(block.wWidth,block.wHeight);
		if(ePool != FTST_NULL)
			block.pPoolData	= &(s_FontPool[ePool]);
		else
			block.pPoolData	= NULL;

		m_dwPixelSize += block.dwSize;

		iW += block.wWidth;
	}
	m_vFrame.push_back(frame);
	m_pCurFrame = &m_vFrame[0];
	return TRUE;
}

void CTextureD3D::ReleaseFontTex(void)
{
	for(unsigned int ui = 0; ui < m_vFrame.size();ui++)
	{
		CTexFrame& frame = m_vFrame[ui];

		for(int i = 0;i < frame.wXBlocks;i++)
		{
			CTexBlock& block = frame.vBlocks[i];

			if(block.wHeight <= 16)
				g_pEFont->m_FixPool16.DelEle(block.pData);
			else if(block.wHeight <= 32)
				g_pEFont->m_FixPool32.DelEle(block.pData);
			else if(block.wHeight <= 64)
				g_pEFont->m_FixPool64.DelEle(block.pData);
		}

		SAFE_DELETE_ARRAY(frame.vBlocks);
		m_vFrame.clear();
	}

	m_dwFrames		= 0;
	m_dwPixelSize	= 0;
	m_pCurFrame		= NULL;
}

void CTextureD3D::UpdateFrame(DWORD dwCunt)
{
	if(m_bSysAnim && m_dwFrames > 1)
	{
		if(m_dwSpeed <= 0)
			m_dwSpeed = 1;

		DWORD dwTemp = (dwCunt / (m_dwSpeed * 2)) % m_dwFrames;
		if(m_dwFrameNow != dwTemp)
			SetCurFrame(dwTemp); 
	}
}

void CTextureD3D::SetCurFrame(DWORD dwFrame)
{
	if(dwFrame >= m_dwFrames)
		dwFrame = 0;

	m_dwFrameNow	= dwFrame;
	m_pCurFrame		= &m_vFrame[dwFrame];
}

void CTextureD3D::SetFrames(DWORD dwFrames)
{
	if (dwFrames <= 0)
	{
		return;
	}

	m_dwFrames = dwFrames;
	m_vFrame.resize(m_dwFrames);
}

CTexFrame*  CTextureD3D::GetFrame(DWORD dwFrame)
{
	if(dwFrame >= m_dwFrames)
		return NULL;

	return &(m_vFrame[dwFrame]);
}

void CTextureD3D::NextFrame(void)
{
	SetCurFrame(m_dwFrameNow + 1);
}

int CTextureD3D::IsPointInTex(int ix,int iy,float fScaleX,float fScaleY)
{
	if(!m_pCurFrame)
		return 0;

	ix = (int)(ix / fScaleX);
	iy = (int)(iy / fScaleY);

	ix += m_pCurFrame->wCenterX - m_pCurFrame->wOffX;
	iy += m_pCurFrame->wCenterY - m_pCurFrame->wOffY;

	if(ix < 0 || iy < 0 || ix > m_pCurFrame->wWidth || iy > m_pCurFrame->wHeight)
		return 0;

	TEX_FRAME&  frame	= *m_pCurFrame;
	int			x0		= 0;
	int			y0		= 0;
	for(int j = 0;j < frame.wYBlocks;j++)
	{
		x0 = 0;
		for(int i = 0;i < frame.wXBlocks;i++)
		{
			TEX_BLOCK &block = frame.vBlocks[j * frame.wXBlocks + i];

			if(ix >= x0 && ix < x0 + block.wWidth && iy >= y0 && iy < y0 + block.wHeight)
			{
				switch(block.ePixelType)
				{
				case TPT_A4R4G4B4:
				case TPT_DXT3:
					{
						BYTE *pBuf = block.pData;
						if(block.bCompr)
						{
							DWORD dwDestSize = NML_POOL_SIZE;
							Uncompress(s_ComprBuf,&dwDestSize,block.pData,block.dwSize);
							pBuf = s_ComprBuf;
						}
						if(block.ePixelType == TPT_DXT3)
						{
							Convert(TPT_A4R4G4B4,s_Buf,block.wWidth * 2,block.wHeight,TPT_DXT3,pBuf,block.wWidth,block.wHeight);
							pBuf = s_Buf;
						}
						WORD *pA4R4G4B4 = (WORD *)pBuf;
						if(pA4R4G4B4[(iy - y0) * block.wWidth + (ix - x0)] & 0xF000)
							return 2;
						return 1;
					}
				default:
					{
						return 2;
					}
				}
			}
			x0 += block.wWidth;
		}
		y0 += frame.vBlocks[j * frame.wXBlocks].wHeight;
	}
	return 1;
}
	
OldTexSizeType PoolData::GetOldPool(int w,int h)
{
	for(int i = 0;i < OLD_POOL_TYPES;i++)
	{
		if(w <= s_OldPool[i].iWidth && h <= s_OldPool[i].iHeight)
			return (OldTexSizeType)i;
	}
	return OLD_POOL_NULL;
}

TexSizeType PoolData::GetTexPool(int w,int h)
{
	for(int i = 0;i < TEX_POOL_TYPES;i++)
	{
		if(w <= s_TexPool[i].iWidth && h <= s_TexPool[i].iHeight)
			return (TexSizeType)i;
	}
	return TEX_POOL_NULL;
}

FontTexSizeType PoolData::GetFontPool(int w,int h)
{
	for(int i = 0;i < FTST_NUMS;i++)
	{
		if(w <= s_FontPool[i].iWidth && h <= s_FontPool[i].iHeight)
			return (FontTexSizeType)i;
	}
	return FTST_NULL;
}

void PoolData::ResetCounter()
{
	for(int i = 0;i < TEX_POOL_TYPES;i++)
		s_TexPool[i].iCounter = 0;

	for(int i = 0;i < OLD_POOL_TYPES;i++)
		s_OldPool[i].iCounter = 0;

	for(int i = 0;i < FTST_NUMS;i++)
		s_FontPool[i].iCounter = 0;
}
