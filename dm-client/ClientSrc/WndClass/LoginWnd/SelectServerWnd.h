
#pragma once

#include "BaseClass/Control/CtrlWindowS.h"
#include "BaseClass/Control/CtrlButton.h"
#include "GameData/GameDefine.h"

class CSelectServerWnd : public CCtrlWindowS
{
	DECLARE_WND_POS(CSelectServerWnd)
protected:
	CCtrlButton * m_pServerButton[MAX_GROUP_NUM];


	bool m_bHavePreLoadSelCharTex;//是否预载过选角色及创建角色界面的图素
public:
	CSelectServerWnd(void);
	~CSelectServerWnd(void);

	virtual void OnCreate();
	virtual void OnClickCloseButton();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	virtual void Draw();
};
