#pragma once

#pragma warning(disable : 4996)
#pragma warning(disable:4482) 

struct LPDIRECTDRAW_STRUCT;
//class DIRECTDRAW_STRUCT;
namespace OpenLogin
{
#ifdef WIN32
#define SDAPI  __stdcall
#endif

	//IEventHandler为事件回调
	class IEventHandler
	{
		public:
			//具体点击了何种登录方式
			virtual void	SDAPI OnSelectLoginType(TCHAR** loginWay) = 0;
			//当认证服务器完成认证后，将会返回一个AUTHRIZE CODE，该CODE需要通过游戏
			//客户端发送给游戏服务器，再由游戏服务器通过服务器端组件去认证服务器端进行
			//再次认证，并获取游戏账号
			virtual void	SDAPI OnReceiveAuthCode(TCHAR** szAuthCode, TCHAR** loginWay) = 0;
	};

	class IOpenLogin
	{
	public:
		enum WindowType
		{
			OAUTHWINDOW, 
			LOGINPASSWINDOW, 
			MORELOGINWINDOW
		};
	
		//进入登陆界面时创建窗口资源
		virtual bool SDAPI CreateLoginWnd(HWND hWnd, LPDIRECTDRAW_STRUCT* device) =0;
		//进入游戏后释放资源
		virtual void SDAPI DestroyAllWnd() = 0;

		// 移动窗口位置到游戏登陆界面的指定的位置
		virtual int SDAPI MoveWindow(WindowType type, int nPosX, int nPosY) = 0;
		
		// 设置事件回调响应函数
		virtual bool SDAPI SetEventHandler(IEventHandler* handle) = 0;

        // nCmdShow参数可选:
        //    SW_SHOW 显示窗口
        //    SW_HIDE 隐藏窗口
        virtual void SDAPI ShowSpecifyWindow(WindowType type, int nCmdShow) = 0;

        // 获取窗口大小
        virtual void SDAPI GetSpecifyWindowSize(WindowType type, int &Width, int &Height) = 0;

		// 登录成功完成后，调用该接口完成本地的一些设置
		virtual bool SDAPI SendUserLoginInfo(const char * sndaid, const char * appid) = 0;
    };
    
	IOpenLogin* SDAPI GetOpenLoginModule();
	void SDAPI  ReleaseHandle();
}
