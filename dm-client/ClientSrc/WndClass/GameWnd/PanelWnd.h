#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CommonViewer.h"
#include "BaseClass/Control/CtrlScroll.h"
#include "BaseClass/Control/GoodGrid.h"


class CPanelWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CPanelWnd)

public:
	CPanelWnd(void);
	~CPanelWnd(void);
    virtual void OnCreate(void);
	virtual void Draw();
	virtual void OnDraw();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual bool IsInControl(int iX,int iY);
	virtual bool OnMouseMove(int iX, int iY);
	virtual bool OnLeftButtonUp(int iX,int iY);
	virtual bool OnLeftButtonDClick(int iX,int iY);
	virtual void ResetControlPos();
	void ShowAnotherMsg();//右侧到左侧飘红
	void ShowOtherMsg();//左侧到右侧飘红
	void ShowOtherMsg2();//闪现飘红
	void ShowShenYouEff();//附身神佑消失提示效果
	void ShowCustomPiaohong();//自动以飘红

	void ChangeTalkAutoWheel();
	bool IsFakeFocus();

protected:
	CCommonViewer* m_pHPMPViewer;
 
	int m_iWCells,m_iHCells;//格子个数
	int m_iCellWidth,m_iCellHeight,m_iCellDisW,m_iCellDisH;//包裹格子大小及间隔
	int m_iDrawTime;//左飘时间次数
	UINT64 m_iLastExp;
	POINT	m_ptLeftPoint;
	

	CGoodGrid *m_pGoodGrid;//腰带物品
	int m_iSuperFrame; //超级红，蓝帧控制


	DWORD m_dwOtherMsgShowTime;
	DWORD m_dwCustomMsgShowTime;		//自定义飘红开始显示时间
	DWORD m_dwOtherMsg2ShowTime;
	DWORD m_dwAnotherMsgShowTime;//飘红显示的时间控制
	DWORD m_dwOtherMsgStayTime;//飘红停留时间
	DWORD m_dwOtherMsg2StayTime;//飘红2停留时间
	bool m_bMsg2FirstDraw;//飘红2左移或右移
	float m_fZhenBaoValuePercent;//珍宝值显示比例

	bool IsInExp(int ix,int iy);//ix,iy是否在经验条位置
	//bool IsInWeight(int ix,int iy);//ix,iy是否在负重条位置
	//bool IsInZhenBaoVal(int ix,int iy);//ix,iy是否在珍宝值位置
	//bool IsInZhenBaoBall(int ix,int iy);//ix,iy是否在珍宝珠位置
	//void ShowZhenBao();//画珍宝值
	void ShowAlarm();//画网络状况及本机性能
	void DrawXiangLuAndTower();//画香塔和香炉
	void InitAlarmState(void);
	int InAlarmTip(int iX, int iY);
	//void ShowSecondKill();//绘制秒杀槽
	//void OnVoiceWndBtnClicked(bool bFromIGW,DWORD dwData = 0);//点击了语聊按钮
	void OnChangeChannel(CControl * pControl);//点了改变频道按钮

	void SetControlState();

};
