#pragma once


#ifdef WEBCTRL_EXPORTS
#define WEBCTRL_API extern "C" __declspec(dllexport)
#else
#define WEBCTRL_API __declspec(dllimport)
#endif

#define IWEBCTRL_INTERFACE_VERSION "IWEBCTRL_INTERFACE2"

//回调参数；
struct KeyValueEvent_t
{
	const char *pTitle;
	const char *pKey;
};

//应用程序应当根据需要扩充该回调
class IWebCallBack
{
public:
	//连接前通知
	virtual void OnBeforeNavigate2(const char *lpszURL) = 0;

	//连接完成报告
	virtual void OnNavigateComplete2(const char *lpszURL, unsigned long nStatusCode) = 0;

	//文档完成时通知
	virtual void OnDocComplete(char *pTitleStr) = 0;
	//返回GUID时通知
	virtual void OnGUID(char *pGuidStr) = 0;	//Fake!
	//交易完成时返回
	virtual void OnTradeOk(char *pTicket) = 0;	//Fake!

	//响应事件
	virtual void OnKeyValue(const char *pKey,const char *pValue)= 0;

};

enum E_PROPERTY
{
	EP_BackGround = 0,	//是否后台执行
	EP_Scroll,			//文档滚动条
	EP_AutoReLink,		//自动重连次数
	EP_ErrorHideWnd,	//是否隐藏错误窗口

	EP_MAX
};
//
//执行常用的Web 操作，包括Html解析和回调控制
//

class IWebCtrl 
{
public:
	//执行初始化
	virtual int Init() = 0;		
	//执行结束
	virtual int ShutDown() = 0;	
	//创建
	virtual BOOL CreateEx(DWORD dwStyle,RECT& rect,HWND hParentWnd,UINT nID) = 0;
	//显示和隐藏窗口
	virtual void ShowWindow(int nCmdShow) = 0;
	//窗口是否显示
	virtual int IsWindowVisable() = 0;
	//移动窗口位置
	virtual int MoveWindow(RECT& rect,bool bAdjParent = 1) = 0;
	//设置回调控制接口
	virtual void SetWebCallBack(IWebCallBack *p) = 0;
	//浏览画面
	virtual void Navigate(LPCTSTR URL, DWORD Flags = 0) = 0;
	//刷新WEb组件
	virtual void Refresh() = 0;
	//用于文档完成时的回调
	virtual int PushEventKeyValue(const char *pTitle,const char *pKey) = 0;
	//支持 post 方式
	virtual HRESULT NavigateEx(const char *url, DWORD Flags ,const char *pPostData,const char *pHeaders) = 0;
	//IE属性
	virtual int SetProperty(int nProperty,DWORD value) = 0;
	virtual int GetProperty(int nProperty,DWORD &value) = 0;
	virtual int Reset() = 0;
};


typedef IWebCtrl* LPIWebCtrl;

typedef void* (*fnCreateInterface)(const char *pName, int *pReturnCode );

//IWebCtrl *CreateWebCtrl();
//WEBCTRL_API IWebCtrl *CreateWebCtrl();
WEBCTRL_API void* CreateInterface( const char *pName, int *pReturnCode );

//extern WEBCTRL_API int nWebCtrl;
//WEBCTRL_API int fnWebCtrl(void);


