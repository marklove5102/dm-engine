///////////////////////////////////////////////////////////////////////
//文件名：   .cpp
//版权：上海盛大网络有限公司版权所有
//作者：
//创建日期：
//版本：
//文件说明：
///////////////////////////////////////////////////////////////////////
//Net.cpp

#include "Net.h"
#include "Global/Timer.h"
#include "GameClient/ReplayManager.h"
#include "Global/Interface/GraphicInterface.h"

#ifdef _DEBUG
#include "global/new.h"
#define new DEBUG_NEW
#endif

#include "Proxy.h"
#include "Global/DebugTry.h"

CNet::CNet(int iMaxServer,HWND hWnd,DWORD dwMsg,long lEvent)
{
	Init(iMaxServer,hWnd,dwMsg,lEvent);
}

void CNet::Init(int iMaxServer,HWND hWnd,DWORD dwMsg,long lEvent)
{
	WSADATA  wsaData;
	WORD m_wVersion	= MAKEWORD(2,2);					//版本设定
	if(WSAStartup( m_wVersion, &wsaData ) != 0)			//初始化Winsock失败
	{
		// 错误处理
		fatal_error("Socket 初始化失败！");
		return;
	}

	for(int i =0; i < iMaxServer; i++)
	{
		SOCKET_INFORMATION *temp = new SOCKET_INFORMATION();
		temp->iSendBuffer = 0;
		m_buf.push_back(temp);
	}

	m_hWnd = hWnd;
	m_dwMsg= dwMsg;
	m_lEvent = lEvent;
	m_nMaxServer	= iMaxServer;

	m_nTimer		= 0;

	memset(&m_stSockAddr,0,sizeof(sockaddr_in));

	m_dwSendBytes		= 0;
	m_dwRecvBytes		= 0;
	m_dwTotalSendBytes	= 0;
	m_dwTotalRecvBytes	= 0;

	m_dwDynType			= 0;
	m_pDynCode			= 0;
	m_lpDynCode_De = NULL;
	m_lpDynCode_En = NULL;
	m_dwDynCodeLen = 0;
}

//析构
CNet::~CNet(void)
{
	if(m_pDynCode)
	{
		VirtualFree(m_pDynCode,0,MEM_RELEASE);
	}

	for(int i = 0; i < m_nMaxServer;i++)
	{
		Close(i);
	}

	for (int i = 0; i < m_buf.size(); i++)
	{
		SAFE_DELETE(m_buf[i]);
	}
	m_buf.clear();
	WSACleanup();
}

void CNet::SetServer(int iServer,LPCTSTR strServerName,int nPort)
{
	if(iServer < 0 || iServer >= (int)m_buf.size())
		return;

	m_buf[iServer]->strServerName.assign(strServerName);
	m_buf[iServer]->iPort = nPort;
}

//连接服务器
bool CNet::Connect(int iServer,BOOL bUseProxy)
{
#ifdef ENABLE_REPLAY
	if(g_ReplayManager.IsInReplay())
	{
		return 0;
	}
#endif


	ZeroMemory(&m_stSockAddr,sizeof(m_stSockAddr));
	m_stSockAddr.sin_family = AF_INET;

	if(bUseProxy && m_strProxyName.length() > 0 && m_nProxyPort > 0)
	{
		m_stSockAddr.sin_addr.s_addr = inet_addr(m_strProxyName.c_str());
		m_stSockAddr.sin_port = htons(m_nProxyPort);
		m_buf[iServer]->bProxyUsed = true;
	}
	else
	{
		m_stSockAddr.sin_addr.s_addr = inet_addr(m_buf[iServer]->strServerName.c_str());
		m_stSockAddr.sin_port = htons( m_buf[iServer]->iPort);
		m_buf[iServer]->bProxyUsed = false;
	}

	m_buf[iServer]->dwLastConnectTime = GetTickCount();

    if((m_buf[iServer]->Socket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP )) == INVALID_SOCKET )  // 创建socket
		return false;

	AsyncSelect(m_buf[iServer]->Socket);

	// 取消发送等待 
	BOOL b = TRUE;
	setsockopt(m_buf[iServer]->Socket,IPPROTO_TCP,TCP_NODELAY,(const char *)(&b),sizeof(b));

	//连接服务端
	if(connect( m_buf[iServer]->Socket, (SOCKADDR*)&m_stSockAddr, sizeof( m_stSockAddr ) ) == SOCKET_ERROR)
	{
		int i = WSAGetLastError();
		return false;
	}


	return true;
}

//异步通信设置
int CNet::AsyncSelect(SOCKET s)
{
	if( WSAAsyncSelect( s, m_hWnd, m_dwMsg, m_lEvent ) == SOCKET_ERROR )
	{
		//出错处理
		return 0;
	}
	return 1;
}


//发送信息
int CNet::Send( int iServer,char * buf,DWORD len )
{
	output_debug("SendToServer %s  Len:%d\n", buf, len);

#ifdef ENABLE_REPLAY
	if(g_ReplayManager.IsInReplay())
	{
		return true;
	}
#endif

	WSABUF	DataBuf;
	DataBuf.buf		= buf;
	DataBuf.len		= len;
	DWORD SendBytes = 0;
	DWORD total		= 0;

	SOCKET_INFORMATION * SocketInfo = m_buf[iServer];

	while(total < len)
	{
		if(WSASend(SocketInfo->Socket, &DataBuf,1,&SendBytes, 0, NULL, NULL) == SOCKET_ERROR)
		{
			int iErr = WSAGetLastError();
			output_debug("WSASend Error:%d\r\n",iErr);
			if( iErr != WSAEWOULDBLOCK )
			{
				if( (SocketInfo->iSendBuffer + len) < DATA_BUFSIZE )
				{
					memcpy(&(SocketInfo->SendBuffer[SocketInfo->iSendBuffer]),buf,len);
					SocketInfo->iSendBuffer += len;
				}
			}
			else
			{
				if(len < DATA_BUFSIZE)
				{
					memcpy(SocketInfo->SendBuffer,buf,len);
					SocketInfo->iSendBuffer = len;
				}
			}

			return -1;
		} 
		else // No error so update the byte count
		{
			total += SendBytes;
		}
	}

	m_dwSendBytes		+= total;
	m_dwTotalSendBytes	+= total;

	return total;
}

void CNet::Close(int iServer)
{
#ifdef ENABLE_REPLAY
	if(g_ReplayManager.IsInReplay())
	{
		return;
	}
#endif

	m_buf[iServer]->bConnect = false;
	if(m_buf[iServer]->Socket == NULL)
		return;

	shutdown(m_buf[iServer]->Socket,SD_BOTH);
	closesocket(m_buf[iServer]->Socket);
	m_buf[iServer]->Socket = 0;
}

// 消息处理，在窗口的消息处理函数中加入这个函数
void CNet::OnCommand(DWORD wParam,DWORD lParam)
{
#ifdef ENABLE_REPLAY
	if(g_ReplayManager.IsInReplay())
	{
		return;
	}
#endif

	if(IsValidSocket((SOCKET)wParam) == false)
		return;
	WORD w = WSAGETSELECTERROR(lParam);
	if(w && w != WSAECONNABORTED )
	{
		OnError(lParam);
	}

	switch(WSAGETSELECTEVENT(lParam))
	{
		case FD_CONNECT:
			OnConnect(wParam,lParam);
			break;
		case FD_READ:
			OnRead(wParam,lParam);
			break;
		case FD_WRITE:
			OnWrite(wParam,lParam);
			break;
		case FD_CLOSE:
			OnClose(wParam,lParam);
			break;
		default:
			break;
	}
}

//封包读取
void CNet::OnRead(DWORD wParam,DWORD lParam)
{
#ifdef ENABLE_REPLAY
	if(g_ReplayManager.IsInReplay())
	{
		return;
	}
#endif

	DWORD RecvBytes = 0;
	DWORD Flags = 0;

	int i = GetSocketInfo(wParam);
	if(i == -1)
		return;

	SOCKET_INFORMATION * SocketInfo = m_buf[i];
	SocketInfo->DataBuf.buf = SocketInfo->Buffer;
	SocketInfo->DataBuf.len = DATA_BUFSIZE;

	Flags = 0; 

	//接收的为结构形式, 改为字符串
	if(WSARecv(SocketInfo->Socket, &(SocketInfo->DataBuf), 1, &RecvBytes, &Flags, NULL, NULL) == SOCKET_ERROR)
	{
		int iError = WSAGetLastError();
		if(iError != WSAEWOULDBLOCK)
		{
			//Save Net Error Info
		}
	} 
	else
	{
		if(RecvBytes == 0)
		{
			OnClose(0,0);
		}

		m_dwRecvBytes		+= RecvBytes;
		m_dwTotalRecvBytes	+= RecvBytes;

		//处理代理服务器的第一个消息
		if(SocketInfo->bProxyUsed && SocketInfo->bWaitReply)
		{
			SocketInfo->BytesRECV = RecvBytes;
			ProcessProxyReply(i);
			return;
		}

		SocketInfo->BytesRECV = RecvBytes;

#ifdef ENABLE_REPLAY
		if (g_ReplayManager.IsInRecord() && g_pGfx)
		{
			g_ReplayManager.SaveMsg(g_pGfx->GetFrameCount(),(WORD)(SocketInfo->BytesRECV),(const char*)SocketInfo->Buffer);
		}
#endif

		ProcessBuffer(SocketInfo);
	}
}

void CNet::OnReadForReplay(const char * strMsg,int iLen)
{
	if (m_buf.empty())
	{
		SOCKET_INFORMATION *temp = new SOCKET_INFORMATION();
		temp->iSendBuffer = 0;
		m_buf.push_back(temp);
	}

	SOCKET_INFORMATION * SocketInfo = m_buf[0];
	memcpy(SocketInfo->Buffer, strMsg, iLen);
	SocketInfo->BytesRECV = iLen;
	ProcessBuffer(SocketInfo);
}

BOOL CNet::SetDynEnDe(const char *buf,UINT len)
{
	DWORD dwType;
	memcpy(&dwType,buf,4);

	if(dwType >= DED_Types)
		return FALSE;

	if(dwType == DED_Old)
	{
		SetDynEnDe(DED_Old,0,NULL);
	}
	else
	{
		UINT iLen;
		memcpy(&iLen,buf + 8,4);

		if(iLen < MAX_DATA_CRYBUFFER && iLen == (len - 12))
		{
			BOOL b = SetDynEnDe(dwType,iLen,(buf + 12));
			if(!b)
			{
				//DetailOutput("Dyndec error!");
				return FALSE;
			}
		}
		else
		{
			//DetailOutput("Dyndec error!");
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CNet::SetDynEnDe(DWORD dwType,DWORD len,const char *buf)
{
	if(dwType >= DED_Types)
		return FALSE;

	switch(dwType)
	{
	case DED_Old:
		{
			break;
		}
	case DED_New:
	case DED_New_Old:
		{
			if(buf && len < MAX_DATA_CRYBUFFER && len > 0)
			{
				
				if(!m_pDynCode)
				{
					m_pDynCode = VirtualAlloc(NULL,MAX_DATA_CRYBUFFER,MEM_COMMIT,PAGE_EXECUTE_READWRITE);
				}

				if(!m_pDynCode)
					return FALSE;

				memset(m_pDynCode,0,MAX_DATA_CRYBUFFER);
				memcpy(m_pDynCode,buf,len);

				m_iDynLen = len;

				LPGETDYNCODE lpGetDynCode = NULL;
				TRY_BEGIN
					lpGetDynCode = (LPGETDYNCODE)m_pDynCode;
					__asm mov ebx,GetProcAddress
					__asm mov ecx,GetModuleHandle
					__asm mov edx,LoadLibrary
					m_lpDynCode_En = lpGetDynCode(1);
					m_lpDynCode_De = lpGetDynCode(2);
				TRY_END_DO(VirtualFree(m_pDynCode,0,MEM_RELEASE);)

				if(!m_lpDynCode_En || !m_lpDynCode_De)
					return FALSE;
			}
			break;
		}
	default:
		{
			return FALSE;
		}
	}

	m_dwDynType = dwType;
	return TRUE;
}

DWORD CNet::GetDynType()
{
	return m_dwDynType;
}

void CNet::SetProxy(LPCTSTR strProxyName,int nPort)
{
	m_strProxyName.assign(strProxyName);
	m_nProxyPort =  nPort;
}


void CNet::LoginProxyReq(int i)
{
	char  szRequest[sizeof(SClientRequest) + sizeof(SProxyPkgHead)];
	SClientRequest*  pstRequest = (SClientRequest*)(szRequest + sizeof(SProxyPkgHead));

	pstRequest->byVersion = PROXY_CLIENT_PROTOCOL_VERSION;
	pstRequest->dwDstAddr = inet_addr(m_buf[i]->strServerName.c_str());
	pstRequest->wDstPort  = htons(m_buf[i]->iPort);
	pstRequest->byAuthMethod = AUTH_METHOD_NONE;

	int nLen = BuildProxyPkgHead((SProxyPkgHead*)szRequest, sizeof(SClientRequest));
	this->Send(i,szRequest,nLen);
}

void CNet::ProcessProxyReply(int i)
{
	SOCKET_INFORMATION* SocketInfo = m_buf[i];

	if(SocketInfo->BytesRECV < sizeof(SProxyPkgHead)+ sizeof(SClientReply)) //数据不对
	{
		this->Close(i); //关闭连接，重新直接连接
		this->Connect(i,FALSE);

		SocketInfo->RecBuf.clear();
		SocketInfo->bWaitReply = false;
		return;
	}

	SProxyPkgHead* pstHeader = (SProxyPkgHead*)(SocketInfo->Buffer);
	DWORD dwLen = CheckProxyPkgHead(pstHeader);
	if(dwLen != sizeof(SProxyPkgHead)+ sizeof(SClientReply)) //数据头非法
	{
		this->Close(i); //关闭连接，重新直接连接
		this->Connect(i,FALSE);

		SocketInfo->RecBuf.clear();
		SocketInfo->bWaitReply = false;
		return;
	}

	SClientReply* pstReply = (SClientReply*)(SocketInfo->Buffer + sizeof(SProxyPkgHead));

	//代理服务器失败
	BYTE byCurVersion = PROXY_CLIENT_PROTOCOL_VERSION;
	if(pstReply->byVersion != byCurVersion || pstReply->byStatus != PROXY_CLIENT_REP_SUCCEED)
	{
		this->Close(i); //关闭连接，重新直接连接
		this->Connect(i,FALSE);

		if(pstReply->byVersion != byCurVersion)
		{
			output_debug("代理服务器版本不对 - Server 0x%2x,Client 0x%2x\n",pstReply->byVersion,byCurVersion);
		}
		else
		{
			OutputProxyError(pstReply->byStatus);
		}
		SocketInfo->RecBuf.clear();
		SocketInfo->bWaitReply = false;
		return;
	}

	//正确的数据，成功了！
	SocketInfo->RecBuf.clear();
	SocketInfo->bWaitReply = false;

	//发送缓冲的数据
	if( SocketInfo->iSendBuffer )
	{
		this->Send(i,SocketInfo->SendBuffer,SocketInfo->iSendBuffer);
		SocketInfo->iSendBuffer = 0;
	}
}

void CNet::OnConnect(DWORD wParam,DWORD lParam)
{
}

int CNet::GetSocketInfo(SOCKET s)
{
	for(unsigned int i = 0; i < m_buf.size();i++)
	{
		if(m_buf[i]->Socket == s)
			return i;
	}
	return -1; // 
}

void CNet::OnError(DWORD wParam)
{
}

void CNet::OnClose(DWORD wParam,DWORD lParam)
{
}

void CNet::OnWrite(DWORD wParam,DWORD lParam)
{
}


//传世
int WINAPI fnEncode6BitBuf(unsigned char *pszSrc, char *pszDest, int nSrcLen, int nDestLen)
{
	int				nDestPos	= 0;
	int				nRestCount	= 0;
	unsigned char	chMade = 0, chRest = 0;

	//长度修正
	if(nSrcLen==0){
		nSrcLen = strlen((char*)pszSrc);
	}

	for (int i = 0; i < nSrcLen; i++)
	{
		if (nDestPos >= nDestLen) break;

		chMade = ((chRest | (pszSrc[i] >> (2 + nRestCount))) & 0x3f);
		chRest = (((pszSrc[i] << (8 - (2 + nRestCount))) >> 2) & 0x3f);

		nRestCount += 2;

		if (nRestCount < 6)
			pszDest[nDestPos++] = chMade + 0x3c;
		else
		{
			if (nDestPos < nDestLen - 1)
			{
				pszDest[nDestPos++]	= chMade + 0x3c;
				pszDest[nDestPos++]	= chRest + 0x3c;
			}
			else
				pszDest[nDestPos++] = chMade + 0x3c;

			nRestCount	= 0;
			chRest		= 0;
		}
	}

	if (nRestCount > 0)
		pszDest[nDestPos++] = chRest + 0x3c;

	pszDest[nDestPos] = '\0';

	return nDestPos;
}

int  WINAPI fnDecode6BitBuf(char *pszSrc, char *pszDest, int nDestLen)
{
	int				nLen = strlen((const char *)pszSrc);
	int				nDestPos = 0, nBitPos = 2;
	int				nMadeBit = 0;
	unsigned char	ch, chCode, tmp;

	for (int i = 0; i < nLen; i++)
	{
		if ((pszSrc[i] - 0x3c) >= 0)
			ch = pszSrc[i] - 0x3c;
		else
		{
			nDestPos = 0;
			break;
		}

		if (nDestPos >= nDestLen) break;

		if ((nMadeBit + 6) >= 8)
		{
			chCode = (tmp | ((ch & 0x3f) >> (6 - nBitPos)));
			pszDest[nDestPos++] = chCode;

			nMadeBit = 0;

			if (nBitPos < 6) 
				nBitPos += 2;
			else
			{
				nBitPos = 2;
				continue;
			}
		}

		tmp = ((ch << nBitPos) & Decode6BitMask[nBitPos - 2]);

		nMadeBit += (8 - nBitPos);
	}

	//	pszDest[nDestPos] = '\0';

	return nDestPos;
}

int WINAPI fnEncodeMessage(_LPTDEFAULTMESSAGE lptdm, char *pszBuf, int nLen)
{
	unsigned char	btBuffer[32];

	memcpy(btBuffer, (void *)lptdm, sizeof(_TDEFAULTMESSAGE));

	return fnEncode6BitBuf(btBuffer, pszBuf, sizeof(_TDEFAULTMESSAGE), nLen);
}

int  WINAPI fnDecodeMessage(_LPTDEFAULTMESSAGE lptdm, char *pszBuf)
{ return fnDecode6BitBuf(pszBuf, (char *)lptdm, sizeof(_TDEFAULTMESSAGE)); }

void WINAPI fnMakeDefMessage(_LPTDEFAULTMESSAGE lptdm, WORD wIdent, int nRecog, WORD wParam, WORD wTag, WORD wSeries)
{ lptdm->wIdent	= wIdent; lptdm->nRecog	= nRecog; lptdm->wParam	= wParam; lptdm->wTag = wTag; lptdm->wSeries = wSeries; }

