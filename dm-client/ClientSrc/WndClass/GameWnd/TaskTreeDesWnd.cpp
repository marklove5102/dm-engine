#include "TaskTreeDesWnd.h"
#include "Global/Interface/AudioInterface.h"
#include "GameControl/GameControl.h"
#include "GameData/MapFinder.h"
#include "GameMap/GameMap.h"
#include "GameData/ItemCfgMgr.h"
#include "GameData/MsgBoxMgr.h"
INIT_WND_POSX(CTaskTreeDesWnd,POS_AUTO,POS_AUTO)

CTaskTreeDesWnd::CTaskTreeDesWnd(void)
{
	m_iIndex = 17675;
}

CTaskTreeDesWnd::~CTaskTreeDesWnd(void)
{
}

void CTaskTreeDesWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}
}

bool CTaskTreeDesWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CTaskTreeDesWnd::OnCreate()
{
	SetCloseButton(208,3,80);
	m_pMVTaskDes = new CMarkViewer(16,2,0,2);
	m_pMVTaskDes->Create(this,11,17,194,36);
	AddControl(m_pMVTaskDes);

	m_pMVTaskDes->SetTagText(&m_TagText);
}

void CTaskTreeDesWnd::SetDesText(string& str)
{
	if (str.empty()) return;

	m_TagText.Parse(str);
}

bool CTaskTreeDesWnd::OnLeftButtonUp(int iX, int iY)
{
	string strCommand;
	if (m_pMVTaskDes)
		strCommand = m_pMVTaskDes->GetCommand();
	else strCommand.clear();

	m_bClick = false;//否则后面弹出的模式框获得不到焦点
	bool bRet = g_pGameControl->DealGotoCommand(strCommand);	
	if (bRet) return true;

	return CCtrlWindowX::OnLeftButtonUp(iX,iY);
}