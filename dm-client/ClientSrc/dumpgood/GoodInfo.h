#pragma once

#include "cgood.h"
#include <vector>
#include <map>

#define MAX_CATCH_SIZE 20 
#define TIME_ITEM_STDMODE  38//存在时间(创建时间、有效期)的物品的stdmode
#define COMPOUND_GEM_STDMODE  61//合成宝石stdmode
#define SETEXT_ITEM_STDMODE 9//附加属性记录特殊信息的stdmode
#define __NEW_PACKAGE
#define __MERSYSTEM__
#define g_server_version  999999


class CGoodInfo
{
public:
	CGoodInfo(void);
	~CGoodInfo(void);

	bool DumpGood(const char* strMsg , char * strResult , int &len) ;
protected:
	std::string  m_strTipText ;
	int g_server_time ;

	void AddText(std::string addtext,DWORD color = 0xFFFFFFFF ) ;
	void GetGoodInfo(CGood& FocusGood) ;

	void ParseAttackSkillInfo(CGood& FocusGood);

	int	  IsHeartMonAttached(CGood& good);          //是否附了魔0,1,2
	bool  IsCanLockStdMode(CGood& temp) ;

	bool DumpInfoFromCGood(CGood& FocusGood , char * strResult , int &len);

	CGood FocusGood; //当前正在解析的物品
	std::map<DWORD,std::string> m_mapSkill;

	char* GetDateTime(const char* fmt,DWORD dwTime);

};

extern CGoodInfo  g_GoodInfo;

