#include "xmlobj.h"
#include <assert.h>
#include "tinyxml.h"

//------------------------------//
//		使用xml的基类			//
//------------------------------//
int   CXmlObj::s_iLastError = 0;
const TiXmlElement*	CXmlObj::s_pCurParsingElement = NULL;
VarMap	CXmlObj::sm_varMap	= { NULL, CXmlObj::sm_entry};
AttrEntry	CXmlObj::sm_entry[]	= {AttrEntry(BAT_string, NULL, NULL, 0)};

bool CXmlObj::Init(const TiXmlElement* ele)
{
	Clear();
	//初始化属性
	InitAttrVar();

	//初始化节点
	if(!ele) return false;
	InitSelf(ele->ToElement());

	//解释属性
	for(TiXmlAttribute* pAttr = ele->FirstAttribute();
		pAttr;
		pAttr = pAttr->Next())
	{
		FindAndSetAttr(pAttr);
	}
	OnCreate();

	//解释节点
	TiXmlNode * child = 0;
	while( child = ele->IterateChildren( child ) )
	{
		switch(child->Type())
		{
		case TiXmlNode::TEXT:
			InitChildText(child->ToText()->Value());
			break;
		case TiXmlNode::COMMENT:
			InitChildComm(child->ToComment()->Value());
			break;
		case TiXmlNode::ELEMENT:
			{
				TiXmlElement* ele1 = child->ToElement();
				s_pCurParsingElement = ele1;
				InitChildElement(ele1->Value());
			}
			break;
		}
	}
	return true;
}


void CXmlObj::InitAttrVar()
{
	VarMap* valmap = GetVarMap();
	while(valmap)
	{
		for(AttrEntry* pEntry = valmap->pEntry;
			pEntry&&pEntry->_fn&&pEntry->_name;	
			pEntry++)
		{
			CallSetFn(pEntry->_fn, pEntry->_initval);
		}
		valmap = valmap->pSuperMap;	//上溯
	}
}


bool CXmlObj::FindAndSetAttr(const TiXmlAttribute* attr)
{
	//安全性检查
	if(attr == NULL) return true;
	const char* pAttName = attr->Name();
	if(pAttName == NULL)	return true;

	VarMap* valmap = GetVarMap();	//获取当前描述表
	while(valmap)
	{
		for(AttrEntry* pEntry = valmap->pEntry;	
			pEntry&&pEntry->_fn&&pEntry->_name;	
			pEntry++)			//轮询描述表
		{
			if(CmpString(pAttName, pEntry->_name, true))	//在表中找到
			{
				switch(pEntry->_type)	//类型匹配
				{
				case BAT_bool:
					CallSetFn(pEntry->_fn, ComVal(attr->BoolValue()));
					break;
				case BAT_string:
					CallSetFn(pEntry->_fn, ComVal(attr->Value()));
					break;
				case BAT_int:
					CallSetFn(pEntry->_fn, ComVal(attr->IntValue()));
					break;
				case BAT_double:
					CallSetFn(pEntry->_fn, ComVal(attr->DoubleValue()));
					break;
				}
				return true;
			}
		}
		valmap = valmap->pSuperMap;	//如果不在表中，向父类描述表中上溯
	}
	return true;
}

bool CXmlObj::CmpName(const TiXmlNode* pNode, const char* pName, bool bIgnorCase) const
{
	if(pNode && pName)
	{
		const char* pEleName = pNode->Value();
		if(!pEleName) return false;
		const char* pTargetName = pName;
		return CmpString(pEleName, pTargetName, bIgnorCase);
	}
	return false;
}

bool CXmlObj::CmpString(const char* sz1, const char* sz2, bool bIgnorCase) const
{
	if(sz1 && sz2)
	{
		if(bIgnorCase)
		{
			const int MAX = 256;
			char cOrigin[MAX] = {0};
			char cTarget[MAX] = {0};
			if(strlen(sz1)<MAX)
			{
				strcpy(cOrigin, sz1);
				strlwr(cOrigin);
				sz1 = cOrigin;
			}
			if(strlen(sz2)<MAX)
			{
				strcpy(cTarget, sz2);
				strlwr(cTarget);
				sz2 = cTarget;
			}
		}
		return (strcmp(sz1, sz2)==0);
	}
	return false;
}

bool CXmlObj::LoadFromBuff(const char* buf)
{
	TiXmlDocument xmlDoc;
	xmlDoc.Clear();
	if (!xmlDoc.Parse(buf))
	{
		return false;
	}

	s_pCurParsingElement = xmlDoc.RootElement();
	if(!s_pCurParsingElement)
		return false;

	return Create();

}

bool CXmlObj::LoadFile(const char* filename)
{
	TiXmlDocument xmlDoc;
	if(!xmlDoc.LoadFile(filename))
		return false;

	s_pCurParsingElement = xmlDoc.RootElement();
	if(!s_pCurParsingElement)
		return false;

	return Create();
}