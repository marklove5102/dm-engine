#pragma once

#include "BaseClass/Control/Control.h"
#include "Global/Interface/FontInterface.h"

#define DECLARE_WND_POS(_class_name)	\
	DECLARE_WND_POS_SPEC(_class_name, CCtrlWindowS)
//
#define INIT_WND_POS(_class_name, x, y)	\
	INIT_WND_POS_SPEC(_class_name, CCtrlWindowS, x, y)
//
#define DECLARE_WND_POS_SPEC(_class_name, _super_name) public: \
	static  POINT sm_##_class_name##Pos;	\
	public:	\
	virtual inline int	GetWndSX() const { return _class_name::sm_##_class_name##Pos.x;}	\
	virtual inline int	GetWndSY() const { return _class_name::sm_##_class_name##Pos.y;}	\
	DTI_DECLARE(_class_name, _super_name)	\
	private:
//
#define INIT_WND_POS_SPEC(_class_name, _super_name,  x, y) \
	POINT _class_name::sm_##_class_name##Pos = {x,y};	\
	DTI_IMPLEMENT(_class_name, _super_name)


//单个页签数据结构
struct S_TabPage
{
	S_TabPage()
	{
		iBackTexX = iBackTexY = 0;
		dwBackTex = 0;
		pParentTabPage = NULL;
		pTabBtn = NULL;
		iCurPage = 0;


		iTabBtnTexX = iTabBtnTexY = 0;
		wTabBtnTex = wTabBtnMonuseOnTex = wTabBtnClickedTex = wTabBtnTexDisableTex = 0;
		bTabTextVertical = false;

		dwTabTextColor = COLOR_TABPAGE_NORMAL;
		dwTabTextMouseOnColor = COLOR_TABPAGE_MOUSEON;
		dwTabTextClickColor = COLOR_TABPAGE_PRESS;
		dwTabTextDisableColor = COLOR_TABPAGE_DISABLE;

		iTabTextFontSize = FONTSIZE_MIDDLE;
		iTabTextFont = FONT_YAHEI;
		dwTabTextFlag = 0;
		iTabTextGap = 0;
		iTextOffX = 0;
		iTextOffY = 0;
		iSwitchPageType = 0;
		bReCreateSamePageWhenClickTabBtn = false;
		szTips = "";
	}

	int iBackTexX,iBackTexY;//背景纹理的在整个窗口中的起画位置
	DWORD dwBackTex;//背景纹理id;

	int iTabBtnTexX,iTabBtnTexY;//页面切换按钮在整个窗口中的位置
	WORD wTabBtnTex,wTabBtnMonuseOnTex,wTabBtnClickedTex,wTabBtnTexDisableTex;//页面切换按钮纹理图
	string strTabText;//页面切换按钮上面的文字
	bool bTabTextVertical;//页面切换按钮上面的文字是否垂直绘制
	DWORD dwTabTextColor,dwTabTextMouseOnColor,dwTabTextClickColor,dwTabTextDisableColor;//页面切换按钮上面的文字绘制颜色
	int iTabTextFontSize;//页面切换按钮上面的文字字体大小
	DWORD dwTabTextFlag;//页面切换按钮上面的文字绘制参数
	int iTabTextFont;//页面切换按钮上面的文字字体
	int iTabTextGap;//页面切换按钮上面的文字绘制间隔
	int iTextOffX,iTextOffY;//页面切换按钮上面的文字绘制偏移
	int iSwitchPageType;//这个页签按钮点了是否要切换页签,0,普通,点了立即切换.如果为1,不会马上切换页签,先给继承窗口发送一个MSG_CTRL_TABPAGE_TABCHANGE消息,返回true后才切换,返回false,不切换
	bool bReCreateSamePageWhenClickTabBtn;//当前页签和要切换的页签一样时要不要重新create
	string szTips;//按钮的tips


	S_TabPage *pParentTabPage;//父页签
	CCtrlButton *pTabBtn;//页签按钮,点了后切换到此页签
	vector<S_TabPage> vSubTabPage;//子页签
	int iCurPage;//子页签当前选中项
};



class CCtrlWindowS : public CCtrlWindow
{
	DTI_DECLARE(CCtrlWindowS, CCtrlWindow)
protected:
	int m_iOldx,m_iOldy;						// 前一次检查时得到的鼠标位置
	int m_iClickX,m_iClickY;					// 点击时的坐标
	bool m_bMoving;                             // 正在移动

	int			m_iIndex;						// 帧索引位置
	int			m_iPages;						// 页数
	int			m_iCurPage;						// 当前页
	vector<DWORD>	m_vTexturesID;  			// 纹理数组
	vector<RECT>		m_vRect;				// 区域数组
	vector<int>         m_vIndex;               // 每一个页签对应的Index

	bool m_bNeedSavePage;//是否要保存最后处于哪个页签，设置了则下次打开窗口时自动切换到该页签
	bool m_bNeedSavePos;//是否保存上一次该窗口的位置,true则下次打开窗口时移动到上一次关闭时的位置

	int  m_iVersion;//版本号，1:老版本的，2:新版本的
	S_TabPage m_TabPage;//新版本中的页签参数数据
	int m_iInputOffX,m_iInputOffY;//创建时传的不X,Y


	void SaveWndPos();

public:
	CCtrlWindowS(void);
	virtual ~CCtrlWindowS(void);

	virtual int  PointInPage(int iX, int iY);
	virtual void SwitchToPage(int iPage,S_TabPage * pParentTabPage = NULL,bool bReCreate = false);//bReCreate为true时就算要切换的页就是当前页也重新创建
	virtual bool Create(CControl* pParent, int iX = POS_AUTO,int iY = POS_AUTO,int iCurPage = 0);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);

	virtual int  GetCurPage();

	// 创建窗口，参数为：父控件、z坐标、左上角x坐标、y坐标、宽度、高度、风格、背景颜色、背景纹理
	virtual bool Create(CControl * pParent,int iX,int iY,int iW,int iH,
		UINT uStyle=CTRL_STYLE_BACKMODE_NODRAW,DWORD dwBackcolor=0xFFFFFFFF,DWORD dwBacktexID = 0)
	{
		m_uStyle = uStyle;
		m_dwBackColor = dwBackcolor;
		m_dwBackTexID = dwBacktexID;

		return this->Create(pParent,iX,iY); //向下兼容父类
	}
	virtual void RemoveBodyChildControl(void); //删除所有子控件

	virtual void OnCreate();
	virtual void Draw();
	virtual bool OnLeftButtonDown(int iX, int iY);
	virtual bool OnLeftButtonUp(int iX, int iY);
	virtual void DrawBkg();

	// 处理公共部分的控件
	void SetPageRect(int iPage,int left,int top,int right,int bottom);//设置ipage的页面切换区域
	void SetPageTex(int iPage,int iPackage,WORD wIdx);//设置ipage的纹理
	//void SetPageTex(int iPage,LPTexture pTex);//设置ipage的纹理

	//新版本添加页签函数,参数意义见S_TabPage,返回新加的页签数据结构指针,单个页签的界面也可以用这个函数
	virtual S_TabPage * AddTabPage(int iBackTexX,int iBackTexY,DWORD dwBackTex,int iTabBtnTexX,int iTabBtnTexY,WORD wTabBtnTex,WORD wTabBtnMonuseOnTex,WORD wTabBtnClickedTex,WORD wTabBtnTexDisableTex,const char *strTabText,S_TabPage *pParentTabPage = NULL,bool bTabTextVertical = false,int iTextOffX = 0,int iTextOffY = 0,int iSwitchPageType = 0,int iTabTextFont = FONT_YAHEI,int iTabTextFontSize = FONTSIZE_MIDDLE,bool bReCreateSamePageWhenClickTabBtn = false,const char * pTips = NULL);
	virtual void SetTabText(S_TabPage *pTabPage,const char * sText,DWORD dwTabTextColor = COLOR_TABPAGE_NORMAL,DWORD dwTabTextMouseOnColor = COLOR_TABPAGE_MOUSEON,DWORD dwTabTextClickColor = COLOR_TABPAGE_PRESS,DWORD dwTabTextDisableColor = COLOR_TABPAGE_DISABLE,int iTabTextFontSize = FONTSIZE_DEFAULT,DWORD dwTabTextFlag = 0,int iTabTextFont = FONT_DEFAULT,bool bTabTextVertical = false,int iTabTextGap = 0,int iTextOffX = 0,int iTextOffY = 0,const char * pTips = NULL);
	virtual void ReCreateTabBtn();//有些地方切换了页签不想调用OnCreate又想页签按钮发现变化可以调用这个函数,选调用ClearTabBtn,再调用CCtrlWindowS::OnCreate
	virtual void ClearTabBtn(S_TabPage *pTabPage);//清除所有页签按钮
	virtual void ResetWidthAndHeight(int iW,int iH);
	virtual bool RelativeMove(int iType = XAL_NONE);

public:
	virtual inline int	GetWndSX() const { return POS_AUTO;}
	virtual inline int	GetWndSY() const { return POS_AUTO;}
};
