#pragma once

//辅助工具公共部分

#include "Global/Global.h"

class CAIAutoMgr
{
public:
	CAIAutoMgr(void);
	~CAIAutoMgr(void);

	void  LaunchWaigua();   //开始游戏
	void  DoLoop();
	void  ExitWaigua();		//退出外挂
	void  ExtraDraw();
	void  AutoSay();
	BOOL  DoAccelKey(WORD wKey);
	void  DealWithHurted(int hp,int hpmax);
	void  SetDrawInfoXY(int x,int y){m_iDrawInfoX = x;m_iDrawInfoY = y;}
	bool  IsEnalbeWaiGua(){return m_bEnableWaigua;}
	void  SetEnalbeWaiGua(bool b){m_bEnableWaigua = b;}
private:
	void  DrawSysInfo();
	void  DrawLockInfo();
	void  DrawMagicTime();
	void  AutoMoGong(); //自动点魔宫

	bool  m_bEnableWaigua; //开启了辅助工具
	DWORD m_dwLastSayTime;
	int   m_iDrawInfoX,m_iDrawInfoY;
};

extern CAIAutoMgr g_AIAutoMgr;