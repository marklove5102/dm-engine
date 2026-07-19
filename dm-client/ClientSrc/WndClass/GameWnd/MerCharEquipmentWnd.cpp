#include "mercharequipmentwnd.h"
#include "GameData/GameData.h"
#include "GameData/MCharacter.h"
#include "GameControl/GameControl.h"
#include "GameAI\AIMSysMgr.h"
#include "BaseClass/Control/GoodGrid.h"
#include "GameData/MsgBoxMgr.h"
#include "BaseClass/Audio/Audio.h"
#include "Baseclass/Control/ParserTip.h"
#include "GameAI/AIGoodMgr.h"

INIT_WND_POSX(CMerCharEquipmentWnd,POS_SETPOS_VARIABLE,POS_SETPOS_VARIABLE)

CMerCharEquipmentWnd::CMerCharEquipmentWnd(void)
{
	if(MCHAR.GetLevel() < MAX_LEVEL)
		m_iIndex = 68;
	else
		m_iIndex = 69;

	m_iPages = 1;
	m_uStyle = CTRL_STYLE_BACKMODE_TEX | CTRL_STYLE_SELFDELETE_BACKTEX;
	m_iAlignType = XAL_TOPLEFT;
	m_iOffX = g_Gfx.GetWidth() - 468;
	m_iOffY	= 0;

}

CMerCharEquipmentWnd::~CMerCharEquipmentWnd(void)
{
}

void CMerCharEquipmentWnd::OnCreate()
{
	g_pMerSys->RefreshMerCharState();

	m_iFrameCount = 0;         
}

bool CMerCharEquipmentWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	return  CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CMerCharEquipmentWnd::OnClickCloseButton()
{
	g_pControl->PopupWindow(OPER_CLOSE,"MerCharEquipmentWnd");
}

void CMerCharEquipmentWnd::Draw(void)
{
	CStateViewer::Draw();                       
	g_pMerSys->RefreshMerCharState() ;
	// 显示动态效果
	unsigned char iCareer = MCHAR.GetCareer();

	LPTexture pTex = g_TexMgr.GetTex(PACKAGE_INTERFACE,850+iCareer);

	if(pTex)
		g_Gfx.DrawTextureNL(m_iScreenX-20-100,m_iScreenY,pTex);

	//if( m_iFrameCount < 36 ) 
	//{
	//	pTex = g_TexMgr.GetTex(PACKAGE_INTERFACE,853+iCareer);
	//	if(pTex)
	//	{
	//		pTex->AnimationContral(false,m_iFrameCount/4);
	//		g_Gfx.SetRenderMode(RM_ADD1);
	//		g_Gfx.DrawTextureNL(m_iScreenX-10-100,m_iScreenY,pTex);
	//		g_Gfx.SetRenderMode();
	//	}
	//}
	//m_iFrameCount++;

	//玩家名字
	const char* szPlayerName = MCHAR.GetName();
	int iNameX = strlen(szPlayerName)*FONT_SMALL/4;
	BYTE dwNameColor = MCHAR.GetNameColor();
	g_Font.DrawText( m_iScreenX+292-iNameX,m_iScreenY+36,	szPlayerName,g_pGameData->GetMirColor(dwNameColor),FONT_SMALL);

	//行会名字
	char strTemp[100]={0};
	sprintf(strTemp,"%s %s",MCHAR.GetTitle(),
		MCHAR.GetGuildTitle());

	int iTileNameX = strlen(strTemp)*FONT_SMALL/4;
	g_Font.DrawText(292 + m_iScreenX-iTileNameX, m_iScreenY+55,strTemp,0xFFFFFFFF,FONT_SMALL);


	stGoodFrom GoodFrom = CGoodGrid::GetDropGoodFrom();
	if(GoodFrom.DropGood.GetID() == 0 || GoodFrom.eFromWnd != Mer_Arm_Wnd)
		m_bTianYi = false;
	else if(GoodFrom.DropGood.GetLooks() == 885 || GoodFrom.DropGood.GetLooks() == 886)
		m_bTianYi = true;
	else
		m_bTianYi = false;

	//显示人物属性
	ShowCharInfo();

	//显示人物状态
	ShowCharState();

	//显示装备(背景层)
	//ShowOtherEquip();

	//显示装备(前层)
	//ShowEquippment();

	//显示小锁
	//ShowSmallLock();
}

//----------------------------------------------------------------------------
//鼠标左键弹起
bool CMerCharEquipmentWnd::OnLeftButtonUp(int iX, int iY)
{
	if(m_bDClicked)
	{
		m_bDClicked = false;
		return CCtrlWindowX::OnLeftButtonUp(iX,iY);
	}

	//int iPos = InGrid(iX,iY);
	//if(iPos == -1)
	//{
	//	if(IsInOther(iX,iY))
	//	{
	//		g_pControl->Msg(MSG_CTRL_CHARWND,2,NULL);
	//		g_pControl->Msg(MSG_CTRL_MER_CHAREQUIP_WND,0,NULL);
	//		return true;
	//	}

	//	return CCtrlWindowX::OnLeftButtonUp(iX,iY);
	//}

	stGoodFrom GoodFrom = CGoodGrid::GetDropGoodFrom();
	/////////////////////////////////////////////////////////////
	////徒弟借元神 限制拖放装备物品
	if(1 == SELF.GetYuanshenOwner())
	{
		std::string strName;
		strName = GoodFrom.DropGood.GetName();
		if(strName.size()>0 && GoodFrom.DropGood.GetStdMode() != 25 && GoodFrom.DropGood.GetStdMode() != 34 )
		{
			return true;
		}
	}
	////徒弟借元神end
	//如果鼠标点击到衣服上 而且鼠标上没有衣服 而且玩家穿这新年衣  那么发送协议 让玩家脱掉新年衣
	//if(iPos == 0 && GoodFrom.DropGood.GetID() == 0 &&   MCHAR.GetIBodyEx()== 16)
	//{
	//	//从包裹中找到新年衣的道具的位置 发送使用道具协议
	//	unsigned int dwTempGoodID = MCHAR.GetLastUsedCloseID();
	//	for(int iTempPos = 0; iTempPos < MAX_MER_PACKAGE; iTempPos++)
	//	{
	//		CGood& tmp = MCHAR.PackageGood().Get(iTempPos);
	//		if(tmp.GetID() == dwTempGoodID && (tmp.GetLooks() == 885 || tmp.GetLooks() == 886))
	//		{
	//			GoodFrom.eFromWnd = Mer_Arm_Wnd;
	//			GoodFrom.DropGood = tmp;
	//			CGoodGrid::SetDropGoodFrom(GoodFrom);

	//			break;
	//		}
	//	}
	//	return true;
	//}

	////如果鼠标点击到衣服上 而且鼠标上是包裹栏里拖过来的新年衣 而且玩家没有穿新年衣 那么发送协议 让玩家穿新年衣
	////然后把新年衣放回包裹
	//if(iPos == 0 && GoodFrom.eFromWnd == Mer_Package_Wnd && ( GoodFrom.DropGood.GetLooks() == 885 || GoodFrom.DropGood.GetLooks() == 886))
	//{
	//	//找个空位把新年衣道具放回包裹栏  发送使用道具协议
	//	if(MCHAR.GetIBodyEx() != 16)
	//		MCHAR.SetLastUsedCloseID( 0 );

	//	for(int iTempPos = 0; iTempPos < MAX_MER_PACKAGE; iTempPos++)
	//	{
	//		if(MCHAR.PackageGood().Get(iTempPos).GetID() == 0)
	//		{
	//			MCHAR.PackageGood().Get(iTempPos) = GoodFrom.DropGood;

	//			if(MCHAR.GetLastUsedCloseID() == 0)
	//			{
	//				g_pGameControl->SEND_YongBing_Use_Object(iTempPos);
	//				MCHAR.SetLastUsedCloseID(GoodFrom.DropGood.GetID());
	//			}
	//			break;
	//		}
	//	}
	//	GoodFrom.DropGood.SetID(0);
	//	CGoodGrid::SetDropGoodFrom(GoodFrom);
	//	return true;
	//}

	//还放下去的
	//if(GoodFrom.DropGood.GetID() != 0 && (GoodFrom.DropGood.GetLooks() == 885 || GoodFrom.DropGood.GetLooks() == 886) && GoodFrom.eFromWnd == Mer_Arm_Wnd)
	//{
	//	if(iPos == 0)
	//	{
	//		GoodFrom.DropGood.SetID(0);
	//		CGoodGrid::SetDropGoodFrom(GoodFrom);
	//	}
	//	return true;
	//}

	//if(GoodFrom.DropGood.GetID() != 0)		//有拖动物品
	//{
	//	if(GoodFrom.eFromWnd == Mer_Package_Wnd)		//包裹中拖过来的物品
	//	{
	//		//使用乾坤锁
	//		if(GoodFrom.DropGood.GetID() > 0 && GoodFrom.DropGood.GetStdMode() == 37 && 
	//			GoodFrom.DropGood.GetShape() == 203 )
	//		{
	//			unsigned long iid = MCHAR.EquipGood().Get(iPos).GetID();
	//			if(iid==0)
	//				return true;
	//			int i = 0;
	//			for(i = 0;i < MAX_PACKAGE_ELEMENT - 6;i++)
	//			{
	//				if(MCHAR.PackageGood().Get(i).GetID() == 0)
	//					break;
	//			}
	//			if(i < MAX_PACKAGE_ELEMENT - 6)
	//			{
	//				MCHAR.PackageGood().Get(i) = GoodFrom.DropGood;

	//				int LockHour = GoodFrom.DropGood.GetAC2()*256 + GoodFrom.DropGood.GetAC() ;
	//				int LeftHour = MCHAR.EquipGood().Get(iPos).GetFlag2() ;
	//				if(g_pMerSys)
	//					g_pMerSys->SetPackGoodID(iid);

	//				GoodFrom.DropGood.SetID(0);
	//				CGoodGrid::SetDropGoodFrom(GoodFrom);

	//				if(!g_AIGoodMgr.IsCanLock(MCHAR.EquipGood().Get(iPos)))
	//				{
	//					g_MsgBoxMgr.PopSimpleAlert("该物品不能被锁定。");
	//					return true;
	//				}

	//				NPC.SetPackGoodID(iid);
	//				m_iPosPkg = i;

	//				char str[200]={0};
	//				if(MCHAR.EquipGood().Get(iPos).GetFlag2() == 0)
	//				{
	//					sprintf(str,"锁定后此物品将在在线累计%d小时内无法交易，无法丢弃，无法摆摊。\n你确定要锁定此物品吗？",LockHour);
	//					g_MsgBoxMgr.PopSimpleComfirm(str,MSG_CTRL_USE_QIANKUNSUO_MEREQ,0);
	//				}
	//				else
	//				{
	//					if(LockHour > LeftHour)
	//					{
	//						sprintf(str,"该物品已经被锁定，锁定有效时间剩余：%d小时。如果再次\n锁定，则此次锁定效果将替代前面的锁定效果。此次锁定的\n有效时间为：%d小时，你确定要再次锁定次物品吗？",LeftHour,LockHour);
	//						g_MsgBoxMgr.PopSimpleComfirm(str,MSG_CTRL_USE_QIANKUNSUO_MEREQ,0);
	//					}
	//					else
	//					{
	//						sprintf(str,"锁定失败。\n该物品的有效锁定时间剩余%d小时，大于或等于这次使用的乾坤锁\n有效锁定时间（%d小时），因此这次锁定失败。",LeftHour,LockHour);
	//						g_MsgBoxMgr.PopSimpleAlert(str);
	//					}
	//				}

	//				return true;

	//			}
	//		}
	//		//使用镇魔符(捆)附加魔力到元神装备上 
	//		if(GoodFrom.DropGood.GetID() > 0 && GoodFrom.DropGood.GetStdMode() == 3 && 
	//			GoodFrom.DropGood.GetShape() == 27 )
	//		{
	//			unsigned long iid = MCHAR.EquipGood().Get(iPos).GetID();
	//			if(iid==0)
	//				return true;
	//			if(!MCHAR.EquipGood().Get(iPos).IsHeartMonAttached())
	//				return true;
	//			int i = 0;
	//			for(i = 0;i < MAX_PACKAGE_ELEMENT - 6;i++)
	//			{
	//				if(MCHAR.PackageGood().Get(i).GetID() == 0)
	//					break;
	//			}
	//			if(i < MAX_PACKAGE_ELEMENT - 6)
	//			{
	//				if(g_pMerSys)
	//					g_pMerSys->AddDemonPowerReq(GoodFrom.DropGood.GetID(),iid);

	//				MCHAR.PackageGood().Get(i) = GoodFrom.DropGood;
	//				GoodFrom.DropGood.SetID(0);
	//				CGoodGrid::SetDropGoodFrom(GoodFrom);

	//				if(iPos>2)//如果是在格子里面
	//				{
	//					//设置参数绘制物品附魔的包裹特效
	//					//NPC.SetPackGoodEffWnd(Mer_Arm_Wnd);
	//					//NPC.SetPackGoodEffPos(iPos);
	//					//NPC.SetPackGoodEffType(5);
	//					//NPC.SetAuthFrame(0);
	//					//NPC.SetPackGoodEffDraw(true);
	//				}

	//				return true;
	//			}
	//		}
	//		//使用玉净瓶附加属性到元神装备上 
	//		if(GoodFrom.DropGood.GetID() > 0 && GoodFrom.DropGood.GetStdMode() == 3 && 
	//			GoodFrom.DropGood.GetShape() == 24 )
	//		{
	//			unsigned long iid = MCHAR.EquipGood().Get(iPos).GetID();
	//			if(iid==0)
	//				return true;
	//			int i = 0;
	//			for(i = 0;i < MAX_PACKAGE_ELEMENT - 6;i++)
	//			{
	//				if(MCHAR.PackageGood().Get(i).GetID() == 0)
	//					break;
	//			}
	//			if(i < MAX_PACKAGE_ELEMENT - 6)
	//			{
	//				if(g_pMerSys)
	//					g_pMerSys->AddDemonGoodReq(iid,GoodFrom.DropGood.GetID(),iPos+1,1);

	//				MCHAR.PackageGood().Get(i) = GoodFrom.DropGood;
	//				GoodFrom.DropGood.SetID(0);
	//				CGoodGrid::SetDropGoodFrom(GoodFrom);

	//				if(iPos>2)//如果是在格子里面
	//				{
	//					//设置参数绘制物品附魔的包裹特效
	//					//NPC.SetPackGoodEffWnd(Mer_Arm_Wnd);
	//					//NPC.SetPackGoodEffPos(iPos);
	//					//NPC.SetPackGoodEffType(5);
	//					//NPC.SetAuthFrame(0);
	//					//NPC.SetPackGoodEffDraw(true);
	//				}

	//				return true;
	//			}
	//		}
	//		//if(IsArmEquipment(GoodFrom.DropGood.GetStdMode(),iPos))	//是否属于装备品
	//		//{
	//		//	//装备物品音乐// 不管成功失败都发声音
	//		//	int i = GoodFrom.DropGood.GetStdMode()+1;
	//		//	if(GoodFrom.DropGood.GetID() != 0)
	//		//	{
	//		//		g_pAudio->Play(EAT_GOODS,i,g_nRand++);
	//		//	}

	//		//	g_pGameControl->SEND_MerSys_Arm_Object(GoodFrom.DropGood,iPos,GoodFrom.iWndPos);
	//		//	GoodFrom.DropGood.SetID(0);
	//		//	CGoodGrid::SetDropGoodFrom(GoodFrom);
	//		//}
	//	}
	//	else if(GoodFrom.eFromWnd == Mer_Arm_Wnd)		//装备窗口本身拖出来的物品					
	//	{	
	//		int j = GoodFrom.DropGood.GetStdMode()+1;
	//		if(GoodFrom.DropGood.GetID() != 0)
	//		{
	//			g_pAudio->Play(EAT_GOODS,j,g_nRand++);
	//		}

	//		//放回装备品
	//		GetCharacterEquipment(GoodFrom.iWndPos) = GoodFrom.DropGood;
	//		GoodFrom.DropGood.SetID(0);
	//		GoodFrom.eFromWnd = NO_Wnd;
	//		CGoodGrid::SetDropGoodFrom(GoodFrom);
	//	}
	//}
	//else		//没有拖动物品
	//{
	//	if(g_pInput->IsShift())
	//	{
	//		//((CMainWnd *)g_pControl)->InsertObjectLink(GetCharacterEquipment(iPos));
	//	}
	//	else
	//	{
	//		/////////////////////////////////////////////////////////////
	//		////徒弟借元神 限制拿装备物品
	//		if(1 == SELF.GetYuanshenOwner())
	//		{
	//			std::string strName;
	//			strName = GetCharacterEquipment(iPos).GetName();
	//			if(strName.size()>0 &&  GetCharacterEquipment(iPos).GetStdMode() != 25 &&  GetCharacterEquipment(iPos).GetStdMode() != 34 )
	//			{
	//				return true;
	//			}
	//		}
	//		////徒弟借元神end
	//		GoodFrom.DropGood = GetCharacterEquipment(iPos);
	//		GoodFrom.eFromWnd = Mer_Arm_Wnd;
	//		GoodFrom.iWndPos = iPos;
	//		CGoodGrid::SetDropGoodFrom(GoodFrom);
	//		GetCharacterEquipment(iPos).SetID(0);
	//	}
	//}
	return CCtrlWindowX::OnLeftButtonUp(iX,iY);
}

//----------------------------------------------------------------------------
//鼠标右键按下
bool CMerCharEquipmentWnd::OnRightButtonDown(int iX, int iY)
{
	stGoodFrom temp = CGoodGrid::GetDropGoodFrom();
	if( temp.DropGood.GetID() !=0 && temp.eFromWnd == Arm_Wnd )
	{
		return CCtrlWindowX::OnRightButtonDown(iX,iY);
	}

	if(temp.DropGood.GetID() !=0 && temp.eFromWnd == Mer_Arm_Wnd)
	{
		int j = temp.DropGood.GetStdMode()+1;
		g_pAudio->Play(EAT_OTHER,j,g_nRand++);

		GetCharacterEquipment(temp.iWndPos) = temp.DropGood;

		temp.DropGood.SetID(0);
		temp.eFromWnd = NO_Wnd;
		CGoodGrid::SetDropGoodFrom(temp);

		return true;
	}

	//if(InGrid(iX,iY) != -1)
	//	return true;
	return CCtrlWindowX::OnRightButtonDown(iX,iY);
}

//-------------------------------------------------------------------------
//鼠标右键弹起
bool CMerCharEquipmentWnd::OnRightButtonUp(int iX,int iY)
{
	int iPos = -1;//InGrid(iX,iY);

	if(iPos!= -1)
	{
		stGoodFrom temp = CGoodGrid::GetDropGoodFrom();

		if(temp.DropGood.GetID() == 0 )
		{
			temp.DropGood = GetCharacterEquipment(iPos);
			temp.eFromWnd = Mer_Arm_Wnd;
			temp.iWndPos = iPos;
			CGoodGrid::SetDropGoodFrom(temp);

			GetCharacterEquipment(iPos).SetID(0);
		}
	}
	return CCtrlWindowX::OnRightButtonUp(iX,iY);
}

//鼠标移动
bool CMerCharEquipmentWnd::OnMouseMove(int iX, int iY)
{
	CGood tempGood;
	CParserTip *pTip = g_pControl->GetTipWnd();

	int iPos = -1;//InGrid(iX,iY);
	if(iPos != -1 )
	{
		tempGood = GetCharacterEquipment(iPos);
		if(tempGood.GetID() > 0)
		{
			pTip->Parse(tempGood,true);

			int x = m_iScreenX + iX + 10;
			int y = m_iScreenY + iY + 10;
			if(x + pTip->GetWidth() > g_Gfx.GetWidth())
			{
				x = m_iScreenX + iX - pTip->GetWidth() - 2;
				if( x < 0)
					x = 0;
			}
			if(y + pTip->GetHeight() > g_Gfx.GetHeight())
			{
				y = m_iScreenY + iY - pTip->GetHeight();
				if( y < 0)
					y = 0;
			}
			pTip->Move(x - m_iScreenX,y - m_iScreenY);
			pTip->SetShow(true);
		}
		else
		{
			pTip->SetShow(false);
		}

		m_iPos = -1;
		if(iPos == 2 || iPos == 3 || iPos == 5 || iPos == 6 || iPos == 7 || iPos == 8 ||
			iPos == 9 || iPos == 10 || iPos == 11 || iPos == 12)
		{
			stGoodFrom temp = CGoodGrid::GetDropGoodFrom();
			if(temp.DropGood.GetID() != 0)
			{
				m_iPos = iPos;
				//if(IsArmEquipment(temp.DropGood.GetStdMode(),iPos) && 
				//	((MCHAR.GetLevel() >= temp.DropGood.GetNeedLevel() &&
				//	temp.DropGood.GetNeed() == 0) || temp.DropGood.GetNeed() > 0))
				//	m_dwAlarmColor = 0x8000FF00;                    
				//else
				//	m_dwAlarmColor = 0x80FF0000; 

				if( 1 == SELF.GetYuanshenOwner())
				{
					if(temp.DropGood.GetStdMode() != 25 && temp.DropGood.GetStdMode() != 34)
						m_dwAlarmColor = 0x800000FF; 
				}
			}
		}
		return true;
	}
	else
	{
		tempGood.SetID(0);
		pTip->SetShow(false);
		m_iPos = -1;
	}

	return CCtrlWindowX::OnMouseMove(iX,iY);
}

void CMerCharEquipmentWnd::ShowCharInfo()
{
	char temp[256] = {0};

	int iCharHeight = 26;
	int iPKStateY = 377;
	int iCharX = 91;
	int iCharY = 60;
	if(g_dwServerVersion >= 1920)
	{
		iCharHeight = 24;
		iCharY = 40;
	}

	//经验值的计算
	DWORD dwExp = MCHAR.GetExp();
	DWORD dwLevelUpExp = MCHAR.GetLevelUpExp() ;

	//经验值
	if(dwLevelUpExp != 0)
	{ 
		if(dwExp > dwLevelUpExp)
			sprintf(temp,"100.00%%");  
		else
			sprintf(temp,"%2.2f%%",(double)dwExp /dwLevelUpExp * 100);
	}
	else
		sprintf(temp,"0.00%");

	if(dwExp >= dwLevelUpExp)
		g_Font.DrawText(m_iScreenX + iCharX,m_iScreenY + iCharY,temp,0xFFFF0000,FONT_SMALL);
	else
		g_Font.DrawText(m_iScreenX + iCharX,m_iScreenY + iCharY,temp,0xFFFFFFFF,FONT_SMALL);

	iCharY += iCharHeight;

	//背包重量
	DWORD dwPackageWeight = MCHAR.GetPackageWeight();
	DWORD dwPackageWeightMax = MCHAR.GetPackageWeightMax();
	sprintf(temp,"%d/%d",dwPackageWeight,dwPackageWeightMax);

	if(dwPackageWeight >= dwPackageWeightMax)
		g_Font.DrawText(m_iScreenX + iCharX, m_iScreenY + iCharY,temp,0xFFFF0000,FONT_SMALL);
	else
		g_Font.DrawText(m_iScreenX + iCharX, m_iScreenY + iCharY,temp,0xFFFFFFFF,FONT_SMALL);
	iCharY += iCharHeight;

	//负重量
	DWORD dwWeight = MCHAR.GetWeight();
	DWORD dwWeightMax = MCHAR.GetWeightMax();

	sprintf(temp,"%d/%d",dwWeight,dwWeightMax);
	if(dwWeight >= dwWeightMax)
		g_Font.DrawText(m_iScreenX + iCharX, m_iScreenY + iCharY,temp,0xFFFF0000,FONT_SMALL);
	else
		g_Font.DrawText(m_iScreenX + iCharX, m_iScreenY + iCharY,temp,0xFFFFFFFF,FONT_SMALL);
	iCharY += iCharHeight;

	// 命中
	sprintf(temp,"+%d",MCHAR.GetPrecision());
	g_Font.DrawText(m_iScreenX + iCharX, m_iScreenY + iCharY,temp,0xFFFFFFFF,FONT_SMALL);
	iCharY += iCharHeight;

	//躲避
	sprintf(temp,"+%d",MCHAR.GetSmartness()); 
	g_Font.DrawText(m_iScreenX + iCharX, m_iScreenY + iCharY,temp,0xFFFFFFFF,FONT_SMALL);
	iCharY += iCharHeight;

	//魔法躲避
	sprintf(temp,"+%d%% ",MCHAR.GetMagicDef()*10); 
	g_Font.DrawText(m_iScreenX + iCharX, m_iScreenY + iCharY,temp,0xFFFFFFFF,FONT_SMALL);
	iCharY += iCharHeight;

	//中毒躲避
	sprintf(temp,"+%d%%",MCHAR.GetPoisonDef() * 10);
	g_Font.DrawText(m_iScreenX + iCharX, m_iScreenY + iCharY,temp,0xFFFFFFFF,FONT_SMALL);
	iCharY += iCharHeight;

	//移动
	int iHp = MCHAR.GetHP();
	if( dwPackageWeight > dwPackageWeightMax || iHp < 10 )
		sprintf(temp,"%d",0);
	else 
		sprintf(temp,"%d",(iHp - 10 )/16+1);
	g_Font.DrawText(m_iScreenX + iCharX, m_iScreenY + iCharY,temp,0xFFFFFFFF,FONT_SMALL);
	iCharY += iCharHeight;

	//体力恢复
	sprintf(temp,"+%d",MCHAR.GetRestoreLife());
	g_Font.DrawText(m_iScreenX + iCharX, m_iScreenY + iCharY,temp,0xFFFFFFFF,FONT_SMALL);
	iCharY += iCharHeight;

	//魔法恢复
	sprintf(temp,"+%d",MCHAR.GetRestoreMana());
	g_Font.DrawText(m_iScreenX + iCharX, m_iScreenY + iCharY,temp,0xFFFFFFFF,FONT_SMALL);
	iCharY += iCharHeight;

	//腕力
	int iWristMax = MCHAR.GetWristMax();
	int iWrist = MCHAR.GetWrist();

	sprintf(temp,"%d/%d",iWrist,iWristMax);

	if(iWrist >= iWristMax)
		g_Font.DrawText(m_iScreenX + iCharX, m_iScreenY + iCharY,temp,0xFFFF0000,FONT_SMALL);
	else
		g_Font.DrawText(m_iScreenX + iCharX, m_iScreenY + iCharY,temp,0xFFFFFFFF,FONT_SMALL);
	iCharY += iCharHeight;

	//灵力值
	sprintf(temp,"%u",MCHAR.GetLingLi());
	g_Font.DrawText(m_iScreenX + iCharX, m_iScreenY + iCharY,temp,0xFFFFFFFF,FONT_SMALL);


	if(g_dwServerVersion >= 1920)
	{
		//光明属性
		iCharY += iCharHeight;
		sprintf(temp,"%u",MCHAR.GetLightPower());
		g_Font.DrawText(m_iScreenX + iCharX, m_iScreenY + iCharY,temp,0xFFFFFFFF,FONT_SMALL);

		//黑暗属性
		iCharY += iCharHeight;
		sprintf(temp,"%u",MCHAR.GetDarkPower());
		g_Font.DrawText(m_iScreenX + iCharX, m_iScreenY + iCharY,temp,0xFFFFFFFF,FONT_SMALL);
	}

	//会旗
	int iLogoPosX = 52;
	int iLogoPosY = 408;

	//显示元神的职业和性别
	int iSex = MCHAR.IsMale() ? 0 : 1;
	int iCareer = MCHAR.GetCareer();
	LPTexture pFace = g_TexMgr.GetTex(PACKAGE_INTERFACE,91 + 3*iSex + iCareer);
	if(pFace)
	{
		g_Gfx.DrawTextureNL(m_iScreenX + iLogoPosX - 10 ,m_iScreenY + iLogoPosY+5,pFace);
	}
}

void CMerCharEquipmentWnd::ShowCharState()
{
	char temp[50] = {0};
	int iTextPosY = 412;
	int iLeftTextPosX = 241;
	int iRightTextPosX = 375;

	DWORD dwColor_DC = 0;//MCHAR.GetColor_DC();
	DWORD dwColor_SC = 0;//MCHAR.GetColor_SC();
	DWORD dwColor_MC = 0;//MCHAR.GetColor_MC();
	DWORD dwColor_AC = 0;//MCHAR.GetColor_AC();
	DWORD dwColor_MAC = 0;//MCHAR.GetColor_MAC();

	//生命值
	sprintf(temp,"%4d/%d",MCHAR.GetHP(),MCHAR.GetHPMax());
	g_Font.DrawText(m_iScreenX + iLeftTextPosX,m_iScreenY + iTextPosY ,temp,0xffffffff,FONT_SMALL);

	//魔法值
	sprintf(temp,"%4d/%d",MCHAR.GetMP(),MCHAR.GetMPMax());
	g_Font.DrawText(m_iScreenX + iRightTextPosX,m_iScreenY + iTextPosY,temp,0xffffffff,FONT_SMALL);
	iTextPosY += 20;

	//攻击力
	sprintf(temp,"%4d-%d",MCHAR.GetDC(),MCHAR.GetDC2());
	g_Font.DrawText(m_iScreenX + iLeftTextPosX,m_iScreenY + iTextPosY,temp,dwColor_DC,FONT_SMALL);

	//防御力
	sprintf(temp,"%4d-%d",MCHAR.GetAC(),MCHAR.GetAC2());
	g_Font.DrawText(m_iScreenX + iRightTextPosX,m_iScreenY + iTextPosY,temp,dwColor_AC,FONT_SMALL);
	iTextPosY += 19;

	//道术攻击
	sprintf(temp,"%4d-%d",MCHAR.GetSC(),MCHAR.GetSC2());
	g_Font.DrawText(m_iScreenX + iLeftTextPosX,m_iScreenY + iTextPosY,temp,dwColor_SC,FONT_SMALL);

	//魔防力
	sprintf(temp,"%4d-%d",MCHAR.GetMAC(),MCHAR.GetMAC2());
	g_Font.DrawText(m_iScreenX + iRightTextPosX,m_iScreenY + iTextPosY,temp,dwColor_MAC,FONT_SMALL);
	iTextPosY += 20;

	//魔法攻击
	sprintf(temp,"%4d-%d",MCHAR.GetMC(),MCHAR.GetMC2());
	g_Font.DrawText(m_iScreenX + iLeftTextPosX,m_iScreenY + iTextPosY,temp,dwColor_MC,FONT_SMALL);

	//声望
}

////////////////////////////////////////////////////////////
//这里作了派生类接口
//  在前层绘制元神装备 纸娃娃

CGood& CMerCharEquipmentWnd::GetCharacterEquipment(int i)
{
	return MCHAR.EquipGood().Get(i);
}

unsigned long CMerCharEquipmentWnd::GetCharacterLooks()
{
	return MCHAR.GetLooks();
}

bool CMerCharEquipmentWnd::IsCharacterGirl()
{
	return !MCHAR.IsMale();
}

int CMerCharEquipmentWnd::GetCharacterMaskLevel()
{
	return MCHAR.GetMaskLevel();
}
int CMerCharEquipmentWnd::GetCharacterIbodyEx()
{
	return MCHAR.GetIBodyEx();
}

//-----------------------------------------------------------------
////   在后层背景绘制玩家的装备 纸娃娃
DWORD CMerCharEquipmentWnd::GetOtherCharLooks()
{
	return SELF.GetLooks().Char.wShape;
}

bool CMerCharEquipmentWnd::IsOtherCharGirl()
{
	return !SELF.IsMale();
}

CGood& CMerCharEquipmentWnd::GetOtherEquipment(int i)
{
	return SELF.EquipGood().Get(i);
}

int CMerCharEquipmentWnd::GetOtherCharMaskLevel()
{
	return SELF.GetMaskLevel();
}

int  CMerCharEquipmentWnd::GetOtherCharIbodyEx()
{
	return SELF.GetIBodyEx();
}

void  CMerCharEquipmentWnd::UseQianKunSuo()
{
	g_pGameControl->SEND_MerSys_Use_Object(m_iPosPkg);
}

