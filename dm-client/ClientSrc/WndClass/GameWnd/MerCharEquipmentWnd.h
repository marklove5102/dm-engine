#pragma once

#include "AvatarWnd.h"


class CMerCharEquipmentWnd : public CStateViewer
{
	DECLARE_WND_POSX(CMerCharEquipmentWnd)

public:
	CMerCharEquipmentWnd(void);
	~CMerCharEquipmentWnd(void);
public:
	//virtual bool Create(CControl * pParent,int iX,int iY);
	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual void OnClickCloseButton();
	virtual void Draw(void);
	virtual bool OnLeftButtonUp(int iX, int iY);
	virtual bool OnRightButtonDown(int iX, int iY);
	virtual bool OnRightButtonUp(int iX,int iY);
	virtual bool OnMouseMove(int iX, int iY);

protected:
	POINT   m_ptEquipmentPos[MAX_EQUIPMENT];	   //装备绘制的位置
	bool	m_bDClicked;					//是否是双击左键
	int		m_iPos;							// 可否装备位置
	int		m_iPosPkg;    //包裹位置
	DWORD	m_dwAlarmColor;						// 红或者绿
	int     m_iButton;
	POINT   m_ptButton;
	int     m_iButtonFrame;

	//头发颜色
	unsigned long m_dwHairColor[16];
	int m_iFrameCount;
	LPTexture m_pWhite;
	LPTexture m_pBrown;
	LPTexture m_pYellow;
	LPTexture m_pRed;
	LPTexture m_pLogo;
	LPTexture m_pLogoBac;

	int        m_weapon_looks_frame;
	int        m_frame_speed;
	bool		  m_bRead;
	DWORD dwQueryYSTime ;
	//SItemEffect         m_sItemEffect; //存放物品特效信息的变量
	bool           m_bTianYi;
	bool		   m_bPlayEffect;
	bool                 m_bShowDemonState;
	LPTexture           m_pStateEx1;
	LPTexture           m_pStateEx2;
	LPTexture           m_pLowDarkExp;
	LPTexture           m_pHighDarkExp;


	void UseQianKunSuo(); //使用乾坤锁
	void SetPlayEffect(){ m_bPlayEffect = true; }

	//bool  IsArmEquipment(int iMode,int iPos);  
	//int   InGrid(int iX, int iY);              
	//bool  IsInOther(int iX,int iY);
	bool  IsInYuanShen(int iX,int iY);
	int   IsInDarkExp(int iX,int iY);
	int   InShowDemonArea(int iX,int iY);

	void ShowCharInfo();
	void ShowCharState();

	virtual CGood& GetCharacterEquipment(int i);
	virtual unsigned long GetCharacterLooks();
	virtual bool   IsCharacterGirl();
	virtual int    GetCharacterMaskLevel();
	virtual int    GetCharacterIbodyEx();

	virtual bool   IsOtherCharGirl();
	virtual DWORD  GetOtherCharLooks();
	virtual CGood& GetOtherEquipment(int i);
	virtual int    GetOtherCharMaskLevel();
	virtual int    GetOtherCharIbodyEx();//衣服协议扩

	void ShowCharStateEx(); //附魔属性从元神转移到玩家
	void ShowTitleStr();
	//void ShowEquippment();     //继承

	void ShowEquipDrawSpecialWeapon();//装备栏的特殊武器
	void ShowEquipHair();      //装备栏的人物头发
	void ShowEquipGoodEff(char * str,int iPos,int iX,int iY);//装备栏,方格内,物品特效
	void ShowEquipDemonEff(int iPos,int iX,int iY);  //装备栏上,方格内,附魔物品的特效框绘制
	void ShowEquipGoodTempEff(int iPos,int iX,int iY); //装备栏上,方格内,物品的临时触发特效 
	//显示小锁
	//void ShowSmallLock();
	void PlayEffect(DWORD dwID);
	//virtual bool   IsLittleCharacter();
	//virtual void   ShowOtherEquip();
};

