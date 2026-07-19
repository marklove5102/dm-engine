#pragma once

#include "Interface/CallBackInterface.h"

class CEngineCallBack : public CCallBackInterface
{
public:
	CEngineCallBack(void);
public:
	~CEngineCallBack(void);

	//获得游戏窗口句柄
	virtual HWND GetWndHandle();
	//获得游戏主目录
	virtual const char * GetDataDir();	
	//获得全局公共buf
	virtual char * GetBuf();
	//写错误日志
	virtual void CallBackSaveException(char *file,char *fun,int line,const CSeException* pException,const char *msg);
	//强制结束海浪生命周期
	virtual void SeaTideLifeOver(int iX,int iY);
	//获得登录类型
	virtual int GetParamGameType();
	//IGW 绘制
	virtual void IgwDraw();
	//OnLostDevice
	virtual void OnLostDevice();
	//OnKillD3D
	virtual void OnKillD3D();
	//DeviceReset
	virtual void OnDeviceReset(void * pParms);
	//OnReset
	virtual void OnDeviceNotReset();
	//获得客户端的版本号
	virtual const char *GetVersion();
	//获得客户端的Build号
	virtual DWORD GetBuildID();
	//是否在录像
	virtual bool IsInRecord();
	//是否在播放录像
	virtual bool IsInReplay();
	//获得录像文件保存目录
	virtual const char * GetReplayDir();	
	//是否同目录下的多开客户端
	virtual bool IsMultiClientInSameDir();
	//OnDownloadedDataFile
	virtual void OnDownloadedDataFile(int iType,DWORD dwID,__int64 iHash,const char *strFileName,const char* buf,DWORD dwLen,bool bCompressed);
	//请求下载文件
	virtual void SendDownloadFile(const char *buf,int iLen);

	virtual HWND GetParentHandle();
protected:
	char    *m_pUnCompressBuffer;//用来解压的buf


};
