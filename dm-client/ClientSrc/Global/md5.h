//md5.h文件

/******************************************************************************
*  Copyright (C) 2000 by Robert Hubley.                                      *
*  All rights reserved.                                                      *
*                                                                            *
*  This software is provided ``AS IS'' and any express or implied            *
*  warranties, including, but not limited to, the implied warranties of      *
*  merchantability and fitness for a particular purpose, are disclaimed.     *
*  In no event shall the authors be liable for any direct, indirect,         *
*  incidental, special, exemplary, or consequential damages (including, but  *
*  not limited to, procurement of substitute goods or services; loss of use, *
*  data, or profits; or business interruption) however caused and on any     *
*  theory of liability, whether in contract, strict liability, or tort       *
*  (including negligence or otherwise) arising in any way out of the use of  *
*  this software, even if advised of the possibility of such damage.         *
*                                                                            *
*****************************************************************************

*MD5.H - header file for MD5C.CPort to Win32 DLL by Robert Hubley 1/5/2000
Original Copyright:Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991.
Allrights reserved.License to copy and use this software is granted provided 
that itis identified as the "RSA Data Security, Inc. MD5 Message-DigestAlgorithm"
in all material mentioning or referencing this softwareor this function.License
is also granted to make and use derivative works providedthat such works are
identified as "derived from the RSA DataSecurity, Inc. MD5 Message-Digest Algorithm"
in all materialmentioning or referencing the derived work.RSA Data Security, Inc.
makes no representations concerning eitherthe merchantability of this software 
or the suitability of thissoftware for any particular purpose. It is provided 
"as is"without express or implied warranty of any kind.These notices must be 
retained in any copies of any part of thisdocumentation and/or software.
*/

/******************************************************************************
*  Copyright (C) 2000 by Robert Hubley.                                      *
*  All rights reserved.                                                      *
*                                                                            *
*  This software is provided ``AS IS'' and any express or implied            *
*  warranties, including, but not limited to, the implied warranties of      *
*  merchantability and fitness for a particular purpose, are disclaimed.     *
*  In no event shall the authors be liable for any direct, indirect,         *
*  incidental, special, exemplary, or consequential damages (including, but  *
*  not limited to, procurement of substitute goods or services; loss of use, *
*  data, or profits; or business interruption) however caused and on any     *
*  theory of liability, whether in contract, strict liability, or tort       *
*  (including negligence or otherwise) arising in any way out of the use of  *
*  this software, even if advised of the possibility of such damage.         *
*                                                                            *
*******************************************************************************/
/******************************************************************************
*   2002-4-18 Modified by Liguangyi.
**   struct MD5_CTX ==> class MD5_CTX.
**   Take off the Globals Functions 
********************************************************************************/

#pragma once

#ifndef _LGY_MD5_H
#define _LGY_MD5_H
#include <string>
#include <tchar.h>
#include <windows.h>/* MD5 Class. */

class MD5_CTX 
{
public:
	MD5_CTX();
	virtual ~MD5_CTX();
	void MD5Update( unsigned char *input, unsigned int inputLen);
	void MD5Final (unsigned char digest[16]);
	void MD5Final32 (unsigned char digest[32]);

private:
	unsigned long int state[4];	/* state (ABCD) */	
	unsigned long int count[2];  /* number of bits,modulo 2^64 (lsb first) */
	unsigned char buffer[64];    /* input buffer */
	unsigned char PADDING[64];   /* What? */

private:
	void MD5Init ();
	void MD5Transform (unsigned long int state[4], unsigned char block[64]);
	void MD5_memcpy (unsigned char* output, unsigned char* input,unsigned int len);
	void Encode (unsigned char *output, unsigned long int *input,unsigned int len);
	void Decode (unsigned long int *output, unsigned char *input, unsigned int len);
	void MD5_memset (unsigned char* output,int value,unsigned int len);
	void MD5_Clear();

public:
	std::string MakePassMD5(std::string m_Input);
	void MakePassMD5(unsigned char* OutputData,unsigned char* InputData,unsigned int InputDataSize);
	void MakePassMD5(unsigned char* OutputData,LPCTSTR FileName);//整个读取,获得16个字节数据,要在外部转换成MD5,每个字节数据转换成2个字节的MD5
	void MD5File32(char* OutputData,LPCTSTR FileName);//分块读取文件,获得32位md5

};

#endif