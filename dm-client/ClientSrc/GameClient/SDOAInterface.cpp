#include "SDOAInterface.h"

#include "strsafe.h"
#include "Global/Global.h"
#include "Global/GlobalMsg.h"
#include "WndClass/MainWnd/MainWnd.h"
#include "GameData/LoginData.h"


BOOL CSDOAInterface::m_bShowUserLoginDialog = FALSE;
BOOL CSDOAInterface::m_bHaveClosedWindow = FALSE;

CSDOAInterface::CSDOAInterface(void)
{
	m_hInstIGWDll = 0;
	m_pIGWInitialize = NULL;
	m_pIGWGetModule = NULL;
	m_pIGWTerminal = NULL;
	m_pIGWSupportExW = NULL;
	m_pSDOADx9 = NULL;
	m_pSDOAAppEx = NULL;
	m_pSDOAAppUtils = NULL;
	m_pSDOACommunity = NULL;
	m_IGWConfigure = NULL;
	m_bLogIn = FALSE;
	m_bShowUserLoginDialog = FALSE;
	m_loginResult.strAdditional.clear();
	m_loginResult.strAppendix.clear();
	m_loginResult.strIdentityState.clear();
	m_loginResult.strSessionId.clear();
	m_loginResult.strSndaid.clear();
}

CSDOAInterface::~CSDOAInterface(void)
{
	CloseIGW();
}

BOOL CSDOAInterface::OpenIGW()
{
	BOOL bRet = false;	
	WCHAR strExePath[MAX_PATH] = {0};
	WCHAR strExeName[MAX_PATH] = {0};
	WCHAR* strLastSlash = NULL;
	GetModuleFileNameW( NULL, strExePath, MAX_PATH );
	strExePath[MAX_PATH-1]=0;
	strLastSlash = wcsrchr( strExePath, TEXT('\\') );
	
	if( strLastSlash )
	{	// 得到EXE所在路径
		StringCchCopyW( strExeName, MAX_PATH, &strLastSlash[1] );
		*strLastSlash = 0;
		strLastSlash = wcsrchr( strExeName, TEXT('.') );
		if( strLastSlash )	*strLastSlash = 0;
	}

	WCHAR strGameWidgetDll[MAX_PATH] = {0};
	StringCchPrintfW( strGameWidgetDll, MAX_PATH, L"%s\\GameWidget.dll", strExePath );

	if ( !(GetFileAttributesW(strGameWidgetDll) != 0xFFFFFFFF) )
	{
		StringCchPrintfW( strGameWidgetDll, MAX_PATH, L"%s\\..\\GameWidget.dll", strExePath );
		
		if ( !(GetFileAttributesW(strGameWidgetDll) != 0xFFFFFFFF) )
		{
			StringCchPrintfW( strGameWidgetDll, MAX_PATH, L"%s\\GameWidget\\GameWidget.dll", strExePath );
			
			if (!(GetFileAttributesW(strGameWidgetDll) != 0xFFFFFFFF))
			{
				return FALSE;
			}
		}
	}

	CloseIGW();

	// 找到组件则加载	
	m_hInstIGWDll = LoadLibraryW(strGameWidgetDll);
	if (m_hInstIGWDll)
	{
		m_pIGWInitialize = (LPigwInitialize)GetProcAddress( m_hInstIGWDll, "igwInitialize" );
		m_pIGWGetModule = (LPigwGetModule)GetProcAddress( m_hInstIGWDll, "igwGetModule" );
		m_pIGWTerminal = (LPigwTerminal)GetProcAddress( m_hInstIGWDll, "igwTerminal" );
		m_pIGWSupportExW = (PFNigwSupportExW)GetProcAddress( m_hInstIGWDll, "igwSupportExW" );

		int iArea = g_Login.GetInnerAreaNo();
		if (iArea == 0) 
			iArea = -1;

		int iGroupId = g_Login.GetInnerGroupNo();
		if (iGroupId == 0) 
			iGroupId = -1;

		if ( m_pIGWInitialize && m_pIGWGetModule && m_pIGWTerminal && m_pIGWSupportExW)
		{// 加载成功
			// 注意AppInfo类型所有字段必须填
			WCHAR wszVersion[128] = {0};
			MultiByteToWideChar(CP_ACP,0,g_strVersion,-1,wszVersion,128);
			AppInfo tmpAppInfo = {
				sizeof(AppInfo),		// 结构体大小，方便扩展
				WS_GAME_ID,				// 接入应用ID，从开发者网站中申请
				L"传奇世界",				// 应用名称
				wszVersion,				// 应用客户端当前版本号
				SDOA_RENDERTYPE_D3D9,	// 客户端支持的图形引擎类型，这里同时支持d3d9和d3d8
				1,						// 可同时在一台机器上游戏的最大人数（通常为1，例如:KOF应该是2）
				iArea,					// 游戏区ID，不可用时传入-1
				iGroupId				// 游戏组ID，不可用时传入-1
			};			

			if (m_pIGWInitialize(SDOA_SDK_VERSION, &tmpAppInfo) == SDOA_OK)
			{	// 初始化组件成功

				// 获取组件接口
				m_pIGWGetModule(__uuidof(ISDOADx9),(void**)&m_pSDOADx9);
				m_pIGWGetModule(__uuidof(ISDOAAppEx),(void**)&m_pSDOAAppEx);
				m_pIGWGetModule(__uuidof(ISDOAAppUtils),(void**)&m_pSDOAAppUtils);
				m_pIGWGetModule(__uuidof(ISDOACommunity),(void**)&m_pSDOACommunity);
				// 增加获取配置接口
				m_IGWConfigure = new IGWConfigureInterface();
				m_pIGWSupportExW(igwConfigureInterface, m_IGWConfigure, sizeof(IGWConfigureInterface));

				if ( (!m_pSDOADx9) || (!m_pSDOAAppEx) || (!m_pSDOAAppUtils))
					CloseIGW();
				else bRet = true;
			}
			else // 初始化失败清除环境
				CloseIGW();
		}
		else  // 加载DLL失败清除环境
			CloseIGW();
	}
	return bRet;
}
void CSDOAInterface::CloseIGW()
{
	if (m_pSDOADx9)
	{
		m_pSDOADx9->Release();
		m_pSDOADx9 = NULL;
	}

	if (m_pSDOAAppEx)
	{
		m_pSDOAAppEx->Release();
		m_pSDOAAppEx = NULL;
	}

	if (m_pSDOAAppUtils)
	{
		m_pSDOAAppUtils->Release();
		m_pSDOAAppUtils = NULL;
	}

	if (m_pIGWTerminal)
	{  // 释放DLL前总是先调用igwTerminal
		m_pIGWTerminal();
		m_pIGWTerminal = NULL;
	}

	if (m_IGWConfigure)
	{
		delete m_IGWConfigure;
		m_IGWConfigure = NULL;
	}

	if (m_hInstIGWDll)
	{
		FreeLibrary(m_hInstIGWDll);
		m_hInstIGWDll = 0;
	}
	m_pIGWInitialize = NULL;
}

BOOL CSDOAInterface::IsDeviceAcceptable( IDirect3D9* pD3D9,D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, 
						D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext )
{
	if( FAILED( pD3D9->CheckDeviceFormat( pCaps->AdapterOrdinal, pCaps->DeviceType,
		AdapterFormat, D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING, 
		D3DRTYPE_TEXTURE, BackBufferFormat ) ) )
		return false;

	return true;
}
HRESULT CSDOAInterface::OnCreateDevice( IDirect3DDevice9* pd3dDevice,D3DPRESENT_PARAMETERS d3dpp,const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	if (m_pSDOADx9)
	{	// 初始化IGW内部图形引擎
		m_pSDOADx9->Initialize( pd3dDevice, &d3dpp, FALSE );
	}
	return S_OK;
}
HRESULT CSDOAInterface::OnResetDevice( IDirect3DDevice9* pd3dDevice,D3DPRESENT_PARAMETERS d3dpp,const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext )
{
	if(m_pSDOADx9)
	{	// 通知IGW d3d设备重置成功		
		m_pSDOADx9->OnDeviceReset( &d3dpp);
	}

	return S_OK;
}
void CSDOAInterface::OnFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext )
{
	// Clear the render target and the zbuffer 
	pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 45, 50, 170), 1.0f, 0);

	// Render the scene
	if( SUCCEEDED( pd3dDevice->BeginScene() ) )
	{
		/* 绘制IGW界面 */
		if (m_pSDOADx9)
			m_pSDOADx9->RenderEx();

		pd3dDevice->EndScene();
	}
}

void CSDOAInterface::OnLostDevice( void* pUserContext )
{
	if(m_pSDOADx9)	m_pSDOADx9->OnDeviceLost();
}

void CSDOAInterface::OnDestroyDevice( void* pUserContext )
{
	/* 释放IGW图形引擎 */
	if (m_pSDOADx9)	m_pSDOADx9->Finalize();
}

void CSDOAInterface::Logout()
{
	if(m_pSDOAAppEx)	m_pSDOAAppEx->Logout();
}

BOOL CSDOAInterface::OnLogin(int nErrorCode, const LoginResult* pLoginResult, int nUserData,int nReserved)
{
	static DWORD sdwTimes = 0;
	BOOL bLogIn = (SDOA_ERRORCODE_OK == nErrorCode);	
	if (bLogIn)
	{ 
		m_bShowUserLoginDialog = FALSE;

		if (pLoginResult && g_pSDOAInterface)
		{
			LoginResultEx& loginResultEx	= g_pSDOAInterface->GetLoginResult();
			loginResultEx.strSndaid			= pLoginResult->szSndaid;
			loginResultEx.strSessionId		= pLoginResult->szSessionId;
			loginResultEx.strIdentityState	= pLoginResult->szIdentityState;
			loginResultEx.strAppendix		= pLoginResult->szAppendix;
			loginResultEx.strAdditional		= pLoginResult->szAdditional;
		}

		SendMessage(g_hWnd,SDOA_WM_CLIENT_LOGIN,0,0);
	}

	if (SDOA_ERRORCODE_CANCEL == nErrorCode)
	{
		if(!m_bHaveClosedWindow)
		{
			PostMessage(g_hWnd,WM_DESTROY,0,0);
		}

		return FALSE;
	}

	if (SDOA_ERRORCODE_SHOWMESSAGE == nErrorCode)
	{ // 登录错误消息处理
		if (nReserved != 0)
		{
			LoginMessage* pCurLoginMsg = (LoginMessage*)nReserved;
			if (pCurLoginMsg->dwSize >= sizeof(LoginMessage))
			{ // 兼容性处理 
				// 修改错误提示对话框标题
				sdwTimes++;
				SysReAllocString(pCurLoginMsg->pbstrTitle, L"[游戏登录失败]");
				//if (sdwTimes >= 3)
				//{
				//	//走老的协议
				//	sdwTimes = 0;
				//	g_Login.SetSDOALogin(false);
				//	if(g_pSDOAInterface)
				//		g_pSDOAInterface->SetShowUserLoginDialog(FALSE);

				//	g_pControl->Msg(MSG_CTRL_LOGIN_IN_NOUSE_OA,0);
				//}
			}
		}
	}
	return bLogIn; // 常规逻辑，登录成功则关闭登录对话框
}

void CSDOAInterface::ChangePlayerInfo(UserDetailInfo& playerInfo)
{
	if(m_pSDOAAppEx) m_pSDOAAppEx->ChangeUserInfo(&playerInfo);
}

void CSDOAInterface::SetWindowPos(int iX,int iY)
{
	if(m_pSDOAAppEx)
	{
		SDOAWinProperty WinProperty;
		int iRet = m_pSDOAAppEx->GetWinProperty(L"igwUserLoginDialog",&WinProperty);

		if(iRet == SDOA_OK)
		{
			WinProperty.nLeft = iX;
			WinProperty.nTop = iY;
			m_pSDOAAppEx->SetWinProperty(L"igwUserLoginDialog",&WinProperty);
		}
	}
}

bool CSDOAInterface::GetWindowPos(int& iX,int& iY)
{
	bool bRet = false;

	if(m_pSDOAAppEx)
	{
		SDOAWinProperty WinProperty;
		int iRet = m_pSDOAAppEx->GetWinProperty(L"igwUserLoginDialog",&WinProperty);

		if(iRet == SDOA_OK)
		{
			iX = WinProperty.nLeft;
			iY = WinProperty.nTop;
			bRet = true;
		}
	}

	return bRet;
}

BOOL CSDOAInterface::SetShowUserLoginDialog(BOOL bShow)
{
	if (!m_pSDOAAppEx) return FALSE;

	BOOL bRet = FALSE;
	SDOAWinProperty WinProperty;
	int iRet = m_pSDOAAppEx->GetWinProperty(L"igwUserLoginDialog",&WinProperty);

	if(iRet == SDOA_OK)
	{
		WinProperty.nStatus = bShow?1:2;

		iRet = m_pSDOAAppEx->SetWinProperty(L"igwUserLoginDialog",&WinProperty);
		if (iRet == SDOA_OK)
		{
			m_bShowUserLoginDialog = bShow;
			bRet = TRUE;
		}

		//if (!bShow)
		//{
		//	m_pSDOAAppEx->CloseWindow(L"igwUserLoginDialog");
		//}
	}

	return bRet;
}

void CSDOAInterface::Login(const UserBasicInfo* pUserInfo, int nLoginType)
{
	if (m_pSDOAAppEx)
		m_pSDOAAppEx->Login(pUserInfo,nLoginType);
}

void CSDOAInterface::SetCharacterInfo(const UserCharacterInfo* pCharacterInfo)
{
	if (m_pSDOACommunity)
		m_pSDOACommunity->SetCharacterInfo(pCharacterInfo);
}

void CSDOAInterface::SetScreenStatus(const int nValue)
{
	if (m_pSDOAAppEx)
		m_pSDOAAppEx->SetScreenStatus(nValue);
}

bool CSDOAInterface::SetTaskBarPosition(const PPOINT ptPosition)
{
	if (m_pSDOAAppEx)
		return m_pSDOAAppEx->SetTaskBarPosition( ptPosition);

	return false;
}

void CSDOAInterface::ChangeLocation(const UserLocation* pUserLocation)
{
	if (m_pSDOACommunity)
		m_pSDOACommunity->ChangeLocation(pUserLocation);
}

void CSDOAInterface::OnCommunityEvent(const COMMUNITYEVENTTYPE EventType, LPCSTR pcsTarget)
{
	if (!pcsTarget) return;

	WCHAR wcTemp[2 * 1024] = {0};
	MultiByteToWideChar(CP_ACP,0,pcsTarget,-1,wcTemp,2 * 1024);
	if (m_pSDOACommunity)
		m_pSDOACommunity->OnCommunityEvent(EventType,wcTemp);
}

void CSDOAInterface::OnGuildTitleChanged(const LPCSTR pcsOldTitle, const LPCSTR pcsNewTitle, DWORD nChange)
{
	WCHAR wcOldTile[2 * 1024] = {0};
	WCHAR wcNewTile[2 * 1024] = {0};
	MultiByteToWideChar(CP_ACP,0,pcsOldTitle,-1,wcOldTile,2 * 1024);
	MultiByteToWideChar(CP_ACP,0,pcsNewTitle,-1,wcNewTile,2 * 1024);
	if (m_pSDOACommunity)
		m_pSDOACommunity->OnGuildTitleChanged(wcOldTile,wcNewTile,nChange);
}

void CSDOAInterface::OnLevelUp(int nLevel)
{
	if (m_pSDOACommunity)
		m_pSDOACommunity->OnLevelUp(nLevel);
}

int CSDOAInterface::OpenWidgetEx(LPCSTR pcsWidgetNameSpace, LPCSTR pcsParam, int nFlag)
{
	WCHAR NameSpace[2 * 1024] = {0};
	WCHAR Param[2 * 1024] = {0};
	MultiByteToWideChar(CP_ACP,0,pcsWidgetNameSpace,-1,NameSpace,2 * 1024);
	MultiByteToWideChar(CP_ACP,0,pcsParam,-1,Param,2 * 1024);

	if (m_pSDOAAppEx)
		return m_pSDOAAppEx->OpenWidgetEx(NameSpace,Param,nFlag);

	return 0;
}

int CSDOAInterface::OpenWidget(LPCSTR pcsWidgetNameSpace)
{
	WCHAR NameSpace[2 * 1024] = {0};
	MultiByteToWideChar(CP_ACP,0,pcsWidgetNameSpace,-1,NameSpace,2 * 1024);
	if (m_pSDOAAppEx)
		return m_pSDOAAppEx->OpenWidget(NameSpace);

	return 0;
}

int CSDOAInterface::ExecuteWidget(LPCSTR pcsWidgetNameSpaceOrGUID, LPCSTR pcsParam)
{
	WCHAR pcwWidgetName[2 * 1024] = {0};
	WCHAR pcwParam[2 * 1024] = {0};
	
	MultiByteToWideChar(CP_ACP,0,pcsParam,-1,pcwParam,4 * 1024);
	MultiByteToWideChar(CP_ACP,0,pcsWidgetNameSpaceOrGUID,-1,pcwWidgetName,2 * 1024);

	if (m_pSDOAAppEx)
		return m_pSDOAAppEx->ExecuteWidget(pcwWidgetName,pcwParam);

	return 0;
}

void CSDOAInterface::NodifyAudioChanged()
{
	if (m_pSDOAAppEx)
		m_pSDOAAppEx->NodifyAudioChanged();
}

void CSDOAInterface::SetLogicScreen(const int sw, const int sh)
{
	if (m_IGWConfigure)
	{
		if (m_IGWConfigure->SetLogicScreen)
		{
			m_IGWConfigure->SetLogicScreen(sw, sh);
		}
	}
}
void CSDOAInterface::GetLogicScreen( int* sw, int* sh )
{
	if (m_IGWConfigure)
	{
		if (m_IGWConfigure->GetLogicScreen)
		{
			m_IGWConfigure->GetLogicScreen(sw, sh);
		}
	}
}

bool CSDOAInterface::HasUi(int x,int y)
{
	if(m_pSDOAAppEx)
	{
		POINT pt;
		pt.x = x;
		pt.y = y;
		return m_pSDOAAppEx->HasUI(pt);
	}

	return false;
}
