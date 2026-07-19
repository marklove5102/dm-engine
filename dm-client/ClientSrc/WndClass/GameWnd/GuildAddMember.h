#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlGrid.h"
#include <vector>

class CGuildAddMember : public CCtrlWindowX
{
	DECLARE_WND_POSX(CGuildAddMember)

public:
	CGuildAddMember();
	virtual void Draw(void);
	virtual bool OnLeftButtonDown(int iX,int iY);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	virtual void OnCreate();
	virtual bool OnWheel(int iWheel);

	void		 SetType(int i){m_iType = i;}
	int			 GetType(void){return m_iType;}
private:
	void OnAddGuildMember();
	void OnAddFriend();
	void OnPhyleAdd();
	void OnAddBlkList();
	int InWhichArea(int,int);
private:
	std::vector<std::string> m_players;
	CCtrlButton * m_pAdd;
	CCtrlButton*  m_pCancel;
	CCtrlGrid*	  m_pGrid;
	//CCtrlButton * m_pUp;
	//CCtrlButton * m_pDown;
	//CCtrlScroll*		m_pScroll;
	CCtrlEdit*			m_pEdit;

private:
	UINT	m_start;
	int		m_select;
	int		m_iType;
};