#include "Global.h"
#include "PerfCheck.h"

CPerfCheck g_PerfCheck;

/////////////////////////////////////////////////////////////////////////
CPerfList::CPerfList(void)
{
	pParent		= NULL;
	bActived	= FALSE;
	u64Begin	= 0;
	u64Count	= 0;

	memset(sName,0,sizeof(sName));
	vChilds.clear();
}

CPerfList::~CPerfList(void)
{
	for(size_t i = 0;i < vChilds.size();i++)
	{
		SAFE_DELETE(vChilds[i]);
	}
}

// 重置标识
void CPerfList::Reset(void)
{
	bActived	= FALSE;
	u64Count	= 0;
	for(size_t i = 0;i < vChilds.size();i++)
	{
		if(vChilds[i])
			vChilds[i]->Reset();
	}
}
/////////////////////////////////////////////////////////////////////////
CPerfCheck::CPerfCheck(void)
{
	m_pRoot			= NULL;			// 根节点
	m_pAddNode		= NULL;			// 正在添加的节点

	m_pDisNode		= NULL;			// 正在显示的节点
	m_pSelectedNode	= NULL;			// 当前选中的节点

	m_bPerfing		= TRUE;			// 开启性能检查
	m_iResultCount	= 0;
	m_dwLastCheck	= 0;
}

CPerfCheck::~CPerfCheck(void)
{
	SAFE_DELETE(m_pRoot);
}

// 增加一个监视点
CPerfList * CPerfCheck::AddPerf(LPCTSTR sName,BOOL bFix)
{
	CPerfList* pPerf = new CPerfList;
	if(!pPerf)
	{
		_asm int 3;			// 分配内存失败
		return pPerf;
	}

	// 不应该出现的情况
	if(m_pRoot && !m_pAddNode)
	{
		_asm int 3;			// 缺少一个总的根节点
		return pPerf;
	}

	// 写入节点名称
	if(strlen(sName) < sizeof(pPerf->sName))
		strcpy(pPerf->sName,sName);
	else
	{
		memcpy(pPerf->sName,sName,sizeof(pPerf->sName) - 1);
		pPerf->sName[sizeof(pPerf->sName) - 1] = 0;
	}

	// 把当前节点添加到节点树上面
	if(bFix && m_pRoot)				// 固定节点必定是根节点的子节点
	{
		pPerf->pParent = m_pRoot;
		m_pRoot->vChilds.push_back(pPerf);
	}
	else							// 非固定节点
	{
		pPerf->pParent = m_pAddNode;
		if(m_pAddNode)
			m_pAddNode->vChilds.push_back(pPerf);
		else
		{
			m_pRoot		= pPerf;
			m_pDisNode	= pPerf;
		}
	}
	return pPerf;
}

// 监视点开始
void CPerfCheck::BeginPerf(CPerfList *pPerf)
{
	if(!m_bPerfing)			// 查看是否正在性能检测
		return;

	m_pAddNode			= pPerf;			// 设置当前焦点节点
	pPerf->bActived		= TRUE;				// 标记该节点被激活过
	pPerf->u64Begin		= g_Timer._GetCPUCount();	// 记下当前时间戳
}

// 监视点结束
void CPerfCheck::EndPerf(CPerfList *pPerf)
{
	if(!m_bPerfing)			// 查看是否正在性能检测
		return;

	m_pAddNode			= pPerf->pParent;						// 设置父节点为当前焦点节点
	pPerf->u64Count		+= g_Timer._GetCPUCount() - pPerf->u64Begin;		// 累计当前节点时间值

	if(pPerf == m_pRoot)	// 性能检测是否结束(根节点结束)
	{
		if((int)(GetTickCount() - m_dwLastCheck) > PERF_SHOW_TIME)
		{
			m_iResultCount	= 0;
			OutputNode(m_pRoot,0);
			m_dwLastCheck = GetTickCount();
		}

		if(m_pRoot)
			m_pRoot->Reset();
	}
}

// 取得结果数
int	CPerfCheck::GetResultNums(void)
{
	return m_iResultCount;
}

// 取得具体某条结果
PerfResult&	CPerfCheck::GetResult(int idx)
{
	return m_vResult[idx];
}

// 输出一个节点的信息
void CPerfCheck::OutputNode(CPerfList *pPerf,DWORD dwData)
{
	if(pPerf)
	{
		if(m_iResultCount < PERF_MAX_RESULT)
		{
			m_vResult[m_iResultCount].dwLevel		= dwData;
			m_vResult[m_iResultCount].sName			= pPerf->sName;
			m_vResult[m_iResultCount].bActived		= pPerf->bActived;
			m_vResult[m_iResultCount].dwTimeCount	= (DWORD)m_pRoot->u64Count;
			m_vResult[m_iResultCount].fPercent		= ((DWORD)pPerf->u64Count) * 100.0f / (DWORD)m_pRoot->u64Count;

			m_iResultCount++;
		}

		for(size_t i = 0;i < pPerf->vChilds.size();i++)
			OutputNode(pPerf->vChilds[i],dwData + 1);
	}
}