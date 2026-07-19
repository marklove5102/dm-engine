#pragma once

enum
{
	PROXY_CLIENT_PROTOCOL_VERSION		    = 0x01,  // 当前协议版本号

	PROXY_CLIENT_REP_SUCCEED				= 0x00,  // 成功连接
	PROXY_CLIENT_REP_PROXYSERVERFAILURE		= 0x01,  // 代理服务器内部错误
	PROXY_CLIENT_REP_AUTHENTICATIONFAILED	= 0x02,  // 认证失败
	PROXY_CLIENT_REP_NETUNREACHABLE			= 0x03,  // 远程网络不可达
	PROXY_CLIENT_REP_HOSTUNREACHABLE		= 0x04,  // 远程主机不可达
	PROXY_CLIENT_REP_CONNECTIONREFUSED		= 0x05,  // 远程主机拒绝连接
	PROXY_CLIENT_REP_UNKNOWHOST				= 0x06,  // DNS失败
	PROXY_CLIENT_REP_CONNECTIONNOTALLOWED	= 0x07,  // 代理服务器不允许该连接 
	PROXY_CLIENT_REP_ILLEGALSTATE			= 0x08,  // 非法的协议状态
	PROXY_CLIENT_REP_REMOTESERVERFAILURE	= 0x09,  // 连接远程服务器失败
	PROXY_CLIENT_REP_UNKNOWFORMAT			= 0x0A,  // 非法的协议包格式
	AUTH_METHOD_NONE = 0x00,                         // 不使用用户认证
};

// ------------------------------------------------------------
// ----   包头，包头的组成和校验可使用后面定义的辅助函数   ----
// ------------------------------------------------------------
#pragma pack(push, 1)

// ------------------------------------------------------------
// ----   包头，包头的组成和校验可使用后面定义的辅助函数   ----
// ------------------------------------------------------------
struct SProxyPkgHead
{
	WORD  wMark;         // 包标记
	WORD  wCheckSum;     // 校验和
	WORD  wDataLen;      // 用户数据长度(为协议固定项和用户自定义数据长度之和，不包括包头)
	BYTE  byFlag;        // 标志位
	BYTE  byOptLen;      // 选项
};


// ----------------------------
// ----  协议固定项 (请求) ----
// ----------------------------
struct SClientRequest
{
	BYTE     byVersion;       // 协议版本号，目前为0x01(PROXY_CLIENT_PROTOCOL_VERSION)   
	DWORD    dwDstAddr;       // 需要连接的远端服务器IP，网络字节序
	WORD     wDstPort;// 需要连接的远端服务器端口，网络字节序
	BYTE     byAuthMethod;    // 认证方式，包括AUTH_METHOD_XXXXXX开头的常数定义
};

// ----------------------------
// ----  协议固定项 (回复) ----
// ----------------------------

struct SClientReply
{
	BYTE      byVersion;      // 协议版本号，目前为0x01(PROXY_CLIENT_PROTOCOL_VERSION)
	BYTE      byStatus;       // 代理服务器返回状态信息，包括PROXY_CLIENT_REP_XXXXXX开头的常数定义
};

#pragma  pack(pop)


#define PROXY_PKG_MARK    0xAAEE
// ---------------------------------
// ----  客户端协议包头辅助函数 ----
// ---------------------------------

inline DWORD BuildProxyPkgHead(SProxyPkgHead *pstHead, WORD wDataLen, BYTE byFlags=0, BYTE byOptLen=0, BYTE *pbyOpt=NULL)
{
	pstHead->wMark      = PROXY_PKG_MARK;
	pstHead->wDataLen   = wDataLen;
	pstHead->wCheckSum  = (pstHead->wDataLen ^ 0xBBCC) & 0x88AA;
	pstHead->byFlag    = byFlags;
	pstHead->byOptLen   = byOptLen;

	//转成网络字节序
	pstHead->wMark = htons(pstHead->wMark);
	pstHead->wDataLen = htons(pstHead->wDataLen);
	pstHead->wCheckSum = htons(pstHead->wCheckSum);
	if(byOptLen != 0)
	{
		memcpy( (char*)pstHead + sizeof(*pstHead), pbyOpt, byOptLen);
	}

	return sizeof(SProxyPkgHead) + wDataLen + byOptLen;
}


inline DWORD CheckProxyPkgHead(SProxyPkgHead *pstHead)
{
	SProxyPkgHead stTempHead;

	stTempHead.wMark = ntohs(pstHead->wMark);
	stTempHead.wDataLen = ntohs(pstHead->wDataLen);
	stTempHead.wCheckSum = ntohs(pstHead->wCheckSum);
	stTempHead.byFlag = pstHead->byFlag;
	stTempHead.byOptLen = pstHead->byOptLen;

	if(stTempHead.wMark != PROXY_PKG_MARK)
		return -1;

	WORD wCheckSum = (stTempHead.wDataLen ^ 0xBBCC) & 0x88AA;
	if(stTempHead.wCheckSum != wCheckSum)
		return -1;

	return (sizeof(SProxyPkgHead) + stTempHead.wDataLen + stTempHead.byOptLen);
}

inline void OutputProxyError(BYTE byStatus)
{
	switch(byStatus)
	{
	case PROXY_CLIENT_REP_PROXYSERVERFAILURE: 
		OutputDebugString("代理服务器内部错误\n");
		break;
	case PROXY_CLIENT_REP_AUTHENTICATIONFAILED:
		OutputDebugString("认证失败\n");
		break;
	case PROXY_CLIENT_REP_NETUNREACHABLE:
		OutputDebugString("远程网络不可达\n");
		break;
	case PROXY_CLIENT_REP_HOSTUNREACHABLE:
		OutputDebugString("远程主机不可达\n");
		break;
	case PROXY_CLIENT_REP_CONNECTIONREFUSED:
		OutputDebugString("远程主机拒绝连接\n");
		break;
	case PROXY_CLIENT_REP_UNKNOWHOST:
		OutputDebugString("DNS失败\n");
		break;
	case PROXY_CLIENT_REP_CONNECTIONNOTALLOWED:
		OutputDebugString("代理服务器不允许该连接\n");
		break; 
	case PROXY_CLIENT_REP_ILLEGALSTATE:
		OutputDebugString("非法的协议状态\n");
		break;
	case PROXY_CLIENT_REP_REMOTESERVERFAILURE:
		OutputDebugString("连接远程服务器失败\n");
		break;
	case PROXY_CLIENT_REP_UNKNOWFORMAT:
		OutputDebugString("非法的协议包格式\n");
		break;
	default:
		OutputDebugString("未知代理服务器错误\n");
		break;
	}
}