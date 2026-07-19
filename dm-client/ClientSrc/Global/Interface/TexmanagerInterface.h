#pragma once

#include "TextureInterface.h"
#include "Engine/TexPool.h"
#include "../include/LufoList.h"

#define	PACKAGE_BEGIN		enum{
#define	PACKAGE(lib,num)	PACKAGE_##lib = num,
#define	PACKAGE_END			};
#include "../../Engine/SglDefine.inl"

enum eTexType
{
	TEX_1555 =      0x01,		// 1555格式
	TEX_4444 =		0x02,		// 4444格式
	TEX_MUST1555=	0x04,		// 必须使用1555格式渲染
	TEX_RLE		=   0x10,		// 行压缩格式
	TEX_S3TC	=   0x20,		// 使用S3TC纹理压缩
	TEX_S3TCALPHA = 0x40,       // 使用S3TC纹理压缩且带Alpha数据块
};


// 缓冲模式读取结果
enum eReadResult
{
	RR_NOFILE		    = 0,	// 没有该纹理
	RR_READING		    = 1,	// 读取线程正在读取
	RR_DOWNLOADING		= 2,	// 下载中
	RR_SUCCESS		    = 3,	// 读取成功(已经在缓冲里面)
};

class CTexPool;

class CTexManagerInterface
{	
public:
	virtual ~CTexManagerInterface(){}
	virtual LPTexture   LoadImage(const char* path) = 0;    // 导入其他格式文件，使用FreeImage

	// 非缓冲模式相关
	//virtual LPTexture   LoadTex(int iPack,int iIdx,eReadPrior ePrior) = 0;    // 根据图包编号和图片编号,外部请用GetTexImm,因为图片可能不存在,要去资源服务器下载
	//virtual LPTexture   LoadTex(FILE* fp) = 0;              // 从文件指针读取
	//virtual LPTexture	LoadTex(LPCTSTR sTexFile,eReadPrior ePrior) = 0;		// 根据文件名读取纹理,因为图片可能不存在,要去资源服务器下载
	virtual LPTexture	LoadTexFromDiskFile(LPCTSTR sTexFile,eReadPrior ePrior) = 0;// 根据文件名读取纹理,不从图包中读取,而是从磁盘文件中读取
	//virtual LPTexture	LoadTex(DWORD dwID,eReadPrior ePrior) = 0;		    // 根据编号读取纹理(优先读取sgl文件中，没有的话读取文件夹中的),因为图片可能不存在,要去资源服务器下载
	virtual LPTexture   LoadMemTex(int iW,int iH,BYTE* pBuf,int iPitch = 0) = 0;
	virtual void		ReleaseTex(LPTexture& pTex) = 0;	// 释放纹理(上面两种加载的方式都需要调用本函数释放，否者会有内存泄漏)
	virtual void        ClearAllTex() = 0;
	virtual void        ClearAllNullTex() = 0;

	// 缓冲模式相关
	virtual LPTexture	GetTex(int iPack,int iIdx,eReadPrior ePrior) = 0;				// 根据图库包，还有索引读取纹理，缓冲模式取得纹理，可以指定读取的优先等级
	virtual LPTexture	GetTexImm(int iPack,int iIdx,eReadPrior ePrior,int *pIResult = NULL) = 0;											// 根据图库包，还有索引读取纹理，缓冲模式立即取得纹理
	virtual LPTexture	GetTexImm(DWORD dwID,eReadPrior ePrior,int *pIResult = NULL) = 0;											        // 根据ID，缓冲模式立即取得纹理
	virtual LPTexture	GetTexFromID(DWORD dwID,eReadPrior ePrior,int *pIResult = NULL) = 0;				// 根据ID，缓冲模式取得纹理，可以指定读取的优先等级
	virtual bool        HasTexInServer(DWORD dwID) = 0;//纹理在服务器是否存在
	virtual bool        HasTexInServer(int iPack,int iIdx) = 0;//纹理在服务器是否存在
	virtual bool        HasTexInLocalWpf(DWORD dwID) = 0;//纹理在本地是否存在
	virtual bool        HasTexInLocalWpf(int iPack,int iIdx) = 0;//纹理在本地是否存在
	virtual bool        GetTexFilePath(int iPack,int iIdx,char *szPath) = 0;//根据纹理编号得到文件路径,外部保存szPath足够长

	virtual void		PreLoad(DWORD dwBeginID,DWORD dwEndID,eReadPrior ePrior) = 0;	// 根据ID预读纹理
	virtual void        PreLoad(int iPack,int iBeginID,int iEndID,eReadPrior ePrior) = 0;// 根据ID预读纹理
	virtual void		PreLoadImm(DWORD dwBeginID,DWORD dwEndID,eReadPrior ePrior) = 0;	// 根据ID预读纹理
	virtual void        PreLoadImm(int iPack,int iBeginID,int iEndID,eReadPrior ePrior) = 0;// 根据ID预读纹理
	virtual void        DelTexFile(int iPack,int iBeginID,int iEndID) = 0;// 根据ID删除对应的图片,删除魔法相关的图片时用到
	virtual int			GetReadingTexNum(void) = 0;			// 取得等待线程读取的纹理数
	virtual eReadResult	GetLastReadStatus(void) = 0;		// 取得最近一次缓冲模式纹理读取的结果

	virtual void        SetMaxCacheSize(int i) = 0;
	virtual __int64     GetCurCacheSize() = 0;
	virtual __int64     GetMaxCacheSize() = 0;
	virtual __int64     GetCurFixedSize() = 0;
	virtual __int64     GetMaxFixedSize() = 0;
	virtual bool        IsValidOldTex(UCHAR * pTex,int iLen) = 0;
	virtual void        OnDownloadTex(DWORD dwID,const char* buf,DWORD dwLen) = 0;//有纹理通过下载服务器下载到本地了
	virtual void        OnSendDownloadTex(DWORD dwID) = 0;//准备下载dwID,更新纹理池状态
	virtual void		PreDownLoadTex(int iPack,int iBeginID,int iEndID,eReadPrior ePrior) = 0;	// 根据ID预下载纹理

};

extern CTexManagerInterface *g_pTexMgr;
