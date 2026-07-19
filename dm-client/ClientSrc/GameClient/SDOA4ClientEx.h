#ifndef SDOA4CLIENTEX_H
#define SDOA4CLIENTEX_H


#include <objbase.h>
#include "windows.h"
#include <string>
#pragma pack(push,1)

// 当前 SDOA 接口版本
#define SDOA_SDK_VERSION                 0x0200

// SDOA 返回值
#define SDOA_OK                      0
#define SDOA_FALSE                   1

// 内存不足
#define SDOA_INSUFFICIENT_BUFFER     122  

// SDOA开始工作后发出的消息
#define SDOA_WM_CLIENT_RUN			0xA604  // WM_USER + 0xA204
// IGW发送给游戏客户端的通知消息
#define SDOA_WM_CLIENT_NOTIFY		0xA610  // WM_USER + 0xA210
#define SDOA_WM_CLIENT_LOGIN		0xBFFF
// wParam: 通知事件ID; lParam: 扩展信息，无内容为0
#define SDOA_NOTIFY_SHOWLOGIN     1    // 呼出登录对话框
#define SDOA_NOTIFY_LOGINCLICK    2    // 点击了“登录”按钮
// 游戏客户端发送给IGW的通知消息
#define SDOA_WM_HOST_NOTIFY			0xA611	// WM_USER + 0xA211

struct GameBasicInfo {
	DWORD		cbSize;				// = sizeof(GameBasicInfo)
	int			GameID;				// 盛大统一游戏ID
	LPCWSTR		GameName;			// 游戏名称
	LPCWSTR		GameVer;			// 游戏版本
	int			GameRenderType;		// 游戏类型              (取值SDOA_RENDERTYPE_UNKNOWN..SDOA_RENDERTYPE_OPENGL)
	int			MaxUser;			// 游戏同时支持用户数目，通常为1
	LPCWSTR		GameArea;			// 游戏所在区名称        (尽可能要有)
	LPCWSTR		GameServer;			// 游戏所在服务器名称    (尽可能要有)
	int			GameAreaID;			// 游戏区ID              (注意：不可用时传-1)
	int			GameServerID;		// 游戏区服务器ID	       (注意：不可用时传-1)
	LPCWSTR     SPID;				// 盛大统一SPID，全付通需要传入(SPID如是数字需转成字符串)
	int         LoginMode;			// 登录模式(全付通取值为1，默认为0)
};

struct AppInfo {
	DWORD		cbSize;				// = sizeof(AppInfo)
	int			nAppID;				// 盛大统一游戏ID
	LPCWSTR		pwcsAppName;		// 游戏名称
	LPCWSTR		pwcsAppVer;		   	// 游戏版本
	int			nRenderType;		// 游戏图形引擎类型 (取值SDOA_RENDERTYPE_UNKNOWN..SDOA_RENDERTYPE_OPENGL)
	int			nMaxUser;			// 游戏可同时在一台机器上游戏的最大人数（通常为1，例如:KOF应该是2）
	int			nAreaId;			// 游戏区ID	(注意：不可用时传-1)
	int			nGroupId;           // 游戏组ID	(注意：不可用时传-1)
};

// 游戏图形引擎类型
#define SDOA_RENDERTYPE_UNKNOWN             0x00
#define SDOA_RENDERTYPE_GDI                 0x01
#define SDOA_RENDERTYPE_DDRAW               0x04
#define SDOA_RENDERTYPE_D3D7                0x08
#define SDOA_RENDERTYPE_D3D8                0x10
#define SDOA_RENDERTYPE_D3D9                0x20
#define SDOA_RENDERTYPE_D3D10               0x40
#define SDOA_RENDERTYPE_OPENGL              0x80

// 登录类型
#define LOGINTYPE_PTACCOUNT         0    // 盛大通行证 + 密码
#define LOGINTYPE_PTNUMID           1    // 数字ID + 密码
#define LOGINTYPE_PTACCOUNTWITHEKEY 2    // 盛大通行证 + 密码 + 密宝
#define LOGINTYPE_PTNUMIDWITHEKEY   3    // 数字ID + 密码 + 密宝
#define LOGINTYPE_SSOACCESSKEY      4    // SSO AccessKey

// 登录模式
#define LOGINMODE_DEFAULT           0		// 盛大内部游戏接入使用
#define LOGINMODE_QFT_UID           1		// 全付通游戏接入，返回SDID（通常为外部游戏）
#define LOGINMODE_QFT_NUMID         2		// 全付通游戏接入，返回数字ID（通常为18游戏）

// 外部控制Screen状态 
#define SDOA_SCREENNONE				0		// Screen没有被创建
#define SDOA_SCREENNORMAL			1		// 正常显示状态
#define SDOA_SCREENMINI				2		// 最小化状态

const
// 内部IWidgetWindows状态控制 
#define SDOA_WINDOWSNONE			0		// 窗口没有被创建
#define SDOA_WINDOWSSHOW			1		// 窗口显示状态
#define SDOA_WINDOWSHIDE			2		// 窗口隐藏
// 窗口类型
#define SDOA_WINDOWSTYPE_NONE		0       // 未知类型
#define SDOA_WINDOWSTYPE_TOOLBAR	1		// 工具栏
#define SDOA_WINDOWSTYPE_WIDGET		2		// Widget窗口
#define SDOA_WINDOWSTYPE_WINDOWS	3		// 普通窗口

// 打开Widget的选项
#define OPENWIDGET_DEFAULT			0		// 默认
#define OPENWIDGET_NOBARICON		1		// 仅打开Widget，不自动添加到侧边栏上

// 接口SetSelfCursor用到的常量
#define SDOA_CURSOR_AUTO		0	// 自动检测是否自绘鼠标指针
#define SDOA_CURSOR_SELF		1	// 始终自绘制鼠标指针
#define SDOA_CURSOR_HIDESELF	2	// 始终不自绘制鼠标指针

// 登录方法
#define SDOA_LOGINMETHOD_SSO        0   // 登录方法：SSO
#define SDOA_LOGINMETHOD_SDO        1   // 登录方法：SDO(CAS)

// 登录状态
#define SDOA_LOGINSTATE_UNKNOWN     -1  // 未知，参数不正确或获取状态异常
#define SDOA_LOGINSTATE_NONE        0   // 还没有调用过登录方法
#define SDOA_LOGINSTATE_OK          1   // 登录成功
#define SDOA_LOGINSTATE_CONNECTING  2   // 正在登录
#define SDOA_LOGINSTATE_FAILED      3   // 登录失败
#define SDOA_LOGINSTATE_LOGOUT      4   // 已登出

// 圈圈工具条展开模式
#define SDOA_BARMODE_ICONBOTTOM		0   // 工具条在圈圈图标下面
#define SDOA_BARMODE_ICONTOP		1   // 工具条在圈圈图标上面

struct  UserBasicInfo{
	DWORD			cbSize;                 // sizeof(UserBasicInfo)
	LPCWSTR			PTAccount;              // 用户账号
	LARGE_INTEGER	PTNumID;				// 用户数字ID
	BYTE			Password[64];           // 用户密码
	LPCWSTR			EKey;                   // 密宝
	BYTE			SSOAccessKey[128];      // SSO AccessKey
};

struct UserDetailInfo{
	DWORD	cbSize;                    // sizeof(UserDetailInfo)
	LPCWSTR	pwcsNickName;              // 昵称
	int     nGender;                   // 性别(0: 女; 1: 男)
	LPCWSTR pwcsGameArea;              // 游戏所在区
	LPCWSTR pwcsGameServer;            // 游戏所在服务器
	int     nGameAreaID;               // 盛大统一游戏区号ID（默认-1）
	int     nGameServerID;             // 盛大统一游戏服务器ID（默认-1）
	// more
};

struct SDOAWinProperty{
	int nWinType;				// 窗口类型(只用于标识窗口类型，设置无效)
	int nStatus;				// 窗口状态
	// 以下是位置、宽高信息
	int nLeft;
	int nTop;
	int nWidth;
	int nHeight;	
};

struct LoginResult {
	DWORD  cbSize;			// sizeof(LoginResult)，为以后扩充提供可能
	LPCSTR szSessionId;		// 用于后台验证的token串
	LPCSTR szSndaid;		// 用户ID
	LPCSTR szIdentityState;	// 是否成年标识，0未成年，1成年
	LPCSTR szAppendix;		// 附加字段，保留
	// 2009.09.02新增
	LPCSTR szAdditional;    // 附加信息，需要调用LoginDirect时传回
};

struct LoginResultEx {
	std::string  strSessionId;		// 用于后台验证的token串
	std::string  strSndaid;			// 用户ID
	std::string  strIdentityState;	// 是否成年标识，0未成年，1成年
	std::string  strAppendix;		// 附加字段，保留
	// 2009.09.02新增
	std::string strAdditional;     // 附加信息，需要调用LoginDirect时传回
};

// 订单支付信息
struct BillingInfo {
	DWORD   dwSize;        // sizeof(BillingInfo)
	LPCSTR  sdorderno;     // 支付订单号 (长度: 16)
	int     amount;        // 支付金额
	LPCSTR  merid;         // 商户代码   (长度: 4)
	LPCSTR  bizcode;       // 业务代码，表示支付方式 (长度: 4)
	LPCSTR  orderdate;     // 订单日期   (长度: 8)
	LPCSTR  callbackaddr;  // 回调地址   (长度: 128)
	LPCSTR  callbacktype;  // 回调地址类别，包括URL、Web Service、Socket (长度: 2)
	LPCSTR  productid;     // 产品名称   (长度: 32)
	int     productnum;    // 产品数量
	LPCSTR  uid;           // 用户ID     (长度: 32)
	LPCSTR  serviceid;     // 应用编号   (长度: 3)
	LPCSTR  ex1;           // 扩展参数
	LPCSTR  ex2;           // 扩展参数
	LPCSTR  signmsg;       // 签名串     (长度: 128)
};

struct LoginMessage {
	DWORD  dwSize;
	int    nErrorCode;   // 登录组件返回的错误码,仅供参考，详细内容请用pbstrContent
	BSTR*  pbstrTitle;	// 提示标题，可以调用SysReallocString修改其中的内容，注意，回调结果后IGW将调用SysFreeString释放已分配的内存
	BSTR*  pbstrContent;	// 提示内容，可以调用SysReallocString修改其中的内容，注意，回调结果后IGW将调用SysFreeString释放已分配的内存
};

// LPLOGINCALLBACKPROC中回调参数ErrorCode值定义
#define SDOA_ERRORCODE_OK               0    // 登录成功
#define SDOA_ERRORCODE_CANCEL           -1   // 用户按取消关闭了登录对话框
#define SDOA_ERRORCODE_UILOST           -2   // 加载对话框UI文件缺失或无效
#define SDOA_ERRORCODE_FAILED           -3   // 登录失败
#define SDOA_ERRORCODE_UNKNOWN          -4   // 未知错误
#define SDOA_ERRORCODE_INVALIDPARAM     -5   // 参数无效
#define SDOA_ERRORCODE_UNEXCEPT         -6   // 未知异常
#define SDOA_ERRORCODE_ALREAYEXISTS     -7   // 已经存在
#define SDOA_ERRORCODE_SHOWMESSAGE      -10  // 显示对示对话框


// 登录回调函数
// nErrorCode:     登录返回错误代码
// pLoginResult:   登录返回结果，只在 nErrorCode = SDOA_ERRORCODE_OK 时才有效
// nUserData:      在调用ShowLoginDialog时传入的用户数据
// nReserved:      保留，暂不使用，目前总是为0(不等于ShowLoginDialog中传入的Reserved参数)
// 返回值:        为TRUE将关闭对话框，为FALSE不关闭
//
// 2009.03.30
// 扩展功能，当nErrorCode = ERRORCODE_SHOWMESSAGE时，Reserved返回LoginMessage*结果
// 返回TRUE时，IGW将不再弹出对话框，游戏可以自行弹出提示对话框等
// 返回FALSE时，IGW继续弹出对话框，可以调用SysReallocString修改Title,Content其中的内容，以便游戏显示自定义的错误提示
//              注意：回调结果后IGW将调用SysFreeString释放已分配的内存
typedef BOOL (CALLBACK *LPLOGINCALLBACKPROC)(int nErrorCode, const LoginResult* pLoginResult, int nUserData, int nReserved);

// 游戏实现的声音控制回调
// 注意音量值的有效范围在0-100
typedef DWORD (CALLBACK *LPGETAUDIOSOUNDVOLUME)();
typedef void  (CALLBACK *LPSETAUDIOSOUNDVOLUME)(DWORD nNewVolume);
typedef DWORD (CALLBACK *LPGETAUDIOEFFECTVOLUME)();
typedef void  (CALLBACK *LPSETAUDIOEFFECTVOLUME)(DWORD nNewVolume);


// 应用接口
typedef interface ISDOAAppEx ISDOAAppEx;
typedef ISDOAAppEx *PSDOAAppEx, *LPSDOAAppEx;

MIDL_INTERFACE("D6D9F26A-AF22-4EB1-9900-177FA8D9780C")
ISDOAAppEx : public IUnknown
{
public:
	STDMETHOD_(bool,InitGameInfo)(THIS_ const GameBasicInfo* pGameBasicInfo) PURE;
	STDMETHOD_(void,Login)(THIS_ const UserBasicInfo* pUserInfo, int nLoginType) PURE;
	STDMETHOD_(void,Logout)(THIS) PURE;
	STDMETHOD_(int,GetLoginState)(THIS_ const int nLoginMethod) PURE;
	STDMETHOD_(void,ChangeUserInfo)(THIS_ const UserDetailInfo* pUserDetailInfo) PURE;
	

	// 显示登录对话框
	STDMETHOD_(int,ShowLoginDialog)(THIS_ const LPLOGINCALLBACKPROC fnCallback, int nUserData, int nReserved) PURE;
	// 显示充值对话框 (目前不使用)
	STDMETHOD_(int,ShowPaymentDialog)(THIS_ int nReserved1, int nReserved2) PURE;
	// 显示支付对话框（支付并显示结果）
	STDMETHOD_(int,ShowBillingDialog)(THIS_ const BillingInfo* pBillingInfo, int nReserved) PURE;

	// 配置SDOA相关
	STDMETHOD_(int,GetScreenStatus)(THIS) PURE;
	STDMETHOD_(void,SetScreenStatus)(THIS_ const int nValue) PURE;
	STDMETHOD_(bool,GetScreenEnabled)(THIS) PURE;
	STDMETHOD_(void,SetScreenEnabled)(THIS_ const bool bValue) PURE;
	STDMETHOD_(bool,GetTaskBarPosition)(THIS_ PPOINT ptPosition) PURE;
	STDMETHOD_(bool,SetTaskBarPosition)(THIS_ const PPOINT ptPosition) PURE;
	STDMETHOD_(bool,GetFocus)(THIS) PURE;
	STDMETHOD_(void,SetFocus)(THIS_ const bool bValue) PURE;
	STDMETHOD_(bool,HasUI)(THIS_ POINT ptPosition) PURE;
	
	STDMETHOD_(int,GetTaskBarMode)(THIS) PURE;
	STDMETHOD_(void,SetTaskBarMode)(THIS_ const int nBarMode) PURE;
	STDMETHOD_(int,GetSelfCursor)(THIS) PURE;
	STDMETHOD_(int,SetSelfCursor)(THIS_ const int nValue) PURE;

	// 声音控制
	// 设置声音控制相关回调
	STDMETHOD_(void,SetAudioSetting)(THIS_ 
		LPGETAUDIOSOUNDVOLUME  fnGetAudioSoundVolume,
		LPSETAUDIOSOUNDVOLUME  fnSetAudioSoundVolume,
		LPGETAUDIOEFFECTVOLUME fnGetAudioEffectVolume,
		LPSETAUDIOEFFECTVOLUME fnSetAudioEffectVolume ) PURE;
	// 通知SDOA游戏声音有变化                          
	STDMETHOD_(void,NodifyAudioChanged)(THIS) PURE;	

	// 功能相关
	STDMETHOD_(int,OpenWidget)(THIS_ LPCWSTR pwcsWidgetNameSpace) PURE;
	STDMETHOD_(int,OpenWidgetEx)(THIS_ LPCWSTR pwcsWidgetNameSpace, LPCWSTR pwcsParam, int nFlag) PURE;
	STDMETHOD_(int,CloseWidget)(THIS_ LPCWSTR pwcsWidgetNameSpace) PURE;
	STDMETHOD_(int,ExecuteWidget)(THIS_ LPCWSTR pwcsWidgetNameSpaceOrGUID, LPCWSTR pwcsParam) PURE;
	STDMETHOD_(int,WidgetExists)(THIS_ LPCWSTR pwcsWidgetNameSpaceOrGUID) PURE;
	STDMETHOD_(int,OpenWindow)(THIS_ LPCWSTR pwcsWinType, LPCWSTR pwcsWinName, LPCWSTR pwcsSrc, int nLeft, int nTop, int nWidth, int nHeight, LPCWSTR pwcsMode) PURE;
	STDMETHOD_(int,CloseWindow)(THIS_ LPCWSTR pwcsWinName) PURE;

	STDMETHOD_(int,GetWinProperty)(THIS_ LPCWSTR pwcsWinName, SDOAWinProperty* pWinProperty) PURE;
	STDMETHOD_(int,SetWinProperty)(THIS_ LPCWSTR pwcsWinName, SDOAWinProperty* pWinProperty) PURE;
};

// 社群接口相关数据类型

/*! 社群事件类型常量，参考IGWInterface.h中的IGWUserCommunityInterfaceA定义 */
enum COMMUNITYEVENTTYPE {
	CE_SET_GUILD_TITLE,		/*!< 在行会中的头衔，参数Target是头衔名称 */
	CE_CREATE_GUILD,		/*!< 创建行会，参数Target是行会名称 */
	CE_JOIN_GUILD,			/*!< 加入行会，参数Target是行会名称 */
	CE_QUIT_GUILD,			/*!< 退出行会，参数Target是行会名称 */
	CE_ADD_FRIEND,			/*!< 添加好友，参数Target是好友昵称 */
	CE_REMOVE_FRIEND,		/*!< 删除好友，参数Target是好友昵称 */
	CE_MARRIED,				/*!< 结婚，参数Target是对象的昵称 */
	CE_DIVORCED,			/*!< 离婚，参数Target是对象的昵称 */
	CE_JOIN_MASTER,			/*!< 拜师，参数Target是师门名称或者师傅名称 */
	CE_LEAVE_MASTER			/*!< 脱离师徒关系，参数Target是师门名称或者师傅名称 */
};

struct UserCharacterInfo{
	LPCWSTR     pwcsChracterName;        // 角色名称
	DWORD       nGender;              // 性别(0: 女; 1: 男)
	LPCWSTR     pwcsOccupation;          // 角色职业
	int         nLevel;               // 角色等级
	LPCWSTR     pwcsGuildName;           // 所在行会
	LPCWSTR     pwcsGuildTitle;          // 在行会中的头衔
};

// 用户位置信息(由 Path + Room 组成，可以类比为定位文件的 Path + File)
// 注：Path/Room是由游戏自定义的内部数据，游戏自己理解即可
//     PathTitle/RoomTitle是显示给用户看的标题
struct UserLocation {
	LPCWSTR pwcsPath;                      // 路径唯一标识(如果有多级，用"/"做分隔符)
	LPCWSTR pwcsPathTitle;                 // 路径标题(如果有多级，用"/"做分隔符)
	LPCWSTR pwcsRoom;                      // 房间唯一标识
	LPCWSTR pwcsRoomTitle;                 // 房间标题
};

typedef void (WINAPI* LPCHANGELOCATION)(const UserLocation* Location);

// 通用数据传输接口
class ICommonChannel
{
public:
	// 下列三个方法是为兼容Delphi的接口而设置，在C++中不需要实现，但需要保留在此位置
	virtual HRESULT _stdcall QueryInterface(REFIID riid, void **ppvObject) = 0;
	virtual ULONG _stdcall AddRef(void) = 0;
	virtual ULONG _stdcall Release(void) = 0;

	virtual void _stdcall SendData(BSTR Request, BSTR* Response) = 0;
	virtual void _stdcall Set_RevertChannel(ICommonChannel* pReverter) = 0;
	virtual ICommonChannel* _stdcall Get_RevertChannel(void) = 0;
	virtual void _stdcall Close(void) = 0;
	virtual void _stdcall Get_ChannelType(LPSTR szChannelType, DWORD nBufferLen) = 0; // max BufferLen = 64
};
typedef ICommonChannel *PCommonChannel;

typedef BOOL (WINAPI *LPCREATECHANNEL)(LPCSTR szChannelType, ICommonChannel** pChannel);

// 社群接口
typedef interface ISDOACommunity ISDOACommunity;
typedef ISDOACommunity *PSDOACommunity, *LPSDOACommunity;

MIDL_INTERFACE("32F3321E-D20C-47AB-B1C1-AC2313245B7C")
ISDOACommunity : public IUnknown
{
public:
	// 设置角色信息
	STDMETHOD_(void,SetCharacterInfo)(THIS_ const UserCharacterInfo* pCharacterInfo) PURE;	
	// 社群相关的通用回调
	STDMETHOD_(void,OnCommunityEvent)(THIS_ const COMMUNITYEVENTTYPE EventType, LPCWSTR pwcsTarget) PURE;
	// 角色升级回调 
	STDMETHOD_(void,OnLevelUp)(THIS_ int nLevel) PURE;	
	// 角色在行会中的职位改变回调
	STDMETHOD_(void,OnGuildTitleChanged)(THIS_ const LPCWSTR pwcsOldTitle, const LPCWSTR pwcsNewTitle, DWORD nChange) PURE;
	// 设置进入场所(房间)回调
	STDMETHOD_(void,SetEnterLocationCallback)(THIS_ const LPCHANGELOCATION fnChangeLocation) PURE;	

	// 设置创建数据通道的回调
	STDMETHOD_(void,SetCreateChannelCallback)(THIS_ const LPCREATECHANNEL fnCreateChannel) PURE;	
	STDMETHOD_(void,ChangeLocation)(THIS_ const UserLocation* pUserLocation) PURE;		
};

///==========================================
/// 注意ISDOAAppUtils中的功能
/// 与ISDOAAppEx::SetAudioSetting、ISDOAAppEx::NodifyAudioChanged、ISDOACommunity::SetCreateChannelCallback
/// 功能重叠这里为兼容新版本代码保留
///==========================================

typedef interface ISDOAAppUtils ISDOAAppUtils;
typedef ISDOAAppUtils *PSDOAAppUtils, *LPSDOAAppUtils;

MIDL_INTERFACE("1170C2F9-28AD-4EA8-8392-E9A219C8FF65")
ISDOAAppUtils : public IUnknown
{
public:
	// 声音控制
	// 设置声音控制相关回调
	STDMETHOD_(void,SetAudioSetting)(THIS_ 
		LPGETAUDIOSOUNDVOLUME  fnGetAudioSoundVolume,
		LPSETAUDIOSOUNDVOLUME  fnSetAudioSoundVolume,
		LPGETAUDIOEFFECTVOLUME fnGetAudioEffectVolume,
		LPSETAUDIOEFFECTVOLUME fnSetAudioEffectVolume ) PURE;
	// 通知SDOA游戏声音有变化                          
	STDMETHOD_(void,NodifyAudioChanged)(THIS) PURE;	
	// 设置创建数据通道的回调
	STDMETHOD_(void,SetCreateChannelCallback)(THIS_ const LPCREATECHANNEL fnCreateChannel) PURE;	
};

// 异步回调
typedef void (CALLBACK *LPQUERYCALLBACK)(int nRetCode, int nUserData);

typedef interface ISDOAClientService ISDOAClientService;
typedef ISDOAClientService *PSDOAClientService, *LPSDOAClientService;

MIDL_INTERFACE("AF56D291-823A-41AA-85A0-EBE5C6163425")
ISDOAClientService : public IUnknown
{
public:
	STDMETHOD_(int, SetValue)(THIS_ const BSTR bstrKey, const BSTR bstrValue ) PURE;
	STDMETHOD_(int, Query)(THIS_ const BSTR bstrService) PURE;	
	STDMETHOD_(int, AsyncQuery)(THIS_ const BSTR bstrService, LPQUERYCALLBACK fnCallback, int nUserData) PURE;	
	STDMETHOD_(int, GetValue)(THIS_ const BSTR bstrKey, BSTR* bstrValue) PURE;	
};

//typedef int (WINAPI* LPigwInitExW)(DWORD dwVersion, const GameBasicInfo* pGameBasicInfo);

/* 注意igwInitialize中dwSdkVersion参数使用宏 SDOA_SDK_VERSION */
typedef int  (WINAPI* LPigwInitialize)(DWORD dwSdkVersion, const AppInfo* pAppInfo);
typedef bool (WINAPI* LPigwGetModule)(REFIID riid, void** intf);
typedef int  (WINAPI* LPigwTerminal)();


#pragma pack(pop)
#endif /* SDOA4CLIENTEX_H */