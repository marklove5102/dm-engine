#pragma once
#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlButton.h"
#include "BaseClass/Control/CtrlRadio.h"
#include "BaseClass/Control/MarkViewer.h"
#include "GameData/good.h"

enum EquipSoulType
{
	EST_Wake = 1,	// Æ÷»ê¾õÐÑ
	EST_LevelUp,	// Æ÷»êµÈ¼¶ÌáÉý
	EST_Peel,		// Æ÷»ê¾§Ê¯°þÀë
};

enum EquipSoulGrid
{
	ESG_Wake_Equip = 1,
	ESG_Wake_Hunshi,
	ESG_LevelUp_Equip,
	ESG_LevelUp_JieJing1,
	ESG_LevelUp_JieJing2,
	ESG_LevelUp_JieJing3,
	ESG_LevelUp_JieJing4,
	ESG_LevelUp_JieJing5,
	ESG_Peel_Equip,
	ESG_Peel_JieJing,
	EquipSoulGridNum
};

// Æ÷»ê¾õÐÑ,Æ÷»êµÈ¼¶ÌáÉý,Æ÷»ê¾§Ê¯°þÀë
class CEquipSoulWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CEquipSoulWnd)

public:
	CEquipSoulWnd(void);
	~CEquipSoulWnd(void);

	virtual void Draw();
	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl /* = NULL */);
	virtual bool OnLeftButtonUp(int iX, int iY);
	virtual bool OnMouseMove(int iX,int iY);

protected:
	void BackToPackage();
	CGood* GetGoodOnGrid(int iX,int iY,int& grid);
	int  IsConditionSatisfied();
	bool IsInValidGoodOnGrid(int grid,CGood& good);
	bool IsInGrid(int iX,int iY, int gridx, int gridy);
	bool IsInGrid(int iX,int iY, int grid);
	void SendEquipSoulMsg();

	DWORD m_dwType;		// EquipSoulType
	CCtrlButton * m_pOkBtn;
	CCtrlButton * m_pCancelBtn;

	CMarkViewer * m_pMarkViewer;
	CTagText	m_Tag;

	int m_iNeedMoney;
	int m_iConditionSatisfy;
	bool m_bInValidGoodOnGrid;
	int m_iGoodOnGrid;
	int m_iOKClick;
	int m_iSuccessFrame;
	BYTE m_UseBind;

	static const POINT m_ptGridPos[EquipSoulGridNum];
};