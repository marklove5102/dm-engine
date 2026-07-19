#pragma once
#include "BaseClass/Control/CtrlWindowX.h"
#include "GameData/TagText.h"
#include "BaseClass/Control/MarkViewer.h"
#include "GameData/OtherData.h"

#define M_ROTNUMCOUNT 7


struct RotItem
{
	RotItem()
	{
		m_iRotTarget = 0;
		m_fPos = 0;		
		m_iRotState = 0;
		m_fRotSpeed = 12.0f;
		m_iRotNum = 0;
		m_fStopPos = 5;
		m_iMaxH = 42;
		m_iDis = 37;
		m_bStopSend = false;
	}

	void SetRotSpeed(float val) { m_fRotSpeed = val; }
	void SetStopPos(float val)	{ m_fStopPos = val; }
	void SetRotTarget(int val)	{ m_iRotTarget = val; }
	void SetDis(int val)		{ m_iDis = val; }
	void SetStopSend(bool bsend)	{ m_bStopSend = bsend; }
	
	virtual void Start();
	virtual void Stop();
	virtual void Draw(int x, int y);

	void GetFinalClip(int& out_iFinalTop, int& out_iFinalBottom, int iClipTop, int iClipBottom, int source_offy, int source_h);
	void DrawClipTex(int x, int y, LPTexture pTex, int iClipTop, int iClipBottom);

	int m_iRotState;
	float m_fStopPos;	//停止的位置
	float m_fPos;		//当前位置
	float m_fRotSpeed;	//滚动的速度
	int m_iRotNum;		
	int m_iRotTarget;
	int m_iMaxH;		//最大高度
	int m_iDis;			//上下两个之间的距离
	bool m_bStopSend;	//停止是否发送消息
};

struct RotPic : public RotItem
{
	RotPic()
	{
		m_fRotSpeed = 12.0f;		
		m_fStopPos = 5;
		m_iMaxH = 42;
		m_iDis = 37+42;
		m_iStopDelay = 0;
		m_itemCount = 10;
	}
	
	void SetStopDelay(int val)		{ m_iStopDelay = val; }

	virtual void Draw(int x, int y);

	void DrawOne(int x, int y, int iPosY, int index);
	
	int m_iStopDelay;	//延迟停止
	int m_itemCount;
};

struct RotGood : public RotItem
{
	RotGood()
	{		
		m_fRotSpeed = 12.0f;		
		m_fStopPos = 5;
		m_iMaxH = 42;
		m_iDis = 32+42;
	}	

	virtual void Draw(int x, int y);

	void DrawGoodEffect(const char *strName,int iX,int iY,int dwClientLooks, int iClipTop, int iClipBottom);
	void DrawGood(int x, int y, int iPosY, int index);

	VQiYuRotItem m_vItems;

};


// 奇遇界面
class CAdventureWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CAdventureWnd)

public:
	CAdventureWnd(void);
	~CAdventureWnd(void);

	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual void Draw(void);
	virtual bool OnLeftButtonUp(int iX, int iY);
	virtual bool OnLeftButtonDown(int iX, int iY);

protected:
	int m_iType;

	RotPic m_picRot[M_ROTNUMCOUNT];
	RotGood m_itemRot;

	int m_iState;
	int m_iFrame;

	CCtrlButton* m_pOK;
	
	CMarkViewer* m_pMKVDes1;
	CMarkViewer* m_pMKVDes2;

	int m_iRewardType; //1:金币,2:物品,3:经验
	int m_iRewardNum;
	int m_iRewardLooks;
};
