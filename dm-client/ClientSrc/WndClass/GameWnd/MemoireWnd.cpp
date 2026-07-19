#include "MemoireWnd.h"
#include "GameData/ItemCfgMgr.h"
#include "GameClient/WidgetManager.h"
INIT_WND_POSX(CMemoireWnd,POS_AUTO,POS_AUTO)

CMemoireWnd::CMemoireWnd(void)
{
	m_iIndex = 17623;
	m_iVersion = 2;

	//标记是否是群英模式帮助[2/25/2010 dujun]
	m_selectedNode = sm_dwWindowType & 0xff00;
	
	if(sm_dwWindowType == 2)
	{
		m_TabPage.iCurPage = 1;
	}

	AddTabPage(0,0,MAKELONG(17623,PACKAGE_INTERFACE),0,38,85,86,87,88,"游戏手册",NULL,true,0,0,0);
	AddTabPage(0,0,MAKELONG(17624,PACKAGE_INTERFACE),0,122,85,86,87,88,"升级指南",NULL,true,0,0,0);
}

CMemoireWnd::~CMemoireWnd(void)
{
	g_WidgetMgr.SetShow(EWT_HOTACTIVITY,false);
	g_WidgetMgr.SetFocus(EWT_HOTACTIVITY,false);
}

void CMemoireWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	if(m_TabPage.iCurPage == 0)
	{
		g_WidgetMgr.SetShow(EWT_HOTACTIVITY,false);

		g_pFont->DrawText(m_iScreenX + 173 + 35,m_iScreenY + 9,"游戏手册",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG,DTF_BlackFrame);
	}
	else if(m_TabPage.iCurPage == 1)
	{
		g_pFont->DrawText(m_iScreenX + 173 + 35,m_iScreenY + 9,"升级指南",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG,DTF_BlackFrame);

		if(g_WidgetMgr.IsShow(EWT_HOTACTIVITY))
		{
			g_WidgetMgr.RenderWidget(EWT_HOTACTIVITY);
		}
		else
		{
			g_pFont->DrawText(m_iScreenX + 157, m_iScreenY + 150,"正在加载数据，请稍候......",-1);
		}
	}
}

void CMemoireWnd::OnCreate()
{
	m_pPromptTree = NULL;

	if(m_TabPage.iCurPage == 0)
	{
		g_WidgetMgr.SetShow(EWT_HOTACTIVITY,false);

		m_pPromptTree = new CCtrlTreeView;
		m_pPromptTree->Create(this,14 + 24,39,224,313,15);
		AddControl(m_pPromptTree);
		m_pPromptTree->SetSelLength(130);

		m_pContent = new CMarkViewer(16,20,0,2);
		AddControl(m_pContent);
		m_pContent->Create(this,209 + 30,46,194,262);

		m_pContent->SetTagText(&m_TagText);		

		BuildTree();	
	}
	else if(m_TabPage.iCurPage == 1)
	{
		g_WidgetMgr.LoginHotActivity();
		g_WidgetMgr.MoveWindow(EWT_HOTACTIVITY,m_iScreenX + 42,m_iScreenY + 42,396,272);
	}
	
	m_pCloseButton = new CCtrlButton();
	AddControl(m_pCloseButton);
	m_pCloseButton->CreateEx(this,405 + 22,4,80,81,82);

	CCtrlWindowX::OnCreate();

	SetMask(17623);
}

bool CMemoireWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch (dwMsg)
	{
	case MSG_CTRL_TASK_WND:
		{
			CTreeViewNode* pTreeNode = (CTreeViewNode*)pControl;
			if (!pTreeNode) break;

			DWORD dwID = pTreeNode->GetID();

			_sPromptInfo* pPromptInfo = g_PromptInfoMgr.FindPromptInfo(dwID);
			if (pPromptInfo)
			{
				m_TagText.Clear();
				m_TagText.Parse(pPromptInfo->strDes,0,"\\",32);
			}
		}
		break;
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CMemoireWnd::BuildTree()
{
	CTreeViewNode* pRootNode = new CTreeViewNode(NULL,m_pPromptTree,NULL,0,0,0,false);
	pRootNode->SetUnBind();
	m_pPromptTree->SetRootNode(pRootNode);

	VSPromptInfo& vspi = g_PromptInfoMgr.GetPromptInfo();

	//暂且不管都放上去
	bool bFirst = true;
	VSPromptInfo::iterator it = vspi.begin();
	while (it != vspi.end())
	{
		CTreeViewNode* pNode = new CTreeViewNode(pRootNode,m_pPromptTree,it->strSec.c_str(),it->dwSecID,17640,17641,false,false,false);
		
		pNode->SetUnBind(false); //  [2/2/2010 dujun] 默认不展开

		std::vector<_sPromptInfo>::iterator itP = it->vPromptInfo.begin();

		while(itP != it->vPromptInfo.end())
		{
			CTreeViewNode* pNodeLeaf = new CTreeViewNode(pNode,m_pPromptTree,itP->strTile.c_str(),itP->dwID,0,0,true);			
			pNode->InsertChildNode(pNodeLeaf);

			//  [2/3/2010 dujun] 如果是阵谱信息
			if(m_selectedNode != 0)
			{
				if(itP->dwID == 91)
				{
					pNodeLeaf->SetSelected(true);
					m_pPromptTree->SetSelectedNode(pNodeLeaf);

					_sPromptInfo* pPromptInfo = g_PromptInfoMgr.FindPromptInfo(pNodeLeaf->GetID());
					if (pPromptInfo)
					{
						m_TagText.Clear();
						m_TagText.Parse(pPromptInfo->strDes,0,"\\",32);
					}
					pNode->SetUnBind(true);
				}
			}
			else if (bFirst)
			{
				
				if(itP->dwID == 101)
				{
					pNodeLeaf->SetSelected(true);
					m_pPromptTree->SetSelectedNode(pNodeLeaf);

					_sPromptInfo* pPromptInfo = g_PromptInfoMgr.FindPromptInfo(pNodeLeaf->GetID());
					if (pPromptInfo)
					{
						m_TagText.Clear();
						m_TagText.Parse(pPromptInfo->strDes,0,"\\",32);
					}

					pNode->SetUnBind(true);

					bFirst = false;

				}	
			}

			itP++;
		}

		pRootNode->InsertChildNode(pNode);

		it++;
	}

	m_pPromptTree->RefreshControl();
}

bool CMemoireWnd::OnMouseMove(int iX,int iY)
{
	if(m_TabPage.iCurPage == 0)
	{
		if(m_pPromptTree)
			m_pPromptTree->SetMouseXY(iX + m_iScreenX,iY + m_iScreenY);	
	}
	else if(m_TabPage.iCurPage == 1)
	{
		g_WidgetMgr.MoveWindow(EWT_HOTACTIVITY,m_iScreenX + 42,m_iScreenY + 42);
	}

	return CCtrlWindowX::OnMouseMove(iX,iY);
}

void  CMemoireWnd::OnSetFocus()
{
	g_WidgetMgr.SetFocus(EWT_HOTACTIVITY,true);
}

void CMemoireWnd::OnKillFocus()
{
	g_WidgetMgr.SetFocus(EWT_HOTACTIVITY,false);
}