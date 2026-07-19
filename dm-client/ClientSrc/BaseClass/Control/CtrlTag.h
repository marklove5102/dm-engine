#pragma once

#include "Global\DebugTry.h"
#include "Global\Global.h"
#include <map>
#include <string>

using namespace std;
//------------------------------//
//		动态类型识别(DTI)		//
//		Dynamic Tag	Identify	//
//------------------------------//
class CCtrlBaseTag;

class CTagClass
{
public:
	CTagClass();
	CTagClass(const char* pName, CTagClass* pSuper, int iSize);
	~CTagClass();

	const char* m_szClassName;	//类别名称
	CTagClass*	m_pSuperClass;	//父类
	static CTagClass* sm_pFirstClass;	//所有类别单链表头指针
	CTagClass*	m_pNextClass;	//链表下一元素
	int 		m_nObjSize;	//该类别对象大小

public:
	BOOL	DoesNameMatch(const char* szName);		//根据字符串判别类型是否相符
	inline const char*	GetName() { return m_szClassName;}	//返回类别名字符串
	static	CTagClass* FindType(const char* szName);		//查找类别
	inline int GetTagSize() { return m_nObjSize;}			//察看该类别大小
};


//CTagClass::m_MLpCreateObjectFun[#_class] = &_class::CreateObject;\

#define DTI_DECLARE(_class, _superClass)	\
	public:	\
	static CTagClass DTI_##_class;	\
	virtual CTagClass*	GetDTIType()	const;	\
	typedef _superClass	Super;		\
	typedef _class ThisClass;


#define	DTI_IMPLEMENT(_class, _superClass)	\
	CTagClass	_class::DTI_##_class(#_class, &(_superClass::DTI_##_superClass), sizeof(_class));\
	CTagClass*	_class::GetDTIType() const	\
	{ return &(_class::DTI_##_class);}

#define DTI(_class)	\
	&(_class::DTI_##_class)

//------------------------------//
//	所有能动态识别类型的基类	//
//------------------------------//
class CCtrlBaseTag
{
public:
	CCtrlBaseTag(void);
	virtual ~CCtrlBaseTag(void);

public:
	BOOL	IsKindOf(CTagClass*	pC);	//是否属于某一基类
	virtual CTagClass*	GetDTIType()	const;	//返回动态类别标志
	const char* GetDTIName();
	static CTagClass DTI_CCtrlBaseTag;	//类别
};