#pragma once
#include "BaseClass/Control/CtrlWindowX.h"

class CRTSInstanceWnd: public CCtrlWindowX
{
	DECLARE_WND_POSX(CRTSInstanceWnd)
public:
	CRTSInstanceWnd(void);
	~CRTSInstanceWnd(void);

	virtual void Draw(void);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	virtual void OnCreate();
private:
	CCtrlButton * m_pAttackSoldier;				//召唤进攻士兵
	CCtrlButton * m_pDefendSoldier;				//召唤守护士兵
	CCtrlButton * m_pTower;						//换取箭塔
	CCtrlButton * m_pChangeState;				//收起按钮

	LPTexture m_pUnfoldTex;						//展开纹理
	LPTexture m_pFoldTex;						//收起纹理

	static bool s_bState;						//状态 false展开:true收起
};
