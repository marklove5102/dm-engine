#include "CtrlTree.h"
#include "GameData/PhyleData.h"

WORD	CTreeNode::m_swContentCols		= 4;
WORD	CTreeNode::m_swDrawRows			= 0;
WORD	CTreeNode::m_swRowHeigh			= 0;
WORD	CTreeNode::m_swIndent			= 0;
WORD	CTreeNode::m_swColWidth			= 0;
DWORD	CTreeNode::m_sdwRow				= 0;
WORD	CTreeNode::m_swFirstRow			= 0;
WORD	CTreeNode::m_swDrawMaxRows		= 0;
bool	CTreeNode::m_sbShowTips			= true;

CTreeNode::CTreeNode(CCtrlTree * pTree,CTreeNode* pParent,DWORD dwID,LPCTSTR szText,bool bLeaf,bool bShow,bool bFavorite,bool bOfficer)
{
	m_dwID		= dwID;
	m_strText	= szText;
	m_pParent	= pParent;
	m_pButton = NULL;
	m_vChildren.clear();
	m_bUnBind=true;
	m_bLeaf = bLeaf;
	m_bFavorite = bFavorite;
	m_bOfficer = bOfficer;
	m_dwColorTile		= COLOR_TEXT_MAIN_TITLE;
	m_dwColorContent	= 0xFFFFFFFF;
	
	if (!bLeaf)
	{
		m_pButton = new CCtrlButton();
		m_pButton->CreateEx(pTree,0,0,17658,17659,17660);
		m_pButton->SetShow(bShow);
		pTree->AddControl(m_pButton);
	}	
	
	m_pTree = pTree;
}
CTreeNode::~CTreeNode()
{
	DeleteAllNode();	
}

void CTreeNode::SetColor(DWORD dwColorStile,DWORD dwContent)
{
	m_dwColorTile = dwColorStile;
	m_dwColorContent = dwContent;
}

void CTreeNode::SetMaxRowAndCols(WORD wRows,WORD wClos)
{
	m_swDrawMaxRows = wRows + 1;
	m_swContentCols = wClos;
}

void CTreeNode::SetRowColRel(WORD wRowHeigh,WORD wColWidth,WORD wIndent)
{
	m_swRowHeigh = wRowHeigh;
	m_swColWidth = wColWidth;
	m_swIndent   = wIndent;
}

void CTreeNode::InsertChildNode(CTreeNode* pNode)
{
	if (pNode)
		m_vChildren.push_back(pNode);
}

bool CTreeNode::DeleteNode(CTreeNode* pNode)
{
	if(pNode)
	{
		CTreeNode* pParent=pNode->GetParent();
		if(!pParent)
		{
			return false;
		}
		ChildrenArray::iterator itor;
		ChildrenArray& pvSibling = pParent->GetChildrenArray();

		for (itor = pvSibling.begin();itor != pvSibling.end();itor++)
		{
			if (pNode == *itor)
			{
				pvSibling.erase(itor);
				delete pNode;
				return true;
			}
		}		
	}
	return false;
}

void CTreeNode::DeleteAllNode()//删除所有子结点
{
	ChildrenArray::iterator itor=m_vChildren.begin();
	while (itor != m_vChildren.end())
	{
		CTreeNode* pTreeNode=*itor;		
		m_vChildren.erase(itor);
		delete pTreeNode;
		itor = m_vChildren.begin();
	}
}

int CTreeNode::GetNodeRows()
{
	if (IsLeaf())	return 0;
	int iCount = 1;
	if(IsUnBind())
	{
		ChildrenArray::iterator itor=m_vChildren.begin();
		if (itor != m_vChildren.end())
		{
			CTreeNode* pNode = *(itor);
			if (pNode->IsLeaf())
			{
				iCount += (m_vChildren.size() + m_swContentCols - 1)/m_swContentCols;
			}
			else
			{			
				while(itor!=m_vChildren.end())
				{
					CTreeNode* pTreeItem=*itor;
					iCount+= pTreeItem->GetNodeRows();
					itor++;
				}
			}
		}
	}
	return iCount;
}

void CTreeNode::DrawIni(WORD wShowFirstRow)
{
	m_sdwRow = 0;
	m_swDrawRows = 0;
	m_swFirstRow = wShowFirstRow;
}

CTreeNode* CTreeNode::FindNode(DWORD dwID)
{
	if (dwID == m_dwID)
		return this;

	CTreeNode* pRet = NULL;
	ChildrenArray::iterator itor=m_vChildren.begin();
	while (itor != m_vChildren.end())
	{
		CTreeNode* pTreeNode=*itor;		
		pRet = pTreeNode->FindNode(dwID);
		if (pRet)	return pRet;
		else		itor ++;
	}
	return NULL;
}

CTreeNode* CTreeNode::FindNode(LPCTSTR szText)
{
	CTreeNode* pRet = NULL;
	if (szText == NULL) return pRet;

	if (m_strText.compare(szText) == 0)
		return this;

	ChildrenArray::iterator itor=m_vChildren.begin();
	while (itor != m_vChildren.end())
	{
		CTreeNode* pTreeNode=*itor;		
		pRet = pTreeNode->FindNode(szText);
		if (pRet)	return pRet;
		else		itor ++;
	}
	return NULL;
}

CTreeNode* CTreeNode::FindNode(CCtrlButton* pButton)
{
	CTreeNode* pRet = NULL;
	if (pButton == NULL || IsLeaf())
		return pRet;

	if (pButton == GetButton())
		return this;

	ChildrenArray::iterator itor=m_vChildren.begin();
	while (itor != m_vChildren.end())
	{
		CTreeNode* pTreeNode=*itor;
		pRet = pTreeNode->FindNode(pButton);
		if (pRet)	return pRet;
		else		itor ++;
	}
	return pRet;
}

void CTreeNode::OnClickButton(bool bShow)
{
	if (m_bLeaf) return;

	if (m_bUnBind)
			m_pButton->ReloadTex(17654,17655,17656);
	else	m_pButton->ReloadTex(17658,17659,17660);

	SetUnBind(!m_bUnBind);

	m_pButton->SetShow(bShow);
	ChildrenArray::iterator itor=m_vChildren.begin();
	while (itor != m_vChildren.end())
	{
		(*itor)->OnClickButton(false);
		itor ++;
	}
}

void CTreeNode::SetUnBind(bool bUnbind)
{
	m_bUnBind = bUnbind;
}

ChildrenArray* CTreeNode::GetSiblingArray()
{
	ChildrenArray* pRet = NULL;
	if (m_pParent)
		pRet = &(m_pParent->GetChildrenArray());

	return pRet;
}

bool CTreeNode::IsInShowSpace()//是否在显示范围内
{
	bool bRet = false;
	if (m_sdwRow >= 0 &&  m_sdwRow - m_swFirstRow >= 0 && m_sdwRow - m_swFirstRow < m_swDrawMaxRows)
		bRet = true;
	return bRet;
}

void CTreeNode::DrawTipInfo(int iX,int iY,string& strTip)
{
	//绘制提示信息

}

void CTreeNode::Draw(int iX,int iY,int iCurLevel,int& iClickX,int& iClickY)//绘制该结点及该结点下所有的子结点
{
	static CTreeNode* psNode = NULL;

	if (IsLeaf())//叶子结点
	{
		if (m_swDrawRows < m_swDrawMaxRows - 1)
		{
			int iLeft = iX;
			int iTop = iY + m_swDrawRows * (m_swRowHeigh);
			int iDrawX = iLeft;

			LPTexture pTex = NULL;
			if (m_dwID) pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,m_dwID,EP_UI);		
			if (m_bFavorite)
			{
				iDrawX = iLeft + 42;
				g_pFont->DrawText(iLeft,iTop,"亲信:",0xFFFF0000);
				if (pTex) g_pGfx->DrawTextureNL(iLeft + 28,iTop - 2,pTex);
				g_pFont->DrawText(iDrawX,iTop,m_strText.c_str(),m_dwColorContent);
			}
			else
			{
				iDrawX = iLeft + 14;
				if (pTex) g_pGfx->DrawTextureNL(iLeft,iTop - 2,pTex);
				g_pFont->DrawText(iDrawX,iTop,m_strText.c_str(),m_dwColorContent);
			}

			int iBottom = iTop + FONTSIZE_DEFAULT;
			int iRight = iDrawX + m_strText.length()/2*FONTSIZE_DEFAULT;
			if(iClickX > iDrawX && iClickX < iRight && iClickY > iTop && iClickY < iBottom)//点中了一个成员,自动密他
			{
				string strTemp = m_strText;
				strTemp += " ";
				g_pControl->Msg(MSG_CTRL_INSERT_TEXT, 1,(CControl*)strTemp.c_str());
				iClickX = iClickY = -1000;
			}

			if (m_sbShowTips)
			{
				int x,y;				
				g_pInput->GetMousePos(x,y);
				if (x >= iLeft && x <= iLeft + m_strText.length() * 12 && y >= iTop && y <= iTop + 12)
				{
					if (psNode != this)
					{
						psNode = this;
						m_pTree->m_sTip->Clear();
						m_pTree->m_sTip->Move(x - m_pTree->GetScreenX(),y - m_pTree->GetScreenY());
						GuildPhyleEle& guildphyle = g_PhyleData.GetGuildPhyleMap();
						GuildPhyleEle::iterator itMap = guildphyle.find(m_strText);
						if (itMap != guildphyle.end())
						{//有宗派
							m_pTree->m_bShowTips = true;
							char ctemp[128]={0};
							if (itMap->second.byType == 0)
							{
								sprintf(ctemp,"%s宗族成员",itMap->second.strName.c_str());
							}
							else if (itMap->second.byType == 1)
							{
								sprintf(ctemp,"%s宗族宗主",itMap->second.strName.c_str());
							}

							m_pTree->m_sTip->AddText(ctemp);
							m_pTree->m_bShowTips = true;
						}					
					}
					else if (psNode != NULL)
					{
						if (!m_pTree->m_sTip->IsEmpty())
						{
							m_pTree->m_bShowTips = true;
						}						
					}
				}
			}

		}
	}
	else//非叶子结点
	{
		if (IsInShowSpace())
		{
			if (m_swDrawRows < m_swDrawMaxRows - 1)
			{
				m_pButton->Move(iCurLevel*20,m_swDrawRows * (m_swRowHeigh));
				m_pButton->SetShow(true);	
				m_pButton->SetEnable(true);
				g_pFont->DrawText(iX + 20,iY + m_swDrawRows * (m_swRowHeigh),m_strText.c_str(),m_dwColorTile);

				if (m_dwID != 0)
				{
					LPTexture pTex = NULL;
					pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,m_dwID,EP_UI);		
					if (pTex) g_pGfx->DrawTextureNL(iX + 20,iY + m_swDrawRows * (m_swRowHeigh),pTex);
				}
			}
			else
			{
				m_pButton->SetShow(false);
				m_pButton->SetEnable(false);
			}

			m_swDrawRows++;//当前绘制的行
			if (m_swDrawRows >= m_swDrawMaxRows)
				return;
		}
		else
		{
			m_pButton->SetShow(false);
			m_pButton->SetEnable(false);
		}
		m_sdwRow++;//当前绘制走到哪一行
		if (!m_bUnBind)
			return;
		iX += m_swIndent;
		//处理子结点	
		ChildrenArray::iterator itor=m_vChildren.begin();
		if (itor == m_vChildren.end())
			return;

		CTreeNode* pTreeNode=*itor;
		if (pTreeNode->IsLeaf())//如果有一个是叶子，则所有都是叶子结点
		{	
			if (pTreeNode->m_bOfficer)
			{
				int iCols = pTreeNode->m_bOfficer?m_swContentCols/2:m_swContentCols;
				int iWidth = pTreeNode->m_bOfficer?m_swColWidth*2:m_swColWidth;

				for (int i = 0;i<m_vChildren.size();i++)
				{
					if (IsInShowSpace())
					{
						pTreeNode = *(itor + i);
						pTreeNode->Draw(iX,iY,iCurLevel,iClickX,iClickY);

						if (i + 1 <m_vChildren.size())
						{
							pTreeNode = *(itor + i + 1);
							if (!pTreeNode->m_bOfficer)
							{
								pTreeNode->Draw(iX + m_swColWidth*2,iY,iCurLevel,iClickX,iClickY);
								i++;
							}							
						}
						m_swDrawRows++;
					}
					m_sdwRow ++;
					if (m_swDrawRows >= m_swDrawMaxRows)
						return;
				}
			}
			else
			{
				for (int i = 0;i < m_vChildren.size() /m_swContentCols;i++)
				{
					if (IsInShowSpace())
					{
						for (int j = 0;j<m_swContentCols;j++)
						{
							pTreeNode = *(itor + i * m_swContentCols + j);
							pTreeNode->Draw(iX + j * m_swColWidth,iY,iCurLevel,iClickX,iClickY);
						}
						m_swDrawRows ++;
					}
					m_sdwRow++;//当前绘制走到哪一行
					if (m_swDrawRows >= m_swDrawMaxRows)
						return;
				}

				int iLeft = m_vChildren.size() % m_swContentCols;
				itor = m_vChildren.begin() + m_vChildren.size()  - iLeft;			

				if ( iLeft > 0 && IsInShowSpace())
				{
					pTreeNode = *itor;	
					for (int i = 0;i<iLeft;i++)
					{
						pTreeNode->Draw(iX + i*m_swColWidth,iY,iCurLevel,iClickX,iClickY);
						itor++;

						if (itor == m_vChildren.end())
							break;
						pTreeNode = *itor;
					}
					m_swDrawRows++;
				}

				if (iLeft != 0)
				{	
					m_sdwRow++;//当前绘制走到哪一行
					if (m_swDrawRows >= m_swDrawMaxRows)
						return;
				}
			}
		}
		else//子根结点
		{
			for(int i = 0;i<m_vChildren.size();i++)
			{
				pTreeNode->Draw(iX,iY,iCurLevel + 1,iClickX,iClickY);
				
				itor++;
				if (itor != m_vChildren.end())
				{
					pTreeNode = *itor;
				}				
			}
		}				
	}	
}

DTI_IMPLEMENT(CCtrlTree, CControl)
CCtrlTree::CCtrlTree(void)
{
	m_bDrawRoot = false;
	m_pRoot = NULL;
}

CCtrlTree::~CCtrlTree(void)
{
	SAFE_DELETE(m_pRoot)
	SAFE_DELETE(m_sTip)
}

bool CCtrlTree::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	if (dwMsg == MSG_CTRL_BUTTON_CLICK)
	{
		if(IsSonCtrl(pControl))//是子控件
		{			
			 CTreeNode* pTreeNode = m_pRoot->FindNode((CCtrlButton*)pControl);
			 if (pTreeNode)
			 {
				 pTreeNode->OnClickButton();			
				 return true;
			 }
		}	
	}

	return CControlContainer::Msg(dwMsg,dwData,pControl);
}

void CCtrlTree::Draw(int& iClickX,int& iClickY)
{
	if (m_pRoot)
	{
		m_bShowTips = false;
		m_pRoot->DrawIni(m_pScroll->GetPos());
		int iX = GetScreenX();
		int iY = GetScreenY();
		CTreeNode* pNode = NULL;
		ChildrenArray& childArray = m_pRoot->GetChildrenArray();
		for (int i = 0;i<childArray.size();i++)
		{
			pNode = childArray[i];
			pNode->Draw(iX,iY,0,iClickX,iClickY);
		}		
	}

	if (m_bShowTips)
		m_sTip->Draw();

	CControlContainer::Draw();
}

bool CCtrlTree::OnWheel(int iWheel)
{
	m_pScroll->OnWheel(iWheel);
	RefreshTreeRows();
	return true;
}

bool CCtrlTree::OnLeftButtonDown(int iX,int iY)
{
	//处理鼠标左键按下的消息
	return CControlContainer::OnLeftButtonDown(iX,iY);
}

bool CCtrlTree::OnLeftButtonUp(int iX,int iY)
{
	return CControlContainer::OnLeftButtonUp(iX,iY);
}

bool CCtrlTree::OnLeftButtonDClick(int iX,int iY)
{
	return true;
}


bool CCtrlTree::Create(CControl * pParent,int l,int t,int r,int b,int lines,int icolumn,float h)
{
	//初始化树结点相关数据

	CTreeNode::SetMaxRowAndCols(lines,icolumn);
	CTreeNode::SetRowColRel((b-t)/lines,(r - l - 50)/icolumn ,20);

	CControl::Create(pParent,l,t,r - l,b - t,CTRL_STYLE_BACKMODE_NODRAW,0x00FFFFFF,NULL);

	m_sTip = new CParserTip();
	m_sTip->Create(this);
	m_sTip->SetBackTexByID(0x88000000);
	m_sTip->SetFrame(0,0);

	return true;
}

void CCtrlTree::OnCreate()
{
	m_pScroll = new CCtrlScroll();
	AddControl(m_pScroll);
	m_pScroll->CreateEx(this,m_iWidth - 14,0,14,m_iHeight);
}

bool CCtrlTree::OnMouseMove(int iX,int iY)
{
	m_sTip->Move(iX + 10,iY + 10);
	return CControlContainer::OnMouseMove(iX,iY);
}

void CCtrlTree::AddUpButton(int iX,int iY,WORD wTex,WORD wMTex,WORD wCTex)
{
	if(m_pUpButton)
		RemoveControl((CControl**)&m_pUpButton);

	m_pUpButton = new CCtrlButton();
	AddControl(m_pUpButton);
	m_pUpButton->CreateEx(this,iX,iY,wTex,wMTex,wCTex);
}

void CCtrlTree::AddDownButton(int iX,int iY,WORD wTex,WORD wMTex,WORD wCTex)
{
	if(m_pDownButton)
		RemoveControl((CControl**)&m_pDownButton);

	m_pDownButton = new CCtrlButton();
	AddControl(m_pDownButton);
	m_pDownButton->CreateEx(this,iX,iY,wTex,wMTex,wCTex);
}

void CCtrlTree::AddScroll(int iX,int iY,int iW,int iH,int iScrollIdx)
{
	if(m_pScroll)
		RemoveControl((CControl**)&m_pScroll);

	m_pScroll = new CCtrlScroll();
	AddControl(m_pScroll);
	m_pScroll->Create(this,iX,iY,iW,iH,iScrollIdx);
	m_iScrollWidth = iW;
	m_pScroll->SetStep(1);
}

void CCtrlTree::RefreshTreeRows()
{
	//有多少行,当前行所在位置	
	if (!m_pRoot) 
	{
		m_iTreeRows = 0;
		return;
	}

	m_iTreeRows = m_pRoot->GetNodeRows();
	if (!m_bDrawRoot)
		m_iTreeRows--;
	m_pScroll->SetRange(m_iTreeRows);
}