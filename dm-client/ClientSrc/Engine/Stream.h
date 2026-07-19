#pragma once

#include "Global/Interface/StreamInterface.h"
#include "Wpf.h"
#include <vector>

#define  SECTION_LEN	  64
#define  PATH_LEN         512
#define  MAX_WPF_HASH_BUFFF_SIZE   0x500 //用来计算hash值



class DataStream: public DataStreamInterface
{
protected:
	string mName;		
	size_t mSize;
#define STREAM_TEMP_SIZE 256
	/// Temporary buffer area used for formatted read
	char mTmpArea[STREAM_TEMP_SIZE];
public:
	/// Constructor for creating unnamed streams
	DataStream() : mSize(0) {}
	/// Constructor for creating named streams
	DataStream(const string& name) : mName(name), mSize(0) {}
	/// Returns the name of the stream, if it has one.
	virtual ~DataStream() {}
	// Streaming operators
	template<typename T> DataStream& operator>>(T& val);
	virtual const char * getName(void) { return mName.c_str(); }
	virtual const char * getLine( bool trimAfter = true );
	virtual size_t size(void) const { return mSize; }
};

class CMemoryStream : public DataStream
{
protected:
	/// Pointer to the start of the data area
	BYTE* mData;
	/// Pointer to the current position in the memory
	BYTE* mPos;
	/// Pointer to the end of the memory
	BYTE* mEnd;
	/// Do we delete the memory on close
	bool mFreeOnClose;			
public:
	CMemoryStream(void* pMem, size_t size, bool freeOnClose = false);
	CMemoryStream(DataStream& sourceStream,bool freeOnClose);

	~CMemoryStream();
	BYTE* getPtr(void) { return mData; }
	BYTE* getCurrentPtr(void) { return mPos; }
	size_t read(void* buf, size_t count);
	size_t readLine(char* buf, size_t maxCount, const char* delim = "\n");
	size_t skipLine(const char* delim = "\n");
	void skip(long count);
	void seek( size_t pos );
	size_t tell(void) const;
	bool eof(void) const;
	void close(void);
	void setFreeOnClose(bool free) { mFreeOnClose = free; }
};

class CFileStream : public DataStream
{
protected:
	FILE* mFileHandle;
	size_t mPos;
	size_t mOff;

	/// Do we delete the memory on close
	bool mFreeOnClose;			
public:
	/// Create stream from a C file handle
	CFileStream(FILE* handle,bool freeOnClose = false);
	/// Create named stream from a C file handle
	CFileStream(const string& name, FILE* handle,bool freeOnClose = false);
	/// Create keyed stream from a C file handle
	CFileStream(FILE* handle,size_t off,size_t size,bool freeOnClose = false);

	~CFileStream();
	size_t read(void* buf, size_t count);
	size_t readLine(char* buf, size_t maxCount, const char* delim = "\n");
	size_t skipLine(const char* delim = "\n");
	void skip(long count);
	void seek( size_t pos );
	size_t tell(void) const;
	bool eof(void) const;
	void close(void);
	void setFreeOnClose(bool free) { mFreeOnClose = free; }
};

class CWpfFile
{
public:
	CWpfFile();
	~CWpfFile();
	CWpf* pWpfFile(){return pWpf;}

private:
	friend class CStreamManager;

	//文件信息
	FILE*	fp1;			// 主线程文件的指针
	FILE*	fp2;			// 读线程文件的指针
	CWpf * pWpf;            // 操作接口

	string  strPath;   // SNDA Packed File文件的路径
	string  strName;   // 文件名称
	int iDx;//编号,用于保存下载下来的文件到正确的图包中
};

class CStreamManager:public CStreamManagerInterface 
{
public:
	CStreamManager();
	virtual ~CStreamManager();

	// 从打包文件内读取
	void Init();
	DataStreamInterface* OpenTextureFile(const char * filename,bool bThread = false,bool bFromDir = false,DWORD dwID = 0,eReadPrior ePrior = EP_MOST_HIGH);//bFromDir是否从目录里打开文件,否则如果有图包从图包中打开,没有还是从目录中打开
	DataStreamInterface* OpenDataFile(const char * filename,bool bThread = false,bool bFromDir = false,bool bFullPath = false,eReadPrior ePrior = EP_MOST_HIGH);//bFromDir是否从目录里打开文件,否则如果有图包从图包中打开,没有还是从目录中打开,bFullPath是否全路径,如果不是会自动加上安装路径\data
	DataStreamInterface* CreateMemoryStream(void* pMem, size_t size, bool freeOnClose = false);

	virtual bool  HasFileInServer(const char * filename);//资源服务器中该文件是否存在
	virtual bool  HasFileInLocalWpf(const char * filename);//本地该文件是否存在
	virtual bool  HasFileInServer(__int64 iHash);//资源服务器中该文件是否存在
	virtual bool  HasFileInLocalWpf(__int64 iHash);//本地该文件是否存在

	virtual __int64 HashFileName(const char * filename);//计算路径的hash值
	void        clear();

	// 读写INI文件
	char* GetConfigStr(const char* strKey,const char* strDefault = "");
	int	  GetConfigInt(const char* strKey,int iDefault = 0);
	void  SetConfigStr(const char* strKey,const char * strValue);
	void  SetConfigInt(const char* strKey,int iValue);

	char* GetOtherConfigStr(const char* strKey,const char* strDefault = "");
	int	  GetOtherConfigInt(const char* strKey,int iDefault = 0);
	void  SetOtherConfigStr(const char* strKey,const char * strValue);
	void  SetOtherConfigInt(const char* strKey,int iValue);

	char* GetGameStr(const char* strKey,const char* strDefault = "");
	void  SetGameStr(const char* strKey,const char* strValue);
	int	  GetGameInt(const char* strKey,int iDefault = 0);
	void  SetGameInt(const char* strKey,int iValue);
	char* GetWebsite(const char* strKey,const char* strDefault = "");

	void  SetSectionPath(const char* szSection,const char* path);
	char* GetINIStr(const char* strKey,const char* strDefault = "");
	int   GetINIInt(const char* strKey,int iDefault = 0);

	bool        IsNeedDownloadFileFromServer() { return m_bNeedDownloadFileFromServer; }
	void        SetNeedDownloadFileFromServer(bool val);

	int   GetDownloadingFiles();//获得正在下载的文件个数
	int   GetSavingFiles();//获得正在保存的文件个数
	void  ClearDownloadingFilesList();//清空获得正在下载的文件列有
	bool  OnDownloadedFile(const char *buf,int iLen);
	bool  SaveDownloadedFileToWpf(const char *buf,int iLen,PSDlProtocalBody pBody = NULL);
	void  PreDownLoadTex(const char *strDir,int iPack,int iBeginID,int iEndID,eReadPrior ePrior);	// 根据ID预下载纹理
	int   CheckDownLoadList();//检查是否有下载超时的,重新请求
	map<__int64,BYTE>& GetAllFileHashMap(){return m_MFileHash;}


private:
	typedef std::vector<CWpfFile*>	 VWpfFile;
	VWpfFile          m_VWpf_Texture;//所有图片文件相关的wpf
	VWpfFile          m_VWpf_Data;//所有数据文件相关的wpf,比如music,mask,map

	static DWORD  m_dwCryptTable[MAX_WPF_HASH_BUFFF_SIZE];//用来计算hash值
	map<__int64,DlProtocalBody> m_MInDownloadFile;//正在下载的文件列表
	map<__int64,DlProtocalBody> m_MInSavingFile;//下载下来了正等待保存的文件列表
	map<__int64,BYTE> m_MFileHash;//所有文件的hash列表;
	CRITICAL_SECTION	m_DlFileCriSect;					// 访问m_MDownloadFileName及写文件到wpf的临界变量
	CRITICAL_SECTION	m_SavingFileMapCriSect;					// 访问m_MInSavingFile及写文件到wpf的临界变量
	CRITICAL_SECTION	m_WpfLock;					// 访问Wpf的临界变量
	int                 m_iInDownloadFileCount;//正在下载的数量


	char         m_szConfigFile[PATH_LEN];
	char         m_szOtherConfigFile[PATH_LEN];
	char         m_szGameFile[PATH_LEN];
	char         m_szWebsiteFile[PATH_LEN];
	char         m_szINIFile[PATH_LEN];

	char         SECTION_INI[SECTION_LEN];
	char         SECTION_NAME[SECTION_LEN];

	bool         m_bNeedDownloadFileFromServer;//没有数据的时候是否要求从下载服务器下载
	bool         m_bIsMultiClient;//是否后面多开的客户端,多开的不能写文件

	bool InitBuffer();
	//计算string的hash值
	DWORD HashString(const char * strFileName,DWORD type);


	bool        Add(const char* strFilePath,VWpfFile& vWpf,int iDx);
	bool        AddHash(const char* strFilePath,VWpfFile & vWpf,int iDx);
	void        SendDownloadFile(const char* strFileName,DWORD dwID,int iRev,eReadPrior ePrior);
	bool        IsInDownloadList(const char* strFileName);//是否正在下载中
	bool        IsInDownloadList(__int64 iHash);//是否正在下载中
	void        SendDownloadFile(VDlProtocalBody &VBody);

	void        MeargeDownloadWpf();//把下载下来的wpf文件合并到图包或释放到指定目录中
};

extern CStreamManager* g_pEStreamMgr;
