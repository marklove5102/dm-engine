#include "SeException.h"
#include "eh.h"
#include <tchar.h>

bool	 g_bAlertException = false;
int      g_iCatchTimes = 0;


void CSeException::Install_Handler()
{
#ifndef _DEBUG
	_set_se_translator(CSeException::Translate);
#endif
}

void CSeException::Translate(unsigned code, EXCEPTION_POINTERS* info)
{
	// Windows可以保证 *(info->ExceptionRecord) 是可用的
	switch (code)//可以根据不同的类型抛出不同类型的从SeException继承的异常 
	{
	case EXCEPTION_ACCESS_VIOLATION:
		throw CSeException(*(info->ExceptionRecord));
		break;
	default:
		throw CSeException(*(info->ExceptionRecord));
	}
}

CSeException::CSeException(const EXCEPTION_RECORD& info)
: m_strWhat("Win32 exception"), m_pAddress(info.ExceptionAddress), m_dwCode(info.ExceptionCode)
{
#define CASE( x ) case EXCEPTION_##x: m_strWhat = _T(#x); break;

	switch (info.ExceptionCode) 
	{

		CASE( ACCESS_VIOLATION )
		CASE( DATATYPE_MISALIGNMENT )
		CASE( BREAKPOINT )
		CASE( SINGLE_STEP )
		CASE( ARRAY_BOUNDS_EXCEEDED )
		CASE( FLT_DENORMAL_OPERAND )
		CASE( FLT_DIVIDE_BY_ZERO )
		CASE( FLT_INEXACT_RESULT )
		CASE( FLT_INVALID_OPERATION )
		CASE( FLT_OVERFLOW )
		CASE( FLT_STACK_CHECK )
		CASE( FLT_UNDERFLOW )
		CASE( INT_DIVIDE_BY_ZERO )
		CASE( INT_OVERFLOW )
		CASE( PRIV_INSTRUCTION )
		CASE( IN_PAGE_ERROR )
		CASE( ILLEGAL_INSTRUCTION )
		CASE( NONCONTINUABLE_EXCEPTION )
		CASE( STACK_OVERFLOW )
		CASE( INVALID_DISPOSITION )
		CASE( GUARD_PAGE )
		CASE( INVALID_HANDLE )

	default:
		m_strWhat = "UnKnown Exception";
		break;

	}
}
