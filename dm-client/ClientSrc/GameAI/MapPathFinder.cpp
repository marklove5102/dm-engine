#include "GameData/ActionStruct.h"
#include "GameData/GameGlobal.h"
#include "GameData/GameData.h"
#include "PathFinder.h"
#include "math.h"
#include "GameControl/GameControl.h"
#include "GameMap/GameMap.h"

//#define AI_DEBUG

//------------------------------------------------------------------------------
// Purpose : 地图路径查找线程,可结束式的，单一进程
// Input   : 
// Output  : 
//------------------------------------------------------------------------------
DWORD WINAPI MapPathFindingThreadFunc(LPVOID lpParam) 
{
	CSeException::Install_Handler();

	bool bOk;
	CMapPathFinderAgent *m_pAgent =(CMapPathFinderAgent *)lpParam ;	//使用主地图的寻路
	m_pAgent->BeginPathFindingThread();
//	while( m_pAgent->IsRunMapPathFindingThread())
	{
		// 处理地图路径查找
		bOk = false;
		if(m_pAgent->HavePathFindTask())
			bOk = m_pAgent->ExcutePathFindTask();//执行查找任务,这是一个耗时的操作
//		if( !bOk ) Sleep(50);
	}

	m_pAgent->ExitPathFindingThread();
	return 0;
}

CMapPathFinderAgent::CMapPathFinderAgent():CMapPathFinder()
{
	m_hMapPathFindingThread			= NULL;		//线程句柄
	m_dwMapPathFindingThreadId		= 0;		//线程句柄标记
	m_bMapPathFindingThreadRunning	= FALSE;	//运行标记
	m_bRunMapPathFindingThread		= FALSE;	//是否打开运行标记

	INIT_HANDLE(m_hMapPathLock);				// 

//	InitPathFindingThread();
}

CMapPathFinderAgent::~CMapPathFinderAgent()
{
	int i;
	m_bRunMapPathFindingThread = FALSE;	//等待线程同步与退出
	for(i=0;i<500;i++)
	{
		if(m_bMapPathFindingThreadRunning)
			Sleep(10);
		else
			break;
	}
	if(m_hMapPathFindingThread)
	{
		if(m_bMapPathFindingThreadRunning)
			TerminateThread(m_hMapPathFindingThread,0);
		CloseHandle(m_hMapPathFindingThread);
	}
	DELETE_HANDLE(m_hMapPathLock);
}

//------------------------------------------------------------------------------
// Purpose : Lock
// Input   : 
// Output  : 
//------------------------------------------------------------------------------
void CMapPathFinderAgent::Lock()
{
	LOCK_HANDLE(m_hMapPathLock);
}

//------------------------------------------------------------------------------
// Purpose : UnLock
// Input   : 
// Output  : 
//------------------------------------------------------------------------------
void CMapPathFinderAgent::UnLock()
{
	UNLOCK_HANDLE(m_hMapPathLock);
}

//------------------------------------------------------------------------------
// Purpose : 初始化线程
// Input   : 
// Output  : 
//------------------------------------------------------------------------------
void CMapPathFinderAgent::InitPathFindingThread()
{
	// 
}

//------------------------------------------------------------------------------
// Purpose : 线程是否运行标记
// Input   : 
// Output  : 
//------------------------------------------------------------------------------
void CMapPathFinderAgent::BeginPathFindingThread()
{
	m_bMapPathFindingThreadRunning = TRUE;	
}

//------------------------------------------------------------------------------
// Purpose : 退出线程，由
// Input   : 
// Output  : 
//------------------------------------------------------------------------------
void CMapPathFinderAgent::ExitPathFindingThread()
{
	//提交任务结果
	m_bRunMapPathFindingThread		= FALSE;
	m_bMapPathFindingThreadRunning	= FALSE;
}



//---代理------------------------------------------------------


//------------------------------------------------------------------------------
// Purpose : 是否有未完成的任务,如果有，则线程进入工作状态
// Input   : 
// Output  : 
//------------------------------------------------------------------------------
bool CMapPathFinderAgent::HavePathFindTask()
{
	if(m_CurrentTask.tag == PFT_WAITING)
		return true;
	if(m_CurrentTask.tag == PFT_FINISHED)
		FinishPathFindTask();	//结束并提交
	return false;
}

//------------------------------------------------------------------------------
// Purpose : 完成路径结果的转储，该函数可能需要多次判断执行
// Input   : 
// Output  : 
//------------------------------------------------------------------------------
bool CMapPathFinderAgent::FinishPathFindTask()
{
	if(m_CurrentTask.tag != PFT_FINISHED)
		return false;

	//
	if(m_LastTask.tag == PFT_WAITING)
	{
		if(!QueueIsWaitTooManyTime(m_LastTask.begin))
			return true;
		m_LastTask.Clear(1);
	}

	if(m_LastTask.tag == PFT_IDLE)
	{
		//
		Lock();

		m_LastTask.Clone(m_CurrentTask,0,true);
		MovePathPoint(m_LastTask.m_PathPoints,m_PathPoints);
		m_LastTask.tag		= PFT_WAITING;
		m_LastTask.begin	= GetTickCount();

		UnLock();
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------
// Purpose : 执行任务，并且把结果返回给m_CurrentTask
// Input   : 
// Output  : 1--成功；0--失败
//------------------------------------------------------------------------------
bool CMapPathFinderAgent::ExcutePathFindTask()
{
	if(!HavePathFindTask())
		return false;
	
	m_CurrentTask.tag = PFT_RUNNING;

	int iIntegrity = 0;
	bool b = CreatePath(m_CurrentTask.src.x, m_CurrentTask.src.y, m_CurrentTask.dst.x, m_CurrentTask.dst.y,&iIntegrity);
	b = b && (!iIntegrity);

	//交替方向搜索？
	/*
	if(iIntegrity)
	{
#ifdef AI_DEBUG	
		output_debug("ExcutePathFindTask : 交替方向搜索\n");
#endif
		m_SearchDir =!m_SearchDir;
		b = CreatePath(m_CurrentTask.src.x, m_CurrentTask.src.y, m_CurrentTask.dst.x, m_CurrentTask.dst.y,&iIntegrity);
		b = b && (!iIntegrity);
	}
	*/

	m_CurrentTask.result = !b;
	if(m_CurrentTask.result)
	{
#ifdef AI_DEBUG	
		output_debug("ExcutePathFindTask : result = %d\n",m_CurrentTask.result);
#endif
	}

	m_CurrentTask.tag = PFT_FINISHED;
	return (b);
};	


//------------------------------------------------------------------------------
// Purpose : 代理接受任务，
// Input   : 
// Output  : 
//------------------------------------------------------------------------------
//！！！！！！和主线程操作同一数据容器，处理同步!!!
bool CMapPathFinderAgent::AcceptTask(MapPathTask_t &mpt)
{
	//当前线程正在执行寻路任务
	if(m_bMapPathFindingThreadRunning)
		return false;

	//判断当前任务是否处于工作状态
	if(m_CurrentTask.tag != PFT_IDLE)
	{
		//首先要清理上次可能未能提交的计算结果
		FinishPathFindTask();
		//如果同一个任务被push2次，直接返回1,需要判断
		if(m_CurrentTask.tag == PFT_RUNNING)
		{
			if(m_CurrentTask.RelateTask(mpt) > PFT_IDLE )
			{
				mpt.tag = PFT_RUNNING;				//修改主线程的任务标记
				return true;
			}
		}
		return false;
	}

	//设置代理任务
	m_CurrentTask.Clone(mpt);
	m_CurrentTask.tag	= PFT_WAITING;	
	m_CurrentTask.begin	= GetTickCount();
	mpt.tag				= PFT_RUNNING;				

	// 结束线程，释放上一次的资源
	if(m_hMapPathFindingThread)
	{
//		TerminateThread(m_hMapPathFindingThread,0);
		CloseHandle(m_hMapPathFindingThread);
		m_hMapPathFindingThread = 0;
	}
	//启动线程，执行任务
	m_bRunMapPathFindingThread		= TRUE;
	m_bMapPathFindingThreadRunning	= FALSE;
	m_hMapPathFindingThread = CreateThread(NULL,0,MapPathFindingThreadFunc,(DWORD *)this,0,&m_dwMapPathFindingThreadId);
	return true;
};

//------------------------------------------------------------------------------
// Purpose : 代理转发任务结果
// Input   : 
// Output  : 返回 ： 1成功； 0 代理正在计算；-1代理任务不存在
//------------------------------------------------------------------------------
//！！！！！！和主线程操作同一数据容器，处理同步!!!
int CMapPathFinderAgent::RelayTaskRusult(MapPathTask_t &mpt)
{
	//代理任务在等待用户提取？
	int res = m_LastTask.RelateTask(mpt);
	if(res == PFT_WAITING)
	{
		//同步
		Lock();

		mpt.Clone(m_LastTask,true,true);
		mpt.tag = PFT_FINISHED;	

		UnLock();

		return 1;
	}
	res = m_CurrentTask.RelateTask(mpt);
	if(res > PFT_IDLE)
	{
		if(res == PFT_FINISHED)
		{
			//为什么不取走？
			FinishPathFindTask();
		}
		return 0 ;
	}
	return -1;
};


//---申请------------------------------------------------------
//
CMapPathFinderClient::CMapPathFinderClient():CMapPathFinder()
{
};
CMapPathFinderClient::~CMapPathFinderClient()
{
};

//------------------------------------------------------------------------------
// Purpose : 申请者向代理提交任务
// Input   : 
// Output  : 
//------------------------------------------------------------------------------
bool CMapPathFinderClient::SubmitTask()
{
	bool b = g_MapPathFinderAgent.AcceptTask(m_CurrentTask);
	return b;
};	


//------------------------------------------------------------------------------
// Purpose : 地图路径寻找
// Input   : 
// Output  : 
//------------------------------------------------------------------------------
bool CMapPathFinderClient::PushPathFindTask(int sx, int sy, int dx, int dy)
{

#ifdef AI_DEBUG
	output_debug("PushPathFindTask \n");
#endif

	const char* MapID = g_pGameMap->GetMapName();

	if(IsBlock(dx,dy)) //阻挡点的，不好过去的，直接判断寻路失败
		return false;

	if(m_DstPathPoint.x == dx && m_DstPathPoint.y == dy) //当前终止点已经存在
		return true;

	if(_closePoint(sx,sy,dx,dy,10))//在十格范围内的，直接设置
	{
		ReleasePathPoint();

		bool b = true;

		m_CurrentRoute.clear();

		m_CurrentRoutePoint.x = dx;
		m_CurrentRoutePoint.y = dy;
		m_dwCurrentRouteTime = GetTickCount();

		SetMapID(MapID);

		m_CurrentTask.Clear(1);
		m_CurrentTask.tag = PFT_FINISHED;
		return b;
	}

	if(m_CurrentTask.IsWaitingTask(MapID,sx, sy, dx, dy))
		return true;

	//设置终止点位置
	m_DstPathPoint.x = dx;
	m_DstPathPoint.y = dy;

	//把原来的路径先清除掉
	ReleasePathPoint();
	m_CurrentRoute.clear();
	m_CurrentRoutePoint.x = m_CurrentRoutePoint.y = -1;

	//--同步
	//g_MapPathFinderAgent->Lock();

	m_CurrentTask.begin = GetTickCount();
	m_CurrentTask.tag	= PFT_WAITING;
	m_CurrentTask.src.SetValue(MapID,sx,sy);
	m_CurrentTask.dst.SetValue(MapID,dx,dy);

	//
	//g_MapPathFinderAgent->UnLock();

	SubmitTask();
	return true;
};

bool CMapPathFinderClient::ImmediatePathFindTask(int sx,int sy,int dx,int dy,int iLimit)
{
#ifdef AI_DEBUG
	output_debug("ImmediatePathFindTask \n");
#endif
	const char* MapID = g_pGameMap->GetMapName();

	if(IsBlock(dx,dy)) //阻挡点的，不好过去的，直接判断寻路失败
		return false;

	if(m_DstPathPoint.x == dx && m_DstPathPoint.y == dy) //当前终止点已经存在
		return true;

	ReleasePathPoint();

	m_DstPathPoint.x = dx;
	m_DstPathPoint.y = dy;

	bool b = true;

	if(_closePoint(sx,sy,dx,dy,iLimit))   //在十格范围内的，直接设置
	{
		m_CurrentRoute.clear();

		m_CurrentRoutePoint.x = dx;
		m_CurrentRoutePoint.y = dy;
		m_dwCurrentRouteTime = GetTickCount();

		SetMapID(MapID);

		m_CurrentTask.Clear(1);
	}
	else
	{
		int iIntegrity = 0;
		b = CreatePath(sx, sy, dx, dy,&iIntegrity);
		if(b)
		{
			if(iIntegrity == 0)
			{
				UsePathAsCurrentRoute(MapID);	//
				m_CurrentTask.Clear(1);
			}
			else
			{
				b = false;
			}
		}
	}
	m_CurrentTask.tag = PFT_FINISHED;
	if(!b)
	{
		m_DstPathPoint.x = -1;
		m_DstPathPoint.y = -1;
	}

	return b;
}

bool CMapPathFinderClient::ReFindPathImm(int sx,int sy)
{
	if(m_DstPathPoint.x <= 0 || m_DstPathPoint.y <= 0)
		return false;
	if(m_DstPathPoint.x == sx && m_DstPathPoint.y == sy) //当前终止点已经存在
		return false;

#ifdef AI_DEBUG
	output_debug("CMapPathFinderClient::ReSubmitFindTask: (%d,%d)->(%d,%d)\n",sx,sy,m_DstPathPoint.x,m_DstPathPoint.y);
#endif

	int dx = m_DstPathPoint.x;
	int dy = m_DstPathPoint.y;

	const char* MapID = g_pGameMap->GetMapName();

	if(IsBlock(dx,dy)) //阻挡点的，不好过去的，直接判断寻路失败
		return false;

	ReleasePathPoint();

	bool b = true;

	int iIntegrity = 0;
	b = CreatePath(sx, sy, dx, dy,&iIntegrity);
	if(b)
	{
		if(iIntegrity == 0)
		{
			UsePathAsCurrentRoute(MapID);	//
			m_CurrentTask.Clear(1);
		}
		else
		{
			b = false;
		}
	}

	m_CurrentTask.tag = PFT_FINISHED;
	if(!b)//没有能找到路径就关闭
	{
		SetOnRoute(0); 
		m_DstPathPoint.x = -1;
		m_DstPathPoint.y = -1;
	}

	return b;
}

//------------------------------------------------------------------------------
// Purpose : 没有找到路径重新寻找路径
// Input   : 
// Output  : 
//------------------------------------------------------------------------------
bool CMapPathFinderClient::ReSubmitFindTask(int sx,int sy)
{
	if(m_DstPathPoint.x <= 0 || m_DstPathPoint.y <= 0)
		return false;

#ifdef AI_DEBUG
	output_debug("CMapPathFinderClient::ReSubmitFindTask: (%d,%d)->(%d,%d)\n",sx,sy,m_DstPathPoint.x,m_DstPathPoint.y);
#endif

	int dx = m_DstPathPoint.x;
	int dy = m_DstPathPoint.y;

	m_DstPathPoint.x = -1;
	m_DstPathPoint.y = -1;

	if(this->PushPathFindTask(sx,sy,dx,dy))
		return true;

	SetOnRoute(0); //没有能找到路径就关闭

	return false;
}

//------------------------------------------------------------------------------
// Purpose : 把m_CurrentTask作为航路，同时删除搜索结果
// Input   : 
// Output  : 
//------------------------------------------------------------------------------
void CMapPathFinderClient::UseTaskPathAsCurrentRoute()
{
	ReleasePathPoint();

	MovePathPoint(m_PathPoints,m_CurrentTask.m_PathPoints);
	UsePathAsCurrentRoute(m_CurrentTask.src.mapID.c_str());	//
	m_CurrentTask.Clear(1);
};

//------------------------------------------------------------------------------
// Purpose : 判断是否有正在进行中的人物搜索结果
// Input   : 
// Output  : 
//------------------------------------------------------------------------------
int CMapPathFinderClient::TaskRunning()
{
	//判断当前的任务
	if(m_CurrentTask.tag == PFT_RUNNING)
	{

		int res = g_MapPathFinderAgent.RelayTaskRusult(m_CurrentTask);
		if(res == 1)
		{
			//还需要判断路径处理结果；因为路径可能不是完整路径
			UseTaskPathAsCurrentRoute();	
		}
		else if(res == -1)
		{
			m_CurrentTask.tag = PFT_WAITING;
			SubmitTask();
		}
		//else// if(res == 0) ----等待
		//{
		//}
		//判断任务是否完成

#ifdef AI_DEBUG
		output_debug("m_CurrentTask.tag == PFT_RUNNING ; res = %d\n",res);
#endif
		return PFT_RUNNING;
	}
	if(m_CurrentTask.tag == PFT_WAITING)
	{
#ifdef AI_DEBUG
		output_debug("m_CurrentTask.tag == PFT_WAITING \n");
#endif
		//判断任务是否等待时间太长？继续提交
//		if(TaskIsWaitTooManyTime(m_CurrentTask.begin))
		if(labs(GetTickCount() - m_CurrentTask.begin) >= 500)
			SubmitTask();
		return PFT_WAITING;
	}
	return PFT_IDLE;
}


//------------------------------------------------------------------------------
// Purpose : 
// Input   : 
// Output  : 
//------------------------------------------------------------------------------
void MapPathTask_t::Clear(bool bRoute)
{
	tag = PFT_IDLE;
	begin = 0;
	result = 0;
	src.SetValue();
	dst.SetValue();

	if(bRoute)
		ReleaseListPoint(m_PathPoints);
};

//从数据源复制到自己
//mpt -- 数据源
//bRoute -- 是否复制路径
//ClearSrc -- 是否清除空数据源
void MapPathTask_t::Clone(MapPathTask_t &mpt,bool bRoute,bool ClearSrc)
{
	tag		= mpt.tag;
	begin	= mpt.begin;
	result	= mpt.result;
	src		= mpt.src;
	dst		= mpt.dst;

	if(bRoute)
		MovePathPoint(m_PathPoints,mpt.m_PathPoints);
	if(ClearSrc)
		mpt.Clear(true);
};

//------------------------------------------------------------------------------
// Purpose : 
// Input   : 
// Output  : 
//------------------------------------------------------------------------------
bool MapPathTask_t::IsWaitingTask(const char* MapID,int sx, int sy, int dx, int dy)
{
	return (tag == PFT_WAITING) && (src.IsMapPoint(MapID,sx,sy) && dst.IsMapPoint(MapID,dx,dy));
};


//------------------------------------------------------------------------------
// Purpose : 判断任务关系，用于任务提交和获取结果的判断
// Input   : 
// Output  : 
//------------------------------------------------------------------------------
int  MapPathTask_t::RelateTask(MapPathTask_t &mpt)
{
	if(src.IsMapPoint(mpt.src) && dst.IsMapPoint(mpt.dst))
		return tag;
	return PFT_ERROR;
};	
