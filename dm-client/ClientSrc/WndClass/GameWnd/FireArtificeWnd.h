#pragma once
#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlButton.h"
#include "GameData/good.h"
#include "GameData/NpcData.h"


//真火炼化
class CFireArtificeWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CFireArtificeWnd)

public:
	CFireArtificeWnd(void);
	~CFireArtificeWnd(void);

	virtual void Draw();
	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl /* = NULL */);
	virtual bool OnLeftButtonUp(int iX, int iY);
	virtual bool OnMouseMove(int iX,int iY);

protected:
	void BackToPackage();
	CGood* GetGoodOnGrid(int iX,int iY,int& gridx, int& gridy);

	int CalcType();
	bool SendFireArtifice(WORD test);

	void UpdateFireArtTime();
	void OnFireArtTimeSet();

	

	CCtrlButton * m_pTestBtn;
	CCtrlButton * m_pOkBtn;
	int m_iType;
	int m_iGridStartX;
	int m_iGridStartY;
	int m_iGridCount;
	

	int m_iState;// -3:无，-1:冷却状态，0：正常状态，1：预览中，2：预览发收消息中，10：炼化发收消息中
	int m_iYuLanFrame;
	int m_iFrame;
	int m_iSussFrame;
	int m_iFailFrame;

	FireArtificeResult m_kRet;
};