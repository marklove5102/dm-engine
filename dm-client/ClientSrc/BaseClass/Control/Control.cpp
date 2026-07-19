// 基础控件，为所有控件的基础

#include "Control.h"
#include "GameData/GameDefine.h"
#include "GameData/GameData.h"
#include "XmlBase.h"
#include "UiManager.h"
#include "LuaScripts/LuaScript.h"
#include "CtrlWindowX.h"
#include "ParserTip.h"
#include "CtrlMenuWnd.h"
#include "Global/Interface/StreamInterface.h"


#ifdef _DEBUG
#include "global/new.h"
#define new DEBUG_NEW
#endif

CControl* g_pThis = NULL;


//////////////////////////////////////////////////////////////////////
// 构造/解析函数
//////////////////////////////////////////////////////////////////////
DWORD CControl::m_dwTrans = 0xE0FFFFFF;
DTI_IMPLEMENT(CControl, CCtrlBaseTag)
CControl::CControl(void)
{
	m_iControlMode	= CTRL_MODE_BASE;
	m_pParent		= NULL;
	m_iX			= 0;	
	m_iY			= 0;
	m_iScreenX		= 0;
	m_iScreenY		= 0;
	m_iWidth		= 0;
	m_iHeight		= 0;
	m_iMX           = 0;
	m_iMY           = 0;
	m_dwBackColor	= 0xFFFFFFFF;
	//m_pBackTex		= NULL;
	m_dwBackTexID   = 0;
	m_pControlNext	= NULL;
	m_pControlPrev	= NULL;
	m_bFocus		= false;
	m_bNoFocus		= false;
	m_bModel		= false;
	m_iTrees		= 0;
	m_iFrame		= 0;
	m_dwFrameColor	= 0xFFFFFFFF;
	m_bClick		= false;
	m_bRBClick		= false;
	m_bMouseOn		= false;
	m_uStyle		= CTRL_STYLE_BACKMODE_NODRAW;
	m_iMarge		= 0;
	m_iFont			= FONT_DEFAULT;
	m_iFontSize		= FONTSIZE_DEFAULT;
	m_dwFontFlag    = 0;
	m_iTextOffX = m_iTextOffY = 0;
	m_dwColor		= 0xFFFFFFFF;
	//m_pTex			= NULL;
	m_bShow			= true;
	m_bEnable		= true;
	m_bNoMove		= false;
	m_bNoChangeLevel= false;
	m_cMask			= NULL;
	m_iMaskWidth	= 0;
	m_iMaskHeight	= 0;
	m_iTipTime		= SHOWTIPS_DELAY;
	m_dwMouseOnFrameCount = 0xFFFFFFFF;
	m_cColorIndex	= 0xFF;
	m_byFadeStep	= 0;
	m_byFadeTarget	= 0xFF;
	m_bEnableBeforeFade	= true;
	m_bNeedDisableWhileFading	= false;
	m_pMainWnd		= NULL;
	m_pInput		= NULL;
	m_iTexOffX	= 0;
	m_iTexOffY	= 0;
	m_iOffX = 0;
	m_iOffY = 0;
	m_iAlignType = XAL_TOPLEFT;
	m_iOriginalWidth	= 0;
	m_iOriginalHeight	= 0;
	m_iOriginalOffX     = 0;
	m_iOriginalOffY     = 0;
	m_bNeedTestAlpha	= FALSE;
	m_pXmlControl   = NULL;
	m_iMask     = 0;
	m_bScale = false;
	m_bScaleWidthAndHeight = true;
	m_bInheritScaleFromParent = true;

	m_dwStartTime = GetTickCount();
}

CControl::~CControl(void)
{
	Destroy();
}


//////////////////////////////////////////////////////////////////////
// 创建函数
//////////////////////////////////////////////////////////////////////

bool CControl::Create(CControl * pParent,int iX,int iY,int iW,int iH,UINT uStyle,DWORD dwBackcolor,DWORD dwBacktexID)
{
	m_pParent	= pParent;
	CCtrlMainWin* pMain = NULL;
	CControl* p = GetParent();
	while(p)
	{
		if(p->IsKindOf(DTI(CCtrlMainWin)))
		{
			if(p == g_pControl)
			{
				m_pInput = g_pInput;
			}

			m_pMainWnd = (CCtrlMainWin*)p;
		}
		p = p->GetParent();
	}

	//如果父窗口支持缩放,所有子控件也必须支持
	if(m_bInheritScaleFromParent && m_pParent && m_pParent->IsScale())
		m_bScale = true;

	SetStyle(uStyle);
	if( !ReSize(iW,iH) ) return false;

	if(!m_pXmlControl)//如果是从xml创建出来的在AssignXML里设置
	{
		m_iOffX = iX;
		m_iOffY = iY;
	}

	m_iOriginalOffX = m_iOffX;
	m_iOriginalOffY = m_iOffY;

	if( !RelativeMove()) return false;

	SetBackTexByID(dwBackcolor,dwBacktexID);
	if(IsFadeIn())
	{
		m_byFadeTarget = (BYTE)(m_dwBackColor >> 24);
		m_dwBackColor &= 0x00FFFFFF;
	}

	if( m_pParent )
		m_iTrees = m_pParent->m_iTrees + 1;
	else
		m_iTrees = 0;

	if(m_iMask > 0)
		SetMask(m_iMask);

	OnCreate();
	ExecuteScript(XSE_OnLoad);
	return true;
}

void CControl::Destroy()
{
	OnClose();

	// 如果风格为自删除，则在这里摧毁纹理
	//if( m_uStyle & CTRL_STYLE_SELFDELETE_TEX )
	//{
	//	g_pTexMgr->ReleaseTex(m_pTex);
	//}
	//if( m_uStyle & CTRL_STYLE_SELFDELETE_BACKTEX )
	//{
	//	g_pTexMgr->ReleaseTex(m_pBackTex);
	//}
	m_sText.clear();
	m_sTips.clear();

	SAFE_DELETE_ARRAY(m_cMask);
	
	if(m_pMainWnd)
	{ 
		if(m_pMainWnd->GetFocusCtrl() == this && m_pParent)
		{
			m_bModel = false;
			m_bClick = false;
			m_bRBClick = false;
			m_pParent->SetFocus();
		}

		if(m_pMainWnd->GetMouseOnCtrl() == this)
		{
			m_pMainWnd->SetMouseOnID(MOUSE_ON_INVALID);
		}

		if(this == m_pMainWnd->GetFocusCtrl() && this != m_pMainWnd)
		{
			g_pControl->SetFocusCtrl(NULL);
			DetailOutput("Lose Focus");
		}

		CCtrlMenuWnd* pMenuWnd = m_pMainWnd->GetMenuWnd();
		if(pMenuWnd && pMenuWnd->GetCaller() == this)//弹出菜单的来源窗口关掉了菜单也应该关掉
		{
			pMenuWnd->CloseMenu();
		}
	}
}

//从XML定义创建对象
bool CControl::CreateXML(CControl* pParent,CXmlControl* ctrl)
{
	if(!pParent || !ctrl)
		return false;

	AssignXML(ctrl);    

	int iW = 0,iH  = 0;
	ctrl->GetSize(iW,iH);

	DWORD dwBackTexID = 0;
	UINT uStyle = 0;
	int iIdx = ctrl->GetBackTexture().GetIndex();
	if(iIdx > 0)
	{
		uStyle |= CTRL_STYLE_BACKMODE_TEX | CTRL_STYLE_SELFDELETE_BACKTEX;
		dwBackTexID= MAKELONG(iIdx,PACKAGE_INTERFACE);
		ctrl->GetBackTexture().GetOffset(m_iTexOffX,m_iTexOffY);
	}
	else
	{
		uStyle |= CTRL_STYLE_BACKMODE_NODRAW;
	}
	return CControl::Create(pParent,m_iX,m_iY,iW,iH,uStyle,0xFFFFFFFF,dwBackTexID);
}

//////////////////////////////////////////////////////////////////////
// 属性相关函数
//////////////////////////////////////////////////////////////////////
//设置缩放值
//void CControl::SetScale(float f)
//{
//	m_fSize = f;
//
//	m_iWidth	= (int)(m_wOriginalWidth*f);
//	m_iHeight	= (int)(m_wOriginalHeight*f);
//	OnReSize();
//}

// 设置背景，背景的具体显示模式有“控件风格”决定
//void CControl::SetBack(DWORD dwBackcolor,LPTexture pBacktex, bool bAmb)
//{
//	m_dwBackColor	= dwBackcolor;
//	if(m_pBackTex != pBacktex)
//	{
//		if( m_pBackTex && (m_uStyle & CTRL_STYLE_SELFDELETE_BACKTEX) )
//		{
//			g_pTexMgr->ReleaseTex(m_pBackTex);
//		}
//		m_pBackTex		= pBacktex;
//	}
//}

void CControl::SetBackTexByID(DWORD dwBackcolor,DWORD dwBackTexID, bool bAmb)
{
	m_dwBackTexID = dwBackTexID;
	m_dwBackColor	= dwBackcolor;
}
// 设置边框
void CControl::SetFrame(int iFrame,DWORD dwFrameColor, bool bAmb)
{
	m_iFrame		= iFrame;
	m_dwFrameColor	= dwFrameColor;
	if( m_uStyle & CTRL_STYLE_AUTOSIZE ) OnAutoSize();
}

// 设置边框
void CControl::SetMarge(int iMarge)
{
	m_iMarge = iMarge; 
	if( m_uStyle & CTRL_STYLE_AUTOSIZE ) OnAutoSize();
}

// 设置控件使用的字体
void CControl::SetFont(int iFont,int iFontSize,DWORD dwFontFlag)
{
	if(iFont >= 0)
	{
		m_iFont	= iFont;
	}
	else
	{
		m_iFont = FONT_DEFAULT;
	}

	m_dwFontFlag = dwFontFlag;

	if(iFontSize > 0)
		m_iFontSize = iFontSize;
	else
		m_iFontSize = FONTSIZE_DEFAULT;

	if( m_uStyle & CTRL_STYLE_AUTOSIZE ) OnAutoSize();
}

void CControl::GetFont(int * pFont, int * pFontSize,DWORD *pFlag)
{
	if( pFont ) *pFont = m_iFont;
	if( pFontSize ) *pFontSize = m_iFontSize;
}

int CControl::GetFont()
{
	return m_iFont;
}

int CControl::GetFontSize()
{
	return m_iFontSize;
}

DWORD CControl::GetFontFlag()
{
	return m_dwFontFlag;
}

// 设置控件使用的字符串
void CControl::SetText(const char * sText)
{
	if(!sText) 
		return;

	m_sText.assign(sText);

	if( m_uStyle & CTRL_STYLE_AUTOSIZE ) OnAutoSize();
}

//void CControl::SetTex(LPTexture pTex)
//{
//	if( m_pTex && m_uStyle & CTRL_STYLE_SELFDELETE_TEX )
//	{
//		g_pTexMgr->ReleaseTex(m_pTex);
//	}
//	m_pTex = pTex; 
//	if( m_uStyle & CTRL_STYLE_AUTOSIZE ) OnAutoSize();
//}

// 判断this控件是不是pCtrl控件的父控件
bool CControl::IsParent(CControl * pCtrl)	
{	
	while(pCtrl && pCtrl->m_iTrees > m_iTrees)
	{
		pCtrl = pCtrl->m_pParent;
	}

	if(pCtrl == this)
		return true;
	else
		return false;
}

bool CControl::SetMask(WORD wMask,int w,int h)
{
	if(wMask == 0)
	{
		return false;
	}

	if(w < 1 || h < 1)
	{
		w = m_iOriginalWidth;
		h = m_iOriginalHeight;
	}

	m_iMask = wMask ;


	int iFolder = wMask/100;
	int iFile = wMask%100;

	string path = StringUtil::format("mask\\%03d\\%05d.Msk",iFolder,wMask);
	if(!g_pStreamMgr)
		return false;

	DataStreamInterface* stream = g_pStreamMgr->OpenDataFile(path.c_str(),false,false,false,EP_MOST_HIGH);

	if (!stream)
	{
		return false;
	}

	//让前面先调用g_pStreamMgr->OpenDataFile,如果文件不存在就会先下载下来
	if (wMask <= 0 || w <= 0 || h <= 0)
	{
		return false;
		SAFE_DELETE(stream);
	}


	m_iMaskWidth  = w;
	m_iMaskHeight = h;
	int len = (w*h+7) / 8;

	SAFE_DELETE_ARRAY(m_cMask);
	m_cMask = new UCHAR[len];
	memset(m_cMask,0,len);

	int i = stream->read(m_cMask, len);
	SAFE_DELETE(stream);

	if( i < len - 1)
	{
		SAFE_DELETE_ARRAY(m_cMask);
		m_iMaskWidth  = 0;
		m_iMaskHeight = 0;
		return false;
	}

	return true;
}

bool CControl::SetMaskBuf(UCHAR * pMask,int w,int h)
{
	m_cMask = pMask;

	if (w > 0)
	{
		m_iMaskWidth  = w;
	}

	if (h > 0)
	{
		m_iMaskHeight = h;
	}

	return true;
}


bool CControl::IsMouseOn(int iX,int iY)
{
	int x = iX - m_iScreenX;
	int y = iY - m_iScreenY;
	return IsInControl(x,y);
}

bool CControl::IsInMask(int iX,int iY)
{

	if(DoesTestAlpha())
	{
		LPTexture pTex = g_pTexMgr->GetTexImm(m_dwBackTexID,EP_UI);
		if(pTex)
		{
			return pTex->IsPointInTex(iX, iY) == 2;
		}
	}

	if( !m_cMask )
		return true;

	if(m_bScale)
	{
		iX = (int)(iX / g_ScaleRate.fx + 0.5);
		iY = (int)(iY / g_ScaleRate.fy + 0.5);
	}

	if( iX<0 || iX>=m_iMaskWidth || iY<0 || iY>=m_iMaskHeight )
		return false;

	int i;
	i = iY*m_iMaskWidth + iX;
	if( (m_cMask[i/8] >> (7-(i%8))) & 0x01 )
		return true;
	else
		return false;
}

bool CControl::IsInControl(int iX,int iY)
{
	if(iX < 0 || iX >= m_iWidth || iY < 0 || iY >= m_iHeight)
		return false;

	return IsInMask(iX,iY);
}

void CControl::SetTips(const char * str)
{
	if(!str) return;
	m_sTips.assign(str);
}

const char * CControl::GetTips()
{
	return m_sTips.c_str();
}

const char * CControl::IsShowTips(DWORD dwFrameCount)
{
	if(m_sTips.empty() || (dwFrameCount-m_dwMouseOnFrameCount)<m_iTipTime)
		return NULL;
	return m_sTips.c_str();
}

//////////////////////////////////////////////////////////////////////
// 位置和大小相关函数
//////////////////////////////////////////////////////////////////////
bool CControl::AssignXML(CXmlControl* ctrl)
{
	if(!ctrl)
		return false;

	string align = ctrl->GetAlign();

	static char* ALIGN_STR[] = 
	{
		"TopLeft",
		"Top",
		"TopRight",
		"Right",
		"BottomRight",
		"Bottom",
		"BottomLeft",
		"Left",
		"Center",
	};

	m_iAlignType = XAL_TOPLEFT;
	while(m_iAlignType < XAL_NUMS)
	{
		if(stricmp(align.c_str(),ALIGN_STR[m_iAlignType]) == 0)
			break;
		m_iAlignType++;
	}

	ctrl->GetOffset(m_iOffX,m_iOffY);
	m_iOriginalOffX = m_iOffX;
	m_iOriginalOffY = m_iOffY;

	m_sTips.assign(ctrl->GetTips());
	m_iMask = ctrl->GetMask();
	m_sName = ctrl->GetName();
	m_pXmlControl = ctrl; //脚本

	return true;
}

bool CControl::RelativeMove(int iType)
{
	int iW = 0,iH = 0;
	if(m_pParent)
	{
		iW = m_pParent->GetWidth();
		iH = m_pParent->GetHeight();
	}
	else //默认父窗口为MainWnd
	{
		iW = g_pControl->GetWidth();
		iH = g_pControl->GetHeight();
	}

	if(m_bScale)
	{
		m_iOffX = (int)(g_ScaleRate.fx * m_iOriginalOffX + 0.5);
		m_iOffY = (int)(g_ScaleRate.fy * m_iOriginalOffY + 0.5);
	}
	else
	{
		m_iOffX = m_iOriginalOffX;
		m_iOffY = m_iOriginalOffY;
	}

	if(iType != XAL_NONE)
		m_iAlignType = iType;

	if(m_iAlignType == XAL_CENTER || m_iAlignType == XAL_TOP || m_iAlignType == XAL_BOTTOM)
	{
		m_iX = (iW - m_iWidth) / 2;
	}
	else if(m_iAlignType == XAL_TOPRIGHT || m_iAlignType == XAL_RIGHT || m_iAlignType == XAL_BOTTOMRIGHT)
	{
		m_iX = iW - m_iWidth;
	}
	else
	{
		m_iX = 0;
	}

	if(m_iAlignType == XAL_CENTER || m_iAlignType == XAL_LEFT || m_iAlignType == XAL_RIGHT)
	{
		m_iY = (iH - m_iHeight) / 2;
	}
	else if(m_iAlignType == XAL_BOTTOMLEFT || m_iAlignType == XAL_BOTTOM || m_iAlignType == XAL_BOTTOMRIGHT)
	{
		m_iY = iH - m_iHeight;
	}
	else
	{
		m_iY = 0;
	}


	Move(m_iX+m_iOffX,m_iY+m_iOffY);
	return true;
}

bool CControl::Move(int iX,int iY)
{
	if(iX < POS_NOMOVE)
		m_iX = iX;

	if(iY < POS_NOMOVE)
		m_iY = iY;

	OnMove();

	return true;
}

bool CControl::ReSize()
{
	m_iWidth  =  m_iOriginalWidth;
	m_iHeight =  m_iOriginalHeight;

	if(m_bScale && m_bScaleWidthAndHeight)
	{
		m_iWidth = (int)(g_ScaleRate.fx * m_iOriginalWidth + 0.5);
		m_iHeight = (int)(g_ScaleRate.fy * m_iOriginalHeight + 0.5);
	}

	OnReSize();
	return true;
}

bool CControl::ReSize(int iW,int iH)
{
	if(iW > POS_NORESIZE)
		m_iOriginalWidth  =  iW;

	if(iH > POS_NORESIZE)
		m_iOriginalHeight =  iH;

	return ReSize();
}

void CControl::OnMove()
{
	if( m_pParent )
	{
		m_iScreenX = m_pParent->m_iScreenX + m_iX;
		m_iScreenY = m_pParent->m_iScreenY + m_iY;
	}
	else 
	{
		m_iScreenX = m_iX;
		m_iScreenY = m_iY;
	}
}

void CControl::OnReSize()
{
}


//////////////////////////////////////////////////////////////////////
// 消息响应函数
//////////////////////////////////////////////////////////////////////

void CControl::OnCreate()
{
}

void CControl::OnClose()
{
}

bool CControl::OnKeyDown(WORD wState,UCHAR cKey)
{
	return false;
}

bool CControl::OnKeyUp(WORD wState,UCHAR cKey)
{
	return false;
}

bool CControl::OnChar(UCHAR cChar)
{
	return false;
}

bool CControl::OnMouseMove(int iX,int iY)
{
	return false;
}

bool CControl::OnWheel(int iWheel)
{
	return false;
}

bool CControl::OnCaptureChanged()
{
	return false;
}

// 左键按下时标记为被点击
bool CControl::OnLeftButtonDown(int iX,int iY)
{	
	m_bClick = true;
	return true;
}

bool CControl::OnLeftButtonUp(int iX,int iY)
{
	if( m_bClick )
	{
		m_bClick = false;
		return OnClick(INPUT_MOUSE_LEFTBT);
	}
	return true;
}

bool CControl::OnLeftButtonDClick(int iX,int iY)
{
	return true;
}

bool CControl::OnRightButtonDown(int iX,int iY)
{
	if( !m_bRBClick ) 
		m_bRBClick = true;
	return true;
}

bool CControl::OnRightButtonUp(int iX,int iY)
{
	if( m_bRBClick )
		m_bRBClick = false;
	return true;
}

bool CControl::OnRightButtonDClick(int iX,int iY)
{
	return true;
}

bool CControl::OnMiddleButtonDown(int iX,int iY)
{
	return true;
}

bool CControl::OnMiddleButtonUp(int iX,int iY)
{
	return true;
}

bool CControl::OnClick(int iButton)
{
	return true;
}

// 是否为子控件的消息
CControl * CControl::IsSonsMsg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	return NULL;
}

// 消息响应函数
bool CControl::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	TRY_BEGIN;
	WORD w1,w2;
	int x,y;

	if( !m_bEnable ) return false;

	w1 = HIWORD( dwData );
	w2 = LOWORD( dwData );

	switch(dwMsg) 
	{
	case MSG_INPUT_KEYDOWN:		// 只有有Focus的控件会收到这个消息
		if( (UCHAR)w2 == VK_TAB) 
		{
			CControl * p = m_pControlNext;
			if( !p ) 
			{
				if( (CControlContainer *)m_pParent ) 
					p = ((CControlContainer *)m_pParent)->GetControls();
				else
					return true;
			}

			while( p && p!=this ) 
			{
				if(p->IsNeedKeyInput() && p->CanFocus())
				{
					p->SetFocus();
					break;
				}
				p = p->GetControlNext();
				if( !p ) 
				{
					if((CControlContainer *)m_pParent ) 
						p = ((CControlContainer *)m_pParent)->GetControls();
					else
						return true;
				}
			}
			return true;
		}
		return OnKeyDown( w1 , (UCHAR)w2 );

	case MSG_INPUT_KEYUP:	// 只有有Focus的控件会收到这个消息
		return OnKeyUp( w1 , (UCHAR)w2 );

	case MSG_INPUT_CHAR:	// 只有有Focus的控件会收到这个消息
		return OnChar( (UCHAR)w2 );

	case MSG_INPUT_WHEEL:	// 只有有Focus的控件会收到这个消息
		return OnWheel( (int)w2 );

	case MSG_INPUT_CAPTURECHANGED:	// 只有有Focus的控件会收到这个消息
		return OnCaptureChanged();

	case MSG_INPUT_MOVE:
		{
			bool bOldMouseOn;
			x = (short)w2 - m_iScreenX;
			y = (short)w1 - m_iScreenY;
			bOldMouseOn = m_bMouseOn;

			if(IsInControl(x,y) )
			{		
				m_bMouseOn = true;
			}
			else
			{
				m_bMouseOn = false;
				if( !m_bClick && !m_bRBClick) return false;
			}
			if( IsSonsMsg(dwMsg,dwData,pControl) ) return true;

			if( m_bMouseOn && !bOldMouseOn )
			{
				m_dwMouseOnFrameCount = g_pGfx->GetFrameCount();
			}
			return OnMouseMove(x,y);
		}

	case MSG_INPUT_LEFTBT_DOWN:
		x = (short)w2 - m_iScreenX;
		y = (short)w1 - m_iScreenY;
		if( !IsInControl(x,y) ) return false;
		if( IsSonsMsg(dwMsg,dwData,pControl) ) return true;
		if( !m_bFocus && !SetFocus() ) return false;
		return OnLeftButtonDown(x,y);

	case MSG_INPUT_LEFTBT_UP:
		x = (short)w2 - m_iScreenX;
		y = (short)w1 - m_iScreenY;
		if( !IsInControl(x,y) && !m_bClick ) return false;
		if( !m_bClick && IsSonsMsg(dwMsg,dwData,pControl) ) return true;
		if( !m_bClick && !m_bFocus && !SetFocus() ) return false;
		return OnLeftButtonUp(x,y);

	case MSG_INPUT_LEFTBT_DCLICK:
		x = (short)w2 - m_iScreenX;
		y = (short)w1 - m_iScreenY;
		if( !IsInControl(x,y) ) return false;
		if( IsSonsMsg(dwMsg,dwData,pControl) ) return true;
		if( !m_bFocus && !SetFocus() ) return false;
		return OnLeftButtonDClick(x,y);

	case MSG_INPUT_RIGHTBT_DOWN:
		x = (short)w2 - m_iScreenX;
		y = (short)w1 - m_iScreenY;
		if( !IsInControl(x,y) ) return false;
		if( IsSonsMsg(dwMsg,dwData,pControl) ) return true;
		if( !m_bFocus && !SetFocus() ) return false;
		return OnRightButtonDown(x,y);
	case MSG_INPUT_RIGHTBT_UP:
		x = (short)w2 - m_iScreenX;
		y = (short)w1 - m_iScreenY;
		if( !IsInControl(x,y) && !m_bRBClick ) return false;
		if( IsSonsMsg(dwMsg,dwData,pControl) ) return true;
		if( !m_bFocus && !SetFocus() ) return false;
		return OnRightButtonUp(x,y);

	case MSG_INPUT_RIGHTBT_DCLICK:
		x = (short)w2 - m_iScreenX;
		y = (short)w1 - m_iScreenY;
		if( !IsInControl(x,y) ) return false;
		if( IsSonsMsg(dwMsg,dwData,pControl) ) return true;
		if( !m_bFocus && !SetFocus() ) return false;
		return OnRightButtonDClick(x,y);

	case MSG_INPUT_MIDDLEBT_DOWN:
		x = (short)w2 - m_iScreenX;
		y = (short)w1 - m_iScreenY;
		if( !IsInControl(x,y) ) return false;
		if( IsSonsMsg(dwMsg,dwData,pControl) ) return true;
		if( !m_bFocus && !SetFocus() ) return false;
		return OnMiddleButtonDown(x,y);

	case MSG_INPUT_MIDDLEBT_UP:
		x = (short)w2 - m_iScreenX;
		y = (short)w1 - m_iScreenY;
		if( !IsInControl(x,y) ) return false;
		if( IsSonsMsg(dwMsg,dwData,pControl) ) return true;
		if( !m_bFocus && !SetFocus() ) return false;
		return OnMiddleButtonUp(x,y);

	case MSG_POPEDIT_CHOICE_SET:
		return true;
	}
	return (IsSonsMsg(dwMsg,dwData,pControl) != NULL);

	TRY_END_DO(
		char szTemp[512]={0};
	sprintf(szTemp,"MsgID:%u,ControlName:%s,DTIName:%s,ParentDTIName:%s",dwMsg,this->GetName(),this->GetDTIName(),m_pParent?m_pParent->GetDTIName():"");
	strAddMsg = szTemp;
	)

		return false;
}

//////////////////////////////////////////////////////////////////////
// 焦点相关函数
//////////////////////////////////////////////////////////////////////
// 设置焦点
bool CControl::SetFocus()
{
	if( !m_pMainWnd) return false;

	if(m_pMainWnd->GetTipWnd())//在最后退出时CtrlMainWin会把TipWnd析构掉，在析构其它控件时会调用SetFocus
	{
		m_pMainWnd->GetTipWnd()->Clear();
		m_pMainWnd->GetTipWnd()->SetShow(false);
	}

	if( m_bNoFocus )	// 如果此控件不需要焦点
	{
		if( m_pParent )
			return m_pParent->SetFocus();
		else
			return false;
	}

	CControl * pFocusNow = m_pMainWnd->GetFocusCtrl();

	CCtrlMenuWnd* pMenuWnd = m_pMainWnd->GetMenuWnd();
	if(pMenuWnd && pMenuWnd == pFocusNow)//弹出菜单失去焦点则关掉
	{
		pMenuWnd->CloseMenu();
		pFocusNow = m_pMainWnd->GetFocusCtrl();
	}

	if(pFocusNow && pFocusNow != this)
	{
		TRY_BEGIN
			if(pFocusNow->KillFocus(this)) 
			{
				m_pMainWnd->SetFocusCtrl(this);
				m_bFocus = true;
				OnSetFocus();
				if(this->IsNeedKeyInput())
				{
					m_pMainWnd->Msg(MSG_CTRL_RELEASE_FAKEFOCUS,0,NULL);
				}
				return true;
			}
			else
				return false;
		TRY_END_RETURN(false)
	}
	else 
	{
		m_pMainWnd->SetFocusCtrl(this);
		m_bFocus = true;
		OnSetFocus();
		return true;
	}
}

// 失去焦点
bool CControl::KillFocus(CControl * pNewFocus)
{
	if(!pNewFocus)
		return false;

	if((m_bClick || m_bRBClick) && !pNewFocus->GetMode())//如果新窗口是模式框,要求获得焦点,否则可能会出现模式框被一个普通大窗口覆盖在上面的情况,这会导致一直没法动
		return false;

	if(IsParent(pNewFocus))
	{
		m_bFocus = false;
		OnKillFocus();
		return true;
	}
	else if(m_bModel && !pNewFocus->m_bModel)
	{
		return false;
	}
	else if(m_pParent)
	{
		if(m_pParent->KillFocus(pNewFocus))
		{
			m_bFocus = false;
			OnKillFocus();
			return true;
		}
		else 
			return false;
	}
	else 
	{
		m_bFocus = false;
		OnKillFocus();
		return true;
	}
	return false ;
}

void CControl::OnSetFocus()
{
}

void CControl::OnKillFocus()
{
}

void CControl::OnAutoSize()
{
}


//////////////////////////////////////////////////////////////////////
// 绘制函数
//////////////////////////////////////////////////////////////////////

void CControl::Draw()
{
	float fScaleX = 1.0f;
	float fScaleY = 1.0f;
	if(m_bScale)
	{
		fScaleX = g_ScaleRate.fx;
		fScaleY = g_ScaleRate.fy;
	}

	int iMX = 0, iMY = 0;
	if(m_pMainWnd)
	{
		m_pMainWnd->GetMouseXY(iMX, iMY);
	}
	iMX -= GetScreenX();
	iMY -= GetScreenY();
	if(IsInControl(iMX, iMY))
	{
		m_bMouseOn = true;
	}
	else
	{
		m_bMouseOn = false;
	}

	bool bShowTips = true;
	if(m_pMainWnd)
	{
		bShowTips = !m_pMainWnd->IsHideCursor();
		if( m_bMouseOn && !m_sTips.empty() && bShowTips)
			m_pMainWnd->SetMouseOnID(MOUSE_ON_CONTROL,this);
	}

	if(IsFadeIn())
	{
		BYTE byCurOpacity = (BYTE)(m_dwBackColor>>24);
		if(byCurOpacity == 0 && m_bNeedDisableWhileFading)
		{
			m_bEnableBeforeFade = IsEnable();
			SetEnable(false);
		}
		if(byCurOpacity + IsFadeIn() < m_byFadeTarget)
		{
			m_dwBackColor += (IsFadeIn()<<24);
		}
		else
		{
			m_dwBackColor |= (m_byFadeTarget<<24);
			SetFadeIn(0);
			if(m_bNeedDisableWhileFading)
			{
				SetEnable(m_bEnableBeforeFade);
			}
		}
	}


	if( m_uStyle & CTRL_STYLE_BACKMODE_NODRAW )
	{
	}
	else if( m_uStyle & CTRL_STYLE_BACKMODE_COLOR )
	{
		if( m_iFrame )
		{
			g_pGfx->DrawFillRect(m_iScreenX,m_iScreenY,(int)(m_iWidth*fScaleX),(int)(m_iHeight*fScaleY), m_dwBackColor);
			g_pGfx->DrawRect(m_iScreenX-m_iFrame,m_iScreenY-m_iFrame,(int)((m_iWidth+m_iFrame)*fScaleX),(int)((m_iHeight+m_iFrame)*fScaleY), m_dwFrameColor);
		}
		else
		{
			g_pGfx->DrawFillRect(m_iScreenX,m_iScreenY,(int)(m_iWidth*fScaleX),(int)(m_iHeight*fScaleY), m_dwBackColor);
		}
	}
	else if( m_uStyle & CTRL_STYLE_BACKMODE_TEX)
	{
		if( m_iFrame ) 
		{
			g_pGfx->DrawRect(m_iScreenX,m_iScreenY,(int)(m_iWidth*fScaleX),(int)(m_iHeight*fScaleY),m_dwFrameColor);
		}

		LPTexture pTex = g_pTexMgr->GetTexImm(m_dwBackTexID,EP_UI);

		if (pTex != NULL)
		{
			//刚开始没有图,后来读到图了,重设大小及位置
			if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0)
			{
				ResetWidthAndHeight(pTex->GetWidth(),pTex->GetHeight());
			}
			if(m_bScale)
			{
				g_pGfx->DrawTextureFX(m_iScreenX + (int)((m_iFrame  + m_iTexOffX)*g_ScaleRate.fx),
					m_iScreenY + (int)((m_iFrame + m_iTexOffY)*g_ScaleRate.fy),
					pTex, m_dwBackColor, NULL, &g_ScaleRate);
			}
			else
			{
				DrawTexture(m_iFrame + m_iTexOffX,m_iFrame + m_iTexOffY,pTex,m_dwBackColor);
			}
		}
	}

	OnDraw();
}

void CControl::OnDraw()
{
}

BOOL CControl::IsNeedKeyInput()
{
	if(m_iControlMode == CTRL_MODE_EDIT || m_iControlMode == CTRL_MODE_MULIEDIT)
		return TRUE;

	return FALSE;
}

void CControl::DrawTexture(int iX,int iY, int iIdx,DWORD dwColor)
{
	LPTexture pTex = g_pTexMgr->GetTexImm(PACKAGE_INTERFACE,iIdx,EP_UI);
	DrawTexture(iX,iY,pTex,dwColor);
}

void CControl::DrawTexture(int iX, int iY, LPTexture pTex, DWORD dwColor,LPRECT pRect)
{
	if(!pTex)
		return;

	if(IsFadeIn())
		dwColor &= m_dwBackColor;

	if(m_bScale)
	{
		g_pGfx->DrawTextureFX(m_iScreenX + (int)(iX*g_ScaleRate.fx + 0.5),
			m_iScreenY + (int)(iY*g_ScaleRate.fy + 0.5),
			pTex, dwColor, pRect, &g_ScaleRate);
	}
	else
	{
		if (pRect)
		{
			g_pGfx->DrawTextureFX(m_iScreenX + iX,m_iScreenY + iY,pTex, dwColor, pRect);
		}
		else
		{
			g_pGfx->DrawTextureNL(m_iScreenX + iX,m_iScreenY + iY,pTex,dwColor);
		}
	}
}

void CControl::TextOut(int iX, int iY, const char * str, DWORD dwColor, DWORD dwFlag, DWORD dwBackColor)
{
	if(IsFadeIn())
	{
		dwBackColor &= m_dwBackColor;
	}
	if(str)
	{
		if(m_bScale)
			g_pFont->DrawText(m_iScreenX + (int)(iX * g_ScaleRate.fx + 0.5),m_iScreenY + (int)(iY * g_ScaleRate.fy + 0.5),str,dwColor,m_iFont,m_iFontSize, dwFlag, dwBackColor);
		else
			g_pFont->DrawText(m_iScreenX + iX,m_iScreenY + iY,str,dwColor,m_iFont,m_iFontSize, dwFlag, dwBackColor);
	}
}

DWORD CControl::GetColorByIndex()
{
	return g_pGameData->GetMirColor(m_cColorIndex);
}

void CControl::SetFadeIn(BYTE byStep)
{
	m_byFadeStep = byStep;
}

void CControl::DrawTextureZoom(int iX, int iY, LPTexture pTex, float fX, float fY, DWORD dwColor, RECT* rc)
{
	if(!pTex)
		return;

	g_pGfx->SetRenderMode(RM_LINEAR);

	g_pGfx->DrawTextureFX(m_iScreenX + iX,m_iScreenY + iY,pTex,dwColor,rc,&POS(fX,fY));

	//g_pGfx->SetRenderMode(RM_POINT);
	g_pGfx->SetRenderMode();
}

bool CControl::ExecuteScript(int e)
{
	if(m_pXmlControl)
	{
		string str = m_pXmlControl->GetScript(e);
		if(str.size() != 0)
		{
			g_pThis = this;
			g_LuaScript.ExecuteString(str);
			return true;
		}
	}
	return false;
}

void CControl::SetTextOff(int iOffX,int iOffY)
{
	m_iTextOffX = iOffX;
	m_iTextOffY = iOffY;
}

void CControl::GetTextOff(int &iOffX,int &iOffY)
{
	iOffX = m_iTextOffX;
	iOffY = m_iTextOffY;
}

int CControl::GetTextOffX()
{
	return m_iTextOffX;
}

int CControl::GetTextOffY()
{
	return m_iTextOffY;
}

void CControl:: SetHaveFocus(bool b)
{
	m_bFocus = b;
}

void CControl::AfterDraw()
{

}

void CControl::SetStartTime(DWORD dwTime)
{
	m_dwStartTime = dwTime;
}

DWORD CControl::GetStartTime()
{
	return m_dwStartTime;
}

void CControl::ResetControlPos()
{
	//if(!m_bScale)
	//	return;

	ReSize();
	RelativeMove();
}

void CControl::SetEnable(bool bEnable)
{ 
	m_bEnable = bEnable; 
	if (!m_bEnable)
	{
		m_bClick = false;
		m_bRBClick = false;
	}
}

void CControl::OnSwitchToTop()
{

}

void CControl::ResetWidthAndHeight(int iW,int iH)
{
	ReSize(iW,iH);

	if (m_iMask > 0)
	{
		SetMask((WORD)m_iMask);
	}

	ResetControlPos();
}
