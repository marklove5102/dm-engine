#include "CtrlTreeView.h"
#include "Global/Global.h"
//TreeViewNode类
CTreeViewNode::CTreeViewNode(CTreeViewNode* pParent,CCtrlTreeView* pTreeView,LPCTSTR szText,DWORD dwID,WORD wTexID,WORD wTexExID,bool bLeaf,bool bIncreaseTex,bool bIncreateTexEx)
{
	m_dwID = dwID;//唯一标识

	m_strText = szText!= NULL?szText:"";
	m_pParent = pParent;
	m_bUnBind = false;

	m_bLeaf = bLeaf;
	m_bSelected = false;
	m_bCheckOn	= false;
	m_dwFocusImg = 0;
	m_dwNormalColor = 0xFFFFFFFF;
	m_dwSelColor = 0xFFFF0000;
	
	m_pButton = new CCtrlButton;
	m_wTexID = wTexID;
	m_wTexExID = wTexExID;
	if (pParent && !pParent->IsUnBind())
	{
		m_pButton->SetShow(false);
		m_pButton->SetEnable(false);
	}

	m_bIncreaseTex = bIncreaseTex;
	m_bIncreateTexEx = bIncreateTexEx;
	m_pTreeView = pTreeView;
	if (m_pButton) 
	{
		if (wTexID != 0)
		{
			if (bIncreaseTex)
				 m_pButton->CreateEx(pTreeView,0,0,wTexID,wTexID + 1,wTexID + 2);
			else m_pButton->CreateEx(pTreeView,0,0,wTexID,wTexID,wTexID);
		}
		else m_pButton->CreateEx(pTreeView,0,0,wTexID,wTexID,wTexID);
	}
	pTreeView->AddControl(m_pButton);
}

CTreeViewNode::~CTreeViewNode()
{
	VChildren::iterator itor=m_vChildren.begin();
	while (itor != m_vChildren.end())
	{
		CTreeViewNode* pTreeNode=*itor;		
		m_vChildren.erase(itor);
		delete pTreeNode;
		itor = m_vChildren.begin();
	}
}

void CTreeViewNode::SetUnBind(bool bUnBind)
{
	m_bUnBind = bUnBind;
	if (m_bUnBind)
	{
		if (m_wTexExID != 0)
		{
			if (m_bIncreateTexEx)
				 m_pButton->ReloadTex(m_wTexExID,m_wTexExID + 1,m_wTexExID + 2);
			else m_pButton->ReloadTex(m_wTexExID,m_wTexExID,m_wTexExID);
		}
	}
	else
	{
		if (m_wTexID != 0)
		{
			if (m_bIncreaseTex)
				 m_pButton->ReloadTex(m_wTexID,m_wTexID + 1,m_wTexID + 2);
			else m_pButton->ReloadTex(m_wTexID,m_wTexID,m_wTexID);
		}
	}
}

void CTreeViewNode::SetChecked(bool b)
{
	m_bCheckOn = b;
	if (!m_pButton) return;

	if (m_bCheckOn)
		 m_pButton->ReloadTex(m_wTexExID,m_wTexExID,m_wTexExID);
	else m_pButton->ReloadTex(m_wTexID,m_wTexID,m_wTexID);
}
void CTreeViewNode::InsertChildNode(CTreeViewNode* pNode)
{
	if (pNode) m_vChildren.push_back(pNode);
}

void CTreeViewNode::ShowButton(bool bShow)
{
	if (m_pButton) 
	{
		m_pButton->SetEnable(bShow);
		m_pButton->SetShow(bShow);
	}
}

bool CTreeViewNode::DeleteNode(CTreeViewNode* pTreeNode)
{
	if (!pTreeNode) return false;
	
	VChildren::iterator it = m_vChildren.begin();
	while (it != m_vChildren.end())
	{
		if (pTreeNode == *it)
		{
			VChildren& vchildren = pTreeNode->GetChildrenArray();
			for (int i = 0;i < vchildren.size();i++)
			{
				DeleteNode(vchildren[i]);
			}

			m_vChildren.erase(it);
			delete pTreeNode;
			return true;
		}
		it++;
	}
		
	return false;
}

int CTreeViewNode::GetNodeRows()
{
	if (IsLeaf())	return 1;
	int iCount = 1;
	if(IsUnBind())
	{
		VChildren::iterator itor=m_vChildren.begin();				
		while(itor!=m_vChildren.end())
		{
			CTreeViewNode* pTreeItem=*itor;
			iCount+= pTreeItem->GetNodeRows();
			itor++;
		}		
	}

	return iCount;
}

CTreeViewNode* CTreeViewNode::FindNode(CCtrlButton* pBtn)
{
	if (pBtn == NULL) return NULL;

	if (pBtn == m_pButton) return this;

	CTreeViewNode* pNode = NULL;
	VChildren::iterator it = m_vChildren.begin();
	while(it != m_vChildren.end())
	{
		pNode = (*it)->FindNode(pBtn);
		if (pNode) return pNode;
		else it++;
	}

	return NULL;
}

CTreeViewNode* CTreeViewNode::FindNode(DWORD dwID)
{
	if (dwID == 0) return NULL;

	if (m_bLeaf && dwID == LOWORD(m_dwID)) return this;

	CTreeViewNode* pNode = NULL;
	VChildren::iterator it = m_vChildren.begin();
	while(it != m_vChildren.end())
	{
		pNode = (*it)->FindNode(dwID);
		if (pNode) return pNode;
		else it++;
	}

	return NULL;
}

CTreeViewNode* CTreeViewNode::FindNode(LPCTSTR szText)
{
	if (szText == NULL) return NULL;

	if (m_strText.compare(szText) == 0)
		return this;

	VChildren::iterator it = m_vChildren.begin();
	while (it != m_vChildren.end())
	{
		CTreeViewNode* pTreeNode = *it;
		if (pTreeNode->FindNode(szText)) return *it;
		else it++;
	}

	return NULL;
}

bool CTreeViewNode::OnClickButton(bool bShow)
{
	if (!m_bLeaf)
	{
		m_bUnBind = !m_bUnBind;
		if (m_bUnBind)
		{
			if (m_bIncreateTexEx)
				 m_pButton->ReloadTex(m_wTexExID,m_wTexExID + 1,m_wTexExID + 2);
			else m_pButton->ReloadTex(m_wTexExID,m_wTexExID,m_wTexExID);
		}
		else
		{
			if (m_bIncreaseTex)
				 m_pButton->ReloadTex(m_wTexID,m_wTexID,m_wTexID);
			else m_pButton->ReloadTex(m_wTexID,m_wTexID + 1,m_wTexID + 2);
		}

		VChildren::iterator itor=m_vChildren.begin();
		while (itor != m_vChildren.end())
		{
			(*itor)->m_pButton->SetEnable(m_bUnBind);
			(*itor)->m_pButton->SetShow(m_bUnBind);
			itor++;
		}
		return true;
	}
	else
	{
		m_pTreeView->GetParent()->Msg(MSG_CTRL_TASK_WND,0xFFFFFFFE,(CControl*)this);
		return false;
	}
}

void CTreeViewNode::OnTracked()
{
	SetChecked(!m_bCheckOn);
	if (m_bCheckOn)
	{
		CTreeViewNode* pSelNode = m_pTreeView->GetSelectedNode();
		if (pSelNode) pSelNode->SetSelected(false);

		m_pTreeView->SetSelectedNode(this);	
		m_bSelected = true;
	}
}
bool CTreeViewNode::OnClickTask(int iLeft,int iTop,int iCurRow,int iStartRow)
{
	if(!m_bLeaf || g_pInput->IsLeftButton()) return false;	

	int iX,iY;
	m_pTreeView->GetClickXY(iX,iY);

	int iBottom = iTop + FONTSIZE_SMALL;
	int iRight = iLeft + m_strText.length()/2*FONTSIZE_SMALL;
	if(iX > iLeft && iX < iRight && iY > iTop && iY < iBottom)
	{
		CTreeViewNode* pSelNode = m_pTreeView->GetSelectedNode();
		if (pSelNode) pSelNode->SetSelected(false);

		m_pTreeView->SetSelectedNode(this);	
		m_bSelected = true;

		m_pTreeView->ClearClickXY();

		//向父窗口发送数据
		m_pTreeView->GetParent()->Msg(MSG_CTRL_TASK_WND,0xFFFFFFFF,(CControl*)this);
	}

	m_pTreeView->GetMouseXY(iX,iY);
	if(iX > iLeft && iX < iRight && iY > iTop && iY < iBottom)
		return true;

	return false;
}

//在绘制的时候修改位置
bool CTreeViewNode::Draw(int iX,int iY,int& iStartRow,int& iCurRow,int iMaxRow)
{
	//先绘制其本身，再绘制其子孙结点

	if (iCurRow >= iMaxRow + iStartRow || iStartRow > iCurRow)
	{
		m_pButton->SetShow(false);
		m_pButton->SetEnable(false);
		if (iStartRow > iCurRow) iStartRow--;
	}
	else
	{
		m_pButton->SetShow(true);
		m_pButton->SetEnable(true);

		if (iCurRow >= iStartRow && m_strText.length() > 0)
		{
			int iLeft = iX + 30;
			int iTop  = iY + iCurRow * m_pTreeView->GetRowHigh();

			DWORD dwColor = m_dwNormalColor;
			
			if(OnClickTask(iLeft,iTop,iCurRow,iStartRow))
				dwColor = m_dwSelColor;				
			if (m_bSelected)
			{
				LPTexture pTex = NULL;
				WORD wBar = m_pTreeView->GetSelectedBar();
				
				if (wBar) pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,wBar,EP_UI);

				if(pTex) g_pGfx->DrawTextureNL(iLeft - 25,iTop - 3,pTex);
				else     g_pGfx->DrawFillRect(iLeft - 3,iTop - 1,m_pTreeView->GetSelLength(),15,0xFF2F8405);
			}

			g_pFont->DrawText(iLeft,iTop,m_strText.c_str(),dwColor);
			m_pButton->Move(iX - m_pTreeView->GetScreenX(),iTop - m_pTreeView->GetScreenY());
		}

		iCurRow++;
	}

	if (m_bUnBind)
	{
		//如果是打开状态，则要绘制相关数据
		VChildren::iterator it = m_vChildren.begin();
		while (it != m_vChildren.end())
		{
			(*it)->Draw(iX + 15,iY,iStartRow,iCurRow,iMaxRow);
			it++;
		}
	}

	return true;
}

//TreeView类
CCtrlTreeView::CCtrlTreeView(void)
{
	m_iClickX = -1000;
	m_iClickY = -1000;
	m_iMouseX = -1000;
	m_iMouseY = -1000;
	m_iLastPos = -1;
	m_iRowHigh = 18;
	m_pRoot = NULL;
	m_pScroll = NULL;
	m_iShowMaxRows = 0;
	m_iSelLen = 192;
	m_pSelectedNode = NULL;
	m_wSelectedBar = 0;
}

CCtrlTreeView::~CCtrlTreeView(void)
{
	SAFE_DELETE(m_pRoot);
}

bool CCtrlTreeView::Create(CControl * pParent,int l,int t,int r,int b,int iRows)
{
	if (iRows <= 0) return false;
	CControl::Create(pParent,l,t,r - l,b - t,CTRL_STYLE_BACKMODE_NODRAW,0x00FFFFFF,NULL);

	m_iShowMaxRows = iRows;
	return true;
}

void CCtrlTreeView::OnCreate(void)
{
	m_pScroll = new CCtrlScroll();
	AddControl(m_pScroll);
	m_pScroll->CreateEx(this,m_iWidth - 14,0,14,m_iHeight);
}

bool CCtrlTreeView::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	if(IsSonCtrl(pControl))//是子控件
	{			
		CTreeViewNode* pTreeNode = m_pRoot->FindNode((CCtrlButton*)pControl);
		if (pTreeNode)
		{
			if(pTreeNode->OnClickButton())
				 RefreshControl();

			return true;
		}
	}

	return CControlContainer::Msg(dwMsg,dwData,pControl);
}

void CCtrlTreeView::Draw()
{
	if(!m_pRoot) return;

	int iCurRow = 0;
	int iStartRow = m_pScroll->GetPos();
	VChildren& vchildren = m_pRoot->GetChildrenArray();
	VChildren::iterator it = vchildren.begin();

	while (it != vchildren.end())
	{
		if((*it)->Draw(m_iScreenX,m_iScreenY,iStartRow,iCurRow,m_iShowMaxRows))
			 it++;
		else break;
	}

	CControlContainer::Draw();
}
bool CCtrlTreeView::OnMouseMove(int iX,int iY)
{	
	return CControlContainer::OnMouseMove(iX,iY);
}

bool CCtrlTreeView::OnWheel(int iWheel)
{
	if (m_pScroll)
		return m_pScroll->OnWheel(iWheel);

	return CControlContainer::OnWheel(iWheel);
}

bool CCtrlTreeView::OnLeftButtonDown(int iX,int iY)
{
	return CControlContainer::OnLeftButtonDown(iX,iY);
}

bool CCtrlTreeView::OnLeftButtonUp(int iX,int iY)
{
	m_iClickX = m_iScreenX + iX;
	m_iClickY = m_iScreenY + iY;

	return CControlContainer::OnLeftButtonUp(iX,iY);
}

bool CCtrlTreeView::OnLeftButtonDClick(int iX,int iY)
{
	return CControlContainer::OnLeftButtonDClick(iX,iY);
}

void CCtrlTreeView::AddScroll(int iX,int iY,int iW,int iH,int iScrollIdx)
{
	if(m_pScroll) RemoveControl((CControl**)&m_pScroll);

	m_pScroll = new CCtrlScroll();
	AddControl(m_pScroll);
	m_pScroll->Create(this,iX,iY,iW,iH,iScrollIdx);	
	m_pScroll->SetStep(1);
}

void CCtrlTreeView::RefreshControl()
{
	//有多少行,当前行所在位置	
	if (!m_pRoot)
	{
		m_iTreeRows = 0;
		return;
	}

	m_iTreeRows = m_pRoot->GetNodeRows();
	m_iTreeRows--;
	m_pScroll->SetRange(m_iTreeRows);
}