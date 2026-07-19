#pragma once

#include "SeException.h"
#include <assert.h>
#include <string>

#ifndef _WINDOWS_
#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
#include <windows.h>
#endif

#ifndef __DEBUG__TRY__
#define __DEBUG__TRY__

void SaveException(char *file,char *fun,int line,const CSeException* pException = NULL,const char *msg = NULL);

#ifndef _DEBUG

//以下try包含的语句范围要尽量小,尽量不要在try里声音有析构函数的对象或变量,出现异常时不会调用try block里声明的对象的析构函数,要可能会导致堆栈破坏
#define TRY_BEGIN try {

#define TRY_END	}\
	        catch(const CSeException& e)\
            {\
			    SaveException(__FILE__,__FUNCTION__,__LINE__,&e);\
            }\
			catch(const exception& e)\
			{\
				SaveException(__FILE__,__FUNCTION__,__LINE__,NULL,e.what());\
			}\
			catch(...)\
			{\
				SaveException(__FILE__,__FUNCTION__,__LINE__);\
			}

//如果strCode里有return,break,goto,continue等跳转语句,请用TRY_END_DO_JUMP
#define TRY_END_DO(strCode)	}\
	        catch(const CSeException& e)\
            {\
			   string strAddMsg = "";\
			   strCode;\
			   SaveException(__FILE__,__FUNCTION__,__LINE__,&e,strAddMsg.c_str());\
            }\
			catch(const exception& e)\
			{\
			    string strAddMsg;\
			    strCode;\
				strAddMsg += e.what();\
				SaveException(__FILE__,__FUNCTION__,__LINE__,NULL,strAddMsg.c_str());\
			}\
	        catch(...)\
            {\
			   string strAddMsg = "";\
			   strCode;\
			   SaveException(__FILE__,__FUNCTION__,__LINE__,NULL,strAddMsg.c_str());\
            }

//因为在strCode里可能有return,break,goto,continue等跳转语句,所以要求在strCode代码中自行在适当位置加上:SaveException(__FILE__,__FUNCTION__,__LINE__,pSec,strAddMsg.c_str());
#define TRY_END_DO_JUMP(strCode) }\
	        catch(const CSeException& e)\
            {\
			   string strAddMsg = "";\
			   const CSeException *pSec = &e;\
			   strCode;\
            }\
			catch(const exception& e)\
			{\
			    std::string strAddMsg = e.what();\
				const CSeException *pSec = NULL;\
			    strCode;\
			}\
	        catch(...)\
            {\
			   string strAddMsg = "";\
			   const CSeException *pSec = NULL;\
			   strCode;\
            }

#define TRY_END_NOTHROW }\
	        catch(const CSeException& e)\
            {\
			    SaveException(__FILE__,__FUNCTION__,__LINE__,&e);\
            }\
			catch(const exception& e)\
			{\
				SaveException(__FILE__,__FUNCTION__,__LINE__,NULL,e.what());\
			}\
			catch(...) \
			{\
			   SaveException(__FILE__,__FUNCTION__,__LINE__);\
			}

#define TRY_END_RETURN(str) }\
	        catch(const CSeException& e)\
            {\
			    SaveException(__FILE__,__FUNCTION__,__LINE__,&e);\
				return str;\
            }\
			catch(const exception& e)\
			{\
				SaveException(__FILE__,__FUNCTION__,__LINE__,NULL,e.what());\
				return str;\
			}\
			catch(...)\
			{\
				SaveException(__FILE__,__FUNCTION__,__LINE__);\
				return str;\
			}

#define TRY_END_FATALERROR(str,code) }\
			catch(...) \
			{\
				fatal_error(str,code);\
			}

#else
#define TRY_BEGIN
#define TRY_END
#define TRY_END_DO(strCode)
#define TRY_END_DO_JUMP(strCode)
#define TRY_END_NOTHROW
#define TRY_END_RETURN(str)
#define TRY_END_FATALERROR(str,code)
#endif



#endif
