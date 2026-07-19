#include "clissproto.h"

CCliSsProto::CCliSsProto()
{
	memset(m_EncodeFuncArray, 0, sizeof(m_EncodeFuncArray));
	memset(m_DecodeFuncArray, 0, sizeof(m_DecodeFuncArray));
	Init();
}

CCliSsProto::~CCliSsProto()
{
}

int CCliSsProto::Encode(int iMsgID, void *pHost, char *pNet, int iNetSize)
{
	m_oData.Prepare(pNet, iNetSize);

	EnFuncCCliSsProto pfnEncode = FindEncodeFunc(iMsgID);
	if(NULL == pfnEncode)
		return -1;

	return (*pfnEncode)(pHost, &m_oData);
}

int CCliSsProto::Decode(int iMsgID, char *pNet, int iNetSize, void *pHost, int iHostSize)
{
	m_oData.Prepare(pNet, iNetSize);
	if(iHostSize < m_iPkgSizeArray[iMsgID - CLISSPROTO_MSGID_OFFSET])
		return -1;

	DeFuncCCliSsProto pfnDecode = FindDecodeFunc(iMsgID);
	if(NULL == pfnDecode)
		return -1;

	return (*pfnDecode)(pHost, &m_oData);
}

EnFuncCCliSsProto CCliSsProto::FindEncodeFunc(int iMsgID)
{
	if(iMsgID - CLISSPROTO_MSGID_OFFSET >= CLISSPROTO_MSGID_RANGE)
		return NULL;
	return m_EncodeFuncArray[iMsgID - CLISSPROTO_MSGID_OFFSET];
}

DeFuncCCliSsProto CCliSsProto::FindDecodeFunc(int iMsgID)
{
	if(iMsgID - CLISSPROTO_MSGID_OFFSET >= CLISSPROTO_MSGID_RANGE)
		return NULL;
	return m_DecodeFuncArray[iMsgID - CLISSPROTO_MSGID_OFFSET];
}

bool CCliSsProto::Init()
{
	memset(m_EncodeFuncArray, 0, sizeof(m_EncodeFuncArray));
	memset(m_DecodeFuncArray, 0, sizeof(m_DecodeFuncArray));

	m_EncodeFuncArray[CLI_SS_MSG - CLISSPROTO_MSGID_OFFSET] = &EncodePKG_CLI_SS_MSG;
	m_DecodeFuncArray[CLI_SS_MSG - CLISSPROTO_MSGID_OFFSET] = &DecodePKG_CLI_SS_MSG;

	m_EncodeFuncArray[SS_CLI_MSG - CLISSPROTO_MSGID_OFFSET] = &EncodePKG_SS_CLI_MSG;
	m_DecodeFuncArray[SS_CLI_MSG - CLISSPROTO_MSGID_OFFSET] = &DecodePKG_SS_CLI_MSG;

	m_iPkgSizeArray[CLI_SS_MSG - CLISSPROTO_MSGID_OFFSET] = (int) sizeof(PKG_CLI_SS_MSG);
	m_iPkgSizeArray[SS_CLI_MSG - CLISSPROTO_MSGID_OFFSET] = (int) sizeof(PKG_SS_CLI_MSG);
	return true;
}

int EncodePKG_CLI_SS_MSG(void *pHost, CNetData* poNetData)
{
	PKG_CLI_SS_MSG *pstIn = (PKG_CLI_SS_MSG*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wMsgLen))
		return -1;

   if (pstIn->wMsgLen > CliSsMsgLen)
		return -1;
	if(-1 == poNetData->AddBuf(pstIn->abyMsgData, pstIn->wMsgLen))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_CLI_SS_MSG(void *pHost, CNetData* poNetData)
{
	PKG_CLI_SS_MSG *pstOut = (PKG_CLI_SS_MSG*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wMsgLen))
		return -1;

   if (pstOut->wMsgLen > CliSsMsgLen)
		return -1;
	if(-1 == poNetData->DelBuf(pstOut->abyMsgData, pstOut->wMsgLen))
		return -1;

	return sizeof(*pstOut);
}

int EncodePKG_SS_CLI_MSG(void *pHost, CNetData* poNetData)
{
	PKG_SS_CLI_MSG *pstIn = (PKG_SS_CLI_MSG*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wMsgLen))
		return -1;

   if (pstIn->wMsgLen > SsCliMsgLen)
		return -1;
	if(-1 == poNetData->AddBuf(pstIn->abyMsgData, pstIn->wMsgLen))
		return -1;

	return poNetData->GetDataLen();
}

int DecodePKG_SS_CLI_MSG(void *pHost, CNetData* poNetData)
{
	PKG_SS_CLI_MSG *pstOut = (PKG_SS_CLI_MSG*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wMsgLen))
		return -1;

   if (pstOut->wMsgLen > SsCliMsgLen)
		return -1;
	if(-1 == poNetData->DelBuf(pstOut->abyMsgData, pstOut->wMsgLen))
		return -1;

	return sizeof(*pstOut);
}

