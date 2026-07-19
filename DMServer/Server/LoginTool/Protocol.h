// Protocol.h - 传奇世界协议编解码（与服务器 _CodeGameCode/_UnGameCode 一致）
// 从 LoginerDlg.cpp 提取，供 LoginWnd 和 RegisterWnd 共享
#pragma once
#include "stdafx.h"

namespace protocol {

// ========== 协议命令码 ==========
constexpr WORD CM_IDPASSWORD      = 2001;
constexpr WORD CM_ADDNEWUSER      = 2002;
constexpr WORD CM_CHANGEPASSWORD  = 2003;
constexpr WORD SM_NEWID_SUCCESS   = 504;
constexpr WORD SM_NEWID_FAIL      = 505;
constexpr WORD SM_REGISTERACCOUNTOK   = 0x01F8;
constexpr WORD SM_REGISTERACCOUNTFAIL = 0x01F9;
constexpr WORD SM_CERTIFICATION_SUCCESS = 500;
constexpr WORD SM_CERTIFICATION_FAIL    = 501;

// ========== 6-bit 编码（与服务器 _CodeGameCode 完全一致） ==========
inline int CodeGameCode(const BYTE* pIn, int size, BYTE* pOut)
{
	BYTE b1 = 0, bcal = 0;
	BYTE bflag1 = 0, bflag2 = 0;
	int i = 0;
	int iptr = 0;
	int optr = 0;
	while (iptr < size)
	{
		b1 = pIn[iptr++] ^ 0xeb;
		if (i < 2)
		{
			bcal = b1;
			bcal >>= 2;
			bflag1 = bcal;
			bcal &= 0x3c;
			b1 &= 3;
			bcal |= b1;
			bcal += 0x3b;
			pOut[optr++] = (char)bcal;
			bflag2 = (bflag1 & 3) | (bflag2 << 2);
		}
		else
		{
			bcal = b1;
			bcal &= 0x3f;
			bcal += 0x3b;
			pOut[optr++] = (char)bcal;
			b1 >>= 2;
			b1 &= 0x30;
			b1 |= bflag2;
			b1 += 0x3b;
			pOut[optr++] = (char)b1;
			bflag2 = 0;
		}
		i++;
		i %= 3;
	}
	pOut[optr] = 0;
	if (i == 0) return optr;
	bcal = bflag2 + 0x3b;
	pOut[optr++] = (char)bcal;
	return optr;
}

// ========== 6-bit 解码（与服务器 _UnGameCode 完全一致） ==========
inline int UnGameCode(const char* pIn, BYTE* pOut)
{
	int ilen = (int)strlen(pIn);
	int iptr = 0;
	int optr = 0;
	BYTE b1, b2, b3, b4;
	int i = 0;
	for (i = 0; i < ilen / 4; i++)
	{
		b1 = pIn[iptr++] - 0x3b;
		b2 = pIn[iptr++] - 0x3b;
		b3 = pIn[iptr++] - 0x3b;
		b4 = pIn[iptr++] - 0x3b;
		pOut[optr++] = ((b1 & 3) | ((b1 & 0x3c) << 2) | (b4 & 0x0c)) ^ 0xeb;
		pOut[optr++] = ((b2 & 3) | ((b2 & 0x3c) << 2) | ((b4 & 0x03) << 2)) ^ 0xeb;
		pOut[optr++] = ((b3 & 0x3f) | ((b4 & 0x30) << 2)) ^ 0xeb;
	}
	ilen -= i * 4;
	if (ilen == 2)
	{
		b1 = pIn[iptr++] - 0x3b;
		b2 = pIn[iptr++] - 0x3b;
		pOut[optr++] = ((b1 & 3) | ((b1 & 0x3c) << 2) | ((b2 & 0x03) << 2)) ^ 0xeb;
	}
	else if (ilen == 3)
	{
		b1 = pIn[iptr++] - 0x3b;
		b2 = pIn[iptr++] - 0x3b;
		b3 = pIn[iptr++] - 0x3b;
		pOut[optr++] = ((b1 & 3) | ((b1 & 0x3c) << 2) | (b3 & 0x0c)) ^ 0xeb;
		pOut[optr++] = ((b2 & 3) | ((b2 & 0x3c) << 2) | ((b3 & 0x03) << 2)) ^ 0xeb;
	}
	pOut[optr] = 0;
	return optr;
}

// 编码消息头（TDefaultMessage: 12字节）
inline std::string EncodeMessage(WORD wCmd, int nRecog, WORD wParam, WORD wTag, WORD wSeries)
{
	char buf[12] = {};
	memcpy(buf, &nRecog, 4);
	memcpy(buf + 4, &wCmd, 2);
	memcpy(buf + 6, &wParam, 2);
	memcpy(buf + 8, &wTag, 2);
	memcpy(buf + 10, &wSeries, 2);

	char encBuf[256];
	int encLen = CodeGameCode((BYTE*)buf, 12, (BYTE*)encBuf);
	return std::string(encBuf, encLen);
}

// 编码字符串
inline std::string EncodeString(const std::string& str)
{
	if (str.empty()) return "";
	char encBuf[4096];
	int encLen = CodeGameCode((BYTE*)str.c_str(), (int)str.size(), (BYTE*)encBuf);
	return std::string(encBuf, encLen);
}

} // namespace protocol
