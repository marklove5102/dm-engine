#include "CtrlTag.h"


//--------------------------//
//		动态类型识别		//
//--------------------------//
CTagClass*	CTagClass::sm_pFirstClass	= NULL;

CTagClass::CTagClass()
{
	m_pSuperClass	= NULL;
	m_szClassName	= NULL;
	m_pNextClass	= NULL;
	m_nObjSize		= 0;
}

CTagClass::CTagClass(const char* pName, CTagClass* pSuper, int iSize)
{
	m_szClassName	= pName;
	m_pSuperClass	= pSuper;

	m_pNextClass	= sm_pFirstClass;
	sm_pFirstClass	= this;
	m_nObjSize		= iSize;
}

CTagClass::~CTagClass()
{
}

BOOL CTagClass::DoesNameMatch(const char* szName)
{
	BOOL	bResult = strcmp(szName, m_szClassName);
	return (!bResult);
}

CTagClass*	CTagClass::FindType(const char* szName)
{
	CTagClass*	pCur = sm_pFirstClass;
	while(pCur)
	{
		if(pCur->DoesNameMatch(szName))
		{
			return pCur;
		}
		pCur = pCur->m_pNextClass;
	}
	return NULL;
}

//------------------------------//
//	所有能动态识别类型的基类	//
//------------------------------//
CTagClass CCtrlBaseTag::DTI_CCtrlBaseTag("CCtrlBaseTag", NULL, sizeof(CCtrlBaseTag));

CCtrlBaseTag::CCtrlBaseTag(void)
{
}

CCtrlBaseTag::~CCtrlBaseTag(void)
{
}

CTagClass*	CCtrlBaseTag::GetDTIType() const
{
	return &(DTI_CCtrlBaseTag);
}


BOOL CCtrlBaseTag::IsKindOf(CTagClass*	pC)
{
	CTagClass*	p = GetDTIType();
	while(p!=NULL)
	{
		if(p == pC) return TRUE;
		p = p->m_pSuperClass;
	}
	return FALSE;
}

const char* CCtrlBaseTag::GetDTIName()
{
	CTagClass*	pType = GetDTIType();
	return (pType)?pType->GetName():"";
}
