#pragma once

#include <D3d9.h>
#include <vector>
#include "../include/FrameCache.h"
#include "Global/Globaldefine.h"


enum eOptionFlag
{
	OF_CHAR_FRAME	= 0x00000001,	// 非系统控制帧速
	OF_CHAR_DOUBLE	= 0x00000002,	// 双层纹理
	OF_RAND_PLAY	= 0x00000004,	// 随机播放
};


typedef std::vector<int> VInt; //Int数组

class CTexFrame;

// 纹理定义
class CTextureInterface
{
public:
	virtual ~CTextureInterface(){}

public:



	// 添加一个32位纹理帧
	virtual void        GetNewDiv(int iw,int ih,VInt& vW,VInt& vH) = 0;
	virtual BOOL        AddFrame(int iw,int ih,BYTE* pData,int iPitch = 0) = 0;

	// 字体纹理的创建和销毁
	virtual BOOL		CreateFontTex(int iw,int ih,BYTE *pBuf) = 0;
	virtual void		ReleaseFontTex(void) = 0;	

	virtual DWORD		GetOption(void) = 0;		        // 纹理属性
	virtual void		SetOption(DWORD dwOption) = 0;		// 纹理属性
	virtual DWORD		GetFrames(void) = 0;		        // 纹理帧数
	virtual void		SetFrames(DWORD dwFrames) = 0;      // 纹理帧数
	virtual DWORD		GetSpeed(void) = 0;			        // 纹理速度
	virtual void		SetSpeed(DWORD dwSpeed) = 0;        // 纹理速度
	virtual DWORD		GetID(void) = 0;			        // 纹理ID
	virtual void		SetID(DWORD dwID) = 0;	            // 设置数据大小
	virtual DWORD		GetFrameNow(void) = 0;		        // 当前纹理帧索引


	virtual int			GetWidth0(void) = 0;		// 当前帧原始宽度
	virtual int			GetHeight0(void) = 0;		// 当前帧原始高度
	virtual int			GetWidth(void) = 0;			// 当前帧宽度
	virtual int			GetHeight(void) = 0;		// 当前帧高度
	virtual int			GetOffX(void) = 0;			// 当前帧偏移X
	virtual int			GetOffY(void) = 0;			// 当前帧偏移Y
	virtual int			GetCenterX(void) = 0;		// 当前帧中心点X
	virtual int			GetCenterY(void) = 0;		// 当前帧中心点Y

	virtual void		EnableSysAnim(BOOL b) = 0;						// 设置是否自动显示动画纹理
	virtual void		SetOff(int iOffX,int iOffY) = 0;				// 设置当前帧偏移
	virtual void		SetCenter(int iCenterX,int iCenterY) = 0;		// 设置当前帧中心点

	virtual void		UpdateFrame(DWORD dwCunt) = 0;					// 更新纹理帧动画的计数基数
	virtual void		SetCurFrame(DWORD dwFrame) = 0;					// 设置当前帧
	virtual void		NextFrame(void) = 0;							// 当前帧的下一帧
	
	virtual CTexFrame*  GetCurFrame() = 0;					            // 获得当前帧
	virtual CTexFrame*  GetFrame(DWORD dwFrame) = 0;		            // 获得指定帧
	virtual void        ClearFrame() = 0;		                        // 清空定帧

	virtual void		SetPixelSize(DWORD dwSize) = 0;				    // 设置数据大小
	virtual DWORD		GetPixelSize() = 0;				                // 获得数据大小


	virtual BOOL		IsOption(eOptionFlag eOF) = 0;
	virtual int 		IsPointInTex(int ix,int iy,float fScaleX = 1.0f,float fScaleY = 1.0f) = 0;		// 判断点是否在纹理区域
};

typedef LPDIRECT3DTEXTURE9		LPD3DTEX;
typedef CTextureInterface*		LPTexture;
