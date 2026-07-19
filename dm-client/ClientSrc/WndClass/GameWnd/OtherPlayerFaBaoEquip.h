#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include "GameData/Good.h"

class COtherPlayerFaBaoEquip:
	public CCtrlWindowX
{
	DECLARE_WND_POSX(COtherPlayerFaBaoEquip)
public:
	COtherPlayerFaBaoEquip(void);
	~COtherPlayerFaBaoEquip(void);

	virtual void Draw(void);
	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	virtual bool OnMouseMove(int iX,int iY);

};
