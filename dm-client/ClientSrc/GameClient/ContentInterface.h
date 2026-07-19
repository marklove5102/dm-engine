#pragma once
#pragma pack(push,1)
#include <unknwn.h>

// LockFlag参数
#define LOCKFLAG_GETBUFFER  0
#define LOCKFLAG_GETDC      1

// QueryHitPoint 返回值
#define HTP_NONE            0    // 无
#define HTP_CLIENT          1    // 客户区
#define HTP_TRANSPARENT     2    // 透明
#define HTP_OUTSIDE         3    // 客户区以外
#define HTP_CAPTION         4    // 可拖拽区

// CreateWidgetContent Flag参数
#define CWCF_EXTENDCONTROL  0x0001    // 使用扩展控件
#define CWCF_THREADCONTROL  0x0002    // 控件内部对象创建到线程中
// 以下为WidgetContent使用的图形模式，影响到CWidgetContent.Lock方法的Flag参数所支持的类型，
// 默认为GDI模式，不能同时指定两种或两种以上图形模式，否则将创建失败
#define CWCF_DIRECTDRAW     0x0100    
#define CWCF_D3D8           0x0200
#define CWCF_D3D9           0x0400
#define CWCF_D3D10          0x0800
#define CWCF_OPENGL         0x1000

typedef struct Rects {
	RECT    _Rect;
	Rects * _Next;
} *PRects;

class CWidgetContent
{
public:
	virtual void   __stdcall Release() = 0;
	virtual void   __stdcall SetBounds(int Left, int Top, int Width, int Height) = 0;
	virtual void   __stdcall SetFocus(BOOL Focus) = 0;
	virtual void   __stdcall SetVisible(BOOL Visible) = 0;
	virtual BOOL   __stdcall OnWindowMessage(UINT Msg, WPARAM wParam, LPARAM lParam, LRESULT& lResult) = 0;
	virtual int    __stdcall QueryHitPoint(const POINT * ClientPoint) = 0;
	virtual BOOL   __stdcall Lock(int Flag, int& Context, int& Width, int& Height, int& Pitch, PRects& Rects) = 0;
	virtual BOOL   __stdcall Unlock() = 0;
	virtual void   __stdcall Paint() = 0;
	virtual void   __stdcall Refresh() = 0;
	virtual BOOL   __stdcall Get_Changed() = 0;
	virtual void   __stdcall Set_Src(const BSTR Value) = 0;
	virtual void   __stdcall Get_Src(BSTR& Value) = 0;
	virtual void   __stdcall Get_DefaultInterface(IUnknown*& pUnk) = 0;
	virtual void   __stdcall DoCommand(const VARIANT * CmdID, const VARIANT * CmdParam, VARIANT& vResult) = 0;
};

class CHostWindowProxy
{
public:
	virtual HWND   __stdcall Get_WindowHandle() = 0;
	virtual void   __stdcall OnPositionChange(const RECT * rcBounds) = 0;
	virtual void   __stdcall OnFocusChange(BOOL Focus) = 0;
	virtual void   __stdcall OnVisibleChange(BOOL Visible) = 0;
	virtual BOOL   __stdcall OnTranslateAccelerator(tagMSG * Msg) = 0;
	virtual BOOL   __stdcall OnTranslateIME(tagMSG * Msg) = 0;
	virtual void   __stdcall OnExecute(const VARIANT * CmdID, const VARIANT * CmdParam, VARIANT& vResult) = 0;
	virtual void   __stdcall OnCloseQuery(BOOL& CanClose) = 0;
	virtual void   __stdcall OnCursorChange(HCURSOR Cursor) = 0;
	virtual void   __stdcall OnFullScreen(BOOL FullScreen) = 0;
};

// IE

// SetUserAgent Flags
#define USERAGENT_DEFAULT   0  // 使用默认的UserAgent（忽略bstrUserAgent参数）
#define USERAGENT_SUBJOIN   1  // 添加到原有UserAgent末尾，例如：Mozilla/4.0 (compatible; MSIE 6.0; ...; IGW 1.0)
#define USERAGENT_REPLACE   2  // 使用bstrUserAgent替换UserAgent

class CHtmlContent: public CWidgetContent
{
public:
	virtual void  __stdcall GoBack() = 0;
	virtual void  __stdcall GoForward() = 0;
	virtual void  __stdcall Navigate(const BSTR URL, DWORD Flags) = 0;
	virtual void  __stdcall NavigateEx(const BSTR URL, DWORD Flags, const BSTR TargetFrameName, const VARIANT * PostData, const BSTR Headers) = 0;
	virtual HRESULT  __stdcall GetElementValue(const BSTR ID, VARIANT& RetVal) = 0;
	virtual HRESULT  __stdcall GetElementAttribute(const BSTR ID, const BSTR Attribute, VARIANT& RetVal) = 0;
	virtual HRESULT  __stdcall execScript(const BSTR code, const BSTR language) = 0;
	virtual void  __stdcall Get_BrowserApplication(IDispatch*& BrowserApp) = 0;
	virtual void  __stdcall SetUserAgent(const BSTR bstrUserAgent, DWORD dwFlags) = 0;
};

class CHtmlWindowProxy: public CHostWindowProxy
{
public:
	virtual void  __stdcall OnBeforeNavigate(const BSTR URL, DWORD Flags, const BSTR TargetFrameName, const VARIANT * PostData, const BSTR Headers, BOOL& Cancel) = 0;
	virtual void  __stdcall OnNavigateError(const BSTR URL, const BSTR Frame,  DWORD StatusCode, BOOL& Cancel) = 0;
	virtual void  __stdcall OnNavigateComplete(const BSTR URL) = 0;
	virtual void  __stdcall OnDownloadBegin() = 0;
	virtual void  __stdcall OnDownloadComplete() = 0;
	virtual void  __stdcall OnDocumentComplete(const BSTR URL) = 0;
	virtual void  __stdcall OnNewWindow(IDispatch*& ppDisp, BOOL& Cancel, DWORD dwFlags, const BSTR bstrUrlContext, const BSTR bstrUrl) = 0;
	virtual void  __stdcall OnProgressChange(int Progress, int ProgressMax) = 0;
	virtual void  __stdcall OnStatusTextChange(const BSTR URL) = 0;
	virtual void  __stdcall OnTitleChange(const BSTR URL) = 0;
	virtual HRESULT  __stdcall OnShowMessage(const BSTR bstrCaption, const BSTR bstrText, DWORD dwType, LRESULT& lResult) = 0;
};

// Flash


// 
typedef DECLSPEC_IMPORT CWidgetContent * (WINAPI* LP_CreateWidgetContent)(CHostWindowProxy *, int);

CWidgetContent * __stdcall CreateWidgetContent(CHostWindowProxy * HostWindow, int Flag);

#pragma pack(pop)
