#include "new.h"
#include "global.h"
#include <stdlib.h> 
#include "crtdbg.h"
#include <stdio.h>
#include "DebugTry.h"

#ifdef TEMP_MEM_CHECK
#include <vector>
#include <map>
#endif

#define NULL 0

//是否写文件
#define OUTPUT_USE_MEM_SIZE

bool g_bGlobalValid = true;

int g_nLinkCount[10];
int g_nMaxLinkCount[10];

//#define TEMP_MEM_CHECK_NOW

#ifdef TEMP_MEM_CHECK
	
	static bool g_bDataIntanse = false;
	// 每次分配的纪录节点
	struct node_t
	{
		size_t idx;		// 文件名和行字符串索引
		size_t iSize;	// 分配了的大小
	};

	typedef std::map<void *,node_t>		InfoMap;
	typedef vector<char *>			StringVector;
	typedef vector<size_t>			SizeVector;

	void MemTrace();

	// 所有纪录的数据
	struct data_t
	{
		data_t():uLeakSize(0),bTracing(FALSE)
		{ 
			InitializeCriticalSection(&cs); 
			g_bDataIntanse = true;
		}
		~data_t() 
		{
			if(g_bException == FALSE)
				MemTrace();
			DeleteCriticalSection(&cs);
			g_bDataIntanse = false;
		}

		InfoMap				mMemTrace;		// 指针映射表
		StringVector		vFileName;		// 文件名称数组
		SizeVector			vFileMemSize;	// 文件名对应的内存的大小

		size_t				uLeakSize;		// 泄漏的大小
		BOOL				bTracing;		// 是否正在跟踪

		CRITICAL_SECTION	cs;				// 防止多线程的时候出问题
	};

	data_t& data() 
	{ 
		static data_t data_temp; 
		return data_temp;
	}

	struct data_lock_t
	{
		data_lock_t() 
		{ 
			EnterCriticalSection(&data().cs); 
		}
		~data_lock_t() 
		{ 
			LeaveCriticalSection(&data().cs); 
		}
	};

	void PrintMem()
	{
		if(data().vFileName.size() == data().vFileName.size())
		{
			FILE *fp = fopen("MemUse.txt","a+");
			if(fp)
			{
				DWORD dwTotal = 0;
				char sTemp[2048] = {0};
				for(size_t i = 0; i < data().vFileName.size(); ++i)
				{
					sprintf(sTemp,"%09d %s%\r\n",data().vFileMemSize[i],data().vFileName[i]);
					fputs(sTemp,fp);

					dwTotal += (DWORD)data().vFileMemSize[i];
				}
				sprintf(sTemp,"Total:%d\r\n",dwTotal);
				fputs(sTemp,fp);
				sprintf(sTemp,"-----------------------------------------------------------------------\r\n");
				fputs(sTemp,fp);

				fclose(fp);
			}
		}
		else
			DetailOutput("匹配不一致!");
	}

	void MemTrace()
	{
		data_lock_t block;

		if(g_bDataIntanse == false || data().bTracing == TRUE)
			goto FREE_FILE_NAME;

		if(data().mMemTrace.size() > 0)
		{
			char sMsg[512] = {0};
			FILE *fp = fopen("../memleak.log","a+");
			if(fp)
			{
				sprintf(sMsg,"------------------------------------------------------------\nMemory leak reports\n");
				fwrite(sMsg,1,strlen(sMsg),fp);
				for(InfoMap::iterator mapIter = data().mMemTrace.begin(); mapIter != data().mMemTrace.end(); ++mapIter)
				{
					sprintf(sMsg,"%s :%d bytes!\n",data().vFileName[(mapIter->second).idx],(mapIter->second).iSize);
					fwrite(sMsg,1,strlen(sMsg),fp);

					data().uLeakSize += (mapIter->second).iSize;
				}
				sprintf(sMsg,"Total size:%d bytes！\n------------------------------------------------------------\n",data().uLeakSize);
				fwrite(sMsg,1,strlen(sMsg),fp);
				fclose(fp);

				MessageBox(0,"检测到内存泄漏！请把游戏目录下的memleak.log提交给开发人员,谢谢！","传奇世界--内存泄漏报告！",0);
			}
		}
FREE_FILE_NAME:
		for(size_t i = 0; i < data().vFileName.size(); ++i)
		{
            if(data().vFileName[i])
				free(data().vFileName[i]);
		}
	}

	void* __cdecl operator new(size_t nSize, int nType, const char * lpszFileName, int nLine)
	{
		data_lock_t block;

		void * p = _malloc_dbg(nSize, nType, lpszFileName, nLine);

		//if(nSize > 100 * 1000)
		//	output_debug("%s(%d):%d\n",lpszFileName,nLine,nSize);

		if(p && g_bDataIntanse == true && lpszFileName != NULL)
		{
			if(data().bTracing == FALSE)
			{
				data().bTracing = TRUE;

				node_t node_temp;
				char sMsg[512];

				sprintf(sMsg,"%s(%d)",lpszFileName,nLine);

				size_t i = 0;

				for(i = 0; i < data().vFileName.size(); ++i)
				{
					if(strcmp(sMsg,data().vFileName[i]) == 0)
						break;
				}

				if(i == data().vFileName.size())
				{
					size_t uLen = strlen(sMsg);
					char *sFile = (char *)malloc(uLen + 1);
					memcpy(sFile,sMsg,uLen);
					sFile[uLen] = 0;

					data().vFileName.push_back(sFile);
					data().vFileMemSize.push_back(0);
				}

				node_temp.idx	= i;
				node_temp.iSize	= nSize;

				data().vFileMemSize[i] += nSize;

				data().mMemTrace[p] = node_temp;

				data().bTracing = FALSE;

				//if(nSize == 65076)
				//	OutputDebugString("------------------\n");
			}
		}
 		return p;
	}

	void __cdecl operator delete(void* p)
	{
		if(p && g_bDataIntanse == true)
		{
			data_lock_t block;
			if(data().bTracing == FALSE)
			{
				data().bTracing = TRUE;
				InfoMap::iterator mapIter = data().mMemTrace.find(p);
				if(mapIter != data().mMemTrace.end())
				{
					node_t nt = mapIter->second;
					if(nt.idx < data().vFileMemSize.size())
					{
						if(data().vFileMemSize[nt.idx] >= nt.iSize)
							data().vFileMemSize[nt.idx] -= nt.iSize;
						else
                            DetailOutput("大小错误!");
					}
					else
						DetailOutput("索引错误!");
					data().mMemTrace.erase(mapIter);
				}
				data().bTracing = FALSE;
			}
		}
		_free_dbg(p, _NORMAL_BLOCK);
	}

	void* __cdecl operator new(size_t nSize, const char * lpszFileName, int nLine)
	{ 
		void *p = ::operator new(nSize, _NORMAL_BLOCK, lpszFileName, nLine);
		return p;
	}

	void* __cdecl operator new[](size_t nSize, const char * lpszFileName, int nLine)
	{
		void *p = ::operator new[](nSize, _NORMAL_BLOCK, lpszFileName, nLine);
		return p;
	}

	void* __cdecl operator new[](size_t nSize, int nType, const char * lpszFileName, int nLine)
	{
		void *p = ::operator new(nSize, nType, lpszFileName, nLine);
		return p;
	}

	void* __cdecl operator new[](size_t nSize)
	{
		void *p = ::operator new(nSize, _NORMAL_BLOCK, "DunnoFile", 0);
		return p;
	}

	// delete 函数
	void __cdecl operator delete(void* pData, const char *,int)
	{
		::operator delete(pData);
	}

	void __cdecl operator delete[](void* pData, const char *,int)
	{
		::operator delete(pData);
	}

	void __cdecl operator delete[](void* p)
	{
		::operator delete(p);
	}
#endif