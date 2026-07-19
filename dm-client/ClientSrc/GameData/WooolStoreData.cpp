#include "wooolstoredata.h"
#include "GameControl/GameControl.h"
#include "GameData/GameData.h"
#include "GameData/MsgBoxMgr.h"
#include "GameData/MacroDefine.h"
#include "GameClient/SDOAInterface.h"

#ifdef _DEBUG
#include "global/new.h"
#define new DEBUG_NEW
#endif

//物品查找迭代器
class CQuickItemIterator : public GoodIterator
{
public:
	CQuickItemIterator(const string& name)
	{
		strName = name;
	}
	bool DoIterator(CGood& tmp)
	{
		if(tmp.GetID() == 0)
			false;

		if(tmp.GetStdMode() == 3 && tmp.GetShape() == 29)//镇魔符(测试),镇魔神符(测试)
			return true;

		return false;
	}
	string strName;
};

const int MAINGROUPSTART  = 10;
const int MAINGROUPEND  = 100;

const int SUBGROUPSTART  = 1010;
const int SUBGROUPEND  = 10000;

const int ITEMGROUPSTART  = 10000;
const int ITEMGROUPEND  = 0x7fffffff;

const char*	MAINGROUPNAME[MAINGROUPCOUNT] = {
	"首页",
	"喜庆",
	"礼包",
	"符石",
	"法宝",
	"其他",
	"帮助",
};

const char*	CWooolStoreItem::VisitChild()
{

	//char szTemp[256];
	if (m_strDesc.size() > 0)
	{
		return (m_strDesc.c_str());
	}
	else
	{// 没有数据,向服务端请求
		//sprintf(szTemp, "%s", m_dwItemID);
		g_pGameControl->SEND_PT_Store_Info(4, m_dwItemID.c_str(), m_dwItemID.size());
	}

	return NULL;

}

CWooolStoreItem* SubGroup::VisitChild(unsigned int iNum)
{

	char szTemp[256];
	if (m_bAlreadyQuery/*m_vSubItem.size() > 0*/)
	{
		if (m_vSubItem.size() > iNum)
		{
			return (&(m_vSubItem[iNum]));
		}
	}
	else
	{// 没有请求过,向服务端请求
		m_bAlreadyQuery = true;
		sprintf(szTemp, "%d", m_dwSubGroupID);
		g_pGameControl->SEND_PT_Store_Info(3, szTemp, strlen(szTemp));
	}

	return NULL;

}

SubGroup*	MainGroup::VisitChild(unsigned int iNum)
{

	char szTemp[256];
	if (m_bAlreadyQuery/*m_vSubGroup.size() > 0*/)
	{
		if (m_vSubGroup.size() > iNum)
		{
			return (&(m_vSubGroup[iNum]));
		}
	}
	else
	{// 没有请求过,向服务端请求
		if(m_iMainGroupID != -1)//-1:帮助
		{
			m_bAlreadyQuery = true;
			sprintf(szTemp, "%d", m_iMainGroupID);
			g_pGameControl->SEND_PT_Store_Info(2, szTemp, strlen(szTemp));
		}
	}

	return NULL;
}

CWooolStoreMgr g_WooolStoreMgr;

CWooolStoreMgr::CWooolStoreMgr(void)
{

	for (int i = 0; i < MAINGROUPCOUNT; ++i)
	{
		if (i == HELPINDEX)
		{
			m_MainGroup[i].m_iMainGroupID = -1;
		}
		else
		{
			m_MainGroup[i].m_iMainGroupID = (i + 1) * 10;
		}

		m_MainGroup[i].m_strMainGroupName.assign(MAINGROUPNAME[i]);
	}

	m_TmpQuickItem.clear();
	m_BuyingQuickItem.clear();
	m_DropQuickItem.clear();
	m_bIsRequesPetQuick = false;
	m_iWooolStorePage = 0;
	m_iPaiMaiStoreType = 0;
}

CWooolStoreMgr::~CWooolStoreMgr(void)
{

}

void CWooolStoreMgr::WSMFacade(const char* Msg, int iLen)
{
	// 所有的网络协议均传入该函数进行处理

	BYTE byType = *(Msg + 6);
	bool bShow = true;
	WORD wType  = *((WORD*)(Msg+8));

	if(!wType && Msg[10])
		bShow = false;// 不弹消息框	
	switch(byType) 
	{
	case 0:
		ParseScrollText(Msg + 12, iLen - 12);
		break;
	case 1:
		ParseHotItem(Msg + 12, iLen - 12);
		break;
	case 2:
		ParseSubMenu(Msg + 12, iLen - 12);
		break;
	case 3:
		ParseConcreteItem(Msg + 12, iLen - 12);
		break;
	case 4:
		ParseItemDesc(Msg + 12, iLen - 12);
		break;
	case 5:	// 直接给出对应的提示
	case 15://绑定元宝购买结果
	case 100:
		ParseTradeResult(Msg + 12, iLen - 12,bShow);
		break;
	case 6:
		if(wType==0)
		{
			ParseResvItem(Msg + 12, iLen - 12);
		}
		break;
	case 7:
		ParseSendResult(Msg + 12, iLen - 12, int(wType) );
		break;
	case 8:
		ParseRecvResult(Msg + 12, iLen - 12,wType);
		break;
	case 9:
		ParseTradeResult(Msg + 12, iLen - 12);
		break;
	default:
		break;
	}

}

// @ ...MainString... |...SubString...|......|...... @
bool CWooolStoreMgr::GetMainString(IN const char*& pData, OUT string& strTarget)	// 获得@  @中的字符串(不包括@),并保存出来
{

	if (pData == NULL)			// Is pData legal?
		return false;

	const char* pEnd = NULL;

	if ((pEnd = strchr(pData, CHARHEADER)) != NULL)
	{
		strTarget.assign(pData, pEnd);
		strTarget += CHARMIDDLE;
		pData = pEnd + 1;
		return true;
	}
	else
		return false;

}

bool CWooolStoreMgr::GetSubString(IN const char*& pData, OUT int& iTarget)				// 获得一个|前的数字
{

	if (pData == NULL)			// Is pData legal?
		return false;

	const char* pEnd = NULL;

	if ((pEnd = strchr(pData, CHARMIDDLE)) != NULL)
	{
		string strTarget(pData, pEnd);
		pData = pEnd + 1;
		iTarget = atoi(strTarget.c_str());
		return true;
	}
	else
		return false;

}

bool CWooolStoreMgr::GetSubString(IN const char*& pData, OUT string& strTarget)			// 获得一个|前的字符
{

	if (pData == NULL)			// Is pData legal?
		return false;

	const char* pEnd = NULL;

	if ((pEnd = strchr(pData, CHARMIDDLE)) != NULL)
	{
		strTarget.assign(pData, pEnd);
		pData = pEnd + 1;
		return true;
	}
	else
		return false;

}

void CWooolStoreMgr::ParseScrollText(IN const char* pData, IN int iLen)
{

	if (pData == NULL || iLen == 0) 
		return;

	m_vScrollText.clear();
	int ioffset = 0;
	string strTemp(pData, iLen);
	string strEle;
	strTemp += "\\\\";
	pData = strTemp.c_str();

	const char* pTemp = NULL;

	while ((pTemp = strstr(pData, "\\\\")) != NULL)
	{
		strEle.assign(pData, pTemp);
		if (strEle.size() > 0)
			m_vScrollText.push_back(strEle);
		pData = pTemp + 2;
	}

}

int CWooolStoreMgr::ParseHotItem(IN const char* pData, IN int iLen)			// 分析热门商品
{

	if (pData == NULL || iLen == 0)			// Is pData legal?
		return -1;

	CWooolStoreItem tempItem;
	string strTemp(pData, iLen);		// 抽取对应数据至strTemp
	string strMain;
	pData = strTemp.c_str() + 1; //去掉头上的@符号

	int iIndex = 0;

	while (GetMainString(pData, strMain))
	{
		const char* pV = strMain.c_str();
		tempItem.clear();
		GetSubString(pV, tempItem.m_dwItemID);
		GetSubString(pV, tempItem.m_dwItemLooks);
		GetSubString(pV, tempItem.m_dwPreview);
		GetSubString(pV, tempItem.m_strName);
		GetSubString(pV, tempItem.m_iPrice);
		GetSubString(pV, tempItem.m_iTotalNum);
		GetSubString(pV, tempItem.m_iRealGood);

		// 对应数据压入容器
		m_HotItems[iIndex] = tempItem;
		iIndex = (iIndex + 1) % HOTITEMCOUNT;
	}
	//如果快捷购物的配置文件为空那么刷新一下
	//加载商城Top10的物品，只取QUICKITEM_COUNT个即可
	for(int i=0;i < QUICKITEM_COUNT;i++)
	{
		if(	m_aQuickPkg[i].strItemID.empty() && !m_HotItems[i].m_dwItemID.empty())
		{
			m_aQuickPkg[i].strItemID = m_HotItems[i].m_dwItemID;
			m_aQuickPkg[i].iItemLooks = m_HotItems[i].m_dwItemLooks;
			m_aQuickPkg[i].strName = m_HotItems[i].m_strName;
			m_aQuickPkg[i].iPrice = m_HotItems[i].m_iPrice;
		}
	}
	return 0;

}

void CWooolStoreMgr::ParseTradeResult(IN const char* pData, IN int iLen, bool bShow)	// 分析购买结果
{

	if (pData != NULL && iLen != 0)
	{
		//bool bHide = false;

		if(bShow)
		{
			string strShow(pData, iLen);
			g_MsgBoxMgr.PopSimpleAlert(strShow.c_str());

			if(strcmp(strShow.c_str(),"此次购买失败。")==0)
			{
				GetBuyingQuickItem().clear();
				GetQuickBuyData().clear();
			}

		}
	}

}

void CWooolStoreMgr::ParseItemDesc(IN const char* pData, IN int iLen)		// 分析某个物品的具体描述
{

	if (pData == NULL || iLen == 0)			// Is pData legal?
		return;

	string strTemp(pData, iLen);
	string strMain;
	string strtempp;
	int iRemainGoodCount=0;


	pData = strTemp.c_str();
	GetMainString(pData, strtempp);	// 将物品的ID保存出来

	strMain.assign(strtempp.c_str(), strtempp.size() - 1);
	strtempp.assign(strMain);	// 物品ID


	iRemainGoodCount = *( (WORD*)pData);
	pData+=2;

	strMain.assign(pData);	// 赋值内容
	char* pTemp = (const_cast<char*>(strMain.c_str()));
	for (int i = 0; i < strMain.size(); ++i,++pTemp)
	{
		if (*pTemp == '\\' && *(pTemp + 1) == '\\')
		{
			*pTemp = '\r';
			*(pTemp + 1) = '\n';
		}
	}

	//if (pSub != NULL)
	{
		CWooolStoreItem* pItem = NULL;

		//for (int i = 0; i < pSub->m_vSubItem.size(); ++ i)
		//{
		//	if (iID == pSub->m_vSubItem.operator [](i).m_dwItemID) 
		//	{
		//		pItem = &(pSub->m_vSubItem.operator [](i));
		//		break;
		//	}
		//}
		bool bUnfind = true;
		for (int i = 0; i < MAINGROUPCOUNT && bUnfind; ++i)
		{
			MainGroup& rMain = m_MainGroup[i];
			for (int j = 0; j < rMain.m_vSubGroup.size() && bUnfind; ++j)
			{
				SubGroup& rSub = rMain.m_vSubGroup.operator [](j);
				for (int k = 0; k < rSub.m_vSubItem.size() && bUnfind; ++k)
				{
					if (rSub.m_vSubItem.operator [](k).m_dwItemID == strtempp)
					{
						pItem = &(rSub.m_vSubItem.operator [](k));	// 所有的对应的都更新
						pItem->m_strDesc.assign(strMain);
						pItem->m_iLimitSaleRemain = iRemainGoodCount;//限卖商品剩余数量
						//bUnfind = false;
					}
				}
			}
		}

		// = GetItemByID(iID, mainindex, subindex, itemindex);

		//if (pItem != NULL)
		//{// 将服务器传来的换行符\\转换成CCtrlMultiEdit控件的换行符\r\n

		//	pItem->m_strDesc.assign(strMain);
		//	//pItem->m_strDesc.assign(pData);
		//}
	}

	g_pControl->MsgToWnd(MSG_CTRL_WOOOL_STORE_WND,MSG_CTRL_WOOOL_STORE_WND,1);

	//return 0;

}

int CWooolStoreMgr::ParseSubMenu(const char* pData, int iLen)		// 分析服务端传来的小类名称
{

	if (pData == NULL || iLen == 0)			// Is pData legal?
		return -1;

	SubGroup tempSubMenu;
	string strTemp(pData, iLen);
	string strMain;

	pData = strTemp.c_str();
	GetMainString(pData, strMain);		// 将大类的ID保存出来
	int iMainID = atoi(strMain.c_str());// 大类ID
	int mainindex;
	MainGroup* pMainGroup = GetMainGroupByID(strMain, mainindex);

	if (pMainGroup != NULL)
	{
		pMainGroup->m_bAlreadyQuery = true;
		while (GetMainString(pData, strMain))
		{
			const char* pV = strMain.c_str();
			tempSubMenu.clear();

			GetSubString(pV, tempSubMenu.m_dwSubGroupID);
			GetSubString(pV, tempSubMenu.m_strSubGroupName);

			tempSubMenu.m_dwSubGroupID += iMainID * 100;

			pMainGroup->m_vSubGroup.push_back(tempSubMenu);

			// 对应数据压入容器
		}
		//加入拍卖区
// 		tempSubMenu.m_bAlreadyQuery = true;
// 		tempSubMenu.m_strSubGroupName.assign("拍卖区");
// 		tempSubMenu.m_dwSubGroupID = -1;
// 		pMainGroup->m_vSubGroup.push_back(tempSubMenu);
	}

	return 0;

}

int CWooolStoreMgr::ParseConcreteItem(const char* pData, int iLen)		// 分析出服务端传来的具体物品的数据
{
	if (pData == NULL || iLen == 0)			// Is pData legal?
		return -1;

	CWooolStoreItem tempItem;
	string strTemp(pData, iLen);		// 抽取对应数据至strTemp
	string strMain;

	pData = strTemp.c_str();
	GetMainString(pData, strMain);			// 将大类小类的ID保存出来
	int iMainSubID = atoi(strMain.c_str());	// 大类小类ID

	int  mainindex, subindex;
	SubGroup* pSubGroup = GetSubGroupByID(strMain, mainindex, subindex);
	COpenStorePara  &OpenStorePara = g_WooolStoreMgr.GetOpenStorePara();
	
	bool bFindID = false;
	int idx = 0;

	if (pSubGroup != NULL)
	{
		pSubGroup->m_bAlreadyQuery = true;
		while (GetMainString(pData, strMain))
		{
			const char* pV = strMain.c_str();
			tempItem.clear();
			GetSubString(pV, tempItem.m_dwItemID);
			GetSubString(pV, tempItem.m_dwItemLooks);
			GetSubString(pV, tempItem.m_dwPreview);
			GetSubString(pV, tempItem.m_strName);
			GetSubString(pV, tempItem.m_iPrice);
			GetSubString(pV, tempItem.m_iTotalNum);
			GetSubString(pV, tempItem.m_iRealGood);
			GetSubString(pV, tempItem.m_iLimitCount);
			GetSubString(pV, tempItem.m_iType);

			if(OpenStorePara.id > 0 && mainindex == OpenStorePara.iMainGroup && subindex == OpenStorePara.iSubGroup && OpenStorePara.id == atoi(tempItem.m_dwItemID.c_str()))
			{
				OpenStorePara.id = 0;
				OpenStorePara.iIndex = idx;
				bFindID = true;
			}

			idx ++;

			//对应数据压入容器
			pSubGroup->m_vSubItem.push_back(tempItem);
		}
	}

	if(bFindID)//通过点击系统公告中的名字跳到对应的商城界面
		g_pControl->PopupWindow(MSG_CTRL_WOOOL_STORE_WND,OPER_CREATE,7);

	return 0;

}
//&对方角色名|Id|looks|dispname|物品占用格子数量|留言信息&
void CWooolStoreMgr::ParseResvItem(const char* pData, int iLen)		   //分析接收赠品的信息
{
	if (pData == NULL || iLen == 0)			// Is pData legal?
		return ;
	int iPos=0;
	for( int i=0;i<RECVCOUNT;i++)
	{
		if(m_RecvData[i].m_strItemID.empty())
		{

			m_RecvData[i].clear();
			string strTemp(pData, iLen);		// 抽取对应数据至strTemp
			string strMain;
			pData = strTemp.c_str() + 1; //去掉头上的@符号
			int iIndex = 0;
			while (GetMainString(pData, strMain))
			{
				const char* pV = strMain.c_str();
				GetSubString(pV, m_RecvData[i].m_strOriginUser);
				GetSubString(pV, m_RecvData[i].m_strItemID);
				GetSubString(pV, m_RecvData[i].m_strItemLooks);
				GetSubString(pV, m_RecvData[i].m_strItemName);
				GetSubString(pV, m_RecvData[i].m_strItemGrid);	
				GetSubString(pV, m_RecvData[i].m_strSendWords);
				iIndex++;
				if(iIndex>6)
					break;
			}

			return;
		}
	}



}

void CWooolStoreMgr::ParseSendResult(const char* pData, int iLen, int iType)		// 分析发送赠送物品的结果
{
	if(iType == 0)
	{
		string strShow;
		strShow = "物品赠送成功！";
		g_MsgBoxMgr.PopSimpleAlert(strShow.c_str());
	}
	else
	{	
		if(iType > 0)
		{
			if (pData != NULL && iLen != 0)
			{
				string strShow(pData, iLen);
				g_MsgBoxMgr.PopSimpleAlert(strShow.c_str());
			}
		}
		else
		{
			string strShow;
			strShow = "物品赠送失败！";
			g_MsgBoxMgr.PopSimpleAlert(strShow.c_str());
		}
	}

}


void CWooolStoreMgr::ParseRecvResult(const char* pData, int iLen,int iType)		// 分析接收物品的结果
{
	//if (pData != NULL && iLen != 0)
	//{

	//	if(atoi(pData)== 0)
	//	{
	//		string strShow;
	//		strShow = "物品接收成功！";
	//		g_MsgBoxMgr.PopSimpleAlert(strShow.c_str());

	//	}
	//	else
	//	{
	//		string strShow;
	//		strShow = "物品接收失败！";
	//		g_MsgBoxMgr.PopSimpleAlert(strShow.c_str());
	//	}
	//}

	if (pData != NULL && iLen != 0)
	{
		if(iType == 0)
		{
			string strShow;
			strShow = "物品接收成功！";
			g_MsgBoxMgr.PopSimpleAlert(strShow.c_str());

		}
		else
		{
			string strShow;
			strShow = "物品接收失败！";
			g_MsgBoxMgr.PopSimpleAlert(strShow.c_str());
		}
	}
}



MainGroup* CWooolStoreMgr::GetMainGroupByID(string& ID, int& iMainIndex)			// 根据subgroup得到其父亲,即一级group
{

	iMainIndex = 0x7FFFFFFF;

	//if (ID >= MAINGROUPSTART && ID < MAINGROUPEND)
	//	iMainIndex = ID / 10 - 1;
	//else if (ID >= SUBGROUPSTART && ID < SUBGROUPEND)
	//	iMainIndex = ID / 1000 - 1;
	//else if (ID >= ITEMGROUPSTART && ID < ITEMGROUPEND)
	//	iMainIndex = ID / 1000000 - 1;

	string	strExtract;

	if (ID.size() > 1) 
		strExtract.assign(ID.c_str(), 2);

	iMainIndex = atoi(strExtract.c_str());
	iMainIndex = iMainIndex / 10 - 1;

	if (iMainIndex >= 0 && iMainIndex < MAINGROUPCOUNT) 
		return (&(m_MainGroup[iMainIndex]));
	else
		return NULL;

}

SubGroup* CWooolStoreMgr::GetSubGroupByID(string& ID, int& iMainIndex, int& iSubIndex)			// 根据item得到父亲,即2级group
{

	iMainIndex = 0x7FFFFFFF;
	iSubIndex = 0x7FFFFFFF;

	//if (ID >= SUBGROUPSTART && ID < SUBGROUPEND)
	//{
	//	iMainIndex = ID / 1000 - 1;
	//	iSubIndex = (ID % 100) / 10 - 1;
	//}
	//else if (ID >= ITEMGROUPSTART && ID < ITEMGROUPEND)
	//{
	//	iMainIndex = ID / 1000000 - 1;
	//	iSubIndex = (ID % 100000) / 10000 - 1;
	//}
	string strExtract;

	if (ID.size() > 3) 
	{
		strExtract.assign(ID.c_str(), 2);
		iMainIndex = atoi(strExtract.c_str());
		strExtract.assign(ID.c_str() + 2, 2);
		iSubIndex = atoi(strExtract.c_str());
		iMainIndex = iMainIndex / 10 - 1;
		iSubIndex = iSubIndex / 10 - 1;
	}

	if (iMainIndex < MAINGROUPCOUNT)
	{
		if (iSubIndex < m_MainGroup[iMainIndex].m_vSubGroup.size())
		{
			return (&(m_MainGroup[iMainIndex].m_vSubGroup[iSubIndex]));
		}
		else
			return NULL;
	}
	else 
		return NULL;

}

CWooolStoreItem* CWooolStoreMgr::GetItemByID(string& ID, int& iMainIndex, int& iSubIndex, int& iItemIndex)
{// 该函数已作废，不曾使用

	iMainIndex = 0x7fffffff;
	iSubIndex = 0x7fffffff;
	iItemIndex = 0x7fffffff;

	//if (ID >= ITEMGROUPSTART && ID < ITEMGROUPEND)
	//{
	//	iMainIndex = ID / 1000000 - 1;
	//	iSubIndex = (ID % 100000) / 10000 - 1;
	//	iItemIndex = (ID % 1000) - 1;
	//}

	if (iMainIndex < MAINGROUPCOUNT)
	{
		if (iSubIndex < m_MainGroup[iMainIndex].m_vSubGroup.size())
		{
			if (iItemIndex < m_MainGroup[iMainIndex].m_vSubGroup[iSubIndex].m_vSubItem.size())
			{
				return (&(m_MainGroup[iMainIndex].m_vSubGroup[iSubIndex].m_vSubItem[iItemIndex]));
			}
			else
				return NULL;
		}
		else
			return NULL;
	}
	else 
		return NULL;

}
int CWooolStoreMgr::GetItemIndexByID(int iMainIndex, int iSubIndex,int id)
{
	if (iMainIndex < MAINGROUPCOUNT)
	{
		if (iSubIndex < m_MainGroup[iMainIndex].m_vSubGroup.size())
		{
			char cTemp[12];
			sprintf(cTemp,"%d",id);
			string strTemp = cTemp;

			std::vector<CWooolStoreItem> & vSubItem = m_MainGroup[iMainIndex].m_vSubGroup[iSubIndex].m_vSubItem;
			size_t size = vSubItem.size();
			for(size_t i = 0; i < size; i ++)
			{
				if(strTemp == vSubItem[i].m_dwItemID)
					return (int)i;
			}

			return -1;
		}
		else
			return -1;
	}
	else 
		return -1;
}

int CWooolStoreMgr::QueryInfo(IN unsigned int iMain, IN unsigned int iSub, IN unsigned int iItem)
{
	//可以嵌套模板来实现该函数
	//返回true表示向服务器提交了请求,false表示本机已有相关数据


	if (iMain == 0 && iSub == -1)
	{
		// 第一次请求
		if (m_MainGroup[0].m_bAlreadyQuery == false)
		{
			g_pGameControl->SEND_PT_Store_Info(1);
		}
		return 0;	// 第一次请求数据
	}
	if(iMain ==0)
	{
		g_pGameControl->SEND_PT_Store_Info(6);//每次进入商城的时候向服务端发送查询接收赠品的指令

	}
	if (iMain >= 0 && iMain < MAINGROUPCOUNT)
	{
		SubGroup* pSubGroup = m_MainGroup[iMain].VisitChild(iSub);
		if (pSubGroup != NULL)
		{
			CWooolStoreItem* pItem = pSubGroup->VisitChild(iItem);
			if (pItem != NULL)
			{
				pItem->VisitChild();
				return 3;	// 请求具体物体的说明或者本机数据齐全无需请求
			}
			else
				return 2;	// 请求了子目录下的内容或是iItem越界
		}
		else
			return 1;	// 请求了主目录下的内容，或者iSub越界
	}

	return -1;	// 输入数据不合法

}

void CWooolStoreMgr::UseQuickItem(int iIndex ,bool bUse)
{
	if(iIndex < 0 || iIndex >= QUICKITEM_COUNT)
		return;

	//快捷消费栏
	int iIndex0 = -1;
	CQuickItem& qitem = m_aQuickPkg[iIndex];

	if(qitem.strName.empty() || qitem.iItemLooks == 0)
		return ;

	qitem.iBuyType = 0;
	if(SELF.GetBindYuanBao() >= qitem.iPrice)		
	{
		qitem.iBuyType = 1;
	}

	//if(bUse)
	//{
	//	CQuickItemIterator itr(qitem.strName);
	//	iIndex0 =  SELF.PackageGood().FindGoodByIterator(itr);

	//	if(iIndex0 == -1)
	//	{
	//		if(SELF.GetUsingTemp().FindGoodByName(qitem.strName.c_str()) >= 0)
	//			return;
	//	}
	//}

	//if(iIndex0 >= 0)
	//{
	//	//使用一次
	//	g_pGameControl->SEND_Use_Object(iIndex0);
	//}
	//else
	{
		//购买后使用
		this->BuyQuickItem(qitem,bUse); 
	}
}

void CWooolStoreMgr::UsePetQuickItem(int iIndex,bool bUse)
{
	if(iIndex < 0 || iIndex >= PETQUICKITEM_COUNT)
		return;

	//快捷消费栏
	int iIndex0 = -1;
	CQuickItem& qitem = GetQuickPetItem(iIndex);
	if(qitem.strName.empty() || qitem.iItemLooks == 0)
		return ;

	for(int i = 0;i< MAX_PACKAGE_ELEMENT;i++)
	{
		CGood& tempGood = SELF.GetPackageGood(i);

		if(tempGood.GetID() == 0 || (tempGood.GetDura() == 0 && tempGood.GetDuraMax() >0))
			continue;				

		if(qitem.strName.compare(tempGood.GetName()) == 0 )
		{
			iIndex0 =  i;
			break;
		}
	}

	if(iIndex0 == -1)
	{
		if(SELF.GetUsingTemp().Size() > 0) 
		{
			if(SELF.GetUsingTemp().FindGoodByName(qitem.strName.c_str()) > 0)
				return;
		}
	}


	if(iIndex0 >= 0 && bUse)
	{
		//使用一次
		g_pGameControl->SEND_Use_Object(iIndex0);
	}
	else
	{
		//购买后使用
		BuyQuickItem(qitem,bUse,1,true); 
	}
}

void CWooolStoreMgr::BuyQuickItem(CQuickItem qitem,bool bUse,int iBuyCount,bool bInputPetPkg,bool bQuickBuy)
{
	//如果元宝不够，加信用够，用信用加元宝购买11
	int iNeedYuanBao = qitem.iPrice * iBuyCount;
	if (qitem.iBuyType == 1)
	{
		if(SELF.GetBindYuanBao() < iNeedYuanBao)		
		{
			g_MsgBoxMgr.PopTagAlert("对不起,您的绑定元宝不足!");
			return;
		}
	}
	else if(SELF.GetYuanBao() < iNeedYuanBao)//元宝不够用
	{
		//if(g_dwServerSwitch & SS_CREDIT)//现在改成元宝不够都走信用
		//{
		//	//if(SELF.GetCreditMax() == 0)//还没有开通信用系统
		//	//{
		//	//	//调用igw打开相关页面问他要不要开通信用
		//	//	g_pSDOAInterface->OpenWidgetExA("sdoCredit","gatetype=1",0);
		//	//	return;
		//	//}
		//	//else
		//	//{
		//	//	int iNeedCredit = (iNeedYuanBao - SELF.GetYuanBao()) * 100;
		//	//	iNeedCredit += (int)(iNeedCredit * SELF.GetCreditFeeRate() + 0.9);
		//	//	if(iNeedCredit > SELF.GetCredit())
		//	//	{
		//	//		g_MsgBoxMgr.PopTagAlert(STRING_NOT_ENOUGH_YUANBAO);
		//	//		return;
		//	//	}
		//	//	else//信用够用,直接发到gs,gs发协议后转到igw
		//	//	{

		//	//	}
		//	//}
		//}
		//else
		{
			g_MsgBoxMgr.PopTagAlert(STRING_NOT_ENOUGH_YUANBAO);
			return;
		}
	}

	//延迟消费，先购买，然后再消费
	std::string strTemp = qitem.strItemID;
	std::string strSend = strTemp;
	if(bQuickBuy && strTemp.size() > 4)
	{
		strSend.assign(strTemp.c_str()+4,strTemp.size()-4);
	}

	char szTemp[128];
	sprintf(szTemp, "%d", iBuyCount);	
	strSend.append(1,'&');
	strSend +=szTemp;

	int iBuyType = 5;
	if (qitem.iBuyType == 1)
	{
		iBuyType = 15;//用绑定元宝购买
	}

	g_pGameControl->SEND_PT_Store_Info(iBuyType, strSend.c_str(), strSend.size(),bQuickBuy,bInputPetPkg);

	//设置延迟消费，如果买后不消费而且不是快捷消息2.0中购买的物品的话，需要将qitem清除,快捷消息2.0中购买的物品成功后要求有一个提示
	if(!bUse && qitem.iType !=1 )
	{
		qitem.clear();
	}

	if (bQuickBuy)
	{
		m_BuyingQuickItem = qitem;
	}
}

void  CWooolStoreMgr::SetQuickBuyItem(string strName)
{
	std::map<string,CQuickItem>::iterator iter;

	if( (iter = m_MQuickBuyItem.find(strName)) == m_MQuickBuyItem.end())
	{
		m_QuickBuyData.pItem = NULL;
	}
	else
	{
		m_QuickBuyData.pItem = &(iter->second);
	}

}
