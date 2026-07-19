#include "talkname.h"

CTalkName g_TalkName;

CTalkName::CTalkName(void)
{
	m_List =NULL;
	m_size =0;
	m_Null = "";

	memset(m_sLWName,0,MAX_NAME_COUNT * 16);
	m_iLWCur  = 0;
	m_iLWSize = 0;
}

CTalkName::~CTalkName(void)
{
	_Name * p;
	_Name * prev;
	p = m_List;
	while (p)
	{
		prev = p;
		p=p->pNextName;
		delete prev;

	}
}

void CTalkName::InsertName(const char * pName)
{

	if(strlen(pName) == 0)
		return ;

	_Name * tempName = NULL;

	//等于第一个人名返回
	if(m_List && m_List->name == pName)
		return;
	//分配空间
	tempName = new _Name(pName);
	if(!tempName)
		return ;
	//插入第一个节点前
	m_size++;
	tempName->pNextName = m_List;
	if(m_List)
		m_List->pPrevName = tempName;
	m_List = tempName;
	//删除重明的或超出长度的节点
	int i=1;
	tempName = m_List->pNextName;
	_Name * prev=m_List;

	while(tempName && tempName->name != pName && i< MAX_NAME_COUNT)
	{
		tempName = tempName->pNextName;
		prev = tempName;
		i++;
	}

	if(tempName)
	{
		tempName->pPrevName->pNextName = tempName->pNextName;
		if(tempName->pNextName)
			tempName->pNextName->pPrevName = tempName->pPrevName;
		delete(tempName);
		tempName = NULL;
		m_size--;
	}
	else if(i>MAX_NAME_COUNT)
	{
		if(prev)
			prev->pPrevName->pNextName=NULL;
		delete(prev);
		prev=NULL;
		m_size= MAX_NAME_COUNT;
	}
}


string &  CTalkName::GetNameList(int pos)	
{
	int i=0;
	_Name * p = m_List;
	while(i<pos && i<m_size && p)
	{
		i++;
		p = p->pNextName;
	}
	if(i == pos && p)
		return p->name;
	else
		return m_Null;
}
void CTalkName::InsertLWName(const char* sName)
{
	if(!sName || strlen(sName) >= 16)
		return;
	char str[16] = "";
	for(int i = 0; i < m_iLWSize; i++)
	{
		if(strcmp(m_sLWName + i * 16,sName) == 0)
		{
			if(i != 0)
			{
				strcpy(str,m_sLWName + i * 16);
				for(int j = i; j > 0; j--)
				{
					strcpy(m_sLWName + j * 16,m_sLWName + (j - 1) * 16);
				}
				strcpy(m_sLWName,str);
			}
			return;
		}
	}
	for(int k = (m_iLWSize >= MAX_NAME_COUNT ? MAX_NAME_COUNT - 1 : m_iLWSize); k > 0; k--)
	{
		strcpy(m_sLWName + k * 16,m_sLWName + (k - 1) * 16);
	}
	strcpy(m_sLWName,sName);
	if(m_iLWSize < MAX_NAME_COUNT)
		m_iLWSize++;
}

char * CTalkName::GetLWName(int iPos)
{
	if(iPos < 0 || iPos >= m_iLWSize)
		return NULL;
	else
		return m_sLWName + iPos * 16;
}

int CTalkName::GetLWNameSize(void)
{
	return m_iLWSize;
}
