#include "ItemCfgMgr.h"
#include <stdio.h>
#include <fstream>
#include "Global/StringUtil.h"
#include <algorithm> 

//const char* const MIDDLESTRING = "_____***_______"; 

CItemCfgMgr g_ItemCfgMgr;
CPromptInfoMgr g_PromptInfoMgr;

struct analyzeini{
	std::string strsect;
	StringMap *pmap;
	analyzeini(StringMap & strmap):pmap(&strmap){}
	void operator()( const std::string & strini)
	{
		int first =strini.find('[');
		int last = strini.rfind(']');
		if( first != string::npos && last != string::npos && first != last+1)
		{
			strsect = strini.substr(first+1,last-first-1);
			return ;
		}
		if(strsect.empty())
			return ;
		if((first=strini.find('='))== string::npos)
			return ;
		string strtmp1= strini.substr(0,first);
		string strtmp2=strini.substr(first+1, string::npos);
		first= strtmp1.find_first_not_of(" \t");
		last = strtmp1.find_last_not_of(" \t");
		if(first == string::npos || last == string::npos)
			return ;
		string strkey = strtmp1.substr(first, last-first+1);
		first = strtmp2.find_first_not_of(" \t");
		if(((last = strtmp2.find(" \t#", first )) != string::npos) ||
			((last = strtmp2.find("#", first )) != string::npos) ||
			((last = strtmp2.find(" \t//", first )) != string::npos)||
			((last = strtmp2.find("//", first )) != string::npos))
		{
			strtmp2 = strtmp2.substr(0, last-first);
		}
		last = strtmp2.find_last_not_of(" \t");
		if(first == string::npos || last == string::npos)
			return ;
		string value = strtmp2.substr(first, last-first+1);
		string mapkey = strsect;// + MIDDLESTRING;
		mapkey += strkey;
		(*pmap)[mapkey]=value;
		return ;
	}
}; 

CItemCfgMgr::CItemCfgMgr()
{
	//初始的时候导入物品特效的全部配置信息
	//导入玩家动作图片的全部配置信息(衣服头发翅膀的图片位置信息)
	ReLoad();
}

CItemCfgMgr::~CItemCfgMgr()
{
	m_mEffectCfg.clear();
}

string CItemCfgMgr::ParseGoodsTips(const char*pkey)
{
	string mapkey;
	mapkey.assign("Tips");
	//mapkey += MIDDLESTRING;
	mapkey += pkey;
	StringMap::iterator it = m_mEffectCfg.find(mapkey);
	if(it == m_mEffectCfg.end())
		return "";
	else
		return it->second;
}

bool CItemCfgMgr::ParseItemEffect(const char* psect,const char*pkey , SItemEffect &sInfo)
{
	memset(&sInfo,0,sizeof(SItemEffect));

	string mapkey = psect;
//	mapkey += "_____***_______";
	mapkey += pkey;
	StringMap::iterator it = m_mEffectCfg.find(mapkey);
	if(it == m_mEffectCfg.end())
		return false;

	string strValue = it->second;
	if(strValue.empty())
		return false;

	sscanf(strValue.c_str(),"%d/%d/%d/%d/%d",&sInfo.iTexturePackage,&sInfo.iTextureID,&sInfo.iRenderMode,&sInfo.iOffsetX,&sInfo.iOffsetY);
	return true;
}

bool CItemCfgMgr::ParseItem2Magic(const char* psect, const char* pkey, SItem2MagicEffect& sInfo)
{
	memset(&sInfo,0,sizeof(SItem2MagicEffect));

	string mapkey = psect;
	//mapkey += MIDDLESTRING;
	mapkey += pkey;

	StringMap::iterator it = m_mEffectCfg.find(mapkey);
	if(it == m_mEffectCfg.end())
		return false;

	string strValue = it->second;
	if(strValue.empty())
		return false;

	sscanf(strValue.c_str(),"%d,%d,%d,%d,%d",&sInfo.iMagicID,&sInfo.iAlpha,&sInfo.iFx,&sInfo.iFy,&sInfo.iUsedXY);
	return true;
}

bool CItemCfgMgr::ParseItemFeed(const char* psect,const char* pkey,unsigned char& byValue)
{
	byValue = 0xFF;
	string mapkey = psect;
	mapkey += pkey;
	StringMap::iterator it = m_mEffectCfg.find(mapkey);
	if(it == m_mEffectCfg.end())
		return false;

	string strValue = it->second;
	if(strValue.empty())
		return false;

	sscanf(strValue.c_str(),"%c",&byValue);
	return true;
}

bool  CItemCfgMgr::ParseCardInfo(const char* psect,const char* pkey,const char* pName,const char* pDesc)
{
	string mapkey = psect;
	mapkey += pkey;
	StringMap::iterator it = m_mEffectCfg.find(mapkey);
	if(it == m_mEffectCfg.end())
		return false;

	string strValue = it->second;
	if(strValue.empty())
		return false;

	int iPos = strValue.find_first_of('/');
	if(iPos == std::string::npos)
	{
		return false;
	}
	else 
	{
		std::string strTemp;

		if(pName != NULL)
		{
			strTemp = strValue.substr(0,iPos);
			sscanf(strTemp.c_str(),"%s", (char*)pName);
		}

		if(pDesc != NULL)
		{
			strTemp = strValue.substr(iPos + 1);			
			sscanf(strTemp.c_str(),"%s", (char*)pDesc);
		}
	}		
	return true;
}

int CItemCfgMgr::ParseRefFoundInfo(const char* psect,const char* pkey,int iLevel)
{
	if (iLevel < 0 || iLevel > 7) return -1;

	string mapkey = psect;
	mapkey += pkey;

	StringMap::iterator it = m_mEffectCfg.find(mapkey);
	if(it == m_mEffectCfg.end()) return -1;

	string strValue = it->second;
	if(strValue.empty()) return -1;

	bool bFound = false;
	char* pValue = NULL;
	for (int i = 0;i<= iLevel;i++)
	{
		if (i == 0) pValue = strtok(const_cast<char*>(strValue.c_str()),"/");
		else pValue = strtok(NULL,"/");
		
		if (i == iLevel)
		{
			bFound = true;
			break;
		}
	}

	if (bFound) return atoi(pValue);

	return -1;
}

void CItemCfgMgr::ParseInfo(const char* psect,const char* pkey,string& strDes)
{
	if (!psect && !pkey) return;

	string mapkey = psect;
	mapkey += pkey;

	StringMap::iterator it = m_mEffectCfg.find(mapkey);
	if(it == m_mEffectCfg.end()) return;

	strDes = it->second;
}

void CItemCfgMgr::ReLoad()
{
	char strPath[256] = {0};
	sprintf_s(strPath,"%s\\config\\ItemCfg.ini",GetGameDataDir());
	std::ifstream fin(strPath);
	if(!fin.is_open())
		return;
	m_mEffectCfg.clear();
	VString strvect;
	while(!fin.eof())
	{
		string inbuf;
		getline(fin, inbuf,'\n');
		strvect.push_back(inbuf);
	}
	if(strvect.empty())
		return;
	for_each(strvect.begin(), strvect.end(), analyzeini(m_mEffectCfg));
	fin.close();
}

CPromptInfoMgr::CPromptInfoMgr()
{
	//LoadPromptInfo();
	LoadTaskTree();
	LoadAvaTask();//  [4/15/2010 dujun]
}

CPromptInfoMgr::~CPromptInfoMgr()
{
}

bool CPromptInfoMgr::ParsePromptInfo(string& strSect,string& strKey,string& strDes,DWORD& dwID,string& str)
{
	if (str.empty()) return false;
	
	int first =str.find('[');
	int last = str.rfind(']');
	if( first != string::npos && last != string::npos && first != last+1)
	{
		strSect = str.substr(first+1,last-first-1);
		return true;
	}

	strSect.clear();
	if((first=str.find('='))== string::npos) return false;

	string strtmp1= str.substr(0,first);
	string strtmp2=str.substr(first+1, string::npos);

	first= strtmp1.find_first_not_of("\t");
	last = strtmp1.find_last_not_of("\t");

	if(first == string::npos || last == string::npos) return false;

	strKey = strtmp1.substr(first, last - first+1);
	first = strtmp2.find_first_not_of("\t");
	if(((last = strtmp2.find("\t#", first )) != string::npos) ||
		((last = strtmp2.find("#", first )) != string::npos) ||
		((last = strtmp2.find("\t//", first )) != string::npos)||
		((last = strtmp2.find("//", first )) != string::npos))
	{
		strtmp2 = strtmp2.substr(0, last-first);
	}

	last = strtmp2.find_last_not_of("\t");
	if(first == string::npos || last == string::npos)return false;

	string value = strtmp2.substr(first, last-first+1);
	first = value.find_first_of('/');
	if (first == string::npos) return false;
	strtmp1 = value.substr(0,first);
	sscanf(strtmp1.c_str(),"%d",&dwID);
	strDes = value.substr(first+1,string::npos);
	return true;
}

bool CPromptInfoMgr::ParseTaskTree(string&strSect,string& strKey,string& strDes,string& str)
{
	if (str.empty()) return false;

	int first =str.find('[');
	int last = str.rfind(']');
	if( first != string::npos && last != string::npos && first != last+1)
	{
		strSect = str.substr(first+1,last-first-1);
		return true;
	}

	strSect.clear();
	if((first=str.find('='))== string::npos) return false;

	string strtmp1= str.substr(0,first);
	string strtmp2=str.substr(first+1, string::npos);

	first= strtmp1.find_first_not_of(" \t");
	last = strtmp1.find_last_not_of(" \t");

	if(first == string::npos || last == string::npos) return false;

	strKey = strtmp1.substr(first, last - first+1);
	first = strtmp2.find_first_not_of(" \t");
	if(((last = strtmp2.find(" \t#", first )) != string::npos) ||
		((last = strtmp2.find("#", first )) != string::npos) ||
		((last = strtmp2.find(" \t//", first )) != string::npos)||
		((last = strtmp2.find("//", first )) != string::npos))
	{
		strtmp2 = strtmp2.substr(0, last-first);
	}

	last = strtmp2.find_last_not_of(" \t");
	if(first == string::npos || last == string::npos)return false;

	strDes = strtmp2.substr(first, last-first+1);
	return true;
}

void CPromptInfoMgr::ParseTaskTree(string& str,string& strSimDes,string& strDes,string& strSubSimDes,string& strSubDes)
{
	if (str.empty()) return;
	
	string strTemp = str;
	string strTemp1 = "";
	int first =str.find('$');
	if (first != string::npos)
	{
		strTemp = str.substr(0,first);
		strTemp1 = str.substr(first+ 1);
	}
	
	ParseTaskTreeDetail(strTemp,strSimDes,strDes);
	ParseTaskTreeDetail(strTemp1,strSubSimDes,strSubDes);
}

void CPromptInfoMgr::ParseTaskTreeDetail(string& str,string& strSimDes,string& strDes)
{
	if (str.empty()) return;
	strSimDes.clear();
	strDes.clear();

	if (str.empty()) return;
	
	int first =str.find('\\');
	if (first != string::npos)
	{
		strSimDes = str.substr(0,first);
		strDes = str.substr(first+ 1);
	}
}


//  [4/19/2010 dujun]
void CPromptInfoMgr::LoadAvaTask()
{
	m_oriAvaTask.clear();

	char strPath[256] = {0};
	sprintf_s(strPath,"%s\\config\\AvaTask.txt",GetGameDataDir());
	std::ifstream fin(strPath);
	if(!fin.is_open()) return;

	VString strvect;
	while (!fin.eof())
	{
		string inbuf;
		getline(fin,inbuf,'\n');
		strvect.push_back(inbuf);
	}

	if(strvect.empty())  return;

	VString::iterator it = strvect.begin();
	while(it != strvect.end())
	{
		string& str = *it;
		string strTitle,strID,strDetail;
		bool bRet = ParseTaskTree(strTitle,strID,strDetail,str);
		if(bRet)
		{
			if(strTitle.empty())
			{
				TaskAva avaTaskNode;
				avaTaskNode.wTaskID = atoi(strID.c_str());
				ParseAvaTask(strDetail,avaTaskNode);
				m_oriAvaTask.push_back(avaTaskNode);
			}
		}

		it++;
	}

	fin.close();
}

void CPromptInfoMgr::ParseAvaTask(string&str,TaskAva& task)
{
	if(str.empty())
		return;

	string strTemp;
	strTemp.clear();

	char split = '\\';
	int findPos = std::string::npos;

	//1 获取任务名
	if((findPos = str.find_first_of(split)) != std::string::npos)
	{
		task.strTaskName = str.substr(0,findPos); 
		strTemp = str.substr(findPos+1);

		//2 获取次数
		if((findPos = strTemp.find_first_of(split)) != std::string::npos)
		{
			task.strTaskDetail = strTemp.substr(0,findPos);
			strTemp = strTemp.substr(findPos+1);

			//3 获取推荐等级
			if((findPos = strTemp.find_first_of(split)) != std::string::npos)
			{
				task.wLevel = atoi(strTemp.substr(0,findPos).c_str());
				strTemp = strTemp.substr(findPos+1);

				//4 NPC
				if((findPos = strTemp.find_first_of(split)) != std::string::npos)
				{
					task.strNPC = strTemp.substr(0,findPos);
					//5 寻路指令
					task.strPath = strTemp.substr(findPos+1);
				}
				
			}
		}
	}

}

void CPromptInfoMgr::UpdateAvaTask(int iLevel)
{
	m_vAvaTask.clear();

	for(int i = 0; i<m_oriAvaTask.size(); i++)
	{
		TaskAva& tmp = m_oriAvaTask.at(i);
		//若任务等级介于角色等级[-5,0]区间内，则可接
		//if(tmp.wLevel>= (iLevel - 5) && tmp.wLevel <= iLevel)
		if(tmp.wLevel <= iLevel)
			m_vAvaTask.push_back(tmp);
	}

}

void CPromptInfoMgr::LoadTaskTree()
{
	m_vTaskTree.clear();
	char strPath[256] = {0};
	sprintf_s(strPath,"%s\\config\\TaskTree.ini",GetGameDataDir());
	std::ifstream fin(strPath);
	if(!fin.is_open()) return;

	StringMap strMap;
	strMap.clear();

	VString strvect;
	while(!fin.eof())
	{
		string inbuf;
		getline(fin, inbuf,'\n');
		strvect.push_back(inbuf);
	}

	if(strvect.empty()) return;

	VString::iterator it = strvect.begin();
	while (it != strvect.end())
	{
		string& str = *it;
		string strSec,strKey,strDes;
		bool bRet = ParseTaskTree(strSec,strKey,strDes,str);

		if (bRet)
		{
			if (!strSec.empty())
			{
				TaskTree tasktree;
				tasktree.bSequence = (*(strSec.c_str()) - '1' == 0);
				tasktree.strTheme.assign(strSec.c_str() + 1);
				m_vTaskTree.push_back(tasktree);
			}
			else
			{
				TaskTreeNode tasknode;
				ParseTaskTree(strDes,tasknode.strSimDes,tasknode.strDes,tasknode.strSubSimDes,tasknode.strSubDes);
				if (m_vTaskTree.size() > 0)
					m_vTaskTree[m_vTaskTree.size() - 1].vTask.push_back(tasknode);
			}
		}

		it++;
	}

	fin.close();
}

void CPromptInfoMgr::LoadPromptInfo()
{
	m_vPromptInfo.clear();
	
	char strPath[256] = {0};
	sprintf_s(strPath,"%s\\config\\PromptInfo.ini",GetGameDataDir());

	std::ifstream fin(strPath);
	if(!fin.is_open()) return;
	
	StringMap strMap;
	strMap.clear();

	VString strvect;
	while(!fin.eof())
	{
		string inbuf;
		getline(fin, inbuf,'\n');
		strvect.push_back(inbuf);
	}
	
	if(strvect.empty()) return;

	VString::iterator it = strvect.begin();
	while (it != strvect.end())
	{
		DWORD dwID = 0;
		string& str = *it;
		string strSec,strKey,strDes;
		bool bRet = ParsePromptInfo(strSec,strKey,strDes,dwID,str);

		if (bRet)
		{
			if (!strSec.empty())
			{
				SPromptInfo spromInfo;
				spromInfo.dwSecID = 0;
				spromInfo.strSec = strSec;
				m_vPromptInfo.push_back(spromInfo);
			}
			else
			{
				_sPromptInfo spi;
				spi.dwID = dwID;
				spi.strTile = strKey;
				spi.strDes = strDes;
				if (m_vPromptInfo.size() > 0)
					m_vPromptInfo[m_vPromptInfo.size() - 1].vPromptInfo.push_back(spi);
			}
		}

		it++;
	}
	
	m_vNewHandInfo.clear();
	m_vActivityInfo.clear();
	for (int i = m_vPromptInfo.size() - 1;i>=0;i--)
	{
		SPromptInfo& proInfo = m_vPromptInfo.at(i);

		if(proInfo.strSec.compare("NewHandPrompt") == 0)
		{
			std::vector<_sPromptInfo>& vpi = proInfo.vPromptInfo;
			std::vector<_sPromptInfo>::iterator itv = vpi.begin();

			while (itv != vpi.end())
			{
				_sPromptInfo spi;
				spi.dwID = itv->dwID;
				spi.bEmergent = (itv->strDes.at(0) == '1');
				spi.strDes.assign(itv->strDes.c_str() + 1);
				spi.strTile = itv->strTile;

				m_vNewHandInfo.push_back(spi);

				itv++;
			}

			m_vPromptInfo.erase(m_vPromptInfo.begin() + i);			
		}
		else if(proInfo.strSec.compare("TodayActivity") == 0)
		{
			std::vector<_sPromptInfo>& vpi = proInfo.vPromptInfo;
			std::vector<_sPromptInfo>::iterator itv = vpi.begin();

			while (itv != vpi.end())
			{
				_ActivityInfo ati;
				ati.dwID = itv->dwID;
				ati.strName = itv->strTile;

				char* pChar = strtok(const_cast<char*>(itv->strDes.c_str()),"$");
				if(NULL == pChar)
				{
					itv++;
					continue;
				}
				ati.strTime = pChar;

				pChar = strtok(NULL,"$");
				if (NULL == pChar)
				{
					itv++;
					continue;
				}
				ati.strActType = pChar;

				pChar = strtok(NULL,"$");
				if (NULL == pChar)
				{
					itv++;
					continue;
				}
				ati.strConfine = pChar;				

				pChar = strtok(NULL,"$");
				if (NULL == pChar)
				{
					itv++;
					continue;
				}
				ati.strAward = pChar;

				pChar = strtok(NULL,"$");
				if (NULL == pChar)
				{
					itv++;
					continue;
				}
				ati.strFinishNum = pChar;

				pChar = strtok(NULL,"$");
				if (NULL == pChar)
				{
					itv++;
					continue;
				}
				ati.strNPCName = pChar;

				pChar = strtok(NULL,"$");
				if (NULL == pChar)
				{
					itv++;
					continue;
				}
				ati.strNPCCont = pChar;

				m_vActivityInfo.push_back(ati);

				itv++;
			}

			m_vPromptInfo.erase(m_vPromptInfo.begin() + i);
		}
		else if(proInfo.strSec.compare("ActivityNow") == 0)
		{
			std::vector<_sPromptInfo>& vpi = proInfo.vPromptInfo;
			std::vector<_sPromptInfo>::iterator itv = vpi.begin();

			while (itv != vpi.end())
			{
				TimerActivityInfo tati;
				tati.dwID = itv->dwID;
				tati.strTile = itv->strTile;

				char* pChar = strtok(const_cast<char*>(itv->strDes.c_str()),"$");
				if(NULL == pChar)
				{
					itv++;
					continue;
				}

				string strTm = pChar;
				std::replace(strTm.begin(),strTm.end(),':','.');
				sscanf(strTm.c_str(),"%f-%f",&tati.fStartTm,&tati.fEndTm);

				pChar = strtok(NULL,"$");
				if (NULL == pChar)
				{
					itv++;
					continue;
				}
				tati.strContent = pChar;

				m_vTimerActInfo.push_back(tati);

				itv++;
			}

			m_vPromptInfo.erase(m_vPromptInfo.begin() + i);
		}
		else if(proInfo.strSec.compare("ThemeActivity") == 0)
		{
			std::vector<_sPromptInfo>& vpi = proInfo.vPromptInfo;
			std::vector<_sPromptInfo>::iterator itv = vpi.begin();

			while (itv != vpi.end())
			{
				ThemeInfo themeinfo;
				themeinfo.wID = (WORD)itv->dwID;
				themeinfo.strSec = itv->strTile;
				themeinfo.strTheme = itv->strDes;
				m_mThemeInfo[(WORD)(itv->dwID)] = themeinfo;
				itv++;
			}

			m_vPromptInfo.erase(m_vPromptInfo.begin() + i);	
		}
		else if (proInfo.strSec.compare("GameThemeActivity") == 0)
		{
			std::vector<_sPromptInfo>& vpi = proInfo.vPromptInfo;
			std::vector<_sPromptInfo>::iterator itv = vpi.begin();

			while (itv != vpi.end())
			{
				ThemeInfo themeinfo;
				themeinfo.wID = (WORD)itv->dwID;
				themeinfo.strSec = itv->strTile;
				themeinfo.strTheme = itv->strDes;
				m_mGameThemeInfo[(WORD)(itv->dwID)] = themeinfo;
				itv++;
			}

			m_vPromptInfo.erase(m_vPromptInfo.begin() + i);	
		}
	}

	fin.close();
}

_sPromptInfo* CPromptInfoMgr::FindPromptInfo(DWORD dwID)
{
	if(dwID == 0) return NULL;

	VSPromptInfo::iterator it = m_vPromptInfo.begin();
	while(it != m_vPromptInfo.end())
	{
		std::vector<_sPromptInfo>& vpi = it->vPromptInfo;

		std::vector<_sPromptInfo>::iterator itv = vpi.begin();

		while (itv != vpi.end())
		{
			if(itv->dwID == dwID)
			{
				//找到
				return &(*itv);
			}
			itv++;
		}
		it++;
	}
	return NULL;
}

_sPromptInfo* CPromptInfoMgr::FindNewHandProInfo(DWORD dwID)
{
	if(dwID == 0) return NULL;

	std::vector<_sPromptInfo>::iterator itv = m_vNewHandInfo.begin();

	while (itv != m_vNewHandInfo.end())
	{
		if(itv->dwID == dwID)
		{
			//找到
			return &(*itv);
		}
		itv++;
	}

	return NULL;
}

int CPromptInfoMgr::FindTaskTree(string& str)
{
	int iRet = -1;
	if (str.empty()) return iRet;

	VTaskTree::iterator it = m_vTaskTree.begin();

	while(it != m_vTaskTree.end())
	{
		iRet++;
		if (it->strTheme.compare(str.c_str()) == NULL)
		{
			return iRet;
		}

		it++;
	}

	return -1;
}

void CPromptInfoMgr::GetActivityInfo(_ActivityInfo** pArray)
{
	if (!pArray || m_vActivityInfo.size() < 2) return;

	std::vector<_ActivityInfo>::iterator it =  m_vActivityInfo.begin();
	pArray[0] = &(*it);
	it++;
	pArray[1] = &(*it);
	it++;

	int iNum = 2;
	SYSTEMTIME stimenow;
	GetLocalTime(&stimenow);
	float fTm = stimenow.wHour + (float)stimenow.wMinute/(float)100;

	float fTm1,fTm2;
	while (it != m_vActivityInfo.end() && iNum < 4)
	{
		string strTm = it->strTime;
		std::replace(strTm.begin(),strTm.end(),':','.');
		sscanf(strTm.c_str(),"%f-%f",&fTm1,&fTm2);

		if (fTm >= fTm1 && fTm <= fTm2)
		{
			pArray[iNum] = &(*it);
			iNum++;
		}
		else if (fTm < fTm1)
		{
			pArray[iNum] = &(*it);
			iNum++;
		}

		it++;
	}

	it =  m_vActivityInfo.begin();
	it++;
	it++;
	if (iNum < 3)
	{
		if (m_vActivityInfo.size() >= 3)
		{
			it++;
			pArray[3] = &(*it);
		}
	}
	
	if (iNum < 4)
	{
		if (m_vActivityInfo.size() >= 4)
		{
			it++;
			pArray[3] = &(*it);
		}
	}
}

_ActivityInfo* CPromptInfoMgr::GetActivityInfo(DWORD dwID)
{
	std::vector<_ActivityInfo>::iterator it =  m_vActivityInfo.begin();
	while (it != m_vActivityInfo.end())
	{
		if (!dwID) return &(*it);

		if (it->dwID == dwID)
		{
			return &(*it);
		}

		it++;
	}

	return NULL;
}

ThemeInfo* CPromptInfoMgr::GetThemeInfo(WORD wID)
{
	std::map<WORD,ThemeInfo>::iterator it =  m_mThemeInfo.find(wID);

	if (it != m_mThemeInfo.end())
	{
		return const_cast<ThemeInfo*>(&(it->second));
	}

	return NULL;
}

ThemeInfo* CPromptInfoMgr::GetGameThemeInfo(WORD wID)
{
	std::map<WORD,ThemeInfo>::iterator it =  m_mGameThemeInfo.find(wID);

	if (it != m_mGameThemeInfo.end())
	{
		return const_cast<ThemeInfo*>(&(it->second));
	}

	return NULL;
}


//以下代码把原来的tips导入到xml
////配置物品说明
//string str;
//FILE *fp = fopen("c:\\TipsCfg.xml","wb");
//fwrite("<?xml version=\"1.0\" encoding=\"gb2312\" standalone=\"yes\" ?>\r\n",strlen("<?xml version=\"1.0\" encoding=\"gb2312\" standalone=\"yes\" ?>\r\n"),1,fp);
//fwrite("<TipsList Ver=\"1\">\r\n",strlen("<TipsList Ver=\"1\">\r\n"),1,fp);
//
//
//StringMap &smap = g_ItemCfgMgr.GetMap();
//for (StringMap::iterator itr = smap.begin();itr != smap.end();itr++)
//{
//	str = "    <tip goodsname = \"";
//	str += itr->first.substr(4,itr->first.length() - 4) + "\">\r\n";
//	fwrite(str.c_str(),str.length(),1,fp);
//
//	string strAddTips = itr->second;
//	int iStart = 0;
//	std::string strTemp = "";
//
//	while (iStart < strAddTips.size())
//	{
//		strTemp = StringUtil::toStr(strAddTips,iStart);
//
//		str = "        <line>\r\n";
//		fwrite(str.c_str(),str.length(),1,fp);
//		str = "            <col content = \"";
//		str += strTemp + "\"/>\r\n";
//		fwrite(str.c_str(),str.length(),1,fp);
//		str = "        </line>\r\n";
//		fwrite(str.c_str(),str.length(),1,fp);
//	}
//
//	str ="    </tip>\r\n";
//	fwrite(str.c_str(),str.length(),1,fp);
//}
//
//str ="</TipsList>\r\n";
//fwrite(str.c_str(),str.length(),1,fp);
//fclose(fp);

