// usbhidiocDlg.h : header file
//

#if !defined(AFX_USBHIDIOCDLG_H__0B2AAA84_F5A9_11D3_9F47_0050048108EA__INCLUDED_)
#define AFX_USBHIDIOCDLG_H__0B2AAA84_F5A9_11D3_9F47_0050048108EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CUsbhidiocDlg dialog
class UsbClientSDK
{
public:
	static UsbClientSDK * __stdcall GetInInstance();

	// Generated message map functions

	virtual int __stdcall GetCardNo(unsigned char * sCardNo);
	virtual int __stdcall GetPWDAuto(unsigned char * idynpasswd,unsigned char * challenge);
	virtual int __stdcall GetPWDManual(unsigned char * idynpasswd,unsigned char * challenge);
	virtual void __stdcall CreateChallenge(char* sChallenge);

private:
	UsbClientSDK();
	~UsbClientSDK(void);

	static UsbClientSDK * m_instance;
};

#endif // !defined(AFX_USBHIDIOCDLG_H__0B2AAA84_F5A9_11D3_9F47_0050048108EA__INCLUDED_)
