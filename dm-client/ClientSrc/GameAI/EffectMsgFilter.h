#pragma once

/////////////////////////////////////////////////////////////
//专门处理服务器端通过MSG_Item_Effect_Switch消息触发的特效

#include "Global/Global.h"

class CEffectMsgFilter
{
public:
	CEffectMsgFilter(void);
	~CEffectMsgFilter(void);

	bool DoFilter(DWORD id,WORD wItemType,WORD iX,WORD iY,DWORD dwColor);
};

extern CEffectMsgFilter g_EffectMsgFilter;
