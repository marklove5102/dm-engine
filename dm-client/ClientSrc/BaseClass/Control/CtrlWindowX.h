#pragma once

#include "CtrlWindowS.h"
#include "XmlControl.h"
#include "UiManager.h"
#include "CtrlScroll.h"
#include "CtrlRadio.h"
#include "CtrlEdit.h"
#include "CtrlButton.h"


#define DECLARE_WND_POSX(_class_name)	\
	public:\
	static CCtrlWindowX* CreateObject(); \
	DECLARE_WND_POS_SPEC(_class_name, CCtrlWindowX)

//
#define INIT_WND_POSX(_class_name, x, y)	\
	INIT_WND_POS_SPEC(_class_name, CCtrlWindowX, x, y) \
	CCtrlWindowX* _class_name::CreateObject()\
	{ return new _class_name;}

//

//从程序创建时要有如下所示设置m_iIndex = 10891;m_iPages = 1;m_uStyle = CTRL_STYLE_BACKMODE_TEX | CTRL_STYLE_SELFDELETE_BACKTEX;
//从xmlwindow创建是不需要设置,以xml中配置的为准
class CCtrlWindowX : public CCtrlWindowS
{
	DTI_DECLARE(CCtrlWindowX,CCtrlWindowS)
public:
	CCtrlWindowX(void);
	~CCtrlWindowX(void);

	virtual bool Create(CControl* pParent,int iCurPage = 0);
	virtual bool CreateXML(CControl* pParent,CXmlWindow* win);
	virtual void OnCreate();
	virtual void OnClickCloseButton();

	void CloseWindow(BOOL bOK = FALSE);

	CCtrlScroll* FindScrollByName(const char* name); 
	CCtrlRadio*  FindRadioByName(const char* name);
	CCtrlButton* FindButtonByName(const char* name);
	CCtrlEdit*   FindEditByName(const char* name);

	CXmlWindow* GetXmlWindow(){return m_pXmlWindow;}

	bool IsAutoCloseWhenTrusteeship() { return m_bAutoCloseWhenTrusteeship; }
	void SetAutoCloseWhenTrusteeship(bool val) { m_bAutoCloseWhenTrusteeship = val; }
protected:
	bool CreateChild(CXmlPage* page);
	CXmlWindow* m_pXmlWindow;  //页签

	bool m_bAutoCloseWhenTrusteeship;//托管之后这个窗口是否开启后自动关闭
};

