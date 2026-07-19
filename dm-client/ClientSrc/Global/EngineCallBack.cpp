#include "EngineCallBack.h"
#include "Global.h"
#include "GameMap\GameMap.h"
#include "GameClient/SDOAInterface.h"
#include "GameControl/GameControl.h"
#include "GameClient/ReplayManager.h"
#include "GameClient/sdrsieclient.h"
#include "BaseClass/Compress/Compr.h"

#define  MAX_FILE_LENGTH        6*1024*1024//解压后单个文件的最大长度


CEngineCallBack::CEngineCallBack(void)
{
	m_pUnCompressBuffer = new char[MAX_FILE_LENGTH];

}

CEngineCallBack::~CEngineCallBack(void)
{
	delete[] m_pUnCompressBuffer;
}

//获得游戏窗口句柄
HWND CEngineCallBack::GetWndHandle()
{
	return g_hWnd;
}
//获得父窗口句柄
HWND CEngineCallBack::GetParentHandle()
{
	return g_hParentWnd;
}

//获得游戏主目录
const char * CEngineCallBack::GetDataDir()
{
	return GetGameDataDir();
}
//获得全局公共buf
char * CEngineCallBack::GetBuf()
{
	return g_cBuf;
}
//写错误日志
void CEngineCallBack::CallBackSaveException(char *file,char *fun,int line,const CSeException* pException,const char *msg)
{
	SaveException(file,fun,line,pException,msg);
}
//强制结束海浪生命周期
void CEngineCallBack::SeaTideLifeOver(int iX,int iY)
{
	if (!g_pGameMap)
	{
		return;
	}
	//修改地图数据上的对应字节段，让该处的海浪发生器处于活跃状态
	SMapTile * pTile = g_pGameMap->GetTile(iX,iY);
	if(pTile)
		pTile->uObject &= ~0x00200000;
}
//获得登录类型
int CEngineCallBack::GetParamGameType()
{
	return g_iParamGameType;
}
//IGW 绘制
void CEngineCallBack::IgwDraw()
{
	if (g_pSDOAInterface && g_pSDOAInterface->GetSDOADx9())
	{
		g_pSDOAInterface->GetSDOADx9()->RenderEx();
	}
}

//IGW Lost Device
void CEngineCallBack::OnLostDevice()
{
	if (g_pSDOAInterface && g_pSDOAInterface->GetSDOADx9())
	{
		g_pSDOAInterface->GetSDOADx9()->OnDeviceLost();
	}

	//StopRainBowSpirt();
}
//IGW DestroyDevice
void CEngineCallBack::OnKillD3D()
{
	if (g_pSDOAInterface && g_pSDOAInterface->GetSDOADx9())
	{
		g_pSDOAInterface->GetSDOADx9()->Finalize();
	}

	//StopRainBowSpirt();
}

//IGW DeviceReset
void CEngineCallBack::OnDeviceReset(void * pParms)
{
	if (g_pSDOAInterface && g_pSDOAInterface->GetSDOADx9() && pParms)
	{
		g_pSDOAInterface->GetSDOADx9()->OnDeviceReset((D3DPRESENT_PARAMETERS*)pParms);
	}

	//StopRainBowSpirt();
}

//OnReset
void CEngineCallBack::OnDeviceNotReset()
{
	if(g_pSDOAInterface)
	{
		if (g_pSDOAInterface->GetSDOADx9())
			g_pSDOAInterface->GetSDOADx9()->Initialize(g_pGfx->GetD3DDev(),g_pGfx->GetD3DPP(),FALSE);

		g_pGameControl->SetIGWInfo();
	}

	//StopRainBowSpirt();
}

//获得客户端的版本号
const char *CEngineCallBack::GetVersion()
{
	return g_strVersion;
}
//获得客户端的Build号
DWORD CEngineCallBack::GetBuildID()
{
	return g_dwBuildID;
}
//是否在录像
bool CEngineCallBack::IsInRecord()
{
#ifdef ENABLE_REPLAY
	return g_ReplayManager.IsInRecord();
#else
	return false;
#endif
}
//是否在播放录像
bool CEngineCallBack::IsInReplay()
{
#ifdef ENABLE_REPLAY
	return g_ReplayManager.IsInReplay();
#else
	return false;
#endif
}
//获得录像文件保存目录
const char * CEngineCallBack::GetReplayDir()
{
	return g_strReplayDir;
}

bool CEngineCallBack::IsMultiClientInSameDir()
{
	return g_bIsMultiClientInSameDir;
}

void CEngineCallBack::SendDownloadFile(const char *buf,int iLen)
{
	if (g_pDownLoadNet)
	{
		g_pDownLoadNet->SendBuf((char *)buf,iLen);
	}
}

void CEngineCallBack::OnDownloadedDataFile(int iType,DWORD dwID,__int64 iHash,const char *strFileName,const char* buf,DWORD dwLen,bool bCompressed)
{
	//通知更新地图
	if (strstr(strFileName,"map\\"))
	{
		const char* pMapDataBuf = buf;
		DWORD dwMapDataLen = dwLen;
		//如果压缩了先解压,是分块压缩的
		if (bCompressed)
		{
			DWORD dwLeftBytes = dwLen,dwBlockBytes = 0,UnCompressByte = MAX_FILE_LENGTH,dwAllUnCompressedBytes = 0;
			while(dwLeftBytes > 0)
			{
				dwBlockBytes = *(DWORD*)(buf + (dwLen - dwLeftBytes));//分块压缩,这四个字节表示后面一段压缩数据的长度
				if (dwBlockBytes <= 0)
				{
					return;
				}

				dwLeftBytes -= 4;//分块压缩,这四个字节表示后面一段压缩数据的长度

				UnCompressByte = MAX_FILE_LENGTH - dwAllUnCompressedBytes;
				Uncompress((BYTE *)m_pUnCompressBuffer + dwAllUnCompressedBytes,&UnCompressByte,(BYTE*)(buf + (dwLen - dwLeftBytes)),dwBlockBytes);
				dwAllUnCompressedBytes += UnCompressByte;

				dwLeftBytes -= dwBlockBytes;
			}

			pMapDataBuf = m_pUnCompressBuffer;
			dwMapDataLen = dwAllUnCompressedBytes;
		}

		g_pGameMap->LockDownloadMap();

		bool flag = false;
		char szPath[MAX_PATH] = {0};
		sprintf(szPath,"map\\%s-3d.nmp",g_pGameMap->GetRealDownloadMapFileName().c_str());
		if (stricmp(strFileName, szPath) == 0)
		{
			flag = true;
		}

		if (!flag)
		{
			sprintf(szPath,"map\\%s.nmp",g_pGameMap->GetRealDownloadMapFileName().c_str());
			if (stricmp(strFileName, szPath) == 0)
			{
				flag = true;
			}
		}

		if (flag)
		{
			char* lbuff = new char[dwMapDataLen];
			memcpy(lbuff, pMapDataBuf, dwMapDataLen);

			g_pGameMap->SafeDeleteDownloadMapDataBuf();
			g_pGameMap->SetDownloadMapDataBuf(lbuff);
			g_pGameMap->SetDownloadMapDataLen(dwMapDataLen);
		}

		if (stricmp(strFileName, g_pGameMap->GetRealDownloadBgMapFileName().c_str()) == 0)
		{
			char* lbuff = new char[dwMapDataLen];
			memcpy(lbuff, pMapDataBuf, dwMapDataLen);

			g_pGameMap->SafeDeleteDownloadBgMapDataBuf();
			g_pGameMap->SetDownloadBgMapDataBuf(lbuff);
			g_pGameMap->SetDownloadBgMapDataLen(dwMapDataLen);
		}

		g_pGameMap->UnLockDownloadMap();
		

		//string szMapName = g_pGameMap->GetMapName();//不可以直接传入g_pGameMap->GetMapName(),LoadMap的时候会清空这个值
		//if (strstr(strFileName,"_bg.nmp"))//背景地图
		//{
		//	if (strstr(strFileName,g_pGameMap->GetRealMapName()))//看看是不是当前地图的背景地图
		//	{
		//		g_pGameMap->LoadBgMap(pMapDataBuf,dwMapDataLen);
		//	}
		//}
		//else
		//{
		//	g_pGameMap->LoadMap(szMapName.c_str(),true,pMapDataBuf,dwMapDataLen);
		//}
	}
}

