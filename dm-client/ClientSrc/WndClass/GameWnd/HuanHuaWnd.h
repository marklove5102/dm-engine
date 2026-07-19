#pragma once
#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlButton.h"
#include "GameData/Good.h"

class CHuanHuaWnd : public CCtrlWindowX
{
    DECLARE_WND_POSX(CHuanHuaWnd)
public:
    CHuanHuaWnd(void);
    ~CHuanHuaWnd(void);

    virtual void Draw(void);
    virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
    virtual void OnCreate();
    virtual bool OnLeftButtonUp(int iX, int iY);
	virtual bool OnMouseMove(int iX,int iY);
    void DrawPetPic(CGood& good,int iX,int iY,bool bIsFirstPet,byte byAlpha = 255);
	void PlayEffect();	
protected:
	bool IsCanHuanHua(CGood& good,int iGrid);
	void StartHuanHua();
    CCtrlButton * m_pOkButton;
    CCtrlButton * m_pCancelButton;

    bool m_bClickOKButton;
	bool m_bPetPicFlash;
	bool m_bSendHuanHua;
	bool m_bPlayEffect;
	bool m_bGradualDisappear;
	bool m_bMouseOnFirstPet;
	bool m_bMouseOnSecondPet;

    RECT m_recGrids[3];

	int m_iFrameCount;

	CGood m_DisappearPet;
};
