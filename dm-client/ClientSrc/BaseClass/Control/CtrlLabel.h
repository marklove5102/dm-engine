#pragma once
#include "control.h"
#include "XmlControl.h"

class CCtrlLabel :	public CControl
{
	DTI_DECLARE(CCtrlLabel, CControl)

public:
	CCtrlLabel(void);
	~CCtrlLabel(void);

public:
	virtual bool Create(CControl* pParent,int iX,int iY,int iW,int iH,WORD wTexx = 0,WORD wMTexx = 0,WORD wCTexx = 0,WORD wDTex = 0);
	virtual bool CreateXML(CControl* pParent,CXmlLabel* btn);
	virtual void Draw(void);
	virtual bool OnClick(int iLabel);
	virtual void Destroy();
	virtual DWORD GetBtnTexID(E_BUTTON_TEX eBtnTex){ return (eBtnTex < BTEX_NUMS)?m_vTexID[eBtnTex]:0;}

	virtual void ReloadTex(int iTexIndex);
	virtual void ReloadTex(WORD wTex,WORD wMTex,WORD wCTex,WORD wDTex = 0);
	virtual void SetTex(E_BUTTON_TEX iIndex,WORD wMTex);
	virtual WORD GetTex(E_BUTTON_TEX iIndex){ return m_vTexID[iIndex];}
	virtual void SetDisableTex(WORD wMTex){SetTex(BTEX_DISABLED,wMTex);}
	virtual void SetEffectTex(WORD wEff,int iOffX,int iOffY);
	virtual void SetText(const char * sText,DWORD dwColor = COLOR_TEXT_NORMAL,DWORD dwMouseOnColor = COLOR_TEXT_NORMAL,DWORD dwClickColor = COLOR_TEXT_NORMAL,DWORD dwDisableColor = COLOR_TEXT_DISABLE,int iFontSize = FONTSIZE_DEFAULT,DWORD dwFlag = 0,DWORD dwMouseOnFlag = 0,DWORD dwClickFlag = 0,int iFont = FONT_YAHEI,bool bVertical = false,int iOffX = 0,int iOffY = 0);
	void SetTextAlignType(int val) { m_iTextAlignType = val; }
	DWORD GetMouseOnColor(){return m_dwMouseColor;}
	DWORD GetClickColor(){return m_dwClickColor;}
	DWORD GetDisableColor(){return m_dwDisableColor;}


protected:
	//virtual void ReleaseLabelTex();
	virtual void LoadLabelTex(int &iW,int &iH);


protected:
	//LPTexture	m_vTexs[BTEX_NUMS];		// 按钮控件的帧数
	WORD        m_vTexID[BTEX_NUMS*2];
	int			m_iIndex;
	//LPTexture   m_pTexEff;  //特效图片
	DWORD       m_dwTexEffID;//特效图片ID
	int         m_iEffOffX,m_iEffOffY;  // 特效图片的偏移
	DWORD       m_dwMouseColor;
	DWORD       m_dwClickColor;
	DWORD       m_dwDisableColor;
	bool        m_bVertical;//文字是否为垂直排列
	int         m_iLines;//垂直排列时的行数
	int         m_iTextAlignType;       //在父窗口中的排列方式,XALINEMENT之一
	DWORD       m_dwMouseOnFontFlag,m_dwClickFlag;// 绘制字体的属性,eDrawTextFlag类型

};
