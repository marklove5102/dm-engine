#pragma once

#include "Global/Global.h"

#define MAX_NAME_COUNT 10

class CTalkName
{
public:
	CTalkName(void);
	~CTalkName(void);
public :
	/////////
	struct  _Name
	{
		string name;
		_Name * pPrevName;
		_Name * pNextName;
		_Name(const char * pName)
		{
			name = pName;
			pPrevName = NULL;
			pNextName = NULL;
		}
	} ;
	string & GetNameList(int pos);	


	int GetSize() {return m_size;}
	void InsertName(const char * pName);
private:

	char m_sLWName[MAX_NAME_COUNT * 16];			// ¡Ù—‘√˚◊÷¡–±Ì
	int	 m_iLWSize;
	int	 m_iLWCur;

	int  m_size;
	_Name * m_List;
	string m_Null;

public:
	void InsertLWName(const char* sName);
	char * GetLWName(int iPos);
	int GetLWNameSize(void);
};

extern CTalkName g_TalkName;
