#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include "GameData/ItemCfgMgr.h"
#include "GameData/GameDefine.h"

class COtherPlayerEquip : public CCtrlWindowX
{
	DECLARE_WND_POSX(COtherPlayerEquip)

public:
	COtherPlayerEquip(void);
	~COtherPlayerEquip(void);

	virtual void OnCreate();
	virtual void Draw(void);
	virtual bool OnLeftButtonDClick(int iX, int iY);
	virtual bool OnLeftButtonUp(int iX, int iY);
	virtual bool OnMouseMove(int iX, int iY);
	virtual bool OnRightButtonDown(int iX, int iY);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
protected:

	LPTexture  m_pTexture;
	LPTexture  m_pLogo;

	unsigned long m_dwHairColor[16];

	POINT m_ptEquipmentPos[MAX_EQUIPMENT];				//装备绘制的位置

	int     m_weapon_looks_frame;
	int     m_frame_speed;

	int     m_logo_pos_x,m_logo_pos_y;
	bool    m_bRead;
	BOOL    m_draw_wing;
	int     m_mask_level;
	int     m_mouse_x,m_mouse_y;
	SItemEffect         m_sItemEffect; //存放物品特效信息的变量
	BYTE   m_byVipCardType;
	BYTE   m_byVipTradeLevel;


	void ShowSmallLock();
	virtual int  InGrid(int iX, int iY);
};
