#pragma once
#include "GameData/Good.h"
#include "BaseClass/Control/CtrlWindowX.h"

using namespace std;

class CSealBookWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CSealBookWnd)
public:
	CSealBookWnd(void);
	~CSealBookWnd(void);

	virtual void OnCreate();
	virtual bool OnRightButtonUp(int iX,int iY);
	virtual bool OnLeftButtonUp(int iX,int iY);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	virtual void Draw();
	void GetBookInfo(CGood* pGood);	//通过点击的物品获得封元册的信息
	bool OnLeftButtonDClick(int iX,int iY);
	bool OnMouseMove(int iX,int iY);

protected:
	void DwawSingleCard(int iX,int iY,int iIndex,int iCount,bool IsFlash);
	void GetSelectedPos(int iX,int iY);
	int  GetSelectedTNum(int iPos);
	int m_iType;
	char m_szName[32];
	POINT m_ptCardPos[10];
	vector<string> m_vecCardNames;
    unsigned long m_dwCurrentBookID;			//封元册所对应的物品
	int m_iSelectedNum;
};
