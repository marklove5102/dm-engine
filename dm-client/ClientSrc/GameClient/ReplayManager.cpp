#include "ReplayManager.h"
#include "GameControl/GameControl.h"
#include "BaseClass/Misc/Input.h"

#ifdef ENABLE_REPLAY

CReplayManager g_ReplayManager;

CReplayManager::CReplayManager(void)
{
	m_bReplay = false;
	m_bRecord = false;

	m_bShift = false;	
	m_bCtrl = false;
	m_bAlt = false;
	m_bEscape = false;
	m_bReplayEnd = false;
}

CReplayManager::~CReplayManager(void)
{
	CloseOpenFile();
}

void CReplayManager::CloseOpenFile()
{
	if (m_pMsgReplay)
	{
		fclose(m_pMsgReplay);
	}
	if (m_pInputReplay)
	{
		fclose(m_pInputReplay);
	}
	if (m_pKeyStateReplay)
	{
		fclose(m_pKeyStateReplay);
	}
	if (m_pOtherDataReplay)
	{
		fclose(m_pOtherDataReplay);
	}
}

bool CReplayManager::StartReplay(bool b)
{
	if(b)
	{
		GetGameDataDir();
		char szTemp[MAX_PATH] = {0};

		sprintf(szTemp,"%s\\MsgLog.txt",g_strReplayDir);
		m_pMsgReplay = fopen(szTemp,"rb");
		sprintf(szTemp,"%s\\InputLog.txt",g_strReplayDir);
		m_pInputReplay = fopen(szTemp,"rb");
		sprintf(szTemp,"%s\\KeyStateLog.txt",g_strReplayDir);
		m_pKeyStateReplay = fopen(szTemp,"rb");
		sprintf(szTemp,"%s\\OtherDataLog.txt",g_strReplayDir);
		m_pOtherDataReplay = fopen(szTemp,"rb");

		if (m_pMsgReplay && m_pInputReplay && m_pKeyStateReplay && m_pOtherDataReplay)
		{
			m_bReplay = b;
		}
		else 
		{
			return false;
		}

		DWORD dwFrameCount;
		WORD wMsgLen;

		char msgbuf[60 * 1024] = "";

		m_pMsgBuf.clear();
		while(fread(&dwFrameCount,4,1,m_pMsgReplay))
		{
			stFrameMsg frameMsg;
			fread(&wMsgLen,2,1,m_pMsgReplay);
			fread(msgbuf,wMsgLen,1,m_pMsgReplay);
			frameMsg.msg.assign(msgbuf,wMsgLen);
			frameMsg.dwFrame = dwFrameCount;
			m_pMsgBuf.push_back(frameMsg);
		}

		m_pInputBuf.clear();
		while(fread(&dwFrameCount,4,1,m_pInputReplay))
		{
			stFrameInput frameInput;
			frameInput.dwFrame = dwFrameCount;
			fread(&frameInput.msg,4,1,m_pInputReplay);
			fread(&frameInput.wParam,4,1,m_pInputReplay);
			fread(&frameInput.lParam,4,1,m_pInputReplay);
			m_pInputBuf.push_back(frameInput);
		}

		m_pKeyState.clear();
		while(fread(&dwFrameCount,4,1,m_pKeyStateReplay))
		{					
			stKeyState keyState;
			keyState.dwFrame = dwFrameCount;
			fread(&keyState.byType,1,1,m_pKeyStateReplay);
			fread(&keyState.byState,1,1,m_pKeyStateReplay);					
			m_pKeyState.push_back(keyState);
		}

		fread(&m_dwStartTime,4,1,m_pOtherDataReplay);
	}
	else if (m_bReplay)
	{
		CloseOpenFile();
		m_pMsgBuf.clear();
		m_pInputBuf.clear();
		m_pKeyState.clear();
		m_bReplay = b;
	}

	m_bReplayEnd = false;
	return true;
}

bool CReplayManager::StartRecord(bool b)
{
	GetGameDataDir();
	char szTemp[MAX_PATH] = {0};

	sprintf(szTemp,"%s",g_strReplayDir);
	mkdir(szTemp);

	if (b)
	{
		sprintf(szTemp,"%s\\MsgLog.txt",g_strReplayDir);
		m_pMsgReplay = fopen(szTemp,"wb");	
		sprintf(szTemp,"%s\\InputLog.txt",g_strReplayDir);
		m_pInputReplay = fopen(szTemp,"wb");
		sprintf(szTemp,"%s\\KeyStateLog.txt",g_strReplayDir);
		m_pKeyStateReplay = fopen(szTemp,"wb");
		sprintf(szTemp,"%s\\OtherDataLog.txt",g_strReplayDir);
		m_pOtherDataReplay = fopen(szTemp,"wb");
	
		if (m_pMsgReplay && m_pInputReplay && m_pKeyStateReplay && m_pOtherDataReplay)
		{
			m_bRecord = b;
		}
		else 
		{
			return false;
		}

		//¸´ÖÆÅäÖÃÎÄ¼þ
		sprintf(szTemp,"%s\\config",g_strReplayDir);
		mkdir(szTemp);

	}
	else if(m_bRecord)
	{
		CloseOpenFile();
		m_bRecord = b;
	}	

	return true;
}

void CReplayManager::SaveMsg( DWORD dwFrameCount,WORD wBufSize,const char *buf )
{
	if (!m_bRecord)
	{
		return;
	}

	fwrite(&dwFrameCount,4,1,m_pMsgReplay);	
	fwrite(&wBufSize,2,1,m_pMsgReplay);
	fwrite(buf,wBufSize,1,m_pMsgReplay);
	fflush(m_pInputReplay);
}

void CReplayManager::SaveInput( DWORD dwFrameCount,DWORD dwMsg,DWORD wParam,DWORD lParam )
{
	if (!m_bRecord)
	{
		return;
	}

	fwrite(&dwFrameCount,4,1,m_pInputReplay);
	fwrite(&dwMsg,4,1,m_pInputReplay);
	fwrite(&wParam,4,1,m_pInputReplay);	
	fwrite(&lParam,4,1,m_pInputReplay);	
	fflush(m_pInputReplay);
}

void CReplayManager::SaveKeyState( DWORD dwFrameCount,BYTE byType,bool bKeyDown )
{
	if (!m_bRecord)
	{
		return;
	}

	SetReplayKeyState(byType,bKeyDown);

	fwrite(&dwFrameCount,4,1,m_pKeyStateReplay);	
	fwrite(&byType,1,1,m_pKeyStateReplay);	

	BYTE byState = bKeyDown ? 1 : 0;
	fwrite(&byState,1,1,m_pKeyStateReplay);
	fflush(m_pInputReplay);
}

void CReplayManager::Replay(DWORD dwFrameCount)
{
	if (m_bReplayEnd)
	{
		return;
	}

	if (m_pMsgBuf.empty() && m_pKeyState.empty() && m_pInputBuf.empty())
	{
		m_bReplayEnd = true;
		return;
	}

	while(m_pMsgBuf.size())
	{
		if(dwFrameCount >= m_pMsgBuf[0].dwFrame)
		{
			//g_pGameControl->GCL_DoMsg(m_pMsgBuf[0].msg.c_str(),m_pMsgBuf[0].msg.size());

			g_pNet->OnReadForReplay(m_pMsgBuf[0].msg.c_str(),m_pMsgBuf[0].msg.size());

			m_pMsgBuf.erase(m_pMsgBuf.begin());
		}
		else
		{
			break;
		}
	}

	while(m_pKeyState.size())
	{
		if(dwFrameCount >= m_pKeyState[0].dwFrame)
		{
			SetReplayKeyState(m_pKeyState[0].byType,(m_pKeyState[0].byState == 1 ? true : false));
			m_pKeyState.erase(m_pKeyState.begin());
		}
		else
		{
			break;
		}
	}			


	while(m_pInputBuf.size())
	{
		if(dwFrameCount >= m_pInputBuf[0].dwFrame)
		{						
			g_pInput->WndMessage(m_pInputBuf[0].msg,m_pInputBuf[0].wParam,m_pInputBuf[0].lParam);
			m_pInputBuf.erase(m_pInputBuf.begin());
		}
		else
		{
			break;
		}
	}
}

void CReplayManager::SetReplayKeyState( BYTE byType,bool bKeyState )
{	
	if(byType == 1)
	{
		m_bShift = bKeyState;
	}
	else if(byType == 2)
	{
		m_bCtrl = bKeyState;
	}
	else if(byType == 3)
	{
		m_bAlt = bKeyState;
	}
	else if(byType == 4)
	{
		m_bEscape = bKeyState;
	}	
}

void CReplayManager::SaveStartTime( DWORD dwTime )
{
	if (!m_bRecord)
	{
		return;
	}

	m_dwStartTime = dwTime;
	fwrite(&dwTime,4,1,m_pOtherDataReplay);
}

#endif
