/*#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class CRsaClient
{
public:
	CRsaClient();
	~CRsaClient();

	bool Init(const char* pPubKeyBuf, int iLen);
	DWORD PubEncrypt(BYTE* from, size_t nFromSize, BYTE* to);
private:
	void* m_pPubRSA; //÷∏’Î
};
*/