#pragma once
//录像及播放管理
#include "Global/Globaldefine.h"
#include "Global/Global.h"

#ifdef ENABLE_REPLAY

struct stFrameMsg
{
	DWORD dwFrame;
	string msg;
};

struct stFrameInput
{
	DWORD dwFrame;
	DWORD msg;	
	DWORD wParam;
	DWORD lParam;
};

struct stKeyState
{
	DWORD dwFrame;
	BYTE byType;		//1shift2ctrl3alt4escape
	BYTE byState;		//0松开1按下
};

class CReplayManager
{
public:
	CReplayManager(void);
	~CReplayManager(void);
	void SaveMsg(DWORD dwFrameCount,WORD wBufSize,const char *buf);
	void SaveInput(DWORD dwFrameCount,DWORD dwMsg,DWORD wParam,DWORD lParam);
	void SaveKeyState(DWORD dwFrameCount,BYTE byType,bool bKeyDown);
	void SaveStartTime(DWORD dwTime);
	DWORD GetStartTime(){ return m_dwStartTime; }

	bool IsReplayShiftKeyDown(){ return m_bShift; }
	bool IsReplayCtrlKeyDown(){ return m_bCtrl; }
	bool IsReplayAltKeyDown(){ return m_bAlt; }
	bool IsReplayEscapeKeyDown(){ return m_bEscape; }
	void SetReplayKeyState(BYTE byType,bool bKeyDown);

	void Replay(DWORD dwFrameCount);

	bool IsInReplay(){ return m_bReplay; }
	bool IsInRecord(){return m_bRecord;}
	bool StartReplay(bool b);
	bool StartRecord(bool b);
	bool IsReplayEnd(){ return m_bReplayEnd; }
	map<string,BYTE> &GetRoleConfigDirMap() { return m_MRoleConfigDir; }
private:
	bool g_bReplay;

	vector<stFrameMsg> m_pMsgBuf;//协议队列
	vector<stFrameInput> m_pInputBuf;//消息队列
	vector<stKeyState> m_pKeyState;//键盘状态变化队列

	DWORD m_dwStartTime;//录制开始时的系统时间

	FILE* m_pMsgReplay;//协议记录文件指针
	FILE* m_pInputReplay;//消息记录文件指针
	FILE* m_pKeyStateReplay;//键盘状态记录文件指针
	FILE* m_pOtherDataReplay;//录像时的系统时间等数据

	bool m_bShift;//shift是否按下
	bool m_bCtrl;//ctrl是否按下
	bool m_bAlt;//alt是否按下
	bool m_bEscape;//esc是否按下

	bool m_bReplay;//是否正在播放
	bool m_bRecord;//是否正在录制
	bool m_bReplayEnd;//是否播放结束
	bool m_bHaveCopyConfigFile;//是否已经复制过配置文件
	map<string,BYTE> m_MRoleConfigDir;//复制过配置文件的角色的配置文件所在目录列表

	void CloseOpenFile();

};

extern CReplayManager g_ReplayManager;

#endif
