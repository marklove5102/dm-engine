#pragma once

#include "../GlobalDefine.h"



#pragma pack (1)

typedef struct DlProtocalHeader
{
	DlProtocalHeader()
	{
		wProtocal = 0x1001;
		wFileCount = 0;
		dwID = 0; 
		i64Hash = 0;
		dwAttribute = 0;
		dwLen = 0;
		dwStartPos = 0;
		dwLastDWORD = 0;
		dwRev = 0;
	}
	WORD  wProtocal;//协议号
	WORD  wFileCount;//文件个数
	DWORD dwID;//id
	INT64 i64Hash;//hash值
	int   iWpf;//哪一个wpf文件,服务器返回给客户端用
	DWORD dwAttribute;    //属性E_FCTATTR类型
	DWORD dwLen;//文件长度,服务器用
	DWORD dwStartPos;//发送的起始位置,服务器用
	DWORD dwLastDWORD;//最后四个字节内容,用来校验文件数据
	DWORD dwRev;
}SDLProtocalHeader,*PSDLProtocalHeader;

typedef struct DlProtocalBody 
{
	DlProtocalBody()
	{
		dwID = 0; 
		i64Hash = 0;
		iWpf = 0;
		byPrior = EP_NORMAL;
		memset(byRev,0,3 * sizeof(BYTE));
		dwStartPos = 0;
		dwTickCount = GetTickCount();
	}

	DWORD dwID;//id
	INT64 i64Hash;//hash值
	int   iWpf;//哪一个wpf文件
	BYTE  byPrior;//优先级,参见enum eReadPrior
	BYTE  byRev[3];
	DWORD dwStartPos;//起始位置,用于断点续传
	DWORD dwTickCount;//请求的时间
}SDlProtocalBody,*PSDlProtocalBody;

#pragma pack ()


typedef vector<DlProtocalBody> VDlProtocalBody;


//服务器中的plist,配置了所有要在后台下载的文件
struct sDlFileList
{
	sDlFileList()
	{
		iBeginiVersion = 0;
		iDestVersion = 0;
	}

	int iBeginiVersion;//起始版本
	int iDestVersion;//目标版本
	string szPathName;//文件名
	string szMD5;//MD5
	string szArea;//有效区服
};
typedef vector<sDlFileList> VDlFileList;


class DataStreamInterface
{
public:
	virtual ~DataStreamInterface() {}
	virtual size_t read(void* buf, size_t count) = 0;
	virtual size_t readLine(char* buf, size_t maxCount, const char* delim = "\n") = 0;
	virtual const char * getLine( bool trimAfter = true ) = 0;
	virtual size_t skipLine(const char* delim = "\n") = 0;
	virtual void skip(long count) = 0;
	virtual void seek( size_t pos ) = 0;
	virtual size_t tell(void) const = 0;
	virtual bool eof(void) const = 0;
	virtual void close(void) = 0;
	virtual const char* getName(void) = 0;
	virtual size_t size(void) const = 0;
};

class CStreamManagerInterface 
{
public:
	virtual ~CStreamManagerInterface(){}

	// 从打包文件内读取
	virtual void Init() = 0;
	virtual DataStreamInterface* OpenTextureFile(const char * filename,bool bThread = false,bool bFromDir = false,DWORD dwID = 0,eReadPrior ePrior = EP_NORMAL) = 0;//bFromDir是否从目录里打开文件,否则如果有图包从图包中打开,没有还是从目录中打开
	virtual DataStreamInterface* OpenDataFile(const char * filename,bool bThread = false,bool bFromDir = false,bool bFullPath = false,eReadPrior ePrior = EP_NORMAL) = 0;//bFromDir是否从目录里打开文件,否则如果有图包从图包中打开,没有还是从目录中打开,bFullPath是否全路径,如果不是会自动加上安装路径\data
	virtual DataStreamInterface* CreateMemoryStream(void* pMem, size_t size, bool freeOnClose = false) = 0;
	virtual bool  HasFileInServer(const char * filename) = 0;//资源服务器中该文件是否存在
	virtual bool  HasFileInLocalWpf(const char * filename) = 0;//本地该文件是否存在
	virtual bool  HasFileInServer(__int64 iHash) = 0;//资源服务器中该文件是否存在
	virtual bool  HasFileInLocalWpf(__int64 iHash) = 0;//本地该文件是否存在
	virtual __int64 HashFileName(const char * filename) = 0;
	virtual void        clear() = 0;

	// 读写INI文件
	virtual char* GetConfigStr(const char* strKey,const char* strDefault = "") = 0;
	virtual int	  GetConfigInt(const char* strKey,int iDefault = 0) = 0;
	virtual void  SetConfigStr(const char* strKey,const char * strValue) = 0;
	virtual void  SetConfigInt(const char* strKey,int iValue) = 0;

	virtual char* GetOtherConfigStr(const char* strKey,const char* strDefault = "") = 0;
	virtual int	  GetOtherConfigInt(const char* strKey,int iDefault = 0) = 0;
	virtual void  SetOtherConfigStr(const char* strKey,const char * strValue) = 0;
	virtual void  SetOtherConfigInt(const char* strKey,int iValue) = 0;

	virtual char* GetGameStr(const char* strKey,const char* strDefault = "") = 0;
	virtual void  SetGameStr(const char* strKey,const char* strValue) = 0;
	virtual int	  GetGameInt(const char* strKey,int iDefault = 0) = 0;
	virtual void  SetGameInt(const char* strKey,int iValue) = 0;
	virtual char* GetWebsite(const char* strKey,const char* strDefault = "") = 0;

	virtual void  SetSectionPath(const char* szSection,const char* path) = 0;
	virtual char* GetINIStr(const char* strKey,const char* strDefault = "") = 0;
	virtual int   GetINIInt(const char* strKey,int iDefault = 0) = 0;

	virtual int   GetDownloadingFiles() = 0;//获得正在下载的文件个数
	virtual int   GetSavingFiles() = 0;//获得正在保存的文件个数
	virtual void  ClearDownloadingFilesList() = 0;//清空获得正在下载的文件列有
	virtual bool  OnDownloadedFile(const char *buf,int iLen) = 0;
	virtual bool  SaveDownloadedFileToWpf(const char *buf,int iLen,PSDlProtocalBody pBody = NULL) = 0;
	virtual bool  IsNeedDownloadFileFromServer() = 0;
	virtual void  SetNeedDownloadFileFromServer(bool val) = 0;
	virtual int   CheckDownLoadList() = 0;//检查是否有下载超时的,重新请求
	virtual void  SendDownloadFile(const char* strFileName,DWORD dwID,int iRev,eReadPrior ePrior) = 0;
	virtual bool  IsInDownloadList(const char* strFileName) = 0;//是否正在下载中
	virtual bool  IsInDownloadList(__int64 iHash) = 0;//是否正在下载中
	virtual map<__int64,BYTE>& GetAllFileHashMap() = 0;

};

extern CStreamManagerInterface* g_pStreamMgr;