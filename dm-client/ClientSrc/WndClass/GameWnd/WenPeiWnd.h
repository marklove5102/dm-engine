#pragma once
#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/MarkViewer.h"
#include "GameData/TagText.h"


#define  PAGE_INDEX_HECHENG         0 //Œ∆≈Â∫œ≥…“≥«©–Ú∫≈
#define  PAGE_INDEX_FENJIE          1 //Œ∆≈Â∑÷Ω‚“≥«©–Ú∫≈
#define  PAGE_INDEX_INFO			2 //Œ∆≈Â◊¥Ã¨“≥«©–Ú∫≈

#define	 PAGE_LARGEICON_START_X		50
#define	 PAGE_LARGEICON_START_Y		70
#define	 PAGE_LARGEICON_CELL_W		72
#define	 PAGE_LARGEICON_CELL_H		72
#define	 PAGE_LARGEICON_INTER_W		5
#define	 PAGE_LARGEICON_INTER_H		5
#define	 PAGE_LARGEICON_ROWS		2
#define	 PAGE_LARGEICON_COLS		5

#define	 PAGE_SMALLICON_START_X		45
#define	 PAGE_SMALLICON_START_Y		68
#define	 PAGE_SMALLICON_CELL_W		36
#define	 PAGE_SMALLICON_CELL_H		36
#define	 PAGE_SMALLICON_INTER_W		3
#define	 PAGE_SMALLICON_INTER_H		3
#define	 PAGE_SMALLICON_ROWS		4
#define	 PAGE_SMALLICON_COLS		10

#define	 PAGE_INFO_START_X			56
#define	 PAGE_INFO_START_Y			73
#define	 PAGE_INFO_CELL_W			72
#define	 PAGE_INFO_CELL_H			72
#define	 PAGE_INFO_INTER_W			120
#define	 PAGE_INFO_INTER_H			16
#define	 PAGE_INFO_ROWS				3
#define	 PAGE_INFO_COLS				2

// Œ∆≈Â
class CWenPeiWnd : public CCtrlWindowX
{
	DECLARE_WND_POSX(CWenPeiWnd)

public:
	CWenPeiWnd(void);
	~CWenPeiWnd(void);

	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual void Draw(void);
	virtual bool OnMouseMove(int iX, int iY);
	virtual bool OnLeftButtonDown(int iX, int iY);
	virtual bool OnLeftButtonUp(int iX,int iY);

protected:
	int GetIndex(int iX,int iY);
	int GetTaskProgressIndex(int iX,int iY);

	void DrawSubWenPei(int x, int y, int looks, LPPOS pScale = NULL, int type = 1);
	void SDrawGoodEffect(const char *strName,int iX,int iY,int dwClientLooks,LPPOS pScale = NULL,int iType = 1);

	void UpdateWenPeiInfo();

	void SetCtrlAll0();

	CCtrlScroll * m_pScroll;

	CCtrlButton* m_pHeCheng1;
	CCtrlButton* m_pHeCheng;
	CCtrlButton* m_pFenJie;
	CCtrlButton* m_pTiQu;
	
	int m_iSelectIndex[2][4];

	CCtrlButton* m_pTaskBtn[PAGE_INFO_ROWS][PAGE_INFO_COLS];

	int m_iSuccessFrame;//∫œ≥…≥…π¶
};
