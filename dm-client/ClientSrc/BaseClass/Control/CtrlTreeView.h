#pragma once
#include "Control.h"
#include "CtrlButton.h"
#include "CtrlScroll.h"
class CTreeViewNode;
class CCtrlTreeView;
typedef std::vector<CTreeViewNode*> VChildren;
class CTreeViewNode
{
public:
	CTreeViewNode(CTreeViewNode* pParent,CCtrlTreeView* pTreeView,LPCTSTR szText,DWORD dwID,WORD wTexID,WORD wTexExID,bool bLeaf = false,bool bIncreaseTex = true,bool bIncreateTexEx = true);
	~CTreeViewNode();
public:
	void InsertChildNode(CTreeViewNode* pNode);
	bool DeleteNode(CTreeViewNode* pTreeNode);//删除子结点
	CTreeViewNode* GetParent(){ return m_pParent;}
	void SetText(LPCTSTR szText){ m_strText = szText;}	
	
	CTreeViewNode* FindNode(DWORD dwID);
	CTreeViewNode* FindNode(LPCTSTR szText);
	CTreeViewNode* FindNode(CCtrlButton* pBtn);

	VChildren& GetChildrenArray(){ return m_vChildren;}

	bool IsUnBind(){return m_bUnBind;}
	void SetUnBind(bool bUnBind = true);
	void SetColor(DWORD dwColor){ m_dwSelColor = dwColor; }
	void SetFocusImg(DWORD dwImg){ m_dwFocusImg = dwImg;}

	void ShowButton(bool bShow);
	int  GetNodeRows();

	CCtrlButton* GetButton(){return m_pButton;}

	bool OnClickButton(bool bShow = true);
	bool IsSelected(){return m_bSelected;}
	void SetSelected(bool b){m_bSelected = b;}
	bool IsChecked(){return m_bCheckOn;}
	void SetChecked(bool b);
	DWORD GetID(){return m_dwID;}
	bool IsLeaf(){return m_bLeaf;}
	void OnTracked();

	bool Draw(int iX,int iY,int& iStartRow,int& iCurRow,int iMaxRow);//当前行和最大行
	bool OnClickTask(int iLeft,int iTop,int iCurRow,int iStartRow);
protected:
	DWORD			m_dwID;//唯一标识
	string			m_strText;
	VChildren		m_vChildren;
	CTreeViewNode*	m_pParent;
	bool			m_bUnBind;
	bool			m_bLeaf;
	bool			m_bCheckOn;
	bool			m_bSelected;
	
	bool			m_bIncreaseTex;
	bool			m_bIncreateTexEx;
	
	DWORD			m_dwSelColor;
	DWORD			m_dwNormalColor;
	DWORD			m_dwFocusImg;
	WORD			m_wTexID;
	WORD			m_wTexExID;
	CCtrlButton*	m_pButton;
	CCtrlTreeView*  m_pTreeView;
};


class CCtrlTreeView : public CControlContainer
{
public:
	CCtrlTreeView(void);
	~CCtrlTreeView(void);
public:
	virtual void OnCreate(void);
	virtual bool Create(CControl * pParent,int l,int t,int r,int b,int iRows);
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	virtual void Draw();
	virtual bool OnMouseMove(int iX,int iY);			// 鼠标移动消息
	virtual bool OnWheel(int iWheel);					// 滚轮消息
	virtual bool OnLeftButtonDown(int iX,int iY);
	virtual bool OnLeftButtonUp(int iX,int iY);
	virtual bool OnLeftButtonDClick(int iX,int iY);

	void AddScroll(int iX,int iY,int iW,int iH,int iScrollIdx);
	
	void SetRootNode(CTreeViewNode* pRoot){m_pRoot = pRoot;}//构造一个树要手工制作
	CTreeViewNode* GetRootNode(){ return m_pRoot;}
	CTreeViewNode* GetSelectedNode(){return m_pSelectedNode;}
	void	SetSelectedNode(CTreeViewNode* pNode){m_pSelectedNode = pNode;}
	void	GetClickXY(int& iX,int& iY){iX = m_iClickX;iY = m_iClickY;}
	void	ClearClickXY(){m_iClickX = -1000,m_iClickY = -1000;}
	void	GetMouseXY(int& iX,int& iY){iX = m_iMouseX;iY = m_iMouseY;}
	void	SetMouseXY(int iX = -1000,int iY = -1000){m_iMouseX = iX;m_iMouseY = iY;}
	void	RefreshControl();
	int		GetRowHigh(){return m_iRowHigh;}
	void	SetRowHigh(int iHigh){m_iRowHigh = iHigh;}
	int		GetSelLength(){return m_iSelLen;}
	void	SetSelLength(int iLen){m_iSelLen = iLen;}
	void    SetSelectedBar(WORD wBar){m_wSelectedBar = wBar;}
	WORD	GetSelectedBar(){return m_wSelectedBar;}
protected:
	CTreeViewNode* m_pRoot;//根结点
	CTreeViewNode* m_pSelectedNode;//当前选中的结点
	CCtrlScroll*   m_pScroll;
	int			   m_iSelLen;
	int			   m_iClickX;
	int			   m_iClickY;
	int			   m_iMouseX;
	int			   m_iMouseY;
	int			   m_iLastPos;
	int			   m_iTreeRows;
	int			   m_iShowMaxRows;//最大显示行数
	int			   m_iRowHigh;
	WORD		   m_wSelectedBar;//选中条
};
