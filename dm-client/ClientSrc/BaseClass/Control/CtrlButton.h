#pragma once

#include "control.h"
#include "XmlControl.h"
#include "Global/StringUtil.h"
enum E_BUTTON_ID
{
	BUTTON_CLOSE=	1,
	BUTTON_SWITCH=	7,
	BUTTON_OK	=	19,
	BUTTON_CANCEL=	25,
	BUTTON_ADD	=	31,
	BUTTON_DEL	=	43,
	BUTTON_LEFT	=	59,
	BUTTON_RIGHT=	65,
	BUTTON_DEFAULT=	71,
	BUTTON_MENU	=	77,
};

class CCtrlButton :	public CControl
{
	DTI_DECLARE(CCtrlButton, CControl)
protected:
	bool		m_bSwitchButton;		// 是否是双态按钮
	bool		m_bSwitchState;			// 双态按钮的状态(假如是双态按钮的话)
	bool        m_bAutoSwitch;          // 双态按钮

	//LPTexture	m_vTexs[BTEX_NUMS];		// 按钮控件的帧数
	WORD        m_vTexID[BTEX_NUMS*2];
	int			m_iIndex;
	//LPTexture   m_pTexEff;  //特效图片
	DWORD       m_dwTexEffID;  //特效图片
	int         m_iEffOffX,m_iEffOffY;  // 特效图片的偏移
	DWORD       m_dwMouseColor;
	DWORD       m_dwClickColor;
	DWORD       m_dwDisableColor;
	E_BUTTON_TEX   m_eBackState;//背景状态
	bool        m_bVertical;//文字是否为垂直排列
	int         m_iLines;//垂直排列时的行数
	int         m_iGap;//字间隔
	VString		m_vText;	
	int         m_iTextAlignType;       //在父窗口中的排列方式,XALINEMENT之一
	int         m_iBtnType;//0:普通,1:CtrlMenuButton用到的,按下文字不要偏移
	DWORD       m_dwFlashTexID;//是否要闪烁,新手帮助有时候要闪烁特定的按钮,为0表示不要闪烁,< 0x0000FFFF表示闪烁的类型,1为第2态图片闪烁, >0x0000FFFF表示闪烁纹理ID
	DWORD       m_dwFlashFrameControl;//用于闪烁控制
	DWORD       m_dwData;//用来存放一些控制数据
public:
	CCtrlButton(void);
	virtual ~CCtrlButton(void);
	virtual bool Create(CControl * pParent, int iX, int iY, int iIndex, bool bSwitch = false);
	virtual bool Create(CControl * pParent,int iX,int iY,UINT uStyle=CTRL_STYLE_BACKMODE_NODRAW|CTRL_STYLE_AUTOSIZE,int iW=-1,int iH=-1);
	virtual bool CreateEx(CControl* pParent,int iX,int iY,WORD wTex,WORD wMTex,WORD wCTex,WORD wDTex = 0);
	virtual bool CreateXML(CControl* pParent,CXmlButton* btn);
	virtual void Draw(void);
	virtual bool OnClick(int iButton);
	virtual void Destroy();
	virtual DWORD GetBtnTexID(E_BUTTON_TEX eBtnTex){ return (eBtnTex < BTEX_NUMS)?m_vTexID[eBtnTex]:0;}
protected:
	virtual void ReleaseButtonTex();
	virtual void LoadButtonTex(int &iW,int &iH);
	void		 ParseText(const char* pText);
public:
	void    SetAutoSwitch(bool bAuto){ m_bAutoSwitch = bAuto; }
	virtual bool IsSmall(void);
	virtual void ReloadTex(int iTexIndex);
	virtual void ReloadTex(WORD wTex,WORD wMTex,WORD wCTex,WORD wDTex = 0);
	virtual void SetTex(E_BUTTON_TEX iIndex,WORD wMTex);
	virtual WORD GetTex(E_BUTTON_TEX iIndex){ return m_vTexID[iIndex];}
	virtual void SetDisableTex(WORD wMTex){SetTex(BTEX_DISABLED,wMTex);}
	virtual void SetSwitchTex(WORD wTex,WORD wMTex,WORD wCTex,WORD wDTex = 0);
	virtual void SetState(bool _bSwitchState);
	virtual void SetEffectTex(WORD wEff,int iOffX,int iOffY);
	virtual void SetText(const char * sText,DWORD dwColor = COLOR_BTN_NORMAL,DWORD dwMouseOnColor = COLOR_BTN_MOUSEON,DWORD dwClickColor = COLOR_BTN_PRESS,DWORD dwDisableColor = COLOR_BTN_DISABLE,int iFontSize = FONTSIZE_DEFAULT,DWORD dwFlag = 0,int iFont = FONT_YAHEI,bool bVertical = false,int iOffX = 0,int iOffY = 0,int iGap = 0);
	virtual void SetBackState(E_BUTTON_TEX eState){m_eBackState = eState;}
	void SetTextAlignType(int val) { m_iTextAlignType = val; }
	void SetButtonType(int iType){m_iBtnType = iType;}
	int  GetButtonType(){return m_iBtnType;}

	DWORD GetMouseOnColor(){return m_dwMouseColor;}
	DWORD GetClickColor(){return m_dwClickColor;}
	DWORD GetDisableColor(){return m_dwDisableColor;}
	DWORD GetFlashTexID(){return m_dwFlashTexID;}
	void  SetFlashTexID(DWORD dwID){m_dwFlashTexID = dwID;}
	
	DWORD GetData(){ return m_dwData; }
	void  SetData(DWORD val) { m_dwData = val; }
};
