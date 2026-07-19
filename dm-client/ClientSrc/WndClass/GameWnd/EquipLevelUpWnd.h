#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include "GameData/NpcData.h"



#define ELW_LION_FADE_FRAMES  60 //狮子淡出总帧数


class CEquipLevelUpWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CEquipLevelUpWnd)

public:
	CEquipLevelUpWnd();
	~CEquipLevelUpWnd(void);
public:
	//重载父类函数
	virtual void Draw(void);
	virtual void OnCreate();
	virtual void OnClickCloseButton();
	virtual bool OnLeftButtonDown(int iX, int iY);
	virtual bool OnLeftButtonUp(int iX, int iY);
	virtual bool OnMouseMove(int iX, int iY);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	void 		SetTipShow(bool b);
	int  		GetType() {return m_iType;}
	int         GetCommonGoodIndex(int iX,int iY);
	void        SetEnableUse(bool b);
	bool        GetEnableUse();
	void        SetReslutEffType(int iType);
	void        BackToPackage(bool backWeapon = true);//backWeapon是否把中间的物品也放回包裹，false则只把其它６个物品放回包裹
	bool        CanLevelUp(int iStdmod,int iShape = 0);
private:	
	RECT    m_rcCommonGood[MAX_LEVELUP_COUNT];
	CCtrlButton*  m_pStartButton;   //开始合成按钮
	int        m_iType;          //类型
	bool       m_bIsCondensing;  //正在合成
	int        m_iMouseOnGood;   //鼠标在此上面的索引
	int        m_iMouseType;     //鼠标在此上面的类型
	bool       m_bEnableUse;     //窗口是否可以放置宝石
	bool       m_bIsStart; //开始播放特效
	bool       m_bEndEff; //结束播放特效
	int	       m_iResulEffType;//升级结果特效
	bool       m_bGoodOn;  //是否放置物品
	int        m_iLevelUpType ;//装备升级的类别
	int m_iEffFrameSpeed ;
	int m_iEffFrameNow   ;
	int        m_iShowLionType;//0普通狮子，1,狮子淡出，最后拖盘出现2，放物品托盘，3，眼睛发亮的狮子
	int        m_iLionFadeCurFram;//狮子淡出当前画了多少帧
	int        m_iFrameCount2;//控制绘制红色的圆环时间条

	//狮子闪光及射光控制
	DWORD dwFrequent;
	int iLightPos0_Elp;
	int iLightPos1_Elp;
	int iLightPos2_Elp;
	int iLightPos3_Elp;
	bool bLionFrash_Elp[MAX_LEVELUP_COUNT-1];
	WORD m_bDrawLionHead[MAX_LEVELUP_COUNT-1];   //0空着1画狮子头>1画原石且值为原石looks
	UINT lastMusicRand;

	void DrawGoods();
	void UpdateLionHead(CGood& good);

	DWORD m_dwTickCount;

	int m_iSelectedYuanShi;
};
