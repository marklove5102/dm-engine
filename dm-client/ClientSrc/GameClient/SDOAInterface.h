#pragma once

#include "SDOA4ClientEx.h"
#include "SDOADx9.h"
#include "SDOA4ClientPatch.h"

class CSDOAInterface
{
public:
	CSDOAInterface(void);
	~CSDOAInterface(void);
public:
	BOOL OpenIGW();
	void CloseIGW();
	BOOL IsDeviceAcceptable( IDirect3D9* pD3D9,D3DCAPS9* pCaps, D3DFORMAT AdapterFormat, 
		D3DFORMAT BackBufferFormat, bool bWindowed, void* pUserContext );
	HRESULT OnCreateDevice( IDirect3DDevice9* pd3dDevice,D3DPRESENT_PARAMETERS d3dpp,const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
	HRESULT OnResetDevice( IDirect3DDevice9* pd3dDevice,D3DPRESENT_PARAMETERS d3dpp,const D3DSURFACE_DESC* pBackBufferSurfaceDesc, void* pUserContext );
	void OnFrameRender( IDirect3DDevice9* pd3dDevice, double fTime, float fElapsedTime, void* pUserContext );
	static BOOL CALLBACK OnLogin(int nErrorCode, const LoginResult* pLoginResult, int nUserData,int nReserved);
	void OnLostDevice( void* pUserContext );
	void OnDestroyDevice( void* pUserContext );
	void Logout();
	ISDOAAppEx* GetSDOAApp(){ return m_pSDOAAppEx;}
	ISDOADx9* GetSDOADx9(){ return m_pSDOADx9;}
	ISDOAAppUtils* GetSDOAAppUtils(){return m_pSDOAAppUtils;}
	LoginResultEx& GetLoginResult(){return m_loginResult;}
	BOOL IsShowUserLoginDialog(){return m_bShowUserLoginDialog;}
	BOOL SetShowUserLoginDialog(BOOL bShow);
	void Login(const UserBasicInfo* pUserInfo, int nLoginType);
	void ChangePlayerInfo(UserDetailInfo& playerInfo);
	void SetCharacterInfo(const UserCharacterInfo* pCharacterInfo);
	void SetScreenStatus(const int nValue);
	bool SetTaskBarPosition(const PPOINT ptPosition);
	void ChangeLocation(const UserLocation* pUserLocation);
	
	void OnCommunityEvent(const COMMUNITYEVENTTYPE EventType, LPCSTR pcsTarget);
	void OnGuildTitleChanged(const LPCSTR pcsOldTitle, const LPCSTR pcsNewTitle, DWORD nChange);
	void OnLevelUp(int nLevel);
	int OpenWidgetEx(LPCSTR pcsWidgetNameSpace, LPCSTR pcsParam, int nFlag);
	int OpenWidget(LPCSTR pcsWidgetNameSpace);
	int ExecuteWidget(LPCSTR pcsWidgetNameSpaceOrGUID, LPCSTR pcsParam);
	void NodifyAudioChanged();

	void SetWindowPos(int iX,int iY);
	bool GetWindowPos(int& iX,int& iY);
	void SetLogicScreen(const int sw, const int sh);
	void GetLogicScreen( int* sw, int* sh );
	BOOL GetHaveClosedWindow() const { return m_bHaveClosedWindow; }
	void SetHaveClosedWindow(BOOL val) { m_bHaveClosedWindow = val; }

	bool HasUi(int x,int y);

private:
	HINSTANCE			m_hInstIGWDll;
	LPigwInitialize		m_pIGWInitialize;
	LPigwGetModule		m_pIGWGetModule;
	LPigwTerminal		m_pIGWTerminal;
	PFNigwSupportExW	m_pIGWSupportExW;
	ISDOADx9*			m_pSDOADx9;
	ISDOAAppEx*			m_pSDOAAppEx;
	ISDOAAppUtils*		m_pSDOAAppUtils;
	ISDOACommunity*		m_pSDOACommunity;
	// 补丁接口增加让外部能设置 圈圈 操作界面逻辑屏幕大小
	LPIGWConfigureInterface	m_IGWConfigure;		// 配置接口
	BOOL				m_bLogIn;
	LoginResultEx		m_loginResult;
	static BOOL			m_bShowUserLoginDialog;
	static BOOL         m_bHaveClosedWindow;//是否已经关闭了窗口,如果开着oa登录界面关闭窗口,会收到OnLogin
};

extern CSDOAInterface* g_pSDOAInterface;
