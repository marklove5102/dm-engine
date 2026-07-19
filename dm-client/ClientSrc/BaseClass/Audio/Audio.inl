
#include "../../Global/Interface/CallBackInterface.h"
#include "Engine/Stream.h"

//读取指定文件的内容,返回所读取的字节数，读取失败返回0
//pBuf：在函数内部申请，请在外部用delete[]释放,
inline long LoadFile(const char *szFileName,char **ppBuf,bool bThread)
{
	if(!szFileName|| *ppBuf != NULL)
		return 0;

	if (!g_pEStreamMgr)
	{
		return 0;
	}

	DataStreamInterface* stream = g_pEStreamMgr->OpenDataFile(szFileName,bThread,false,false,EP_AUDIO);
	if(!stream)
		return 0;

	long size = stream->size();
	if(size <= 0)
		return 0;

	*ppBuf = new char[size];
	if( stream->read(*ppBuf,size) <= 0) 
	{
		size = 0;
	}

	delete stream;

    return size;
}