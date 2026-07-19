#pragma once
#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlButton.h"
#include "BaseClass/Control/CtrlRadio.h"
#include "GameData/good.h"

enum GridType
{
	EquipToDrill = 1,
	EquipToEmbed,
	EquipToChangeHole,
	EquipToRemoveFuShi,
	FuShiToEmbed,
	FuShiToUpgrade,
	FuShiToCombine1,
	FuShiToCombine2,
	FuShiToCombine3,
	FuShiToCombine4,
	FuShiToCombine5,
	Drill,
	XianQianFu,
	QianKunFu,
	DiaoZhuoFu,
	HeChengFu,
	ZhaiChuFu,	
	EquipToUpgrade,
	QiangHuaFu,
	XingYunFu,
	FuShiToDegrade,
	HuanYuanFu,
	GridNum,
};

class CFuShiSeriesWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CFuShiSeriesWnd)
public:
	CFuShiSeriesWnd(void);
	~CFuShiSeriesWnd(void);

	virtual void Draw();
	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl /* = NULL */);
	virtual bool OnMouseMove(int iX,int iY);
	virtual bool OnLeftButtonUp(int iX, int iY);
private:
	CGood* GetGridGood(int iX,int iY,int& grid);
	bool IsConditionSatisfied();
	bool IsInValidGoodOnGrid(int grid,CGood& good);
	void ShowEffect();

	CCtrlButton * m_pOkBtn;
	CCtrlButton * m_pCancelBtn;

	CCtrlRadio * m_pHole[10];	
	CCtrlRadio * m_pHoleToConvert[10];

	POINT m_ptHoleToConvert[10];
	POINT m_ptGridPos[GridNum];

	bool m_bConditionSatisfy;

	bool m_bInValidGoodOnGrid;
	int m_iGoodOnGrid;

	int m_iNeedMoney;

	int m_iType;

	int m_iSuccessFrame;
	int m_iFailFrame;

	int m_iFrame;

	bool m_bFirstClick;

	bool m_bWithHuBaoFu;
};