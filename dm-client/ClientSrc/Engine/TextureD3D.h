#pragma once

#include "Define.h"
#include "Global/Interface/TextureInterface.h"

// A8R8G8B8格式
union A8R8G8B8
{
	struct
	{
		BYTE b;
		BYTE g;
		BYTE r;
		BYTE a;
	};
	DWORD	col;
};


union A0R8G8B8
{
	struct
	{
		BYTE b;
		BYTE g;
		BYTE r;
	};
	DWORD	col;
};


// A4R4G4B4格式
union A4R4G4B4
{
	struct
	{
		WORD b:4;
		WORD g:4;
		WORD r:4;
		WORD a:4;
	};
	WORD	col;
};

// A0R5G6B5格式
union A0R5G6B5
{
	struct
	{
		WORD b:5;
		WORD g:6;
		WORD r:5;
	};
	WORD	col;
};

struct A8L8
{
	BYTE a;
	BYTE l;
};

// 纹理格式类型
enum TexPixelType
{
	TPT_A8R8G8B8 = 0,
	TPT_A4R4G4B4	,
	TPT_A0R5G6B5	,
	TPT_A8L8		,
	TPT_DXT1		,
	TPT_DXT3		,
	TPT_DXT5		,

	TPT_TYPES		,
};

// 像素格式索引表
struct PixelTable
{
	TexPixelType D3DFormat;
	DWORD		 dwPerPitchs;
};

// 纹理池大小类型
enum TexSizeType
{
	TEX_POOL_NULL	= -1,

	TEX_POOL_032_032,
	TEX_POOL_032_064	,
	TEX_POOL_032_128	,
	TEX_POOL_032_256	,
	TEX_POOL_064_032	,
	TEX_POOL_064_064	,
	TEX_POOL_064_128	,
	TEX_POOL_064_256	,
	TEX_POOL_128_128	,
	TEX_POOL_256_256	,
	TEX_POOL_512_512	,

	TEX_POOL_TYPES		,
};

enum OldTexSizeType
{
	OLD_POOL_NULL   = -1,
	OLD_POOL_032_032,
	OLD_POOL_064_064,
	OLD_POOL_064_256,
	OLD_POOL_128_016,
	OLD_POOL_128_064,
	OLD_POOL_128_128,
	OLD_POOL_256_016,
	OLD_POOL_256_064,
	OLD_POOL_256_128,
	OLD_POOL_256_256,
	OLD_POOL_512_512,

	OLD_POOL_TYPES,
};


// 字体纹理类型
enum FontTexSizeType
{
	FTST_NULL		= -1,
	FTST_032_032		,
	FTST_064_064		,
	FTST_128_016		,
	FTST_128_032		,
	FTST_128_064		,
	FTST_128_128        ,
	FTST_256_064        ,

	FTST_NUMS			,
};


// 纹理池数据结构
struct PoolData
{
	BOOL							bCanCreate;		// 是否可以再创建
	int								iMaxSize;		// 最大缓冲数
	int                             iCounter;       // 计数
	BOOL                            bCreateFail;    // 创建失败

	int								iWidth;			// 宽度
	int								iHeight;		// 高度
	D3DPOOL							PoolType;		// D3D池类型
	D3DFORMAT						D3DFormat;		// 纹理格式
	LPDIRECT3DTEXTURE9				pMemTex;		// 系统内存更换纹理
	CFrameCache<IDirect3DTexture9>	Pool;			// 所有显存缓冲纹理

	static OldTexSizeType PoolData::GetOldPool(int w,int h);
	static TexSizeType PoolData::GetTexPool(int w,int h);
	static FontTexSizeType PoolData::GetFontPool(int w,int h);
	static void PoolData::ResetCounter();

};

// 帧块数据
class CTexBlock : public CFrameCache<IDirect3DTexture9>::CUser
{
public:

	CTexBlock(void);
	~CTexBlock(void);

	virtual LPDIRECT3DTEXTURE9 GetVideoTex(LPDIRECT3DDEVICE9 pDev);

	// 块属性
	BYTE			bCompr;		// 是否压缩
	BYTE			ePixelType;	// 纹理格式
	WORD			wReserved;	// 保留

	// 其他
	WORD			wWidth;		// 块宽度
	WORD			wHeight;	// 块高度
	DWORD			dwSize;		// 数据字节数
	BYTE *			pData;		// 具体数据

	PoolData *		pPoolData;	// 池指针
};

// 纹理帧数据
class CTexFrame
{
public:
	CTexFrame()
	{
		memset(this,0,sizeof(CTexFrame));
	}
	WORD		wWidth0;		// 原始宽度
	WORD		wHeight0;		// 原始高度
	WORD		wWidth;			// 宽度
	WORD		wHeight;		// 高度
	SHORT		wOffX;			// X向偏移
	SHORT		wOffY;			// Y向偏移
	SHORT		wCenterX;		// 中心点X
	SHORT		wCenterY;		// 中心点Y
	WORD		wXBlocks;		// X向块数
	WORD		wYBlocks;		// Y向块数
	CTexBlock*	vBlocks;		// 块数据
};

typedef std::vector<CTexFrame> VTexFrame;
typedef CTexBlock	TEX_BLOCK;
typedef CTexFrame	TEX_FRAME;

// 纹理定义
class CTextureD3D : public CTextureInterface
{
	friend class CFontD3D;
	friend class CGraphicD3D;
	friend class CTexManager;

public:

	DWORD		m_dwOption;			// 纹理属性
	DWORD		m_dwFrames;			// 帧数
	DWORD		m_dwSpeed;			// 播放速度

	DWORD		m_dwID;				// 纹理ID
	DWORD		m_dwPixelSize;		// 像素数据大小
	DWORD		m_dwFrameNow;		// 当前帧
	BOOL		m_bSysAnim;			// 是否使用系统动画

	VTexFrame	m_vFrame;			// 帧数组
	CTexFrame*	m_pCurFrame;		// 当前帧指针


	// 添加一个32位纹理帧
	void        GetNewDiv(int iw,int ih,VInt& vW,VInt& vH);
	BOOL        AddFrame(int iw,int ih,BYTE* pData,int iPitch = 0);

	// 字体纹理的创建和销毁
	BOOL		CreateFontTex(int iw,int ih,BYTE *pBuf);
	void		ReleaseFontTex(void);

public:
	CTextureD3D(void);
	~CTextureD3D(void);



	DWORD		GetOption(void);		        // 纹理属性
	void		SetOption(DWORD dwOption);		// 纹理属性
	DWORD		GetFrames(void);		        // 纹理帧数
	void		SetFrames(DWORD dwFrames);      // 纹理帧数
	DWORD		GetSpeed(void);			        // 纹理速度
	void		SetSpeed(DWORD dwSpeed);        // 纹理速度
	DWORD		GetID(void);			        // 纹理ID
	void		SetID(DWORD dwID);              // 设置数据大小
	DWORD		GetFrameNow(void);		        // 当前纹理帧索引

	int			GetWidth0(void);		// 当前帧原始宽度
	int			GetHeight0(void);		// 当前帧原始高度
	int			GetWidth(void);			// 当前帧宽度
	int			GetHeight(void);		// 当前帧高度
	int			GetOffX(void);			// 当前帧偏移X
	int			GetOffY(void);			// 当前帧偏移Y
	int			GetCenterX(void);		// 当前帧中心点X
	int			GetCenterY(void);		// 当前帧中心点Y

	void		EnableSysAnim(BOOL b);						// 设置是否自动显示动画纹理
	void		SetOff(int iOffX,int iOffY);				// 设置当前帧偏移
	void		SetCenter(int iCenterX,int iCenterY);		// 设置当前帧中心点

	void		UpdateFrame(DWORD dwCunt);					// 更新纹理帧动画的计数基数
	void		SetCurFrame(DWORD dwFrame);					// 设置当前帧
	void		NextFrame(void);							// 当前帧的下一帧

	CTexFrame*  GetCurFrame();					            // 获得当前帧
	CTexFrame*  GetFrame(DWORD dwFrame);		            // 获得指定帧
	VTexFrame&  GetVFrame();					            // 获得帧数组
	void        ClearFrame();		                        // 清空定帧
	void		SetPixelSize(DWORD dwSize);				    // 设置数据大小
	DWORD		GetPixelSize();				                // 获得数据大小


	BOOL		IsOption(eOptionFlag eOF);
	int 		IsPointInTex(int ix,int iy,float fScaleX = 1.0f,float fScaleY = 1.0f);		// 判断点是否在纹理区域
};

typedef CTextureD3D*		LPETexture;


#include "TextureD3D.inl"