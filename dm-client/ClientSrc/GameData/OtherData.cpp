#include "OtherData.h"
#include "GameData.h"
#include "MagicCtrlMgr.h"
#include "Global/Interface/TexManagerInterface.h"
#include "GameControl/GameControl.h"

COtherData g_OtherData;

#define MAKERGB(r,g,b)  ((DWORD)(((BYTE)(b)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(r))<<16)|0xFF000000) )

COtherData::COtherData(void)
{
    m_vecSummonPoint.push_back(MAKELONG(18,48));
    m_vecSummonPoint.push_back(MAKELONG(22,44));
    m_vecSummonPoint.push_back(MAKELONG(26,40));
    m_vecSummonPoint.push_back(MAKELONG(22,36));
   
    m_vecSummonPoint.push_back(MAKELONG(23,48));
    m_vecSummonPoint.push_back(MAKELONG(27,44));
    m_vecSummonPoint.push_back(MAKELONG(31,40));
    m_vecSummonPoint.push_back(MAKELONG(27,36));
    m_vecSummonPoint.push_back(MAKELONG(23,32));

    m_vecSummonPoint.push_back(MAKELONG(19,28));
    m_vecSummonPoint.push_back(MAKELONG(31,35));
    m_vecSummonPoint.push_back(MAKELONG(27,31));
   

    m_vecSummonPoint.push_back(MAKELONG(27,23));
    m_vecSummonPoint.push_back(MAKELONG(31,19));
    m_vecSummonPoint.push_back(MAKELONG(35,23));
    m_vecSummonPoint.push_back(MAKELONG(39,27));
    m_vecSummonPoint.push_back(MAKELONG(35,31));
    
	m_VRtsSummonPoint.push_back(SPoint(130,41));
	m_VRtsSummonPoint.push_back(SPoint(130,60));
	m_VRtsSummonPoint.push_back(SPoint(143,69));
	m_VRtsSummonPoint.push_back(SPoint(69,110));
	m_VRtsSummonPoint.push_back(SPoint(76,124));
	m_VRtsSummonPoint.push_back(SPoint(70,140));

	// 头发染色
	m_dwHairColor[ 0] = 0xFFFFFFFF;
	m_dwHairColor[ 1] = 0xFFBBBBBB;
	m_dwHairColor[ 2] = 0xFF555555;
	m_dwHairColor[ 3] = 0xFF806F62;
	m_dwHairColor[ 4] = 0xFFB3918E;
	m_dwHairColor[ 5] = 0xFF60548C;
	m_dwHairColor[ 6] = 0xFF6F8E74;
	m_dwHairColor[ 7] = 0xFFC7B95C;
	m_dwHairColor[ 8] = 0xFF7976A6;
	m_dwHairColor[ 9] = 0xFFC05653;
	m_dwHairColor[10] = 0xFF6F83C6;
	m_dwHairColor[11] = 0xFF99E76D;
	m_dwHairColor[12] = 0xFFEEE150;
	m_dwHairColor[13] = 0xFF9A66A2;
	m_dwHairColor[14] = 0xFF849940;
	m_dwHairColor[15] = 0xFFBBB55E;

	// 男染色
	m_dwBodyColor[0][ 0] = MAKERGB(61,59,106);
	m_dwBodyColor[0][ 1] = MAKERGB(39,160,102);
	m_dwBodyColor[0][ 2] = MAKERGB(200,24,35);
	m_dwBodyColor[0][ 3] = MAKERGB(24,95,198);
	m_dwBodyColor[0][ 4] = MAKERGB(180,180,180);
	m_dwBodyColor[0][ 5] = MAKERGB(156,68,168);
	m_dwBodyColor[0][ 6] = MAKERGB(123,46,0);
	m_dwBodyColor[0][ 7] = MAKERGB(123,0,70);
	m_dwBodyColor[0][ 8] = MAKERGB(0,74,128);
	m_dwBodyColor[0][ 9] = MAKERGB(104,202,198);
	m_dwBodyColor[0][10] = MAKERGB(242,101,34);
	m_dwBodyColor[0][11] = MAKERGB(30,30,30);
	m_dwBodyColor[0][12] = MAKERGB(0,174,239);
	m_dwBodyColor[0][13] = MAKERGB(0,89,82);
	m_dwBodyColor[0][14] = MAKERGB(253,253,253);
	m_dwBodyColor[0][15] = MAKERGB(50,0,75);

	// 女染色
	m_dwBodyColor[1][ 0] = MAKERGB(0,89,82);
	m_dwBodyColor[1][ 1] = MAKERGB(39,255,102);
	m_dwBodyColor[1][ 2] = MAKERGB(255,24,35);
	m_dwBodyColor[1][ 3] = MAKERGB(24,95,198);
	m_dwBodyColor[1][ 4] = MAKERGB(255,242,0);
	m_dwBodyColor[1][ 5] = MAKERGB(129,0,146);
	m_dwBodyColor[1][ 6] = MAKERGB(123,46,0);
	m_dwBodyColor[1][ 7] = MAKERGB(123,0,70);
	m_dwBodyColor[1][ 8] = MAKERGB(180,180,180);
	m_dwBodyColor[1][ 9] = MAKERGB(28,187,180);
	m_dwBodyColor[1][10] = MAKERGB(242,101,34);
	m_dwBodyColor[1][11] = MAKERGB(236,0,140);
	m_dwBodyColor[1][12] = MAKERGB(30,30,30);
	m_dwBodyColor[1][13] = MAKERGB(0,174,239);
	m_dwBodyColor[1][14] = MAKERGB(255,0,255);
	m_dwBodyColor[1][15] = MAKERGB(253,253,253);

	m_dwLastMovePosTime = 0;
	m_bAutoEnter = false;
	m_VDelCharList.clear();
	m_VCharList.clear();
	m_iCharNo    = 0;
	m_bReceivedCharList = false;
	m_dwExpUpTime = 0;
	m_dwMapInTime = 0;
	m_OtherPlayer.Clear();
	m_bDay = true;
	m_sOffLinePrivate.strCharName = "";
	m_bAutoLeaveWord = false;
	m_dwCenserStartTime = 0;
	m_dwCenserDurTime = 0;
	m_bShowSecondKill = false;
	//memset(&m_sWuXingFlag,0,sizeof(WUXINGFLAG));

	for(int i = 0;i<8;i++)
	{
		m_dwUpdateMedalExp[i] = 0;
	}
	m_strDeleteMember.clear();
	m_dwTelTime = 0;
	m_dwSetMsgStartTime = 0;
	m_TexShow = NULL;
	m_bRadioOpen = false;
	for(int i = 0;i < MAPARR_WIDTH;i++)
	{
		for(int j = 0;j < MAPARR_WIDTH;j++)
		{
			m_FireMagic[i][j] = NULL;
		}
	}

	m_dwReadUsbEkeyDelayStartTime = 0;
	m_iReLivePrice = 3;

	m_MArrorTips[EP_FirstInGmae] = "在空地上点击鼠标左键可往该方向上行走";
	m_MArrorTips[EP_FirstInGmae_Step2] = "连续点击或按住鼠标左键可往该方向上连续行走";
	m_MArrorTips[EP_FirstInGmae_Step3] = "在空地上点击鼠标右键可往该方向上奔跑";
	m_MArrorTips[EP_FirstInGmae_Step4] = "连续点击或按住鼠标右键可往该方向上连续奔跑";
	m_MArrorTips[EP_First_TalkWithNPC] = "点击此处完成任务";
	m_MArrorTips[EP_First_AutoFindPath] = "点击此处自动寻路";
	m_MArrorTips[EP_First_StudySkill_PaoPao] = "点击技能按钮查看新学会的技能";
	m_MArrorTips[EP_First_SetShortcutKey_Base] = "该技能无需手动释放，攻击时自动附带该效果";
	m_MArrorTips[EP_First_SetShortcutKey_Fire] = "将鼠标移到目标身上，点击快捷键即可释放";
	m_MArrorTips[EP_First_SetShortcutKey_Poison] = "该技能需要毒药方可使用，毒药可在药材商处购买";
	m_MArrorTips[EP_First_SetShortcutKey_Symbol] = "该技能需要道符方可使用，道符可在杂货商处购买";
	m_MArrorTips[EP_First_Sell] = "将物品拖到此处后，点击确定出售";
	m_MArrorTips[EP_First_Repair] = "将物品拖到此处后，点击确定修理";
	m_MArrorTips[EP_First_Repair] = "将物品拖到此处后，点击确定修理";
	m_MArrorTips[EP_First_Level_30] = "点击进入圣殿纹佩系统";
	m_MArrorTips[EP_First_Open_WeiPeiWnd] = "进入纹佩图鉴查看纹佩获取方法";


	m_MArrorTips[EP_First_PackageFull_Panel_PaoPao] = "包裹负重已满 ，无法携带更多物品";
	m_MArrorTips[EP_First_CutZhangShu_PaoPao] = "鼠标移动到树木上，按住ALT后双击鼠标左键收获木材";
	m_MArrorTips[EP_First_WatchGood_Panel_PaoPao] = "站在物品上，即可拾取该物品";
	m_MArrorTips[EP_First_GetTaoMuJian_PaoPao1] = "点击打开包裹查看所获得的物品信息";
	m_MArrorTips[EP_First_Team_Step1] = "点击打开组队界面";
	m_MArrorTips[EP_First_Team_TeamWnd] = "勾选允许组队后，才可以看到其它玩家的组队邀请";
	//m_MArrorTips[EP_FirstGetNewTask_PaoPao] = "打开任务面板，查看已接任务";
	m_MArrorTips[EP_First_FaYan_Step1] = "选择喊话频道，在聊天框输入发言内容";
	m_MArrorTips[EP_First_FaYan_Step2] = "点击输入喊话内容，按回车键发送";
	m_MArrorTips[EP_First_FaYan_Step3] = "点击开启/停止在聊天框显示喊话频道内容";
	m_MArrorTips[EP_First_UseSkill_PaoPao] = "使用TAB键，可开启/关闭自动施法";
	m_MArrorTips[EP_First_GetFaMuBook_PaoPao] = "双击使用学会伐木技能";
	m_MArrorTips[EP_First_GetLianYuTiYuanKa_PaoPao] = "双击使用可免费进入炼狱地图";
	m_MArrorTips[EP_First_GetAttackSkillBook_PaoPao] = "双击使用学会该技能";
	m_MArrorTips[EP_First_GetTaoMuJian_PaoPao] = "双击可将道具装备在人物身上";
	m_MArrorTips[EP_FirstWalkToZhongZhou_PaoPao] = "点击按钮，查看当前区域地图";
	m_MArrorTips[EP_First_WeigthFull_PaoPao] = "人物负重量满时 ，无再法装备其它物品";
	m_MArrorTips[EP_First_WanLiFull_PaoPao] = "人物无法拿起比腕力更重的武器";
	m_MArrorTips[EP_FirstLevelUp_PaoPao] = "点击查看当前等级信息";
	m_MArrorTips[EP_FirstOpenMapWnd_PaoPao] = "点击下拉框，可选择查看其它区域地图";
	m_MArrorTips[EP_FirstSelectMap_PaoPao] = "点击下拉框，查看此地图的附属地图";
	m_MArrorTips[EP_First_GetQunYingBaoJian_PaoPao] = "双击使用群英宝鉴，可获得各种奖励";
	m_MArrorTips[EP_First_ShortcutKeyWnd_PaoPao] = "点击对应按钮，为技能设置快捷键，然后点击确认按钮";
	m_MArrorTips[EP_First_Skill_SkillWnd_PaoPao] = "点击技能图标，设置最新技能快捷方式";
	m_MArrorTips[EP_First_StudySkill_PaoPao] = "打开技能界面，设置最新技能快捷方式";
	m_MArrorTips[EP_FirstOpenTaskWnd_PaoPao] = "点击带有下划线的文字，启动自动寻路";
	m_MArrorTips[EP_First_OpenWuSeTaskWnd_PaoPao] = "点击带有下划线的文字，也可以领取任务。";
	m_MArrorTips[EP_First_FirstHurt_PaoPao] = "双击物品或者使用快捷键来使用物品";



	this->Clear();
}

COtherData::~COtherData(void)
{
}

DWORD COtherData::GetHairColor(int i)
{
	if( i < 0 || i >= MAX_HAIR_COLOR)
		return 0;
	else
		return m_dwHairColor[i];
}

DWORD COtherData::GetBodyColor(int i,int iSex)
{
	 if( i<0 || i>= MAX_BODY_COLOR)
		 return 0;
	 else
		 return m_dwBodyColor[iSex][i];
}

const char* COtherData::GetCharName()
{
	if(m_iCharNo >= m_VCharList.size())
		return "";

	return m_VCharList.at(m_iCharNo).szRoleName;
}

void COtherData::CopyToSelf()
{
	if(m_iCharNo >= m_VCharList.size())
		return;

	_CharInfo& inf = m_VCharList.at(m_iCharNo);

	SELF.SetName(inf.szRoleName);
	SELF.SetSex(inf.bySex);
	SELF.SetCareer(inf.byJob);
	SELF.SetLevel(inf.wLevel);
}

void COtherData::JumpClear()
{
	m_dwCenserStartTime = 0;
	m_dwCenserDurTime = 0;
	m_bCenserStart = false;

	//m_bDrinkSuperBlue = false;
	//m_bDrinkSuperRed = false;
	m_dwLockHunDunNvWangLockID = 0;
	m_dwHunDunNvWangLock = 0;
}

void COtherData::Clear()
{
	//memset(&m_sWuXingFlag,0,sizeof(WUXINGFLAG));
	m_strTel.clear();
	m_vecRelation.clear();
	m_vecLeaveWords.clear();
	m_VCommonConfirm.clear();
	m_VOtherMsg.clear();
	m_VOtherMsg2.clear();
	m_VAnotherMsg.clear();
	m_NoteTitle.clear();
	m_NoteBody.clear();
	m_sOffLinePrivate.bLeaveWords = false;
	m_dwTelTime = 0;
	m_dwCenserStartTime = 0;
	m_dwCenserDurTime = 0;
	m_bCenserStart = false;
	m_dwSetMsgStartTime = 0;
	m_strSetMsg.clear();
	m_MazeMap.clear();
	m_KfzGuildData.VKfzMemberList.clear();
	m_TimeOut.clear();
	m_byCreditAlertState = 0;
	m_strPhyleName.clear();

	m_bChangeLogo = false;

	m_iDelCharSelectIdx = -1;
	m_iUnDelCharSelectIdx = -1;
	m_dwEnterGameTime = 0;
	m_bNeedFlashProduceTab = false;
	m_iFlashRelationTabType = -1;

	m_MeritoriousnessUpdate.dwMeriToExp = 0;
	m_MeritoriousnessUpdate.dwMostPerHour = 0;
	m_MeritoriousnessUpdate.dwNextLevel = 0;
	m_MeritoriousnessUpdate.dwPresToExp = 0;

	m_bHaveRequestQunYingReport = false;
	m_VQunYingReports.clear();
	m_dwLastHeartBeat = 0;

	m_iExp100 = 0;
	m_strJieYiLeader = "";

	for (int i = 0; i < 4; ++i)
	{
		m_vSubWenPei[i].clear();
	}

	m_bHaveQiYu = false;
	m_iQiYuState = 0;
	m_iQiYuFrame = 0;
	m_bQiYuShow = true;
	m_vQiYuItems.clear();


	m_dwFireArtTime = 0;
	m_dwFireArtStartTime = 0;

	ZeroMemory(&m_SanWeiZhenHuoInfo,sizeof(m_SanWeiZhenHuoInfo));
	ZeroMemory(&m_SVipTrade,sizeof(m_SVipTrade));

	m_vBings.clear();

	memset(m_YiHuoMsg.szType,0,sizeof(m_YiHuoMsg.szType));
	memset(m_YiHuoMsg.szBtn1,0,sizeof(m_YiHuoMsg.szBtn1));
	memset(m_YiHuoMsg.szBtn2,0,sizeof(m_YiHuoMsg.szBtn2));
	m_YiHuoMsg.iShowTime = 0;
	m_YiHuoMsg.iStartTime = 0;

	m_dwLockHunDunNvWangLockID = 0;
	m_dwHunDunNvWangLock = 0;
	m_sHorseManShip.clear();

}

void COtherData::ParseNote(std::string& str)
{
	m_NoteTitle.clear();
	m_NoteBody.clear();

	char *p0 = (char*)str.c_str();
	char *p = p0;

	int iNum = 4;

	bool bFirstBody = false;
	bool bBody = false;

	NoteTitle temp;
	temp.iBegin = temp.iEnd = -1;

	while(1)
	{
		if(*p == '<')				// title开始				
		{
			if(temp.iEnd >= temp.iBegin && temp.iEnd != -1)
				m_NoteTitle.push_back(temp);
			temp.iBegin = temp.iEnd = -1;
			bBody = false;
			p0 = p + 1;
		}
		else if(*p == '>')			// title结束
		{
			if(p > p0)
			{
				std::string sTitle(p0,p);
				temp.sTitle = sTitle;
			}
			bBody = false;
			bFirstBody = true;						
			p0 = p + 1;
		}
		else if(*p == '#')			// 一行body开始
		{
			p0 = p;
			bBody = true;
		}
		else if(*p == 13 || *p == '\0')			// 一行body结束
		{
			if(bBody && p > p0 + 4)
			{
				std::string sNum(p0 + 1,p0 + 4);
				temp.iEnd = atoi(sNum.c_str());
				if(bFirstBody)
				{
					temp.iBegin = temp.iEnd;
					bFirstBody = false;
				}

				std::string sNo(p0,p0 + 4);
				std::string sBody(p0 + 4,p);
				m_NoteBody[sNo] = sBody;
			}
			if(*p == '\0')
				break;
			p0 = p + 2;
			bBody = false;
		}
		p++;
	}
	if(temp.iEnd >= temp.iBegin && temp.iEnd != -1)
		m_NoteTitle.push_back(temp);
}

void COtherData::SetCenserStartTime(DWORD dwStartTime)
{
	if(m_dwCenserStartTime != 0 && dwStartTime == 0 && g_pMagicCtrl)
	{
		g_pMagicCtrl->FinishMagic(g_pMagicCtrl->FindMagicByID(MAGICID_SENCER_EFFECT));
	}

	m_dwCenserStartTime = dwStartTime;
}

const char * COtherData::GetTelStr(WORD& color)  //得到千里传音
{
	color = m_wTelColor;
	return m_strTel.c_str();
}

void COtherData::SetTelStr(const char* str,WORD color) //设置千里传音
{
	m_wTelColor = color;
	m_strTel.assign(str);
	m_dwTelTime = GetTickCount();
}

Magic_Show_s** COtherData::GetFireShowList(int x,int y)
{
	if(x >= 0 && y >= 0)
	{
		return &(m_FireMagic[x % MAPARR_WIDTH][y % MAPARR_WIDTH]);
	}
	else 
	{
		return NULL;
	}
}

void COtherData::ClearFireTexShow()
{
	if(m_TexShow)
	{
		DEL_TEX_SHOW(m_TexShow);
	}

	m_TexShow = NULL;
}

bool COtherData::ClearFireShowList(int x,int y)
{
	if(!(x >= 0 && y >= 0))
		return false;

	Magic_Show_s* ms = m_FireMagic[x % MAPARR_WIDTH][y % MAPARR_WIDTH],*ms2 = NULL;
	if(!ms)
		return false;

	while(ms)
	{
		ms2 = ms->pNext;
		
		if(ms->ref->pTarget)
			TargetDataClear(ms->ref->pTarget);

		g_pMagicCtrl->FinishMagic(ms);	
		
		ms = ms2;
	}
	return true;
}

void COtherData::ClearAllFireShowList()
{
	for(int i = 0;i < MAPARR_WIDTH;i++)
	{
		for(int j = 0;j < MAPARR_WIDTH;j++)
		{
			ClearFireShowList(i,j);
		}
	}
}

void COtherData::GetLogoTex(const char * pBuf)
{
	DWORD dwTexID = *((DWORD*)(pBuf+4));
	long len =  *((DWORD*)pBuf);

	if( len > 20 && g_pTexMgr->IsValidOldTex((UCHAR *)(pBuf+16),len))
	{ 
		char name[128] = "";
		sprintf(name,"%s\\Cache",GetGameDataDir());
		CreateDirectory(name,NULL);		
		sprintf(name,"%s\\Cache\\%08X.tex",GetGameDataDir(),dwTexID);
		FILE * fp = fopen(name,"wb");
		fwrite(pBuf+16,len,1,fp);
		fclose(fp);

		if(IsChangeLogo())
		{	
			//会长第一次取图片
			char str[128]={0};
			int size =0;

			strcpy(str,"@@guildlogo");
			size = strlen(str);
			str[size] = 0x0D;
			ltoa(dwTexID,str+size+1,10);

			g_pGameControl->SEND_Exchange_Menu_Click(*((DWORD*)(pBuf+8)),str);
			SetChangeLogo(false);
			SELF.SetGettingGuildLogo(false);


		}
		else
		{
			if(dwTexID == SELF.GetGuildLogoID())
				SELF.SetGettingGuildLogo(false);
			else if(dwTexID == g_OtherData.OtherPlayer().GetGuildLogoID())
				g_OtherData.OtherPlayer().SetGettingGuildLogo(false);
			/*{
			g_pGameData->GetSelfCharacter().SettingGuildLogo(false);
			g_pGameData->GetOtherSimpleData().GetOtherPlayerInfo().SettingGuildLogo(false);
			}*/

		}
	}
	else //写日至
	{
		SetChangeLogo(false);
	}
}

bool COtherData::AddTimeOut(_TimeOut &timeout)
{
	for (int i = 0; i < m_TimeOut.size(); i++)
	{
		_TimeOut &tou = m_TimeOut[i];
		if ((tou.iType == timeout.iType) && (tou.dwMonsterID == timeout.dwMonsterID))
		{
			tou.bShowSecond = timeout.bShowSecond;
			tou.dwLastTime = timeout.dwLastTime;
			tou.dwStartTime = timeout.dwStartTime;
			tou.iLastMusicSecond = timeout.iLastMusicSecond;
			tou.strMsg = timeout.strMsg;
			return false;
		}
	}

	m_TimeOut.push_back(timeout);	
	return true;
}

void COtherData::SetMeritoriousnessUpdate(DWORD dwNextLevel,DWORD dwMostPerHour,DWORD dwMeriToExp,DWORD dwPresToExp)
{
	m_MeritoriousnessUpdate.dwNextLevel = dwNextLevel;
	m_MeritoriousnessUpdate.dwMostPerHour = dwMostPerHour;
	m_MeritoriousnessUpdate.dwMeriToExp = dwMeriToExp;
	m_MeritoriousnessUpdate.dwPresToExp = dwPresToExp;
};

bool COtherData::IsRtsSummonPoint(int x,int y)
{ 
	for (vector<SPoint>::iterator itr = m_VRtsSummonPoint.begin(); itr != m_VRtsSummonPoint.end(); itr++)
	{
		if (abs(itr->x - x) <= 1 && abs(itr->y - y) <= 1)
		{
			x = itr->x;
			y = itr->y;
			return true;
		}
	}

	return false;
}

VSubWenPei& COtherData::GetSubWenPei(int i) 
{
	if (i >= 0 && i < 4)
	{
		return m_vSubWenPei[i];
	}
	return m_vSubWenPei[0];
}

int COtherData::GetSubWenPeiDraw(int i)
{
	switch (i)
	{
	case 0:
		return 1;
	case 1:
		return 3;
	case 2:
		return 2;
	case 3:
		return 0;
	default:
		break;
	}

	return 0;
}

bool COtherData::IsCanClickQiYu()
{
	if (m_bHaveQiYu)
	{
		if (m_iQiYuState == 1 || m_iQiYuState == 2)
		{
			if (m_bQiYuShow)
			{
				return true;
			}
		}
	}
	return false;
}

void COtherData::SetQiYuState(int i)	
{ 
	m_iQiYuState = i; 
	if (m_iQiYuState == 0) 
		m_iQiYuFrame = 0;
}


S_Bing* COtherData::GetBingByType(BYTE byType)
{
	for (size_t i = 0; i < m_vBings.size(); ++i)
	{
		if (m_vBings[i].info.byType == byType)
		{
			return &m_vBings[i];
		}
	}
	return 0;
}

void COtherData::InitAlarmState(void)
{
	// 各种状态的颜色
	m_dwAlarmColor[DEADLY]		= 0xFF808080;
	m_dwAlarmColor[VERY_SLOW]	= 0xFFFF9A00;
	m_dwAlarmColor[SLOW]		= 0xFFFFCF00;
	m_dwAlarmColor[NORMAL]		= 0xFFFFFF00;
	m_dwAlarmColor[GOOD]		= 0xFF9CCF00;
	m_dwAlarmColor[VERY_GOOD]	= 0xFF00FF00;

	// 显示位置
	if (g_EutUiType == EUT_CLASSIC)
	{
		m_Alarm[0].x = 629;
		m_Alarm[0].y = 44;
		m_Alarm[1].x = 641;
		m_Alarm[1].y = 44;
		m_Alarm[2].x = 653;
		m_Alarm[2].y = 44;
	}
	else
	{
		m_Alarm[0].x = 140;
		m_Alarm[0].y = 47;
		m_Alarm[1].x = 152;
		m_Alarm[1].y = 47;
		m_Alarm[2].x = 164;
		m_Alarm[2].y = 47;
	}

	// 初始状态
	m_Alarm[0].iState = GOOD;
	m_Alarm[1].iState = GOOD;
	m_Alarm[2].iState = GOOD;	

	// 提示名称
	strcpy(m_Alarm[0].sAlarmName,"本机当前性能");
	strcpy(m_Alarm[1].sAlarmName,"网络连接状况");
	strcpy(m_Alarm[2].sAlarmName,"服务器当前状况");

	// 每类的具体提示
	// 本机的
	memset(m_Alarm[0].sAlarmTip,0,STATE_NUM * 64);
	strcpy(m_Alarm[0].sAlarmTip + 0 * 64,"奇差");
	strcpy(m_Alarm[0].sAlarmTip + 1 * 64,"很差");
	strcpy(m_Alarm[0].sAlarmTip + 2 * 64,"较差");
	strcpy(m_Alarm[0].sAlarmTip + 3 * 64,"一般");
	strcpy(m_Alarm[0].sAlarmTip + 4 * 64,"较好");
	strcpy(m_Alarm[0].sAlarmTip + 5 * 64,"优异");

	// 网络的
	memset(m_Alarm[1].sAlarmTip,0,STATE_NUM * 64);
	strcpy(m_Alarm[1].sAlarmTip + 0 * 64,"断开");
	strcpy(m_Alarm[1].sAlarmTip + 1 * 64,"很差");
	strcpy(m_Alarm[1].sAlarmTip + 2 * 64,"较差");
	strcpy(m_Alarm[1].sAlarmTip + 3 * 64,"一般");
	strcpy(m_Alarm[1].sAlarmTip + 4 * 64,"较好");
	strcpy(m_Alarm[1].sAlarmTip + 5 * 64,"流畅");

	// 服务器的
	memset(m_Alarm[2].sAlarmTip,0,STATE_NUM * 64);
	strcpy(m_Alarm[2].sAlarmTip + 0 * 64,"没响应");
	strcpy(m_Alarm[2].sAlarmTip + 1 * 64,"很慢");
	strcpy(m_Alarm[2].sAlarmTip + 2 * 64,"缓慢");
	strcpy(m_Alarm[2].sAlarmTip + 3 * 64,"一般");
	strcpy(m_Alarm[2].sAlarmTip + 4 * 64,"较好");
	strcpy(m_Alarm[2].sAlarmTip + 5 * 64,"优异");
}

void COtherData::UpdateLocalState(void)
{
	int iFPS = g_pGfx->GetFPS();
	if(iFPS < 4)
		m_Alarm[0].iState = DEADLY;
	else if(iFPS < 8)
		m_Alarm[0].iState = VERY_SLOW;
	else if(iFPS < 14)
		m_Alarm[0].iState = SLOW;
	else if(iFPS < 19)
		m_Alarm[0].iState = NORMAL;
	else if(iFPS < 28)
		m_Alarm[0].iState = GOOD;
	else
		m_Alarm[0].iState = VERY_GOOD;
}

void COtherData::SetAlarmState(int iAlarm, int iState)
{
	if(iAlarm < 0 || iAlarm >= 3)
		return;

	m_Alarm[iAlarm].iState = iState;
}