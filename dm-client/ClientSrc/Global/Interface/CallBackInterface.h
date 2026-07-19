#pragma once

#include "../Globaldefine.h"
#include "../DebugTry.h"

class CSeException;

class CCallBackInterface
{
public:
	virtual ~CCallBackInterface(){}
	//获得游戏窗口句柄
	virtual HWND GetWndHandle() = 0;
    //获得游戏主目录
	virtual const char * GetDataDir() = 0;	
    //获得全局公共buf
	virtual char * GetBuf() = 0;
	//写错误日志
	virtual void CallBackSaveException(char *file,char *fun,int line,const CSeException* pException,const char *msg) = 0;
	//强制结束海浪生命周期
	virtual void SeaTideLifeOver(int iX,int iY) = 0;
	//获得登录类型
	virtual int GetParamGameType() = 0;
	//IGW 绘制
	virtual void IgwDraw() = 0;
	//OnLostDevice
	virtual void OnLostDevice() = 0;
	//OnKillD3D
	virtual void OnKillD3D() = 0;
	//DeviceReset
	virtual void OnDeviceReset(void * pParms) = 0;
	//OnReset
	virtual void OnDeviceNotReset() = 0;
	//获得客户端的版本号
	virtual const char *GetVersion() = 0;
	//获得客户端的Build号
	virtual DWORD GetBuildID() = 0;
	//是否在录像
	virtual bool IsInRecord() = 0;
	//是否在播放录像
	virtual bool IsInReplay() = 0;
    //获得录像文件保存目录
	virtual const char * GetReplayDir() = 0;	
	//是否同目录下的多开客户端
	virtual bool IsMultiClientInSameDir() = 0;
	//OnDownloadedDataFile
	virtual void OnDownloadedDataFile(int iType,DWORD dwID,__int64 iHash,const char *strFileName,const char* buf,DWORD dwLen,bool bCompressed) = 0;
	//请求下载文件
	virtual void SendDownloadFile(const char *buf,int iLen) = 0;
	//获得父窗口
	virtual HWND GetParentHandle() = 0;
};


extern CCallBackInterface * g_pCallBack;
