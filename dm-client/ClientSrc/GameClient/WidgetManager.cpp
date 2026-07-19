#include "WidgetManager.h"
#include "GameData/WooolStoreData.h"
#include "GameControl/GameControl.h"
#include "BaseClass/Control/Control.h"
#include "Global/Interface/StreamInterface.h"
#include "GameData/OtherData.h"
#include "GameData/BoothData.h"
#include "Global/Standard3DES.h"

#include <unknwn.h>
#include <comutil.h>
#include <OleAuto.h>

CWidgetManager g_WidgetMgr;
HMODULE        a_DllModule = NULL;

//从Dll中导出CWidgetContent的函数
typedef DECLSPEC_IMPORT CWidgetContent * (WINAPI* LP_CreateWidgetContent)(CHostWindowProxy *, int);
LP_CreateWidgetContent _CreateWidgetContent;

CWidgetContent * __stdcall CreateWidgetContent(CHostWindowProxy * HostWindow, int Flag)
{
	if (!_CreateWidgetContent)
	{
		a_DllModule = LoadLibrary("WidgetContent.IE.dll");
		if (a_DllModule) 
			_CreateWidgetContent = (LP_CreateWidgetContent)GetProcAddress(a_DllModule, "CreateWidgetContent");

	}

	if (_CreateWidgetContent)
		return _CreateWidgetContent(HostWindow, Flag);
	else
		return (CWidgetContent *)NULL;
}

CWidgetManager::CWidgetManager(void)
{
	for (int t = EWT_FIRST;t < EWT_NUM;t++)
	{
		//m_pWidgetContent[t] = NULL;
		m_pWindowProxy[t] = NULL;
		m_pWidgetTex[t] = NULL;
		m_bShow[t] = false;
		m_bFocus[t] = false;
		m_rcBound[t] = SRect(0,0,0,0);
	}
}

CWidgetManager::~CWidgetManager(void)
{
	for (int t = EWT_FIRST;t < EWT_NUM;t++)
	{
		if(m_pWidgetTex[t])
			g_pTexMgr->ReleaseTex(m_pWidgetTex[t]);

		//if(m_pWidgetContent[t])
		//	m_pWidgetContent[t]->Release();

		SAFE_DELETE(m_pWindowProxy[t]);
	}
}

void CWidgetManager::Release()
{
	for (int t = EWT_FIRST;t < EWT_NUM;t++)
	{
		if(m_pWidgetTex[t])
			g_pTexMgr->ReleaseTex(m_pWidgetTex[t]);

		//if(m_pWidgetContent[t])
		//{
		//	m_pWidgetContent[t]->Release();
		//	m_pWidgetContent[t] = NULL;
		//}
		SAFE_DELETE(m_pWindowProxy[t]);
	}
}

void CWidgetManager::LoginRadio(const char* strRadioUrl)
{
	SetShow(EWT_RADIO,false);
	Navigate(EWT_RADIO,strRadioUrl);
	g_OtherData.SetRadioOpen(false);
}

void CWidgetManager::LogoutRadio()
{
	Navigate(EWT_RADIO,"http://localhost/uer345_24_ea_v123.htm");
	g_OtherData.SetRadioOpen(false);
}

void CWidgetManager::LoginPaiMai()
{
	string URL = g_pStreamMgr->GetWebsite("SnMallGuid");
	if(URL.empty())
		URL = "http://snmall.sdo.com/SnMall.Web/guid.aspx";

	string url = StringUtil::format("%s?id=%u",URL.c_str(),GetTickCount());

	SetShow(EWT_FIRST,false);
	Navigate(EWT_FIRST,url.c_str());
}

void CWidgetManager::LoginPayServer()
{
	string URL = g_pStreamMgr->GetWebsite("PTPayGuid");
	if(URL.empty())
	{
		URL = "http://ingame.pay.sdo.com/GameDeposit/guid.aspx";
	}
	string url = StringUtil::format("%s?id=%u",URL.c_str(),GetTickCount());
	SetShow(EWT_FIRST,false);
	Navigate(EWT_FIRST,url.c_str());
}

void CWidgetManager::LoginRealStore()
{
	string URL = g_pStreamMgr->GetWebsite("RSGuild");
	if(URL.empty())
		URL = "http://gameshop.sdo.com/guid.aspx";

	string url = StringUtil::format("%s?id=%u",URL.c_str(),GetTickCount());

	SetShow(EWT_FIRST,false);
	Navigate(EWT_FIRST,url.c_str());
}

void CWidgetManager::LoginJiFengStore()
{
	string URL = g_pStreamMgr->GetWebsite("JiFenGuid");
	if(URL.empty())
		URL = "http://gameshop.sdo.com/guid.aspx";

	string url = StringUtil::format("%s?id=%u",URL.c_str(),GetTickCount());
	SetShow(EWT_FIRST,false);
	Navigate(EWT_FIRST,url.c_str());
}

void CWidgetManager::LoginFengHongStore()
{
	string URL = g_pStreamMgr->GetWebsite("FengHongGuid");
	if(URL.empty())
		URL = "http://61.172.251.32:8081/ForSSO/Guid.aspx";

	string url = StringUtil::format("%s?id=%u",URL.c_str(),GetTickCount());

	SetShow(EWT_FIRST,false);
	Navigate(EWT_FIRST,url.c_str());
}

//void CWidgetManager::OpenNpcIEUrl(const char* pUrl)
//{
//	if (pUrl == NULL) return;
//	
//	Navigate(EWT_WEBNPC,pUrl);
//}

void CWidgetManager::LoginRegister()
{
	string URL = "";

// 	FILE *fp = fopen("../SDO178/SDOSpreaderIdentity.dat","rb");
// 	if (fp)
// 	{
// 		char szUrl[2048] = {0};
// 		long length = 0;
// 		fseek( fp, 0, SEEK_END );
// 		length = ftell( fp );
// 		fseek( fp, 0, SEEK_SET );
// 
// 		fread(szUrl,length,1,fp);
// 
// 		CDES *pObj = CreateDES();
// 		string strKey = "456#$%iu";//密钥
// 
// 		char pDecryptResult[2048] = {0};
// 		Decrypt(pObj,szUrl,length,strKey.c_str(),strKey.length(),pDecryptResult);//解密
// 		DestroyDES(pObj);
// 
// 		URL = pDecryptResult;
// 
// 		fclose(fp);
// 	}

	if(URL.empty())
		URL = g_pStreamMgr->GetWebsite("RegisterUrl");

	if(URL.empty())
		URL = StringUtil::format("http://register.sdo.com/PTNew/input_pt.aspx?from=%u&zone=home_embed&cssid=emb&HideIdName=true",WS_GAME_ID);

	SetShow(EWT_INNER_REGISTER,false);
	//Navigate("http://172.30.17.27/regiester/register.htm");
	Navigate(EWT_INNER_REGISTER,URL.c_str());

}

void CWidgetManager::LoginAuthen()
{
	//m_strAuthenURL = "http://localhost/postinfo.html";
	if(m_strAuthenURL.empty())
		return;

	SetShow(EWT_AUTHEN,false);
	Navigate(EWT_AUTHEN,m_strAuthenURL.c_str());
}

void CWidgetManager::LoginTodayActivity()
{
	string URL = g_pStreamMgr->GetWebsite("TodayActivity");
	if(URL.empty())
		URL = "http://act.ws.sdo.com/project/201112_07/ad.asp";

	SetShow(EWT_TODAYACTIVITY,false);
	Navigate(EWT_TODAYACTIVITY,URL.c_str());

}

void CWidgetManager::LoginVipPrivilege()
{
	string URL = g_pStreamMgr->GetWebsite("VipPrivilege");
	if(URL.empty())
		URL = "http://act.ws.sdo.com/project/201112_06/index.html";

	SetShow(EWT_WATCH_PRIVILEGE,false);
	Navigate(EWT_WATCH_PRIVILEGE,URL.c_str());
}

void CWidgetManager::LoginKfzRange()
{
	string URL = g_pStreamMgr->GetWebsite("KfzRange");
	if(URL.empty())
		URL = "http://home.woool.sdo.com/project/20081008_kfz/03.asp";

	SetShow(EWT_KFZ_IE,false);
	Navigate(EWT_KFZ_IE,URL.c_str());
}

void CWidgetManager::LoginXLRange()
{
	string URL = g_pStreamMgr->GetWebsite("XLRange");
	if(URL.empty())
		URL = "http://home.woool.sdo.com/project/200903_zy/index.asp";

	SetShow(EWT_KFZ_IE,false);
	Navigate(EWT_KFZ_IE,URL.c_str());
}

void CWidgetManager::LoginHotActivity()
{
	int iLevel = SELF.GetLevel();
	char level[32] = "";

	if(iLevel <= 20)
	{
		sprintf(level,"l%02d",iLevel);
	}
	else if(iLevel >= 21 && iLevel != 30 && iLevel != 50)
	{
		strcpy(level,"l21");
	}
	else if(iLevel == 30)
	{
		strcpy(level,"l30");
	}
	else if(iLevel == 50)
	{
		strcpy(level,"l50");
	}

	char url[128] = "";
	sprintf(url,"http://act.ws.sdo.com/project/201112_gameintro/%s.html",level);

	SetShow(EWT_HOTACTIVITY,false);
	Navigate(EWT_HOTACTIVITY,url);
}

void CWidgetManager::LoginKfzVSMap()
{
	string URL = g_pStreamMgr->GetWebsite("KfzVSMap");
	if(URL.empty())
		URL = "http://home.woool.sdo.com/project/20081008_kfz/02.asp";

	SetShow(EWT_KFZ_IE,false);
	Navigate(EWT_KFZ_IE,URL.c_str());
}

void CWidgetManager::LoginCheckMask(const char* pUrl)
{
	if (pUrl == NULL)
		return;

	SetShow(EWT_LOGIN_CHECK,false);
	Navigate(EWT_LOGIN_CHECK,pUrl);
}

void CWidgetManager::LoginGuildGreatEvent(int iArea,int iGroup,string strGuildName)
{
	string URL = g_pStreamMgr->GetWebsite("GuildEvent");
	if(URL.empty())
		URL = "http://home.woool.sdo.com/project/200905_1.978iframe/guildevent.asp";

	strGuildName = StringUtil::UrlEncoding(strGuildName);

	char temp[512] = "";
	sprintf(temp,"?area=%d&group=%d&guildname=%s&gameid=%u",iArea,iGroup,strGuildName.c_str(),WS_GAME_ID);
	URL += temp;

	SetShow(EWT_GREAT_EVENT,false);
	Navigate(EWT_GREAT_EVENT,URL.c_str());
}

void CWidgetManager::LoginTopRange()
{
	string URL = g_pStreamMgr->GetWebsite("KfzTopRange");
	if(URL.empty())
		URL = "http://home.woool.sdo.com/project/20081008_kfz/04.asp";

	SetShow(EWT_KFZ_IE,false);
	Navigate(EWT_KFZ_IE,URL.c_str());
}

void CWidgetManager::LoginNoticeIE(const string& url)
{	
	if(url.empty())
		return;

	SetShow(EWT_NOTICE,false);
	Navigate(EWT_NOTICE,url.c_str());
}

void CWidgetManager::LoginCaiHong(int iArea, int iGroup, const std::string& strChannelName)
{
	char temp[512] = {0};
	sprintf(temp,"http://chhelp.sdrs.sdo.com:8083/sdchweb/Default.asp?co=1&tp=6&ut=1&id=&gd=11&gm=188&ar=%d&sv=%d&rl=1&ranid=&ck=&v=0&s=chbzgame&platid=%s"
		, iArea, iGroup, strChannelName.c_str());

	SetShow(EWT_CAIHONGHELP,false);	
	Navigate(EWT_CAIHONGHELP,temp);
}

bool CWidgetManager::Init()
{
	//不要一开始初始化,用到的时候再new,不然浪费,而有的时候会卡一段时间
	//for (int t = EWT_FIRST;t < EWT_NUM;t++)
	//{
	//	if (m_pWidgetContent[t] == NULL)
	//	{
	//		if(m_pWindowProxy[t] == NULL)
	//			m_pWindowProxy[t] = new CWidgetProxy();

	//		m_pWidgetContent[t] = (CHtmlContent*)CreateWidgetContent((CHostWindowProxy *)m_pWindowProxy[t],CWCF_EXTENDCONTROL);

	//		if(m_pWidgetContent[t])
	//		{
	//			m_pWidgetContent[t]->SetBounds(0,0,0,0);
	//		}
	//		if(m_pWindowProxy[t])
	//		{
	//			m_pWindowProxy[t]->m_pWidgetContent = m_pWidgetContent[t]; //设置
	//		}
	//	}
	//}

	return true;
}

void CWidgetManager::NewWidgetContent(E_WEB_TYPE t)
{
	if(t < 0 || t >= EWT_NUM)
		return;

	/*
	if (m_pWidgetContent[t] == NULL)
	{
		if(m_pWindowProxy[t] == NULL)
			m_pWindowProxy[t] = new CWidgetProxy();

		m_pWidgetContent[t] = (CHtmlContent*)CreateWidgetContent((CHostWindowProxy *)m_pWindowProxy[t],CWCF_EXTENDCONTROL);

		if(m_pWidgetContent[t])
		{
			m_pWidgetContent[t]->SetBounds(0,0,0,0);
		}
		if(m_pWindowProxy[t])
		{
			m_pWindowProxy[t]->m_pWidgetContent = m_pWidgetContent[t]; //设置
		}
	}*/
}

void CWidgetManager::MoveWindow(E_WEB_TYPE t,int x,int y,int w,int h)
{
	if(t < 0 || t >= EWT_NUM /*|| m_pWidgetContent[t] == NULL*/)
		return;

	/*
	if(m_pWidgetContent[t] == NULL)
	{
		NewWidgetContent(t);
	}

	if(m_pWidgetContent[t] == NULL)
	{
		return;
	}
	*/

	if(w == 0)
		w = m_rcBound[t].right - m_rcBound[t].left;
	if(w < 0) w = 0;

	if(h == 0)
		h = m_rcBound[t].bottom - m_rcBound[t].top;

	if(h < 0) h = 0;

	m_rcBound[t].left = x;
	m_rcBound[t].top = y;
	m_rcBound[t].right = x + w;
	m_rcBound[t].bottom = y + h;

	//m_pWidgetContent[t]->SetBounds(x,y,w,h);
}

void CWidgetManager::Navigate(E_WEB_TYPE t,const char* szUrl)
{
	if(t < 0 || t >= EWT_NUM /*|| m_pWidgetContent[t] == NULL*/)
		return;

	/*
	if (m_pWidgetContent[t] == NULL)
	{
		NewWidgetContent(t);
	}

	if(m_pWidgetContent[t] == NULL)
	{
		return;
	}*/

	string url = szUrl;
	BSTR URL = string2BSTR(url);
	//m_pWidgetContent[t]->Navigate(URL,0);
	SAFE_DEL_BSTR(URL);
}

BOOL CWidgetManager::OnWindowMessage(DWORD dwMsg,WPARAM wParam,LPARAM lParam,LRESULT& lRet)
{
	bool bInIEWnd = false;

	/*
	for (int t = EWT_FIRST;t < EWT_NUM;t++)
	{
		if(m_pWidgetContent[t] == NULL) 
			continue;

		if(!m_bShow[t] || !m_bFocus[t]) //隐藏了
			continue;

		if((dwMsg >= WM_MOUSEFIRST && dwMsg <= WM_MOUSELAST))
		{
			POINT pt;
			pt.x = lParam & 0xFFFF;
			pt.y = lParam >> 16;

			if (m_rcBound[t].PtInRect(pt.x,pt.y))   
			{
				pt.x -= m_rcBound[t].left;      
				pt.y -= m_rcBound[t].top;       

				if (m_pWidgetContent[t]->QueryHitPoint(&pt) == HTP_CLIENT)  
				{
					LPARAM lpt = MAKELPARAM(pt.x, pt.y);          
					if (m_pWidgetContent[t]->OnWindowMessage(dwMsg, wParam, lpt, lRet))
						return TRUE;
				}

				bInIEWnd = true;
			}
		}
		else if(dwMsg == WM_SETCURSOR)
		{
			if(m_pWidgetContent[t]->OnWindowMessage(dwMsg,wParam,lParam,lRet))
				return TRUE;
		}
	}

	//if(dwMsg >= WM_MOUSEFIRST && dwMsg <= WM_MOUSELAST && !bInIEWnd && ::GetFocus() != g_hWnd) //设置焦点
	//{
	//	bool bInIGW = false;
	//	//if(g_pSDOAInterface)//没测试,先不判,目前是鼠标在圈圈应用上,圈圈那边不会把消息放过来
	//	//{
	//	//	  bInIGW = g_pSDOAInterface->HasUi(lParam & 0xFFFF,lParam >> 16);
	//	//}
	//	if(!bInIGW)
	//		::SetFocus(g_hWnd);
	//}
	*/

	return FALSE;
}

void CWidgetManager::RenderWidget(E_WEB_TYPE t)
{
	if(t < 0 || t >= EWT_NUM)
		return;

	/*
	if(m_pWidgetContent[t] == NULL || !m_bShow[t])
		return;

	if(m_pWidgetContent[t]->Get_Changed())
	{
		m_pWidgetContent[t]->Paint();
		int m_Width, m_Height, m_Pitch;
		PRects m_Rects;
		unsigned int* pwcBufPtr;

		if (FALSE != m_pWidgetContent[t]->Lock(LOCKFLAG_GETBUFFER, (int&)pwcBufPtr, m_Width, m_Height, m_Pitch, m_Rects))
		{
			// 写入像素
			g_pTexMgr->ReleaseTex(m_pWidgetTex[t]); //是否掉原来的纹理

			m_pWidgetTex[t] = g_pTexMgr->LoadMemTex(m_Width,m_Height,(BYTE*)pwcBufPtr,m_Pitch);
			m_pWidgetContent[t]->Unlock();
		}
	}
	*/
	if(m_pWidgetTex[t])
	{
		g_pGfx->DrawTextureNL(m_rcBound[t].left,m_rcBound[t].top,m_pWidgetTex[t]);
	}
}

void CWidgetManager::OnDocComplete(const char * strFileName,const char * strFullUrl,CWidgetProxy *pControl) //显示窗口
{
	if(strnicmp(strFileName,"guid.",5) == 0)
	{
		m_bShow[EWT_FIRST] = false;
	}
	else if(pControl == m_pWindowProxy[EWT_FIRST])
	{
		//上传页面不显示
		int iPage = g_WooolStoreMgr.GetWooolStorePage();
/*		if(iPage == WOOOLSTORE_PAIMAI && g_WooolStoreMgr.GetPaiMaiStoreType() == 4)
			m_bShow[EWT_FIRST] = false;
		else */if(iPage > 0)
			m_bShow[EWT_FIRST] = true;
		else 
			m_bShow[EWT_FIRST] = false;
	}
	//else if(pControl == m_pWindowProxy[EWT_WEBNPC])//IENPC
	//{		
	//	m_bShow[EWT_WEBNPC] = true;
	//}
	else if(pControl == m_pWindowProxy[EWT_RADIO])//内嵌IE电台
	{	
		string strUrl = strFullUrl;

		if(strstr(strFullUrl,"radio"))
		{
			g_OtherData.SetRadioOpen(true);
		}

		if(strstr(strFullUrl,"uer345_24_ea_v123.htm"))
		{
			m_bShow[EWT_RADIO] = false;
		}
		else
		{
			m_bShow[EWT_RADIO] = true;
		}

	}
	else if(pControl == m_pWindowProxy[EWT_KFZ_IE])
	{	
		m_bShow[EWT_KFZ_IE] = true;
	}
	else if(pControl == m_pWindowProxy[EWT_HOTACTIVITY])
	{	
		m_bShow[EWT_HOTACTIVITY] = true;
	}
	else if(pControl == m_pWindowProxy[EWT_GREAT_EVENT])
	{	
		m_bShow[EWT_GREAT_EVENT] = true;
	}
	else if(pControl == m_pWindowProxy[EWT_INNER_REGISTER])
	{	
		m_bShow[EWT_INNER_REGISTER] = true;
	}
	else if(pControl == m_pWindowProxy[EWT_TODAYACTIVITY])
	{	
		m_bShow[EWT_TODAYACTIVITY] = true;
	}
	else if(pControl == m_pWindowProxy[EWT_AUTHEN])
	{
		m_bShow[EWT_AUTHEN] = true;
	}
	else if(pControl == m_pWindowProxy[EWT_NOTICE])
	{
		m_bShow[EWT_NOTICE] = true;
	}
	else if(pControl == m_pWindowProxy[EWT_WATCH_PRIVILEGE])
	{
		m_bShow[EWT_WATCH_PRIVILEGE] = true;
	}
	else if (pControl == m_pWindowProxy[EWT_LOGIN_CHECK])
	{
		m_bShow[EWT_LOGIN_CHECK] = true;
	}
	else if (pControl == m_pWindowProxy[EWT_CAIHONGHELP])
	{
		m_bShow[EWT_CAIHONGHELP] = true;
	}
}

void CWidgetManager::OnTitleChange(const char* str,CWidgetProxy *pControl)
{
	if (str)
	{
		output_debug((char*)str);
	}

	if(pControl == m_pWindowProxy[EWT_AUTHEN])
	{
		if ( (strstr(str, "[0]") != 0) || (strstr(str, "[3000]") != 0) || (strstr(str, "[6000]") != 0))//0成功,3000,6000失败后要logingaet重发token
		{
			g_pGameControl->SENDNeedAuthen_Ack();
		}
	}
}

void CWidgetManager::OnGUID(const char *pGuidStr) //返回GUID
{
	if(NULL == pGuidStr)
		return;

TRY_BEGIN
		g_WooolStoreMgr.SetWooolStoreGUID(pGuidStr);
		g_pGameControl->SEND_WoolStore_Guid(pGuidStr,g_WooolStoreMgr.GetWooolStorePage());
TRY_END
}

void CWidgetManager::OnSNDAMark(DWORD iNum) //您的可用积分
{



}

void CWidgetManager::OnHostApp(DWORD AppID,LPARAM lParam)
{
	if(g_pControl->GetGameState() != MSG_CTRL_GAMEWND)
		return;

	if(lParam == HOSTAPP_CLOSE)
	{
		switch(AppID)
		{
		case APPID_VOICE_CHAT:
			g_pControl->MsgToWnd(MSG_CTRL_PANEL_WND,MSG_CTRL_VOICE_CHAT_BEGIN, lParam);
			break;
		//case APPID_C2C:
		//	if(g_WooolStoreMgr.GetWooolStorePage() == WOOOLSTORE_PAIMAI)
		//	{
		//		g_pControl->PopupWindow(MSG_CTRL_WOOOL_STORE_WND,OPER_CLOSE);
		//	}
		//	break;
		}
	}
	else
	{
		switch(AppID)
		{
		case APPID_VOICE_CHAT:
			g_pControl->MsgToWnd(MSG_CTRL_PANEL_WND,MSG_CTRL_VOICE_CHAT_BEGIN, lParam);
			break;
		case APPID_C2C:
			if(g_PetBoothData.IsPaimaiClosed()) //判断是否关闭了拍卖行
				return;

			g_WidgetMgr.LoginPaiMai();
			g_pControl->PopupWindow(MSG_CTRL_WOOOL_STORE_WND,OPER_CREATE,3);

			break;
		}
	}
}

void CWidgetManager::OnCloseQuery(BOOL& CanClose, CWidgetProxy *pControl)
{
	if (pControl == m_pWindowProxy[EWT_CAIHONGHELP])
	{
		CanClose = FALSE;
		g_pControl->PopupWindow(MSG_CTRL_CAIHONGHELP_WND,OPER_CLOSE);
	}
	else
	{
		CanClose = TRUE;
		PostMessage(g_hWnd, WM_CLOSE, 0, 0);
	}
}

bool CWidgetManager::ExecScript(E_WEB_TYPE t, const char* script)
{
	/*
	if (m_pWidgetContent[t])
	{		
		HRESULT hr = m_pWidgetContent[t]->execScript(str2BSTR(script),str2BSTR("javascript"));
		if (SUCCEEDED(hr))
		{
			return true;
		}		
	}
	*/
	return false;
}
