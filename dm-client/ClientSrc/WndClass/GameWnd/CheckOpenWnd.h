#pragma once
#include "BaseClass/Control/CtrlButton.h"
#include "BaseClass/Control/CtrlRadio.h"
#include "BaseClass/Control/CtrlWindowX.h"
#include <string>

class CCheckOpenWnd : public CCtrlWindowX
{
    DECLARE_WND_POSX(CCheckOpenWnd)
public:
    CCheckOpenWnd();
    ~CCheckOpenWnd(void);
    virtual void OnCreate();    
    virtual bool Create(CControl* pParent,int iCurPage);
    virtual void OnClickCloseButton();
    virtual void Draw();
    virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
private:
    CCtrlRadio	* m_pNoPopRadio;
    CCtrlButton  * m_pOkButton;
    CCtrlButton  * m_pCancelButton;
    int m_iType;
    std::string m_strText;
};

