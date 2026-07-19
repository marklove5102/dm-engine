#pragma once

#include "Global/Globaldefine.h"

//#define PERF_CHECK

#define PERF_MAX_RESULT		256
#define PERF_SHOW_TIME		1000

#ifdef PERF_CHECK
	#define PERF_SEG_BEGIN(name,bFix)	{\
											static CPerfList* s_p##name##PerfNode = g_PerfCheck.AddPerf(#name,bFix);\
											CPerfTemp temp##name##Perf(s_p##name##PerfNode)
	#define PERF_SEG_END(name)			}
	#define PERF_SEG_ALL(name,bFix)		static CPerfList* s_p##name##PerfNode = g_PerfCheck.AddPerf(#name,bFix);\
										CPerfTemp temp##name##Perf(s_p##name##PerfNode)
#else
	#define PERF_SEG_BEGIN(name,bFix)
	#define PERF_SEG_END(name)
	#define PERF_SEG_ALL(name,bFix)
#endif
///////////////////////////////////////

class CPerfList;
class CPerfCheck;

extern CPerfCheck g_PerfCheck;

struct PerfResult
{
	PerfResult(DWORD lvl = 0,LPCTSTR s = NULL,BOOL b = FALSE,DWORD count = 0,float per = 0.0f):
	dwLevel(lvl),sName(s),bActived(b),dwTimeCount(count),fPercent(per)
	{
	}

	DWORD	dwLevel;
	LPCTSTR	sName;
	BOOL	bActived;
	DWORD	dwTimeCount;
	float	fPercent;
};

// 计数保存链表
class CPerfList
{
	friend class CPerfCheck;

protected:
	CPerfList *				pParent;	// 父节点
	vector<CPerfList*>	vChilds;	// 子节点

	char	sName[32];					// 当前节点名称
	UINT64	u64Begin;					// 开始时间

	// 需要清空的
	BOOL	bActived;					// 本帧该节点是否激活
	UINT64	u64Count;					// 时间计数

public:
	CPerfList(void);
	~CPerfList(void);

	void	Reset(void);				// 从新设置
};

// 性能检测类
class CPerfCheck
{
private:
	BOOL			m_bPerfing;					// 是否正在性能检测

	DWORD			m_dwLastCheck;				// 上次检测时间
	int				m_iResultCount;				// 检查结果计数
	PerfResult		m_vResult[PERF_MAX_RESULT];	// 检测结果

	CPerfList *		m_pRoot;			// 根节点
	CPerfList *		m_pAddNode;			// 正在添加的节点

	CPerfList *		m_pDisNode;			// 正在显示的节点
	CPerfList *		m_pSelectedNode;	// 当前选中的节点

	void		OutputNode(CPerfList *pPerf,DWORD dwData);	// 输出一个节点的信息

public:
	CPerfCheck(void);
	~CPerfCheck(void);

	CPerfList *	AddPerf(LPCTSTR sName,BOOL bFix);	// 增加一个节点
	void		BeginPerf(CPerfList *pPerf);		// 节点开始计数
	void		EndPerf(CPerfList *pPerf);			// 节点结束计数
	int			GetResultNums(void);				// 取得结果数
	PerfResult&	GetResult(int idx);					// 取得具体某条结果
};

// 临时类，为了保证BeginPerf和EndPerf成对的调用
class CPerfTemp
{
	CPerfList *p;
public:
	CPerfTemp(CPerfList* pPerf)
	{
		g_PerfCheck.BeginPerf(pPerf);
		p = pPerf;
	}

	~CPerfTemp(void)
	{
		g_PerfCheck.EndPerf(p);
	}
};