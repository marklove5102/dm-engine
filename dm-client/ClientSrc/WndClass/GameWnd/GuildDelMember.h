#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlGrid.h"
#include <vector>

class CGuildDelMember : public CCtrlWindowX
{
	DECLARE_WND_POSX(CGuildDelMember)

public:
	CGuildDelMember(void);
	~CGuildDelMember(void);
	virtual void OnCreate();
	virtual void Draw(void);
	virtual bool OnLeftButtonDown(int iX,int iY);
	virtual bool OnWheel(int iWheel);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);

	void		 SetType(int i){m_iType = i;}
	int			 GetType(){return m_iType;}
private:
	void OnForbid();
	void OnAppoint();
	void OnAssginTael();
	void OnDelGuildMember();
	int InWhichArea(int,int);
private:
	int					m_iType;	
	CCtrlGrid*			m_pGrid;
	CCtrlEdit*			m_pEdit;
	CCtrlButton*  m_pDel;
	CCtrlButton*  m_pCancel;
	//CCtrlButton * m_pUp;
	//CCtrlButton * m_pDown;
	std::vector<std::string> m_players;

private:
	UINT	m_start;
	int		m_select;
	WORD    m_wData;
};
