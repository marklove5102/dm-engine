#pragma once

#include "MagicDefine.h"

class TiXmlElement;

class CMagicCtrlMgr
{
public:
	CMagicCtrlMgr();
	~CMagicCtrlMgr();

	//魔法显示
	Magic_Show_s*   GetShowList(){ return m_ShowList; }
	Magic_Show_s*	CreateMagic(WORD wMagicID,DWORD  dwMagicAttr = 0,UINT uOwnerID = 0,UINT uTargetID = 0,int iMagicDir = -1,DWORD dwColor = 0xFFFFFFFF,int iOffX = 0,int iOffY = 0,Magic_Show_s** pShowList = NULL,bool bCanRepeat = true);//iMagicDir = -1时根据owner和target计算方向,这个传进去的方向必须是相对16方向来说的，showmagic中会根据实际方向数转化，如果相对的是8方向的乘以2再传进去iOffX ,iOffY魔法偏移,对所有子结点有效,pShowList:创建好了以后放到那个list中
	Magic_Show_s *  CreateMagic(Magic_Show_s *ms);
	void            FinishMagic(Magic_Show_s* ms);
	void            FinishMagicByID(WORD wMagicID);
	void            FinishAllMagic(bool bJumpMap = false);//bJumpMap==true时有EMP_MAGIC_CAN_JUMPMAP这个属性的不会结束掉，否则全部结束

	Magic_Show_s*   FindMagicByID(WORD wMagicID,Magic_Show_s* pStart = NULL);
	Magic_Show_s*   FindMagicByAll(WORD wMagicID = 0,DWORD uOwnerID = -1,DWORD uTargetID = -1,DWORD dwIncludeAttr = -1,DWORD dwExcludeAttr = -1,Magic_Show_s* pStart = NULL,DWORD uMagicUint = -1,int iOwnerPosX = -10000,int iOwnerPosY = -10000,int iTargetPosX = -10000,int iTargetPosY = -10000);//-1表示不比较这个参数,dwIncludeAttr:包含的属性,dwExcludeAttr:不包含的属性
	Magic_Show_s*   FindMagicByUnitID(WORD wUnitID = 0,DWORD dwTargetID = 0);

	Magic_Show_s*   FindMagicByCreatedID(int createId,Magic_Show_s* pStart = NULL);
	Magic_Show_s*   FindMagicByOwner(UINT uOwnerID,Magic_Show_s* pStart = NULL);
	Magic_Show_s*   FindMagicByAttr(WORD wMagicID,UINT uOwnerID,DWORD attr,Magic_Show_s* pStart = NULL);
	Magic_Show_s*   FindMagicByPos(int iX,int iY,bool bTargetPos = false,DWORD dwCreateID = 0,DWORD attr = -1,Magic_Show_s* pStart = NULL);//根据位置找到魔法，默认为owner的位置,bTargetPos=true时为target的位置

	//魔法配置
	Magic_Root_s *  GetMagicRoot(WORD id);
	Magic_Unit_s *  GetMagicUnit(WORD id);
	Magic_Unit_s *  GetMagicUnitByRoot(WORD id);

	Magic_Root_s*	MagicNode2Root(Magic_Node_s *pNode); // 从一个node获取它的root
	void            DestroyNode(Magic_Node_s *node);
	Magic_Node_s*	FindNodeByAttr(Magic_Node_s* parent,DWORD attr); //递归找具有某个属性的节点

	//从文件读取和写入
	bool			ReadXmlNode(TiXmlElement* node,Magic_Node_s *p,int deep);
	bool			LoadXmlFile();
	bool            ReadNode(Magic_Node_s *p,int deep = 0);  //递归读取
	bool            LoadFile();
	void			Clear();

#ifdef _DEBUG
	void DelMagicTex(WORD wMagicID);
	void DelMagicTex(Magic_Node_s *pNode);
#endif

private:
	Magic_Show_s *  CreateMagicShow(Magic_Show_s** ppShowList);//显示列表,创建好了放到那个showlist
	void            RemoveMagicShow(Magic_Show_s *ms);

	string          m_strPath;      //路径             
	Magic_Show_s*	m_ShowList;		//显示列表
	int             m_iCreatedMagicId;
	MMagic_Root_s	m_MagicRoots;
	MMagic_Unit_s	m_MagicUnits;
	FILE*           fp;
};