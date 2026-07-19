#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/MarkViewer.h"
#include "BaseClass/Control/CtrlGrid.h"
#include "GameData/TagText.h"
#include "GameData/PetData.h"
#include "GameData/WooolStoreData.h"
#include "BaseClass/Control/CtrlMenuWnd.h"

#define MAX_PETS_NUM 60

class CPetWnd:public CCtrlWindowX
{
	DECLARE_WND_POSX(CPetWnd)

public:
	CPetWnd(void);
	~CPetWnd(void);
public:
	virtual void Draw(void);
	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual bool OnMouseMove(int iX,int iY);
	virtual bool OnLeftButtonDown(int iX, int iY);
	virtual bool OnLeftButtonUp(int iX, int iY);
	virtual bool OnLeftButtonDClick(int iX,int iY);
	void UpdatePetList();
	
protected:
	void CreateWnd();
	bool DealPetListMsg(DWORD dwMsg,DWORD dwData,CControl* pControl);
	bool DealPetAdoptMsg(DWORD dwMsg,DWORD dwData,CControl* pControl);
	bool DealPetFosterMsg(DWORD dwMsg,DWORD dwData,CControl* pControl);


	void DrawPetListWnd();
	void DrawPetPkgWnd();
	void DrawPetAdoptWnd();
	void DrawPetFosterWnd();

	void DrawHelpMsg();
	void DrawWaitReading();
	void DrawAdoptPetInfo();
	void DrawEndAdoptPetInfo();
	void DrawEndAdoptLineData(int i,PET_ADOPT &petData);


	void OnRideBtnClicked(int iIndex);
	void OnReleaseExp(int iIndex);
	bool FeedPet(DWORD dwID);
	void OnFeedPetUseAllFood(int iIndex);
	void OnFollowBack(int iIndex);
	void OnHelpBtn();
	void UpdateBtnState();
	void OnClickMenuButton(int iPos);
protected:
	int					m_iCurPage;
	CCtrlButton* m_pHelpBtn;//帮助
	CCtrlButton* m_pCloseHelpBtn;//关闭帮助按钮
	CMarkViewer* m_pMarkViewer;
	//宠物列表
	CCtrlButton* m_pReleaseExpBtn[3];//释放经验
	CCtrlButton* m_pFollowBackBtn[3];//跟随(收回)
	CCtrlButton* m_pFeedBtn[3];//喂食
	CCtrlButton* m_pBeckon[3];
	//CCtrlButton* m_pBoothBtn[3];//守摊
	//CCtrlButton* m_pRideBtn[3];//骑乘
	//CCtrlButton* m_pMerRideBtn[3];   //元神骑乘
	CCtrlButton* m_pFunc[3];   //功能按钮

	int					m_iIniSendNum;//最初有几个被释放出来
	int m_iClothPos;//御兽开袍／衣 在包裹中的位置
	vector<DWORD>		m_vectorPet;
	bool				m_bIniHorseSendout;

	DWORD				m_dwCurUsing;
	int					m_iCurUsingPos;
	CTagText			m_npcText;

	CCtrlButton* m_pPetListUpBtn;
	CCtrlButton* m_pPetListDownBtn;
	CCtrlScroll*		m_pPetListScroll;//滚动条控件

	CGood*				m_pGood[MAX_PETS_NUM];
	int					m_iPkgPos[MAX_PETS_NUM];
	int					m_iPetNum;
	DWORD				m_dwLastCheckFeedTime;//最后一次检查是否解包豹粮捆是否完成的时间

	bool				m_bFollow;
	bool				m_bFeed;
	bool				m_bRide;
	bool                m_bMerRide;
	int					m_iHorsePos;//马的位置
	DWORD				m_dwPetID[3];
	void DrawOnLivePetData(DWORD dwPetID,int iPos);
	void DrawOnLiveHorseData(int iPos);			//iPos 指处于当前页的序号
	void DrawOffLinePetData(DWORD dwPetID,int iPos);
	void DrawPetData(DWORD dwPetID,int iPos);
	void DrawPetEmptyData(int iPos);

	//宠物包裹
	CCtrlButton* m_pExtendPkgBtn;//扩充宠物包裹
	CCtrlButton* m_pMoreShorCut;//连到商城
	int					m_iType;   //类型，1：5个格子，2：10个格子
	int					m_iCellW,m_iCellH;				//单元格的宽度和高度
	int					m_iWCells,m_iHCells;			//水平和垂直方向的格数
	int					m_iCellDisW,m_iCellDisH;		//单元格水平和垂直的间隔
	POINT				m_ptGoodPoint;		    	    //物品栏左上角点坐标
	bool				m_bDClicked;					//是否是双击左键
	void				GetGoodGrid(int* iX, int* iY, int ix, int iy);	//取得物品所在单元格
	bool				IsInGoodGrid(int ix,int iy);    //是否在物品栏内
	CQuickItem			m_QuickItem;
	std::string			m_strID;

	//宠物领养所
	CCtrlButton*		m_pEndAdoptBtn[5];
	CCtrlButton*		m_pAdoptBtn[5];

	CCtrlGrid*			m_pGridEndAdopt;
	CCtrlGrid*			m_pGridAdopt;

	DWORD				m_LastSendTick;
	bool				m_bSending;
	int					m_nWaitReading;
	int					m_oldPos_Adopt;

	void				DrawLineData(int i,PET_ADOPT &petData);
	void				UpdateAdoptPetInfo();

	//宠物寄养所
	CCtrlButton*		m_pFosterBtn[5];
	CCtrlButton*		m_pEndFosterBtn[5];	

	int					m_oldPos;
	CCtrlGrid*			m_pGridCarryBack;
	CCtrlGrid*			m_pGridFosterage;

	void DrawFosteragePetInfo();
	void DrawCarryBackInfo();
	void DrawFosterLineData(int i,PET_ADOPT &petData);
	void DrawFosterageLineData(int i,CGood &good);
	void FosteragePet(int i);	

	void OnExplore(int i);

	DWORD               m_dwTickCount;

	int m_iPos;     //保存弹出菜单的行数

	CCtrlMenuButtonWnd * m_pMenu;
	CCtrlButton * m_pArrow[3];
};
