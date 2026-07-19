#include "WidgetProxy.h"
#include "WidgetManager.h"

#include "comutil.h"
#pragma comment(lib,"comsuppw.lib")

BSTR str2BSTR(const char *s)
{
	BSTR bstr= 0;
#ifndef UNICODE
	{
		wchar_t  *buffer;
		DWORD    size;

		size = MultiByteToWideChar(CP_ACP, 0, s, -1, 0, 0);
		if (!(buffer = (wchar_t *)GlobalAlloc(GMEM_FIXED, sizeof(wchar_t) * size)))
			return 0;
		MultiByteToWideChar(CP_ACP, 0, s, -1, buffer, size);
		bstr = SysAllocString(buffer);
		GlobalFree(buffer);
	}
#else
	bstr = SysAllocString(s);
#endif
	return bstr;
};

BSTR string2BSTR(std::string &s)
{
	return str2BSTR(s.c_str());
};

bool BSTR2string(BSTR bstr,std::string &s)
{
	if(!bstr)
		return false;

	_bstr_t b(bstr);
	s = (char *)b;

	return true;
}

bool VariantBSTR2string(VARIANT a,std::string &s)
{
	if (a.vt == VT_BSTR) 
	{
		return BSTR2string(a.bstrVal,s);
	}
	return false;
}

bool Variant2Str(VARIANT &arg,std::string &s)
{
	if(arg.vt == VT_BSTR)
	{
		return BSTR2string(arg.bstrVal,s);
	}
	else if(arg.vt == (VT_VARIANT|VT_BYREF))
	{
		VARIANT *pvarVal = arg.pvarVal;
		if(pvarVal && pvarVal->vt == VT_BSTR)
			return BSTR2string(pvarVal->bstrVal,s);
	}
	else 
	{
		VARIANT a{};
		if (::VariantChangeType(&a, &arg, 0, VT_BSTR) == S_OK) 
		{
			if (a.vt == VT_BSTR) 
				return BSTR2string(a.bstrVal,s);
		}
	}
	return false;
}

CWidgetProxy::CWidgetProxy(void)
{
	//m_pWidgetContent = NULL;
}

CWidgetProxy::~CWidgetProxy(void)
{
}

HWND   __stdcall CWidgetProxy::Get_WindowHandle()
{
	return g_hWnd;
}

void   __stdcall CWidgetProxy::OnPositionChange( const RECT * rcBounds )
{

}

void   __stdcall CWidgetProxy::OnFocusChange( BOOL Focus )
{

}

void   __stdcall CWidgetProxy::OnVisibleChange( BOOL Visible )
{

}

BOOL   __stdcall CWidgetProxy::OnTranslateAccelerator( tagMSG * Msg )
{
	return FALSE;
}

BOOL   __stdcall CWidgetProxy::OnTranslateIME( tagMSG * Msg )
{
	return FALSE;
}

void    __stdcall CWidgetProxy::OnExecute(const VARIANT * CmdID, const VARIANT * CmdParam, VARIANT& vResult)
{
	//m_pWidgetContent->DoCommand(CmdID, CmdParam, vResult);
}

void    __stdcall CWidgetProxy::OnCloseQuery(BOOL& CanClose)
{	
	g_WidgetMgr.OnCloseQuery(CanClose, this);
}

void    __stdcall CWidgetProxy::OnCursorChange(HCURSOR Cursor)
{
}

void    __stdcall CWidgetProxy::OnFullScreen(BOOL FullScreen)
{

}

void   __stdcall CWidgetProxy::OnBeforeNavigate(const BSTR URL, DWORD Flags, const BSTR TargetFrameName, const VARIANT * PostData, const BSTR Headers, BOOL& Cancel)
{
	Cancel = FALSE;
}

void   __stdcall CWidgetProxy::OnNavigateError(const BSTR URL, const BSTR Frame, DWORD StatusCode, BOOL& Cancel)
{
}

void   __stdcall CWidgetProxy::OnNavigateComplete(const BSTR URL)
{
}

void   __stdcall CWidgetProxy::OnDownloadBegin()
{
}

void   __stdcall CWidgetProxy::OnDownloadComplete()
{
}

void   __stdcall CWidgetProxy::OnDocumentComplete(const BSTR URL)
{
	string str;
	BSTR2string(URL,str);

	//output_debug("RETURN:%s\n",str.c_str());

	int pos = str.find_last_of('/');
	if(pos < 0)
		return;

	int endpos = str.find_last_of('?');
	if(endpos < pos || endpos < 0)
		endpos = str.size();

	string strFileName = str.substr(pos+1,endpos-pos-1);
	g_WidgetMgr.OnDocComplete(strFileName.c_str(),str.c_str(),this);

	if(strnicmp(strFileName.c_str(),"guid.",5) == 0) //GUID页面
	{
		BSTR id = str2BSTR("guid");
		//VARIANT RetVal;
		/*
		if(S_OK == m_pWidgetContent->GetElementValue(id,RetVal))
		{
			string strVal;
			if(Variant2Str(RetVal,strVal))
			{
				g_WidgetMgr.OnGUID(strVal.c_str());
			}
		}
		*/
		SAFE_DEL_BSTR(id);
	}
}

void   __stdcall CWidgetProxy::OnNewWindow(IDispatch*& ppDisp, BOOL& Cancel, DWORD dwFlags, const BSTR bstrUrlContext, const BSTR bstrUrl)
{
	Cancel = TRUE;
    //Cancel = FALSE;//允许弹出新窗口,否则内嵌充值中的弹出页面就会弹不出来
}

void   __stdcall CWidgetProxy::OnProgressChange(int Progress, int ProgressMax)
{
}

void   __stdcall CWidgetProxy::OnStatusTextChange(const BSTR URL)
{
}

void   __stdcall CWidgetProxy::OnTitleChange(const BSTR URL)
{
	string str;
	BSTR2string(URL,str);
	g_WidgetMgr.OnTitleChange(str.c_str(),this);
}

HRESULT  __stdcall CWidgetProxy::OnShowMessage(const BSTR bstrCaption, const BSTR bstrText, DWORD dwType, LRESULT& lResult)
{
	string strCaption,strText;
	BSTR2string(bstrCaption,strCaption);
	BSTR2string(bstrText,strText);

	lResult = ::MessageBox(g_hWnd,strText.c_str(),strCaption.c_str(),dwType);

	return S_OK;
}
