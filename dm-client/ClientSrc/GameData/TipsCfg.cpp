#include "TipsCfg.h"
#include "Global/StringUtil.h"
#include "BaseClass/TiXml/tinyxml.h"

CTipsCfg g_TipsCfg;

CTipsCfg::CTipsCfg(void)
{
	//ReLoadGoodsTipsCfg();
	//ReLoadSkillTipsCfg();
	//ReLoadOtherTipsCfg();
}

CTipsCfg::~CTipsCfg(void)
{
}

sTipsCfg * CTipsCfg::GetCfgTips(const char * szName)
{
	if (!szName)
	{
		return NULL;
	}

	MTipsCfg::iterator itr = m_MGoodsTipsCfg.find(szName);
	if (itr != m_MGoodsTipsCfg.end())
	{
		return &(itr->second);
	}
	else
	{
		return NULL;
	}
}

sTipsCfg * CTipsCfg::GetSkillTips(const char * szName)
{
	if (!szName)
	{
		return NULL;
	}

	MTipsCfg::iterator itr = m_MSkillTipsCfg.find(szName);
	if (itr != m_MSkillTipsCfg.end())
	{
		return &(itr->second);
	}
	else
	{
		return NULL;
	}
}

sTipsCfg * CTipsCfg::GetOtherTips(const char * szName)
{
	if (!szName || szName[0] == 0)
	{
		return NULL;
	}

	MTipsCfg::iterator itr = m_MOtherTipsCfg.find(szName);
	if (itr != m_MOtherTipsCfg.end())
	{
		return &(itr->second);
	}
	else
	{
		return NULL;
	}
}



//<?xml version="1.0" encoding="gb2312" standalone="yes" ?>
//<TipsList Ver="1">
//    <tip goodsname = "test" maxwidth = "150">
//      <line>    
//	    	<col font = "FONT_SONGTI" fontsize = "12" color = "0xFFFFCF63" flag = "4" backcolor = "-1" framecolor = "0" offx = "1" offy = "0" content = "作用：将账号作用：将账号B"/>       
//		    <col font = "FONT_YAHEI"  fontsize = "26" color = "0xFFFF0000" flag = "4" framecolor = "0xFF000000" content = "升A级为至尊账号或兑换奖励。"/>       
//   	</line> 
//	    <line>
// 		    <col picpackage = "3" picidx = "71" />       
//		    <col color = "0xFF0000FF" ix = "61" content = "勋章上面有"/>
//	    </line>
//	    <line>
//	    	<col font = "FONT_SONGTI" fontsize = "14" color = "0xFFFFFF00" ix = "61" iy = "145" content = "暗沉的光"/>
//    	</line>
//    	<line>
//    		<col picpackage = "3" picidx = "73" />       
//	    	<col color = "0xFF00FF00" AutoCutByLeftWidth = "true" content = "位于炼狱地图中与任意地点老兵对话可以"/>
//	    	<col color = "0xFF0000FF" AutoCutByLeftWidth = "false" content = "aaaaaaaaaaaaaaaaaa"/>
//    	</line>        
//    	<line>    
//    		<col font = "FONT_YAHEI"  fontsize = "12" color = "0xFFFFCF63" flag = "0" framecolor = "0xFF000000" content = "升级为至尊账号或兑换奖励。"/>       
//    	</line>   
//    </tip> 
//</TipsList>

bool CTipsCfg::ReLoadGoodsTipsCfg()
{
	char path[MAX_PATH] = {0};
	sprintf(path,"%s\\config\\TipsCfg.xml",GetGameDataDir());

	return ReLoadTipsCfg(path,m_MGoodsTipsCfg);
}

bool CTipsCfg::ReLoadSkillTipsCfg()
{
	char path[MAX_PATH] = {0};
	sprintf(path,"%s\\config\\SkillHelp.xml",GetGameDataDir());

	return ReLoadTipsCfg(path,m_MSkillTipsCfg);
}

bool CTipsCfg::ReLoadOtherTipsCfg()
{
	char path[MAX_PATH] = {0};
	sprintf(path,"%s\\config\\OtherTipsCfg.xml",GetGameDataDir());
	return ReLoadTipsCfg(path,m_MOtherTipsCfg);
}

bool CTipsCfg::ReLoadTipsCfg(const char* szXmlPath,MTipsCfg &mtipsCfg)
{
	TiXmlDocument xmlDoc;
	if(!xmlDoc.LoadFile(szXmlPath)) //读取失败
		return false;

	mtipsCfg.clear();

	TiXmlElement* rootNode = xmlDoc.RootElement();
	TiXmlElement* node = rootNode->FirstChildElement();

	string strName;//名称
	int iFont = FONT_DEFAULT;//字体
	int iFontSize = FONTSIZE_DEFAULT;//字号
	DWORD dwColor = TIPS_KHAKI;//颜色
	DWORD dwBackColor = 0;
	DWORD dwFrameColor = 0xFF000000;
	DWORD dwFlag = 0;
	int iMaxWidth = 0;
	int iX = -1,iY = -1;//位置
	int iOffX = 0,iOffY = 0;//偏移
	int iPicPackage = 0,iPicIdx = 0;//图片位置
	bool bAutoCutByLeftWidth = false;
	int  iType = 0;
	string strContent;//内容
	char   *stopstring = NULL;

	for(;node;node = node->NextSiblingElement())
	{
		for(TiXmlAttribute* attr = node->FirstAttribute();attr;attr = attr->Next())
		{
			if(stricmp(attr->Name(),"goodsname") == 0 || stricmp(attr->Name(),"name") == 0)
				strName = attr->Value();
			else if(stricmp(attr->Name(),"maxwidth") == 0)
				iMaxWidth = attr->IntValue();
		}

		if (strName.empty())//没有名称属性，非法项
		{
			continue;
		}

		sTipsCfg tipsCfg;
		tipsCfg.iMaxWidth = iMaxWidth;

		TiXmlElement* child = node->FirstChildElement();
		for(;child;child = child->NextSiblingElement())
		{
			if(stricmp(child->Value(),"line") == 0)
			{
				sTipsCfgLine line;

				TiXmlElement* subchild = child->FirstChildElement();
				for(;subchild;subchild = subchild->NextSiblingElement())
				{
					if(stricmp(subchild->Value(),"col") == 0)
					{
						iFont = FONT_DEFAULT;
						iFontSize = FONTSIZE_DEFAULT;
						dwColor = TIPS_KHAKI;
						dwBackColor = 0;
						dwFrameColor = 0xFF000000;
						iX = -1;iY = -1;
						iOffX = 0;iOffY = 0;//偏移
						iPicPackage = 0;iPicIdx = 0;//图片位置
						dwFlag = 0;
						iType = 0;
						strContent.clear();

						for(TiXmlAttribute* attr = subchild->FirstAttribute();attr;attr = attr->Next())
						{
							if(stricmp(attr->Name(),"content") == 0)
							{
								strContent = attr->Value();
							}
							else if(stricmp(attr->Name(),"color") == 0)
							{
								dwColor = strtoul(attr->Value(),&stopstring,16);
							}
							else if(stricmp(attr->Name(),"font") == 0)
							{
								string strFont = attr->Value();
								if (strFont == "FONT_SONGTI")
								{
									iFont = FONT_SONGTI;
								}
								else if (strFont == "FONT_LISHU")
								{
									iFont = FONT_LISHU;
								}
								else if (strFont == "FONT_YAHEI")
								{
									iFont = FONT_YAHEI;
								}
							}
							else if(stricmp(attr->Name(),"fontsize") == 0)
							{
								iFontSize = attr->IntValue();
							}
							else if(stricmp(attr->Name(),"backcolor") == 0)
							{
								dwBackColor = strtoul(attr->Value(),&stopstring,16);
							}
							else if(stricmp(attr->Name(),"framecolor") == 0)
							{
								dwFrameColor = strtoul(attr->Value(),&stopstring,16);
							}
							else if(stricmp(attr->Name(),"flag") == 0)
							{
								dwFlag = strtoul(attr->Value(),&stopstring,16);
							}
							else if(stricmp(attr->Name(),"offx") == 0)
							{
								iOffX = attr->IntValue();
							}
							else if(stricmp(attr->Name(),"offy") == 0)
							{
								iOffY = attr->IntValue();
							}
							else if(stricmp(attr->Name(),"picpackage") == 0)
							{
								iPicPackage = attr->IntValue();
							}
							else if(stricmp(attr->Name(),"picidx") == 0)
							{
								iPicIdx = attr->IntValue();
							}
							else if(stricmp(attr->Name(),"ix") == 0)
							{
								iX = attr->IntValue();
							}
							else if(stricmp(attr->Name(),"iy") == 0)
							{
								iY = attr->IntValue();
							}
							else if(stricmp(attr->Name(),"AutoCutByLeftWidth") == 0)
							{
								bAutoCutByLeftWidth = attr->BoolValue();
							}
							else if(stricmp(attr->Name(),"type") == 0)
							{
								iType = attr->IntValue();
							}
						}

						line.VCols.push_back(sTipsCfgCol(strContent,iFont,iFontSize,dwColor,dwFlag,dwBackColor,dwFrameColor,iX,iY,iOffX,iOffY,iPicPackage,iPicIdx,bAutoCutByLeftWidth,iType));
					}

				}

				tipsCfg.VLines.push_back(line);
			}
		}

		mtipsCfg[strName] = tipsCfg;
	}

	return true;
}
