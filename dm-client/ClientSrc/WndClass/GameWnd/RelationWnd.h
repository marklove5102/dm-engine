#pragma once
#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlRadio.h"
#include "BaseClass/Control/CtrlMenuButton.h"
#include "BaseClass/Control/CtrlMultiEdit.h"
#include "BaseClass/Control/CtrlGrid.h"
#include "BaseClass/Control/CtrlTree.h"
#include "GameData/GuildData.h"
#include "BaseClass/Control/MarkViewer.h"
#include "GameData/Good.h"

#include <vector>
#include <string>

#define PAGE_STARTX 55
#define PAGE_STARTY 66
#define PAGE_WIDTH  68
#define PAGE_HEIGHT 20
#define PAGE_COUNT  5
#define PAGE_MAX_ROW 15

class CRelationWnd :public CCtrlWindowX
{
	DECLARE_WND_POSX(CRelationWnd)

public:
	CRelationWnd(void);
	~CRelationWnd(void);
	struct _SRelation
	{
		std::string sName;
		DWORD		iMuoQi;	// 默契度
		int			iJob;
		int			iLevel;
		int			iOnline;
		int			iPhyle;
		int			iJinJie;
		std::string sGuild;

		DWORD			iCurLive;
		DWORD			iYesterLive;

		_SRelation()
		{
			sName = "";
			iMuoQi = 0;
			iJob = 0;
			iLevel = 0;
			iOnline = 0;
			iPhyle = 0;
			iJinJie = 0;
			sGuild = "";

			iCurLive = 0;
			iYesterLive = 0;
		}
	};
	struct sHR
	{
		bool bStile;		//是否为封号名
		bool bOnline;		//是否在线
		bool bAssOnline;	//亲信是否在线
		CCtrlButton* pBtnAppoint;
		CCtrlButton* pBtnFavorite;
		string		 strName;
		string       strFavName;
		WORD		 wJob;
		DWORD		 dwColorName;
		DWORD		 dwColorFavName;
		sHR()
		{
			bStile = false;
			bOnline = false;
			bAssOnline = false;
			pBtnAppoint = NULL;
			pBtnFavorite = NULL;
			strName = "";
			strFavName = "";
			dwColorName = 0xFFFFFFFF;
			dwColorFavName = 0xFFFFFFFF;
		}
	};	
public:
	virtual void OnCreate(void);
	virtual void Draw();
	virtual bool Msg(DWORD dwMsg, DWORD dwData, CControl* pControl = NULL);
	virtual bool OnLeftButtonUp(int iX, int iY);
	virtual bool OnLeftButtonDown(int iX,int iY);
	virtual bool OnMouseMove(int iX,int iY);
	virtual bool OnLeftButtonDClick(int iX,int iY);
	virtual bool OnWheel(int iWheel);
	void OnSetFocus();
	void OnKillFocus();
	void OnMove();

	void   FillRelationData(int iType);
	void   RequestGuildMember();
	static std::string& GetAppointName(){return m_sStrAppointName;}

private:	
	void   OnDeleteFriend();
	void   OnTeamInvite();
	void   OnLeaveNote();
	void   OnToMaster(DWORD& dwTime);
	void   OnBackMaster(DWORD& dwTime);
	void   OnDeleteBlkList();
	void   OnDeletePhyle();

	//void   SwitchToPage(int iShowWnd,int iPage);
	void   CreateGuildWnd();
	void   CreateRelationWnd();
	void   CreateGuildTowerWnd();//创建行会塔窗口

	void   DealGuildMemberOnOrOffline(CGuildData::sTileNode* pNode);

	void   DrawRelationWnd();
	void   DrawGuildWnd();
	void   DrawGuildTower();

	void   FillBlackList();
	void   ReshHRScroll();
	void   ReshGuildControl();
	void   ConstructTree();
	void   RemoveBackDeal();//用来清除相关数据
	void   UpdateConstruct();
	void   ConstructPhyleTree();

	bool   DealRelationMsg(DWORD dwMsg, DWORD dwData, CControl* pControl = NULL);
	bool   DealGuildMsg(DWORD dwMsg, DWORD dwData, CControl* pControl = NULL);
	bool   DelAllySuccess(const char * strName);
	bool   AddAllySuccess(const char * strName);
	bool   DealGuildTowerMsg(DWORD dwMsg,DWORD dwData,CControl* pControl = NULL);

	int GetGrid(int iX, int iY,int *pRealMouseOnGrid = NULL);
	bool IsStop();

	void ContributeRes(bool bView = false);

	void OnClickCloseButton();
	void Send_Hero_List();//发送名单
	void NoteResult();
	void SetTabBtnFlashTexID(int iType);
	int  GetZhenBaoGeGrids();

	void OnClickTrans();
	void OnDeleteJieYi();
	void OnAddJieYi();

	bool				m_bNoCreate;
	//int 				m_iShowWnd;		//0表示显示的是江湖人脉，1表示显示的是行会
	//int					m_iCurPage;		//当前属性页
	int					m_iPublicChat;	//公告，聊天信息 //0是公告，1是聊天信息
	int					m_iCurShowRow;	//当前显示行
	//int					m_iCurSelRow;
	int                 m_iClickX,m_iClickY;//鼠标单击在屏幕中的位置，在绘制的时候看是不是点中某个成员
// 	bool				m_bTemp;
	bool				m_bRequestGuildPub;//是否请求过行会公告

	//LPTexture			m_pTextureChat;
	CCtrlGrid *   m_pGrid;//社会关系
	CCtrlGrid *   m_pGrid1;//联盟行会
	CCtrlGrid *   m_pGrid2;//敌对行会
	CCtrlTree *   m_pGuildTree;//行会树
	CCtrlRadio*			m_pRadioBtn;
	bool                m_bShowOffLineMember;
	DWORD				m_dwOnlineColor;
	DWORD				m_dwOfflineColor;
	vector<_SRelation>	m_vRelation;

	//CCtrlButton*		m_paTabRelBtn[5];
	//CCtrlButton*		m_paTabBgBtn[2];

	CCtrlButton* m_pAddFriend;
	CCtrlButton* m_pDeleteFriend;
	CCtrlButton* m_pAddToBlkList;
	CCtrlButton* m_pTeamInvite;
	CCtrlButton* m_pLeaveNote;
	//CCtrlButton* m_pToMasPreBrand;
	//CCtrlButton* m_pBackMasPreBrand;
	CCtrlButton* m_pRemove;

	//行会信息量
	CCtrlScroll* m_pGuildScroll01;
	CCtrlButton* m_pUpBtn01;
	CCtrlButton* m_pDownBtn01;
	CCtrlScroll* m_pGuildScroll02;
	CCtrlButton* m_pUpBtn02;
	CCtrlButton* m_pDownBtn02;

	CCtrlButton* m_pAddMemberBtn;
	CCtrlButton* m_pDelMemberBtn;
	CCtrlButton* m_pEditPublicBtn;
	CCtrlButton* m_pEditTitleBtn;
	CCtrlButton* m_pSetUnionBtn;
	CCtrlButton* m_pCancelUnionBtn;
	CCtrlButton* m_pGuildNoChatBtn;
	CCtrlButton* m_pPersonNoChatBtn;
	CCtrlButton* m_pQuitGuildBtn;

	//宗派
	CCtrlButton* m_pRecruitMem;
	CCtrlButton* m_pDismissMem;
	CCtrlButton* m_pPhyleOperate;//宗派操作，离开宗派或解散宗派
	//行会人事信息
	std::vector<sHR> m_vHR;//任命按钮
	CCtrlScroll* m_pScrollHR;//人事滚动条
	CCtrlButton* m_pHRUpBtn;//滚动条处上按钮
	CCtrlButton* m_pHRDownBtn;//滚动条向下按钮
// 	CCtrlButton* m_pAssignTael;//分配公共银两
	CCtrlButton* m_pHRUpDesBtn;//人事描术向上按钮
	CCtrlButton* m_pHRDownDesBtn;//人事描述向下按钮
	//CCtrlScroll* m_pScrollHRDes;//人事描述滚动条
	int			 m_iMouseInJob;//鼠标停在第几个职位，默认为0表示总描述
	vector<string> m_vStrJobDes;//职位描述
	CTagText	 m_TagText;//文本描述

	static string m_sStrAppointName;

	//行会塔
	CMarkViewer* m_pMarkViewer;
	CMarkViewer* m_pMarkViewer2;
	//点开NPC时总界面
	CCtrlButton* m_pContributeBtn;	//捐献资源
	CCtrlButton* m_pUpdateCons;		//升级建筑	
	//行会塔页签
	//1、行会灵塔
	//控件 捐献资源 升级建筑
	//2、珍宝阁
	//a、珍宝阁介绍 控件 开启珍宝阁
	CCtrlButton* m_pEnterTrea;		//进入珍宝阁
	//b、珍宝阁
	//c、极品阁
	//3、神工坊
	//a、神工坊简介 控件 获得盾牌 提升品质
	//b、获得盾盾   控件 获得盾牌
	//c、提升品质   控件 炼制
	CCtrlButton* m_pGetProperty;//获得道具
	CCtrlButton* m_pPropertyRefine;//道具炼制
	//CCtrlButton* m_pGetGoldLotus;//获得金莲
	//CCtrlButton* m_pGetBlackLotus;//获得黑莲

	CCtrlButton* m_pGetShield;
	CCtrlButton* m_pUpdateShield;
	CCtrlButton* m_pShengWangUpdate;
	CCtrlButton* m_pRefineShield;
	//4、议事厅
	//a、任务面板   控件 接取 完成
	//b、任务完成

	//捐献资源
	CCtrlButton* m_pViewRes;//查看资源

 	CCtrlScroll* m_pScrollTask;//任务列表
// 	CCtrlButton* m_pUpTask;
// 	CCtrlButton* m_pDownTask;

// 	CCtrlScroll* m_pScrollDes;//任务描述
// 	CCtrlButton* m_pUpDes;
// 	CCtrlButton* m_pDownDes;

	CCtrlButton* m_pAcceptTask;//接取任务
	CCtrlButton* m_pFinishTask;//完成任务	
	CCtrlButton* m_pOpenPrice;//开启每日福利
	CCtrlButton* m_pGetPrice;//领取每日福利

	CCtrlRadio*  m_pArrayFlag[4];//旗帜

	//仙灵碑
	CCtrlButton* m_pSteleLand;//挑战仙灵大陆
	CCtrlButton* m_pSteleHero;//挑战精英战将
	CCtrlButton* m_pSteleBani;//发动动员令
	CCtrlButton* m_pAddHero;//增加精英
	CCtrlButton* m_pRemoveHero;//删除精英
	CCtrlButton* m_pSteleAssess;//评定战斗值
	CCtrlScroll* m_pFghValueScroll;
	CCtrlButton* m_pFghValueUpBtn;
	CCtrlButton* m_pFghValueDwnBtn;
	CCtrlButton* m_pBtnArray[8];


	CCtrlButton* m_pEnterMagicAera;//进入魔域
	CCtrlButton* m_pSteleMercenary;//了解仙林大陆

	vector<string> m_vHeroList;//选中的战将名单
	vector<string> m_vGuildMember;//行会列表

	CCtrlScroll* m_pScrollStelHero;
	CCtrlButton* m_pStelHeroUpBtn;
	CCtrlButton* m_pStelHeroDwnBtn;
	CCtrlScroll* m_pScrollGuildStel;
	CCtrlButton* m_pStelGuildUpBtn;
	CCtrlButton* m_pStelGuildDwnBtn;
	int			 m_iSelectGuild;//行会选中
	int			 m_iSelectHero;//英雄选中

	int  m_iFrame;
	bool m_bSendButtonClick;
	CGood m_tmpShield;		//缓存盾牌good，炼制失败后也能显示
	//int m_iRandomNum;
	//int m_iRound;
	WORD m_wSelTask;//选中的任务
// 	bool m_bClickDes;//是否点击了描述框

	CGood		 m_good;			//要捐赠的物品
	int  m_iTowerLevelUpFrame;
	int  m_iSkullFrame;
	int  m_iFireFrame;
	int  m_iShineFrame;
	bool m_bRequestedGuildMsg;//打开窗口后是否请求过行会公告


	CCtrlButton* m_pJieYiTrans;		// 结义传送
	CCtrlButton* m_pDeleteJieYi;	// 删除结义
	CCtrlButton* m_pJieYiTeamInvite;// 组队邀请

	CCtrlButton* m_pAddJieYi;		// 结义

	DWORD			m_dwCurLive;
	DWORD			m_dwYesterLive;
	bool			m_bJieYi;

	CMarkViewer* m_pMarkViewerJieYi;
};
