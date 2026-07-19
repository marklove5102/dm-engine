#pragma once
#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlButton.h"
#include "BaseClass/Control/CtrlRadio.h"
#include "GameData/good.h"


enum EFaBaoType
{
	EFB_FBQH = 1,	// 法宝强化
	EFB_OpenLS,		// 开启灵识
	EFB_SLJH,		// 圣灵净化
	EFB_SLZR,		// 圣灵注入
	EFB_SLHY,		// 圣灵还原
	EFB_ChangeLS,	// 更改灵识属性
	EFB_SLRH,		// 圣灵融合
	EFB_SLTQ,		// 圣灵提取	
};

enum EFaBaoGrid
{
	EFBG_FBQH_MainFaBao = 0,
	EFBG_FBQH_Fu,
	EFBG_FBQH_SecondFaBao1,
	EFBG_FBQH_SecondFaBao2,
	EFBG_FBQH_SecondFaBao3,

	EFBG_OpenLS_FaBao,		//需要开启灵识的法宝
	EFBG_OpenLS_CaiLiao,	//开启灵识的材料

	EFBG_SLJH_ShengLing,	//需要净化的圣灵
	EFBG_SLJH_CaiLiao,		//圣灵净化符

	EFBG_SLZR_FaBao,		//需要注入的法宝
	EFBG_SLZR_ShengLing,	//注入的圣灵
	EFBG_SLZR_CaiLiao,		//圣灵注入符

	EFBG_SLHY_ShengLing,	//需要还原的圣灵
	EFBG_SLHY_CaiLiao,		//圣灵还原符

	EFBG_ChangeLS_FaBao,	//法宝
	EFBG_ChangeLS_CaiLiao,	//转换乾坤道具

	EFBG_SLRH_ShengLing1,	//需要融合的圣灵
	EFBG_SLRH_ShengLing2,
	EFBG_SLRH_ShengLing3,
	EFBG_SLRH_ShengLing4,
	EFBG_SLRH_ShengLing5,
	EFBG_SLRH_CaiLiao,		//圣灵融合符

	EFBG_SLTQ_FaBao,		//需要提取圣灵的法宝
	EFBG_SLTQ_CaiLiao,		//圣灵提取符
	
	EFaBaoGoodGridNum		// ==MAX_FABAO_GOOD_COUNT
};


//法宝升级强化
class CFaBaoLevelUpWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CFaBaoLevelUpWnd)

public:
	CFaBaoLevelUpWnd(void);
	~CFaBaoLevelUpWnd(void);

	virtual void Draw();
	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl /* = NULL */);
	virtual bool OnLeftButtonUp(int iX, int iY);
	virtual bool OnMouseMove(int iX,int iY);

protected:
	void BackToPackage();
	int IsConditionSatisfied();
	bool IsInGrid(int iX,int iY, int gridx, int gridy);
	bool IsInGrid(int iX,int iY, int grid);
	CGood* GetGoodOnGrid(int iX,int iY,int& grid);
	bool CheckValidGoodOnGrid(int grid,CGood& good);

	void GetGridStartEnd(int& gridstart, int& gridend);

	void ShowEffect();

	bool SendMsg();
	

	int m_iType;		// EFaBaoType

	bool m_bInValidGoodOnGrid;
	int m_iGoodOnGrid;

	CCtrlButton * m_pOkBtn;
	CCtrlButton * m_pCancelBtn;


	CCtrlRadio * m_pHole[10];	
	CCtrlRadio * m_pHoleToConvert[10];

	static const POINT m_ptHoleToConvert[10];	

	int m_iNeedMoney;
	int m_iConditionSatisfy;

	int m_iOKClick;	

	int m_iSuccessFrame;
	int m_iFailFrame;	

	static const POINT m_ptGridPos[EFaBaoGoodGridNum];
};