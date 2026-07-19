#pragma once

#include "Global/Global.h"

enum eITEMSHOWTYPE
{
	ITEMSHOWTYPE_NOFOUND   =   0x00000020,
	ITEMSHOWTYPE_DROP   =      0x00000010,
	ITEMSHOWTYPE_BEST   =      0x00000008,
	ITEMSHOWTYPE_EXPENSIVE =   0x00000004,
	ITEMSHOWTYPE_PICK   =      0x00000002,
	ITEMSHOWTYPE_SHOW   =      0x00000001,
	ITEMSHOWTYPE_HIDE   =      0x00000000,
};

struct CItemClass
{
	CItemClass()
	{
		bBest = false;
		bExpensive = false;
		bPick = false;
		bShow = false;
	}
	string strName;
	bool   bBest; //是否极品
	bool   bExpensive; //是否贵重
	bool   bPick; //是否捡取
	bool   bShow; //是否显示
};

class CAIPickCfgMgr
{
public:
	CAIPickCfgMgr(void);
	~CAIPickCfgMgr(void);

	void Clear();
	bool LoadConfig();

	bool  GetItemColor(const char* name,DWORD& color); //返回值表示是否显示
	bool  GetItemColor(DWORD dwType,DWORD& color);   //返回值表示是否显示

	//物品配置文件
	bool IsAutoPick(const char* name); //是否需要经过就捡的物品
	bool IsAutoPick(DWORD dwType); 
	bool IsGoPick(const char* name);   //是否需要飞去捡的物品
	bool IsGoPick(DWORD dwType);
	bool IsBest(const char* name);     //是否极品
	bool IsBest(DWORD dwType);
	bool IsPetPick(const char* name);
	bool IsPetPick(DWORD dwType);      //是否宠物捡的物品
	DWORD  GetShowType(const char* name);    //返回要显示的类型
	CItemClass* GetItemClass(int i) const;   //注意：不能用此指针直接改变名字
	int  GetItemClassCount() const {   return (int)m_vecClass.size(); }
	void FlipBest(int i);
	void FlipExpensive(int i);
	void FlipPick(int i);
	void FlipShow(int i);
	int  InsertItemClass(const char* strName,int pos);
	int  RemoveItemClass(int pos);
private:

	//物品列表
	typedef map<string,CItemClass*> MItemClass;
	typedef vector<CItemClass*>   VItemClass;
	MItemClass m_mapClass;
	VItemClass m_vecClass;
	string     m_strItemFile;

	void WriteConfigFile();
};

extern CAIPickCfgMgr g_PickCfgMgr;