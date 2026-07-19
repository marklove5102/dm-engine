#include "MagicCtrlMgr.h"
#include "GameData/ActionStruct.h"
#include "GameData/GameData.h"
#include "BaseClass/Control/Control.h"
#include "BaseClass/TiXml/tinyxml.h"
#include "GameData/GameGlobal.h"
#include "GameData/OtherData.h"
#include "Global/Interface/AudioInterface.h"
#include "GameData/ConfigData.h"


IMPLEMENT_POOL(Magic_Ref_s,128,16);
IMPLEMENT_POOL(Magic_Show_s,128,16);

bool Magic_Show_s::IsEnd(DWORD tTime)
{
	if(iFrameSpeed == 0)
		return true;

	if(iFrameCount == 0)
		return true;

	if(tFrameStart == 0)
		return false;

	if((tTime - tFrameStart) >= iFrameSpeed * iFrameCount)
		return true;

	return false;
}

int Magic_Show_s::GetFrame(DWORD tTime) //计算当前帧
{
	DWORD tPass = 0;
	if(tFrameStart != 0)
		tPass = tTime - tFrameStart;
	else
		tFrameStart = tTime;

	if(iFrameSpeed == 0 || iFrameCount == 0)
		return 0;

	return (tPass / iFrameSpeed);
}

//----------------------------------------------------------------------
#define	 MAGIC_FILE_ID     "magic des"

CMagicCtrlMgr::CMagicCtrlMgr()
{
	m_ShowList = NULL;
	m_iCreatedMagicId = 0;
	m_strPath = StringUtil::format("%s\\config\\magicfile.dat",GetGameDataDir());

	LoadFile();
}

CMagicCtrlMgr::~CMagicCtrlMgr()
{
	Clear();
}

//	销毁结点
void CMagicCtrlMgr::DestroyNode(Magic_Node_s *node) 
{
	if(node == NULL)
		return;

	if(node->child)
		DestroyNode(node->child);
	if(node->sibling)
		DestroyNode(node->sibling);

	SAFE_DELETE(node);
}

//递归查找一个第一个具有指定属性的节点;
//用于解决根据状态启动某个指定节点的特殊情况；如中毒、麻痹、土神守护等基于主体状态的特效
//这些特效具有一个普遍的特性：就是这些特效可能都具有起首，但考虑到人物的走动，我们并不能
//重复启动(起首式)，而只能直接启动循环状态节点；因为在主动触发者和/或特效拥有者眼中，必然
//要具有起首式；而在某一个中间态，特效拥有者进入其他观察者视野的时候，该特效应当立即启动，
//并进入循环状态，而不应当有起首；
//额外的解释就是：起首式是个短暂态，成为过去式之后，就不能重复，无论对所有者还是观察者。

//（为了达到能够获得高度为h、并具有该属性的首节点，应当优先将该节点置于大儿子的位置。）
//不是一般性：深度优先
Magic_Node_s* CMagicCtrlMgr::FindNodeByAttr(Magic_Node_s* parent,DWORD attr)
{
	Magic_Node_s* ret  = NULL;

	if (parent == NULL)			  	//自身
		return NULL;

	if(parent->attr & attr)
		return parent;

	if(parent->child)	//如果存在儿子;
	{
		ret = FindNodeByAttr(parent->child,attr);
		if(ret)
			return ret;
	}

	if(parent->sibling)	//如果存在兄弟
	{
		ret = FindNodeByAttr(parent->sibling,attr);
		if(ret)
			return ret;
	}
	return NULL;
}

//查找魔法根
Magic_Root_s *CMagicCtrlMgr::GetMagicRoot(WORD	id)
{
	MMagic_Root_s::iterator itr = m_MagicRoots.find(id);
	if(itr == m_MagicRoots.end())
		return NULL;

	return &itr->second;
};	

//查找魔法单元
Magic_Unit_s *CMagicCtrlMgr::GetMagicUnit(WORD	id)
{
	MMagic_Unit_s::iterator itr = m_MagicUnits.find(id);

	if(itr == m_MagicUnits.end())
		return NULL;

	return &itr->second;
}	 

Magic_Unit_s* CMagicCtrlMgr::GetMagicUnitByRoot(WORD id)
{
	Magic_Root_s* root = GetMagicRoot(id);
	if(root == NULL)
		return NULL;

	if(root->m && root->m->child)
	{
		return GetMagicUnit(root->m->child->unit_idx);
	}
	return NULL;
}

// 从一个Node查找到它所在的Root
Magic_Root_s* CMagicCtrlMgr::MagicNode2Root(Magic_Node_s *pNode)
{
	if(!pNode)
		return NULL;

	if(m_MagicRoots.size() == 0)
		return NULL;

	Magic_Node_s *rootNode = pNode;		// 根结点
	while (rootNode->parent)
	{
		rootNode = rootNode->parent;
	}

	MMagic_Root_s::iterator itr;

	for(itr = m_MagicRoots.begin();itr != m_MagicRoots.end();++itr)
	{
		Magic_Root_s& root = itr->second;
		if (root.m == rootNode)
		{
			return &root;
		}
	}
	return NULL;
}

void CMagicCtrlMgr::Clear()
{
	FinishAllMagic();

	while(!m_MagicRoots.empty())
	{
		MMagic_Root_s::iterator itr = m_MagicRoots.begin();
		DestroyNode(itr->second.m);
		m_MagicRoots.erase(itr);
	}
	m_MagicUnits.clear();
}

bool CMagicCtrlMgr::ReadNode(Magic_Node_s *p,int deep) //递归读取结点
{
	if (p == NULL)//第一个儿子
		return false;

	fread(&p->unit_idx,sizeof(WORD),1,fp);
	fread(&p->cycle,sizeof(WORD),1,fp);
	fread(&p->attr,sizeof(DWORD),1,fp);
	fread(&p->speed,sizeof(WORD),1,fp);
	fread(&p->flyspeed,sizeof(WORD),1,fp);
	fread(&p->offx,sizeof(short),1,fp);
	fread(&p->offy,sizeof(short),1,fp);
	fread(&p->sound,sizeof(WORD),1,fp);	
	fread(&p->byRev1,sizeof(BYTE),1,fp);
	fread(&p->iRev1,sizeof(int),1,fp);

	deep++;

	WORD hasChild = 0,hasSibling = 0;

	fread(&hasChild,sizeof(WORD),1,fp);
	fread(&hasSibling,sizeof(WORD),1,fp);

	if(hasChild)		//儿子
	{
		Magic_Node_s* child = new Magic_Node_s;
		child->parent = p;
		p->child = child;
		ReadNode(child,deep);
	}

	if(hasSibling)		//兄弟
	{
		Magic_Node_s* ppp = new Magic_Node_s;
		ppp->parent = p->parent;
		p->sibling = ppp;
		ReadNode(ppp,deep);
	}
	return true;
}

bool CMagicCtrlMgr::LoadFile()
{
	Clear();

	fp = fopen(m_strPath.c_str(),"rb");
	if (NULL == fp)
		return false;

	char buf[32] = {0};
	fread(buf,sizeof(char),16,fp);

	//判断数据是否完好
	if (strcmp(buf,MAGIC_FILE_ID) != 0)
	{
		fclose(fp);
		return false;
	}

	WORD wMinVer = 0,wMainVer = 0;
	fread(&wMinVer,sizeof(WORD),1,fp);
	fread(&wMainVer,sizeof(WORD),1,fp);

	if(wMinVer != 0x0001 && wMainVer > 2)
	{
		fclose(fp);
		return false;
	}

	//读取--魔法
	int count =0;
	fread(&count,sizeof(int),1,fp);

	MMagic_Root_s::iterator itr;

	for(int i = 0;i<count;++i)
	{
		Magic_Root_s t;

		fread(&t.id,sizeof(DWORD),1,fp);
		fread(&t.attr,sizeof(DWORD),1,fp);

		if(wMainVer <= 1)
			fread(t.name,1,16,fp);
		else
			fread(t.name,1,NAME_LEN,fp);

		fread(&t.d1,sizeof(DWORD),1,fp);
		fread(&t.d2,sizeof(DWORD),1,fp);
		fread(&t.d3,sizeof(DWORD),1,fp);
		fread(&t.d4,sizeof(DWORD),1,fp);

		WORD hasChild = 0;
		WORD hasSibling = 0;

		fread(&hasChild,sizeof(WORD),1,fp);
		fread(&hasSibling,sizeof(WORD),1,fp);

		t.m = new Magic_Node_s();

		if(hasChild)   ////根节点没有邻居
		{
			t.m->child = new Magic_Node_s;
			t.m->child->parent = t.m;
			ReadNode(t.m->child,0);
		}

		itr = m_MagicRoots.find(t.id);
		if(itr != m_MagicRoots.end())
		{
#ifdef _DEBUG
			output_debug("重复魔法id = %d\n",t.id);
#endif
			DestroyNode(itr->second.m);
			m_MagicRoots.erase(itr);
		}
		m_MagicRoots[t.id] = t;
	}

	//读取--魔法单元
	fread(&count,sizeof(int),1,fp);

	MMagic_Unit_s::iterator itr1;

	for(int i = 0;i<count;++i)
	{
		Magic_Unit_s t;
		fread(&t,sizeof(Magic_Unit_s),1,fp);

		itr1 = m_MagicUnits.find(t.id);
		if(itr1 != m_MagicUnits.end())
		{
#ifdef _DEBUG
			output_debug("重复图组id = %d\n",t.id);
#endif
			m_MagicUnits.erase(itr1);
		}
		m_MagicUnits[t.id] = t;
	}

	fclose(fp);
	fp = NULL;
	return true;
}

bool CMagicCtrlMgr::ReadXmlNode(TiXmlElement* node,Magic_Node_s *p,int deep)
{
	if (p == NULL)//第一个儿子
		return false;

	TiXmlAttribute* attr = node->FirstAttribute();
	while(attr)
	{
		if(stricmp(attr->Name(),"unit_idx") == 0)
			p->unit_idx = attr->IntValue();
		else if(stricmp(attr->Name(),"attr") == 0)
			p->attr = strtoul(attr->Value(),NULL,16);
		else if(stricmp(attr->Name(),"cycle") == 0)
			p->cycle = attr->IntValue();
		else if(stricmp(attr->Name(),"speed") == 0)
			p->speed = attr->IntValue();
		else if(stricmp(attr->Name(),"flyspeed") == 0)
			p->flyspeed = attr->IntValue();
		else if(stricmp(attr->Name(),"offx") == 0)
			p->offx = attr->IntValue();
		else if(stricmp(attr->Name(),"offy") == 0)
			p->offy = attr->IntValue();
		else if(stricmp(attr->Name(),"sound") == 0)
			p->sound = attr->IntValue();
		else if(stricmp(attr->Name(),"byRev1") == 0)
			p->byRev1 = attr->IntValue();
		else if(stricmp(attr->Name(),"iRev1") == 0)
			p->iRev1 = attr->IntValue();

		attr = attr->Next();
	}

	deep++;

	TiXmlElement* childNode = node->FirstChildElement();
	if(childNode)  //儿子
	{
		Magic_Node_s* child = new Magic_Node_s;
		child->parent = p;
		p->child = child;
		ReadXmlNode(childNode,child,deep);
	}


	TiXmlElement* nextNode = node->NextSiblingElement();
	if(nextNode)		//兄弟
	{
		Magic_Node_s* ppp = new Magic_Node_s;
		ppp->parent = p->parent;
		p->sibling = ppp;
		ReadXmlNode(nextNode,ppp,deep);
	}
	return true;
}

bool CMagicCtrlMgr::LoadXmlFile()
{
	Clear();

	TiXmlDocument xmlDoc;
	if(!xmlDoc.LoadFile(m_strPath.c_str())) //读取失败
		return false;

	TiXmlElement* rootNode = xmlDoc.RootElement();

	TiXmlElement* attrNode = rootNode->FirstChildElement();
	for(;attrNode;attrNode = attrNode->NextSiblingElement())
	{
		if(stricmp(attrNode->Value(),"MagicTree") == 0)
		{
			MMagic_Root_s::iterator itr;

			TiXmlElement* node = attrNode->FirstChildElement();
			for(;node; node = node->NextSiblingElement())
			{
				Magic_Root_s t;
				t.d1 = t.d2 = t.d3 = t.d4 = 0;

				for(TiXmlAttribute* attr = node->FirstAttribute();attr; attr = attr->Next())
				{
					if(stricmp(attr->Name(),"magic_id") == 0)
						t.id = attr->IntValue();
					else if(stricmp(attr->Name(),"attr") == 0)
						t.attr = strtoul(attr->Value(),NULL,16);
					else if(stricmp(attr->Name(),"name") == 0)
						strncpy(t.name,attr->Value(),NAME_LEN);
					else if(stricmp(attr->Name(),"d1") == 0)
						t.d1 = attr->IntValue();
					else if(stricmp(attr->Name(),"d2") == 0)
						t.d2 = attr->IntValue();
					else if(stricmp(attr->Name(),"d3") == 0)
						t.d3 = attr->IntValue();
					else if(stricmp(attr->Name(),"d4") == 0)
						t.d4 = attr->IntValue();

				}
				t.m = new Magic_Node_s();

				TiXmlElement* child = node->FirstChildElement();
				if(child)
				{
					t.m->child = new Magic_Node_s;
					t.m->child->parent = t.m;
					ReadXmlNode(child,t.m->child,0);
				}

				itr = m_MagicRoots.find(t.id);
				if(itr != m_MagicRoots.end())
				{
#ifdef _DEBUG
					output_debug("重复魔法id = %d\n",t.id);
#endif
					SAFE_DELETE(itr->second.m);
					m_MagicRoots.erase(itr);
				}

				m_MagicRoots[t.id] = t;
			}
		}
		else if(stricmp(attrNode->Value(),"MagicList") == 0)
		{
			MMagic_Unit_s::iterator itr1;

			TiXmlElement* node = attrNode->FirstChildElement();
			for(;node;node = node->NextSiblingElement())
			{
				Magic_Unit_s t;

				for(TiXmlAttribute* attr = node->FirstAttribute();attr;attr = attr->Next())
				{
					if(stricmp(attr->Name(),"unit_id") == 0)
						t.id = attr->IntValue();
					else if(stricmp(attr->Name(),"tex") == 0)
						t.tex = attr->IntValue();
					else if(stricmp(attr->Name(),"frames") == 0)
						t.frames = attr->IntValue();
					else if(stricmp(attr->Name(),"framereal") == 0)
						t.framereal = attr->IntValue();
					else if(stricmp(attr->Name(),"name") == 0)
						strncpy(t.name,attr->Value(),NAME_LEN);
					else if(stricmp(attr->Name(),"dirs") == 0)
						t.dirs = attr->IntValue();
					else if(stricmp(attr->Name(),"package") == 0)
						t.package = attr->IntValue();
					else if(stricmp(attr->Name(),"alpha") == 0)
						t.alpha = attr->IntValue();
				}

				itr1 = m_MagicUnits.find(t.id);
				if(itr1 != m_MagicUnits.end())
				{
#ifdef _DEBUG
					output_debug("重复图组id = %d\n",t.id);
#endif
					m_MagicUnits.erase(itr1);
				}
				m_MagicUnits[t.id] = t;
			}
		}
	}
	return true;
}


#ifdef _DEBUG
void CMagicCtrlMgr::DelMagicTex(WORD wMagicID)
{
	if(wMagicID == 0)
		return;

	Magic_Root_s *pRoot = GetMagicRoot(wMagicID);
	Magic_Show_s *show_ret = NULL;

	if(!pRoot || !pRoot->m || !pRoot->m->child)
		return;

	DelMagicTex(pRoot->m->child);
}

void CMagicCtrlMgr::DelMagicTex(Magic_Node_s *pNode)	
{
	if (NULL == pNode)
		return;

	///////////////////
	Magic_Unit_s * pUnit = NULL;
	while (pNode)
	{
		//先删除子结点
		if (pNode->child)
		{
			DelMagicTex(pNode->child);
		}

		//再删除自己
		if(pUnit = GetMagicUnit(pNode->unit_idx))
		{
			int iBegin = pUnit->tex;
			int iEnd = iBegin + pUnit->dirs * pUnit->frames;
			g_pTexMgr->DelTexFile(PACKAGE_magic1+pUnit->package,iBegin,iEnd);

		}
		//再兄弟结点
		pNode = pNode->sibling;//查找下一个兄弟节点
	}
}
#endif





//------------------------------------------------------------------------------
// 通用魔法创建程序
// wMagicType -- 魔法显示和创建的控制参数
//				1.是否需要等待服务器==EMP_MAGIC_WAIT
//				2.是否需要直接进入loop state ==EMP_MAGIC_LOOP
//				3.需要iMagicDir = 主体的方向，创建时初始设置，后期直接根据父节点来获得；目前想到的情况都可以解决了。	
//               iMagicDir = -1时根据owner和target计算方向,这个传进去的方向必须是相对16方向来说的，showmagic中会根据实际方向数转化，如果相对的是8方向的乘以2再传进去
//				4.如果是服务端请求的特殊魔法，必须指定EMP_MAGIC_SERVER_ADVISE属性；
//创建显示,优先从cache中获得
Magic_Show_s* CMagicCtrlMgr::CreateMagic(WORD wMagicID,DWORD dwMagicAttr,UINT uOwnerID,UINT uTargetID,int iMagicDir,DWORD dwColor,int iOffX,int iOffY,Magic_Show_s** pShowList,bool bCanRepeat)
{
	if(!g_Config.IsEnableMagic())
		return NULL;

	if(wMagicID == 0)
		return NULL;

	if (!bCanRepeat)
	{
		if (FindMagicByAll(wMagicID,uOwnerID,uTargetID,dwMagicAttr))
		{
			return NULL;
		}
	}

	Magic_Root_s *pRoot = GetMagicRoot(wMagicID);
	Magic_Show_s *show_ret = NULL;

	if(!pRoot || !pRoot->m || !pRoot->m->child)
		return NULL;

	if(pRoot->attr & EMP_MAGIC_SERVER_ADVISE)	//服务端魔法必须检查创建参数
	{
		if((dwMagicAttr & EMP_MAGIC_SERVER_ADVISE) == 0)
			return NULL;
	}

	int iOwnerDir = -1;
	Magic_Unit_s *pUnit = NULL;
	Magic_Node_s *pNextNode = pRoot->m->child;
	Magic_Ref_s  *ref = NEW_MAGIC_REF();
	memset(ref,0,sizeof(Magic_Ref_s));

	ref->create_id	= m_iCreatedMagicId++;		//可以根据该标识来查找所有的效果部分
	ref->create_time = GetTickCount(); //创建时间
	ref->ref_count	= 0;

	//状态变更魔法：所有通过状态变化产生的中间系统，必须直接过度loop ？？
	ref->wMagicID	= wMagicID;
	ref->uOwnerID	= uOwnerID;
	ref->uTargetID	= uTargetID;
	ref->attr = dwMagicAttr;
	ref->wExtraAttr = 0;
	ref->iOffX = iOffX;
	ref->iOffY = iOffY;
	if(pShowList == NULL)
		ref->ppShowList = &m_ShowList;
	else
		ref->ppShowList = pShowList;


	int iX,iY;
	//计算源参量；
	if((dwMagicAttr & EMP_MAGIC_NOOWNER) != 0)
	{
		ref->wOriginTileX = LOWORD(uOwnerID);
		ref->wOriginTileY = HIWORD(uOwnerID);
		uOwnerID = 0;
	}
	else
	{
		CSimpleCharacter * pChar = g_pGameData->FindCharacterByID(uOwnerID);
		if(pChar) 
		{
			pChar->GetXY(iX,iY);
			ref->wOriginTileX = iX;
			ref->wOriginTileY = iY;
			iOwnerDir = pChar->GetDir();
		}
		else
		{
			if (uOwnerID != 0 && uOwnerID != -1)
			{
				DEL_MAGIC_REF(ref);
				return NULL;//如果不返回NULL有可能会出现预料不到的现象,比如随机位置出现一个魔法等
			}

			ref->wOriginTileX = LOWORD(uOwnerID);
			ref->wOriginTileY = HIWORD(uOwnerID);
			uOwnerID = 0;
		}

		if (uOwnerID == SELF.GetID() && uTargetID == 0)
		{
			uTargetID = g_pControl->GetMouseOnID();
		}
	}

	//计算目的参数
	if((dwMagicAttr & EMP_MAGIC_NOTARGET) != 0 && ref->uTargetID != 0)
	{
		ref->uTargetID = 0;
		ref->wTargetTileX = LOWORD(uTargetID);
		ref->wTargetTileY = HIWORD(uTargetID);
	}
	else
	{
		CSimpleCharacter * pChar = g_pGameData->FindCharacterByID(uTargetID);
		if(pChar) 
		{ 
			pChar->GetXY(iX,iY);
			ref->wTargetTileX = iX;
			ref->wTargetTileY = iY;	
		}
		else
		{
			if (uTargetID != 0 && uTargetID != -1)
			{
				DEL_MAGIC_REF(ref);
				return NULL;//如果不返回NULL有可能会出现预料不到的现象,比如随机位置出现一个魔法等
			}

			ref->uTargetID = 0;
			ref->wTargetTileX = LOWORD(uTargetID);
			ref->wTargetTileY = HIWORD(uTargetID);

			//ref->wTargetTileX = 0;
			//ref->wTargetTileY = 0;
			
			//if (iMagicDir == - 1 && iOwnerDir >= 0)//有owner,即使没有target,飞行魔法可以向着owner的方向飞行,如果不是飞行魔法,比如EMP_MAGIC_OBJECT,会在showmagic的时候自动结束自己
			//{
			//	iMagicDir = iOwnerDir * 2;
			//}
		}
	}

	int iDir = 0;
	switch (wMagicID)
	{
	case MAGICID_HELL_FIRE:
	case MAGICID_TRACE_THUNDER:
	case MAGICID_ICE_DRAG:
	case MAGICID_SWT:
	case MAGICID_LEOPARD_SWT:
		iDir = GetLineDirect(ref->wOriginTileX,ref->wOriginTileY,ref->wTargetTileX,ref->wTargetTileY)*2%16;
		if (iDir < 0)
		{
			if(uOwnerID == SELF.GetID())
			{
				iDir = SELF.GetDir() * 2;
			}
			else 
			{
				CSimpleCharacterNode * pChar = g_pGameData->FindSimpleCharacter(uOwnerID);
				if(pChar) 
				{
					iDir = pChar->GetDir() * 2;
				}
				else
				{
					iDir = 0;
				}
			}
		}
		break;
	default:
		{
			if(iMagicDir < 0)
				iDir = GetDir16(ref->wTargetTileX,ref->wTargetTileY,ref->wOriginTileX,ref->wOriginTileY);
			else
				iDir = iMagicDir;
		}
		break;
	}

	ref->iDir = iDir;

	//如果请求创建loop中间态，那么将直接进入中间态
	//递归产生所有效果
	//当需要循环时，找到第一个具有EMP_MAGIC_LOOP属性的节点
	//对于状态特效：只要设定loop属性就可以了；其启动和消亡，根据状态位的变化操作。

	if((ref->attr & EMP_MAGIC_LOOP) != 0)
	{
		pNextNode = FindNodeByAttr(pNextNode,EMP_MAGIC_LOOP);
	}

	while (pNextNode)
	{
		if(pUnit = GetMagicUnit(pNextNode->unit_idx))
		{
			//创建存储空间
			Magic_Show_s * show = CreateMagicShow(ref->ppShowList);
			if (show)
			{
				int iShowDir = ref->iDir;				
				if(pUnit->dirs > 0 && pUnit->dirs < 16)//ms->iDir是按16方向得到的，如果pUnit->dirs小于16方向，把ms->iDir转化成pUnit对应的方向
				{
					iShowDir = iShowDir*pUnit->dirs/16 % pUnit->dirs;
				}
				
				show->fx				= TILE_WIDTH / 2;	//原始偏移
				show->fy				= TILE_HEIGHT / 2;

				//复制父节点的基本参数				
				show->ref = ref;
				show->pUnit = pUnit;

				ref->ref_count++;				//本魔法创建显示效果，参考
				show->show_node	= pNextNode;	//当前魔法显示所参考的图片节点
				show->attr = pNextNode->attr | ref->attr;   //属性
				//if(ref->attr & EMP_MAGIC_SERVER_ADVISE)
				//	show->attr &= ~EMP_MAGIC_WAIT_SERVER;

				show->iCycles = pNextNode->cycle;//循环次数
				show->iOffX = pNextNode->offx + ref->iOffX;//偏移
				show->iOffY = pNextNode->offy + ref->iOffY;//偏移
				show->wSound = pNextNode->sound;//音效
				show->iCurCycle = 0;
				show->iDir		= iShowDir;		//方向
				show->iFrameDir	= pUnit->frames;	//每个方向图片间隔数
				show->iFrameCount	= pUnit->framereal;	//图片总帧数
				show->iFrameReal = pUnit->framereal;//实际帧数	
				show->byRev1 = pNextNode->byRev1;
				show->iRev1 = pNextNode->iRev1;

				//配了EMP_MAGIC_LOOP而且show->iCycles > 0,且没有配EMP_MAGIC_END_TIME,用EMP_MAGIC_LOOP表示一个循环的帧数(show->iFrameCount),比如护身真气MAGICID_PROTECT_SKIN
				//配了EMP_MAGIC_END_TIME和EMP_MAGIC_LOOP用Cycles表示结束时间(*100ms)
				if((show->attr & EMP_MAGIC_LOOP) && (show->attr & EMP_MAGIC_END_TIME) == 0 && show->iCycles > 0)
				{
					show->iFrameCount = show->iCycles;
				}

				show->byAlpha = pUnit->alpha;//混合方式
				show->dwColor	= dwColor;	//绘制魔法的颜色

				if(pNextNode->speed == 0)			//播放速度
					show->iFrameSpeed = 60;
				else
					show->iFrameSpeed = pNextNode->speed;

				if(pNextNode->flyspeed == 0)			//fly速度
					show->iFlySpeed = 60;
				else
					show->iFlySpeed = pNextNode->flyspeed;

				if (show_ret == NULL)//创建成功了后返回第一个结点
				{
					show_ret = show;
				}
			}
		}
		pNextNode = pNextNode->sibling;					//查找下一个兄弟节点
	}
	if(ref->ref_count == 0)										//没有创建成功
	{
		DEL_MAGIC_REF(ref);
	}
	else
	{
		//没有动作
	}

	return show_ret;											//返回最后创建的魔法；位于显示列表的首部；如果需要继续控制本显示的相关参数；可以在返回值后面，遍历同create_id魔法show.
 }

//创建本显示节点的后继显示
//FIXME:屏蔽掉带有起首动作的状态类魔法的循环状态；转移魔法所有者。
Magic_Show_s *CMagicCtrlMgr::CreateMagic(Magic_Show_s *ms)	
{
	if (NULL == ms)
		return NULL;

	if (NULL == ms->ref  || (ms->ref->attr & EMP_MAGIC_EXIT)
		||  ms->ref->ref_count == 0 || NULL == ms->show_node)		//impossible
	{
		return NULL;
	}

	int iattr2		 = 0;
	Magic_Root_s *pRoot = NULL;
	Magic_Unit_s *pUnit = NULL;
	Magic_Show_s *show = NULL;
	Magic_Node_s *pNextNode = ms->show_node->child;

	int random = -1;
	if(ms->show_node->attr & EMP_MAGIC_RANDOM_SONS)	
	{
		pNextNode = ms->show_node->child;

		int  nNodeIndex = 0;
		while(pNextNode) //查找下一个兄弟节点
		{
			pNextNode = pNextNode->sibling;
			++nNodeIndex;
		}

		if(nNodeIndex <1)
			nNodeIndex = 1;

		//特殊随即规则
		random = rand() % nNodeIndex;
	}

	pNextNode = ms->show_node->child;

	for (int  nNodeIndex = 0;pNextNode;pNextNode = pNextNode->sibling,++nNodeIndex)//查找下一个兄弟节点
	{
		if(ms->show_node->attr & EMP_MAGIC_RANDOM_SONS)	
		{
			if(nNodeIndex!=random)
				continue;
		}

		pRoot = g_pMagicCtrl->GetMagicRoot(ms->ref->wMagicID);

		//如果是状态魔法的循环节点：如果没有创建，则创建以攻击者为主体的循环帧
		//这里的逻辑是:所有的状态魔法,通过帧循环来创建,而不通过子节点创建;这样可以保证所有的状态魔法只会创建一次;
		if(pRoot->d2 && (pNextNode->attr & EMP_MAGIC_LOOP))
		{
			continue;
		}

		pUnit = GetMagicUnit(pNextNode->unit_idx);
		if(!pUnit)
		{
			continue;
		}
		//创建存储空间
		show = CreateMagicShow(ms->ref->ppShowList);
		if (show)
		{
			int iShowDir = ms->ref->iDir;				
			if(pUnit->dirs > 0 && pUnit->dirs < 16)//ms->iDir是按16方向得到的，如果pUnit->dirs小于16方向，把ms->iDir转化成pUnit对应的方向
			{
				iShowDir = iShowDir*pUnit->dirs/16 % pUnit->dirs;
			}

			show->fx				= TILE_WIDTH / 2;	//原始偏移
			show->fy				= TILE_HEIGHT / 2;

			show->ref	= ms->ref;
			show->pUnit = pUnit;

			show->ref->ref_count++;		//本魔法创建显示效果，参考
			show->attr = pNextNode->attr | show->ref->attr;//属性
			//if(show->ref->attr & EMP_MAGIC_SERVER_ADVISE)
			//	show->attr &= ~EMP_MAGIC_WAIT_SERVER;

			show->iDir	= iShowDir;		//在某些特殊的情况，需要继承父节点的方向
			show->show_node			= pNextNode;		//当前魔法显示所参考的图片节点
			show->iCycles = pNextNode->cycle;
			show->iOffX = pNextNode->offx + ms->ref->iOffX;//偏移
			show->iOffY = pNextNode->offy + ms->ref->iOffY;//偏移
			show->wSound = pNextNode->sound;//音效
			show->iCurCycle = 0;
			show->iFrameDir	= pUnit->frames;	//每个方向图片间隔数
			show->iFrameCount	= pUnit->framereal;	//图片总帧数
			show->iFrameReal = pUnit->framereal;//实际帧数	
			show->byRev1 = pNextNode->byRev1;
			show->iRev1 = pNextNode->iRev1;

			//配了EMP_MAGIC_LOOP而且show->iCycles > 0,且没有配EMP_MAGIC_END_TIME,用EMP_MAGIC_LOOP表示一个循环的帧数(show->iFrameCount),比如护身真气MAGICID_PROTECT_SKIN
			//配了EMP_MAGIC_END_TIME和EMP_MAGIC_LOOP用Cycles表示结束时间(*100ms)
			if((show->attr & EMP_MAGIC_LOOP) && (show->attr & EMP_MAGIC_END_TIME) == 0 && show->iCycles > 0)
			{
				show->iFrameCount = show->iCycles;
			}

			show->byAlpha = pUnit->alpha;//混合方式
			show->dwColor	= ms->dwColor;	//绘制魔法的颜色

			if(pNextNode->speed == 0)			//播放速度
				show->iFrameSpeed = 60;
			else
				show->iFrameSpeed = pNextNode->speed;

			if(pNextNode->flyspeed == 0)			//fly速度
				show->iFlySpeed = 60;
			else
				show->iFlySpeed = pNextNode->flyspeed;
		}
	}
	return show;
}

Magic_Show_s* CMagicCtrlMgr::FindMagicByID(WORD wMagicID,Magic_Show_s* pStart)
{
	Magic_Show_s *ms = NULL;

	if(pStart == NULL)
		ms = m_ShowList;
	else
		ms = pStart->pNext;

	while(ms)
	{
		if(ms->ref && ms->ref->wMagicID == wMagicID)
			return ms;

		ms = ms->pNext;
	}
	return NULL;
}

Magic_Show_s* CMagicCtrlMgr::FindMagicByUnitID(WORD wUnitID,DWORD dwTargetID)
{
	Magic_Show_s *ms = NULL;

	if(wUnitID == 0) 
		return NULL;
	ms = m_ShowList;
	while(ms)
	{
		if(ms->pUnit)
		{
			if(wUnitID == ms->pUnit->id && dwTargetID == ms->ref->uTargetID)
				return ms;
		}

		ms = ms->pNext;
	}
	return NULL;
}

Magic_Show_s* CMagicCtrlMgr::FindMagicByAll(WORD wMagicID,DWORD uOwnerID,DWORD uTargetID,DWORD dwIncludeAttr,DWORD dwExcludeAttr,Magic_Show_s* pStart,DWORD uMagicUint,int iOwnerPosX,int iOwnerPosY,int iTargetPosX,int iTargetPosY)
{
	for(Magic_Show_s *ms = pStart?pStart->pNext:m_ShowList; ms != NULL; ms = ms->pNext)
	{
		if(ms->ref)
		{
			if(wMagicID != 0 && ms->ref->wMagicID != wMagicID)
				continue;
			if(uOwnerID != -1 && ms->ref->uOwnerID != uOwnerID)
				continue;
			if(uTargetID != -1 && ms->ref->uTargetID != uTargetID)
				continue;
			if(dwIncludeAttr != -1 && (ms->attr & dwIncludeAttr) == 0)
				continue;
			if(dwExcludeAttr != -1 && (ms->attr & dwExcludeAttr) != 0)
				continue;
			if(uMagicUint != -1 && ms->pUnit && ms->pUnit->id != uMagicUint)
				continue;
			if(iOwnerPosX != -10000 && ms->ref->wOriginTileX != iOwnerPosX)
				continue;
			if(iOwnerPosY != -10000 && ms->ref->wOriginTileY != iOwnerPosY)
				continue;
			if(iTargetPosX != -10000 && ms->ref->wTargetTileX != iTargetPosX)
				continue;
			if(iTargetPosY != -10000 && ms->ref->wTargetTileY != iTargetPosY)
				continue;

			return ms;
		}
	}
	return NULL;
}

Magic_Show_s* CMagicCtrlMgr::FindMagicByCreatedID(int createId,Magic_Show_s* pStart)
{
	Magic_Show_s *ms = NULL;

	if(pStart == NULL)
		ms = m_ShowList;
	else
		ms = pStart->pNext;

	while(ms)
	{
		if(ms->ref && ms->ref->create_id == createId)
			return ms;

		ms = ms->pNext;
	}
	return NULL;
}

Magic_Show_s* CMagicCtrlMgr::FindMagicByOwner(UINT uOwnerID,Magic_Show_s* pStart)
{
	Magic_Show_s *ms = NULL;

	if(pStart == NULL)
		ms = m_ShowList;
	else
		ms = pStart->pNext;

	while(ms)
	{
		if(ms->ref && ms->ref->uOwnerID == uOwnerID)
			return ms;

		ms = ms->pNext;
	}
	return NULL;
}

Magic_Show_s* CMagicCtrlMgr::FindMagicByAttr(WORD wMagicID,UINT uOwnerID,DWORD attr,Magic_Show_s* pStart)
{
	Magic_Show_s* ms = FindMagicByOwner(uOwnerID,pStart);
	while(ms)
	{
		if(ms->ref && ms->ref->wMagicID == wMagicID && (ms->attr & attr) != 0)
			return ms;

		ms = FindMagicByOwner(uOwnerID,ms);
	}
	return NULL;
}

Magic_Show_s* CMagicCtrlMgr::FindMagicByPos(int iX,int iY,bool bTargetPos,DWORD dwCreateID,DWORD attr,Magic_Show_s* pStart)
{
	Magic_Show_s *ms = NULL;

	if(pStart == NULL)
		ms = m_ShowList;
	else
		ms = pStart->pNext;

	while(ms)
	{
		if(ms->ref)
		{
			if(bTargetPos && ms->ref->wTargetTileX == iX && ms->ref->wTargetTileY == iY && (ms->attr & attr) != 0 && (dwCreateID == 0 || dwCreateID == ms->ref->create_id))
			{
				return ms;
			}
			else if(!bTargetPos && ms->ref->wOriginTileX == iX && ms->ref->wOriginTileY == iY && (ms->attr & attr) != 0 && (dwCreateID == 0 || dwCreateID == ms->ref->create_id))
			{
				return ms;
			}
		}
		ms = ms->pNext;
	}

	return NULL;
}

Magic_Show_s *CMagicCtrlMgr::CreateMagicShow(Magic_Show_s** ppShowList)
{
	if(ppShowList == NULL)
		return NULL;

	Magic_Show_s *ms = NEW_MAGIC_SHOW();

	memset(ms,0,sizeof(Magic_Show_s));
	ms->tFrameStart = 0;
	ms->dwStartTime = GetTickCount();

	ms->pNext = *ppShowList;
	if(*ppShowList)
		(*ppShowList)->pPrev = ms;

	*ppShowList = ms;
	return *ppShowList;
}

void CMagicCtrlMgr::RemoveMagicShow(Magic_Show_s *ms)
{
	if(NULL == ms || ms->ref == NULL || ms->ref->ppShowList == NULL)
		return;


	//处理显示列表
	if(*(ms->ref->ppShowList) == ms)				//头指针
	{
		*(ms->ref->ppShowList) = (*(ms->ref->ppShowList))->pNext;
		if(*(ms->ref->ppShowList))
			(*(ms->ref->ppShowList))->pPrev = NULL;
	}
	else 
	{
		if(ms->pPrev)				//非头指针
			ms->pPrev->pNext = ms->pNext;
		if(ms->pNext)				
			ms->pNext->pPrev = ms->pPrev;
	}

	if (0 == ms->ref->ref_count)
	{
		DEL_MAGIC_REF(ms->ref);
		ms->ref = NULL;
	}

	DEL_MAGIC_SHOW(ms);
}

void CMagicCtrlMgr::FinishAllMagic(bool bJumpMap)
{
	Magic_Show_s* ms = m_ShowList,*ms2 = NULL;
	while(ms)
	{
		ms2 = ms->pNext;
		if(!bJumpMap || (ms->attr & EMP_MAGIC_CAN_JUMPMAP) == 0)//bJumpMap为true时有EMP_MAGIC_CAN_JUMPMAP这个属性的不结束掉
		{
			if(ms->ref->pTarget)
				TargetDataClear(ms->ref->pTarget);

			FinishMagic(ms);
		}
		ms = ms2;
	}

	g_OtherData.ClearAllFireShowList();

}

void CMagicCtrlMgr::FinishMagic(Magic_Show_s* ms)
{
	if(ms == NULL)
		return;

	//MAGICID_CREATE_MAGIC_WHEN_END:这个结点结束时创建新魔法
	if (ms->pUnit && ms->pUnit->id == MAGICID_CREATE_MAGIC_WHEN_END)
	{
		if(ms->ref)
		{
			DWORD dwOwnerID = ms->ref->uOwnerID;
			DWORD dwTargetID = ms->ref->uTargetID;
			if(dwOwnerID == 0)
				dwOwnerID = MAKELONG(ms->ref->wOriginTileX,ms->ref->wOriginTileY);
			if(dwTargetID == 0)
				dwTargetID = MAKELONG(ms->ref->wTargetTileX,ms->ref->wTargetTileY);

			if(ms->attr & EMP_MAGIC_HIT_TARGET)
			{
				DWORD dwTemp = dwTargetID;
				dwTargetID = dwOwnerID;
				dwOwnerID = dwTemp;
			}

			Magic_Show_s* pNewMs = g_pMagicCtrl->CreateMagic(ms->iRev1,ms->ref->attr,dwOwnerID,dwTargetID);

		}
	}

	if(ms->nSoundRand != 0 && (ms->attr & EMP_MAGIC_MUSIC_LOOP) != 0)
	{
		g_pAudio->Stop(EAT_MAGIC,ms->wSound,ms->nSoundRand);
	}

	if (ms->ref)
	{
		--ms->ref->ref_count;
		if (0 == ms->ref->ref_count)
		{
			if(ms->ref->pDynData)
			{
				SAFE_DELETE_ARRAY(ms->ref->pDynData);
			}

			if(ms->ref->pTarget && (ms->attr & EMP_MAGIC_POST_ATTACKED))
			{
				EffectAttacked(ms->ref->pTarget);
			}
			else if(ms->ref->pTarget)
			{
				STargetData *pSrcTarget = ms->ref->pTarget;		//保存pTarget，用于删除！
				for(;pSrcTarget;pSrcTarget = pSrcTarget->pNext)
				{					
					if(pSrcTarget->iType == 1)     //人物消失
					{
						MapArray.DeleteCharacter(pSrcTarget->dwID);     
					}					
				}
				TargetDataClear(ms->ref->pTarget);
			}
		}
	}


	RemoveMagicShow(ms);
}

void CMagicCtrlMgr::FinishMagicByID(WORD wMagicID)
{
    Magic_Show_s *ms = NULL;
	ms = FindMagicByID(wMagicID);
	while (ms)
	{
		FinishMagic(ms);
		ms = FindMagicByID(wMagicID);
	}
}
