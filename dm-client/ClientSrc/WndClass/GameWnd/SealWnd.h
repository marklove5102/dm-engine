#pragma once

#include "GameData/Good.h"
#include "BaseClass/Control/CtrlWindowX.h"

using namespace std;

class CSealWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CSealWnd)

public:
	CSealWnd(void);
	~CSealWnd(void);

	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	virtual void Draw();
	void GetCardInfo(CGood* pGood);	//通过点击的封元印获得卡片的信息
	void GetCardInfo(int iType,int iNum);//获得封元印卡片的信息

protected:
	CCtrlButton * m_pCloseButton;
	char m_szName[64];
	int m_iCardNum;
	int m_iRarity;	
	int m_iType;
	bool m_bFlashCard;		//玄华封元印
};
