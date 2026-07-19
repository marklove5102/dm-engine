#include "XmlUser.h"
#include <assert.h>

BEGIN_VAR(CXmlGoodGrid,CXmlControl)
INIT_VAR(int, GridType, 0)
INIT_VAR(int, WCells, 0)
INIT_VAR(int, HCells, 0)
INIT_VAR(int, CellWidth, 0)
INIT_VAR(int, CellHeight, 0)
INIT_VAR(int, CellDisW, 0)
INIT_VAR(int, CellDisH, 0)
END_VAR(CXmlGoodGrid,CXmlControl)

CXmlGoodGrid::CXmlGoodGrid()
{
	m_xmlMode = XVE_XGOODGRID;
}

CXmlGoodGrid::~CXmlGoodGrid()
{    
}

void CXmlGoodGrid::Clear()
{
	SUPERCLASS::Clear();
}

bool CXmlGoodGrid::InitChildElement(const char* pValue)
{
	if(SUPERCLASS::InitChildElement(pValue))
		return true;

	assert(pValue);
	return false;
}