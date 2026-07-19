#pragma once

#include "../Global/GlobalDefine.h"
#include <vector>
#include <map>

// 纹理内存池
class CTexPool
{
	struct stPool;
	struct stEle;
	typedef vector<BYTE *>			VPoint;
	typedef vector<stPool>			VPools;
	typedef std::map<BYTE *,stEle>		MPoint;

	// 每个池的信息
	struct stPool
	{
		DWORD	dwEleSize;			// 每个单元大小
		DWORD	dwBaseEles;			// 基本单元个数

		DWORD	dwCurEles;			// 当前单元个数
		DWORD	dwMaxEles;			// 最大单元个数
		DWORD	dwCurAvlSize;		// 当前有效字节数

		VPoint	vFreePool;			// 空闲池
	};

	// 每个元素的信息
	struct stEle
	{
		DWORD	dwSize;				// 元素大小
		stPool*	pPool;				// 元素所在的池
	};

	DWORD	m_dwCurAvlSize;		// 当前池实用字节数
	DWORD	m_dwMaxAvlSize;		// 最大池实用字节数
	DWORD	m_dwCurAllSize;		// 当前池占用字节数
	DWORD	m_dwMaxAllSize;		// 最大池占用字节数

	BOOL	m_bUsePool;			// 是否使用缓冲
	MPoint	m_mUsedEles;		// 使用中的单元
	VPools	m_vPools;			// 池数组

	DWORD	m_dwOverSize;		// 使用字节数
	DWORD	m_dwMaxOverSize;	// 使用系统最大字节数

	// 为了线程安全
	CRITICAL_SECTION	m_CriSect;
private:
	BOOL	InitPool(void);
	void	ReleaseAll(void);

public:
	CTexPool(BOOL bUsePool = TRUE);
	~CTexPool(void);

	BYTE *	NewTexMem(DWORD size);
	void	DelTexMen(BYTE *p);
	BOOL	CleanUp(void);
};