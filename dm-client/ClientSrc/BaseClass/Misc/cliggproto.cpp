#include "cliggproto.h"

CCliGgProto::CCliGgProto()
{
	memset(m_EncodeFuncArray, 0, sizeof(m_EncodeFuncArray));
	memset(m_DecodeFuncArray, 0, sizeof(m_DecodeFuncArray));
	Init();
}

CCliGgProto::~CCliGgProto()
{
}

int CCliGgProto::Encode(int iMsgID, void *pHost, char *pNet, int iNetSize)
{
	m_oData.Prepare(pNet, iNetSize);

	EnFuncCCliGgProto pfnEncode = FindEncodeFunc(iMsgID);
	if(NULL == pfnEncode)
		return -1;

	return (*pfnEncode)(pHost, &m_oData);
}

int CCliGgProto::Decode(int iMsgID, char *pNet, int iNetSize, void *pHost, int iHostSize)
{
	m_oData.Prepare(pNet, iNetSize);
	if(iHostSize < m_iPkgSizeArray[iMsgID - CLIGGPROTO_MSGID_OFFSET])
		return -1;

	DeFuncCCliGgProto pfnDecode = FindDecodeFunc(iMsgID);
	if(NULL == pfnDecode)
		return -1;

	return (*pfnDecode)(pHost, &m_oData);
}

EnFuncCCliGgProto CCliGgProto::FindEncodeFunc(int iMsgID)
{
	if(iMsgID - CLIGGPROTO_MSGID_OFFSET >= CLIGGPROTO_MSGID_RANGE)
		return NULL;
	return m_EncodeFuncArray[iMsgID - CLIGGPROTO_MSGID_OFFSET];
}

DeFuncCCliGgProto CCliGgProto::FindDecodeFunc(int iMsgID)
{
	if(iMsgID - CLIGGPROTO_MSGID_OFFSET >= CLIGGPROTO_MSGID_RANGE)
		return NULL;
	return m_DecodeFuncArray[iMsgID - CLIGGPROTO_MSGID_OFFSET];
}

bool CCliGgProto::Init()
{
	memset(m_EncodeFuncArray, 0, sizeof(m_EncodeFuncArray));
	memset(m_DecodeFuncArray, 0, sizeof(m_DecodeFuncArray));

	m_EncodeFuncArray[LG_CLI_GroupList_NTF - CLIGGPROTO_MSGID_OFFSET] = &EncodePKG_LG_CLI_GroupList_NTF;
	m_DecodeFuncArray[LG_CLI_GroupList_NTF - CLIGGPROTO_MSGID_OFFSET] = &DecodePKG_LG_CLI_GroupList_NTF;

	m_EncodeFuncArray[CLI_LG_SelGroup_REQ - CLIGGPROTO_MSGID_OFFSET] = &EncodePKG_CLI_LG_SelGroup_REQ;
	m_DecodeFuncArray[CLI_LG_SelGroup_REQ - CLIGGPROTO_MSGID_OFFSET] = &DecodePKG_CLI_LG_SelGroup_REQ;

	m_EncodeFuncArray[LG_CLI_SelGroup_ACK - CLIGGPROTO_MSGID_OFFSET] = &EncodePKG_LG_CLI_SelGroup_ACK;
	m_DecodeFuncArray[LG_CLI_SelGroup_ACK - CLIGGPROTO_MSGID_OFFSET] = &DecodePKG_LG_CLI_SelGroup_ACK;

	m_EncodeFuncArray[LG_CLI_SS_INFO - CLIGGPROTO_MSGID_OFFSET] = &EncodePKG_LG_CLI_SS_INFO;
	m_DecodeFuncArray[LG_CLI_SS_INFO - CLIGGPROTO_MSGID_OFFSET] = &DecodePKG_LG_CLI_SS_INFO;

	m_EncodeFuncArray[CLI_GG_Login_REQ - CLIGGPROTO_MSGID_OFFSET] = &EncodePKG_CLI_GG_Login_REQ;
	m_DecodeFuncArray[CLI_GG_Login_REQ - CLIGGPROTO_MSGID_OFFSET] = &DecodePKG_CLI_GG_Login_REQ;

	m_EncodeFuncArray[GG_CLI_Login_ACK - CLIGGPROTO_MSGID_OFFSET] = &EncodePKG_GG_CLI_Login_ACK;
	m_DecodeFuncArray[GG_CLI_Login_ACK - CLIGGPROTO_MSGID_OFFSET] = &DecodePKG_GG_CLI_Login_ACK;

	m_EncodeFuncArray[GG_CLI_Challenge_REQ - CLIGGPROTO_MSGID_OFFSET] = &EncodePKG_GG_CLI_Challenge_REQ;
	m_DecodeFuncArray[GG_CLI_Challenge_REQ - CLIGGPROTO_MSGID_OFFSET] = &DecodePKG_GG_CLI_Challenge_REQ;

	m_EncodeFuncArray[CLI_GG_Challenge_ACK - CLIGGPROTO_MSGID_OFFSET] = &EncodePKG_CLI_GG_Challenge_ACK;
	m_DecodeFuncArray[CLI_GG_Challenge_ACK - CLIGGPROTO_MSGID_OFFSET] = &DecodePKG_CLI_GG_Challenge_ACK;

	m_EncodeFuncArray[GG_CLI_Verify_REQ - CLIGGPROTO_MSGID_OFFSET] = &EncodePKG_GG_CLI_Verify_REQ;
	m_DecodeFuncArray[GG_CLI_Verify_REQ - CLIGGPROTO_MSGID_OFFSET] = &DecodePKG_GG_CLI_Verify_REQ;

	m_EncodeFuncArray[CLI_GG_Verify_ACK - CLIGGPROTO_MSGID_OFFSET] = &EncodePKG_CLI_GG_Verify_ACK;
	m_DecodeFuncArray[CLI_GG_Verify_ACK - CLIGGPROTO_MSGID_OFFSET] = &DecodePKG_CLI_GG_Verify_ACK;

	m_EncodeFuncArray[CLI_GG_SelectRole_REQ - CLIGGPROTO_MSGID_OFFSET] = &EncodePKG_CLI_GG_SelectRole_REQ;
	m_DecodeFuncArray[CLI_GG_SelectRole_REQ - CLIGGPROTO_MSGID_OFFSET] = &DecodePKG_CLI_GG_SelectRole_REQ;

	m_EncodeFuncArray[GG_CLI_SelectRole_ACK - CLIGGPROTO_MSGID_OFFSET] = &EncodePKG_GG_CLI_SelectRole_ACK;
	m_DecodeFuncArray[GG_CLI_SelectRole_ACK - CLIGGPROTO_MSGID_OFFSET] = &DecodePKG_GG_CLI_SelectRole_ACK;

	m_EncodeFuncArray[CLI_GG_CreateRole_REQ - CLIGGPROTO_MSGID_OFFSET] = &EncodePKG_CLI_GG_CreateRole_REQ;
	m_DecodeFuncArray[CLI_GG_CreateRole_REQ - CLIGGPROTO_MSGID_OFFSET] = &DecodePKG_CLI_GG_CreateRole_REQ;

	m_EncodeFuncArray[GG_CLI_CreateRole_ACK - CLIGGPROTO_MSGID_OFFSET] = &EncodePKG_GG_CLI_CreateRole_ACK;
	m_DecodeFuncArray[GG_CLI_CreateRole_ACK - CLIGGPROTO_MSGID_OFFSET] = &DecodePKG_GG_CLI_CreateRole_ACK;

	m_EncodeFuncArray[CLI_GG_DelRole_REQ - CLIGGPROTO_MSGID_OFFSET] = &EncodePKG_CLI_GG_DelRole_REQ;
	m_DecodeFuncArray[CLI_GG_DelRole_REQ - CLIGGPROTO_MSGID_OFFSET] = &DecodePKG_CLI_GG_DelRole_REQ;

	m_EncodeFuncArray[GG_CLI_DelRole_ACK - CLIGGPROTO_MSGID_OFFSET] = &EncodePKG_GG_CLI_DelRole_ACK;
	m_DecodeFuncArray[GG_CLI_DelRole_ACK - CLIGGPROTO_MSGID_OFFSET] = &DecodePKG_GG_CLI_DelRole_ACK;

	m_EncodeFuncArray[CLI_GG_ReselRole_REQ - CLIGGPROTO_MSGID_OFFSET] = &EncodePKG_CLI_GG_ReselRole_REQ;
	m_DecodeFuncArray[CLI_GG_ReselRole_REQ - CLIGGPROTO_MSGID_OFFSET] = &DecodePKG_CLI_GG_ReselRole_REQ;

	m_EncodeFuncArray[GG_CLI_ReselRole_ACK - CLIGGPROTO_MSGID_OFFSET] = &EncodePKG_GG_CLI_ReselRole_ACK;
	m_DecodeFuncArray[GG_CLI_ReselRole_ACK - CLIGGPROTO_MSGID_OFFSET] = &DecodePKG_GG_CLI_ReselRole_ACK;

	m_EncodeFuncArray[CLI_GG_UnDelRole_REQ - CLIGGPROTO_MSGID_OFFSET] = &EncodePKG_CLI_GG_UnDelRole_REQ;
	m_DecodeFuncArray[CLI_GG_UnDelRole_REQ - CLIGGPROTO_MSGID_OFFSET] = &DecodePKG_CLI_GG_UnDelRole_REQ;

	m_EncodeFuncArray[GG_CLI_UnDelRole_ACK - CLIGGPROTO_MSGID_OFFSET] = &EncodePKG_GG_CLI_UnDelRole_ACK;
	m_DecodeFuncArray[GG_CLI_UnDelRole_ACK - CLIGGPROTO_MSGID_OFFSET] = &DecodePKG_GG_CLI_UnDelRole_ACK;

	m_EncodeFuncArray[GG_CLI_KickOff_NTF - CLIGGPROTO_MSGID_OFFSET] = &EncodePKG_GG_CLI_KickOff_NTF;
	m_DecodeFuncArray[GG_CLI_KickOff_NTF - CLIGGPROTO_MSGID_OFFSET] = &DecodePKG_GG_CLI_KickOff_NTF;

	m_EncodeFuncArray[CLI_GG_Logout_NTF - CLIGGPROTO_MSGID_OFFSET] = &EncodePKG_CLI_GG_Logout_NTF;
	m_DecodeFuncArray[CLI_GG_Logout_NTF - CLIGGPROTO_MSGID_OFFSET] = &DecodePKG_CLI_GG_Logout_NTF;

	m_EncodeFuncArray[CLI_GG_DynCode_REQ - CLIGGPROTO_MSGID_OFFSET] = &EncodePKG_CLI_GG_DynCode_REQ;
	m_DecodeFuncArray[CLI_GG_DynCode_REQ - CLIGGPROTO_MSGID_OFFSET] = &DecodePKG_CLI_GG_DynCode_REQ;

	m_EncodeFuncArray[GG_CLI_DynCode_NTF - CLIGGPROTO_MSGID_OFFSET] = &EncodePKG_GG_CLI_DynCode_NTF;
	m_DecodeFuncArray[GG_CLI_DynCode_NTF - CLIGGPROTO_MSGID_OFFSET] = &DecodePKG_GG_CLI_DynCode_NTF;

	m_EncodeFuncArray[CLI_GG_VerifyStoreEkey_ACK - CLIGGPROTO_MSGID_OFFSET] = &EncodePKG_CLI_GG_VerifyStoreEkey_ACK;
	m_DecodeFuncArray[CLI_GG_VerifyStoreEkey_ACK - CLIGGPROTO_MSGID_OFFSET] = &DecodePKG_CLI_GG_VerifyStoreEkey_ACK;

	m_EncodeFuncArray[CLI_GG_FCMFillInfo_REQ - CLIGGPROTO_MSGID_OFFSET] = &EncodePKG_CLI_GG_FCMFillInfo_REQ;
	m_DecodeFuncArray[CLI_GG_FCMFillInfo_REQ - CLIGGPROTO_MSGID_OFFSET] = &DecodePKG_CLI_GG_FCMFillInfo_REQ;

	m_EncodeFuncArray[GG_CLI_ReservePwd_REQ - CLIGGPROTO_MSGID_OFFSET] = &EncodePKG_GG_CLI_ReservePwd_REQ;
	m_DecodeFuncArray[GG_CLI_ReservePwd_REQ - CLIGGPROTO_MSGID_OFFSET] = &DecodePKG_GG_CLI_ReservePwd_REQ;

	m_EncodeFuncArray[CLI_GG_ReservePwd_ACK - CLIGGPROTO_MSGID_OFFSET] = &EncodePKG_CLI_GG_ReservePwd_ACK;
	m_DecodeFuncArray[CLI_GG_ReservePwd_ACK - CLIGGPROTO_MSGID_OFFSET] = &DecodePKG_CLI_GG_ReservePwd_ACK;

	m_EncodeFuncArray[CLI_GG_Extend_MSG - CLIGGPROTO_MSGID_OFFSET] = &EncodePKG_CLI_GG_Extend_MSG;
	m_DecodeFuncArray[CLI_GG_Extend_MSG - CLIGGPROTO_MSGID_OFFSET] = &DecodePKG_CLI_GG_Extend_MSG;

	m_EncodeFuncArray[GG_CLI_Extend_MSG - CLIGGPROTO_MSGID_OFFSET] = &EncodePKG_GG_CLI_Extend_MSG;
	m_DecodeFuncArray[GG_CLI_Extend_MSG - CLIGGPROTO_MSGID_OFFSET] = &DecodePKG_GG_CLI_Extend_MSG;

	m_EncodeFuncArray[GG_CLI_GroupInfo_NTF - CLIGGPROTO_MSGID_OFFSET] = &EncodePKG_GG_CLI_GroupInfo_NTF;
	m_DecodeFuncArray[GG_CLI_GroupInfo_NTF - CLIGGPROTO_MSGID_OFFSET] = &DecodePKG_GG_CLI_GroupInfo_NTF;

	m_EncodeFuncArray[CLI_GG_OfflineTrustee_NTF - CLIGGPROTO_MSGID_OFFSET] = &EncodePKG_CLI_GG_OfflineTrustee_NTF;
	m_DecodeFuncArray[CLI_GG_OfflineTrustee_NTF - CLIGGPROTO_MSGID_OFFSET] = &DecodePKG_CLI_GG_OfflineTrustee_NTF;

	m_EncodeFuncArray[GG_CLI_NeedAuthen_REQ - CLIGGPROTO_MSGID_OFFSET] = &EncodePKG_GG_CLI_NeedAuthen_REQ;
	m_DecodeFuncArray[GG_CLI_NeedAuthen_REQ - CLIGGPROTO_MSGID_OFFSET] = &DecodePKG_GG_CLI_NeedAuthen_REQ;

	m_EncodeFuncArray[CLI_GG_NeedAuthen_ACK - CLIGGPROTO_MSGID_OFFSET] = &EncodePKG_CLI_GG_NeedAuthen_ACK;
	m_DecodeFuncArray[CLI_GG_NeedAuthen_ACK - CLIGGPROTO_MSGID_OFFSET] = &DecodePKG_CLI_GG_NeedAuthen_ACK;

	m_EncodeFuncArray[GG_CLI_QQInfo_NTF - CLIGGPROTO_MSGID_OFFSET] = &EncodePKG_GG_CLI_QQInfo_NTF;
	m_DecodeFuncArray[GG_CLI_QQInfo_NTF - CLIGGPROTO_MSGID_OFFSET] = &DecodePKG_GG_CLI_QQInfo_NTF;

	m_EncodeFuncArray[GG_CLI_InGameVerify_REQ - CLIGGPROTO_MSGID_OFFSET] = &EncodePKG_GG_CLI_InGameVerify_REQ;
	m_DecodeFuncArray[GG_CLI_InGameVerify_REQ - CLIGGPROTO_MSGID_OFFSET] = &DecodePKG_GG_CLI_InGameVerify_REQ;

	m_EncodeFuncArray[CLI_GG_InGameVerify_ACK - CLIGGPROTO_MSGID_OFFSET] = &EncodePKG_CLI_GG_InGameVerify_ACK;
	m_DecodeFuncArray[CLI_GG_InGameVerify_ACK - CLIGGPROTO_MSGID_OFFSET] = &DecodePKG_CLI_GG_InGameVerify_ACK;

	m_EncodeFuncArray[CLI_GG_FreshVerify_Req - CLIGGPROTO_MSGID_OFFSET] = &EncodePKG_CLI_GG_FreshVerify_Req;
	m_DecodeFuncArray[CLI_GG_FreshVerify_Req - CLIGGPROTO_MSGID_OFFSET] = &DecodePKG_CLI_GG_FreshVerify_Req;

	m_EncodeFuncArray[GG_CLI_LoginVerify_REQ - CLIGGPROTO_MSGID_OFFSET] = &EncodePKG_GG_CLI_LoginVerify_REQ;
	m_DecodeFuncArray[GG_CLI_LoginVerify_REQ - CLIGGPROTO_MSGID_OFFSET] = &DecodePKG_GG_CLI_LoginVerify_REQ;

	m_EncodeFuncArray[CLI_GG_LoginVerify_ACK - CLIGGPROTO_MSGID_OFFSET] = &EncodePKG_CLI_GG_LoginVerify_ACK;
	m_DecodeFuncArray[CLI_GG_LoginVerify_ACK - CLIGGPROTO_MSGID_OFFSET] = &DecodePKG_CLI_GG_LoginVerify_ACK;

	m_EncodeFuncArray[CLI_GG_MERGE_PT_REQ - CLIGGPROTO_MSGID_OFFSET] = &EncodePKG_CLI_GG_MERGE_PT_REQ;
	m_DecodeFuncArray[CLI_GG_MERGE_PT_REQ - CLIGGPROTO_MSGID_OFFSET] = &DecodePKG_CLI_GG_MERGE_PT_REQ;

	m_EncodeFuncArray[CLI_GG_MERGE_PT_ACK - CLIGGPROTO_MSGID_OFFSET] = &EncodePKG_CLI_GG_MERGE_PT_ACK;
	m_DecodeFuncArray[CLI_GG_MERGE_PT_ACK - CLIGGPROTO_MSGID_OFFSET] = &DecodePKG_CLI_GG_MERGE_PT_ACK;

	m_iPkgSizeArray[LG_CLI_GroupList_NTF - CLIGGPROTO_MSGID_OFFSET] = (int) sizeof(PKG_LG_CLI_GroupList_NTF);
	m_iPkgSizeArray[CLI_LG_SelGroup_REQ - CLIGGPROTO_MSGID_OFFSET] = (int) sizeof(PKG_CLI_LG_SelGroup_REQ);
	m_iPkgSizeArray[LG_CLI_SelGroup_ACK - CLIGGPROTO_MSGID_OFFSET] = (int) sizeof(PKG_LG_CLI_SelGroup_ACK);
	m_iPkgSizeArray[LG_CLI_SS_INFO - CLIGGPROTO_MSGID_OFFSET] = (int) sizeof(PKG_LG_CLI_SS_INFO);
	m_iPkgSizeArray[CLI_GG_Login_REQ - CLIGGPROTO_MSGID_OFFSET] = (int) sizeof(PKG_CLI_GG_Login_REQ);
	m_iPkgSizeArray[GG_CLI_Login_ACK - CLIGGPROTO_MSGID_OFFSET] = (int) sizeof(PKG_GG_CLI_Login_ACK);
	m_iPkgSizeArray[GG_CLI_Challenge_REQ - CLIGGPROTO_MSGID_OFFSET] = (int) sizeof(PKG_GG_CLI_Challenge_REQ);
	m_iPkgSizeArray[CLI_GG_Challenge_ACK - CLIGGPROTO_MSGID_OFFSET] = (int) sizeof(PKG_CLI_GG_Challenge_ACK);
	m_iPkgSizeArray[GG_CLI_Verify_REQ - CLIGGPROTO_MSGID_OFFSET] = (int) sizeof(PKG_GG_CLI_Verify_REQ);
	m_iPkgSizeArray[CLI_GG_Verify_ACK - CLIGGPROTO_MSGID_OFFSET] = (int) sizeof(PKG_CLI_GG_Verify_ACK);
	m_iPkgSizeArray[CLI_GG_SelectRole_REQ - CLIGGPROTO_MSGID_OFFSET] = (int) sizeof(PKG_CLI_GG_SelectRole_REQ);
	m_iPkgSizeArray[GG_CLI_SelectRole_ACK - CLIGGPROTO_MSGID_OFFSET] = (int) sizeof(PKG_GG_CLI_SelectRole_ACK);
	m_iPkgSizeArray[CLI_GG_CreateRole_REQ - CLIGGPROTO_MSGID_OFFSET] = (int) sizeof(PKG_CLI_GG_CreateRole_REQ);
	m_iPkgSizeArray[GG_CLI_CreateRole_ACK - CLIGGPROTO_MSGID_OFFSET] = (int) sizeof(PKG_GG_CLI_CreateRole_ACK);
	m_iPkgSizeArray[CLI_GG_DelRole_REQ - CLIGGPROTO_MSGID_OFFSET] = (int) sizeof(PKG_CLI_GG_DelRole_REQ);
	m_iPkgSizeArray[GG_CLI_DelRole_ACK - CLIGGPROTO_MSGID_OFFSET] = (int) sizeof(PKG_GG_CLI_DelRole_ACK);
	m_iPkgSizeArray[CLI_GG_ReselRole_REQ - CLIGGPROTO_MSGID_OFFSET] = (int) sizeof(PKG_CLI_GG_ReselRole_REQ);
	m_iPkgSizeArray[GG_CLI_ReselRole_ACK - CLIGGPROTO_MSGID_OFFSET] = (int) sizeof(PKG_GG_CLI_ReselRole_ACK);
	m_iPkgSizeArray[CLI_GG_UnDelRole_REQ - CLIGGPROTO_MSGID_OFFSET] = (int) sizeof(PKG_CLI_GG_UnDelRole_REQ);
	m_iPkgSizeArray[GG_CLI_UnDelRole_ACK - CLIGGPROTO_MSGID_OFFSET] = (int) sizeof(PKG_GG_CLI_UnDelRole_ACK);
	m_iPkgSizeArray[GG_CLI_KickOff_NTF - CLIGGPROTO_MSGID_OFFSET] = (int) sizeof(PKG_GG_CLI_KickOff_NTF);
	m_iPkgSizeArray[CLI_GG_Logout_NTF - CLIGGPROTO_MSGID_OFFSET] = (int) sizeof(PKG_CLI_GG_Logout_NTF);
	m_iPkgSizeArray[CLI_GG_DynCode_REQ - CLIGGPROTO_MSGID_OFFSET] = (int) sizeof(PKG_CLI_GG_DynCode_REQ);
	m_iPkgSizeArray[GG_CLI_DynCode_NTF - CLIGGPROTO_MSGID_OFFSET] = (int) sizeof(PKG_GG_CLI_DynCode_NTF);
	m_iPkgSizeArray[CLI_GG_VerifyStoreEkey_ACK - CLIGGPROTO_MSGID_OFFSET] = (int) sizeof(PKG_CLI_GG_VerifyStoreEkey_ACK);
	m_iPkgSizeArray[CLI_GG_FCMFillInfo_REQ - CLIGGPROTO_MSGID_OFFSET] = (int) sizeof(PKG_CLI_GG_FCMFillInfo_REQ);
	m_iPkgSizeArray[GG_CLI_ReservePwd_REQ - CLIGGPROTO_MSGID_OFFSET] = (int) sizeof(PKG_GG_CLI_ReservePwd_REQ);
	m_iPkgSizeArray[CLI_GG_ReservePwd_ACK - CLIGGPROTO_MSGID_OFFSET] = (int) sizeof(PKG_CLI_GG_ReservePwd_ACK);
	m_iPkgSizeArray[CLI_GG_Extend_MSG - CLIGGPROTO_MSGID_OFFSET] = (int) sizeof(PKG_CLI_GG_Extend_MSG);
	m_iPkgSizeArray[GG_CLI_Extend_MSG - CLIGGPROTO_MSGID_OFFSET] = (int) sizeof(PKG_GG_CLI_Extend_MSG);
	m_iPkgSizeArray[GG_CLI_GroupInfo_NTF - CLIGGPROTO_MSGID_OFFSET] = (int) sizeof(PKG_GG_CLI_GroupInfo_NTF);
	m_iPkgSizeArray[CLI_GG_OfflineTrustee_NTF - CLIGGPROTO_MSGID_OFFSET] = (int) sizeof(PKG_CLI_GG_OfflineTrustee_NTF);
	m_iPkgSizeArray[GG_CLI_NeedAuthen_REQ - CLIGGPROTO_MSGID_OFFSET] = (int) sizeof(PKG_GG_CLI_NeedAuthen_REQ);
	m_iPkgSizeArray[CLI_GG_NeedAuthen_ACK - CLIGGPROTO_MSGID_OFFSET] = (int) sizeof(PKG_CLI_GG_NeedAuthen_ACK);
	m_iPkgSizeArray[GG_CLI_QQInfo_NTF - CLIGGPROTO_MSGID_OFFSET] = (int) sizeof(PKG_GG_CLI_QQInfo_NTF);
	m_iPkgSizeArray[GG_CLI_InGameVerify_REQ - CLIGGPROTO_MSGID_OFFSET] = (int) sizeof(PKG_GG_CLI_InGameVerify_REQ);
	m_iPkgSizeArray[CLI_GG_InGameVerify_ACK - CLIGGPROTO_MSGID_OFFSET] = (int) sizeof(PKG_CLI_GG_InGameVerify_ACK);
	m_iPkgSizeArray[CLI_GG_FreshVerify_Req - CLIGGPROTO_MSGID_OFFSET] = (int) sizeof(PKG_CLI_GG_FreshVerify_Req);
	m_iPkgSizeArray[GG_CLI_LoginVerify_REQ - CLIGGPROTO_MSGID_OFFSET] = (int) sizeof(PKG_GG_CLI_LoginVerify_REQ);
	m_iPkgSizeArray[CLI_GG_LoginVerify_ACK - CLIGGPROTO_MSGID_OFFSET] = (int) sizeof(PKG_CLI_GG_LoginVerify_ACK);
	m_iPkgSizeArray[CLI_GG_MERGE_PT_REQ - CLIGGPROTO_MSGID_OFFSET] = (int) sizeof(PKG_CLI_GG_MERGE_PT_REQ);
	m_iPkgSizeArray[CLI_GG_MERGE_PT_ACK - CLIGGPROTO_MSGID_OFFSET] = (int) sizeof(PKG_CLI_GG_MERGE_PT_ACK);
	return true;
}

int EncodeSGroupName(void *pHost, CNetData* poNetData)
{
	SGroupName *pstIn = (SGroupName*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wNameLen))
		return -1;

   if (pstIn->wNameLen > GROUPNAME_LEN)
		return -1;
	if(-1 == poNetData->AddBuf(pstIn->abyName, pstIn->wNameLen))
		return -1;

	return poNetData->GetDataLen();
}

int DecodeSGroupName(void *pHost, CNetData* poNetData)
{
	SGroupName *pstOut = (SGroupName*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wNameLen))
		return -1;

   if (pstOut->wNameLen > GROUPNAME_LEN)
		return -1;
	if(-1 == poNetData->DelBuf(pstOut->abyName, pstOut->wNameLen))
		return -1;

	return sizeof(*pstOut);
}

int EncodePKG_LG_CLI_GroupList_NTF(void *pHost, CNetData* poNetData)
{
	PKG_LG_CLI_GroupList_NTF *pstIn = (PKG_LG_CLI_GroupList_NTF*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byCount))
		return -1;

	int i;
	if ((int)pstIn->byCount > GROUP_MAX_NUM)
       return -1;
	for(i = 0; i < GROUP_MAX_NUM; ++i)
	{
		if(i >= (int)pstIn->byCount)
			break;
		if(-1 == EncodeSGroupName(&pstIn->astNameList[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

int DecodePKG_LG_CLI_GroupList_NTF(void *pHost, CNetData* poNetData)
{
	PKG_LG_CLI_GroupList_NTF *pstOut = (PKG_LG_CLI_GroupList_NTF*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byCount))
		return -1;

	int i;
	if ((int)pstOut->byCount > GROUP_MAX_NUM)
       return -1;
	for(i = 0; i < GROUP_MAX_NUM; ++i)
	{
		if(i >= (int)pstOut->byCount)
			break;
		if(-1 == DecodeSGroupName(&pstOut->astNameList[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

int EncodePKG_CLI_LG_SelGroup_REQ(void *pHost, CNetData* poNetData)
{
	PKG_CLI_LG_SelGroup_REQ *pstIn = (PKG_CLI_LG_SelGroup_REQ*)pHost;

	if(-1 == poNetData->AddString(pstIn->szName, sizeof(pstIn->szName)))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_CLI_LG_SelGroup_REQ(void *pHost, CNetData* poNetData)
{
	PKG_CLI_LG_SelGroup_REQ *pstOut = (PKG_CLI_LG_SelGroup_REQ*)pHost;

	if(-1 == poNetData->DelString(pstOut->szName, sizeof(pstOut->szName)))
		return -1;

	return sizeof(*pstOut);
}

int EncodeUnUSelGroupAck(void *pHost, int iSelect, CNetData* poNetData)
{
	switch(iSelect){
	case PROTO_SUCCESS:
		return EncodeSIPAddr(pHost, poNetData);
	case PROTO_FAILURE:
		return EncodeSErrCode(pHost, poNetData);
	default: return -1;
	}
}

int DecodeUnUSelGroupAck(void *pHost, int iSelect, CNetData* poNetData)
{
	switch(iSelect){
	case PROTO_SUCCESS:
		return DecodeSIPAddr(pHost, poNetData);
	case PROTO_FAILURE:
		return DecodeSErrCode(pHost, poNetData);
	default: return -1;
	}
}

int EncodePKG_LG_CLI_SelGroup_ACK(void *pHost, CNetData* poNetData)
{
	PKG_LG_CLI_SelGroup_ACK *pstIn = (PKG_LG_CLI_SelGroup_ACK*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byResult))
		return -1;

	if(-1 == EncodeUnUSelGroupAck(&pstIn->stReply, pstIn->byResult, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_LG_CLI_SelGroup_ACK(void *pHost, CNetData* poNetData)
{
	PKG_LG_CLI_SelGroup_ACK *pstOut = (PKG_LG_CLI_SelGroup_ACK*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byResult))
		return -1;

	if(-1 == DecodeUnUSelGroupAck(&pstOut->stReply, pstOut->byResult, poNetData))
		return -1;

	return sizeof(*pstOut);
}

int EncodePKG_LG_CLI_SS_INFO(void *pHost, CNetData* poNetData)
{
	PKG_LG_CLI_SS_INFO *pstIn = (PKG_LG_CLI_SS_INFO*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byResult))
		return -1;

	if(-1 == EncodeUnUSelGroupAck(&pstIn->stReply, pstIn->byResult, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_LG_CLI_SS_INFO(void *pHost, CNetData* poNetData)
{
	PKG_LG_CLI_SS_INFO *pstOut = (PKG_LG_CLI_SS_INFO*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byResult))
		return -1;

	if(-1 == DecodeUnUSelGroupAck(&pstOut->stReply, pstOut->byResult, poNetData))
		return -1;

	return sizeof(*pstOut);
}

int EncodePKG_CLI_GG_DynCode_REQ(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_DynCode_REQ *pstIn = (PKG_CLI_GG_DynCode_REQ*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byClientType))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_CLI_GG_DynCode_REQ(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_DynCode_REQ *pstOut = (PKG_CLI_GG_DynCode_REQ*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byClientType))
		return -1;

	return sizeof(*pstOut);
}

int EncodePKG_GG_CLI_DynCode_NTF(void *pHost, CNetData* poNetData)
{
	PKG_GG_CLI_DynCode_NTF *pstIn = (PKG_GG_CLI_DynCode_NTF*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwDataLen))
		return -1;

   if (pstIn->dwDataLen > DYNCODE_BUF_LEN)
		return -1;
	if(-1 == poNetData->AddBuf(pstIn->abyDataBuf, pstIn->dwDataLen))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_GG_CLI_DynCode_NTF(void *pHost, CNetData* poNetData)
{
	PKG_GG_CLI_DynCode_NTF *pstOut = (PKG_GG_CLI_DynCode_NTF*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwDataLen))
		return -1;

   if (pstOut->dwDataLen > DYNCODE_BUF_LEN)
		return -1;
	if(-1 == poNetData->DelBuf(pstOut->abyDataBuf, pstOut->dwDataLen))
		return -1;

	return sizeof(*pstOut);
}

int EncodePKG_CLI_GG_PTLogin_REQ(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_PTLogin_REQ *pstIn = (PKG_CLI_GG_PTLogin_REQ*)pHost;

	if(-1 == poNetData->AddString(pstIn->szPTID, sizeof(pstIn->szPTID)))
		return -1;

	if(-1 == poNetData->AddString(pstIn->szPasswd, sizeof(pstIn->szPasswd)))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwClientVersion))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_CLI_GG_PTLogin_REQ(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_PTLogin_REQ *pstOut = (PKG_CLI_GG_PTLogin_REQ*)pHost;

	if(-1 == poNetData->DelString(pstOut->szPTID, sizeof(pstOut->szPTID)))
		return -1;

	if(-1 == poNetData->DelString(pstOut->szPasswd, sizeof(pstOut->szPasswd)))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwClientVersion))
		return -1;

	return sizeof(*pstOut);
}

int EncodePKG_CLI_GG_SDOALogin_REQ(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_SDOALogin_REQ *pstIn = (PKG_CLI_GG_SDOALogin_REQ*)pHost;

	if(-1 == poNetData->AddString(pstIn->szDigitID, sizeof(pstIn->szDigitID)))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwClientVersion))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wTokenLen))
		return -1;

   if (pstIn->wTokenLen > TOKEN_LEN)
		return -1;
	if(-1 == poNetData->AddBuf(pstIn->abyToken, pstIn->wTokenLen))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_CLI_GG_SDOALogin_REQ(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_SDOALogin_REQ *pstOut = (PKG_CLI_GG_SDOALogin_REQ*)pHost;

	if(-1 == poNetData->DelString(pstOut->szDigitID, sizeof(pstOut->szDigitID)))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwClientVersion))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wTokenLen))
		return -1;

   if (pstOut->wTokenLen > TOKEN_LEN)
		return -1;
	if(-1 == poNetData->DelBuf(pstOut->abyToken, pstOut->wTokenLen))
		return -1;

	return sizeof(*pstOut);
}

int EncodePKG_CLI_GG_MailLogin_REQ(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_MailLogin_REQ *pstIn = (PKG_CLI_GG_MailLogin_REQ*)pHost;

	if(-1 == poNetData->AddString(pstIn->szMailID, sizeof(pstIn->szMailID)))
		return -1;

	if(-1 == poNetData->AddString(pstIn->szPasswd, sizeof(pstIn->szPasswd)))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwClientVersion))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_CLI_GG_MailLogin_REQ(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_MailLogin_REQ *pstOut = (PKG_CLI_GG_MailLogin_REQ*)pHost;

	if(-1 == poNetData->DelString(pstOut->szMailID, sizeof(pstOut->szMailID)))
		return -1;

	if(-1 == poNetData->DelString(pstOut->szPasswd, sizeof(pstOut->szPasswd)))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwClientVersion))
		return -1;

	return sizeof(*pstOut);
}

int EncodePKG_CLI_GG_SpeTokenLogin_REQ(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_SpeTokenLogin_REQ *pstIn = (PKG_CLI_GG_SpeTokenLogin_REQ*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwClientVersion))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wTokenLen))
		return -1;

   if (pstIn->wTokenLen > TOKEN_LEN)
		return -1;
	if(-1 == poNetData->AddBuf(pstIn->abyToken, pstIn->wTokenLen))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_CLI_GG_SpeTokenLogin_REQ(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_SpeTokenLogin_REQ *pstOut = (PKG_CLI_GG_SpeTokenLogin_REQ*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwClientVersion))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wTokenLen))
		return -1;

   if (pstOut->wTokenLen > TOKEN_LEN)
		return -1;
	if(-1 == poNetData->DelBuf(pstOut->abyToken, pstOut->wTokenLen))
		return -1;

	return sizeof(*pstOut);
}

int EncodePKG_CLI_GG_SpePwdLogin_REQ(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_SpePwdLogin_REQ *pstIn = (PKG_CLI_GG_SpePwdLogin_REQ*)pHost;

	if(-1 == poNetData->AddString(pstIn->szAccountID, sizeof(pstIn->szAccountID)))
		return -1;

	if(-1 == poNetData->AddString(pstIn->szPasswd, sizeof(pstIn->szPasswd)))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwClientVersion))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_CLI_GG_SpePwdLogin_REQ(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_SpePwdLogin_REQ *pstOut = (PKG_CLI_GG_SpePwdLogin_REQ*)pHost;

	if(-1 == poNetData->DelString(pstOut->szAccountID, sizeof(pstOut->szAccountID)))
		return -1;

	if(-1 == poNetData->DelString(pstOut->szPasswd, sizeof(pstOut->szPasswd)))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwClientVersion))
		return -1;

	return sizeof(*pstOut);
}

int EncodePKG_CLI_GG_APLogin_REQ(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_APLogin_REQ *pstIn = (PKG_CLI_GG_APLogin_REQ*)pHost;

	if(-1 == poNetData->AddString(pstIn->szAuthAct, sizeof(pstIn->szAuthAct)))
		return -1;

	if(-1 == poNetData->AddString(pstIn->szAuthStr, sizeof(pstIn->szAuthStr)))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwClientVersion))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_CLI_GG_APLogin_REQ(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_APLogin_REQ *pstOut = (PKG_CLI_GG_APLogin_REQ*)pHost;

	if(-1 == poNetData->DelString(pstOut->szAuthAct, sizeof(pstOut->szAuthAct)))
		return -1;

	if(-1 == poNetData->DelString(pstOut->szAuthStr, sizeof(pstOut->szAuthStr)))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwClientVersion))
		return -1;

	return sizeof(*pstOut);
}

int EncodePKG_CLI_GG_EXPLogin_REQ(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_EXPLogin_REQ *pstIn = (PKG_CLI_GG_EXPLogin_REQ*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwClientVersion))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_CLI_GG_EXPLogin_REQ(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_EXPLogin_REQ *pstOut = (PKG_CLI_GG_EXPLogin_REQ*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwClientVersion))
		return -1;

	return sizeof(*pstOut);
}

int EncodePKG_CLI_GG_SGILogin_REQ(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_SGILogin_REQ *pstIn = (PKG_CLI_GG_SGILogin_REQ*)pHost;

	if(-1 == poNetData->AddString(pstIn->szPlatform, sizeof(pstIn->szPlatform)))
		return -1;

	if(-1 == poNetData->AddString(pstIn->szAuthCode, sizeof(pstIn->szAuthCode)))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwClientVersion))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_CLI_GG_SGILogin_REQ(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_SGILogin_REQ *pstOut = (PKG_CLI_GG_SGILogin_REQ*)pHost;

	if(-1 == poNetData->DelString(pstOut->szPlatform, sizeof(pstOut->szPlatform)))
		return -1;

	if(-1 == poNetData->DelString(pstOut->szAuthCode, sizeof(pstOut->szAuthCode)))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwClientVersion))
		return -1;

	return sizeof(*pstOut);
}

int EncodeUnLoginReqInfo(void *pHost, int iSelect, CNetData* poNetData)
{
	switch(iSelect){
	case LOGIN_PTSDK:
		return EncodePKG_CLI_GG_PTLogin_REQ(pHost, poNetData);
	case LOGIN_SDOA:
		return EncodePKG_CLI_GG_SDOALogin_REQ(pHost, poNetData);
	case LOGIN_MAIL:
		return EncodePKG_CLI_GG_MailLogin_REQ(pHost, poNetData);
	case LOGIN_SPETK:
		return EncodePKG_CLI_GG_SpeTokenLogin_REQ(pHost, poNetData);
	case LOGIN_SPEPWD:
		return EncodePKG_CLI_GG_SpePwdLogin_REQ(pHost, poNetData);
	case LOGIN_AP:
		return EncodePKG_CLI_GG_APLogin_REQ(pHost, poNetData);
	case LOGIN_EXP:
		return EncodePKG_CLI_GG_EXPLogin_REQ(pHost, poNetData);
	case LOGIN_SGI:
		return EncodePKG_CLI_GG_SGILogin_REQ(pHost, poNetData);
	default: return -1;
	}
}

int DecodeUnLoginReqInfo(void *pHost, int iSelect, CNetData* poNetData)
{
	switch(iSelect){
	case LOGIN_PTSDK:
		return DecodePKG_CLI_GG_PTLogin_REQ(pHost, poNetData);
	case LOGIN_SDOA:
		return DecodePKG_CLI_GG_SDOALogin_REQ(pHost, poNetData);
	case LOGIN_MAIL:
		return DecodePKG_CLI_GG_MailLogin_REQ(pHost, poNetData);
	case LOGIN_SPETK:
		return DecodePKG_CLI_GG_SpeTokenLogin_REQ(pHost, poNetData);
	case LOGIN_SPEPWD:
		return DecodePKG_CLI_GG_SpePwdLogin_REQ(pHost, poNetData);
	case LOGIN_AP:
		return DecodePKG_CLI_GG_APLogin_REQ(pHost, poNetData);
	case LOGIN_EXP:
		return DecodePKG_CLI_GG_EXPLogin_REQ(pHost, poNetData);
	case LOGIN_SGI:
		return DecodePKG_CLI_GG_SGILogin_REQ(pHost, poNetData);
	default: return -1;
	}
}

int EncodePKG_CLI_GG_Login_REQ(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_Login_REQ *pstIn = (PKG_CLI_GG_Login_REQ*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byType))
		return -1;

	if(-1 == EncodeUnLoginReqInfo(&pstIn->stInfo, pstIn->byType, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_CLI_GG_Login_REQ(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_Login_REQ *pstOut = (PKG_CLI_GG_Login_REQ*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byType))
		return -1;

	if(-1 == DecodeUnLoginReqInfo(&pstOut->stInfo, pstOut->byType, poNetData))
		return -1;

	return sizeof(*pstOut);
}

int EncodePKG_GG_CLI_Challenge_REQ(void *pHost, CNetData* poNetData)
{
	PKG_GG_CLI_Challenge_REQ *pstIn = (PKG_GG_CLI_Challenge_REQ*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byType))
		return -1;

	if(-1 == poNetData->AddString(pstIn->szChallenge, sizeof(pstIn->szChallenge)))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_GG_CLI_Challenge_REQ(void *pHost, CNetData* poNetData)
{
	PKG_GG_CLI_Challenge_REQ *pstOut = (PKG_GG_CLI_Challenge_REQ*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byType))
		return -1;

	if(-1 == poNetData->DelString(pstOut->szChallenge, sizeof(pstOut->szChallenge)))
		return -1;

	return sizeof(*pstOut);
}

int EncodePKG_CLI_GG_Challenge_ACK(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_Challenge_ACK *pstIn = (PKG_CLI_GG_Challenge_ACK*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byType))
		return -1;

	if(-1 == poNetData->AddString(pstIn->szDPWD, sizeof(pstIn->szDPWD)))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_CLI_GG_Challenge_ACK(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_Challenge_ACK *pstOut = (PKG_CLI_GG_Challenge_ACK*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byType))
		return -1;

	if(-1 == poNetData->DelString(pstOut->szDPWD, sizeof(pstOut->szDPWD)))
		return -1;

	return sizeof(*pstOut);
}

int EncodePKG_GG_CLI_Login_SUC(void *pHost, CNetData* poNetData)
{
	PKG_GG_CLI_Login_SUC *pstIn = (PKG_GG_CLI_Login_SUC*)pHost;

	if(-1 == poNetData->AddUint64(pstIn->qwDigitID))
		return -1;

	if(-1 == EncodeLoginData(&pstIn->stLoginData, poNetData))
		return -1;

	if(-1 == EncodeSRoleList(&pstIn->stRoleList, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_GG_CLI_Login_SUC(void *pHost, CNetData* poNetData)
{
	PKG_GG_CLI_Login_SUC *pstOut = (PKG_GG_CLI_Login_SUC*)pHost;

	if(-1 == poNetData->DelUint64(pstOut->qwDigitID))
		return -1;

	if(-1 == DecodeLoginData(&pstOut->stLoginData, poNetData))
		return -1;

	if(-1 == DecodeSRoleList(&pstOut->stRoleList, poNetData))
		return -1;

	return sizeof(*pstOut);
}

int EncodePKG_GG_CLI_APLogin_RES(void *pHost, CNetData* poNetData)
{
	PKG_GG_CLI_APLogin_RES *pstIn = (PKG_GG_CLI_APLogin_RES*)pHost;

	if(-1 == poNetData->AddInt(pstIn->nResult))
		return -1;

	if(-1 == poNetData->AddString(pstIn->szAuthAct, sizeof(pstIn->szAuthAct)))
		return -1;

	if(-1 == poNetData->AddString(pstIn->szAuthResStr, sizeof(pstIn->szAuthResStr)))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwDigitID))
		return -1;

	if(-1 == EncodeLoginData(&pstIn->stLoginData, poNetData))
		return -1;

	if(-1 == EncodeSRoleList(&pstIn->stRoleList, poNetData))
		return -1;

	if(-1 == poNetData->AddString(pstIn->szchannel, sizeof(pstIn->szchannel)))
		return -1;

	if(-1 == poNetData->AddString(pstIn->szaccount, sizeof(pstIn->szaccount)))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_GG_CLI_APLogin_RES(void *pHost, CNetData* poNetData)
{
	PKG_GG_CLI_APLogin_RES *pstOut = (PKG_GG_CLI_APLogin_RES*)pHost;

	if(-1 == poNetData->DelInt(pstOut->nResult))
		return -1;

	if(-1 == poNetData->DelString(pstOut->szAuthAct, sizeof(pstOut->szAuthAct)))
		return -1;

	if(-1 == poNetData->DelString(pstOut->szAuthResStr, sizeof(pstOut->szAuthResStr)))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwDigitID))
		return -1;

	if(-1 == DecodeLoginData(&pstOut->stLoginData, poNetData))
		return -1;

	if(-1 == DecodeSRoleList(&pstOut->stRoleList, poNetData))
		return -1;

	if(-1 == poNetData->DelString(pstOut->szchannel, sizeof(pstOut->szchannel)))
		return -1;

	if(-1 == poNetData->DelString(pstOut->szaccount, sizeof(pstOut->szaccount)))
		return -1;

	return sizeof(*pstOut);
}

int EncodePKG_GG_CLI_EXPLogin_RES(void *pHost, CNetData* poNetData)
{
	PKG_GG_CLI_EXPLogin_RES *pstIn = (PKG_GG_CLI_EXPLogin_RES*)pHost;

	if(-1 == poNetData->AddInt(pstIn->nResult))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwDigitID))
		return -1;

	if(-1 == EncodeLoginData(&pstIn->stLoginData, poNetData))
		return -1;

	if(-1 == EncodeSRoleList(&pstIn->stRoleList, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_GG_CLI_EXPLogin_RES(void *pHost, CNetData* poNetData)
{
	PKG_GG_CLI_EXPLogin_RES *pstOut = (PKG_GG_CLI_EXPLogin_RES*)pHost;

	if(-1 == poNetData->DelInt(pstOut->nResult))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwDigitID))
		return -1;

	if(-1 == DecodeLoginData(&pstOut->stLoginData, poNetData))
		return -1;

	if(-1 == DecodeSRoleList(&pstOut->stRoleList, poNetData))
		return -1;

	return sizeof(*pstOut);
}

int EncodePKG_GG_CLI_SGILogin_RES(void *pHost, CNetData* poNetData)
{
	PKG_GG_CLI_SGILogin_RES *pstIn = (PKG_GG_CLI_SGILogin_RES*)pHost;

	if(-1 == poNetData->AddInt(pstIn->nResult))
		return -1;

	if(-1 == poNetData->AddUint64(pstIn->qwDigitID))
		return -1;

	if(-1 == EncodeLoginData(&pstIn->stLoginData, poNetData))
		return -1;

	if(-1 == EncodeSRoleList(&pstIn->stRoleList, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_GG_CLI_SGILogin_RES(void *pHost, CNetData* poNetData)
{
	PKG_GG_CLI_SGILogin_RES *pstOut = (PKG_GG_CLI_SGILogin_RES*)pHost;

	if(-1 == poNetData->DelInt(pstOut->nResult))
		return -1;

	if(-1 == poNetData->DelUint64(pstOut->qwDigitID))
		return -1;

	if(-1 == DecodeLoginData(&pstOut->stLoginData, poNetData))
		return -1;

	if(-1 == DecodeSRoleList(&pstOut->stRoleList, poNetData))
		return -1;

	return sizeof(*pstOut);
}

int EncodeUnPKG_GG_CLI_Login_UN(void *pHost, int iSelect, CNetData* poNetData)
{
	switch(iSelect){
	case PROTO_SUCCESS:
		return EncodePKG_GG_CLI_Login_SUC(pHost, poNetData);
	case PROTO_FAILURE:
		return EncodeSErrCode(pHost, poNetData);
	case PROTO_AP_RES:
		return EncodePKG_GG_CLI_APLogin_RES(pHost, poNetData);
	case PROTO_EXP_RES:
		return EncodePKG_GG_CLI_EXPLogin_RES(pHost, poNetData);
	case PROTO_SGI_RES:
		return EncodePKG_GG_CLI_SGILogin_RES(pHost, poNetData);
	default: return -1;
	}
}

int DecodeUnPKG_GG_CLI_Login_UN(void *pHost, int iSelect, CNetData* poNetData)
{
	switch(iSelect){
	case PROTO_SUCCESS:
		return DecodePKG_GG_CLI_Login_SUC(pHost, poNetData);
	case PROTO_FAILURE:
		return DecodeSErrCode(pHost, poNetData);
	case PROTO_AP_RES:
		return DecodePKG_GG_CLI_APLogin_RES(pHost, poNetData);
	case PROTO_EXP_RES:
		return DecodePKG_GG_CLI_EXPLogin_RES(pHost, poNetData);
	case PROTO_SGI_RES:
		return DecodePKG_GG_CLI_SGILogin_RES(pHost, poNetData);
	default: return -1;
	}
}

int EncodePKG_GG_CLI_Login_ACK(void *pHost, CNetData* poNetData)
{
	PKG_GG_CLI_Login_ACK *pstIn = (PKG_GG_CLI_Login_ACK*)pHost;

	if(-1 == poNetData->AddString(pstIn->szPTID, sizeof(pstIn->szPTID)))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byResult))
		return -1;

	if(-1 == EncodeUnPKG_GG_CLI_Login_UN(&pstIn->stReply, pstIn->byResult, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_GG_CLI_Login_ACK(void *pHost, CNetData* poNetData)
{
	PKG_GG_CLI_Login_ACK *pstOut = (PKG_GG_CLI_Login_ACK*)pHost;

	if(-1 == poNetData->DelString(pstOut->szPTID, sizeof(pstOut->szPTID)))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byResult))
		return -1;

	if(-1 == DecodeUnPKG_GG_CLI_Login_UN(&pstOut->stReply, pstOut->byResult, poNetData))
		return -1;

	return sizeof(*pstOut);
}

int EncodePKG_GG_CLI_Verify_REQ(void *pHost, CNetData* poNetData)
{
	PKG_GG_CLI_Verify_REQ *pstIn = (PKG_GG_CLI_Verify_REQ*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wPicLen))
		return -1;

   if (pstIn->wPicLen > VERIFY_PIC_LEN)
		return -1;
	if(-1 == poNetData->AddBuf(pstIn->abyPicture, pstIn->wPicLen))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_GG_CLI_Verify_REQ(void *pHost, CNetData* poNetData)
{
	PKG_GG_CLI_Verify_REQ *pstOut = (PKG_GG_CLI_Verify_REQ*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wPicLen))
		return -1;

   if (pstOut->wPicLen > VERIFY_PIC_LEN)
		return -1;
	if(-1 == poNetData->DelBuf(pstOut->abyPicture, pstOut->wPicLen))
		return -1;

	return sizeof(*pstOut);
}

int EncodePKG_CLI_GG_Verify_ACK(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_Verify_ACK *pstIn = (PKG_CLI_GG_Verify_ACK*)pHost;

	if(-1 == poNetData->AddString(pstIn->szAnswer, sizeof(pstIn->szAnswer)))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_CLI_GG_Verify_ACK(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_Verify_ACK *pstOut = (PKG_CLI_GG_Verify_ACK*)pHost;

	if(-1 == poNetData->DelString(pstOut->szAnswer, sizeof(pstOut->szAnswer)))
		return -1;

	return sizeof(*pstOut);
}

int EncodePKG_CLI_GG_SelectRole_REQ(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_SelectRole_REQ *pstIn = (PKG_CLI_GG_SelectRole_REQ*)pHost;

	if(-1 == poNetData->AddString(pstIn->szPTID, sizeof(pstIn->szPTID)))
		return -1;

	if(-1 == poNetData->AddString(pstIn->szRoleName, sizeof(pstIn->szRoleName)))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_CLI_GG_SelectRole_REQ(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_SelectRole_REQ *pstOut = (PKG_CLI_GG_SelectRole_REQ*)pHost;

	if(-1 == poNetData->DelString(pstOut->szPTID, sizeof(pstOut->szPTID)))
		return -1;

	if(-1 == poNetData->DelString(pstOut->szRoleName, sizeof(pstOut->szRoleName)))
		return -1;

	return sizeof(*pstOut);
}

int EncodeSelRoleRet(void *pHost, CNetData* poNetData)
{
	SelRoleRet *pstIn = (SelRoleRet*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwNPC))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byServerNo))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wAreaNo))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wMsgLen))
		return -1;

   if (pstIn->wMsgLen > ONLINEMSG_LEN)
		return -1;
	if(-1 == poNetData->AddBuf(pstIn->abyMsgData, pstIn->wMsgLen))
		return -1;

	return poNetData->GetDataLen();
}

int DecodeSelRoleRet(void *pHost, CNetData* poNetData)
{
	SelRoleRet *pstOut = (SelRoleRet*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwNPC))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byServerNo))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wAreaNo))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wMsgLen))
		return -1;

   if (pstOut->wMsgLen > ONLINEMSG_LEN)
		return -1;
	if(-1 == poNetData->DelBuf(pstOut->abyMsgData, pstOut->wMsgLen))
		return -1;

	return sizeof(*pstOut);
}

int EncodeUnPKG_GG_CLI_SelectRole_UN(void *pHost, int iSelect, CNetData* poNetData)
{
	switch(iSelect){
	case PROTO_SUCCESS:
		return EncodeSelRoleRet(pHost, poNetData);
	case PROTO_FAILURE:
		return EncodeSErrCode(pHost, poNetData);
	default: return -1;
	}
}

int DecodeUnPKG_GG_CLI_SelectRole_UN(void *pHost, int iSelect, CNetData* poNetData)
{
	switch(iSelect){
	case PROTO_SUCCESS:
		return DecodeSelRoleRet(pHost, poNetData);
	case PROTO_FAILURE:
		return DecodeSErrCode(pHost, poNetData);
	default: return -1;
	}
}

int EncodePKG_GG_CLI_SelectRole_ACK(void *pHost, CNetData* poNetData)
{
	PKG_GG_CLI_SelectRole_ACK *pstIn = (PKG_GG_CLI_SelectRole_ACK*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byResult))
		return -1;

	if(-1 == EncodeUnPKG_GG_CLI_SelectRole_UN(&pstIn->stReply, pstIn->byResult, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_GG_CLI_SelectRole_ACK(void *pHost, CNetData* poNetData)
{
	PKG_GG_CLI_SelectRole_ACK *pstOut = (PKG_GG_CLI_SelectRole_ACK*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byResult))
		return -1;

	if(-1 == DecodeUnPKG_GG_CLI_SelectRole_UN(&pstOut->stReply, pstOut->byResult, poNetData))
		return -1;

	return sizeof(*pstOut);
}

int EncodePKG_CLI_GG_CreateRole_REQ(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_CreateRole_REQ *pstIn = (PKG_CLI_GG_CreateRole_REQ*)pHost;

	if(-1 == EncodeSCreateRoleInfo(&pstIn->stInfo, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_CLI_GG_CreateRole_REQ(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_CreateRole_REQ *pstOut = (PKG_CLI_GG_CreateRole_REQ*)pHost;

	if(-1 == DecodeSCreateRoleInfo(&pstOut->stInfo, poNetData))
		return -1;

	return sizeof(*pstOut);
}

int EncodePKG_GG_CLI_CreateRole_ACK(void *pHost, CNetData* poNetData)
{
	PKG_GG_CLI_CreateRole_ACK *pstIn = (PKG_GG_CLI_CreateRole_ACK*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byResult))
		return -1;

	if(-1 == EncodeUnURoleOpAck(&pstIn->stReply, pstIn->byResult, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_GG_CLI_CreateRole_ACK(void *pHost, CNetData* poNetData)
{
	PKG_GG_CLI_CreateRole_ACK *pstOut = (PKG_GG_CLI_CreateRole_ACK*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byResult))
		return -1;

	if(-1 == DecodeUnURoleOpAck(&pstOut->stReply, pstOut->byResult, poNetData))
		return -1;

	return sizeof(*pstOut);
}

int EncodePKG_CLI_GG_DelRole_REQ(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_DelRole_REQ *pstIn = (PKG_CLI_GG_DelRole_REQ*)pHost;

	if(-1 == poNetData->AddString(pstIn->szPTID, sizeof(pstIn->szPTID)))
		return -1;

	if(-1 == poNetData->AddString(pstIn->szRoleName, sizeof(pstIn->szRoleName)))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_CLI_GG_DelRole_REQ(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_DelRole_REQ *pstOut = (PKG_CLI_GG_DelRole_REQ*)pHost;

	if(-1 == poNetData->DelString(pstOut->szPTID, sizeof(pstOut->szPTID)))
		return -1;

	if(-1 == poNetData->DelString(pstOut->szRoleName, sizeof(pstOut->szRoleName)))
		return -1;

	return sizeof(*pstOut);
}

int EncodePKG_GG_CLI_DelRole_ACK(void *pHost, CNetData* poNetData)
{
	PKG_GG_CLI_DelRole_ACK *pstIn = (PKG_GG_CLI_DelRole_ACK*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byResult))
		return -1;

	if(-1 == EncodeSErrCode(&pstIn->stReason, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_GG_CLI_DelRole_ACK(void *pHost, CNetData* poNetData)
{
	PKG_GG_CLI_DelRole_ACK *pstOut = (PKG_GG_CLI_DelRole_ACK*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byResult))
		return -1;

	if(-1 == DecodeSErrCode(&pstOut->stReason, poNetData))
		return -1;

	return sizeof(*pstOut);
}

int EncodePKG_CLI_GG_UnDelRole_REQ(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_UnDelRole_REQ *pstIn = (PKG_CLI_GG_UnDelRole_REQ*)pHost;

	if(-1 == poNetData->AddString(pstIn->szPTID, sizeof(pstIn->szPTID)))
		return -1;

	if(-1 == poNetData->AddString(pstIn->szRoleName, sizeof(pstIn->szRoleName)))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_CLI_GG_UnDelRole_REQ(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_UnDelRole_REQ *pstOut = (PKG_CLI_GG_UnDelRole_REQ*)pHost;

	if(-1 == poNetData->DelString(pstOut->szPTID, sizeof(pstOut->szPTID)))
		return -1;

	if(-1 == poNetData->DelString(pstOut->szRoleName, sizeof(pstOut->szRoleName)))
		return -1;

	return sizeof(*pstOut);
}

int EncodePKG_GG_CLI_UnDelRole_ACK(void *pHost, CNetData* poNetData)
{
	PKG_GG_CLI_UnDelRole_ACK *pstIn = (PKG_GG_CLI_UnDelRole_ACK*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byResult))
		return -1;

	if(-1 == EncodeSErrCode(&pstIn->stReason, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_GG_CLI_UnDelRole_ACK(void *pHost, CNetData* poNetData)
{
	PKG_GG_CLI_UnDelRole_ACK *pstOut = (PKG_GG_CLI_UnDelRole_ACK*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byResult))
		return -1;

	if(-1 == DecodeSErrCode(&pstOut->stReason, poNetData))
		return -1;

	return sizeof(*pstOut);
}

int EncodePKG_CLI_GG_Logout_NTF(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_Logout_NTF *pstIn = (PKG_CLI_GG_Logout_NTF*)pHost;

	if(-1 == poNetData->AddString(pstIn->szPTID, sizeof(pstIn->szPTID)))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_CLI_GG_Logout_NTF(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_Logout_NTF *pstOut = (PKG_CLI_GG_Logout_NTF*)pHost;

	if(-1 == poNetData->DelString(pstOut->szPTID, sizeof(pstOut->szPTID)))
		return -1;

	return sizeof(*pstOut);
}

int EncodePKG_CLI_GG_ReselRole_REQ(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_ReselRole_REQ *pstIn = (PKG_CLI_GG_ReselRole_REQ*)pHost;

	if(-1 == poNetData->AddString(pstIn->szPTID, sizeof(pstIn->szPTID)))
		return -1;

	if(-1 == poNetData->AddString(pstIn->szRoleName, sizeof(pstIn->szRoleName)))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_CLI_GG_ReselRole_REQ(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_ReselRole_REQ *pstOut = (PKG_CLI_GG_ReselRole_REQ*)pHost;

	if(-1 == poNetData->DelString(pstOut->szPTID, sizeof(pstOut->szPTID)))
		return -1;

	if(-1 == poNetData->DelString(pstOut->szRoleName, sizeof(pstOut->szRoleName)))
		return -1;

	return sizeof(*pstOut);
}

int EncodeUnPKG_GG_CLI_ReselRole_UN(void *pHost, int iSelect, CNetData* poNetData)
{
	switch(iSelect){
	case PROTO_SUCCESS:
		return EncodeSRoleList(pHost, poNetData);
	case PROTO_FAILURE:
		return EncodeSErrCode(pHost, poNetData);
	default: return -1;
	}
}

int DecodeUnPKG_GG_CLI_ReselRole_UN(void *pHost, int iSelect, CNetData* poNetData)
{
	switch(iSelect){
	case PROTO_SUCCESS:
		return DecodeSRoleList(pHost, poNetData);
	case PROTO_FAILURE:
		return DecodeSErrCode(pHost, poNetData);
	default: return -1;
	}
}

int EncodePKG_GG_CLI_ReselRole_ACK(void *pHost, CNetData* poNetData)
{
	PKG_GG_CLI_ReselRole_ACK *pstIn = (PKG_GG_CLI_ReselRole_ACK*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byResult))
		return -1;

	if(-1 == EncodeUnPKG_GG_CLI_ReselRole_UN(&pstIn->stReply, pstIn->byResult, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_GG_CLI_ReselRole_ACK(void *pHost, CNetData* poNetData)
{
	PKG_GG_CLI_ReselRole_ACK *pstOut = (PKG_GG_CLI_ReselRole_ACK*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byResult))
		return -1;

	if(-1 == DecodeUnPKG_GG_CLI_ReselRole_UN(&pstOut->stReply, pstOut->byResult, poNetData))
		return -1;

	return sizeof(*pstOut);
}

int EncodePKG_GG_CLI_KickOff_NTF(void *pHost, CNetData* poNetData)
{
	PKG_GG_CLI_KickOff_NTF *pstIn = (PKG_GG_CLI_KickOff_NTF*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wReason))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_GG_CLI_KickOff_NTF(void *pHost, CNetData* poNetData)
{
	PKG_GG_CLI_KickOff_NTF *pstOut = (PKG_GG_CLI_KickOff_NTF*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wReason))
		return -1;

	return sizeof(*pstOut);
}

int EncodePKG_CLI_GG_VerifyStoreEkey_ACK(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_VerifyStoreEkey_ACK *pstIn = (PKG_CLI_GG_VerifyStoreEkey_ACK*)pHost;

	if(-1 == poNetData->AddString(pstIn->szPTID, sizeof(pstIn->szPTID)))
		return -1;

	if(-1 == poNetData->AddString(pstIn->szRoleName, sizeof(pstIn->szRoleName)))
		return -1;

	if(-1 == poNetData->AddString(pstIn->szDPWD, sizeof(pstIn->szDPWD)))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_CLI_GG_VerifyStoreEkey_ACK(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_VerifyStoreEkey_ACK *pstOut = (PKG_CLI_GG_VerifyStoreEkey_ACK*)pHost;

	if(-1 == poNetData->DelString(pstOut->szPTID, sizeof(pstOut->szPTID)))
		return -1;

	if(-1 == poNetData->DelString(pstOut->szRoleName, sizeof(pstOut->szRoleName)))
		return -1;

	if(-1 == poNetData->DelString(pstOut->szDPWD, sizeof(pstOut->szDPWD)))
		return -1;

	return sizeof(*pstOut);
}

int EncodePKG_CLI_GG_FCMFillInfo_REQ(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_FCMFillInfo_REQ *pstIn = (PKG_CLI_GG_FCMFillInfo_REQ*)pHost;

	if(-1 == poNetData->AddString(pstIn->szPTID, sizeof(pstIn->szPTID)))
		return -1;

	if(-1 == poNetData->AddString(pstIn->szName, sizeof(pstIn->szName)))
		return -1;

	if(-1 == poNetData->AddString(pstIn->szIDCard, sizeof(pstIn->szIDCard)))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byFCMFlag))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_CLI_GG_FCMFillInfo_REQ(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_FCMFillInfo_REQ *pstOut = (PKG_CLI_GG_FCMFillInfo_REQ*)pHost;

	if(-1 == poNetData->DelString(pstOut->szPTID, sizeof(pstOut->szPTID)))
		return -1;

	if(-1 == poNetData->DelString(pstOut->szName, sizeof(pstOut->szName)))
		return -1;

	if(-1 == poNetData->DelString(pstOut->szIDCard, sizeof(pstOut->szIDCard)))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byFCMFlag))
		return -1;

	return sizeof(*pstOut);
}

int EncodePKG_GG_CLI_ReservePwd_REQ(void *pHost, CNetData* poNetData)
{
	PKG_GG_CLI_ReservePwd_REQ *pstIn = (PKG_GG_CLI_ReservePwd_REQ*)pHost;

	return poNetData->GetDataLen();
}

int DecodePKG_GG_CLI_ReservePwd_REQ(void *pHost, CNetData* poNetData)
{
	PKG_GG_CLI_ReservePwd_REQ *pstOut = (PKG_GG_CLI_ReservePwd_REQ*)pHost;

	return sizeof(*pstOut);
}

int EncodePKG_CLI_GG_ReservePwd_ACK(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_ReservePwd_ACK *pstIn = (PKG_CLI_GG_ReservePwd_ACK*)pHost;

	if(-1 == poNetData->AddString(pstIn->szPTID, sizeof(pstIn->szPTID)))
		return -1;

	if(-1 == poNetData->AddString(pstIn->szRoleName, sizeof(pstIn->szRoleName)))
		return -1;

	if(-1 == poNetData->AddString(pstIn->szDPWD, sizeof(pstIn->szDPWD)))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_CLI_GG_ReservePwd_ACK(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_ReservePwd_ACK *pstOut = (PKG_CLI_GG_ReservePwd_ACK*)pHost;

	if(-1 == poNetData->DelString(pstOut->szPTID, sizeof(pstOut->szPTID)))
		return -1;

	if(-1 == poNetData->DelString(pstOut->szRoleName, sizeof(pstOut->szRoleName)))
		return -1;

	if(-1 == poNetData->DelString(pstOut->szDPWD, sizeof(pstOut->szDPWD)))
		return -1;

	return sizeof(*pstOut);
}

int EncodePKG_CLI_GG_Extend_MSG(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_Extend_MSG *pstIn = (PKG_CLI_GG_Extend_MSG*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwExtendID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wMsgLen))
		return -1;

   if (pstIn->wMsgLen > LOGINMSG_LEN)
		return -1;
	if(-1 == poNetData->AddBuf(pstIn->abyMsgData, pstIn->wMsgLen))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_CLI_GG_Extend_MSG(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_Extend_MSG *pstOut = (PKG_CLI_GG_Extend_MSG*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwExtendID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wMsgLen))
		return -1;

   if (pstOut->wMsgLen > LOGINMSG_LEN)
		return -1;
	if(-1 == poNetData->DelBuf(pstOut->abyMsgData, pstOut->wMsgLen))
		return -1;

	return sizeof(*pstOut);
}

int EncodePKG_GG_CLI_Extend_MSG(void *pHost, CNetData* poNetData)
{
	PKG_GG_CLI_Extend_MSG *pstIn = (PKG_GG_CLI_Extend_MSG*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwExtendID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wMsgLen))
		return -1;

   if (pstIn->wMsgLen > LOGINMSG_LEN)
		return -1;
	if(-1 == poNetData->AddBuf(pstIn->abyMsgData, pstIn->wMsgLen))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_GG_CLI_Extend_MSG(void *pHost, CNetData* poNetData)
{
	PKG_GG_CLI_Extend_MSG *pstOut = (PKG_GG_CLI_Extend_MSG*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwExtendID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wMsgLen))
		return -1;

   if (pstOut->wMsgLen > LOGINMSG_LEN)
		return -1;
	if(-1 == poNetData->DelBuf(pstOut->abyMsgData, pstOut->wMsgLen))
		return -1;

	return sizeof(*pstOut);
}

int EncodePKG_GG_CLI_GroupInfo_NTF(void *pHost, CNetData* poNetData)
{
	PKG_GG_CLI_GroupInfo_NTF *pstIn = (PKG_GG_CLI_GroupInfo_NTF*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wAreaId))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wGroupId))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_GG_CLI_GroupInfo_NTF(void *pHost, CNetData* poNetData)
{
	PKG_GG_CLI_GroupInfo_NTF *pstOut = (PKG_GG_CLI_GroupInfo_NTF*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wAreaId))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wGroupId))
		return -1;

	return sizeof(*pstOut);
}

int EncodePKG_CLI_GG_OfflineTrustee_NTF(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_OfflineTrustee_NTF *pstIn = (PKG_CLI_GG_OfflineTrustee_NTF*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byTrusteeMode))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_CLI_GG_OfflineTrustee_NTF(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_OfflineTrustee_NTF *pstOut = (PKG_CLI_GG_OfflineTrustee_NTF*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byTrusteeMode))
		return -1;

	return sizeof(*pstOut);
}

int EncodePKG_GG_CLI_NeedAuthen_REQ(void *pHost, CNetData* poNetData)
{
	PKG_GG_CLI_NeedAuthen_REQ *pstIn = (PKG_GG_CLI_NeedAuthen_REQ*)pHost;

	if(-1 == poNetData->AddString(pstIn->szAuthenURL, sizeof(pstIn->szAuthenURL)))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_GG_CLI_NeedAuthen_REQ(void *pHost, CNetData* poNetData)
{
	PKG_GG_CLI_NeedAuthen_REQ *pstOut = (PKG_GG_CLI_NeedAuthen_REQ*)pHost;

	if(-1 == poNetData->DelString(pstOut->szAuthenURL, sizeof(pstOut->szAuthenURL)))
		return -1;

	return sizeof(*pstOut);
}

int EncodePKG_CLI_GG_NeedAuthen_ACK(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_NeedAuthen_ACK *pstIn = (PKG_CLI_GG_NeedAuthen_ACK*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byReserve))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_CLI_GG_NeedAuthen_ACK(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_NeedAuthen_ACK *pstOut = (PKG_CLI_GG_NeedAuthen_ACK*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byReserve))
		return -1;

	return sizeof(*pstOut);
}

int EncodePKG_GG_CLI_QQInfo_NTF(void *pHost, CNetData* poNetData)
{
	PKG_GG_CLI_QQInfo_NTF *pstIn = (PKG_GG_CLI_QQInfo_NTF*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wAreaId))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wGroupId))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byOA))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byQQ2))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byTrustee))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_GG_CLI_QQInfo_NTF(void *pHost, CNetData* poNetData)
{
	PKG_GG_CLI_QQInfo_NTF *pstOut = (PKG_GG_CLI_QQInfo_NTF*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wAreaId))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wGroupId))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byOA))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byQQ2))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byTrustee))
		return -1;

	return sizeof(*pstOut);
}

int EncodePKG_GG_CLI_InGameVerify_REQ(void *pHost, CNetData* poNetData)
{
	PKG_GG_CLI_InGameVerify_REQ *pstIn = (PKG_GG_CLI_InGameVerify_REQ*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wPopupTime))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wTotalTime))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wPicLen))
		return -1;

   if (pstIn->wPicLen > VERIFY_PIC_LEN)
		return -1;
	if(-1 == poNetData->AddBuf(pstIn->abyPicture, pstIn->wPicLen))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_GG_CLI_InGameVerify_REQ(void *pHost, CNetData* poNetData)
{
	PKG_GG_CLI_InGameVerify_REQ *pstOut = (PKG_GG_CLI_InGameVerify_REQ*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wPopupTime))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wTotalTime))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wPicLen))
		return -1;

   if (pstOut->wPicLen > VERIFY_PIC_LEN)
		return -1;
	if(-1 == poNetData->DelBuf(pstOut->abyPicture, pstOut->wPicLen))
		return -1;

	return sizeof(*pstOut);
}

int EncodePKG_CLI_GG_InGameVerify_ACK(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_InGameVerify_ACK *pstIn = (PKG_CLI_GG_InGameVerify_ACK*)pHost;

	if(-1 == poNetData->AddString(pstIn->szAnswer, sizeof(pstIn->szAnswer)))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_CLI_GG_InGameVerify_ACK(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_InGameVerify_ACK *pstOut = (PKG_CLI_GG_InGameVerify_ACK*)pHost;

	if(-1 == poNetData->DelString(pstOut->szAnswer, sizeof(pstOut->szAnswer)))
		return -1;

	return sizeof(*pstOut);
}

int EncodePKG_CLI_GG_FreshVerify_Req(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_FreshVerify_Req *pstIn = (PKG_CLI_GG_FreshVerify_Req*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byReserve))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_CLI_GG_FreshVerify_Req(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_FreshVerify_Req *pstOut = (PKG_CLI_GG_FreshVerify_Req*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byReserve))
		return -1;

	return sizeof(*pstOut);
}

int EncodePKG_GG_CLI_LoginVerify_REQ(void *pHost, CNetData* poNetData)
{
	PKG_GG_CLI_LoginVerify_REQ *pstIn = (PKG_GG_CLI_LoginVerify_REQ*)pHost;

	if(-1 == poNetData->AddString(pstIn->szVerifyURL, sizeof(pstIn->szVerifyURL)))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_GG_CLI_LoginVerify_REQ(void *pHost, CNetData* poNetData)
{
	PKG_GG_CLI_LoginVerify_REQ *pstOut = (PKG_GG_CLI_LoginVerify_REQ*)pHost;

	if(-1 == poNetData->DelString(pstOut->szVerifyURL, sizeof(pstOut->szVerifyURL)))
		return -1;

	return sizeof(*pstOut);
}

int EncodePKG_CLI_GG_LoginVerify_ACK(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_LoginVerify_ACK *pstIn = (PKG_CLI_GG_LoginVerify_ACK*)pHost;

	if(-1 == poNetData->AddString(pstIn->szAnswer, sizeof(pstIn->szAnswer)))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_CLI_GG_LoginVerify_ACK(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_LoginVerify_ACK *pstOut = (PKG_CLI_GG_LoginVerify_ACK*)pHost;

	if(-1 == poNetData->DelString(pstOut->szAnswer, sizeof(pstOut->szAnswer)))
		return -1;

	return sizeof(*pstOut);
}

int EncodePKG_CLI_GG_MERGE_PT_REQ(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_MERGE_PT_REQ *pstIn = (PKG_CLI_GG_MERGE_PT_REQ*)pHost;

	if(-1 == poNetData->AddString(pstIn->szAccount, sizeof(pstIn->szAccount)))
		return -1;

	if(-1 == poNetData->AddString(pstIn->szPasswd, sizeof(pstIn->szPasswd)))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwClientVersion))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_CLI_GG_MERGE_PT_REQ(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_MERGE_PT_REQ *pstOut = (PKG_CLI_GG_MERGE_PT_REQ*)pHost;

	if(-1 == poNetData->DelString(pstOut->szAccount, sizeof(pstOut->szAccount)))
		return -1;

	if(-1 == poNetData->DelString(pstOut->szPasswd, sizeof(pstOut->szPasswd)))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwClientVersion))
		return -1;

	return sizeof(*pstOut);
}

int EncodePKG_CLI_GG_MERGE_PT_ACK(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_MERGE_PT_ACK *pstIn = (PKG_CLI_GG_MERGE_PT_ACK*)pHost;

	if(-1 == poNetData->AddInt(pstIn->nResult))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_CLI_GG_MERGE_PT_ACK(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GG_MERGE_PT_ACK *pstOut = (PKG_CLI_GG_MERGE_PT_ACK*)pHost;

	if(-1 == poNetData->DelInt(pstOut->nResult))
		return -1;

	return sizeof(*pstOut);
}

