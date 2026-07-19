#include "commdefs.h"

CCommDefs::CCommDefs()
{
	memset(m_EncodeFuncArray, 0, sizeof(m_EncodeFuncArray));
	memset(m_DecodeFuncArray, 0, sizeof(m_DecodeFuncArray));
	Init();
}

CCommDefs::~CCommDefs()
{
}

int CCommDefs::Encode(int iMsgID, void *pHost, char *pNet, int iNetSize)
{
	m_oData.Prepare(pNet, iNetSize);

	EnFuncCCommDefs pfnEncode = FindEncodeFunc(iMsgID);
	if(NULL == pfnEncode)
		return -1;

	return (*pfnEncode)(pHost, &m_oData);
}

int CCommDefs::Decode(int iMsgID, char *pNet, int iNetSize, void *pHost, int iHostSize)
{
	m_oData.Prepare(pNet, iNetSize);
	if(iHostSize < m_iPkgSizeArray[iMsgID - COMMDEFS_MSGID_OFFSET])
		return -1;

	DeFuncCCommDefs pfnDecode = FindDecodeFunc(iMsgID);
	if(NULL == pfnDecode)
		return -1;

	return (*pfnDecode)(pHost, &m_oData);
}

EnFuncCCommDefs CCommDefs::FindEncodeFunc(int iMsgID)
{
	if(iMsgID - COMMDEFS_MSGID_OFFSET >= COMMDEFS_MSGID_RANGE)
		return NULL;
	return m_EncodeFuncArray[iMsgID - COMMDEFS_MSGID_OFFSET];
}

DeFuncCCommDefs CCommDefs::FindDecodeFunc(int iMsgID)
{
	if(iMsgID - COMMDEFS_MSGID_OFFSET >= COMMDEFS_MSGID_RANGE)
		return NULL;
	return m_DecodeFuncArray[iMsgID - COMMDEFS_MSGID_OFFSET];
}

bool CCommDefs::Init()
{
	memset(m_EncodeFuncArray, 0, sizeof(m_EncodeFuncArray));
	memset(m_DecodeFuncArray, 0, sizeof(m_DecodeFuncArray));

	m_EncodeFuncArray[CS_Network_BenchMark_Msg - COMMDEFS_MSGID_OFFSET] = &EncodePKG_CS_Network_BenchMark_Msg;
	m_DecodeFuncArray[CS_Network_BenchMark_Msg - COMMDEFS_MSGID_OFFSET] = &DecodePKG_CS_Network_BenchMark_Msg;

	m_EncodeFuncArray[CS_HeartBeat_Req - COMMDEFS_MSGID_OFFSET] = &EncodePKG_CS_HeartBeat_Req;
	m_DecodeFuncArray[CS_HeartBeat_Req - COMMDEFS_MSGID_OFFSET] = &DecodePKG_CS_HeartBeat_Req;

	m_EncodeFuncArray[CS_HeartBeat_Ack - COMMDEFS_MSGID_OFFSET] = &EncodePKG_CS_HeartBeat_Ack;
	m_DecodeFuncArray[CS_HeartBeat_Ack - COMMDEFS_MSGID_OFFSET] = &DecodePKG_CS_HeartBeat_Ack;

	m_iPkgSizeArray[CS_Network_BenchMark_Msg - COMMDEFS_MSGID_OFFSET] = (int) sizeof(PKG_CS_Network_BenchMark_Msg);
	m_iPkgSizeArray[CS_HeartBeat_Req - COMMDEFS_MSGID_OFFSET] = (int) sizeof(PKG_CS_HeartBeat_Req);
	m_iPkgSizeArray[CS_HeartBeat_Ack - COMMDEFS_MSGID_OFFSET] = (int) sizeof(PKG_CS_HeartBeat_Ack);
	return true;
}

int EncodeSIPAddr(void *pHost, CNetData* poNetData)
{
	SIPAddr *pstIn = (SIPAddr*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwIP))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wPort))
		return -1;

	return poNetData->GetDataLen();
}

int DecodeSIPAddr(void *pHost, CNetData* poNetData)
{
	SIPAddr *pstOut = (SIPAddr*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwIP))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wPort))
		return -1;

	return sizeof(*pstOut);
}

int EncodeSErrCode(void *pHost, CNetData* poNetData)
{
	SErrCode *pstIn = (SErrCode*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wErrCode))
		return -1;

	return poNetData->GetDataLen();
}

int DecodeSErrCode(void *pHost, CNetData* poNetData)
{
	SErrCode *pstOut = (SErrCode*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wErrCode))
		return -1;

	return sizeof(*pstOut);
}

int EncodeSRoleBasicInfo(void *pHost, CNetData* poNetData)
{
	SRoleBasicInfo *pstIn = (SRoleBasicInfo*)pHost;

	if(-1 == poNetData->AddString(pstIn->szRoleName, sizeof(pstIn->szRoleName)))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byJob))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byHair))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wLevel))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->bySex))
		return -1;

	if(-1 == poNetData->AddString(pstIn->szDeleteTime, sizeof(pstIn->szDeleteTime)))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byDelete))
		return -1;

	return poNetData->GetDataLen();
}

int DecodeSRoleBasicInfo(void *pHost, CNetData* poNetData)
{
	SRoleBasicInfo *pstOut = (SRoleBasicInfo*)pHost;

	if(-1 == poNetData->DelString(pstOut->szRoleName, sizeof(pstOut->szRoleName)))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byJob))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byHair))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wLevel))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->bySex))
		return -1;

	if(-1 == poNetData->DelString(pstOut->szDeleteTime, sizeof(pstOut->szDeleteTime)))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byDelete))
		return -1;

	return sizeof(*pstOut);
}

int EncodeSRoleList(void *pHost, CNetData* poNetData)
{
	SRoleList *pstIn = (SRoleList*)pHost;

	if(-1 == poNetData->AddByte(pstIn->bySelect))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byRoleCount))
		return -1;

	int i;
	if ((int)pstIn->byRoleCount > ROLE_MAX_NUM_ALL)
       return -1;
	for(i = 0; i < ROLE_MAX_NUM_ALL; ++i)
	{
		if(i >= (int)pstIn->byRoleCount)
			break;
		if(-1 == EncodeSRoleBasicInfo(&pstIn->astRoleList[i], poNetData))
			return -1;
	}

	return poNetData->GetDataLen();
}

int DecodeSRoleList(void *pHost, CNetData* poNetData)
{
	SRoleList *pstOut = (SRoleList*)pHost;

	if(-1 == poNetData->DelByte(pstOut->bySelect))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byRoleCount))
		return -1;

	int i;
	if ((int)pstOut->byRoleCount > ROLE_MAX_NUM_ALL)
       return -1;
	for(i = 0; i < ROLE_MAX_NUM_ALL; ++i)
	{
		if(i >= (int)pstOut->byRoleCount)
			break;
		if(-1 == DecodeSRoleBasicInfo(&pstOut->astRoleList[i], poNetData))
			return -1;
	}

	return sizeof(*pstOut);
}

int EncodeSItem(void *pHost, CNetData* poNetData)
{
	SItem *pstIn = (SItem*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwID))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wIndex))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wDura))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wDuraMax))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byDemonFlag))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wEvilAttr1))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wEvilAttr2))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wEvilAttr3))
		return -1;

	if(-1 == poNetData->AddBuf(pstIn->abyReserved, 14))
		return -1;

	if(-1 == poNetData->AddBuf(pstIn->abyReserved2, 17))
		return -1;

	if(-1 == poNetData->AddBuf(pstIn->abyReserved3, 16))
		return -1;

	return poNetData->GetDataLen();
}

int DecodeSItem(void *pHost, CNetData* poNetData)
{
	SItem *pstOut = (SItem*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwID))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wIndex))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wDura))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wDuraMax))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byDemonFlag))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wEvilAttr1))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wEvilAttr2))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wEvilAttr3))
		return -1;

	if(-1 == poNetData->DelBuf(pstOut->abyReserved, 14))
		return -1;

	if(-1 == poNetData->DelBuf(pstOut->abyReserved2, 17))
		return -1;

	if(-1 == poNetData->DelBuf(pstOut->abyReserved3, 16))
		return -1;

	return sizeof(*pstOut);
}

int EncodeSCreateRoleInfo(void *pHost, CNetData* poNetData)
{
	SCreateRoleInfo *pstIn = (SCreateRoleInfo*)pHost;

	if(-1 == poNetData->AddString(pstIn->szPTID, sizeof(pstIn->szPTID)))
		return -1;

	if(-1 == poNetData->AddString(pstIn->szRoleName, sizeof(pstIn->szRoleName)))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byHair))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byJob))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->bySex))
		return -1;

	return poNetData->GetDataLen();
}

int DecodeSCreateRoleInfo(void *pHost, CNetData* poNetData)
{
	SCreateRoleInfo *pstOut = (SCreateRoleInfo*)pHost;

	if(-1 == poNetData->DelString(pstOut->szPTID, sizeof(pstOut->szPTID)))
		return -1;

	if(-1 == poNetData->DelString(pstOut->szRoleName, sizeof(pstOut->szRoleName)))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byHair))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byJob))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->bySex))
		return -1;

	return sizeof(*pstOut);
}

int EncodeUnURoleOpAck(void *pHost, int iSelect, CNetData* poNetData)
{
	switch(iSelect){
	case PROTO_SUCCESS:
		return EncodeSRoleList(pHost, poNetData);
	case PROTO_FAILURE:
		return EncodeSErrCode(pHost, poNetData);
	default: return -1;
	}
}

int DecodeUnURoleOpAck(void *pHost, int iSelect, CNetData* poNetData)
{
	switch(iSelect){
	case PROTO_SUCCESS:
		return DecodeSRoleList(pHost, poNetData);
	case PROTO_FAILURE:
		return DecodeSErrCode(pHost, poNetData);
	default: return -1;
	}
}

int EncodeLoginInfo(void *pHost, CNetData* poNetData)
{
	LoginInfo *pstIn = (LoginInfo*)pHost;

	if(-1 == poNetData->AddString(pstIn->szSpreader, sizeof(pstIn->szSpreader)))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byLockStore))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byEkeyType))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byPTType))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byPWLimited))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byReferUser))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byMobileUser))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->bySpreader))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byANPMove))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byECardType))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byECardBindFlag))
		return -1;

	if(-1 == poNetData->AddString(pstIn->szChallenge, sizeof(pstIn->szChallenge)))
		return -1;

	if(-1 == poNetData->AddString(pstIn->szProxyTicket, sizeof(pstIn->szProxyTicket)))
		return -1;

	return poNetData->GetDataLen();
}

int DecodeLoginInfo(void *pHost, CNetData* poNetData)
{
	LoginInfo *pstOut = (LoginInfo*)pHost;

	if(-1 == poNetData->DelString(pstOut->szSpreader, sizeof(pstOut->szSpreader)))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byLockStore))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byEkeyType))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byPTType))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byPWLimited))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byReferUser))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byMobileUser))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->bySpreader))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byANPMove))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byECardType))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byECardBindFlag))
		return -1;

	if(-1 == poNetData->DelString(pstOut->szChallenge, sizeof(pstOut->szChallenge)))
		return -1;

	if(-1 == poNetData->DelString(pstOut->szProxyTicket, sizeof(pstOut->szProxyTicket)))
		return -1;

	return sizeof(*pstOut);
}

int EncodeLoginData(void *pHost, CNetData* poNetData)
{
	LoginData *pstIn = (LoginData*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byFCMFill))
		return -1;

	if(-1 == poNetData->AddWord(pstIn->wMsgLen))
		return -1;

	if(-1 == poNetData->AddString(pstIn->szMsg, sizeof(pstIn->szMsg)))
		return -1;

	return poNetData->GetDataLen();
}

int DecodeLoginData(void *pHost, CNetData* poNetData)
{
	LoginData *pstOut = (LoginData*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byFCMFill))
		return -1;

	if(-1 == poNetData->DelWord(pstOut->wMsgLen))
		return -1;

	if(-1 == poNetData->DelString(pstOut->szMsg, sizeof(pstOut->szMsg)))
		return -1;

	return sizeof(*pstOut);
}

int EncodePlayerReportInfo(void *pHost, CNetData* poNetData)
{
	PlayerReportInfo *pstIn = (PlayerReportInfo*)pHost;

	if(-1 == poNetData->AddString(pstIn->szGroupName, sizeof(pstIn->szGroupName)))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byServerID))
		return -1;

	int i;
	for(i = 0; i < REPORT_SIZE; ++i)
	{
		if(-1 == poNetData->AddDword(pstIn->adwNPlayerNum[i]))
			return -1;
	}

	for(i = 0; i < REPORT_SIZE; ++i)
	{
		if(-1 == poNetData->AddDword(pstIn->adwBPlayerNum[i]))
			return -1;
	}

	return poNetData->GetDataLen();
}

int DecodePlayerReportInfo(void *pHost, CNetData* poNetData)
{
	PlayerReportInfo *pstOut = (PlayerReportInfo*)pHost;

	if(-1 == poNetData->DelString(pstOut->szGroupName, sizeof(pstOut->szGroupName)))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byServerID))
		return -1;

	int i;
	for(i = 0; i < REPORT_SIZE; ++i)
	{
		if(-1 == poNetData->DelDword(pstOut->adwNPlayerNum[i]))
			return -1;
	}

	for(i = 0; i < REPORT_SIZE; ++i)
	{
		if(-1 == poNetData->DelDword(pstOut->adwBPlayerNum[i]))
			return -1;
	}

	return sizeof(*pstOut);
}

int EncodeBMTimeVal(void *pHost, CNetData* poNetData)
{
	BMTimeVal *pstIn = (BMTimeVal*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwSec))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwUSec))
		return -1;

	return poNetData->GetDataLen();
}

int DecodeBMTimeVal(void *pHost, CNetData* poNetData)
{
	BMTimeVal *pstOut = (BMTimeVal*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwSec))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwUSec))
		return -1;

	return sizeof(*pstOut);
}

int EncodeCGSNetworkBenchMark(void *pHost, CNetData* poNetData)
{
	CGSNetworkBenchMark *pstIn = (CGSNetworkBenchMark*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwCliID))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwGgID))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwSvrID))
		return -1;

	if(-1 == EncodeBMTimeVal(&pstIn->stReqCliSendTime, poNetData))
		return -1;

	if(-1 == EncodeBMTimeVal(&pstIn->stReqGgRecvTime, poNetData))
		return -1;

	if(-1 == EncodeBMTimeVal(&pstIn->stReqGgSendTime, poNetData))
		return -1;

	if(-1 == EncodeBMTimeVal(&pstIn->stReqSvrRecvTime, poNetData))
		return -1;

	if(-1 == EncodeBMTimeVal(&pstIn->stAckSvrSendTime, poNetData))
		return -1;

	if(-1 == EncodeBMTimeVal(&pstIn->stAckGgRecvTime, poNetData))
		return -1;

	if(-1 == EncodeBMTimeVal(&pstIn->stAckGgSendTime, poNetData))
		return -1;

	if(-1 == EncodeBMTimeVal(&pstIn->stAckCliRecvTime, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

int DecodeCGSNetworkBenchMark(void *pHost, CNetData* poNetData)
{
	CGSNetworkBenchMark *pstOut = (CGSNetworkBenchMark*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwCliID))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwGgID))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwSvrID))
		return -1;

	if(-1 == DecodeBMTimeVal(&pstOut->stReqCliSendTime, poNetData))
		return -1;

	if(-1 == DecodeBMTimeVal(&pstOut->stReqGgRecvTime, poNetData))
		return -1;

	if(-1 == DecodeBMTimeVal(&pstOut->stReqGgSendTime, poNetData))
		return -1;

	if(-1 == DecodeBMTimeVal(&pstOut->stReqSvrRecvTime, poNetData))
		return -1;

	if(-1 == DecodeBMTimeVal(&pstOut->stAckSvrSendTime, poNetData))
		return -1;

	if(-1 == DecodeBMTimeVal(&pstOut->stAckGgRecvTime, poNetData))
		return -1;

	if(-1 == DecodeBMTimeVal(&pstOut->stAckGgSendTime, poNetData))
		return -1;

	if(-1 == DecodeBMTimeVal(&pstOut->stAckCliRecvTime, poNetData))
		return -1;

	return sizeof(*pstOut);
}

int EncodeCSNetworkBenchMark(void *pHost, CNetData* poNetData)
{
	CSNetworkBenchMark *pstIn = (CSNetworkBenchMark*)pHost;

	if(-1 == poNetData->AddDword(pstIn->dwCliID))
		return -1;

	if(-1 == poNetData->AddDword(pstIn->dwSvrID))
		return -1;

	if(-1 == EncodeBMTimeVal(&pstIn->stReqCliSendTime, poNetData))
		return -1;

	if(-1 == EncodeBMTimeVal(&pstIn->stReqSvrRecvTime, poNetData))
		return -1;

	if(-1 == EncodeBMTimeVal(&pstIn->stAckSvrSendTime, poNetData))
		return -1;

	if(-1 == EncodeBMTimeVal(&pstIn->stAckCliRecvTime, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

int DecodeCSNetworkBenchMark(void *pHost, CNetData* poNetData)
{
	CSNetworkBenchMark *pstOut = (CSNetworkBenchMark*)pHost;

	if(-1 == poNetData->DelDword(pstOut->dwCliID))
		return -1;

	if(-1 == poNetData->DelDword(pstOut->dwSvrID))
		return -1;

	if(-1 == DecodeBMTimeVal(&pstOut->stReqCliSendTime, poNetData))
		return -1;

	if(-1 == DecodeBMTimeVal(&pstOut->stReqSvrRecvTime, poNetData))
		return -1;

	if(-1 == DecodeBMTimeVal(&pstOut->stAckSvrSendTime, poNetData))
		return -1;

	if(-1 == DecodeBMTimeVal(&pstOut->stAckCliRecvTime, poNetData))
		return -1;

	return sizeof(*pstOut);
}

int EncodeUnUnBenchMarkBody(void *pHost, int iSelect, CNetData* poNetData)
{
	switch(iSelect){
	case BENCHMARK_CGS:
		return EncodeCGSNetworkBenchMark(pHost, poNetData);
	case BENCHMARK_CS:
		return EncodeCSNetworkBenchMark(pHost, poNetData);
	default: return -1;
	}
}

int DecodeUnUnBenchMarkBody(void *pHost, int iSelect, CNetData* poNetData)
{
	switch(iSelect){
	case BENCHMARK_CGS:
		return DecodeCGSNetworkBenchMark(pHost, poNetData);
	case BENCHMARK_CS:
		return DecodeCSNetworkBenchMark(pHost, poNetData);
	default: return -1;
	}
}

int EncodePKG_CS_Network_BenchMark_Msg(void *pHost, CNetData* poNetData)
{
	PKG_CS_Network_BenchMark_Msg *pstIn = (PKG_CS_Network_BenchMark_Msg*)pHost;

	if(-1 == poNetData->AddByte(pstIn->byFrom))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byTo))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byDir))
		return -1;

	if(-1 == poNetData->AddByte(pstIn->byType))
		return -1;

	if(-1 == EncodeUnUnBenchMarkBody(&pstIn->stContent, pstIn->byType, poNetData))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_CS_Network_BenchMark_Msg(void *pHost, CNetData* poNetData)
{
	PKG_CS_Network_BenchMark_Msg *pstOut = (PKG_CS_Network_BenchMark_Msg*)pHost;

	if(-1 == poNetData->DelByte(pstOut->byFrom))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byTo))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byDir))
		return -1;

	if(-1 == poNetData->DelByte(pstOut->byType))
		return -1;

	if(-1 == DecodeUnUnBenchMarkBody(&pstOut->stContent, pstOut->byType, poNetData))
		return -1;

	return sizeof(*pstOut);
}

int EncodePKG_CS_HeartBeat_Req(void *pHost, CNetData* poNetData)
{
	PKG_CS_HeartBeat_Req *pstIn = (PKG_CS_HeartBeat_Req*)pHost;

	return poNetData->GetDataLen();
}

int DecodePKG_CS_HeartBeat_Req(void *pHost, CNetData* poNetData)
{
	PKG_CS_HeartBeat_Req *pstOut = (PKG_CS_HeartBeat_Req*)pHost;

	return sizeof(*pstOut);
}

int EncodePKG_CS_HeartBeat_Ack(void *pHost, CNetData* poNetData)
{
	PKG_CS_HeartBeat_Ack *pstIn = (PKG_CS_HeartBeat_Ack*)pHost;

	return poNetData->GetDataLen();
}

int DecodePKG_CS_HeartBeat_Ack(void *pHost, CNetData* poNetData)
{
	PKG_CS_HeartBeat_Ack *pstOut = (PKG_CS_HeartBeat_Ack*)pHost;

	return sizeof(*pstOut);
}

