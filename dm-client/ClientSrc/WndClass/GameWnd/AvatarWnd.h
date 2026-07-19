#pragma once

#include "BaseClass/Control/Control.h"
#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlButton.h"
#include "GameData/GameDefine.h"
#include "GameData/GameGlobal.h"
#include "GameData/Good.h"


#define NEWWND_DELTA_X 32
#define NEWWND_DELTA_Y

class CStateViewer : public CCtrlWindowX
{
public:
    CStateViewer();
    ~CStateViewer();
    virtual void OnDraw();    
    virtual void ShowCharState();
	virtual void ShowWuXingState();
    //virtual void ShowProduct();
	//virtual void SetShowDemonState(int iState);
    //virtual int GetShowDemonState();
  	//virtual void ShowHairEquip();		
		    void DrawBackground();  
	
protected:
	virtual CGood& GetCharacterEquipment(int i) = 0;
	virtual __int64 GetCharacterLooks() = 0;
	virtual bool   IsCharacterGirl() = 0;
	virtual int    GetCharacterMaskLevel() = 0;

	int     m_iButtonFrame;
    POINT   m_ptButton;
	bool    m_bPickRiderClothes;                    //是否点选装备栏中的御兽天衣(袍) 
	int		m_iHighLightWuXing;		//五行界面高亮显示的五行属性
	bool	m_bSwitch;

	int m_iFrame;

    virtual void PrintStateInfoPair(int iX,int iY,char* szFormat,DWORD dwArg1,DWORD dwArg2,DWORD dwColor = COLOR_TEXT_NORMAL);
    virtual void PrintStateInfoSingle(int iX,int iY,char* szFormat,DWORD dwArg,DWORD dwColor = COLOR_TEXT_NORMAL);
    //virtual void ShowExpChart(int iX,int iY,DWORD dwLowDarkExp,DWORD dwHighDarkExp);

    //int  m_iShowDemonState;//显示类型

    //LPTexture  m_pStateEx1;
    //LPTexture  m_pStateEx2;
	//LPTexture  m_pStateEx3;
	//LPTexture  m_pStateEx4;				

    //LPTexture  m_pLowDarkExp;
    //LPTexture  m_pHighDarkExp;
    //RECT m_rcHighDarkExp;
    //RECT m_rcLowDarkExp;

    //LPTexture  m_pWhite;
    //LPTexture  m_pBrown;
    //LPTexture  m_pYellow;
    //LPTexture  m_pRed;   
	DWORD m_dwWhiteID;
	DWORD m_dwBrownID;
	DWORD m_dwYellowID;
	DWORD m_dwRedID;

};

class CAvatarWnd : public CStateViewer
{
	DECLARE_WND_POSX(CAvatarWnd)

public:
	CAvatarWnd(void);
	~CAvatarWnd(void);
    virtual void OnCreate();
    virtual bool OnMouseMove(int iX, int iY);
    virtual bool OnLeftButtonUp(int iX, int iY);
    virtual void Draw();
    virtual void OnDraw();
    virtual bool OnLeftButtonDown(int iX, int iY);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	//virtual bool OnLeftButtonDClick(int iX,int iY);
			void ShowCharAttr();
			void ShowPKState();
			void ShowPlayerInfo();
protected:
	virtual CGood& GetCharacterEquipment(int i);
	virtual __int64 GetCharacterLooks();
	virtual bool   IsCharacterGirl();
	virtual int    GetCharacterMaskLevel();
	virtual bool IsInControl(int iX,int iY);
	void PlayEffect(DWORD dwID);

    bool	m_bDClicked;					        //是否是双击左键
    int		m_iPosPkg;                              //包裹位置
    DWORD	m_dwAlarmColor;						    //红或者绿  
    bool    m_bRead;   
	//LPTexture  m_pLogo;
	//LPTexture  m_pLogoBac;
	//LPTexture m_pWuXingTex;
	//LPTexture m_pGuildLogo;

	DWORD  m_dwLogoID;
	DWORD  m_dwLogoBacID;
	DWORD  m_dwWuXingTexID;
	DWORD  m_dwGuildLogoID;

	int    m_iPlayEffect;                          //双击圣灵精华后播放特效1为升级武器2为升级盾牌
	int     m_iEffectFrameCount;
	//LPTexture m_pSinglePlayerMaskTex;	//单独玩家蒙板

	int m_iEffectCurFrame;

	CCtrlButton * m_pShowWuXing;	
};
