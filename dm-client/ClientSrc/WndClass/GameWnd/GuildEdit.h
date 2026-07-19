#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlMultiEdit.h"
#include "GameData/GuildData.h"
class CGuildEdit: public CCtrlWindowX
{
	DECLARE_WND_POSX(CGuildEdit)

public:
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	bool Create(CControl * pParent,int iX,int iY);
	virtual void OnCreate();
	virtual void Draw();
	virtual bool OnLeftButtonDown(int iX, int iY);
	virtual bool OnLeftButtonUp(int iX, int iY);
	virtual bool OnMouseMove(int iX, int iY);

	CGuildEdit();
	~CGuildEdit(void);
protected:
	bool SecurityCheck();//安全性检验
	bool ConstructTile();//构造原始数据

private:
	CCtrlMultiEdit		*	m_pEdit;
	CCtrlButton*	m_pOk;
	CCtrlButton* m_pCancel;
	CCtrlButton* m_pSearchBtn;
	CCtrlButton* m_pSearchLastBtn;
	CCtrlEdit*          m_pSearchEdit;
	int                 m_iSearchLine;

	//滚动条
	//CCtrlButton* m_pUpButton;
	//CCtrlButton* m_pDownButton;
	int   m_iSliderX,m_iSliderY;
	int   m_iSliderBegin,m_iSliderEnd;
	bool  m_bSliderClick;

	int	  m_state; //类型，1：封号，2：公告
	std::string m_strSearch;
	int    m_iLastFindPos;
	std::vector<CGuildData::sTileNode> m_vNewTile;
};
