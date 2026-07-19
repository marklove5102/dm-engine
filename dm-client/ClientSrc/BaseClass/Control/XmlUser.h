#pragma once

#include "XmlControl.h"

class CXmlGoodGrid : public CXmlControl
{
	DERIVE(CXmlGoodGrid,CXmlControl)
public:
	CXmlGoodGrid();
	~CXmlGoodGrid();
	virtual void Clear();
	virtual bool InitChildElement(const char* pValue);
protected:
    DECLARE_VAR(int, GridType)
    DECLARE_VAR(int, WCells)
    DECLARE_VAR(int, HCells)
    DECLARE_VAR(int, CellWidth)
    DECLARE_VAR(int, CellHeight)
    DECLARE_VAR(int, CellDisW)
    DECLARE_VAR(int, CellDisH)
};