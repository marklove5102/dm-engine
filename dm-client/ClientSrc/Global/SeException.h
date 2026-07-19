#pragma once

#ifndef _WINDOWS_
#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
#include <windows.h>
#endif

#include <exception>
#include <string>

extern	bool	g_bAlertException;//出现异常报警，只用于内部测试
extern  int     g_iCatchTimes;


class CSeException: public std::exception
{
public:
	static void Install_Handler();//在每个线程启动后要先调用这个函数
	virtual const char* What() const { return m_strWhat.c_str(); };
	const void* Address() const { return m_pAddress; };
	DWORD Code() const { return m_dwCode; };
protected:
	CSeException(const EXCEPTION_RECORD& info);
	static void Translate(unsigned code, EXCEPTION_POINTERS* info);
private:
	std::string m_strWhat;
	const void* m_pAddress;
	DWORD m_dwCode;
};
