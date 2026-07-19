#pragma once

#include "Global/Global.h"
#include "GameProtocol.h"
#include "GameData/ActionStruct.h"
#include "GameData\Good.h"
#include "BaseClass/Misc/Net.h"
#include "GameData/MapFinder.h"

#define GAME_VERSION   201204024

#define MAX_MESSAGE_BUFFER_SIZE				8196  // 消息队列大小
#define MAX_MESSAGE_BUFFER					1024 * 128
#define NET_BUF_SIZE                        65536
#define MAX_SEND_MESSAGE_SIZE               1024 * 64

#define CMD_SIZE       12

//传世方法, 制定cmd
#define SET_COMMAND(cmd,len) \
	int   iBLen = CMD_SIZE; \
	int   iCmd = cmd;\
    char *temp = m_cBuf;\
	memset(temp,0,len);\
	*((WORD *)(temp + 4)) = cmd

#define ASSIGN_ID(v)        *((DWORD*)temp) = v
#define ASSIGN_WORD(pos,v)  *((WORD*)(temp + pos)) = v
#define ASSIGN_DWORD(pos,v) *((DWORD*)(temp + pos)) = v
#define ASSIGN_INT(pos,v) *((int*)(temp + pos)) = v
#define ASSIGN_INT64(pos,v) *((__int64*)(temp + pos)) = v
#define ASSIGN_BYTE(pos,v)  *((BYTE*)(temp + pos)) = v

#define ADD_STR(s)   strcpy(temp+iBLen,s); iBLen += strlen(s)//用ADD_STR必须保证s中没有'\0',否则后面的内容无法复制
#define ADD_WORD(v)  *((WORD*)(temp+iBLen)) = v; iBLen += sizeof(WORD)
#define ADD_DWORD(v) *((DWORD*)(temp+iBLen)) = v; iBLen += sizeof(DWORD)
#define ADD_BYTE(v)  *((BYTE*)(temp+iBLen)) = v; iBLen += sizeof(BYTE)
#define ADD_ARRAY(v,len) memcpy(temp+iBLen,v,len); iBLen += len

#define SEND_GAME_SERVER()    g_pNet->SendBuf(SERVER_GAME,temp,iBLen, iCmd, true)

class CStringLine;

class CGameControl
{
	typedef void (CGameControl::*lpFNMSGProc)(const char *,int);

protected:
	_TDEFAULTMESSAGE	DefMsg;

public:
	CGameControl(void);
	~CGameControl(void);

	//公共的系统内部函数
	void GCL_ClearMsgBuf();
	bool GCL_AddMsg(const char * pBuf,int iLen);
	void GCL_AddErr();

	void GCL_Run();
	void GCL_DoMsg(const char * strMsg,int iLen);
public:
	bool DealGotoNpcCommand(const std::string & strCommand);//处理自动寻路指定NPC,参数为命令字符串	//<老兵/@@goto mapName npcName 20,20>\\";//strCommand = @@goto mapName npcName 20,20
	bool DealGoPlaceCommand(const std::string & strCommand);//处理自动寻路指定地点,参数为命令字符串	//<到××处去杀××/@@goplace mapName 20,20>\\";//strCommand = @@goplace mapName 20,20
	bool DealGotoCommand(JumpPoint* pJump);
	bool DealGotoCommand(const std::string & strCommand);
	void DoOverMapFindPath();//处理跨地图自动寻路


	//bool DealGotoNPcCommand2(const std::string & strCommand);
	//bool DealGoPlaceCommand2(const std::string & strCommand);
	bool DealGotoNpc(const char* pszMapName,WORD wX,WORD wY);
	bool DealGotoPlace(const char* pszMapName,WORD wX,WORD wY);
	void SetIGWInfo();

	void SEND_TestProto(WORD id);

	//安全登陆
	//void  SEND_Login_Req_Public_Key(); //向登陆服务器请求公钥
	//void  SEND_Login_Key_UserPwd(bool bPT,const char * strUser,const char *strPwd);
	//void  MSG_Login_Public_Key(const char* msg,int iLen);

	//连接登陆服务器
	bool SEND_Login_LoginInServer();
	//void SEND_Uuid_Login(const char * sUuID,const char *sPassword);
	//void SEND_Login_UserPwd(const char * strUser,const char * strPwd);
	void SEND_Login_Tongji(int iSubType, const string& strFrom);
	void SEND_Login_Tongji2(bool bLogoutTime);

	//void MSG_Login_UserPwdOK(const char * msg,int iLen);
	void MSG_Login_TuiGuang(const char * msg,int iLen);
	//void MSG_Login_CharServer(const char * msg,int iLen);
	//void MSG_Login_UserPwdFail(const char * msg,int iLen);

	void MSG_Password_Change_Ok			(const char * msg,int iLen);
	void MSG_Password_Change_Fail		(const char * msg,int iLen);
	void MSG_Create_Character_Fail		(const char * msg,int iLen);
	//void MSG_Character_Info				(const char * msg,int iLen);
	void MSG_User_Full					(const char * msg,int iLen);
	void MSG_Objects_Position			(const char * msg,int iLen);
	void MSG_Session_ID					(const char * msg,int iLen);
	void MSG_Username_Info 				(const char * msg,int iLen);
	void MSG_GS_Jump_Begin				(const char * msg,int iLen);

	// UUID
	void MSG_Need_Upto_Uuid				(const char * msg,int iLen);
	void MSG_Server_Temp_Uuid			(const char * msg,int iLen);
	void MSG_Change_Uuid_Result			(const char * msg,int iLen);

	void MSG_Dynamic_Password_Result	(const char * msg,int iLen);
	void MSG_Undynamic_Password_Result	(const char * msg,int iLen);
	void MSG_Assert_Bind				(const char * msg,int iLen);
	void MSG_Input_Reserve_Password		(const char * msg,int iLen);
	void MSG_Card_Result				(const char * msg,int iLen);
	void MSG_Normal_MessageBox			(const char * msg,int iLen);
	void MSG_Object_Detail				(const char * msg,int iLen);
	void MSG_Rungate_Encrypt			(const char * msg,int iLen);
	//void MSG_Level_Check				(const char * msg,int iLen);
	void MSG_ADD_Internet_Msg			(const char * msg,int iLen);
	void MSG_Internet_Error				(int nNum,DWORD dwParam = 0);

	bool SEND_Login_ServerName			();
	//void SEND_Login_Register_Info       (const char* szName,const char* szIdentityCard,int iAdult);
	void SEND_FCMFillInfo_Req       (const char* szName,const char* szIdentityCard,int iAdult);

    //////////////////////////////////////////////////////////////////////////
    // 新流程相关协议
    
    // 组列表通知
    void MSG_GroupList_Ntf              (const char * msg,int iLen);

    // 下载服务器ip通知
    void MSG_DlServerIP_Ntf              (const char * msg,int iLen);

    // 选组请求-响应
    void SEND_SelGroup_Req              (const char * groupName);
    void MSG_SelGroup_Ack               (const char * msg,int iLen);

    // 获取动态验证码请求
    void SEND_DynCode_Req               ();

    // 登陆请求-响应
    void SEND_PTLogin_Req               (const char * name, const char * passWd);
    void SEND_OALogin_Req               (const char * token,const char * loginid);
	void SEND_APLogin_Req               (const char* sendtype, const char * auth);
	void SEND_EXPLogin_Req              ();
	void SEND_SGILogin_Req              (const char * code);
    void MSG_Login_Ack                  (const char * msg,int iLen);
	void MSG_Login_Error_Ack			(const char * msg,int iLen);
	void MSG_Login_Succ				(const char * msg,int iLen);

	void SEND_MergePT_Req				(const char * account, const char * passWd);
	void MSG_MergePT_Ack                  (const char * msg,int iLen);

    // 验证请求-响应
    void MSG_Challenge_Req              (const char * msg,int iLen);
    void SEND_Challenge_Ack             (const char * challenge);
	void SEND_VerifyStoreEkey_Ack       (const char * challenge);
    
    // 创建角色请求-响应
    void SEND_CreateRole_Req            (const char* name, BYTE byJob,BYTE bySex, BYTE byHair);
    void MSG_CreateRole_Ack             (const char * msg, int iLen);

    // 选择角色请求-响应
    void SEND_SelRole_Req               (const char* ptID, const char* roleName);
    void MSG_SelRole_Ack                (const char * msg, int iLen);

    // 删除角色请求-响应
    void SEND_DelRole_Req               (const char* ptID, const char* roleName);
    void MSG_DelRole_Ack                (const char * msg, int iLen);
  
	// 恢复角色请求-响应
    void SEND_UnDelRole_Req             (const char* ptID, const char* roleName);
    void MSG_UnDelRole_Ack              (const char * msg, int iLen);

    // 图形验证请求-响应
    void MSG_Verify_Req                 (const char * msg, int iLen);
    void SEND_Verify_Ack                (const char * answer);

    // 小退请求-响应
    void SEND_ReselRole_Req             (const char* ptID, const char* roleName);
    void MSG_ReselRole_Ack              (const char * msg,int iLen);

    // 下线通知
    void SEND_Logout_Ntf                (const char* ptID);

	//KickOff
	void MSG_KickOff_NTF(const char * msg,int iLen);

	//发给oa的区组信息,收到这条协议就表示gs支持oa登录
	void MSG_GroupInfo_NTF(const char * msg,int iLen);

	//动态加解密
	void MSG_DynCode_NTF(const char * msg,int iLen);

    // 游戏中图形验证请求-响应
	void MSG_InGameVerify_Req(const char * msg,int iLen);
	void SEND_InGameVerify_Ack(const char * answer);
	void SEND_FreshVerify_Ack();


	//请求离线托管
	void SEND_OfflineTrustee(BYTE byType);




    //////////////////////////////////////////////////////////////////////////

	// SelectChar 消息
	//void MSG_Selchar_CharInfo			(const char * msg,int iLen);
	//void MSG_Restore_Char_Error			(const char * msg,int iLen);
	//void MSG_Selchar_RunServer			(const char * msg,int iLen);
	//void MSG_Deleted_Char_Info			(const char * msg,int iLen);
	void MSG_Dynamic_Password_Change	(const char * msg,int iLen);
	//void MSG_Restore_Char_Ok			(const char * msg,int iLen);
	void MSG_Kick_Out					(const char * msg,int iLen);
	void MSG_Rungate_Answer				(const char * msg,int iLen);

	//bool SEND_Selchar_CharServer	();
// 	bool SEND_Selchar_SelectChar		();
	//void SEND_Restore_Char				(int iRecoverChar = 0);
	void SEND_Level_Check 				(const char * strName);

// 	void SEND_Deleted_Char_Info			();
	//bool SEND_Game_Connect				();
	bool SEND_Game_EnterGame			();
	void SEND_Message_Confirm			();
	void SEND_Upto_Uuid					();// UUID
	void SEND_Change_Uuid				(const char *str);
	void SEND_Test_Rungate_Answer();
	void SEND_Next_Dynamic_Password		(const char * str);

	void MSG_NEED_DYN_CRYPT(const char * msg, int iLen);//动态加解密算法
	void SEND_DYN_CRYPT_OK(DWORD n);
	void SEND_ReRequest_DynCode();

	// 怪物，玩家出现消息
	void MSG_Player_Connected(const char * strMsg,int iLen);
	void MSG_Play_Attacked_Sound        (const char * msg, int iLen);
	void MSG_Player_Position			(const char * msg,int iLen);
	void MSG_Player_Refresh				(const char * msg,int iLen);
	void MSG_Player_Attribute			(const char * msg,int iLen);
	void MSG_Player_Info2				(const char * msg,int iLen);
	void MSG_Player_Info1				(const char * msg,int iLen);
	void MSG_Player_Arm_Info			(const char * msg,int iLen);
	void MSG_Player_Success				(const char * msg,int iLen);
	void MSG_Player_Fail				(const char * msg,int iLen);
	void MSG_Player_Money				(const char * msg,int iLen);
	void MSG_Player_Life				(const char * msg,int iLen);
	void MSG_Player_Experience_Up		(const char * msg,int iLen);
	void MSG_Player_Magic_Color			(const char * msg,int iLen);
	void MSG_Player_Rank_Up				(const char * msg,int iLen);	
	void MSG_Player_Arm_Refresh			(const char * msg,int iLen);
	void MSG_Player_Arm_Fail			(const char * msg,int iLen);
	void MSG_Player_Unarm_Refresh		(const char * msg,int iLen);
	void MSG_Magic_Ability				(const char * msg,int iLen);
	void MSG_Player_Unarm_Fail			(const char * msg,int iLen);
	void MSG_Player_Skill_Delete		(const char * msg,int iLen);
	void MSG_Player_Skill_Add			(const char * msg,int iLen);
	void MSG_Train_Success				(const char * msg,int iLen);


	void MSG_Message_Private			(const char * msg,int iLen);
	void MSG_Monster_Corpus				(const char * msg,int iLen);
	void MSG_Monster_Bone				(const char * msg,int iLen);
	void MSG_Monster_Disappearance		(const char * msg,int iLen);
	void MSG_Monster_Hurted				(const char * msg,int iLen);
	void MSG_Monster_Hurted2			(const char * msg,int iLen);
	void MSG_Monster_ExtraState         (const char * msg, int iLen);
	void MSG_Monster_Appearance			(const char * msg,int iLen);
	void MSG_Monster_Walk				(const char * msg,int iLen);
	void MSG_Monster_Run				(const char * msg,int iLen);
	void MSG_Monster_Name				(const char * msg,int iLen);
	void MSG_Map_Name					(const char * msg,int iLen);
	void MSG_Monster_NameColor			(const char * msg,int iLen);
	void MSG_Monster_State				(const char * msg,int iLen);
	void MSG_Monster_Dead				(const char * msg,int iLen);
	void MSG_Monster_Attack1			(const char * msg,int iLen);
	void MSG_Monster_Attack2			(const char * msg,int iLen);
	void MSG_Monster_Attack3			(const char * msg,int iLen);
	void MSG_Monster_Attack_Kill		(const char * msg,int iLen);
	void MSG_Monster_Attack_Stick		(const char * msg,int iLen);
	void MSG_Monster_Attack_Moon		(const char * msg,int iLen);
	void MSG_Monster_Attack_Fire		(const char * msg,int iLen);
	void MSG_Monster_Attack_Sword		(const char * msg,int iLen);
	void MSG_Monster_Cut				(const char * msg,int iLen);
	void MSG_Monster_Back				(const char * msg,int iLen);
	void MSG_Monster_Back1				(const char * msg,int iLen);
	void MSG_Monster_Back2				(const char * msg,int iLen);
	void MSG_Monster_Animate_Appear		(const char * msg,int iLen);
	void MSG_Monster_Animate_Disappear	(const char * msg,int iLen);
	void MSG_Corpse_Relive				(const char * msg,int iLen);
	void MSG_Monster_Magic_Fly			(const char * msg,int iLen);
	void MSG_Monster_Protector_Color	(const char * msg,int iLen);
	void MSG_Monster_SoulWall_State		(const char * msg,int iLen);
	void MSG_Group_Enable				(const char * msg,int iLen);
	void MSG_Monster_Attack_Magic		(const char * msg,int iLen);
	void MSG_Monster_Refresh			(const char * msg,int iLen);
	void MSG_Monster_StopGo				(const char * msg,int iLen);
	void MSG_Monster_MovePosition		(const char * msg,int iLen);
	void MSG_MonsterAttack_Effect       (const char * msg,int iLen);

	void SEND_Messagebox_Reply(DWORD dwNpcID,const char * info = NULL,bool bForbidDirtyWord = false); //消息框的返回消息
	bool SEND_Use_Object(int iPos,DWORD dwLinkID = 0,DWORD dwData2 = 0);
	bool SEND_AddObjToObj(CGood &srcGood,CGood &destGood,bool bAddSrcToUseTemp = true);                   //加一个物品加到另一个物品上
	void SEND_ExitMazeMap();											//离开迷仙阵
	void SEND_PetUse_Object			    (CGood& good);

	//pt商城,发送数据
	void SEND_PT_Store_Info(int iProtocol, const char * szAppendStr = NULL, int iAppendLen = 0,BOOL bSilent = FALSE,BOOL bInputPetPkg = FALSE);
	void MSG_PT_Store_Info(const char * msg,int iLen);

	//物品包裹
	void MSG_Package_All_Info			(const char * msg,int iLen);
	void MSG_Package_Add_Object			(const char * msg,int iLen);
	void MSG_Package_Reject_Fail		(const char * msg,int iLen);
	void MSG_Package_Reject_Confirm		(const char * msg,int iLen);
	void MSG_Package_Capacity           (const char * msg,int iLen);
	void MSG_Package_Update_Object		(const char * msg,int iLen);
	void MSG_Object_Dura				(const char * msg,int iLen);
	void MSG_Object_Use_Success			(const char * msg,int iLen);
	void MSG_Object_Use_Failed			(const char * msg,int iLen);
	void MSG_Object_Appearance			(const char * msg,int iLen);
	void MSG_Object_Disappearance		(const char * msg,int iLen);
	void MSG_Object_Extern_Info			(const char * msg,int iLen);

	void MSG_PetGoodInfo                (const char * strMsg2,int iLen);//兽魔石附近属性信息

	void MSG_Make_Poison_Fail			(const char * msg,int iLen);
	void MSG_Make_Poison_Remain_Money	(const char * msg,int iLen);

	void MSG_Jump_Refresh				(const char * msg,int iLen);
	void MSG_Magic_Effectived			(const char * msg,int iLen);
	void MSG_Magic_NoEffectived			(const char * msg,int iLen);
	void MSG_Magic_Position_Success		(const char * msg,int iLen);
	void MSG_Magic_Position_Failed		(const char * msg,int iLen);
	void MSG_Magic_Continue_Begin		(const char * msg,int iLen);
	void MSG_Magic_Continue_End			(const char * msg,int iLen);
	void MSG_Magic_Monster1_Attack		(const char * msg,int iLen);
	void MSG_Magic_Monster2_Attack		(const char * msg,int iLen);

	void MSG_Group_Member_Delete_Ok		(const char * msg,int iLen);
	void MSG_Group_Member_Delete_Name	(const char * msg,int iLen);
	void MSG_Group_Member_Delete_Failed	(const char * msg,int iLen);
	void MSG_Group_Member_Pos			(const char * msg,int iLen);
	void MSG_Group_Member_Info			(const char * msg,int iLen);
	void MSG_Group_Company_Failed		(const char * msg,int iLen);
	void MSG_Group_Member_Add_Failed	(const char * msg,int iLen);
	void MSG_Group_Request				(const char * msg,int iLen);

	void MSG_Guild_Message				(const char * msg,int iLen);
	void MSG_Guild_Message_Fail			(const char * msg,int iLen);
	void MSG_Guild_Delete_Member_Failed	(const char * msg,int iLen);
	void MSG_Guild_Addin_Guild_Failed	(const char * msg,int iLen);
	void MSG_Guild_Delete_Member_Success(const char * msg,int iLen);
	void MSG_Guild_Ret_Exp_Val			(const char * msg,int iLen);
	void MSG_Guild_Member_List			(const char * msg,int iLen);
	void MSG_Guild_Member_List_New		(const char * msg,int iLen);
	void MSG_Guild_Phyle_List			(const char * msg,int iLen);
	void MSG_Guild_War_Message			(const char * msg,int iLen);
	void MSG_Guild_Invited				(const char * msg,int iLen);
	void MSG_Exchange_Talk_Menu			(const char * msg,int iLen);
	void MSG_Exchange_Buy_Menu			(const char * msg,int iLen);
	void MSG_Exchange_PT_Buy_Menu       (const char * msg,int iLen);
	void MSG_Exchange_Sale_Fail			(const char * msg,int iLen);
	void MSG_Exchange_Buy_Fail			(const char * msg,int iLen);
	void MSG_Exchange_Sale_Object		(const char * msg,int iLen);
	void MSG_Exchange_List_Menu         (const char * msg,int iLen);
	void MSG_Exchange_Buy_Ok			(const char * msg,int iLen);
	void MSG_Exchange_Sale_Window		(const char * msg,int iLen);
	void MSG_Exchange_Repair_Window		(const char * msg,int iLen);
	void MSG_Exchange_Sale_Ok			(const char * msg,int iLen);
	void MSG_Exchange_Repair_Money		(const char * msg,int iLen);
	void MSG_Exchange_Repair_Success	(const char * msg,int iLen);
	void MSG_Exchange_Repair_Fail		(const char * msg,int iLen);
	void MSG_Exchange_Repair_Duration   (const char * msg,int iLen);
	void MSG_Exchange_Talk_End			(const char * msg,int iLen);
	void MSG_Exchange_Storage_Window	(const char * msg,int iLen);
	void MSG_Exchange_Getback_Window 	(const char * msg,int iLen);
	void MSG_Exchange_Poison_Menu		(const char * msg,int iLen);
	void MSG_Exchange_Poison_Need		(const char * msg,int iLen);
	void MSG_Exchange_Poison_Money		(const char * msg,int iLen);
	void MSG_Exchange_Storage_Failed	(const char * msg,int iLen);
	void MSG_Exchange_Storage_Ok		(const char * msg,int iLen);
	void MSG_Exchange_Getback_Ok		(const char * msg,int iLen);
	void MSG_Exchange_Getback_Fail		(const char * msg,int iLen);
	void MSG_Exchange_Second_Buy_Menu	(const char * msg,int iLen);

	void MSG_Person_State               (const char * msg,int iLen);
	void SEND_Person_Setting            ();

	void Get_Guild_Logo					(DWORD npcid,const char * act);
	void MSG_Bad_Good					(const char * msg,int iLen);
	void MSG_Exchange_Forge_Window		(const char * msg,int iLen);
	void MSG_Exchange_Forge_Percent		(const char * msg,int iLen);
	void MSG_Exchange_Forge_Result		(const char * msg,int iLen);
	void MSG_Exchange_Bless_Result		(const char * msg,int iLen);
	void MSG_Exchange_Forge_Experience	(const char * msg,int iLen);
	void MSG_Exchange_Gem_Result        (const char * msg,int iLen);
	void MSG_Exchange_Box_Result        (const char * msg,int iLen); 
	void MSG_System_Skyrocket			(const char * msg,int iLen);
	void MSG_Other_Messages				(const char * msg,int iLen);
	void MSG_Input_DPWD					(const char * msg,int iLen);
	//void SEND_Input_DPWD				(const char * msg,BYTE byType = 0);
	void MSG_Rungate_DPWD				(const char * msg,int iLen);
	void SEND_Rungate_DPWD				(const char * msg);
	void MSG_Custom_Piaohong			(const char * msg,int iLen);
	//延时输入密宝
	void MSG_Input_MIBAO_LATER   		(const char * msg,int iLen);

	void MSG_Friend_List				(const char * msg,int iLen);
	void MSG_Add_Friend_Success			(const char * msg,int iLen);
	void MSG_Del_Friend_Success			(const char * msg,int iLen);
	void MSG_Friend_Error				(const char * msg,int iLen);
	void MSG_Friend_Notice				(const char * msg,int iLen);
	void MSG_Friend_Request				(const char * msg,int iLen);
	void MSG_Friend_Request_IM				(const char * msg,int iLen);
	void MSG_Friend_MoQi_Change				(const char * msg,int iLen);
	void MSG_General_Reply_Confirm		(const char * msg,int iLen);

	void MSG_Unread_Leave_Word			(const char * msg,int iLen);
	void MSG_Chat_Not_Online			(const char * msg,int iLen);

	void MSG_Map_Jump					(const char * msg,int iLen);
	void MSG_Map_Jump_Success			(const char * msg,int iLen);
	void MSG_Map_Desc						(const char * msg,int iLen);
	void MSG_Map_Change					(const char * msg,int iLen);
	void MSG_Magic_Quick_Move			(const char * msg,int iLen);
	void MSG_Magic_Quick_Move_Position	(const char * msg,int iLen);
	void MSG_Trade_Enable				(const char * msg,int iLen);
	void MSG_Trade_Disable				(const char * msg,int iLen);
	void MSG_Trade_Player_Object_Info	(const char * msg,int iLen);
	void MSG_Trade_Object_Confirm		(const char * msg,int iLen);
	void MSG_Trade_Failed				(const char * msg,int iLen);
	void MSG_Trade_Success				(const char * msg,int iLen);
	void MSG_Trade_Money_Info			(const char * msg,int iLen);
	void MSG_Trade_Player_Money_Info	(const char * msg,int iLen);
	void MSG_Trade_Tackin_Failed		(const char * msg,int iLen);
	void MSG_Trade_Player_Object_Takeout(const char * msg,int iLen);
	void MSG_Guild_Align_Failed			(const char * msg,int iLen);
	void MSG_Guild_Ally_Success  	    (const char * msg,int iLen);
	void MSG_Guild_Align_Cancel_Failed	(const char * msg,int iLen);
	void MSG_Create_Account_Fail		(const char * msg,int iLen);
	void MSG_Create_Account_Success		(const char * msg,int iLen);
	void MSG_Object_Take				(const char * msg,int iLen);
	void MSG_Map_No_Small_Map			(const char * msg,int iLen);
	void MSG_Map_Small_Map				(const char * msg,int iLen);
	void MSG_Magic_Skill_Value			(const char * msg,int iLen);
	void MSG_Guild_Create_Fail			(const char * msg,int iLen);
	void MSG_Guild_Create_Success		(const char * msg,int iLen);
	void MSG_Connect_Error				(const char * msg,int iLen);
	void MSG_Error_Login				(const char * msg,int iLen);
	void MSG_Other_Player_Info			(const char * msg,int iLen);
	void MSG_Guild_Member_Get_Title		(const char * msg,int iLen);
	void MSG_Guild_Member_Title_Edit_Failed(const char * msg,int iLen);

	void MSG_Open_Super_Attack_Kill		(const char * msg,int iLen);
	void MSG_Open_Attack_Kill 			(const char * msg,int iLen);
	void MSG_Open_Attack_Stick 			(const char * msg,int iLen);
	void MSG_Close_Attack_Stick			(const char * msg,int iLen);
	void MSG_Open_Attack_Moon 			(const char * msg,int iLen);
	void MSG_Close_Attack_Moon 			(const char * msg,int iLen);
	void MSG_Open_Attack_Fire 			(const char * msg,int iLen);
	void MSG_Close_Attack_Fire 			(const char * msg,int iLen);
	void MSG_Valid_Dig		 			(const char * msg,int iLen);
	void MSG_Internet_Good				(const char * msg,int iLen);
	void MSG_Internet_Fail				(const char * msg,int iLen);
	void MSG_Weather	 				(const char * msg,int iLen);
	void MSG_Player_RideEnd				(const char * msg,int iLen);
	void MSG_Player_Salute				(const char * msg,int iLen);
	void MSG_Open_Remain_Shade			(const char * msg,int iLen);
	void MSG_Close_Remain_Shade			(const char * msg,int iLen);
	void MSG_Open_Blood_Shade			(const char * msg,int iLen);
	void MSG_Close_Blood_Shade			(const char * msg,int iLen);
	void MSG_Open_Whole_Moon			(const char * msg,int iLen);
	void MSG_Close_Whole_Noon			(const char * msg,int iLen);
	void MSG_Open_Thunder_Fire			(const char * msg,int iLen);
	void MSG_Close_Thunder_Fire			(const char * msg,int iLen);
	void MSG_Open_Destroy_Weapon		(const char * msg,int iLen);
	void MSG_Close_Destroy_Weapon		(const char * msg,int iLen);
	void MSG_Yuanbao_Info				(const char * msg,int iLen);
	void MSG_Bind_Yuanbao_Info          (const char * msg,int iLen);

	void MSG_Open_Destroy_Shield        (const char * strMsg,int iLen);
	void MSG_Close_Destroy_Shield       (const char * strMsg,int iLen);
	void MSG_Open_Destroy_Shell         (const char * strMsg,int iLen); 
	void MSG_Close_Destroy_Shell        (const char *strMsg, int iLen);

	void MSG_Version_NoMatch			(const char * msg,int iLen);
	void MSG_Npc_Msgbox					(const char * msg,int iLen);

	void MSG_Player_Pet_State			(const char * msg,int iLen);

	void MSG_Isvalid_Action				(const char * msg,int iLen);
	void MSG_Player_Sword_Switch		(const char * msg,int iLen);
	void MSG_Player_Use_Sword			(const char * msg,int iLen);
	void MSG_Player_Dig_Break			(const char * msg,int iLen);

	void MSG_Exchange_Tray_Wnd			(const char * msg,int iLen);
	void MSG_Return_Result				(const char * msg,int iLen);
	void MSG_System_Play_Midi			(const char * msg,int iLen);

	// GameServer 消息
	void MSG_Weapon_Break				(const char * msg,int iLen);
	void MSG_Player_Auto_Arm			(const char * msg,int iLen);

	void MSG_StoryBoard_URL             (const char * msg,int iLen);

	void MSG_Special_Monster_Effect     (const char * msg,int iLen);
	void MSG_Fu_Sheng_Time              (const char * msg,int iLen);

	//摆摊,拍卖行
	void MSG_Booth_Request_Result       (const char * msg,int iLen);
	void MSG_Booth_Show_Other_Stall     (const char * msg,int iLen);
	void MSG_C2C_Booth_Info     (const char * msg,int iLen);//拍卖行物品信息

	void MSG_Player_Task_Info           (const char * msg,int iLen);
	void MSG_Monster_Magic_Effect       (const char * msg,int iLen);
	void MSG_Item_Effect_Switch         (const char * msg,int iLen);
	void MSG_Self_Character_Action      (const char * msg,int iLen);

	void SEND_Use_ReliveItem(DWORD dwTargetID, DWORD dwGoodID);

	void MSG_WoolStore_Token            (const char * msg,int iLen);
	void SEND_WoolStore_Guid            (const char * str,int iType = 0);

	//排名系统
	void MSG_Range_Sign_Up_Result		(const char *msg,int iLen);
	void MSG_Range_Request_Result		(const char *msg,int iLen);
	void MSG_Range_FindOtherPlace_Result(const char *msg,int iLen);
	void MSG_Range_Friend_Result		(const char *msg,int iLen);

	void Send_Range_Sign_Up				(DWORD id,WORD type);
	void Send_Range_Request				(DWORD id,WORD type,WORD vesion,WORD start);
	void Send_Range_FindOtherPlace		(WORD type,const char * name);
	void Send_Range_Friend				(WORD type);

	void MSG_Prentice_Flag                (const char *msg,int iLen);

	void MSG_Game_Greeting				(const char * msg,int iLen);
	void MSG_Message_System				(const char * msg,int iLen);
	void MSG_Message_Talk 				(const char * msg,int iLen);
	void MSG_Message_Guild 				(const char * msg,int iLen);
	void MSG_ServerVersion				(const char * msg,int iLen);
	void MSG_Message_General				(const char * msg,int iLen);

	void SEND_Message_Send				(CStringLine* pLine,const char * szHeader);
	void SEND_Message_Send              (const char * str,int iLen);
	void SEND_Message_Private           (CStringLine* pLine);
	void SEND_Message_Private           (const char * str,int iLen,bool bShow = true);

	void SEND_Guild_Ally				(const char * str);
	void SEND_Message_Rest				(const char * str, int iLen);

	void SEND_Player_Pet_State();

	//攻击方式
	void SEND_Player_Squat(char dir);
	void SEND_Player_Turn(char dir);
	void SEND_Player_Force_Attack(char dir);
	void SEND_Player_Normal_Attack(char dir,UINT uID = 0);
	void SEND_Player_Attack_Sword(char dir,WORD wMagicID);
	void SEND_Player_Attack_Magic(DWORD aimID,WORD aimX,WORD aimY,WORD magID);
	void SEND_Player_Use_Magic_Reel(DWORD aimID,WORD aimX,WORD aimY,WORD magID,DWORD dwBookID);
	void SEND_Player_Abandon_Task(WORD wTaskID);

	bool SEND_Package_Object_Reject		(CGood &Good);
	void SEND_Object_Pickup				(int x,int y);
	void SEND_Drop_Gold					(long lDropGood); // 扔下金币的数量 

	void SEND_Object_Keeping			(DWORD npcid,DWORD objID,const char * objName);
	void SEND_Object_Getback			(DWORD npcid,DWORD objID,const char * objName);
	void SEND_Make_Poison				(DWORD npcid,const char * PoisonName);


	void SEND_Package_Object_Arm		(CGood & m,int nPos);
	void SEND_Package_Object_Unarm		(CGood & m,int nPos);

	void SEND_Open_MapView				();
	void SEND_Open_Box_Requese          (int i,BYTE byContinueOpenNewBox = 0);

	void SEND_Guild_Message_Edit		(const char * content);
	void SEND_Guild_Member_Title_Edit	(const char * content,bool bNew = false);
	void SEND_Guild_Look_Message		();
	void SEND_Guild_Look_Web			();
	void SEND_Guild_Look_Member_List	();
	void SEND_Guild_Look_Exp_Val		();
	void SEND_Guild_Delete_Member		(const char * str);
	void SEND_Guild_Add_Member			(const char * str);
	void SEND_Guild_Reply_Invited		(bool bJoin);

	void SEND_Guild_Appoint_Dismiss		(const char* pName,WORD wJob,bool bAppoint);//任罢免官员
	void SEND_Guild_Online_Tael			();//申请行会在线成员银两信息
	void SEND_General_Protocol			(WORD wProtocol);//发送通用协议

	void SEND_Exchange_Repair			(DWORD npcid,DWORD weaponID,const char * strName,BYTE byRepairType = 0,BYTE byUseBind = 0); // 把武器放入托盘中修理
	void SEND_Exchange_Sale_Takein		(DWORD npcid,DWORD goodID,const char * strName);
	void SEND_Exchange_Sale_Ok			(DWORD npcid,DWORD goodID,const char * strName);
	void SEND_Exchange_Storage_Ok		(DWORD npcid,DWORD goodID,const char * strName,BYTE byType = 0);
	void SEND_Exchange_Buy_Ok			(DWORD npcid,DWORD goodID,const char * strName,BYTE byType = 0);
	void SEND_Exchange_Getback_Ok		(DWORD npcid,DWORD goodID,const char * strName,BYTE byType = 0);
	void SEND_Exchange_Poison_Ok		(DWORD npcid,const char * strName);
	void SEND_Exchange_Buy_Kind_Ok		(DWORD npcid,const char * strName,BYTE byType = 0);
	void SEND_Exchange_Get_Tex			(DWORD npcid,DWORD TexID);
	void SEND_Exchange_Forge_Ok			(DWORD npcid,DWORD goodID,const char * strName ,BYTE type);
	void SEND_Exchange_Forge			(DWORD npcid,DWORD goodID,const char * strName,BYTE type);
	void SEND_Exchange_Gem_Ok           (int iType,bool bTest);	//宝石凝练/合成消息


	void SEND_Group_Open_Team			(bool bOpen);
	void SEND_Group_Open_VoiceChat			(bool bOpen);
	void SEND_Group_Create				(const char * name);
	void SEND_Group_Add					(const char * name);
	void SEND_Group_Delete				(const char * name);
	void SEND_Group_Query				();
	void SEND_Exchange_Repair_Ok		(DWORD npcid,DWORD goodID,const char * strName,BYTE byType = 0,BYTE byUseBind = 0);
	void SEND_Trade_Goin				();
	bool SEND_Trade_Object_Tackin		(CGood & good);
	void SEND_Trade_Object_Tackout		(CGood & good);
	void SEND_Trade_Money_Tackin		(DWORD money,bool bYuanBao = false);
	void SEND_Trade_Ok					();
	void SEND_Trade_End					();
	void SEND_Warrantor					(const char * strName);

	void SEND_ShortCut_Key_Change       (WORD magID,char key);
	void SEND_Create_Character			(const char * info,int iLen);
	//void SEND_Delete_Character			(const char * info);

	void SEND_Create_Account			(const char * info,bool bBind=false,DWORD dwState = 0,const char * strValidate = NULL);
	void SEND_Login_Userpwd_Change		(const char * info,DWORD dwState = 0);
	bool SEND_Query_Username			(const char * str, int iLen,DWORD dwState = 0);

	void SEND_Bind_Dynamic_Password		(const char * sUser,const char * sPass,const char * sSerial,const char * sDPass);
	void SEND_Unbind_Dynamic_Password	(const char * sUser,const char * sPass,const char * sSerial,const char * sDPass,const char * sServeID);
	void SEND_Assert_Bind				(int iFlag);
	//void SEND_Assert_Reserve			(const char *sReserve,bool bSafeCard = false);
	void SEND_Assert_Card				();

	void SEND_Ready_Quit(int iQuitType);
	void SEND_Look_Player_Info			(DWORD id,int x, int y,char dir);
	void SEND_Player_Dig_Corpse			(DWORD id,int x, int y,char dir);
	void SEND_Player_Attack_Position	(char cPosition);

	void SEND_Out_Game					(bool bOpenSelCharWnd = true);

	void SEND_Player_Ride_Horse			(int iType = 0,DWORD dwID = 0);
	void SEND_Player_Train_Horse		(int x,int y);
	void SEND_Objects_Position			();

	void SEND_Phyle_Operate				(const char * name,BYTE byOperate);
	void SEND_Add_Friend				(const char * name);
	void MSG_Phyle_Guild_AddOrExit		(const char * strMsg,int iLen);//以宗派为单位来操作
	void MSG_Phyle_AddOrExit	(const char * strMsg,int iLen);

	void SEND_Del_Friend				(const char * name);
	void SEND_Answer_Friend_Request		(const char * name, bool bApprove);	
	void SEND_Answer_Group_Request		(const char * name, bool bApprove);

	void SEND_New_Version();
	//摆摊消息
	void SEND_Booth_Request_Stall       ();
	void SEND_Booth_Cancel_Stall        ();

	void SEND_Leave_Message_Info(const char* szName,const char* szAddr,const char* szContent);
	void SEND_User_Real_Info(const char* szName,bool bMale,const char* szTelphone,
		const char* szAddress,const char* szPostCode);

	void SEND_Message_Booth(const char * str,int iLen,bool bBuyPart);
	void SEND_Login_Validate();
	void MSG_Login_Validate(const char * strMsg,int iLen);

	//1.908 商城道具	黄榜大旗
	void SEND_Input_YelloFlag(DWORD id,string& str,int x,int y,WORD type);
	void SEND_Message_ChangshengXuanbin(DWORD id,WORD type,char i);
	void MSG_ChangshengXuanbin(const char * strMsg,int iLen);
	void SEND_Message_See_Mon(DWORD id,WORD i);
	void MSG_Extern_Messgebox_info(const char * strMsg,int iLen);
	void SEND_LeavWords(DWORD id,const string& str,WORD type);

	void MSG_CellPhoneUser_InputRequest(const char* strMsg,int iLen);
	void SEND_CellPhoneUser_Input(string& str,WORD type);

	void SEND_SmallGem_Condense_Request(int i1,int i2,int i3);
	void SEND_SmallGem_Count_Request();
	void MSG_SmallGem_Count(const char* strMsg,int iLen);

	void SEND_SndaMark_Request(); 
	void MSG_SndaMark_Answer(const char* strMsg,int iLen) ;

	// 人物消息
	bool SEND_Player_Walk(int x,int y);
	bool SEND_Player_Run(int x,int y);
	void SEND_Leopard_Action(int x,int y);

	void SEND_Quit_Game(bool bSendQuit = true);

	// 和npc交互
	void SEND_Exchange_Goin(DWORD npcid);
	void SEND_Exchange_Menu_Click(DWORD npcid,const char * act,int type=0,DWORD texID = 0,WORD x=-1,WORD y=-1);

	void SEND_Change_hair(UCHAR cHair);
	void SEND_Change_BodyColor(UCHAR cBody);
	void SEND_Player_StopGo();

	void SEND_Player_Salute();
	void SEND_Refash_All();
	void SEND_Exchange_Submit(DWORD dwNpcID,DWORD dwGoodID);
	void SEND_Message_Submit(DWORD dwNpcID,BYTE cType,BYTE cFlag,char *sText);
	void SEND_Midi_For_Other(char *sName,char *sBody,DWORD dwMidiID);
	void SEND_Npc_Msgbox(DWORD dwNpcID,char *sBody);
    //客户端请求一个物品名称对应的数据，为了在npctext里显示图片对应的tips
    void SEND_Good_Info_Require(const char *strGoodName);
    //服务器应答物品请求数据
    void  MSG_Good_Info_Answer(const char* msg,int iLen);

	// 宠物包裹
	void SEND_Pet_Object_Storage(CGood& Good);
	void SEND_Pet_Object_GetBack(CGood& Good);
	void SEND_Pet_Object_Pickup(int iX,int iY);
	void MSG_Pet_Object_Add(const char* msg, int iLen);
	void MSG_Pet_Object_Storage_Result(const char* msg, int iLen);
	void MSG_Pet_Object_GetBack_Result(const char* msg, int iLen);
	void MSG_Pet_Object_Info(const char* msg, int iLen);
	void MSG_Pet_Package_Capacity(const char* msg, int iLen);
	void MSG_Pet_Object_Pickup_Fail(const char* msg,int iLen);
	void MSG_Pet_State_Change(const char* msg,int iLen);

	void SEND_Request_Server_Time();
	void MSG_Answer_Server_Time(const char* msg, int iLen);

	void MSG_Investigation(const char* msg,int iLen);//调查问卷
	void MSG_Web_MessageBox(const char* strMsg,int iLen);
	void MSG_Special_Effect(const char * msg,int iLen);

	//拍卖行
	void SEND_SUBMIT_PAIMAI_GOOD(const char* strGUID);
	void MSG_PAIMAI_GOOD_INFO(const char * strMsg,int iLen);
	void MSG_PAIMAI_RESULT(const char * strMsg,int iLen);
	void MSG_PAIMAI_SALE_RESULT(const char * strMsg,int iLen);
	void MSG_Check_User(const char * strMsg, int iLen);
	void SEND_Check_User();

	void MSG_Server_Switch(const char* strMsg,int iLen);

	void  SEND_Deal_Special_Obj(DWORD dwObjId1,DWORD dwObjId2,BYTE x);//豹子复活相关
	void  MSG_Deal_Special_Obj(const char* strMsg,int iLen);//豹子复活相关
	//宠物寄养
	void  SEND_Pet_Adopt_Request(BYTE cRequestType,DWORD dwData = 0,DWORD dwID = 0);
	void  MSG_Pet_Adopt_Info(const char* msg,int iLen);//宠物寄养相关信息
	void  MSG_SendOutPet_Info(const char* msg,int iLen);//自己放出来的的豹子的相关信息
	void  MSG_MyAdoptPet_Info(const char* msg,int iLen);//自己放领养回来的豹子的相关信息
	void  SEND_Pet_Info_Request(BYTE cRequestType = 0,DWORD dwData = 0,DWORD dwID = 0);//请求宠物数据

	void  MSG_PET_ADOPT_END(const char * msg,int iLen);
	void  MSG_Npc_Quick_Exchange(const char* msg,int iLen);//NPC购物图形化，弹出快捷买卖、修理窗口,仓库
	void  SEND_Npc_Quick_Exchange(BYTE byType,DWORD dwNpcID,DWORD dwGoodID,const char* strName);//NPC图形化购物
	void  MSG_Player_Trade(const char* msg,int iLen);//服务器通过有玩家请求和自己交易
	void  SEND_Player_Trade(BYTE byType,DWORD dwID,BYTE byData = 0);//请求交易新协议
	void  SEND_Master_Prentice_Request(DWORD dwId,BYTE byType,BYTE byData = 0);// 拜师与招徒相关请求
	void  MSG_Master_Prentice(const char* msg,int iLen);//拜师与招徒相关信息
	void  MSG_Split_Stone(const char * msg,int iLen);//拆分符石
	void  SEND_Stone_Split_Request(DWORD dwID,DWORD dwSplitNum,BYTE byType);//拆分符石
	void  SEND_Set_Use_Object_Para(BYTE byObjType,DWORD dwObjID,BYTE byData,const char * pStrData = NULL);//使用物品的参数设置，如回城神石
	void  MSG_Set_Use_Object_Para(const char * msg,int iLen);//新手怪物召唤卡
	void  MSG_Player_SkillPower(const char* msg,int iLen);//元气值
	void  SEND_Feed_Pet_From_Interface(DWORD dwPetID,DWORD dwFoodID);//从宠物界面给指定宠物喂食


	//语音聊天
	void  SEND_Voice_Request_Info(int id,const char* strPasWd,int Type = 1);
	void  SEND_Voice_Request_Server_Addr();
	void  MSG_VoiceChat_Enable(const char * msg,int iLen);
	void  MSG_Voice_Info(const char* msg,int iLen);
	void  MSG_Voice_Server_Addr(const char* msg,int iLen);
    void  MSG_Flash_Voice_Btn(const char * strMsg2,int iLen);//服务器通知客户端闪烁语聊按钮

	void  SEND_Get_Medal_Exp();
	void  Msg_Update_Medal_Attribute(const char * msg,int iLen);//更新勋章属性
	void  Msg_Update_Medal_Exp(const char * msg,int iLen);//更新勋章升级经验
	void  Msg_Take_Put_Seal(const char* msg,int iLen);//存取封元印
	void  Msg_WuXing_Flag(const char* msg,int iLen);//五行令旗
	void  SEND_Seal_Info(DWORD dwID,int iType,DWORD dwConID);
	void  SEND_WuXing_Request_Prize();//五行玄关完成后请求服务器发奖
    void  SEND_PLAY_WXXG(BYTE byType); //
	void  SEND_WuXing_Apply_Monster(DWORD dwID,int iX,int iY);

	void  MSG_FengShengTakeObject(const char *msg,int iLen);//取走分神道具
    void SEND_Fill_LingFuBag(DWORD dwBagID,DWORD dwLingFuID);//灵符加入灵符袋
    void SEND_PET_MERGE(BYTE byType,DWORD dwFirstPetID,DWORD dwSecondPetID,DWORD dwSealID);//宠物幻化,客户端请求检查灵兽石type为0，客户端请求幻化灵兽石type为1
    void MSG_PET_MERGE(const char* strMsg2,int iLen);//宠物幻化
    //void SEND_SessionId_To_SelGate();//把sessionid发给selgate
	void MSG_BOX_INFO(const char* msg,int iLen);
	void SEND_BOX_INFO(int iType,DWORD dwGoodID,BYTE byArea);
	void MSG_MAZE_INFO(const char* msg,int iLen);
	void Msg_Guild_Ini_Info(const char* msg,int iLen);//行会初始化信息
	void Msg_Guild_Status_Switch(const char* msg,int iLen);//行会状态开关
	void Msg_Guild_Off_Appoint_Dis(const char* msg,int iLen);//行会官员任罢免响应协议
	void SEND_Guild_Off_Appoint_Confirm(bool bAccept);//任职确认
	void Msg_Guild_Change_Tile(const char* msg,int iLen);//行会成员封号改变协议
	void Msg_Guild_Online_Tael(const char* msg,int iLen);//行会在线人员的银两信息
	void Msg_Guild_Member_OnOffLine(const char* msg,int iLen);//行会成员上下线协议
	void Msg_Guild_DevoteResource_Result(const char* msg,int iLen);//行会捐献资源结果
	void SEND_Guild_Assign_Tael(DWORD dwTael,const char* pName);
	void SEND_Guild_Contribute(DWORD dwID1 = 0,DWORD dwID2 = 0,DWORD dwID3 = 0);//个人捐赠
	void Msg_Guild_Resource_Owner(const char* msg,int iLen);
	void Msg_Guild_Person_Tael(const char* msg,int iLen);//行会个人银两
	void Msg_Guild_Change_Tael(const char* msg,int iLen);//行会分配银两修改银两数
	void SEND_ZHENBAO_INFO(int iType,BYTE iNum = 0);
	void MSG_ZHENBAO_INFO(const char* msg,int iLen);
	void SEND_GUILD_TASK_INFO(BYTE byType);//0是发布，1是接取,2是发布内容，3是接取内容	
	void SEND_SHENGONGFANG_INFO(DWORD dwShieldID,DWORD dwWoodID,DWORD dwLuckID,BYTE byType);
	void MSG_SHENGONGFANG_INFO(const char* msg,int iLen);
	//void SEND_UsbMb(BYTE byType,DWORD dwData);//获得us密宝,然后根据不同类型发送不同的协议
	void Msg_TuTeng_State(const char* msg,int iLen);//各种行会图腾效果	

	void SEND_Kfz_Activate();//激活跨服战帐号
	void SEND_Kfz_Confirm_Member();//会长确认参加跨服战成员名单
	void SEND_Request_Kfz_Join_Member();//请求本服参加跨服战的成员名单
	void MSG_Kfz_Join_Member(const char *msg,int iLen);//服务器告诉客户端本服参加跨服战的成员名单
	void MSG_Kfz_Activate_Result(const char *msg,int iLen);//服务器告诉客户端激活结果
	void MSG_Kfz_Visitor_Join(const char *msg,int iLen);//观众报名观看跨服战
	void MSG_Second_TimeOut(const char *msg,int iLen);//读秒倒计时
	void MSG_Kfz_Result(const char *msg,int iLen);//跨服战结果
	void MSG_Kfz_MemberState(const char *msg,int iLen);//跨服战成员状态
	void MSG_Kfz_ArenaList(const char *msg,int iLen);//跨服战擂台列表
	void SEND_Request_Kfz_ArenaList(int iStart,int iEnd);//请求跨服战擂台列表
	void SEND_Kfz_Arena_Join(const char * strMapName);//请求攻擂
	void MSG_Guild_Stele_Register_Result(const char* strMsg2,int iLen);
	void MSG_Guild_Stele_Parter(const char *msg,int iLen);//仙灵碑报到列表
	void SEND_Guild_Stele_Register();
	void MSG_Guild_Stele_HeroNum(const char* strMsg2,int iLen);//返回精英战将人数
	void MSG_Guild_Stele_Hero_Result(const char* strMsg2,int iLen);//挑选挑战精英战将结果
	void MSG_Guild_Stele_Level(const char* strMsg2,int iLen);//评定仙级结果

	void SEND_Kfz_MemberState(BYTE byType);//确认跨服战成员状态
	void SEND_EnlargePackage(BYTE byType);//下标为6的那个字节表示类型,1为扩包裹,2为扩钱袋
	void MSG_MsgBox_Command(const char * msg,int iLen);//弹出对话框,按确认后根据不同类型做不同的事
	void SEND_General_Reply_Confirm(bool bAccept,BYTE byType);//通用确认
	void Send_Pet_Explore(DWORD id,byte type);	//凤凰探宝
	void MSG_Guild_Phyle_State(const char* msg,int iLen);//行会修神还是修魔 0是凡人1是神人2是魔
	void MSG_Credit_Info(const char * msg,int iLen);//信用信息
	void MSG_Credit_Req(const char * msg,int iLen);//购买道具元宝不足时询问玩家是否用信用购买请求，没有开通信用跳到igw提示通
	void MSG_Credit_Token(const char * msg,int iLen);//信用购买数字签名串
	void MSG_General_Operate_Result(const char* msg,int iLen);//通用操作结果
	void MSG_HeavenCity_Status(const char* msg,int iLen);//锦罗天城
	void MSG_DynObj_Appear(const char * msg,int iLen);//动态物体出现
	void MSG_DynObj_DisAppear(const char * msg,int iLen);//动态物体消失
	void MSG_HeiYanMo_Refraction(const char * msg,int iLen);//黑怨魔折射效果
	void MSG_Quick_TG_Monster_List(const char *msg ,int iLen);//快速救公主gs发过来的死亡怪物列表
	void MSG_Vip_Store_Info(const char *msg ,int iLen);//vip商城信息
	void MSG_Vip_Store_Personal_Info(const char *msg ,int iLen);//vip商城个人信息
	void MSG_Elixir_Info(const char* msg,int iLen);


	void SEND_DISASSEMBLE_GOODS(DWORD id,bool btest);
	void MSG_DISASSEMBLE_GOODS(const char * msg,int iLen);
	void SEND_COMBINE_YUANSHI(DWORD id,bool btest);
	void MSG_COMBINE_YUANSHI(const char * msg,int iLen);

	void MSG_JINGLI_VALUE(const char * msg,int iLen);
	//void MSG_LIANHUA_VALUE(const char * msg,int iLen);
	//void MSG_LIANHUA_LEVEL(const char * msg,int iLen);

	void Send_Extract_Gem();
	void Send_Remove_Gem(DWORD dwGemID,BYTE byType);
	void Send_Del_Skill(DWORD dwGoodID,string& strSkillName);

	void Send_Elixir_Buy_Apply(int iType,int iCount);

	void Msg_Relive_Demon(const char * msg,int iLen);
	//void Send_Relive_Demon();

	void Send_Drill_Equip(DWORD dwEquipID,DWORD dwLuckID,bool bTest);
	void Msg_Success_Probability(const char * msg,int iLen);

	void Send_Embed_Fushi(DWORD dwEquipID,DWORD dwFushiID,DWORD dwLuckID,bool bTest);
	void Send_Remove_Fushi(DWORD dwEquipID,BYTE byHoleNum,DWORD dwLuckID,bool bTest);
	void Send_Upgrade_Fushi(DWORD dwFushiID,DWORD dwLuckID,bool bTest);
	void Send_Combine_Fushi(DWORD dwLuckID,DWORD FushiID1,DWORD FushiID2,DWORD FushiID3,DWORD FushiID4 = 0,DWORD FushiID5 = 0,bool bTest = true);
	void Send_Change_Hole(DWORD dwEquipID,BYTE bySrcHole,BYTE byDestHole,DWORD dwLuckID,bool bTest);
	void Send_Upgrade_Equip(DWORD dwEquipID,DWORD dwFushiID,DWORD dwLuckID);
	void Send_Degrade_Fushi(DWORD dwFushiID,DWORD dwLuckID);

	void Send_EquipSoul(WORD type, DWORD dwEquipID,DWORD dwAddGoodID = 0);
	void Send_EquipSoulLevelUp(BYTE byUseBind,DWORD dwEquipID,DWORD JingshiID1 = 0,DWORD JingshiID2 = 0,DWORD JingshiID3 = 0,DWORD JingshiID4 = 0,DWORD JingshiID5 = 0);

	void MSG_NPC_LOOK_CHANGE(const char * msg,int iLen);
	void Msg_Player_Prompt_Info(const char* msg,int iLen);
	void Send_Player_Prompt_Status(int i,int iType = 0);//0为泡泡
	//void Send_Player_Prompt_Status(ePaoPaoPromptStatus paopao);
	void Send_Produce_Info(DWORD dwID,BYTE byType);

	void MSG_Open_LuckQYZWnd(const char * msg,int iLen);//打开幸运珍宝阁窗口
	void Send_LuckQYYZ_Select(BYTE pos);//发送抽奖信息
	void MSG_LuckQYZ_Prize(const char * msg, int iLen);//抽奖结果

	void Send_BlessZhongZhou_Change(DWORD dwCharID, DWORD dwChangeID, BYTE ChangeTo);//发送天佑中州字符转换消息
	void MSG_Open_BlessZhongZhouChangeDlg(const char * msg, int iLen);//打开天佑中州字符转换对话框
	void Send_Change_Material(DWORD dwGoodID, DWORD dwChangeID, BYTE MtrlType, BYTE ChangeTo);//发送转换材料消息

	void MSG_INSTANCEZONE_POPUP(const char * msg, int iLen);
	void SEND_INSTANCEZONE_POPUP(byte byType,byte bySelect);

	void SEND_SELECT_BAOJIAN(byte byRound);
	void MSG_SELECT_BAOJIAN(const char * msg, int iLen);

	void MSG_YuanbaoSell_Info(const char * msg, int iLen);
	void Send_Buy_YuanBao(string& userId,string& userName,DWORD dwCount,DWORD dwPrice);
	void Send_MyYuanBao_Query(byte byType);
	void Msg_MyYuanBao_Query(const char * msg, int iLen);
	void Send_OtherYuanBao_Query();
	void Send_YuanBao_Cancel();
	void Send_YuanBao_Sell(DWORD dwNum,DWORD dwPrice);
	void Sned_Take_YuanBaoMoney();
	void Msg_GiveOnlinePrize(const char * msg, int iLen);
	void Msg_Txzg(const char * msg, int iLen);
	void SEND_Txzg_GetPrize();
	void SEND_RUNE_INTO_TIGER(DWORD tigerID, DWORD runeID, BYTE index);						//向虎符放入字符
	void Msg_Meritoriousness_Update(const char *msg, int iLen);
	void Send_Meritoriousness_Update(BYTE requestType,BYTE exchangeType = 0,DWORD number =0);
	void Send_RTSInstance_Exchange_ByResource(BYTE type);

	//租赁摊位,离线摆摊
	void Msg_LeaseBooth_State(const char * msg, int iLen);
	void Send_LeaseBooth_Request(int iType);
	void Msg_LeaseBooth_Response(const char * msg, int iLen);
	void Send_LeaseBooth_Start(DWORD npcid, bool onlyOne = false);
	void Msg_LeaseBooth_Start(const char * msg, int iLen);
	void Send_LeaseBooth_BuyOthers(DWORD npcid, DWORD goodid,BYTE buyOthers,const char * strName);
	void Msg_LeaseBooth_BuyOthers(const char * msg, int iLen);
	void Send_LeaseBooth_TakeMoneyBack(WORD flag);
	void Msg_LeaseBooth_TakeMoneyBackResponse(const char * msg, int iLen);
	void Send_LeaseBooth_LeaveMessage(DWORD npcid,WORD msgLen,const char * message);
	void Msg_LeaseBooth_OverdueTime(const char * msg, int iLen);

	//商行,离线收购
	void Send_Purchase_Request(DWORD goodsID,WORD wPage,BYTE byType,BYTE bySelf,BYTE level = 0,BYTE tradeType = -1);
	void Msg_GetFirmItemList(const char * msg, int iLen);
	void Msg_SelectFirmItem(const char * msg, int iLen);
	void Msg_PurchaseItem(const char * msg, int iLen);
	void Send_MyPurchaseInfo(DWORD itemIdx,DWORD nlevel,DWORD nUnitNum,DWORD nNumPerUnit,DWORD dwPrice,BYTE byTradeType);
	void Send_SellItem(DWORD dwRowID,DWORD dwGoodsID);
	void Send_CancelPurchase(DWORD dwRowID);
	void Send_TakeBackGoods();
	void Send_GetMoneyOver();
	//////////////////////////////////托管////////////////////////////////////////
// 	void Send_Trusteeship(const char* pName,BYTE byMode);
// 	void MSG_Trustship(const char* msg,int iLen);
// 
// 	void Send_TrusteeshipSwitch(BYTE byMode,bool bValue,bool bAddExp = false);
// 	void MSG_TrusteeshipSwitch(const char* msg,int iLen);
// 
// 	void MSG_TrusteeshipHeadMode(const char* msg,int iLen);
// 	void MSG_TrusteeshipHeadPos(const char* msg,int iLen);
// 
// 	void Send_TrusteeshipMode(const char* pName,DWORD dwMode, DWORD dwParam = 0);
// 	void MSG_TrusteeshipMode(const char* msg,int iLen);
// 
// 	void Send_TrusteeshipRespond(bool b, BYTE byMode, const char* pName);
// 	void MSG_TrusteeshipAsk(const char* msg,int iLen);
//
// 	void Send_TrusteeshipHeadAttackTarget(DWORD dwID);
// 	void MSG_TrusteeshipHeadAttackTarget(const char* msg,int iLen);
// 	void MSG_TneupMember(const char* msg, int iLen);
// 
// 	void Send_Trusteeship_DataExchange(DWORD dwPlayerID,WORD wType);
// 	void MSG_Trusteeship_DataExchange(const char* msg,int iLen);
// 
// 	bool Send_Trusteeship_GoodExchange(CGood& Good,DWORD dwMemberID,BYTE type);
// 	bool SEND_Trusteeship_Use_Object(int iPos);		//使用托管包裹里的物品
// 	void MSG_Trusteeship_GoodExchange(const char* msg, int iLen);
// 	void MSG_Trusteeship_NeedJob(const char* msg, int iLen);
// 	void Send_Trusteeship_NeedJob(DWORD dwGoodID);
// 
// 	void Send_Trusteeship_HPMP(BYTE byPos, WORD wMaxHP, WORD wMaxMP, WORD wHP, WORD wMP);
// 	void MSG_Trusteeship_HPMP(const char* msg, int iLen);
// 
// 	void SEND_MicroControl_Select(DWORD dwID,BYTE byType = 0);
// 	void MSG_MicroControl_Select(const char* msg, int iLen);
// 
// 	void SEND_MicroControl_SendProtocol(DWORD dwID);
// 	void MSG_MicroControl_ReceiveProtocol(const char* msg, int iLen);
 	void MSG_Monster_Change_Ball(const char * msg, int iLen);
// 
// 	void Send_Refresh_QunYing_Members();
// 	void MSG_Refresh_QunYing_Members(const char* msg, int iLen);
// 
// 	void Send_QunYing_Reports();
// 	void MSG_QunYing_Reports(const char* msg, int iLen);
// 
// 	void MSG_Trusteeship_Member_Is_Offline(const char* msg, int iLen);
// 
// 	void Send_Trustepship_Only_Friend(BYTE byType);
// 	void MSG_Trustepship_Only_Friend(const char* msg, int iLen);
	//////////////////////////////////托管////////////////////////////////////////

	// 升级翅膀
	void Send_LevelUpWing(int pos, DWORD dwGoodId = 0);

	void MSG_Full_ZhenBao_Left_Time(const char* msg, int iLen);
	void MSG_ZhenBao_Value(const char* msg, int iLen);
	void MSG_Honor_Value(const char* msg, int iLen);


	//强制补全防沉迷
	void SENDNeedAuthen_Ack();
	void MSG_NeedAuthen_Req(const char * msg,int iLen);

	//神佑融合
	void Send_BlessCompound(DWORD dwGoodId1, DWORD dwGoodId2, DWORD dwFu = 0);	
	void Send_BlessExtend(DWORD dwGoodId, DWORD dwFu);

	void MSG_Swear_List(const char * msg,int iLen);
	void SEND_Add_Swear(const char * name);
	void SEND_Del_Swear(const char * name);
	void MSG_Swear_Request(const char * msg,int iLen);
	void SEND_Answer_Swear_Request(const char * name, bool bApprove);
	void MSG_AddDel_Swear_Success(const char * msg,int iLen);
	void MSG_Add_Swear_Success(const char * msg,int iLen);
	void MSG_Del_Swear_Success(const char * msg,int iLen);
	void MSG_Swear_Error(const char * msg,int iLen);


	void MSG_WenPei_List(const char * msg,int iLen);
	void SEND_WenPei_Compound(int id1, int id2, int id3, int id4);
	void SEND_WenPeiSub_Decompound(int iSubId, int dwId);
	void SEND_WenPei_Decompound(DWORD dwId);
	void SEND_WenPei_Streng(DWORD dwGoodId1, DWORD dwGoodId2, DWORD dwFu);
	void SEND_WenPei_RongHe(DWORD dwLeftGoodId1, DWORD dwRightGoodId2, DWORD dwMdGoodId);
	void SEND_WenPeiSub_TiQu(int iSubId, int dwId);

	void SEND_Heart_Beat();

	void SEND_EnterShan();

	void SEND_Request_Adventrue();
	void SEND_Request_Give_Adventrue_Reward();
	void MSG_Notify_Adventure_Status(const char * msg,int iLen);
	void MSG_Notify_Adventure_Reward(const char * msg,int iLen);

	void SEND_HasWaiGua(bool bHas);
	void MSG_HasWaiGua(const char * msg,int iLen);

	void SEND_Mac_To_GS();

	void MSG_Player_ConSkillBuff(const char * msg,int iLen);

	void SEND_Equip_Compose(DWORD dwGoodId1, DWORD dwGoodId2);
	void SEND_Szzp_Compose(DWORD dwGoodId1, DWORD dwGoodId2);

	void MSG_SANWEIZHENHUOINFO(const char * msg,int iLen);
	void MSG_NPC_LINGWUSANWEIZHENHUO(const char * msg,int iLen);

	void SEND_GoLingWu();
	
	void SEND_Request_FaBaoRenZhu(DWORD dwGoodId);
	void SEND_Request_FaBaoZhuangBei(CGood & m ,int nPos);

	void Send_FabaoChaifen(DWORD dwGoodId);

	void MSG_NoticeIE(const char * msg,int iLen);
	void SEND_FireArtifice(int iGridCount, WORD test);
	void MSG_FireArtifice(const char * msg,int iLen);
	void MSG_FireArtificeTime(const char * msg,int iLen);

	void Send_Upgrade_FaBao(DWORD dwGoodMainFaBao, DWORD dwGoodSecondFaBao1, DWORD dwGoodSecondFaBao2, DWORD dwGoodSecondFaBao3, DWORD dwGoodFu);	

	void MSG_New_Storage_Info			(const char * msg,int iLen);	//新仓库协议
	void SEND_Objects_Storage_Position  ();

	void MSG_FABAOATTACK(const char * msg,int iLen);
	void MSG_ZHUANGBEIFABAO(const char * msg,int iLen);
	void MSG_FABAOAXILING(const char * msg,int iLen);

	void SEND_Player_Vip_Option(int iType,DWORD dwData,DWORD dwData2 = 0,DWORD dwData3 = 0,const char *szData = NULL);//vip及vip商城操作
	void MSG_Player_VipInfo(const char * msg,int iLen);
	void MSG_VipStoreInfo(const char * msg,int iLen);

	void Send_Login_Check_Mask(const char* pData);
	void Msg_Login_Check_Mask(const char* msg,int iLen);

	//12宫布兵
	void SEND_EditBing(WORD wEditType, BYTE byType, WORD wInOut, WORD wBingID, int x, int y);
	void SEND_EditBingAdd(BYTE byType, WORD wInOut, int x, int y);
	void SEND_EditBingDelete(WORD wInOut, WORD wBingID);
	void SEND_EditBingMove(WORD wInOut, WORD wBingID, int newx, int newy);
	void SEND_GetBingInfo();
	void MSG_BingInfo(const char * msg,int iLen);
	void SEND_BingControl(WORD type, DWORD dwTargetID = 0);
	void MSG_OpenBingCtrlWnd(const char * msg,int iLen);

	//void MSG_YIHUO_REQUEST(const char * msg,int iLen);
	void SEND_YIHUO_NAME_SHORTCUT(BYTE shortcut, const char* pName);

	void MSG_Fire_Shield(const char * msg,int iLen);
	void MSG_DaoZun_JiangLin(const char * msg,int iLen);
	void MSG_ShadowKill8(const char * msg,int iLen);

	void MSG_YiHuoZhanMsg(const char * msg,int iLen);
	void SEND_YiHuoZhanMsg(BYTE btn);

	void MSG_Activerate_Prize(const char * msg,int iLen);
	void SEND_Activerate_Prize(BYTE byType,BYTE bySelPrize);

	void MSG_LianJi_Show(const char * msg,int iLen);
	
	void SEND_Equip_Chaifen(DWORD dwGoodId, BYTE type);
	void MSG_Equip_Chaifen(const char * msg,int iLen);

	// 赤金护符
	void SEND_HufuFlush(BYTE type, DWORD dwGoodId, DWORD dwAddGoodId);
	void MSG_YHufuFlush(const char * msg,int iLen);

	void Send_HorseManShip_Watch_Req(BYTE type, const char * strName, DWORD dwGoodId);
	void MSG_HorseManShip_Watch(const char * msg,int iLen);
	void MSG_HorseManShip_ActionPoint(const char * msg,int iLen);

	void MSG_Monster_Hdnw_Lock(const char * msg,int iLen);
	void SEND_Enlarge_Package_Info_Req();
	void MSG_Enlarge_Package_Info_Ack(const char * msg,int iLen);

	void MSG_PLAYER_HORSEMANSHIP_INFO(const char * msg,int iLen);

protected:
	struct SMsgBuf
	{
		int iPos;
		int iLen;
	};
	vector<SMsgBuf>   m_msgs; //待处理的消息
	char      m_cMsgBuf[MAX_MESSAGE_BUFFER];

	char	  m_cBuf[MAX_SEND_MESSAGE_SIZE];

	DWORD m_dwJumpTime ;

	std::map<WORD,lpFNMSGProc> m_callback; //消息处理函数和ID的对应表

	//void CheckUseBCSS(CGood temp,int iPos);//是否触发快捷消息2.0的冰泉圣水
	void DealHeavenCityStatus(DWORD dwType,DWORD dwPos,DWORD dwStatus);
	bool IsEquipXWSkillSuit();
	void ModifyAppInfo();
	void SendGameInfoForCheck();		//向客户端监控系统发送游戏信息
};
