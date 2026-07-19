#include "cligsproto.h"

CCliGsProto::CCliGsProto()
{
	memset(m_EncodeFuncArray, 0, sizeof(m_EncodeFuncArray));
	memset(m_DecodeFuncArray, 0, sizeof(m_DecodeFuncArray));
	Init();
}

CCliGsProto::~CCliGsProto()
{
}

int CCliGsProto::Encode(int iMsgID, void *pHost, char *pNet, int iNetSize)
{
	m_oData.Prepare(pNet, iNetSize);

	EnFuncCCliGsProto pfnEncode = FindEncodeFunc(iMsgID);
	if(NULL == pfnEncode)
		return -1;

	return (*pfnEncode)(pHost, &m_oData);
}

int CCliGsProto::Decode(int iMsgID, char *pNet, int iNetSize, void *pHost, int iHostSize)
{
	m_oData.Prepare(pNet, iNetSize);
	if(iHostSize < m_iPkgSizeArray[iMsgID - CLIGSPROTO_MSGID_OFFSET])
		return -1;

	DeFuncCCliGsProto pfnDecode = FindDecodeFunc(iMsgID);
	if(NULL == pfnDecode)
		return -1;

	return (*pfnDecode)(pHost, &m_oData);
}

EnFuncCCliGsProto CCliGsProto::FindEncodeFunc(int iMsgID)
{
	if(iMsgID - CLIGSPROTO_MSGID_OFFSET >= CLIGSPROTO_MSGID_RANGE)
		return NULL;
	return m_EncodeFuncArray[iMsgID - CLIGSPROTO_MSGID_OFFSET];
}

DeFuncCCliGsProto CCliGsProto::FindDecodeFunc(int iMsgID)
{
	if(iMsgID - CLIGSPROTO_MSGID_OFFSET >= CLIGSPROTO_MSGID_RANGE)
		return NULL;
	return m_DecodeFuncArray[iMsgID - CLIGSPROTO_MSGID_OFFSET];
}

bool CCliGsProto::Init()
{
	memset(m_EncodeFuncArray, 0, sizeof(m_EncodeFuncArray));
	memset(m_DecodeFuncArray, 0, sizeof(m_DecodeFuncArray));

	m_EncodeFuncArray[CLI_GS_OLD_MSG - CLIGSPROTO_MSGID_OFFSET] = &EncodePKG_CLI_GS_OLD_MSG;
	m_DecodeFuncArray[CLI_GS_OLD_MSG - CLIGSPROTO_MSGID_OFFSET] = &DecodePKG_CLI_GS_OLD_MSG;

	m_EncodeFuncArray[GS_CLI_OLD_MSG - CLIGSPROTO_MSGID_OFFSET] = &EncodePKG_GS_CLI_OLD_MSG;
	m_DecodeFuncArray[GS_CLI_OLD_MSG - CLIGSPROTO_MSGID_OFFSET] = &DecodePKG_GS_CLI_OLD_MSG;

	m_iPkgSizeArray[CLI_GS_OLD_MSG - CLIGSPROTO_MSGID_OFFSET] = (int) sizeof(PKG_CLI_GS_OLD_MSG);
	m_iPkgSizeArray[GS_CLI_OLD_MSG - CLIGSPROTO_MSGID_OFFSET] = (int) sizeof(PKG_GS_CLI_OLD_MSG);
	return true;
}

int EncodePKG_CLI_GS_OLD_MSG(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GS_OLD_MSG *pstIn = (PKG_CLI_GS_OLD_MSG*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wMsgLen))
		return -1;

	int i;
	if((int)pstIn->wMsgLen > CliGsOldMsgLen)
       return -1;
	for(i = 0; i < CliGsOldMsgLen; ++i)
	{
		if(i >= (int)pstIn->wMsgLen)
			break;
		if(-1 == poNetData->AddByte(pstIn->abyMsgData[i]))
			return -1;
	}

	return poNetData->GetDataLen();
}

int DecodePKG_CLI_GS_OLD_MSG(void *pHost, CNetData* poNetData)
{
	PKG_CLI_GS_OLD_MSG *pstOut = (PKG_CLI_GS_OLD_MSG*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wMsgLen))
		return -1;

	int i;
	if ((int)pstOut->wMsgLen > CliGsOldMsgLen)
       return -1;
	for(i = 0; i < CliGsOldMsgLen; ++i)
	{
		if(i >= (int)pstOut->wMsgLen)
			break;
		if(-1 == poNetData->DelByte(pstOut->abyMsgData[i]))
			return -1;
	}

	return sizeof(*pstOut);
}

int EncodePKG_GS_CLI_OLD_MSG(void *pHost, CNetData* poNetData)
{
	PKG_GS_CLI_OLD_MSG *pstIn = (PKG_GS_CLI_OLD_MSG*)pHost;

	if(-1 == poNetData->AddWord(pstIn->wMsgLen))
		return -1;

	int i;
	if((int)pstIn->wMsgLen > CliGsOldMsgLen)
       return -1;
	for(i = 0; i < CliGsOldMsgLen; ++i)
	{
		if(i >= (int)pstIn->wMsgLen)
			break;
		if(-1 == poNetData->AddByte(pstIn->abyMsgData[i]))
			return -1;
	}

	return poNetData->GetDataLen();
}

int DecodePKG_GS_CLI_OLD_MSG(void *pHost, CNetData* poNetData)
{
	PKG_GS_CLI_OLD_MSG *pstOut = (PKG_GS_CLI_OLD_MSG*)pHost;

	if(-1 == poNetData->DelWord(pstOut->wMsgLen))
		return -1;

	int i;
	if ((int)pstOut->wMsgLen > CliGsOldMsgLen)
       return -1;
	for(i = 0; i < CliGsOldMsgLen; ++i)
	{
		if(i >= (int)pstOut->wMsgLen)
			break;
		if(-1 == poNetData->DelByte(pstOut->abyMsgData[i]))
			return -1;
	}

	return sizeof(*pstOut);
}

