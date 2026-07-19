#pragma once
#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlButton.h"
#include "BaseClass/Control/CtrlRadio.h"
#include "GameData/good.h"
#include "GameData/NpcData.h"


enum EHuFuFlushGrid
{
	EFBG_HufuFlush_Hufu = 0,
	EFBG_HufuFlush_AddGood,
	EHuFuFlushGridNum
};

//³à½ð»¤·û
class CHuFuFlushWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CHuFuFlushWnd)

public:
	CHuFuFlushWnd(void);
	~CHuFuFlushWnd(void);

	virtual void Draw();
	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl /* = NULL */);
	virtual bool OnLeftButtonUp(int iX, int iY);
	virtual bool OnMouseMove(int iX,int iY);

protected:
	void BackToPackage();
	bool IsInGrid(int iX,int iY, int gridx, int gridy);
	bool IsInGrid(int iX,int iY, int grid);
	CGood* GetGoodOnGrid(int iX,int iY,int& grid);
	bool CheckValidGoodOnGrid(int grid,CGood& good);

	void GetGridStartEnd(int& gridstart, int& gridend);
	CGood* GetGoodByGrid(int grid);

	void ShowEffect();

	CCtrlButton * m_pBaoliuBtn;
	CCtrlButton * m_pShuaxinBtn;

	bool m_bShowRet;
	SChiJinRetPacket m_kRet;
	int m_iSuccessFrame;

	static const POINT m_ptGridPos[EHuFuFlushGridNum];
};