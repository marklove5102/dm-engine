#pragma once
#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlGrid.h"
#include "BaseClass/Control/MarkViewer.h"
#include "GameData/TagText.h"


#define M_FRIEND_LINE_COUNT 4

class CYuShouLevelUpWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CYuShouLevelUpWnd)
public:
	struct _SRelation
	{
		std::string sName;
		int			iOnline;

		_SRelation()
		{
			sName = "";
			iOnline = 0;
		}
	};


	CYuShouLevelUpWnd(void);
	~CYuShouLevelUpWnd(void);

	virtual void OnCreate();
	virtual void Draw(void);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	virtual bool OnMouseMove(int iX,int iY);
	virtual bool OnLeftButtonUp( int iX, int iY );
	virtual bool OnLeftButtonDown(int iX, int iY);
	virtual bool OnWheel(int iWheel);

protected:
	void SetAll0();
	void DrawFriendList();
	void UpdateFriendList();
	void UpdateBtnState();
	void UpdateLog();

	void DoChaFriend();

	CCtrlButton* m_pStartButton;
	CCtrlButton* m_pChanZaButton;
	CCtrlButton* m_pTiChunButton;
	CCtrlButton* m_pChaButton;

	CMarkViewer* m_pMarkViewer;
	CTagText	 m_kText;

	CCtrlGrid *   m_pPlayerGrid;

	vector<_SRelation>	m_vRelation;
};
