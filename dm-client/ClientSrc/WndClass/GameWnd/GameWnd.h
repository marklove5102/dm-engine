#pragma once

#include "Global/Global.h"
#include "Global/Timer.h"
#include "BaseClass/Control/Control.h"
#include "BaseClass/Control/CtrlScroll.h"
#include "BaseClass/Control/CtrlButton.h"
#include "BaseClass/Control/TalkViewer.h"
#include "BaseClass//Control/ParserTip.h"
#include "BaseClass/Control/TaskTracker.h"
#include "GameManager.h"  //控制部分
#include "GSceneManager.h" //绘制和显示部分
#include "GMagicManager.h"
#include "RunHorseLampWnd.h"


// 地图引擎窗口，转。
class CGameWnd : public CCtrlWindow
{
DTI_DECLARE(CGameWnd, CCtrlWindow)

public:
	CGameWnd(void);
	~CGameWnd(void);

	virtual bool Create(CControl * pParent,int iX,int iY,int iW,int iH);				// 创建函数
	virtual void OnCreate();							

	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	virtual void Draw();

	virtual bool OnLeftButtonDown(int iX,int iY);
	virtual bool OnLeftButtonUp(int iX,int iY);
	virtual bool OnLeftButtonDClick(int iX,int iY);
	virtual bool OnRightButtonDown(int iX,int iY);
	virtual bool OnRightButtonUp(int iX,int iY);
	virtual bool OnRightButtonDClick(int iX,int iY);
	virtual bool OnMiddleButtonDown(int iX,int iY);
	virtual bool OnMouseMove(int iX,int iY);
	virtual bool OnKeyDown(WORD wState,UCHAR cKey);
	virtual bool OnKeyUp(WORD wState,UCHAR cKey);
	virtual void OnKillFocus();						// 系统将失去焦点时此函数被调用
	//virtual bool ReSize(int iW,int iH);
	virtual void OnEscape(void);
	virtual void ResetControlPos();

	void OnTimer();
	bool GetDropedObject(int iTileX,int iTileY);//如果自己所有坐标==iTileX,iTileY拾取这个坐标处地面上的物品
private:
	CSceneManager m_SceneMgr;
	CGameManager m_GameMgr;
	CMagicManager m_MagicMgr;

	CCtrlButton* m_pAttackType;      //选择修改攻击方式按钮
	CCtrlButton* m_pServerMsg;       //系统公告
	//CCtrlButton* m_pPlayerMsg_Lock;  //锁定玩家相关消息
	//CCtrlButton* m_pPlayerMsg_Hide;  //隐藏玩家相关消息，把他放到聊档显示
	CCtrlButton* m_pPromptInfo;		//提示信息
	CCtrlButton* m_pInstantHint;	//即时提示信息
	DWORD		 m_dwEmergentProInfo;
	CRunHorseLampWnd* m_pRunHorseLamp;

	CCtrlButton* m_pBtnAutoKill; //自动打怪	
	CCtrlButton* m_pWidowBtn; //窗口模式,从全部切换到窗口模式

	CCtrlButton* m_pBtnQiYu; //奇遇

	CCtrlButton* m_pBtnNoticeIE; //NoticeIE	

	CCtrlButton* m_pBtnBing;		//12宫兵种控制

	CCtrlButton* m_pBtnBindPT; //绑定通行证

	bool		 m_bStartAI;

	//CTalkViewer* m_pSystemTalkView;   //左边的系统提示信息
	CTalkViewer* m_pServerTalkView;   //上面中间部分的系统公告

	CTaskTracker* m_pTaskTrackViewer; //任务追踪

	bool m_bDrawTalkServer;
	//bool m_bCreated;
	BYTE m_byLocalAreaType;           // 所在的城区
	DWORD m_dwShowPiaoHongTime;       //画精英勋章飘红时间控制

	bool  m_bStartDefend;	     //显示“开始防守”四个字
	DWORD m_dwDefenceTime;		//开始显示“开始防守”四个字的时间

	//int	  m_iRunHorsePosX;		//跑马灯位置
	//int	  m_iRunHorsePosY;
	//int	  m_iOldRHPX;
	//int	  m_iOldRHPY;
	//bool  m_bOnMoveRHP;
	//int   m_iHorseDrawX;
    
	void OnChangeAttackType();//点了改变攻击模式按钮
	//void OnHidePlayerMsg();//点了隐藏系统消息按钮，把系统消息放到panelwnd里
	void DrawScrollMsg();//绘制滚动淡出的的系统公告
	void OnClickSeverMsg();//系统公告中如果出现商城道具，点击以后跳到相应的商城页面(openstore/大类/小类/ItemID) 如:"测试：商城里有御兽天衣(包月)出售: <御兽天衣(包月)/@@openstore/50/20/1424>"
	void DrawClientPiaoHong();//画精英勋章最后一小时每5分钟一次的飘红
	void DrawWXXGTimeOut();     //画五行玄关时间
    void DrawWXXGTips();        //画五行玄关提示
	void DrawTimeOut();         //绘制倒计时,通用
	void DrawFlyText();         //绘制飞行文字
	void DrawPromptInfo();
	void DrawTXZGTips();//铁血战歌副本提示
	void DrawTSJB();	//世界杯比分提示
	void DrawQiYu();
	void DrawConSkillBuff(); //绘制技能连击buff
	void DrawNoticeIE();
	void Draw8DunProgress();
};