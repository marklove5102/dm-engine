#pragma once

#include "Global/Global.h"
#include "BaseClass\Control\Control.h"
#include "GameData/WooolStoreData.h"
#include "BaseClass/Control/CtrlButton.h"
#include "BaseClass/Control/CtrlMultiEdit.h"
#include "BaseClass/Control/CtrlEdit.h"
#include "baseclass/Control/CtrlScroll.h"
#include "BaseClass/Control/CtrlWindowX.h"

#ifdef _DEBUG
#include "global/new.h"
#define new DEBUG_NEW
#endif

#define ICONCOUNT 10    //商城中每页10个物品
#define HOTCOUNT 10      //商城中每页的热卖商品为10个
#define NEXTPAGEUNION 4 //页面空键每页显示单位为4页 至少为3以上

#define STOREGROUPCOUNT 5 //三种类型的商城 0 游戏商城 1 实物商城 2 拍卖行 3  积分商城 4 分红商城

//////////////////////////////////////////////////////////////////////////
//#define SUBGROUP_ACTIVECOLOR 0xffE6C800			// 子菜单的激活和非激活状态的文字颜色
//#define SUBGROUP_COMMONCOLOR 0xff71723A
#define SUBGROUP_ACTIVECOLOR 0xff00ff00			// 子菜单的激活和非激活状态的文字颜色
#define SUBGROUP_COMMONCOLOR 0xffffffff
//////////////////////////////////////////////////////////////////////////
#define ITEMCTRL_ACTIVECOLOR 0xff00ff00			// 物品的激活和非激活状态的文字颜色
#define ITEMCTRL_COMMONCOLOR 0xffffffff
//////////////////////////////////////////////////////////////////////////


enum DRAWTYPE
{
	HADT_TEXT = 0,
	HADT_TEX = 1,
	HADT_ICON = 2,
	HADT_HOT,
	HADT_MAX,
	HADT_PAGE
};

enum WooolStoreIndex
{
	StoreIndexGame = 0, // 游戏商城
	StoreIndexRS = 1,   // 实物商城 易宝商城
	StoreIndexPM = 2,   // 拍卖行
	StoreIndexCS = 3,   // 出售物品
	StoreIndexFH = 4,   // 分红商城
	StoreIndexJF = 5,   // 积分商城
	StoreIndexNums
};

struct PTStoreHotArea
{
	
	PTStoreHotArea(int l, int r, int t, int b, int tx = -1, int ty = -1, int iIndex = -1, int iPackage = PACKAGE_INTERFACE, const char* pText = "")
		:left(l),right(r),top(t),bottom(b), texX(tx), texY(ty), strTitle(pText)
	{
		m_iHeadTexPackage = iPackage;
		m_iHeadTexIndex = iIndex;

		if (tx == -1)	tx = l;
		if (ty == -1)	ty = t;
		pAppendItem = NULL;
		m_bActive = false;
	}
	int left;
	int right;
	int top;
	int bottom;
	std::string	strTitle;
	int m_iHeadTexPackage,m_iHeadTexIndex;
	CWooolStoreItem*	pAppendItem;// 与控件绑定的物品
	int texX, texY;				// 图片绘制位置
	bool m_bActive;

	void SetHAText(const std::string& strIn){strTitle = strIn;}

	void SetAppend(CWooolStoreItem* pItem)
	{
		pAppendItem = pItem;
	}

	void ClearAppend()
	{
		pAppendItem = NULL;
		m_iHeadTexPackage = -1;
		m_iHeadTexIndex = -1;
	}

	bool IsClick(int iX, int iY)
	{
		if(m_bActive)
			return ((iX > left) && (iX < right) && (iY > top) && (iY < bottom));
		else
			return false;
	}

	void SetClickEnable(bool bActive){m_bActive = bActive;}//设置该控件是否可以点击
	bool GetClickEnable(){return m_bActive;}

	void Draw(int iX, int iY, DRAWTYPE drawtype, bool bActive = false)
	{
		char szTemp[128];
		switch(drawtype)
		{
		case HADT_PAGE:	// 显示页码控件
			{
				//g_pFont->DrawText(iX + left, iY + top, strTitle.c_str(), (bActive ? SUBGROUP_ACTIVECOLOR : SUBGROUP_COMMONCOLOR), FONT_GDI_SONG_CU,true);
				g_pFont->DrawText(iX + left, iY + top, strTitle.c_str(), (bActive ? SUBGROUP_ACTIVECOLOR : SUBGROUP_COMMONCOLOR), FONT_YAHEI,FONTSIZE_SMALL);
			break;
			}
		case HADT_TEXT:	// 文字控件的绘制方法，二级目录控件
		{
			g_pFont->DrawText(iX + left, iY + top, strTitle.c_str(), (bActive ? SUBGROUP_ACTIVECOLOR : SUBGROUP_COMMONCOLOR), FONT_YAHEI,FONTSIZE_MIDDLE);
			break;
		}
		case HADT_TEX:	// 纯纹理的绘制， 一级主目录控件
		{
			if (bActive && m_iHeadTexPackage > 0 && m_iHeadTexIndex > 0)
			{
				LPTexture pTexture = g_pTexMgr->GetTexImm(m_iHeadTexPackage, m_iHeadTexIndex,EP_UI);
				if(pTexture)
					g_pGfx->DrawTextureNL(iX + texX, iY + texY, pTexture); 
			}
			break;
		}
		case HADT_ICON:	// 物品图标的绘制，混合了大图标(ICON 48×48)和文字
		{
			if (pAppendItem != NULL) 
			{
				sprintf(szTemp, "%d 元宝", pAppendItem->m_iPrice);
				
				// 绘制名称和价钱，对于选中的，用别的颜色的文字来区别

				g_pFont->DrawText(iX + left + 42, iY + top + 0, pAppendItem->m_strName.c_str(), (bActive ? ITEMCTRL_ACTIVECOLOR : ITEMCTRL_COMMONCOLOR),FONT_YAHEI,FONTSIZE_SMALL);
				g_pFont->DrawText(iX + left + 42, iY + top + 22, szTemp, (bActive ? ITEMCTRL_ACTIVECOLOR : ITEMCTRL_COMMONCOLOR),FONT_YAHEI,FONTSIZE_SMALL);
				
				if(pAppendItem->m_iLimitCount >0)
				{

					sprintf(szTemp,"限卖%d件",pAppendItem->m_iLimitCount);

					if( pAppendItem->m_iLimitSaleRemain >= 0)
						sprintf(szTemp,"限卖%d/剩%d",pAppendItem->m_iLimitCount,pAppendItem->m_iLimitSaleRemain);
					else
						sprintf(szTemp,"限卖%d/剩...",pAppendItem->m_iLimitCount);

					g_pFont->DrawText(iX + left + 90, iY + top + 22, szTemp, 0xFFB0F000,FONT_YAHEI,FONTSIZE_SMALL);

				}
				// 绘制物品图标
				LPTexture pTex = g_pTexMgr->GetTexImm( PACKAGE_items, pAppendItem->m_dwItemLooks,EP_UI);
				if (pTex != NULL)
				{
					g_pGfx->DrawTextureNL(iX + left, iY + top + 2, pTex);
					if ((pAppendItem->m_iType & 0x1) || (pAppendItem->m_iType & 0x2))
					{
						g_pGfx->SetRenderMode(RM_ADD2);
						g_pGfx->DrawTextureNL(iX + left - 18, iY + top + 2 - 16, g_pTexMgr->GetTex( PACKAGE_INTERFACE, 22120,EP_UI));
						g_pGfx->SetRenderMode();
					}
				}

				//pAppendItem->m_iType = 0x1 | 0x2;
				int liStartX = -10;
				if (pAppendItem->m_iType & 0x1)
				{
					pTex = g_pTexMgr->GetTexImm( PACKAGE_INTERFACE, 10403,EP_UI);
					if (pTex != NULL)
						g_pGfx->DrawTextureNL(iX + left + liStartX, iY + top - 23, pTex); 
					liStartX += 42;
				}
				if (pAppendItem->m_iType & 0x2)
				{
					pTex = g_pTexMgr->GetTexImm( PACKAGE_INTERFACE, 10404,EP_UI);
					if (pTex != NULL)
						g_pGfx->DrawTextureNL(iX + left + liStartX, iY + top - 23, pTex); 
				}
				
			}
			break;
		}
		case HADT_HOT:	// 热区物品的绘制，混合了小图标(ICON 30×30)和文字
		{
			if (pAppendItem != NULL && pAppendItem->m_dwItemID.size() > 0) 
			{
				//sprintf(szTemp, "%d", pAppendItem->m_iPrice);

				// 绘制名称和价钱
				//g_pFont->DrawText(iX + left - 56, iY + top + 45, pAppendItem->m_strName.c_str(),-1,FONT_YAHEI,FONTSIZE_SMALL);
				//g_pFont->DrawText(iX + left - 30, iY + top + 4, szTemp,-1,FONT_YAHEI,FONTSIZE_SMALL);
				//g_pFont->DrawText(iX + left - 30, iY + top + 20, "元宝",0xFFFFFF00,FONT_YAHEI,FONTSIZE_SMALL);

				// 绘制物品图标
				LPTexture pTex = g_pTexMgr->GetTexImm( PACKAGE_items, pAppendItem->m_dwItemLooks,EP_UI);
				if (pTex != NULL)
					g_pGfx->DrawTextureNL(iX + left + 1, iY + top + 1, pTex); 
			}
			break;
		}
		default:
			break;
		}
	}
};

struct HotAreaContainer 
{// 控件容器类，包含一个vector存放对应控件数据
	typedef std::vector<PTStoreHotArea> VHA;
	VHA	vHotArea;
	int iActiveIndex;
	int iActiveIndex2;
	int iCtrlType;

	HotAreaContainer(){iActiveIndex = 0;iActiveIndex2 = 0;iCtrlType = 0; }	// 默认情况，第一个选中; 控件类型初始为0

	int GetCtrlType(){return iCtrlType;}
	void SetCtrlType(int iType){iCtrlType = iType;}
	void PushElement(int l, int r, int t, int b, int tx = -1, int ty = -1, int iIndex = -1, int iPackage = PACKAGE_INTERFACE, const char* pText = "")
	{// 添加一个控件
		vHotArea.push_back(PTStoreHotArea(l, r, t, b, tx, ty, iIndex, iPackage, pText));
	}

	int	ClickArea(int iX, int iY)
	{// 判断哪个子控件被点击了
		for (int i = 0; i < vHotArea.size(); ++i)
		{
#ifdef _DEBUG
			PTStoreHotArea& rtemp = vHotArea[i];
#endif
			if (vHotArea[i].IsClick(iX, iY))
			{
				if(GetCtrlType()==1)
				{//这里修正 页面控件 的激活页面编号
					if(i< NEXTPAGEUNION)
					{
						return(iActiveIndex = i);
						iActiveIndex2 = i;
					}
					else
					{
						int iPage = (i+1)/(NEXTPAGEUNION+2) ;//第几页算法					
						if( (i+2)%(NEXTPAGEUNION+2)==1    )//点击到了上一页按钮
						{
                            iActiveIndex = (iPage-1)*NEXTPAGEUNION;
							iActiveIndex2 = i-NEXTPAGEUNION-1;							
						}
						else if( (i+2)%(NEXTPAGEUNION+2)==0    )//点击到了下一页按钮
						{
							iActiveIndex = (iPage+1)*NEXTPAGEUNION;
							iActiveIndex2 = i+2;
						}
						else
						{
							iActiveIndex = iPage*NEXTPAGEUNION + (i+2)%(NEXTPAGEUNION+2)-2;
							iActiveIndex2 = i;
						}						
					    return iActiveIndex;
					}
				}
				else
				return (iActiveIndex = i);	// 有子控件被点击，返回控件的顺序
			}
		}
		return -1;
	}

	void Draw(int iX, int iY, DRAWTYPE drawtype)
	{// 绘制，调用子控件的绘制函数
		for (int i = 0; i < vHotArea.size(); ++i)
		{
			if(drawtype == HADT_PAGE && GetCtrlType()==1)//如果是页面控件的绘制 作特殊处理 仅仅绘制当前的显示单位
			{
			    if(vHotArea.size()<=NEXTPAGEUNION)//直接绘制
				{  
					vHotArea[i].SetClickEnable(true);
					vHotArea[i].Draw(iX, iY, drawtype, iActiveIndex == i);	
				}
				else if( i+(NEXTPAGEUNION-1)<vHotArea.size())//包含上页和下页的
				{
					int iTempPage   = iActiveIndex/NEXTPAGEUNION ;
					if( (i >= iTempPage*(NEXTPAGEUNION + 2)-1) &&  ( i <= iTempPage*(NEXTPAGEUNION + 2)+NEXTPAGEUNION ) )
					{
						vHotArea[i].SetClickEnable(true);
						if(i<=(NEXTPAGEUNION))
						vHotArea[i].Draw(iX, iY, drawtype, iActiveIndex == i);	// 子控件完成自绘制
						else
						vHotArea[i].Draw(iX, iY, drawtype, iActiveIndex2 == i);	// 子控件完成自绘制
					}
					else
					{
						vHotArea[i].SetClickEnable(false);
					}
				}
				else
				{
					int iTempPage   = iActiveIndex/NEXTPAGEUNION ;
					if( (i >= iTempPage*(NEXTPAGEUNION + 2)-1) &&  ( i <= iTempPage*(NEXTPAGEUNION + 2)+NEXTPAGEUNION-1 ) )
					{
						vHotArea[i].SetClickEnable(true);
						vHotArea[i].Draw(iX, iY, drawtype, iActiveIndex2 == i);	// 子控件完成自绘制
					}
					else
					{
						vHotArea[i].SetClickEnable(false);
					}
				}
			}
			else//普通控件绘制
			{
					vHotArea[i].SetClickEnable(true);
					vHotArea[i].Draw(iX, iY, drawtype, iActiveIndex == i);	// 子控件完成自绘制	
			}
		}
	}
};

class CWooolStoreWnd : public CCtrlWindowX
{// 商城窗口
	DECLARE_WND_POSX(CWooolStoreWnd)
public:
	CWooolStoreWnd(void);
	~CWooolStoreWnd(void);

	virtual void OnCreate(void);
	virtual bool OnLeftButtonDown(int iX, int iY);
	virtual bool OnRightButtonDown(int iX,int iY);
	virtual bool OnRightButtonUp(int iX,int iY);
	virtual bool OnLeftButtonUp(int iX, int iY);
	virtual bool OnLeftButtonDClick(int iX, int iY);
	virtual bool OnMouseMove(int iX,int iY);
	virtual void OnMove();
	virtual void OnSetFocus();
	virtual void OnKillFocus();
	virtual bool IsInControl(int iX,int iY); //是否在控件内

	virtual void OnDraw();
	virtual bool Msg(DWORD dwMsg, DWORD dwData, CControl* pControl = NULL);

	void    BuyItem(int i,bool bBind = false);
	void    SwitchSubPage(int  i2Real); // 切换到实物页面否
	void    SetGameStoreGroup();//设置游戏商城的大类、子类、页

protected:
	void	DrawPreviewAndTips();
	void	DrawSysInfo(); //绘制系统信息  


	HotAreaContainer    m_StoreGroupCtrl;   //游戏商城还是实物商城

	HotAreaContainer	m_MainGroupCtrl;	//主类控件，6个大类，固定
	HotAreaContainer	m_SubGroupCtrl;		//子类，不固定
	HotAreaContainer	m_ItemGroupCtrl;	//物品控件，一页8个物品，固定
	HotAreaContainer	m_HotGroupCtrl;		//热销商品

	HotAreaContainer	m_PageCtrl;			//页面设置

	int m_iWndType;//窗口类型

	int iStoreIndex;    //0:游戏商城，1实物商城,2 拍卖行 3 积分商城

	int iMainIndex;		// 当前选中项的对应索引
	int iSubIndex;
	int iItemIndex;

	int iOffset;		// 物品的相对偏移页数，0，1，2，在热区选择时
	int iTextPos;		// 滚动的位置
	std::string iBuyItemID;		// 确认购买的物品的ID

	MainGroup*	pMain;	// 当前选中项的对应指针,主项
	SubGroup*	pSub;	//  当前选中项的对应指针,子项
	CWooolStoreItem* pItem;	//  当前选中项的对应指针,物品项

	CCtrlButton *m_pButtonBuy[ICONCOUNT];	// 对应的几个控件，购买按钮
	CCtrlButton *m_pButtonSend[ICONCOUNT];	// 赠送按钮
	CCtrlButton *m_pLeaveStore;				// 离开商城按钮
	CCtrlButton *m_pAddYuanBao;				// 我要冲值按钮
	CCtrlButton *m_pVipMemberBtn;			// vip会员按钮
	//CCtrlButton *m_pSNDAJiFen;				// 盛大积分按钮
	//CCtrlButton *m_pClose;				    // 关闭按钮
	CCtrlButton *m_pActAward;				// 活动领奖
	//CCtrlMultiEdit	   *m_pDisplayText;		// 左下详细信息的提示框
	CCtrlButton *m_pCreditBtn;				// 信用按钮

	//CCtrlButton *m_pArrowUp;
	//CCtrlButton *m_pArrowDown;
	//CCtrlScroll		   *m_pScroll;			// 滚动条控件

	int     m_iWidgetX,m_iWidgetY;          // Widget在游戏中相对界面

	// 返回击中的按钮, 返回1表示点中购买按钮，-1表示赠送按钮，0表示没有按钮被点中， ppFocusItem表示对应的item，没点中时为NULL
	int FindFocusButton(IN CControl* pControl, OUT CWooolStoreItem** ppFocusItem);	
	void OnBuy(CWooolStoreItem& FocusItem, DWORD dwBind = 2);			// 点击对应的购买按钮，传入的是对应的物品的引用，如果没有物品则不会调用该函数
	void OnSend(CWooolStoreItem& FocusItem);	
	int	 GetLeftGrid();	// 得到包裹中剩余的个子数
	void OnPopMenuBuy();
	void OnPopMenuPresent();
	void OnPopMenuSetQuicItem();

	void CheckRecv(void);
	DWORD m_dwLastCheckTime; //

	bool	m_bDClicked;					//是否是双击左键
};