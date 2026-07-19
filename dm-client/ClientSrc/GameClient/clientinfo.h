
#if !defined(AFX_AAAAAVIEW_H__6D97851C_36E3_414B_B63A_BFE5EA3B1440__INCLUDED_)
#define AFX_AAAAAVIEW_H__6D97851C_36E3_414B_B63A_BFE5EA3B1440__INCLUDED_

#ifdef WIN32
#ifndef UINT
	typedef unsigned int UINT;
#endif
#endif //WIN32

#pragma pack(1)

typedef struct InGameInfo
{
	struct GameVersion
	{
		UINT GameId;
		UINT GameArea;
		UINT GameGroup;
		UINT ClientType;
		char Version[64];
		char VersionType[64];
	};

	struct Player
	{
		UINT Level;
		char ID[64];
		char RoleName[64];
		char Race[32];
		char Gender[4];
	};

	struct SocialRelations
	{
		char Friends[64];
		char Guild[64];
		char Team[64];
		char Teacher[64];
		char Marriage[64];
	};

	struct Action
	{
		struct PlayerAction
		{
			UINT Trade;
			UINT Running;
			UINT Mission;
			UINT Copy;
			UINT AttendActivity;
			UINT Duration;
		};
		struct MapActivity
		{
			char Name[64];
			UINT Duration;
		};
		PlayerAction PlayerActionInfo;
		MapActivity MapActivityInfo;
	};

	struct GameSetting
	{
		struct ClientStartup
		{
			UINT ScreenMode;
			UINT Double;
			char ExternalSoftware[64];
			char Resolution[16];
			char InstallPath[128];
			char RegistryIntegrity[128];
		};
		struct EffectSetting
		{
			char InterpolationAlg[32];
			char Weather[32];
			char Shadow[32];
		};
		ClientStartup ClientStartupInfo;
		EffectSetting EffectSettingInfo;
	};

	struct Quality
	{
		UINT NetworkDelay;
		UINT ChangeMapDelay;
		UINT StartUp;
		UINT RenderingFrameRate;
		char UnexceptBreak[128];
		char Exception[128];
	};
	GameVersion GameVersionInfo;
	Player PlayerInfo;
	SocialRelations SocialRelationsInfo;
	Action ActionInfo;
	GameSetting GameSettingInfo;
	Quality QualityInfo;
} INGAMESTRUCT;
#pragma pack()
#endif // !defined(AFX_AAAAAVIEW_H__6D97851C_36E3_414B_B63A_BFE5EA3B1440__INCLUDED_)