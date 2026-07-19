#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include "GameData/ItemCfgMgr.h"


class CSmallGemCon : public CCtrlWindowX 
{
	DECLARE_WND_POSX(CSmallGemCon)

public:
	CSmallGemCon(void);
	~CSmallGemCon(void);

public:
	//重载父类函数
	virtual void OnCreate();//创建窗体上的控件
	virtual void Draw(void);
	virtual void OnClickCloseButton();
	virtual bool OnLeftButtonDown(int iX, int iY);
	virtual bool OnLeftButtonUp(int iX, int iY);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual bool OnMouseMove(int iX,int iY);

	int IsInGrid(int iX,int iY);
	int IsInGemGrid(int iX,int iY);
	bool bInSlidArea(int iX,int iY);
	void CheckSlideAear(int iX,int iY);
	bool IsInRightArea(int iX,int iY);
	void BackToPackage();

protected:
	CCtrlButton* m_pCondense;//合成按钮
	CCtrlButton* m_pLeftButton_Red;//红宝石向左按钮
	CCtrlButton* m_pRightButton_Red;//红宝石向右按钮
	CCtrlButton* m_pLeftButton_Blue;//兰宝石向左按钮
	CCtrlButton* m_pRightButton_Blue;//兰宝石向右按钮
	CCtrlButton* m_pLeftButton_Green;//绿宝石向左按钮
	CCtrlButton* m_pRightButton_Green;//绿宝石向右按钮

	CCtrlEdit*          m_pEditRedGem;
	CCtrlEdit*          m_pEditBlueGem;
	CCtrlEdit*          m_pEditGreenGem;

	RECT    m_rc[15];
	int m_iGemCount_Red ;//红宝石数量 需要熔炼的
	int m_iGemCount_Blue ;//兰宝石数量
	int m_iGemCount_Green ;//绿宝石数量

	int m_iMaxRedGem; //包裹中的宝石数量 + 仓库中的宝石数量
	int m_iMaxBlueGem;
	int m_iMaxGreenGem;

	POINT m_SlidePos1;
	POINT m_SlidePos2;
	POINT m_SlidePos3;

	int  m_iSlidePos;
	bool m_bIsSlide;
	bool m_bIsInRightArea;
	bool m_bIsbeenMelt;//是否已经熔炼过了

	int        m_iMouseOnGood;   //鼠标在此上面的索引
	int        m_iMouseType;     //鼠标在此上面的类型
	SItemEffect         m_sItemEffect; //存放物品特效信息的变量
};
