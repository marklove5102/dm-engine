#pragma once

#include "Global/Global.h"

//提供给Host Window调用的接口类，
//相当于处理回调函数

#include "ContentInterface.h"

BSTR string2BSTR(std::string &s);
BSTR str2BSTR(const char *s);
bool BSTR2string(BSTR bstr,std::string &s);
bool VariantBSTR2string(VARIANT a,std::string &s);
bool Variant2Str(VARIANT &arg,std::string &s);

#define SAFE_DEL_BSTR(a) if(a){::SysFreeString(a);a = 0;};

class CWidgetProxy : public CHtmlWindowProxy
{
	friend class CWidgetManager;
public:
	CWidgetProxy(void);
	~CWidgetProxy(void);

	HWND   __stdcall Get_WindowHandle();
	void   __stdcall OnPositionChange(const RECT * rcBounds);
	void   __stdcall OnFocusChange(BOOL Focus);
	void   __stdcall OnVisibleChange(BOOL Visible);
	BOOL   __stdcall OnTranslateAccelerator(tagMSG * Msg);
	BOOL   __stdcall OnTranslateIME(tagMSG * Msg);
	void   __stdcall OnExecute(const VARIANT * CmdID, const VARIANT * CmdParam, VARIANT& vResult);
	void   __stdcall OnCloseQuery(BOOL& CanClose);
	void   __stdcall OnCursorChange(HCURSOR Cursor);
	void   __stdcall OnFullScreen(BOOL FullScreen);

	void   __stdcall OnBeforeNavigate(const BSTR URL, DWORD Flags, const BSTR TargetFrameName, const VARIANT * PostData, const BSTR Headers, BOOL& Cancel);
	void   __stdcall OnNavigateError(const BSTR URL, const BSTR Frame,  DWORD StatusCode, BOOL& Cancel);
	void   __stdcall OnNavigateComplete(const BSTR URL);
	void   __stdcall OnDownloadBegin();
	void   __stdcall OnDownloadComplete();
	void   __stdcall OnDocumentComplete(const BSTR URL);
	void   __stdcall OnNewWindow(IDispatch*& ppDisp, BOOL& Cancel, DWORD dwFlags, const BSTR bstrUrlContext, const BSTR bstrUrl);
	void   __stdcall OnProgressChange(int Progress, int ProgressMax);
	void   __stdcall OnStatusTextChange(const BSTR URL);
	void   __stdcall OnTitleChange(const BSTR URL);
	HRESULT  __stdcall OnShowMessage(const BSTR bstrCaption, const BSTR bstrText, DWORD dwType, LRESULT& lResult);

private:
	//CHtmlContent*   m_pWidgetContent;
};
