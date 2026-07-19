#ifndef CODER_H_INCLUDE
#define CODER_H_INCLUDE

#ifdef _WIN32
#include <Windows.h>
#else
#include <netinet/in.h>
#endif

#pragma pack(push, 1)

struct STPipeMsgHeader
{
    UINT16  wMsgID;
    UINT32  dwTransID;
};

struct STNetMsgHeader
{
    UINT16  wMsgID;
    UINT16  wDataLen;
};

struct STGateMsgHeader
{
    UINT16  wMsgID;
};

#pragma pack(pop)

const INT32 gnPipeMsgHeaderSize = sizeof(STPipeMsgHeader);

const INT32 gnNetMsgHeaderSize  = sizeof(STNetMsgHeader);

const INT32 gnGateMsgHeaderSize = sizeof(STGateMsgHeader);

const INT32 PACKAGE_LENGTH      = 65536;

inline unsigned long long htonll(unsigned long long number)
{
	return ( htonl( (unsigned long)((number >> 32) & 0xFFFFFFFF)) |
		((unsigned long long) (htonl((unsigned long)(number & 0xFFFFFFFF)))  << 32));
}

inline unsigned long long ntohll(unsigned long long number)
{
	return ( ntohl( (unsigned long)((number >> 32) & 0xFFFFFFFF) ) |
		((unsigned long long) (ntohl((unsigned long)(number & 0xFFFFFFFF)))  << 32));
}

class CNetData
{
public:
	CNetData();
	virtual ~CNetData();

	void Prepare(char *pNetData, int iSize);
	void Reset();

    char* GetData(){ return m_pBuf; }
    int GetDataLen(){ return m_iPos; }

	int AddByte(unsigned char byByte);
	int DelByte(unsigned char &byByte);

	int AddChar(char chChar);
	int DelChar(char &chChar);

	int AddWord(unsigned short wWord);
	int DelWord(unsigned short &wWord);

	int AddShort(short shShort);
	int DelShort(short &shShort);

	int AddDword(unsigned int dwDword);
	int DelDword(unsigned int &dwDword);

	int AddInt(int iInt);
	int DelInt(int &iInt);

	int AddUint64(unsigned long long qwUint64);
	int DelUint64(unsigned long long& qwUint64);

	int AddInt64(long long llInt64);
	int DelInt64(long long &llInt64);

	int AddFloat(float fFloat);//08-10-06 liujunhui add: 增加解析float
	int DelFloat(float &fFloat);//08-10-06 liujunhui add: 增加解析float

	int AddDouble(double dbDouble);		// 09-09-07 cwy add 
	int DelDouble(double &dbDouble);	// 09-09-07 cwy add

	int AddString(const char *pszString, int iSize);
	int DelString(char *pszOut, int iSize);

	int Strnlen(const char *pszString, int iSize);

	int AddBuf(const unsigned char* pbyBuf, int iSize);//09-11-18 cwy add, enhance byte array copy
	int DelBuf(unsigned char* pbyBuf, int iSize);//09-11-18 cwy add, enhance byte array copy

protected:
	char * m_pBuf;
	int m_iSize;
	int m_iPos;
};

#endif
