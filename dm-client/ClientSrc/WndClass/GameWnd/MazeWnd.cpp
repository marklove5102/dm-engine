#include "MazeWnd.h"
#include "GameData/OtherData.h"
#include "GameData/MsgBoxMgr.h"
#include "GameControl/GameControl.h"
INIT_WND_POSX(CMazeWnd,POS_AUTO,POS_AUTO)

CMazeWnd::CMazeWnd(void)
{
	m_iIndex = 15905;
}

CMazeWnd::~CMazeWnd(void)
{
}

void CMazeWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	_MazeMap& MapZone = g_OtherData.GetMazeMap();

	int iX = 0,iY = 0;
	for (int i = 0;i<49;i++)
	{
		//计算地图位置
		
		//绘制每个地图块
		LPTexture pTex =  NULL;
		iX =((i%7) * 32) + GetScreenX() + 110;
		iY =((i/7) * 32) + GetScreenY() + 57;
		int iIndex = 15905;

		//绘制黑块
		pTex = g_pTexMgr->GetTexImm(PACKAGE_INTERFACE,15906,EP_UI);
		if (pTex)
			g_pGfx->DrawTextureFX(iX,iY,pTex);

		//计算绘制位置
		if (MapZone.maze[i].byType & 0x01)//地图
		{
			//地图
			BYTE byMap = MapZone.maze[i].byMap;
			int iGateNum = 0;
			bool bArray[4] = {false};
			if (byMap & 0x01)
			{
				iGateNum++;
				bArray[0] = true;
			}
			if (byMap & 0x04)
			{
				iGateNum++;
				bArray[1] = true;
			}
			if (byMap & 0x10)
			{
				iGateNum++;
				bArray[2] = true;
			}
			if(byMap & 0x40)
			{
				iGateNum++;
				bArray[3] = true;
			}

			if(iGateNum == 1)//有四个
			{				
				if (bArray[0]) iIndex += 2;
				else if (bArray[1]) iIndex += 3;
				else if (bArray[2]) iIndex += 4;
				else if (bArray[3]) iIndex += 5;
			}
			else if(iGateNum == 2)
			{
				//有六个
				if(bArray[0] && bArray[1]) iIndex += 6;
				else if(bArray[1] && bArray[2]) iIndex += 7;
				else if(bArray[2] && bArray[3]) iIndex += 8;
				else if(bArray[3] && bArray[0]) iIndex += 9;
				else if(bArray[1] && bArray[3]) iIndex += 10;
				else if(bArray[0] && bArray[2]) iIndex += 11;
			}
			else if(iGateNum == 3)
			{
				//有四个
				if (!bArray[0]) iIndex += 12;
				else if (!bArray[1]) iIndex += 13;
				else if (!bArray[2]) iIndex += 14;
				else if (!bArray[3]) iIndex += 15;
			}
			else if(iGateNum == 4)//只有一个
			{				
				iIndex += 16;
			}
		}

		if (15905 < iIndex)
		{
			pTex = g_pTexMgr->GetTexImm(PACKAGE_INTERFACE,iIndex,EP_UI);
			if (pTex)
				g_pGfx->DrawTextureFX(iX,iY,pTex);
		}

		if (MapZone.maze[i].byType & 0x02)//小宝
			iIndex = 15922;
		else if (MapZone.maze[i].byType & 0x04)//大宝
			iIndex = 15923;
		else if (MapZone.maze[i].byType & 0x08)//问号
			iIndex = 15925;

		if (iIndex == 15922 || iIndex == 15923 || iIndex == 15925)
		{
			pTex = g_pTexMgr->GetTexImm(PACKAGE_INTERFACE,iIndex,EP_UI);
			if (pTex)
				g_pGfx->DrawTextureFX(iX,iY,pTex);
		}

		//如果自已在则画自己
		if (MapZone.byPlayerIndex == i)
		{
			//绘制自己
			pTex = g_pTexMgr->GetTexImm(PACKAGE_INTERFACE,15924,EP_UI);
			if (pTex)
				g_pGfx->DrawTextureFX(iX,iY,pTex);
		}
	}	
}
void CMazeWnd::OnCreate()
{
	SetCloseButton(419,5);
	m_pExitButton = new CCtrlButton();
	AddControl(m_pExitButton);
	m_pExitButton->CreateEx(this,340,250,15926,15927,15928);
}

bool CMazeWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	if (pControl == m_pExitButton)
	{
		string strTemp = "你将被传送回英雄岛的迷仙阵入口处。";
		g_MsgBoxMgr.PopOkCancelBox(strTemp.c_str(),MSG_CTRL_EXIT_MAZE_OK,0,NULL,MSG_CTRL_EXIT_MAZE_CANCEL);
		OnClickCloseButton();
		return true;
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}