#pragma once
#include "BaseClass/Control/CtrlWindowX.h"

class CTigerRuneWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CTigerRuneWnd)
public:
	CTigerRuneWnd(void);
	~CTigerRuneWnd(void);

	virtual void Draw(void);
	virtual void OnCreate();
	virtual bool OnLeftButtonUp(int iX, int iY);
private:
	int		GetRuneIndex(int pos);					//虎符成语字符的位置与纹理索引的对应关系
	bool	IsRightPos(int rune, int pos);			//判断字符是否属于该位置
private:
	POINT		m_runePos[16];					//高亮字符的显示位置
	int			m_iLastPutIn;					//最近一次放入的字符的索引
	DWORD		m_dwStartShowTime;				//放光框一帧的播放时间
	LPTexture	pLightingBorder;				//放光框的动画
	DWORD		m_dwCurFrameOfLightingBorder;	//放光框的动画的当前帧
	string		m_dwExp[4];						//经验值
	DWORD		m_dwTigerRuneID;
};
