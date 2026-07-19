#pragma once
namespace SGDP
{

#ifndef SDAPI
	#define SDAPI __stdcall
#endif
	

	struct SDSamplerVersion
	{
		unsigned short wMajorVersion; //主版本号
		unsigned short wMinorVersion; //副版本号
		unsigned short wMinimumVersion; //最小版本号
		unsigned short wBuildNumber; //编译版本号
	};

	const SDSamplerVersion SDSAMPLE_VERSION = {0,0,0,1};


    class ISDSampler
    {
    	public:
    	    //完成组件初始化工作
    	    virtual   int SDAPI SetHead(int gameID,						//表示游戏ID
										short gameArea,						//游戏区ID
										short gameGroup,					//游戏组ID
										const char* userID,					//用户名
										UINT clientType = 1					//客户端类型，默认1为普通客户端版本，2为WEB版本，其他的保留
										) = 0;
			//采集结构化数据
			virtual	  int SDAPI LogStruct(	int nIDentID,       //消息类型
											char *structData,	//数据地址
											int length			//数据长度
										 ) = 0;
    	    //完成组件清理工作
    	    virtual   int SDAPI Release() = 0;
    };
    //获取模块
	 ISDSampler*  SDAPI SDSamplerGetModule(const SDSamplerVersion* pstVersion);
    //指向SDSamplerGetModule的函数指针
    typedef ISDSampler* (SDAPI *PFN_SDSamplerGetModule)(const SDSamplerVersion* pstVersion);
}
