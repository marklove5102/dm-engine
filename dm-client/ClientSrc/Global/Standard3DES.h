
#ifndef DLL_STUFF
#	ifdef DLL_EXPORT
#		define DLL_STUFF	extern "C" __declspec(dllexport)
#	else
#		define DLL_STUFF	extern "C" __declspec(dllimport)
#	endif
#endif

class CDES;

DLL_STUFF CDES* CreateDES(void);
DLL_STUFF void DestroyDES(CDES* pObj);
DLL_STUFF int Decrypt(CDES* pObj,const char *szData, int nDataLen, const char *szKey, int nKeyLen, char *pResult);
DLL_STUFF int Encrypt(CDES* pObj,const char *szData, int nDataLen, const char *szKey, int nKeyLen, char *pResult);

