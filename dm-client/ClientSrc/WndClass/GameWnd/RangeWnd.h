#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include "GameData/RangeStruct.h"
#include "BaseClass/Control/CtrlMenuButton.h"
#include "BaseClass/Control/CtrlGrid.h"
#include "BaseClass/Control/CtrlTip.h"

//排名滚动的文字
typedef struct
{
	//LPTexture pLeftTex;
	//LPTexture pMidTex;
	//LPTexture pRightTex;
	DWORD dwLeftTex;
	DWORD dwMidTex;
	DWORD dwRightTex;

	WORD wType;
	char cNow;
}Range_Text;

class CRangeWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CRangeWnd)
public:
	CRangeWnd(void);
	~CRangeWnd(void);

private:

	LPTexture m_pGunLunTex[3];	//滚轮纹理

	CCtrlButton * m_pLeftButton; //左转页
	CCtrlButton * m_pRightButton; //右转页

	//CCtrlButton * m_pSelectTabButton; //选择TAB
	CCtrlButton * m_pUnSelectTabButton[6]; //未选择TAB

	CCtrlMenuButton * m_pMBLevel; //下拉各等级

	CCtrlButton * m_pHelpCloseButton; //帮助关闭
	//CCtrlButton * m_pHelpPaneButton; //帮助底版

	CCtrlTip  m_pTipMyInfo;

	bool m_bRightRoll,m_bLeftRoll;
	int m_iFrameCount;
	int m_wNow;

	int m_iLine; //当前显示行数

	int m_CurrentType;
	int m_CurrentSmallType;
	DWORD m_LastSendTick;
	DWORD m_MyLastSendTick;

	Range_Text * m_pRangeText;//排名文字纹理

	//LPTexture m_pLeftTex;
	//LPTexture m_pMidTex;
	//LPTexture m_pRightTex;

	DWORD m_dwLeftTex;
	DWORD m_dwMidTex;
	DWORD m_dwRightTex;

	//LPTexture m_pDragListTex; //下拉框
	//LPTexture m_pScrollTex;  //下拉条
	//LPTexture m_pSmallPaneTex;  //小面板1
	//LPTexture m_pSmallPaneTex2;  //小面板2
	//LPTexture m_pHelpLight; //帮助高亮
	//LPTexture m_pHelpTex;	//帮助字
	//LPTexture m_pSBKText;
	//LPTexture m_pGradeText;

	CCtrlGrid*  m_pGrid;

	map<WORD,string>	m_BottomText;
	map<WORD,string>	m_ColumnText;
	map<WORD,string>	m_ColumnWidth;

	//LPTexture m_pPet2BottomText;
	//LPTexture m_pNoneBottomText;

	//LPTexture m_pLeftLight;
	//LPTexture m_pRightLight;

	//LPTexture m_pPingWen;
	//LPTexture m_pUp;
	//LPTexture m_pDown;
	//LPTexture m_pZiTeXiao; //小字特效

	bool m_bLeftLight;
	bool m_bRightLight;
	bool m_bHelpLight;
	bool m_bHelp;
	
	string m_strColumnWidth;

	string m_MerTiPo[3];

	LP_RANGE_DADA mapRangeData;
	LP_RANGE_DADA m_pMyMapRangeData;

	int m_nWaitReading;

	int	m_GeLevel[16];
	int m_nowGeLevel;

	int m_nMyPlace;

	bool m_bSend,m_bMySend;
	bool m_bSendUp;
	int m_oldPos;

	DWORD m_dwTickCount;
public:
	//重载父类函数
	virtual void Draw();
	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual void OnClickCloseButton();
	virtual bool OnLeftButtonUp(int iX, int iY);
	virtual bool OnMouseMove(int iX,int iY);

	void GotoPlace(int place);
	void SendServerExpUp();

private:

	void DrawRangeMenu();
	void InitTexByType();
	bool SetTexByType();
	void SetSmallType();
	void Initcolumn();
	void DrawListData();
	void DrawColumn();
	void DrawLineData(int i,void *RangeData);
	void SetTips(int x,int y);
	void DrawMyInfo();
	void DrawWaitReading();
	void DrawHelp();
	void FindMyPlace();

};
