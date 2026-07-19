#include "GameControl.h"
#include "GameData\MagicDefine.h"
#include "BaseClass/Control/Control.h"
#include "GameData/OtherData.h"
#include "GameData/RangeStruct.h"

std::map<WORD,LP_RANGE_DADA>		m_mapRangeData;		//所有排名
std::map<WORD,LP_RANGE_DADA>		m_mapMyRangeData;	//个人信息
std::map<WORD,char>				m_mapMyRangeQushi;	//个人排名趋势
std::map<WORD,WORD>				m_mapTotalNum;	//排名数
std::map<WORD,WORD>				m_mapRangePos;	//上一次的滚柱及小类、pos位置,key=类型

WORD m_old_range=0;	//旧的排名
WORD  g_iSetRangeType = 0;//打开后要求设定的排名类别
WORD  g_iSetSmallRangeType = 0;//打开后要求设定的排名子类别
WORD g_Range[32]={0};
WORD g_RangeNum = 0;
WORD g_RangeVesion[32]={0};
bool g_HaveSBK=false;  //拥有沙城
std::map<WORD,WORD>				m_HesternalPlace; //昨日名次
string		 g_strFindRange;  //需要查找好友名次的名字

//顺序初始化项目
WORD g_initRange[32] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32};
////////////////
// 收到的消息处理

//获得排名列表
void CGameControl::MSG_Range_Sign_Up_Result(const char *msg,int iLen)
{
	ZeroMemory(g_Range,sizeof(g_Range));
	g_RangeNum=0;
	DWORD id = *(DWORD*)(msg);
	DWORD result = *(DWORD*)(msg + 6);
	for(int i = 0,j = 0;i < 32;i++)
	{
		if( ((result >> i) & 1) == 1)
		{
			g_Range[j]=g_initRange[i];
			j++;
			g_RangeNum = j;
		}
	}

	//打开后要求设定指定的排名,打开就跳过去
	if(g_iSetRangeType > 0)
	{
		for(int i = 0; i < g_RangeNum; i++)
		{
			if(g_iSetRangeType == g_Range[i])
			{
				m_old_range = (i << 8);//子类别设为0
				break;
			}
		}
		g_iSetRangeType = 0;
	}
	//打开后要求设定指定的排名,打开就跳过去
	if(g_iSetSmallRangeType > 0)
	{
		m_old_range += g_iSetSmallRangeType;
		g_iSetSmallRangeType = 0;
	}

	if(g_pControl && g_RangeNum>0)
		g_pControl->Msg(MSG_CTRL_RANGE_WND,OPER_RECREATE);

}

void CGameControl::MSG_Range_Request_Result(const char *msg,int iLen)
{

	WORD TotalNum = *(WORD*)(msg);
	WORD type = *(WORD*)(msg+6);
	WORD version = *(WORD*)(msg+8);
	WORD num = *(WORD*)(msg+10);
	if(num==0 || type ==0 )
		return;

	void * pvData = NULL;

	if(type == 0x0601)
	{
		g_HaveSBK = true;
		type = 0x0600;
	}
	else if(type == 0x0600)
		g_HaveSBK = false;

	m_mapTotalNum[type] = TotalNum;

	if(num==0xffff)//自己信息
	{
		LP_RANGE_DADA lpData;
		if(m_mapMyRangeData[type]==NULL)
		{
			lpData = m_mapMyRangeData[type] = new RANGE_DADA;
		}
		else
		{
			lpData = m_mapMyRangeData[type];
		}
		if((*lpData)[0]!=NULL)
		{
			DeleteStruct(type,((STRUCT_RANGE_DADA*)((*lpData)[0]))->vData);
			delete ((STRUCT_RANGE_DADA*)((*lpData)[0]));
			(*lpData)[0] = NULL;
		}
		int len = NewStruct(type,&pvData);
		if(pvData)
		{
			if(iLen>=(12+len))
			{
				memcpy(pvData,msg+12,len);
			}
			STRUCT_RANGE_DADA * structdata = new  STRUCT_RANGE_DADA ;
			(*lpData)[0] = structdata;
			structdata->vData = pvData;
			structdata->version = version;
			m_mapMyRangeQushi[type] = msg[2];
			g_RangeVesion[type>>8] =version;

			//昨天的名次
			if(iLen>=(14+len))
				m_HesternalPlace[type] = *(WORD*)(msg+12+len);
		}

		return;
	}
	if((type>>8)>=32) return;
	if(iLen<14) return;

	g_RangeVesion[type>>8] =version;
	WORD start_place = *(WORD*)(msg+12);
	if(start_place==0 && type != 0x0600) return; //沙城行会特殊，用smalltype=1表示

	if(g_HaveSBK && type == 0x0600) //如果是有沙城行会
	{
		start_place += 1;
	}
	void *temppvData = NULL;
	LP_RANGE_DADA pRANGE_DADA = NULL;
	for(int i=0;i<num;i++)
	{
		int len = NewStruct(type,&pvData);
		if(pvData)
		{
			if(iLen<12+(i+1)*len)
			{
				DeleteStruct(type,pvData);
				break;
			}
		}
		else
			return;
		bool isOk =  FindData(type,start_place+i,&temppvData,&pRANGE_DADA);

		if(isOk)	//找到已有数据,先删除
		{
			DeleteStruct(type,temppvData);
			delete (*pRANGE_DADA)[start_place+i];
			(*pRANGE_DADA)[start_place+i] = NULL;
		}

		memcpy(pvData,msg+12+i*len,len);
		STRUCT_RANGE_DADA * structdata = new  STRUCT_RANGE_DADA ;
		structdata->version = version;
		structdata->vData = pvData;
		(*pRANGE_DADA)[start_place+i] = structdata;

	}

}

void CGameControl::MSG_Range_FindOtherPlace_Result(const char *msg,int iLen)
{

	WORD type = *(WORD*)(msg+6);
	WORD wPlace = *(WORD*)(msg+8);

	if(iLen>12)
	{
		string name;
		name.assign(msg + 12,iLen-12);
		if(name == g_strFindRange)
		{			
			if(g_pControl->FindWindowByName("RangeWnd") != NULL)
				g_pControl->MsgToWnd(MSG_CTRL_RANGE_WND,MSG_CTRL_RANGE_GOTO_PLACE,wPlace);
		}
	}

}
void CGameControl::MSG_Range_Friend_Result(const char *msg,int iLen)
{
	WORD type = *(WORD*)(msg+6);
	WORD friendRangUp = *(WORD*)(msg+8);  // wYes=1是否有好友名次上升，wYes=0没有

	int i=0;
	while(i<iLen - 12)
	{	
		if(iLen - 12 - i <2)
			break;
		char cQushi = msg[12+i];
		string name = msg +13+i;
		i = i+1+(int)name.size();

		VRelationStruct::iterator itr;
		for(itr=g_OtherData.GetRelationVector().begin();itr!=g_OtherData.GetRelationVector().end();itr++)
		{
			if((*itr).strName == name)
			{
				(*itr).cQushi = cQushi;
				if(cQushi==1) //放到好友列表前列
				{
					_RelationStruct ft = (*itr);
					g_OtherData.GetRelationVector().erase(itr);
					g_OtherData.GetRelationVector().insert(g_OtherData.GetRelationVector().begin(),ft);
				}
				break;
			}
		}

	}

}

//发送的消息
//type=0:获取菜单
//type=1;更新自身数据
void CGameControl::Send_Range_Sign_Up(DWORD id,WORD type)
{
	SET_COMMAND(CS_RANGE_SIGN_UP,12);
	ASSIGN_ID(id);
	ASSIGN_WORD(6,type);
	SEND_GAME_SERVER();
}

void CGameControl::Send_Range_Request(DWORD id,WORD type,WORD version,WORD start)
{
	SET_COMMAND(CS_RANGE_REQUEST,12);
	ASSIGN_ID(id);
	ASSIGN_WORD(6,type);
	ASSIGN_WORD(8,version);
	ASSIGN_WORD(10,start);
	SEND_GAME_SERVER();
}

void CGameControl::Send_Range_FindOtherPlace(WORD type,const char * name)
{
	int nameLen = strlen(name);
	if(nameLen>188) return;

	SET_COMMAND(CS_RANGE_FINDOTHERPLACE,256);
	ASSIGN_WORD(6,type);
	ADD_STR(name);
	SEND_GAME_SERVER();
}

void CGameControl::Send_Range_Friend(WORD type)
{
	SET_COMMAND(CS_RANGE_FRIEND,12);
	ASSIGN_WORD(6,type);
	SEND_GAME_SERVER();
}

/*
1.	等级排名
2.	名师排名
3.	元神排名
4.	宠物豹
5.	锻造武器
6.	行会排行
7.	灭魔排行
8、 英雄擂台排名
9、 恶魔城排名
10、武馆排名
13. 宗族排名
*/
int NewStruct(WORD type,void **RangeData)
{
	int len =0,iAddLen = 0;
	switch(type>>8)
	{
	case 1:	
		len = sizeof(ALL_RANGE);
		*RangeData = new ALL_RANGE;
		break;
	case 2:
		len = sizeof(TEACHER_RANGE);		
		*RangeData = new TEACHER_RANGE;
		break;
	case 4:	
		len = sizeof(PET_RANGE);
		*RangeData = new PET_RANGE;
		break;
	case 5:
		len = sizeof(ZHUZAO_RANGE);		
		*RangeData = new ZHUZAO_RANGE;
		break;
	case 6:	
		len = sizeof(GUILD_RANGE);		
		*RangeData = new GUILD_RANGE;
		break;
	case 7:	
		len = sizeof(MIEMO_RANGE);		
		*RangeData = new MIEMO_RANGE;		
		break;
	case 8:
		len = sizeof(ARENA_RANGE);		
		*RangeData = new ARENA_RANGE;
		break;
	case 9:
		len = sizeof(RS_RANGE);		
		*RangeData = new RS_RANGE;
		break;
	case 10:
		len = sizeof(WUGUAN_RANGE);
		*RangeData = new WUGUAN_RANGE;
		break;
	case 13:
		len = sizeof(PHYLE_RANGE);
		*RangeData = new PHYLE_RANGE;
		break;
	case 14:
		len = sizeof(PRODUCE_SKILL_RANGE);
		*RangeData = new PRODUCE_SKILL_RANGE;
		break;
	case 15:
		len = sizeof(RTS_RANGE);
		*RangeData = new RTS_RANGE;
		break;
	case 16:
		len = sizeof(RTS_FIRE);
		*RangeData = new RTS_FIRE;
		break;
	case 17:
		len = sizeof(PINNA_RANGE);
		*RangeData = new PINNA_RANGE;
		break;
	case 18:
		len = sizeof(HORSEMANSHIP_RANGE);
		*RangeData = new HORSEMANSHIP_RANGE;
		break;

	default:
		return 0;

	}
	ZeroMemory(*RangeData,len);
	return len - iAddLen;	
}

bool FindData(WORD type,WORD startPlace ,void **RangeData,LP_RANGE_DADA *lpRangMap)
{
	if(m_mapRangeData[type]==NULL)
	{
		*lpRangMap = m_mapRangeData[type] = new RANGE_DADA;
		return false;
	}
	*lpRangMap = m_mapRangeData[type];

	RANGE_DADA::iterator itr;
	if((itr = (*lpRangMap)->find(startPlace))!=(*lpRangMap)->end())
	{
		*RangeData = itr->second;
		if((*RangeData) != NULL)
		{
			*RangeData = ((STRUCT_RANGE_DADA*)(*RangeData))->vData;
			return true;
		}
	}
	return false;

}

void DeleteStruct(WORD type,void	*RangeData)
{
	if(RangeData==NULL) return;

	switch(type>>8)
	{
	case 1:	
		delete (ALL_RANGE*)RangeData;
		break;
	case 2:
		delete (TEACHER_RANGE*)RangeData;
		break;
	case 4:	
		delete (PET_RANGE*)RangeData;
		break;
	case 5:
		delete (ZHUZAO_RANGE*)RangeData;
		break;
	case 6:	
		delete (GUILD_RANGE*)RangeData;
		break;
	case 7:	
		delete (MIEMO_RANGE*)RangeData;
		break;
	case 8:
		delete (ARENA_RANGE*)RangeData;
		break;
	case 9:
		delete (RS_RANGE*)RangeData;
		break;
	case 10:
		delete (WUGUAN_RANGE*)RangeData;
		break;
	case 13:
		delete (PHYLE_RANGE*)RangeData;
		break;
	case 14:
		delete (PRODUCE_SKILL_RANGE*)RangeData;
		break;
	case 15:
		delete (RTS_RANGE*)RangeData;
		break;
	case 16:
		delete (RTS_FIRE*)RangeData;
		break;

	default:
		return ;
	}
	RangeData = NULL;
}

void ClearAllRangeData()
{
	ALL_RANGE_DADA::iterator big_itr;
	RANGE_DADA::iterator range_itr;

	WORD type;
	for(big_itr=m_mapRangeData.begin();big_itr!=m_mapRangeData.end();big_itr++)
	{
		type = big_itr->first;

		for(range_itr=big_itr->second->begin();range_itr!=big_itr->second->end();range_itr++)
		{
			STRUCT_RANGE_DADA* pRange = ((STRUCT_RANGE_DADA *)(range_itr->second));
			if(pRange != NULL)
			{
				DeleteStruct(type,pRange->vData);
				delete (STRUCT_RANGE_DADA *)(range_itr->second);
				(range_itr->second) = NULL;
			}
		}

		if(big_itr->second != NULL)
		{
			delete (LP_RANGE_DADA)(big_itr->second);
		}

	}
	for(big_itr=m_mapMyRangeData.begin();big_itr!=m_mapMyRangeData.end();big_itr++)
	{
		type = big_itr->first;

		for(range_itr=big_itr->second->begin();range_itr!=big_itr->second->end();range_itr++)
		{
			STRUCT_RANGE_DADA* pRange = ((STRUCT_RANGE_DADA *)(range_itr->second));
			if(pRange != NULL)
			{
				DeleteStruct(type,pRange->vData);
				delete (STRUCT_RANGE_DADA *)(range_itr->second);
				(range_itr->second) = NULL;
			}
		}
		if(big_itr->second != NULL)
		{
			delete (LP_RANGE_DADA)(big_itr->second);
		}
	}
	m_mapRangeData.clear();
	m_mapMyRangeData.clear();
	m_HesternalPlace.clear();
}

void ClearMyRangeData()
{
	ALL_RANGE_DADA::iterator big_itr;
	RANGE_DADA::iterator range_itr;

	WORD type;

	for(big_itr=m_mapMyRangeData.begin();big_itr!=m_mapMyRangeData.end();big_itr++)
	{
		type = big_itr->first;

		for(range_itr=big_itr->second->begin();range_itr!=big_itr->second->end();range_itr++)
		{
			STRUCT_RANGE_DADA* pRange = ((STRUCT_RANGE_DADA *)(range_itr->second));
			if(pRange != NULL)
			{
				DeleteStruct(type,pRange->vData);
				delete (STRUCT_RANGE_DADA *)(range_itr->second);
				(range_itr->second) = NULL;
			}
		}
		if(big_itr->second != NULL)
		{
			delete (LP_RANGE_DADA)(big_itr->second);
		}
	}
	m_mapMyRangeData.clear();
	m_mapMyRangeQushi.clear();
	m_HesternalPlace.clear();
}