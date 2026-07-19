/*
Control.h 为系统基础控件的头文件，此文件中定义的控件为基础控件，
具体使用时，应该生成基础控件的派生控件而不应该直接生成基础控件的实例。
*/
#pragma once

#include "ControlDefine.h"
#include "Global\Global.h"
#include "Global/GlobalMsg.h"
#include "Global/Timer.h"
#include "Global\DebugTry.h"
#include "BaseClass/Misc/Input.h"
#include "CtrlTag.h"
#include "XmlControl.h"

#include "Global/Interface/TexManagerInterface.h"
#include "Global/Interface/FontInterface.h"
#include "Global/Interface/GraphicInterface.h"
#include "Global/Interface/TextureInterface.h"


//如果存在对应的xmlwindow以xml配置的为准，否则在CCtrlWindowS::Create中，
//如果iX或iY == POS_SETPOS_AUTO,那么就默认居中,
//在各个CtrlWindowX中调用INIT_WND_POSX(XXXXX,POS_SETPOS_AUTO,POS_SETPOS_AUTO)创建时就会居中

//如果存在对应的xmlwindow以xml配置的为准，否则在CCtrlWindowS::Create中，
//如果iX或iY == POS_SETPOS_VARIABLE,那么就根据m_iAlignType，m_iOffX，m_iOffY
//这几个变量的值来调整窗口的创建位置,
//在各个CtrlWindowX中调用INIT_WND_POSX(XXXXX,POS_SETPOS_VARIABLE,POS_SETPOS_VARIABLE)
//并设定m_iAlignType，m_iOffX，m_iOffY三个变量的值

#define  POS_NOMOVE   2000           //在控件的move,onmove中，如果传入的参数>=POS_NOMOVE则不执行
#define  POS_NORESIZE -2000          //在控件的ReSize中，如果传入的参数<=POS_NORESIZE则不执行
#define  POS_AUTO     -2000       
#define  POS_VARIABLE -3000   //

class CCtrlMainWin;
class CCtrlWindowX;

//动态创建控件实例的函数指针(每个从CtrlWindowX都有一个这样的静态方法CreateObject)
typedef CCtrlWindowX * (*LPCREATEOBJECT)();

// 基础控件，提供控件的公共属性的操作。目前此类中包含一些用不到的属性，是为了简化以后控件的开发。
class CControl : public CCtrlBaseTag
{
	DTI_DECLARE(CControl, CCtrlBaseTag)
protected:
	string m_sName;              // 控件名称
	int  m_iControlMode;					// 控件类型
	int  m_iX,m_iY;						// 控件位置，为相对于父控件的相对坐标
	int  m_iScreenX,m_iScreenY;			// 控件左上角绝对坐标
	int  m_iWidth,m_iHeight;				// 控件尺寸
	int  m_iMX,m_iMY;                    // 当前鼠标相对位置
	bool m_bFocus;						// 控件是否有输入焦点
	bool m_bNoFocus;					// 控件是否应该获得输入焦点，比如静态文字控件就不应该有输入焦点。
	bool m_bModel;						// 控件是否是模式化的，即在控件关闭前，不能丧失焦点
	bool m_bNoMove;						// 窗口是否可以移动
	bool m_bNoChangeLevel;				// 不改变层次关系
	UINT m_uStyle;						// 控件的风格
	int  m_iTrees;						// 控件树结构层次，从0递增，0层总是指主窗口控件
	bool m_bClick;						// 控件是否被鼠标左键点住没有释放
	bool m_bRBClick;	
	bool m_bShow;						// 是否显示这个控件
	bool m_bEnable;						// 是否激活这个控件
	bool m_bMouseOn;					// 鼠标在此控件上
	DWORD m_dwMouseOnFrameCount;		// 鼠标开始在控件上
	string m_sTips;						// tip

	int m_iFrame;						// 控件边框宽度
	int m_dwFrameColor;					// 控件边框颜色
	int m_iMarge;						// 边距

	string m_sText;						// 文字，此文字的内存空间会自动创建，在控件被摧毁时，会自动释放此空间。
	int m_iFont;						// 字体
	int m_iFontSize;					// 字体号码
	DWORD m_dwFontFlag;                 // 绘制字体的属性,eDrawTextFlag类型
	int m_iTextOffX,m_iTextOffY;        // m_sText文字绘制的偏移

	DWORD m_dwColor;					// 主体颜色
	DWORD m_dwTexID; 					// 图片

	static DWORD m_dwTrans;				// 透明度
	DWORD m_dwBackColor;				// 控件背景颜色
	DWORD m_dwBackTexID;				// 控件背景纹理

	CControl * m_pParent;				// 父控件
	CControl * m_pControlNext;			// 下一个同等级的控件
	CControl * m_pControlPrev;			// 上一个同等级的控件

	CCtrlMainWin*		m_pMainWnd;		// 根控件
	CInput*				m_pInput;		// 同上

	UCHAR * m_cMask;
	int m_iMaskWidth,m_iMaskHeight;
	int m_iTipTime;
	unsigned char	m_cColorIndex;

	BYTE	m_byFadeStep;
	BYTE	m_byFadeTarget;
	bool	m_bEnableBeforeFade;
	bool	m_bNeedDisableWhileFading;

	int			m_iTexOffX;         //背景纹理绘制时的偏移  
	int			m_iTexOffY;         
	int			m_iOffX;            //在父窗口中的x坐标
	int			m_iOffY;            //在父窗口中的y坐标
	int         m_iAlignType;       //在父窗口中的排列方式,XALINEMENT之一

	int		    m_iOriginalWidth;	//新界面缩放用
	int		    m_iOriginalHeight;	//新界面缩放用
	int		    m_iOriginalOffX;	//新界面缩放用
	int		    m_iOriginalOffY;	//新界面缩放用
	bool	    m_bScale;//是否要缩放,是指这个控件内容绘制的时候是否要缩放,并控制子控件相应属性
	bool        m_bScaleWidthAndHeight;//如果m_bScale属性为true,是否要缩放宽度和高度,一级窗口一般不用,因为传进来的大小一般都会缩放之前的大小
	bool	    m_bInheritScaleFromParent;//是否要继承父控件的缩放属性

	BOOL		m_bNeedTestAlpha;
	int         m_iMask;  

	DWORD       m_dwStartTime;//控件的开始生效或创建时间

	CXmlControl* m_pXmlControl;        //Xml处理脚本信息,以备程序修改

public:
	virtual void SetAlignType(int i){m_iAlignType = i;}
	virtual int GetAlignType(){return m_iAlignType;}
	virtual void SetOffX(int i){m_iOffX = i;}
	virtual int GetOffX(){return m_iOffX;}
	virtual void SetOffY(int i){m_iOffY = i;}
	virtual int GetOffY(){return m_iOffY;}
	virtual void SetOriginalHeight(int i){m_iOriginalHeight = i;}
	virtual int GetOriginalHeight(){return m_iOriginalHeight;}
	virtual void SetOriginalWidth(int i){m_iOriginalWidth = i;}
	virtual int GetOriginalWidth(){return m_iOriginalWidth;}

	virtual void SetOriginalOffX(int i){m_iOriginalOffX = i;}
	virtual int GetOriginalOffX(){return m_iOriginalOffX;}
	virtual void SetOriginalOffY(int i){m_iOriginalOffY = i;}
	virtual int GetOriginalOffXY(){return m_iOriginalOffY;}

	virtual void OnSwitchToTop();

protected:
	virtual CControl * IsSonsMsg(DWORD dwMsg,DWORD dwData,CControl * pControl);		// 判断一个消息是否是此控件的子控件的

	bool ExecuteScript(int e);       //运行事件脚本
    void GetScriptFileContent();
public:
	CControl(void);
	virtual ~CControl(void);

	static void SetTrans(DWORD dwTrans)		{	m_dwTrans = dwTrans;	}

	// 创建窗口，参数为：父控件、z坐标、左上角x坐标、y坐标、宽度、高度、风格、背景颜色、背景纹理
	virtual bool Create(CControl * pParent,int iX,int iY,int iW,int iH,UINT uStyle=CTRL_STYLE_BACKMODE_NODRAW,DWORD dwBackcolor=0xFFFFFFFF,DWORD dwBacktexID = 0);
	virtual void Destroy();

	// 从Xml定义创建对象
	virtual bool CreateXML(CControl* pParent,CXmlControl* ctrl);

	// 名字
	const char* GetName(){ return m_sName.c_str(); }
	void  SetName(const char* name){ m_sName.assign(name); }

	// 属性相关函数
	// 位置和尺寸
	int GetX()		  { return m_iX; }
	int GetY()		  { return m_iY; }
	virtual int GetWidth()	  { return m_iWidth; }
	virtual int GetHeight()	  { return m_iHeight; }
	virtual void SetWidth(int w)	  { m_iWidth = w; }
	virtual void SetHeight(int h)	  { m_iHeight = h; }
	int GetScreenX()  { return m_iScreenX; }
	int GetScreenY()  { return m_iScreenY; }
	int GetMX()		  { return m_iMX; }
	int GetMY()		  { return m_iMY; }

	// 控件管理
	void SetControlNext(CControl * pCtrl) { m_pControlNext = pCtrl; }
	void SetControlPrev(CControl * pCtrl) { m_pControlPrev = pCtrl; }
	CControl * GetControlNext() { return m_pControlNext; }
	CControl * GetControlPrev() { return m_pControlPrev; }
	CControl * GetParent() { return m_pParent; }

	// 焦点，判断此控件是否能获取焦点
	inline bool CanFocus() { return !m_bNoFocus; }
	inline void SetNoFocus(bool b) { m_bNoFocus = b;}
	// 风格，风格何以用与操作来结合： '|'
	UINT GetStyle() { return m_uStyle; }
	void SetStyle(UINT uStyle) { m_uStyle = uStyle; }
	// 背景
	//virtual void SetBack(DWORD dwBackcolor,LPTexture pBacktex = NULL, bool bAmb = false);
	virtual void SetBackTexByID(DWORD dwBackcolor,DWORD dwBackTexID = 0, bool bAmb = false);
	virtual DWORD	GetBackTexID() { return m_dwBackTexID;}
	// 边框
	virtual void SetFrame(int iFrame,DWORD dwFrameColor, bool bAmb = false);
	virtual int GetFrame() { return m_iFrame; }
	// 边距
	void SetMarge(int iMarge);
	int  GetMarge() { return m_iMarge; }
	// 文字,字体
	virtual void SetText(const char * sText);
	virtual const char * GetText() { return m_sText.c_str(); }
	virtual void SetTextOff(int iOffX,int iOffY);
	virtual void GetTextOff(int &iOffX,int &iOffY);
	virtual int GetTextOffX();
	virtual int GetTextOffY();
	virtual void SetFont(int iFont = -1,int iFontSize = -1,DWORD dwFontFlag = 0);
	void GetFont(int * pFont,int * pFontSize,DWORD *pFlag);
	int GetFont();
	int GetFontSize();
	DWORD GetFontFlag();
	// 颜色
	void SetColor(DWORD dwColor) { m_dwColor = dwColor; }
	DWORD GetColor() { return m_dwColor; }
	// 纹理，注意：更换纹理后，系统不会删除旧的纹理
	//void SetTex(LPTexture pTex);
	//LPTexture GetTex() { return m_pTex; }

	// 是否显示
	void Show() { m_bShow = true; }
	void Hide() { m_bShow = false; }
	void SetShow(bool bShow) { m_bShow = bShow; }
	bool IsShow() { return m_bShow; }

	// 是否激活
	void Enable(){ m_bEnable = true; }
	void Disable() { m_bEnable = false; }
	virtual void SetEnable(bool bEnable);
	bool IsEnable() { return m_bEnable; }
	//
	void SetMode(bool bMode) { m_bModel = bMode; }
	bool GetMode()	{ return m_bModel; }
	// 判断是否该释放按下的键
	virtual bool IsMouseOn(int iX,int iY);
	virtual bool IsClick() { return m_bClick; }	
	virtual void SetClick(bool b) { m_bClick = b; }	
	virtual bool IsRBClick() { return m_bRBClick; }	
	virtual void SetRBClick(bool b) { m_bRBClick = b; }	
	virtual bool IsParent(CControl * pCtrl);	// 判断this控件是否是pCtrl控件的父控件
	//
	bool IsNoChangeLevel() { return m_bNoChangeLevel; }
	int  GetControlMode() { return m_iControlMode; }

	// Tips
	void SetTips(const char * str);
	const char * GetTips();
	const char * IsShowTips(DWORD dwFrameCount);
	void SetTipTime(int i = SHOWTIPS_DELAY) { m_iTipTime = i; }
	int  GetTipTime() { return m_iTipTime; }

	// 设置焦点
	bool SetMask(WORD wMask,int w = 0,int h = 0);
	bool SetMaskBuf(UCHAR * pMask,int w = -1,int h = -1);
	UCHAR *  GetMaskBuf(){return m_cMask;}

	// 绘图和输出文字
	void DrawTexture(int iX,int iY, int iIdx,DWORD dwColor = 0xFFFFFFFF);
	void TextOut(int iX,int iY,const char *str,DWORD dwColor = 0xFFFFFFFF,DWORD dwFlag = 0,DWORD dwBackColor = 0);

	// 移动以及尺寸变更
	bool AssignXML(CXmlControl* ctrl);
	virtual bool RelativeMove(int iType = XAL_NONE);
	virtual bool Move(int iX,int iY);
	virtual bool ReSize();
	virtual bool ReSize(int iW,int iH);
	virtual void OnMove();
	virtual void OnReSize();

	// 创建摧毁窗口时调用的函数
	virtual void OnCreate();
	virtual void OnClose();

	// 焦点
	virtual bool SetFocus();							// 设置本控件获得焦点，返回如果为false表明获取焦点失败
	virtual void SetHaveFocus(bool b);                  // 设置m_bFocus的值
	virtual bool KillFocus(CControl * pNewFocus);		// 设置本控件将丢失焦点，参数为即将拥有焦点的控件
	virtual void OnSetFocus();							// 系统将获得焦点时此函数被调用
	virtual void OnKillFocus();							// 系统将失去焦点时此函数被调用

	// 消息响应函数
	virtual bool OnKeyDown(WORD wState,UCHAR cKey);		// KEYDOWN 
	virtual bool OnKeyUp(WORD wState,UCHAR cKey);		// KEYUP
	virtual bool OnChar(UCHAR cChar);					// 有字符输入
	virtual bool OnMouseMove(int iX,int iY);			// 鼠标移动消息
	virtual bool OnWheel(int iWheel);					// 滚轮消息
	virtual bool OnCaptureChanged();					
	virtual bool OnLeftButtonDown(int iX,int iY);
	virtual bool OnLeftButtonUp(int iX,int iY);
	virtual bool OnLeftButtonDClick(int iX,int iY);
	virtual bool OnRightButtonDown(int iX,int iY);
	virtual bool OnRightButtonUp(int iX,int iY);
	virtual bool OnRightButtonDClick(int iX,int iY);
	virtual bool OnMiddleButtonDown(int iX,int iY);
	virtual bool OnMiddleButtonUp(int iX,int iY);
	virtual bool OnClick(int iButton);					// 鼠标的一个按键按在本控件上市，此函数被调用
	virtual void OnAutoSize();
	virtual bool IsInMask(int iX,int iY);
	virtual bool IsInControl(int iX,int iY); //是否在控件内

	// 消息接受函数，接受子控件和输入模块发来的消息。参数为：消息，消息附带数据，发出消息的控件（NULL一般表示此消息是输入模块发过来的）
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);

	// 绘图函数，子控件可以重载此函数，以实现自绘
	virtual void OnDraw();
	virtual void Draw();
	virtual void AfterDraw();
	virtual void SetColorIndex(unsigned char c) { m_cColorIndex = c;}
	virtual DWORD GetColorByIndex();

	//淡入效果
	inline BYTE IsFadeIn() { return m_byFadeStep;}
	void SetFadeIn(BYTE byStep);

	// 其他
	inline CCtrlMainWin* GetMainWnd() const { return m_pMainWnd;}
	inline CInput* GetInput() const { return m_pInput;}
	//void SetScale(float f);

	void SetTestAlpha(BOOL b) { m_bNeedTestAlpha = b;}
	BOOL DoesTestAlpha() { return m_bNeedTestAlpha;}

	BOOL IsNeedKeyInput();

	virtual void SetStartTime(DWORD dwTime);//控件的开始生效或创建时间
	virtual DWORD GetStartTime();

	virtual bool IsScale(){return m_bScale;}
	virtual void SetScale(bool b){m_bScale = b;}
	virtual bool IsScaleWidthAndHeight(){ return m_bScaleWidthAndHeight; }
	virtual void SetScaleWidthAndHeight(bool val) { m_bScaleWidthAndHeight = val; }
	virtual bool IsInheritScaleFromParent() { return m_bInheritScaleFromParent; }
	virtual void SetInheritScaleFromParent(bool val) { m_bInheritScaleFromParent = val; }

	virtual void ResetControlPos();
	virtual void ResetWidthAndHeight(int iW,int iH);

	//消息
public:
	struct MSG_STRUCT
	{
		DWORD dwMsg;
		DWORD dwData;
		CControl*	pControl;

		MSG_STRUCT() { dwMsg = NULL; dwData = NULL; pControl = NULL;}
		void Assign(DWORD _dwMsg, DWORD _dwData, CControl* _pControl)
		{
			dwMsg = _dwMsg; dwData = _dwData; pControl = _pControl;
		}
	};

	//辅助
	void DrawTexture(int iX,int iY, LPTexture pTex, DWORD dwColor = 0xFFFFFFFF,LPRECT pRect = NULL);
	void DrawTextureZoom(int iX, int iY, LPTexture pTex, float fX, float fY, DWORD dwColor = 0xFFFFFFFF, RECT* rc = NULL);
};

typedef vector<CControl*>  VLPControl;

// 容器类，可以包含其它控件
class CControlContainer : public CControl
{
	DTI_DECLARE(CControlContainer, CControl)
protected:
	CControl * m_pControls;		// 第一个子控件
	CControl * m_pDrawControl;//正在绘制的控件,如果在绘制的时候删除这个控件应该对对这个变量重新赋值,
	CControl* m_pLastDrawControls;	//第一个最后画的子控件

protected:
	virtual CControl * IsSonsMsg(DWORD dwMsg,DWORD dwData,CControl* pControl);	// 判断一个消息是否是此控件的子控件的

public:
	CControlContainer();
	~CControlContainer();

	CControl * GetControls() { return m_pControls; }				// 得到子控件链表的头
	void	SetControls(CControl * pCtrl) { m_pControls = pCtrl; }		// 设置子控件链表的头
	BOOL	IsSonCtrl(CControl *pControl);							// 是否是本控件的子控件

	virtual void OnMove();										// 处理窗口的拖动

	virtual bool AddControl(CControl * pCtrl,int iPos = -1,bool bAddToLastDraw = false);	// 添加一个控件
	virtual bool RemoveControl(CControl** pCtrl,bool bRemoveFromLastDraw = false);				// 删除一个控件
	virtual bool RemoveControl(int iPos);						// 删除一个控件
	CControl*    FindControl(int iPos);                         // 查找指定位置的控件
	CControl*    FindControlByName(const char* name,eCtrlMode filter = CTRL_MODE_ALL);// 根据名字查找到控件
	virtual void ResetControlPos();
	virtual bool SwitchToTop(CControl * pCtrl,bool bNeedCallOnSwitchToTop = true);

	virtual void Draw();
	virtual void AfterDraw();		////  要画最后画的子控件

protected:
	POINT	m_ptFroneTexPos;
public:
	inline void SetFrontTexPos(int iX, int iY) { m_ptFroneTexPos.x = iX; m_ptFroneTexPos.y = iY;}
	inline int GetFrontTexX()	{ return m_ptFroneTexPos.x;}
	inline int GetFrontTexY()	{ return m_ptFroneTexPos.y;}
};

class CCtrlButton;

typedef enum eWIN_OPER
{
	OPER_CLOSE	= 0x10000,    //关闭窗口
	OPER_CREATE = 0x20000,            //创建新窗口
	OPER_UPDATE = 0x30000,            //如果窗口开着则关了，如果关了则打开
	OPER_RECREATE = 0x40000,          //重新创建，也就先执行OPER_CLOSE，再执行OPER_CREATE
	OPER_CUSTOM = 0x50000,
} WIN_OPER;


// 窗口类，窗口可以拖动，且按照点击顺序显示
class CCtrlWindow : public CControlContainer
{
	DTI_DECLARE(CCtrlWindow, CControlContainer)
public:
	CCtrlWindow();
	~CCtrlWindow();

	virtual void SetCloseButton(int iX,int iY,int iTexIndex = 0);
	virtual void OnClickCloseButton();
	virtual bool OnMouseMove(int iX,int iY);	// 接受鼠标移动消息，以处理拖动
	virtual void Draw();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	virtual bool OnKeyDown(WORD wState, UCHAR cKey);
	virtual void OnEscape(void);
	virtual bool AddArrowTip(int iTipID,int iX,int iY,int iAlignType = XAL_TOPLEFT,bool bShowNow = false,int iTipDir = XAL_TOPRIGHT,int iTipType = 0,DWORD dwData = 0,DWORD dwLastTime = 0,bool bNeedMsg = true);//ShowNow是否立即显示,否则自动计算等该窗口所有的arrowtip显示结束了再显示,后面三个参数见CArrowTip::Create


	//窗口可以有生命周期的，自动关闭
	void SetLifeTime(DWORD dwTime)	{ m_dwLifeTime = dwTime; }
	DWORD GetLifeTime() { return m_dwLifeTime; }
	bool IsDisableEscape(){return m_bDisableEscape;}

protected:
	int m_iOldx,m_iOldy;						// 前一次检查时得到的鼠标位置
	int m_iClickX,m_iClickY;					// 点击时的坐标
	CCtrlButton* m_pCloseButton;                // 关闭按钮
	DWORD        m_dwLifeTime;
	bool m_bDisableEscape;                      // 按Escape时不关闭窗口
	static DWORD   sm_dwWindowType;             // 创建参数传进来的窗口类型,有时传递一个DWORD类型的消息、物品或人物ID等
};

// 主控件窗口类，游戏应该产生一个此类的派生类
class CParserTip;
class CCtrlTip;
class CCtrlMenuWnd;

#include <deque>

//记录窗口的位置
struct stSPos
{
	stSPos()
	{
		iLastX = iLastY = POS_AUTO;
		iLastPage = 0;
	}
	int    iLastX;  //上次位置
	int    iLastY;
	int    iLastPage; //上次绘制
};

typedef std::map<string, CCtrlWindow*> MWindow;

class CCtrlMainWin : public CCtrlWindow
{
	DTI_DECLARE(CCtrlMainWin, CCtrlWindow)
public:
	struct MSG_EX_STRUCT
	{
		CControl::MSG_STRUCT	_msgStruct;
		CControl*				_msgReciever;
	};
protected:
	CControl * m_pFocusNow;		// 当前拥有焦点的控件
	int     m_iMouseX,m_iMouseY;
	UINT    m_uMouseOnID;
	int     m_iMouseOnType;
	int     m_iHouseOnHeight;
	CControl * m_uMouseOnCtrl;
	CControl * m_pTipOwnerWnd;
	CParserTip * m_pParserTip;
	CCtrlTip * m_pCtrlTip;
	CCtrlMenuWnd * m_pMenuWnd;
	BOOL    m_bHideCursor;
	DWORD	m_dwWindowMode;

	typedef std::deque<MSG_EX_STRUCT>	MSG_DEQUE;
	typedef std::map<string,LPCREATEOBJECT> AutoWndMap;
	typedef std::map<DWORD,string> MsgWndMap; //
	typedef std::map<string,stSPos> SPosMap;

	MSG_DEQUE	m_vFrameMsg;
	AutoWndMap  m_MAutoWnd; //自定义窗口列表，脚本可弹出,string为窗口名字，WND_ATTR为相关属性
	MsgWndMap   m_MMsgWnd;  //消息和名字的对应关系表
	AutoWndMap   m_MWnd_MicroControl;  //微操模式下允许打开的窗口列表
	AutoWndMap   m_MWnd_CaptionSubstitute;  //附身模式下允许打开的窗口列表,包括微操模式所允许的
	MWindow     m_WndMap;   //所有开着的窗体
	SPosMap     m_SPosMap;   //窗口位置信息
public:
	static void UpdateLayOut();
	static void DisableIME();

	CCtrlMainWin();
	~CCtrlMainWin();
	virtual bool Create(int iW,int iH);

	inline int GetMouseX() { return m_iMouseX; }
	inline int GetMouseY() { return m_iMouseY; }
	inline void GetMouseXY(int &iX,int &iY) { iX = m_iMouseX; iY = m_iMouseY; }
	void SetMouseOnID(UINT uID,CControl * pCtrl = NULL);
	inline UINT GetMouseOnID() { return m_uMouseOnID; }
	inline int  GetMouseOnType(){ return m_iMouseOnType; }
	inline void SetMouseOnType(int iType){ m_iMouseOnType = iType; }
	inline int  GetMouseOnHeight(){ return m_iHouseOnHeight; }
	inline void SetMouseOnHeight(int iHeight){ m_iHouseOnHeight = iHeight; }
	inline CControl*	GetMouseOnCtrl() { return m_uMouseOnCtrl;}
	inline void SetTipOwnerWnd(CControl *p) { m_pTipOwnerWnd = p; }
	inline CControl * GetTipOwnerWnd() { return m_pTipOwnerWnd; }
	inline CParserTip*  GetTipWnd(){ return m_pParserTip; }
	inline CCtrlMenuWnd*  GetMenuWnd(){ return m_pMenuWnd; }
	void SetHideCursor(BOOL b)	{ m_bHideCursor = b;}
	BOOL IsHideCursor()	{ return m_bHideCursor;}
	inline DWORD GetGameState() const { return m_dwWindowMode;}	//游戏状态

	inline CControl * GetFocusCtrl() { return m_pFocusNow; }	// 获得当前拥有焦点的控件
	void SetFocusCtrl(CControl * pCtrl);				// 设置当前拥有焦点的控件
	bool SetTopWindowFocus();//把焦点给最上层窗口
	bool CloseTopWindow();//关闭最上层窗口

	void ADDCONTROL(CControl* pCtrl);
	void DELETECONTROL(CControl** pCtrl);

	virtual bool OnMouseMove(int iX,int iY);			// 接受鼠标移动，处理拖动
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);	// 消息函数
	virtual bool AutoCloseWndProc(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);	//自动关闭特殊窗口
	virtual void ClearChildControl(void);
	virtual void Draw();
	virtual bool IsPanelFakeFocus(){return false;}
	
	//PopupWindow:弹出窗口,szName为窗口名字，iPos表示多XML窗口使用第几个xml文件
	//函数先看有没对应的窗口类，有的话通过这个类的createobject来创建对象，
	//没有话再查询有没有对应的xmlwindow，有话通过xmlwindow来创建
	bool PopupWindow(const char* szName,DWORD dwData = OPER_CREATE,DWORD dwWindowType = 0);
	bool PopupWindow(DWORD dwMsg,DWORD dwData = OPER_CREATE,DWORD dwWindowType = 0);
	//把指定消息发到指定窗口，dwWndMsg目标窗口对应的dwMsg编号(MSG_GAMEWNDS_MESSAGE_BEGIN之后的消息),szName窗口名称,dwMsg,dwData,pControl为发到窗口去的消息
	virtual bool MsgToWnd(DWORD dwWndMsg,DWORD dwMsg,DWORD dwData = 0,CControl * pControl = NULL);
	virtual bool MsgToWnd(const char *szName,DWORD dwMsg,DWORD dwData = 0,CControl * pControl = NULL);

	CCtrlWindow* FindWindowByName(const char* szName);

	bool GetSPos(const char* szName,stSPos& Pos);//获得窗口下次开起时的位置
	void SetSPos(const char* szName,const stSPos& Pos);//设置窗口下次开启时的位置
    bool DelSPos(const char* szName);//删除窗口szName记录的位置
	//获得窗口下次开启时的位置，如果窗口开着则返回
	bool GetWindowPos(int &iX,int &iY,const char* szName);
	//设置窗口位置获得窗口位置,窗口开着，移动到对应位置，否则调用SetSPos设置该窗口下次启动的位置
	void SetWindowPos(int iX,int iY,const char* szName);
	virtual void ReSetAllWndPos();

	virtual bool PopupArrowTip(DWORD dwWndMsg,int iTipID,int iX,int iY,int iAlignType = XAL_TOPLEFT,bool bShowNow = false,int iTipDir = XAL_TOPRIGHT,int iTipType = 0,DWORD dwData = 0,DWORD dwLastTime = 0,bool bNeedMsg = true);//ShowNow是否立即显示,否则自动计算等该窗口所有的arrowtip显示结束了再显示
	virtual bool PopupArrowTip(const char* szWndName,int iTipID,int iX,int iY,int iAlignType = XAL_TOPLEFT,bool bShowNow = false,int iTipDir = XAL_TOPRIGHT,int iTipType = 0,DWORD dwData = 0,DWORD dwLastTime = 0,bool bNeedMsg = true);//ShowNow是否立即显示,否则自动计算等该窗口所有的arrowtip显示结束了再显示

	//异步处理消息
	void PushMsg(DWORD dwMsg, DWORD dwData, CControl* pControl = NULL, CControl* pReciever = NULL);	//不能在会使pReciever实效的函数中调用，如删除
	void RemoveMsg(DWORD dwMsg, DWORD dwData, CControl* pControl = NULL, CControl* pReciever = NULL);

	void ExecuteMsg();
	void ClearMsg();
	int  FindMsg(DWORD dwMsg, DWORD dwData, CControl* pControl = NULL);

	virtual bool ChangeUi(E_UITYPE eUi = EUT_NONE,bool bCheckWindowWidth = false);//改变Ui,传EUT_NONE表示界面轮流切换,否则切换成指定的,bCheckWindowWidth:是否检查分辨率

	MWindow &GetOpenWndMap(){return m_WndMap;}
};

extern CControl* g_pThis; 
