#include "stdafx.h"
#include "humanplayer.h"
#include "guildex.h"

VOID CHumanPlayer::ReviewAroundNameColor()
{
	CGuildEx* pGuild = GetGuild();
	if (pGuild == nullptr /*|| this->InCityArea()*/ /*|| this->InWarArea() */)return;

	//BYTE	btEnemy = 0x45;
	//BYTE	btAlly = 0xfc;
	//if( InCityArea() )
	//	btEnemy = 0xff, btAlly = 0xff;   
	SendMsg(GetId(), 0x2a, GetNameColor(this), 0, 0, (LPVOID)GetViewName());
	xListHelper<VISIBLE_OBJECT> helper(&this->m_xVisibleObjectList);
	for (VISIBLE_OBJECT* pVo = helper.first(); pVo != nullptr; pVo = helper.next())
	{
		if (pVo &&
		pVo->pObject &&
		pVo->pObject->GetType() == OBJ_PLAYER)
		{
			CHumanPlayer* p = (CHumanPlayer*)pVo->pObject;
			if (p->GetGuild())
			{
				//if( pGuild->IsKillGuild( p->GetGuild() ) )
				SendMsg(p->GetId(), 0x290, p->GetNameColor(this), 0, 0);
				//else if( pGuild->IsAllyGuild( p->GetGuild() ) )
				//	SendMsg( p->GetId(), 0x290, p->GetNameColor( this ), 0, 0 );
			}
		}
	}
}
