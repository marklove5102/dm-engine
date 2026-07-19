#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlButton.h"
#include "BaseClass/Control/CtrlEdit.h"
#include "GameData/NpcData.h"

//快捷消费提醒框
class CYuanBaoWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CYuanBaoWnd)

public:
	CYuanBaoWnd();
	~CYuanBaoWnd(void);

	virtual bool Create(CControl* pParent,int iCurPage = 0);
	virtual void Draw(void);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	virtual void OnCreate();

private:
	int m_iType;

	int m_iHeaderHeight;

	CCtrlButton * m_pBuyBtn[6];

	CCtrlButton * m_pRemoveBtn;
	CCtrlButton * m_pSellBtn;
	CCtrlButton * m_pTakeMoneyBtn;

	CCtrlEdit * m_pNumEdit;
	CCtrlEdit * m_pPriceEdit;

	_YuanBaoAllInfo::_YuanBaoSellInfo m_YuanBaoSellInfo;

};