#pragma once

#include "Global/Global.h"

class CRelationData
{
	struct Relation_t
	{
		string      strName;
		int			iRelType;
		int			iJob;
		int			iLevel;
		int			iOnline;
		string      strGuild;
		//好友名次排行趋势
		char		cQushi;
		bool        bHasXuanTieFlag;//是否收到玄铁令牌

		void   AddRelationType(DWORD type)   {  iRelType |= type; }
		void   RemoveRelationType(DWORD type){  iRelType &= ~type; }
	};

	struct Friend_t
	{
		string strName;
		int    iType;
	};
public:
	CRelationData(void);
	~CRelationData(void);
};
