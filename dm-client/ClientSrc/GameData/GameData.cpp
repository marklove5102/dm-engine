#include "GameData.h"
#include "Global/Global.h"
#include "OtherData.h"
#include "TalkMgr.h"
#include "PetData.h"
#include "GuildData.h"
#include "NpcData.h"
#include "BoothData.h"
#include "TradeData.h"
#include "TaskData.h"
#include "TagText.h"
#include "Global/Interface/TexManagerInterface.h"
#include "Global/Interface/AudioInterface.h"
#include "BaseClass/Control/GoodGrid.h"
#include "GameAI/AIAutoEatMgr.h"
#include "LoginData.h"
#include "WooolStoreData.h"
#include "ConfigData.h"
#include "MagicCtrlMgr.h"
#include "RangeStruct.h"
#include "GameMap/GameMap.h"
#include "GameMap/MinMap.h"
#include "Global\GlobalMsg.h"
#include "BaseClass/Control/Control.h"
#include "GameAI/AIPromptMgr.h"
#include "GameAI/PathFinder.h"
#include "XmlCfg.h"
#include "Global/Interface/TexmanagerInterface.h"

//跟游戏逻辑相关的一些全局变量

UINT	MAX_PACKAGE_ELEMENT = 66 + 60 + 60;//增加到3页
UINT    MAX_PACKAGE_NUM     = 60 + 60 + 60;
UINT    MAX_PET_PACKAGE = 0;

DWORD   g_dwDeathTime  = 0;

CGameData::CGameData(void):
m_dwSingingTimeStart(0),
m_nSingingTechID(0)
{
    g_pChar = &m_self;
	InitMirColor(); //初始化调色板
    m_iDropMoneyFrom	= EDMYF_NONE;
    m_iDropYuanBaoFrom	= EDMYF_NONE;
	m_bTroop = false;
	m_iMouseType = MOUSE_STANDARD;
	m_MemberPos.clear();


	m_IsAutoKillMonster = false;

	//m_int64Status = 0;
	m_bSelfCursor = false;
	m_bHardCursor = true;
	m_iSuperArmCondenseType = 0;
	m_dwMouseTexID = MAKELONG(376,PACKAGE_INTERFACE);
	m_mouseRM = RM_ALPHA;
	IniSkillData();
	LoadDynMiniMapData();

	m_int64PaoPaoStatus = -1;
	m_int64PaoPaoStatus_Closed = -1;
	m_bShowWuXing = true;
	for (int i = 0;i<16;i++)
	{
		m_byNewHandStatus[i] = 0xFF;
	}
	//m_int64Status = -1;

	m_eSendTalkChannelType = ETC_COMMON;

	m_iNoticeIEState = 0;
	m_iNoticeFrame = 100000;

	m_bShowFireLianHuaHelp = true;
	m_iUseHuLuGoodPos = -1;
	m_strCheckMask.clear();
}

CGameData::~CGameData(void)
{
}

void CGameData::Clear()
{
	m_IsAutoKillMonster = false;

	g_pMagicCtrl->FinishAllMagic();
	m_self.clear();
	m_vecTroopMember.clear();

	g_BoothData.Clear();
	MAX_PET_PACKAGE = 0;
	g_OtherBoothData.Clear();
	g_PetBoothData.ClearC2CGoods();
	g_GuildData.Clear();
	g_TalkMgr.Clear();
	g_PetData.Clear();
	g_TagTextMgr.Clear();
	g_TradeData.Clear();
	g_TaskData.ClearTask();
	g_OtherData.Clear();
	m_MemberPos.clear();
	m_iMouseType = MOUSE_STANDARD;
	m_iDropMoneyFrom	= EDMYF_NONE;
	m_iDropYuanBaoFrom	= EDMYF_NONE;
	m_bTroop = false;
	m_int64PaoPaoStatus = -1;
	for (int i = 0;i<16;i++)
	{
		m_byNewHandStatus[i] = 0xFF;
	}
	//m_int64Status = -1;
	m_iSuperArmCondenseType = 0;
	m_iMouseType = MOUSE_STANDARD;
	m_dwMouseTexID = MAKELONG(376,PACKAGE_INTERFACE);
	m_mouseRM = RM_ALPHA;
	CGoodGrid::ClearGoodFrom();
    g_Login.SetShowRainBow(true);
    //g_Login.SetEnterCheckType(0);
	g_Login.SetIsTianJueMoYu(false);
	g_WooolStoreMgr.ClearData() ;
	m_BoxInfo.clear();
	SetMouseType(0);
	//删除自己的排名信息
	ClearMyRangeData();
	g_pGameMap->ClearMap();
	g_AIPromptMgr.set_str("");
	g_PetData.GetPetStatus().clear();
	m_bShowWuXing = true;
// 	g_TrusteeshipData.Clear();
// 	g_pControl->PopupWindow(MSG_CTRL_QUITTNEUP_WND, OPER_CLOSE);

	while(!m_HistoryLines.empty())
	{
		CStringLine* pLine = m_HistoryLines.back();
		SAFE_DELETE(pLine);
		m_HistoryLines.pop_back();
	}

	g_XmlCfg.Clear();
	m_strNoticeIEURL.clear();
	m_iNoticeIEState = 0;
	m_iNoticeFrame = 100000;

	m_bShowFireLianHuaHelp = true;
	if (g_pStreamMgr)
	{
		g_pStreamMgr->ClearDownloadingFilesList();
	}
	if (g_pTexMgr)
	{
		g_pTexMgr->ClearAllNullTex();
	}

	JumpClear();
	m_strCheckMask.clear();
}
void CGameData::LoadDynMiniMapData()
{
	if (!g_pStreamMgr)
	{
		return;
	}

	DataStreamInterface* stream = g_pStreamMgr->OpenDataFile("config\\DynMiniMapObj.ini",false,true,false,EP_MOST_HIGH);
	if (!stream)
	{
		return;
	}

	m_MDynMiniMapObj.clear();
	while(!stream->eof())
	{
		std::string inbuf = stream->getLine(false);
		size_t ipos = inbuf.find_first_of('=');
		if (ipos != -1)
		{
			std::string strMapName = inbuf.substr(0,ipos);
			std::vector<DynMiniMapObj> vDynMiniMapObj;

			int iCurPos = ipos + 1;
			while (iCurPos < inbuf.size())
			{
				DynMiniMapObj dynobj;
				dynobj.wID = StringUtil::toUInt(inbuf,iCurPos);				

				dynobj.wX = StringUtil::toUInt(inbuf,iCurPos);
				dynobj.wY = StringUtil::toUInt(inbuf,iCurPos);
				dynobj.wMX = StringUtil::toUInt(inbuf,iCurPos);
				dynobj.wMY = StringUtil::toUInt(inbuf,iCurPos);
				dynobj.wTex = StringUtil::toUInt(inbuf,iCurPos);
				DWORD dwShow = StringUtil::toUInt(inbuf,iCurPos);
				dynobj.bShow = (dwShow != 0);
				if (dynobj.wID >= 1 && dynobj.wID <= 5)
				{
					dynobj.strDes = "冰路机关";
				}
				else if (dynobj.wID >= 6 && dynobj.wID <= 10)
				{
					dynobj.strDes = "火链机关";
				}
				vDynMiniMapObj.push_back(dynobj);
			}

			m_MDynMiniMapObj[strMapName] = vDynMiniMapObj;
		}
	}

	SAFE_DELETE(stream);
}

void CGameData::DelPromptInfo(DWORD dwID)
{
	if (m_VPromptInfo.size() <= 0) return;

	VPROMPTINFO::iterator it = m_VPromptInfo.begin();
	if (dwID == 0xFFFFFFFF)
	{
		m_VPromptInfo.erase(it);
	}
	else
	{
		while (it != m_VPromptInfo.end())
		{
			if (it->dwID == dwID)
			{
				m_VPromptInfo.erase(it);
				break;
			}
			it++;
		}
	}	
}

void CGameData::DelCareForInfo(DWORD dwID)
{
	if (m_VCareForInfo.size() <= 0) return;

	VCAREFORINFO::iterator it = m_VCareForInfo.begin();
	if (dwID == 0xFFFFFFFF)
	{
		m_VCareForInfo.erase(it);
	}
	else
	{
		while (it != m_VCareForInfo.end())
		{
			if (it->dwID == dwID)
			{
				m_VCareForInfo.erase(it);
				break;
			}
			it++;
		}
	}
}

void CGameData::ClearCareForInfo()
{
	m_VCareForInfo.clear();
}

void CGameData::AddCareForInfo(CareForInfo& info)
{
	VCAREFORINFO::iterator it = m_VCareForInfo.begin();
	while (it != m_VCareForInfo.end())
	{
		if (it->dwType == info.dwType)
		{
			it++;
		}
		else
		{
			//不等
			if (it->dwType == PROMPT_URGENT)
			{
				it++;
			}
			else
			{
				m_VCareForInfo.insert(it,info);
				return;
			}
		}
	}

	m_VCareForInfo.push_back(info);
}

void CGameData::AddCareForInfo(_sPromptInfo* pPromptInfo)
{
	if (!pPromptInfo) return;
	CareForInfo info;
	info.dwType = pPromptInfo->bEmergent?PROMPT_URGENT:PROMPT_NORMAL;
	info.dwID = pPromptInfo->dwID;
	info.strTile = pPromptInfo->strTile;
	info.strContent = pPromptInfo->strDes;

	VCAREFORINFO::iterator it = m_VCareForInfo.begin();
	while (it != m_VCareForInfo.end())
	{
		if (it->dwType == info.dwType)
			it++;
		else
		{
			//不等
			if (it->dwType == PROMPT_URGENT)
				it++;
			else
			{
				m_VCareForInfo.insert(it,info);
				return;
			}
		}
	}

	m_VCareForInfo.push_back(info);
}

void CGameData::ResetDynMiniMapObj(char* szMapName)
{
	if (!szMapName) return;

	MDynMiniMapObj& MiniMapObj = g_pGameData->GetDynMinMapObj();
	MDynMiniMapObj::iterator it = MiniMapObj.find(szMapName);
	if (it != MiniMapObj.end())
	{
		std::vector<DynMiniMapObj>& VDynMinMap = it->second;
		for (int i = 0;i < VDynMinMap.size();i++)
		{
			VDynMinMap[i].bShow = true;
		}
	}
}

void CGameData::SetDynMiniMapObj(char* szMapName,WORD wIndex,bool bShow)
{
	if (!szMapName || wIndex == 0 ) return;

	MDynMiniMapObj& MiniMapObj = g_pGameData->GetDynMinMapObj();
	MDynMiniMapObj::iterator it = MiniMapObj.find(szMapName);
	if (it != MiniMapObj.end())
	{
		std::vector<DynMiniMapObj>& VDynMinMap = it->second;
		for (int i = 0;i < VDynMinMap.size();i++)
		{
			if(VDynMinMap[i].wID == wIndex)
			{
				VDynMinMap[i].bShow = bShow;
				CMinMap::MExLocalNpcInfo& localNpc = g_pMinMap->GetExLocalNpc();
				if(bShow)
				{
					BOOL bFound = FALSE;
					vector<NpcInfo_s>& vNpcInfo = localNpc[szMapName];
					for (int j = 0;j<vNpcInfo.size();j++)
					{
						if (vNpcInfo[j].iX ==  VDynMinMap[i].wMX && vNpcInfo[j].iY == VDynMinMap[i].wMY)
						{
							bFound = true;
							break;
						}
					} 

					if(!bFound)
					{
						NpcInfo_s npcInfo;
						npcInfo.iX = VDynMinMap[i].wMX;
						npcInfo.iY = VDynMinMap[i].wMY;
						npcInfo.iExNpcType = 4;
						if (wIndex >= 1 && wIndex <= 5)
						{
							npcInfo.strDesc = "由玄冰柱控制的道路机关";
						}
						else if (wIndex >= 6 && wIndex <= 10)
						{
							npcInfo.strDesc = "由离火柱控制的铁链机关";
						}

						localNpc[szMapName].push_back(npcInfo);
					}
				}
				else
				{//为消失，将之清除
					CMinMap::MExLocalNpcInfo::iterator it = localNpc.find(szMapName);
					if (it != localNpc.end())
					{
						vector<NpcInfo_s>& vNpcInfo = it->second;
						for (int j = 0;j<vNpcInfo.size();j++)
						{
							if (vNpcInfo[j].iExNpcType == 4 && vNpcInfo[j].iX == VDynMinMap[i].wMX && vNpcInfo[j].iY == VDynMinMap[i].wMY)
							{
								vNpcInfo.erase(vNpcInfo.begin() + j);
								break;
							}
						}
					}
				}

				break;
			}
		}
	}
}

void CGameData::SafeClear()
{
}

void CGameData::JumpClear()
{
	MapArray.Clear();
	if(!SELF.IsDead())
	{
		SELF.InitAction(ACTION_STAND);
		SELF.ClearAllNext();
	}

	while(!m_mapExternString.empty())
	{
		IDStringMap::iterator itr = m_mapExternString.begin();
		m_mapExternString.erase(itr);
	}

	SELF.GetMapPathFinder().SetOnRoute(0);
	m_iMouseType = MOUSE_STANDARD;
}

SDynRecover* CGameData::GetDynRecover(DWORD dwID,WORD wStartX,WORD wStartY)
{
	for (int i = 0;i<m_VDynRecover.size();i++)
	{
		if(m_VDynRecover[i].dwID == dwID)
		{
			if (wStartX == m_VDynRecover[i].wStartX && wStartY == m_VDynRecover[i].wStartY)
			{
				return &(m_VDynRecover[i]);
			}
		}
	}

	return NULL;
}

void CGameData::InitMirColor(void)
{
	m_dwMirColor[  0] = 0xff000000;
	m_dwMirColor[  1] = 0xff800000;
	m_dwMirColor[  2] = 0xff008000;
	m_dwMirColor[  3] = 0xff808000;
	m_dwMirColor[  4] = 0xff000080;
	m_dwMirColor[  5] = 0xff800080;
	m_dwMirColor[  6] = 0xff008080;
	m_dwMirColor[  7] = 0xffc0c0c0;
	m_dwMirColor[  8] = 0xff558097;
	m_dwMirColor[  9] = 0xff9db9c8;
	m_dwMirColor[ 10] = 0xff7b7373;
	m_dwMirColor[ 11] = 0xff2d2929;
	m_dwMirColor[ 12] = 0xff5a5252;
	m_dwMirColor[ 13] = 0xff635a5a;
	m_dwMirColor[ 14] = 0xff423939;
	m_dwMirColor[ 15] = 0xff1d1818;
	m_dwMirColor[ 16] = 0xff181010;
	m_dwMirColor[ 17] = 0xff291818;
	m_dwMirColor[ 18] = 0xff100808;
	m_dwMirColor[ 19] = 0xfff27971;
	m_dwMirColor[ 20] = 0xffe1675f;
	m_dwMirColor[ 21] = 0xffff5a5a;
	m_dwMirColor[ 22] = 0xffff3131;
	m_dwMirColor[ 23] = 0xffd65a52;
	m_dwMirColor[ 24] = 0xff941000;
	m_dwMirColor[ 25] = 0xff942918;
	m_dwMirColor[ 26] = 0xff390800;
	m_dwMirColor[ 27] = 0xff731000;
	m_dwMirColor[ 28] = 0xffb51800;
	m_dwMirColor[ 29] = 0xffbd6352;
	m_dwMirColor[ 30] = 0xff421810;
	m_dwMirColor[ 31] = 0xffffaa99;
	m_dwMirColor[ 32] = 0xff5a1000;
	m_dwMirColor[ 33] = 0xff733929;
	m_dwMirColor[ 34] = 0xffa54a31;
	m_dwMirColor[ 35] = 0xff947b73;
	m_dwMirColor[ 36] = 0xffbd5231;
	m_dwMirColor[ 37] = 0xff522110;
	m_dwMirColor[ 38] = 0xff7b3118;
	m_dwMirColor[ 39] = 0xff2d1810;
	m_dwMirColor[ 40] = 0xff8c4a31;
	m_dwMirColor[ 41] = 0xff942900;
	m_dwMirColor[ 42] = 0xffbd3100;
	m_dwMirColor[ 43] = 0xffc67352;
	m_dwMirColor[ 44] = 0xff6b3118;
	m_dwMirColor[ 45] = 0xffc66b42;
	m_dwMirColor[ 46] = 0xffce4a00;
	m_dwMirColor[ 47] = 0xffa56339;
	m_dwMirColor[ 48] = 0xff5a3118;
	m_dwMirColor[ 49] = 0xff2a1000;
	m_dwMirColor[ 50] = 0xff150800;
	m_dwMirColor[ 51] = 0xff3a1800;
	m_dwMirColor[ 52] = 0xff080000;
	m_dwMirColor[ 53] = 0xff290000;
	m_dwMirColor[ 54] = 0xff4a0000;
	m_dwMirColor[ 55] = 0xff9d0000;
	m_dwMirColor[ 56] = 0xffdc0000;
	m_dwMirColor[ 57] = 0xffde0000;
	m_dwMirColor[ 58] = 0xfffb0000;
	m_dwMirColor[ 59] = 0xff9c7352;
	m_dwMirColor[ 60] = 0xff946b4a;
	m_dwMirColor[ 61] = 0xff734a29;
	m_dwMirColor[ 62] = 0xff523118;
	m_dwMirColor[ 63] = 0xff8c4a18;
	m_dwMirColor[ 64] = 0xff884411;
	m_dwMirColor[ 65] = 0xff4a2100;
	m_dwMirColor[ 66] = 0xff211810;
	m_dwMirColor[ 67] = 0xffd6945a;
	m_dwMirColor[ 68] = 0xffc66b21;
	m_dwMirColor[ 69] = 0xffef6b00;
	m_dwMirColor[ 70] = 0xffff7700;
	m_dwMirColor[ 71] = 0xffa59484;
	m_dwMirColor[ 72] = 0xff423121;
	m_dwMirColor[ 73] = 0xff181008;
	m_dwMirColor[ 74] = 0xff291808;
	m_dwMirColor[ 75] = 0xff211000;
	m_dwMirColor[ 76] = 0xff392918;
	m_dwMirColor[ 77] = 0xff8c6339;
	m_dwMirColor[ 78] = 0xff422910;
	m_dwMirColor[ 79] = 0xff6b4218;
	m_dwMirColor[ 80] = 0xff7b4a18;
	m_dwMirColor[ 81] = 0xff944a00;
	m_dwMirColor[ 82] = 0xff8c847b;
	m_dwMirColor[ 83] = 0xff6b635a;
	m_dwMirColor[ 84] = 0xff4a4239;
	m_dwMirColor[ 85] = 0xff292118;
	m_dwMirColor[ 86] = 0xff463929;
	m_dwMirColor[ 87] = 0xffb5a594;
	m_dwMirColor[ 88] = 0xff7b6b5a;
	m_dwMirColor[ 89] = 0xffceb194;
	m_dwMirColor[ 90] = 0xffa58c73;
	m_dwMirColor[ 91] = 0xff8c735a;
	m_dwMirColor[ 92] = 0xffb59473;
	m_dwMirColor[ 93] = 0xffd6a573;
	m_dwMirColor[ 94] = 0xffefa54a;
	m_dwMirColor[ 95] = 0xffefc68c;
	m_dwMirColor[ 96] = 0xff7b6342;
	m_dwMirColor[ 97] = 0xff6b5639;
	m_dwMirColor[ 98] = 0xffbd945a;
	m_dwMirColor[ 99] = 0xff633900;
	m_dwMirColor[100] = 0xffd6c6ad;
	m_dwMirColor[101] = 0xff524229;
	m_dwMirColor[102] = 0xff946318;
	m_dwMirColor[103] = 0xffefd6ad;
	m_dwMirColor[104] = 0xffa58c63;
	m_dwMirColor[105] = 0xff635a4a;
	m_dwMirColor[106] = 0xffbda57b;
	m_dwMirColor[107] = 0xff5a4218;
	m_dwMirColor[108] = 0xffbd8c31;
	m_dwMirColor[109] = 0xff353129;
	m_dwMirColor[110] = 0xff948463;
	m_dwMirColor[111] = 0xff7b6b4a;
	m_dwMirColor[112] = 0xffa58c5a;
	m_dwMirColor[113] = 0xff5a4a29;
	m_dwMirColor[114] = 0xff9c7b39;
	m_dwMirColor[115] = 0xff423110;
	m_dwMirColor[116] = 0xffefad21;
	m_dwMirColor[117] = 0xff181000;
	m_dwMirColor[118] = 0xff292100;
	m_dwMirColor[119] = 0xff9c6b00;
	m_dwMirColor[120] = 0xff94845a;
	m_dwMirColor[121] = 0xff524218;
	m_dwMirColor[122] = 0xff6b5a29;
	m_dwMirColor[123] = 0xff7b6321;
	m_dwMirColor[124] = 0xff9c7b21;
	m_dwMirColor[125] = 0xffdea500;
	m_dwMirColor[126] = 0xff5a5239;
	m_dwMirColor[127] = 0xff312910;
	m_dwMirColor[128] = 0xffcebd7b;
	m_dwMirColor[129] = 0xff635a39;
	m_dwMirColor[130] = 0xff94844a;
	m_dwMirColor[131] = 0xffc6a529;
	m_dwMirColor[132] = 0xff109c18;
	m_dwMirColor[133] = 0xff428c4a;
	m_dwMirColor[134] = 0xff318c42;
	m_dwMirColor[135] = 0xff109429;
	m_dwMirColor[136] = 0xff081810;
	m_dwMirColor[137] = 0xff081818;
	m_dwMirColor[138] = 0xff082910;
	m_dwMirColor[139] = 0xff184229;
	m_dwMirColor[140] = 0xffa5b5ad;
	m_dwMirColor[141] = 0xff6b7373;
	m_dwMirColor[142] = 0xff182929;
	m_dwMirColor[143] = 0xff18424a;
	m_dwMirColor[144] = 0xff31424a;
	m_dwMirColor[145] = 0xff63c6de;
	m_dwMirColor[146] = 0xff44ddff;
	m_dwMirColor[147] = 0xff8cd6ef;
	m_dwMirColor[148] = 0xff736b39;
	m_dwMirColor[149] = 0xfff7de39;
	m_dwMirColor[150] = 0xfff7ef8c;
	m_dwMirColor[151] = 0xfff7e700;
	m_dwMirColor[152] = 0xff6b6b5a;
	m_dwMirColor[153] = 0xff5a8ca5;
	m_dwMirColor[154] = 0xff39b5ef;
	m_dwMirColor[155] = 0xff4a9cce;
	m_dwMirColor[156] = 0xff3184b5;
	m_dwMirColor[157] = 0xff31526b;
	m_dwMirColor[158] = 0xffdeded6;
	m_dwMirColor[159] = 0xffbdbdb5;
	m_dwMirColor[160] = 0xff8c8c84;
	m_dwMirColor[161] = 0xfff7f7de;
	m_dwMirColor[162] = 0xff000818;
	m_dwMirColor[163] = 0xff081839;
	m_dwMirColor[164] = 0xff081029;
	m_dwMirColor[165] = 0xff081800;
	m_dwMirColor[166] = 0xff082900;
	m_dwMirColor[167] = 0xff0052a5;
	m_dwMirColor[168] = 0xff007bde;
	m_dwMirColor[169] = 0xff10294a;
	m_dwMirColor[170] = 0xff10396b;
	m_dwMirColor[171] = 0xff10528c;
	m_dwMirColor[172] = 0xff215aa5;
	m_dwMirColor[173] = 0xff10315a;
	m_dwMirColor[174] = 0xff104284;
	m_dwMirColor[175] = 0xff315284;
	m_dwMirColor[176] = 0xff182131;
	m_dwMirColor[177] = 0xff4a5a7b;
	m_dwMirColor[178] = 0xff526ba5;
	m_dwMirColor[179] = 0xff293963;
	m_dwMirColor[180] = 0xff104ade;
	m_dwMirColor[181] = 0xff292921;
	m_dwMirColor[182] = 0xff4a4a39;
	m_dwMirColor[183] = 0xff292918;
	m_dwMirColor[184] = 0xff4a4a29;
	m_dwMirColor[185] = 0xff7b7b42;
	m_dwMirColor[186] = 0xff9c9c4a;
	m_dwMirColor[187] = 0xff5a5a29;
	m_dwMirColor[188] = 0xff424214;
	m_dwMirColor[189] = 0xff393900;
	m_dwMirColor[190] = 0xff595900;
	m_dwMirColor[191] = 0xffca352c;
	m_dwMirColor[192] = 0xff6b7321;
	m_dwMirColor[193] = 0xff293100;
	m_dwMirColor[194] = 0xff313910;
	m_dwMirColor[195] = 0xff313918;
	m_dwMirColor[196] = 0xff424a00;
	m_dwMirColor[197] = 0xff526318;
	m_dwMirColor[198] = 0xff5a7329;
	m_dwMirColor[199] = 0xff314a18;
	m_dwMirColor[200] = 0xff182100;
	m_dwMirColor[201] = 0xff183100;
	m_dwMirColor[202] = 0xff183910;
	m_dwMirColor[203] = 0xff63844a;
	m_dwMirColor[204] = 0xff6bbd4a;
	m_dwMirColor[205] = 0xff63b54a;
	m_dwMirColor[206] = 0xff63bd4a;
	m_dwMirColor[207] = 0xff5a9c4a;
	m_dwMirColor[208] = 0xff4a8c39;
	m_dwMirColor[209] = 0xff63c64a;
	m_dwMirColor[210] = 0xff63d64a;
	m_dwMirColor[211] = 0xff52844a;
	m_dwMirColor[212] = 0xff317329;
	m_dwMirColor[213] = 0xff63c65a;
	m_dwMirColor[214] = 0xff52bd4a;
	m_dwMirColor[215] = 0xff10ff00;
	m_dwMirColor[216] = 0xff182918;
	m_dwMirColor[217] = 0xff4a884a;
	m_dwMirColor[218] = 0xff4ae74a;
	m_dwMirColor[219] = 0xff005a00;
	m_dwMirColor[220] = 0xff008800;
	m_dwMirColor[221] = 0xff009400;
	m_dwMirColor[222] = 0xff00de00;
	m_dwMirColor[223] = 0xff00ee00;
	m_dwMirColor[224] = 0xff00fb00;
	m_dwMirColor[225] = 0xff4a5a94;
	m_dwMirColor[226] = 0xff6373b5;
	m_dwMirColor[227] = 0xff7b8cd6;
	m_dwMirColor[228] = 0xff6b7bd6;
	m_dwMirColor[229] = 0xff7788ff;
	m_dwMirColor[230] = 0xffc6c6ce;
	m_dwMirColor[231] = 0xff94949c;
	m_dwMirColor[232] = 0xff9c94c6;
	m_dwMirColor[233] = 0xff313139;
	m_dwMirColor[234] = 0xff291884;
	m_dwMirColor[235] = 0xff180084;
	m_dwMirColor[236] = 0xff4a4252;
	m_dwMirColor[237] = 0xff52427b;
	m_dwMirColor[238] = 0xff635a73;
	m_dwMirColor[239] = 0xffceb5f7;
	m_dwMirColor[240] = 0xff8c7b9c;
	m_dwMirColor[241] = 0xff7722cc;
	m_dwMirColor[242] = 0xffddaaff;
	m_dwMirColor[243] = 0xfff0b42a;
	m_dwMirColor[244] = 0xffdf009f;
	m_dwMirColor[245] = 0xffe317b3;
	m_dwMirColor[246] = 0xfffffbf0;
	m_dwMirColor[247] = 0xffa0a0a4;
	m_dwMirColor[248] = 0xff808080;
	m_dwMirColor[249] = 0xffff0000;
	m_dwMirColor[250] = 0xff00ff00;
	m_dwMirColor[251] = 0xffffff00;
	m_dwMirColor[252] = 0xff0000ff;
	m_dwMirColor[253] = 0xffff00ff;
	m_dwMirColor[254] = 0xff00ffff;
	m_dwMirColor[255] = 0xffffffff;	
}

void CGameData::IniSkillData()
{
	m_mapSkill[MAGICID_BLOOD_SHADE] = "血影刀法";
	m_mapSkill[MAGICID_REMAIN_SHADE] = "残影刀法";
	m_mapSkill[MAGICID_ATTACK_KILL] = "攻杀剑法";
	m_mapSkill[MAGICID_ATTACK_STICK] = "刺杀剑术";
	m_mapSkill[MAGICID_THUNDER_FIRE] = "雷霆剑法";
	m_mapSkill[MAGICID_ATTACK_FIRE] = "烈火剑法";
	m_mapSkill[MAGICID_WHOLE_MOON] = "抱月刀";
	m_mapSkill[MAGICID_ATTACK_MOON] = "半月弯刀";

	m_mapSkill[MAGICID_PROTECT_SYMBOL] = "灵魂道符";
	m_mapSkill[MAGICID_FIRE_JUJU] = "幽冥火咒";
	m_mapSkill[MAGICID_POISON_MAGIC] = "施毒术";
	m_mapSkill[MAGICID_CURSE] = "诅咒术";

	m_mapSkill[MAGICID_FIREBALL] = "小火球";
	m_mapSkill[MAGICID_HELL_FIRE] = "地狱火焰";
	m_mapSkill[MAGICID_THUNDER] = "雷电术";
	m_mapSkill[MAGICID_ATTACK_STICK] = "刺杀剑术";
	m_mapSkill[MAGICID_ICE_ARROW] = "冰箭术";
	m_mapSkill[MAGICID_ROTATE_FIRE] = "风火轮";
	m_mapSkill[MAGICID_ADV_FIREBALL] = "火炎刀";
	m_mapSkill[MAGICID_BLOW_FIRE] = "爆裂火焰";
	m_mapSkill[MAGICID_FIRE_WALL] = "火墙";
	m_mapSkill[MAGICID_ICE_DRAG] = "冰龙破";
	m_mapSkill[MAGICID_TRACE_THUNDER] = "疾光电影";
	m_mapSkill[MAGICID_ICE_ARRAY] = "玄冰刃";
	m_mapSkill[MAGICID_HELL_THUNDER] = "地狱雷光";
	m_mapSkill[MAGICID_ICE_STORM] = "冰咆哮";
	m_mapSkill[MAGICID_MULTI_THUDER] = "五雷轰";
	m_mapSkill[MAGICID_ICE_WHIRLWIND] = "冰旋风";
	m_mapSkill[MAGICID_DRAGON_LIGHT] = "狂龙紫电";
	m_mapSkill[MAGICID_FIRE_RAIN] = "流星火雨";

	m_mapSkill[MAGICID_SUPER_FIREBALL] = "强化火球术";
	m_mapSkill[MAGICID_SUPER_ATTACK_KILL] = "强化攻杀剑法";
	m_mapSkill[MAGICID_SUPER_POISON_MAGIC] = "强化施毒术";
	m_mapSkill[MAGICID_SUPER_CURSE] = "强化诅咒术";

	m_mapSkill[MAGICID_PROTECT_SKIN] = "护身真气";
	m_mapSkill[MAGICID_HEAL] = "治疗术";

	m_mapSkill[MAGICID_WILD_COLLIDE] = "野蛮冲撞";
	m_mapSkill[MAGICID_MOVE_QUICK] = "瞬间移动";
	m_mapSkill[MAGICID_MAGIC_PROTECT] = "魔法盾";
	m_mapSkill[MAGICID_PROTECT_GHOST] = "幽灵盾";
	m_mapSkill[MAGICID_PROTECT_ARMOR] = "神圣战甲术";

	m_mapSkill[MAGICID_AOFENGCUT_ATTACK] = "翱风斩";
	m_mapSkill[MAGICID_ZHONGLEI_ATTACK] = "纵雷诀";
	m_mapSkill[MAGICID_DULINGBO_ATTACK] = "毒凌波";

	m_mapSkill[MAGICID_TRANSFER_HURT] = "斗转星移";
	m_mapSkill[MAGICID_ANGRYWHIRLWIND] = "狂怒旋风";
	m_mapSkill[MAGICID_COLDSTROM] = "寒冰风暴";
	m_mapSkill[MAGICID_QUNMOTUNSHI] = "心灵爆破";
	m_mapSkill[MAGICID_SKULLCAGE] = "骷髅囚笼";
	m_mapSkill[MAGICID_HEROROAR] = "英勇咆哮";
	m_mapSkill[MAGICID_SOULATTACH] = "电灵附体";
}


CGood* CGameData::SearchGoodByID(DWORD id,int &iFromWnd)
{
	CGood* pGood = NULL;
	if(pGood = SELF.EquipGood().FindGood(id))
	{
		iFromWnd = Arm_Wnd;
		return pGood;
	}

	if(pGood = SELF.PackageGood().FindGood(id))
	{
		iFromWnd = Package_Wnd;
		return pGood;
	}

	if(pGood = g_NPC.FindNpcGood(id))
	{
		return pGood;
	}

	if(pGood = SELF.PetGood().FindGood(id))
	{
		iFromWnd = Pet_Package_Wnd;
		return pGood;
	}

	if(pGood = SELF.GetUsingTemp().FindGood(id))
	{
		return pGood;
	}

	if(SELF.GetEquipTemp().GetID() == id)
	{
		pGood = &(SELF.GetEquipTemp());
		return pGood;
	}

	if(pGood = SELF.GetDropTemp().FindGood(id))
	{
		return pGood;
	}

	if(id == CGoodGrid::GetDropGoodFrom().DropGood.GetID())
	{
		return &(CGoodGrid::GetDropGoodFrom().DropGood);
	}

	if(id == g_NPC.GetDrillResult().m_equip.GetID())
	{
		return &(g_NPC.GetDrillResult().m_equip);
	}
	else if(id == g_NPC.GetDrillResult().m_Fushi.GetID())
	{
		return &(g_NPC.GetDrillResult().m_Fushi);
	}
	else if(id == g_NPC.GetDrillResult().m_Luck.GetID())
	{
		return &(g_NPC.GetDrillResult().m_Luck);
	}
	
	for(int i = 0;i < 5;i++)
	{
		if(id == g_NPC.GetDrillResult().m_FuShiToCombine[i].GetID())
		{
			return &(g_NPC.GetDrillResult().m_FuShiToCombine[i]);
		}
	}

	if (id == g_NPC.GetEquipSoulInfo().m_Equip.GetID())
		return &(g_NPC.GetEquipSoulInfo().m_Equip);

	if (id == g_NPC.GetEquipSoulInfo().m_Hunshi.GetID())
		return &(g_NPC.GetEquipSoulInfo().m_Hunshi);

	for(int i = 0;i < 5;++i)
	{
		if(id == g_NPC.GetEquipSoulInfo().m_Jiejing[i].GetID())
			return &(g_NPC.GetEquipSoulInfo().m_Jiejing[i]);
	}

	if (id == g_NPC.GetWuSeGood().GetID())
		return &(g_NPC.GetWuSeGood());

	if (id == g_NPC.GetBlessGoodLeft().GetID())
		return &(g_NPC.GetBlessGoodLeft());

	if (id == g_NPC.GetBlessGoodRight().GetID())
		return &(g_NPC.GetBlessGoodRight());

	if (id == g_NPC.GetBlessGoodFu().GetID())
		return &(g_NPC.GetBlessGoodFu());

	if (id == g_NPC.GetECGoodLeft().GetID())
		return &(g_NPC.GetECGoodLeft());

	if (id == g_NPC.GetECGoodRight().GetID())
		return &(g_NPC.GetECGoodRight());

	for(int i = 0;i < MAX_FABAO_GOOD_COUNT;++i)
	{
		if(id == g_NPC.GetFaBaoGood(i).GetID())
			return &(g_NPC.GetFaBaoGood(i));
	}

	for (int j = 0; j < 6; ++j)
	{
		for (int i = 0; i < 6; ++i)
		{
			if(id == g_NPC.GetFireArtGood(i,j).GetID())
				return &(g_NPC.GetFireArtGood(i,j));			
		}
	}

	if (id == g_NPC.GetChaifenGood().GetID())
		return &(g_NPC.GetChaifenGood());

	if (id == g_NPC.GetGoodHorsemanship().GetID())
		return &(g_NPC.GetGoodHorsemanship());


	return NULL;
}

CGood* CGameData::SearchOtherGoodByID(DWORD id)
{
	CGood* pGood = NULL;
	if(id == 0)
		return pGood;

	int j = 0;
	for(j = 0; j <  MAX_EQUIPMENT ; j++)   
	{
		pGood = &g_OtherData.OtherPlayer().GetEquipGood(j);
		if(pGood->GetID() == id)
			return pGood;
	}

	//查看摆摊
	for(j = 0; j < MAX_BOOTH_GOODS_NUM ; j++)
	{
		pGood = &g_OtherBoothData.GetGoods(j);
		if(pGood->GetID() == id)
			return pGood;
	}

	//查看交易
	for(j = 0; j < MAX_TRADE_OBJECTS ; j++)
	{
		pGood = &g_TradeData.GetAnotherGood(j);
		if(pGood->GetID() == id)
			return pGood;
	}

	//察看宠物包裹
	for(j = 0; j < MAX_BOOTH_GOODS_NUM ; j++)
	{
		pGood = &g_PetBoothData.GetGoods(j);
		if(pGood->GetID() == id)
			return pGood;
	}
	return NULL;
}

bool CGameData::TakeoutGoodByID(DWORD id)
{
	if(SELF.PetGood().DeleteGood(id))
	{
		DelExternString(id);
		return true;
	}

	int iFromWnd = NO_Wnd;
	CGood* pGood = SearchGoodByID(id,iFromWnd);
    if(!pGood && id == g_NPC.GetPetMergeData().SecondPet.GetID())
    {
        pGood = &(g_NPC.GetPetMergeData().SecondPet);
        map<DWORD,CGoodPetInfo>& mapGoodPetInfo = g_PetData.GetGoodPetInfoMap();
        mapGoodPetInfo.erase(id);
    }
    else if(!pGood && id == g_NPC.GetPetMergeData().Seal.GetID())
    {
        pGood = &(g_NPC.GetPetMergeData().Seal);
    }
	else if(!pGood && id == g_NPC.GetYuanShiResult().m_Good.GetID())
	{
		g_NPC.GetYuanShiResult().m_Good.SetID(0);
	}
	else g_NPC.DelOperateGood(id);

	if(pGood)
	{
		if (iFromWnd == Package_Wnd)
		{
			g_AutoEatMgr.UseSuccess(*pGood);
		}

		pGood->SetID(0);
		DelExternString(id);
		return true;
	}

	return false;
}

BOOL CGameData::FindTroopMember(char* name)
{
	for(ListString::iterator it = m_vecTroopMember.begin(); it!= m_vecTroopMember.end(); ++it)
	{
		if(strcmp(it->c_str(),name) == 0)
			return TRUE;
	}
	return FALSE;
}

const char * CGameData::GetMemberName(int i)
{
	int j = 0;
	for(ListString::iterator it = m_vecTroopMember.begin(); it!= m_vecTroopMember.end(); ++it,j++)
	{
		if(i == j)
			return it->c_str();
	}
	return "";
}

//扩展字符串
string CGameData::GetExternString(DWORD id)
{
	IDStringMap::iterator itr = m_mapExternString.find(id);
	if(itr != m_mapExternString.end())
		return itr->second;
	return "";
}

void CGameData::InsertExternString(DWORD id,const string& str)
{
	DelExternString(id);
	m_mapExternString[id] = str;
}

void CGameData::DelExternString(DWORD id)
{
	map<DWORD,string>::iterator itr;
	if((itr = m_mapExternString.find(id)) !=m_mapExternString.end())
		m_mapExternString.erase(itr);
}

bool Init_GameData()
{
	return true;
}

bool Release_GameData()
{
	return true;
}

void  CGameData::SetDropMoneyFrom(int lMoney)
{ 
	m_iDropMoneyFrom = lMoney;
	if(m_iDropMoneyFrom != EDMYF_NONE)
	{
		g_pAudio->Play(EAT_OTHER,13,g_pAudio->GetRand()++);
	}
}
void  CGameData::SetDropYuanBaoFrom(int lYuanBao)
{ 
	m_iDropYuanBaoFrom = lYuanBao; 
	if(m_iDropYuanBaoFrom != EDMYF_NONE)
	{
		g_pAudio->Play(EAT_OTHER,13,g_pAudio->GetRand()++);
	}
}

void CGameData::SetMouseType(int iType)
{
	if(iType == m_iMouseType)
		return;

	m_iMouseType = iType;
	if(m_iMouseType == MOUSE_STANDARD)
	{
		m_dwMouseTexID = MAKELONG(376,PACKAGE_INTERFACE);
		m_mouseRM = RM_ALPHA;
		bool bEableSelfCursor = (g_Config.GetMouse() != 0?true:false);
		EnableSelfCursor(bEableSelfCursor);
	}
	else
	{
		m_mouseRM = RM_ALPHA;
		switch(m_iMouseType)
		{
		case MOUSE_REPAIR:
			m_dwMouseTexID = MAKELONG(14535,PACKAGE_INTERFACE);
			break;
		case MOUSE_SPECIALREPAIR:
			m_dwMouseTexID = MAKELONG(14536,PACKAGE_INTERFACE);
			break;
		case MOUSE_SELFORCEATTOBJ:
			m_dwMouseTexID = MAKELONG(21583 ,PACKAGE_INTERFACE);
			break;
		case MOUSE_SELGARRSIONXY:
			m_dwMouseTexID = MAKELONG(21584 ,PACKAGE_INTERFACE);
			break;
		case MOUSE_HULU:
			m_dwMouseTexID = MAKELONG(22990,PACKAGE_INTERFACE);			
			break;
		case MOUSE_BUBING:
			m_dwMouseTexID = MAKELONG(22991,PACKAGE_INTERFACE);
			break;		
		default:
			break;

		}

		EnableSelfCursor(true);
	}
}

void CGameData::EnableSelfCursor(bool b)
{
	if( m_bSelfCursor == b ) 
		return;

	::ShowCursor(!b);
	m_bHardCursor = !b;
	m_bSelfCursor = b;
}

void CGameData::ChangeMapClear(const char *strNewMapName)
{
	g_OtherData.GetCommonConfirmVector().clear();
	g_pControl->Msg(MSG_CTRL_PERSONS_GUIDE_WND,OPER_CLOSE);//人物指南窗口
	g_pControl->Msg(MSG_CTRL_COMMON_CONFIRM_WND,OPER_CLOSE);//请求拜师、交易等确认窗口
	g_pControl->Msg(MSG_CTRL_VIPSTORE_WND,OPER_CLOSE);//VIP秘市窗口
	g_pControl->Msg(MSG_CTRL_EXTRACTUPGRADE_YUANSHI,OPER_CLOSE);
	g_pControl->Msg(MSG_CTRL_SPLITSTONE_WND,OPER_CLOSE);
	g_pControl->Msg(MSG_CTRL_TASKWUSEDAILY_WND,OPER_CLOSE);
	g_pControl->Msg(MSG_CTRL_EQUIPSOUL_WND,OPER_CLOSE);
	g_OtherData.GetTXZG().bStarted = false;
	g_OtherData.GetTSJB().bStarted = false;

	g_pControl->PopupWindow(MSG_CTRL_LEASEOTHERBOOTH_WND,OPER_CLOSE);
	g_pControl->PopupWindow(MSG_CTRL_LEASEBOOTH_WND,OPER_CLOSE);
	g_pControl->PopupWindow(MSG_CTRL_FIRM_WND,OPER_CLOSE);

	if ( strcmp(strNewMapName, "sjs2n11") == 0
		|| strcmp(strNewMapName, "sjs2n12") == 0
		|| strcmp(strNewMapName, "sjs2n13") == 0
		|| strcmp(strNewMapName, "sjs2n14") == 0
		)
	{
		//在收到MSG_OpenBingCtrlWnd时打开
		//g_pControl->PopupWindow(MSG_CTRL_DEFEND12CTRL_WND,OPER_CREATE);
	}
	else
	{
		g_pControl->PopupWindow(MSG_CTRL_DEFEND12CTRL_WND,OPER_CLOSE);
		g_pControl->Msg(MSG_CTRL_DEFEND12GONGBTN,0);
	}

}

void CGameData::SetSkillShortCut(int iMagicID,int iConSkillID,int iShortCut,int iShortCutEx)
{
	m_SkillShortCut.iMagicID = iMagicID;
	m_SkillShortCut.iConSkillID = iConSkillID;
	m_SkillShortCut.iShortCut = iShortCut;
	m_SkillShortCut.iShortCutEx = iShortCutEx;

	if (iConSkillID > 0)
	{
		m_SkillShortCut.pConSkill = g_AIMgr.FindConSkillByID(iConSkillID);	
	}
	else
	{
		m_SkillShortCut.pConSkill = 0;
	}
}
