#include "TagText.h"
#include "Global/StringUtil.h"
#include "GameControl/GameControl.h"
#include "Global/Interface/TexManagerInterface.h"
#include "GameGlobal.h"

CTagTextMgr g_TagTextMgr;

map<std::string,CGood> CTagText::sm_GoodMap;

CTagText::CTagText(void)
{
	Clear();
}

CTagText::~CTagText(void)
{
	Clear();
}

void CTagText::Clear()
{
	m_content.clear();
}

void CTagText::setText(string& str)
{
	vector<_Tag> strTemp;
	_Tag con;
	con.text = str;

	m_content.clear();
	m_content.push_back(strTemp);
	m_content[0].push_back(con);
}

bool CTagText::ParsePic(const string& str,int nLeft,int nRight,int j,size_t &nLinePos,int iOffX,int iOffY)
{
	//</@@!PIC 物品名称>
	std::string strTemp = str; 
	int nTemp = strTemp.find("!PIC",nLeft);
	if(nTemp < 0 || nTemp >= nRight)
		return false;

	//</@@!PIC2 图包编号,图片在图包中的位置>
	int nTemp2 = strTemp.find("!PIC2",nLeft);
	if(nTemp2 >= 0 && nTemp2 < nRight)
	{
		_Tag con;

		int iPos1 = strTemp.find(",",nTemp);
		int iPos2 = strTemp.find(">",nTemp);
		if(iPos1 < 0 || iPos2 < 0 || iPos2 > nRight || iPos1 >= nRight)
			return false;

		con.iOffX = iOffX;
		con.iOffY = iOffY;
		con.color = 0xFFFFFFFF;
		std::string strPackage = strTemp.substr(nTemp+6,iPos1-(nTemp+6) );
		std::string strIndex = strTemp.substr(iPos1+1,iPos2-(iPos1+1) );
		int iPackage = atoi(strPackage.c_str());
		int iIndex = atoi(strIndex.c_str());

		if(iPackage == 10)//原来的PACKAGE_items
			iPackage = PACKAGE_items;

		con.dwGoodLooks = MAKELONG(iIndex,iPackage);

		m_content[j].push_back(con);
		nLinePos += iPos2 - nLeft -1;
	}
	else
	{
		_Tag con;
		int iPos1 = strTemp.find(">",nTemp);
		if(iPos1 < 0 || iPos1 > nRight)
			return false;

		con.iOffX = iOffX;
		con.iOffY = iOffY;
		con.color = 0xFFFFFFFF;
		con.strGoodName = strTemp.substr(nTemp+5,iPos1-nTemp-5);

		m_content[j].push_back(con);
		nLinePos += iPos1 - nLeft -1;

		map<string,CGood>  &npcGoods = CTagText::sm_GoodMap;
		if(npcGoods.find(con.strGoodName) == npcGoods.end())
		{
			//先发一个空物品进去，再请求相关物品数据，如果再请求失败，后面判断到空上物品为空时会重新请求
			//因为任务中的物品是在登录游戏之前发过来的，那时请求物品数据的协议都不会回应
			CGood tempGood;
			tempGood.SetName(con.strGoodName.c_str());
			tempGood.SetID(0);
			sm_GoodMap[con.strGoodName] = tempGood;

			g_pGameControl->SEND_Good_Info_Require(con.strGoodName.c_str());
		}
	}

	return true;
}


void CTagText::Parse(string& str,int nPos,const char* sLR,int iLimit,DWORD dwDefaultColor)
{
	m_content.clear();
	if(sm_GoodMap.size() > 30)
		sm_GoodMap.clear();

	VString row;
	if(nPos <= 0)
	{
		nPos = 0;
	}

	while(nPos < (int)str.size())
	{
		int temp = str.find_first_of(sLR,nPos);

		if(temp == string::npos)
		{
			row.push_back(str.substr(nPos,str.size() - nPos));
			break;
		}
		row.push_back(str.substr(nPos,temp - nPos));

		nPos = temp + 1;
		if(str[nPos] == 0x0D) // 略过 0D
		{
			nPos++;
		}
		if(str[nPos] == 0x0A) // 略过 0A
		{
			nPos++;
		}
	}

	for(unsigned int i = 0 ,j = 0; i < row.size(); i++,j++)
	{
		std::vector<_Tag> VTagTemp;
		_Tag con;
		m_content.push_back(VTagTemp);

		int iPos = 0;
		string szContent;
		while(iPos >= 0)
		{
			//找到偏移
			int iOffX = 0,iOffY = 0;
			bool bFindOffXY = false;
			int iOffBegin = row[i].find("<off",iPos);
			int iOffEnd = string::npos;
			if (iOffBegin != string::npos)
			{
				int iOffXStartPos = row[i].find("x=\"",iOffBegin);
				int iOffXEndPos = row[i].find("\"",iOffXStartPos + 3);
				if (iOffXStartPos != string::npos && iOffXEndPos != string::npos)
				{
					string strOffX = row[i].substr(iOffXStartPos + 3,iOffXEndPos - (iOffXStartPos + 3));
					iOffX = atoi(strOffX.c_str());
				}
				int iOffYStartPos = row[i].find("y=\"",iOffBegin);
				int iOffYEndPos = row[i].find("\"",iOffYStartPos + 3);
				if (iOffYStartPos != string::npos && iOffYEndPos != string::npos)
				{
					string strOffY = row[i].substr(iOffYStartPos + 3,iOffYEndPos - (iOffYStartPos + 3));
					iOffY = atoi(strOffY.c_str());
					iOffYEndPos = row[i].find(">",iOffYEndPos + 1);
				}

				if (iOffYEndPos != string::npos)
				{
					//row[i].erase(iOffBegin,iOffYEndPos - iOffBegin + 1);//删除偏移标记
					iOffEnd = row[i].find("</off>",iOffYEndPos + 1);
					if (iOffEnd != string::npos && iOffEnd > iOffYEndPos)
					{
						//row[i].erase(iOffEnd,6);//删除偏移标记
						szContent = row[i].substr(iOffYEndPos + 1,iOffEnd - (iOffYEndPos + 1));
						bFindOffXY = true;
						iPos = iOffEnd + 6;
						if (iPos >= row[i].size())
						{
							iPos = -1;
						}
					}
				}
			}	

			if (!bFindOffXY)
			{
				szContent = row[i].substr(iPos,-1);
				iPos = -1;
			}
			//end 偏移///////



			int nLeft = szContent.find_first_of("<");
			if(nLeft == string::npos)
			{  
				std::string subStr;
				int subpos =0;
				if(iLimit > 0)//iLimit的长度就自动换行 
				{			
					VString   vCutStr;
					CutByUnicode(szContent.c_str(),vCutStr,iLimit);
					int iVCutSize = vCutStr.size();
					for(int ic = 0; ic < iVCutSize; ic++)
					{
						con.iOffX = iOffX;
						con.iOffY = iOffY;
						iOffY = 0;
						con.text = vCutStr[ic];
						con.color = (dwDefaultColor!= 0)?dwDefaultColor:COLOR_DEFAULT;
						con.comm.clear();
						if(ic == iVCutSize - 1)//自动换行的最后一行
						{
							m_content[j].push_back(con);
						}
						else
						{
							m_content[j++].push_back(con);
							m_content.push_back(VTagTemp);		
						}
					}

					continue;
				}

				con.iOffX = iOffX;
				con.iOffY = iOffY;
				con.color = (dwDefaultColor != 0)?dwDefaultColor:COLOR_DEFAULT;
				con.comm.clear();
				con.text = szContent;
				m_content[j].push_back(con);

				continue;
			}

			int nBegin = 0;
			int nRight = szContent.find_first_of(">");

			while(nBegin <= (int)szContent.size())
			{
				con.iOffX = iOffX;
				iOffX = 0;//一个[off][/off]标签中只有第一列要赋值,后面不要赋,会自动向后偏移,不然就会操成多次偏移
				con.iOffY = iOffY;

				con.color = (dwDefaultColor != 0)?dwDefaultColor:COLOR_DEFAULT;
				con.text.clear();
				con.comm.clear();
				if(nRight == string::npos)
				{
					if(szContent.size() > nBegin)
					{
						con.text = szContent.substr(nBegin,szContent.size() - nBegin);
						m_content[j].push_back(con);
					}
					break;
				}
				if(nLeft != nBegin)         //中间有字
				{
					con.text = szContent.substr(nBegin,nLeft - nBegin);
					m_content[j].push_back(con);
					con.text.clear();
				}

				//看看是不是一张图片
				size_t nLinePos = 0;
				bool bParseed = false;
				if(nLeft < nRight)
				{
					bParseed |= ParsePic(szContent,nLeft,nRight,j,nLinePos,con.iOffX,con.iOffY);
				}

				int nTemp = szContent.find("/@",nLeft);
				if(nTemp == string::npos)
				{
					nTemp = szContent.find_first_of("/",nLeft);
				}

				if(!bParseed && nTemp != string::npos && nTemp < nRight)
				{
					con.text=szContent.substr(nLeft + 1,nTemp - nLeft - 1);
					con.comm=szContent.substr(nTemp + 1,nRight - nTemp - 1);
					m_content[j].push_back(con);
				}
				else if(!bParseed)
				{
					std::string comm = szContent.substr(nLeft + 1,nRight - nLeft - 1);
					std::string colorstr;
					int iPos = comm.find_first_of(' ');
					if(iPos >= 6)
					{
						colorstr = comm.substr(6,iPos - 6);
					}
					else
					{
						iPos = 0;
					}
					con.text=comm.substr(iPos+1,comm.size() - iPos - 1);

					if(colorstr.empty())
						con.color = (dwDefaultColor != 0)?dwDefaultColor:COLOR_DEFAULT;
					else if(stricmp(colorstr.c_str(),"red") == 0)
						con.color = 0xFFFF0000;
					else if(stricmp(colorstr.c_str(),"green") == 0)
						con.color = 0xFF00FF00;
					else if (stricmp(colorstr.c_str(),"ggreen") == 0)
						con.color = 0xFF87C23A;
					else if(stricmp(colorstr.c_str(),"blue") == 0)
						con.color = 0xFF0000FF;
					else if(stricmp(colorstr.c_str(),"black") == 0)
						con.color = 0xFF000000;
					else if(stricmp(colorstr.c_str(),"yellow") == 0)
						con.color = 0xFFFFFF00;
					else if(stricmp(colorstr.c_str(),"orange") == 0)
						con.color = 0xFFFF8000;
					else if(stricmp(colorstr.c_str(),"purple") == 0)
						con.color = 0xFFFF00FF;
					else if(stricmp(colorstr.c_str(),"gray") == 0)
						con.color = 0xFFC0C0C0;
					else if(stricmp(colorstr.c_str(),"white") == 0)
						con.color = 0xFFFFFFFF;
					else if(stricmp(colorstr.c_str(),"graygolden") == 0)
						con.color = 0xFFDEAA52;
					else
					{
						con.color = strtoul(colorstr.c_str(),NULL,16);
						con.color |= 0xFF000000;
					}
					m_content[j].push_back(con);
				}
				nBegin = nRight + 1;
				nLeft = szContent.find_first_of("<",nBegin);
				nRight = szContent.find_first_of(">",nBegin);
			}


		}		

	}
}


//////////////////////////////////////////////////////////////////////////////////
//NPC文字界面数据
int  CTagText::GetRow()
{ 
	return (int)m_content.size();
}

int  CTagText::GetColumn(int n)
{ 
	if(n > (int)m_content.size())
		return 0;
	return m_content.at(n).size();
}

bool  CTagText::IsCommand(int row,int col)
{
	_Tag* con = GetAt(row,col);
	if(con)
	{
		if(con->comm.size() == 0)
			return false;
		else
			return true;
	}
	return false;
}

const char *  CTagText::GetString(int row,int col)
{
	_Tag* con = GetAt(row,col);
	if(con)
		return con->text.c_str();
	else
		return NULL;
}

bool CTagText::IsGoodName(int row,int col)
{
    _Tag* con = GetAt(row,col);
    if(con)
    {
        if(con->strGoodName.size() == 0)
            return false;
        else
            return true;
    }
    return false;
}

const char* CTagText::GetTagGoodName(int row,int col)
{
    _Tag* con = GetAt(row,col);
    if(con)
        return con->strGoodName.c_str();
    else
        return NULL;
}

bool CTagText::IsGoodLooks(int row,int col)
{
    _Tag* con = GetAt(row,col);
    if(con)
    {
        if(con->dwGoodLooks == 0)
            return false;
        else
            return true;
    }
    return false;
}

DWORD CTagText::GetTagGoodLooks(int row,int col)
{
    _Tag* con = GetAt(row,col);
    if(con)
        return con->dwGoodLooks;
    else
        return 0;
}

DWORD CTagText::GetColor(int row,int col)
{
	_Tag* con = GetAt(row,col);
	if(con)
		return con->color;
	else
		return COLOR_DEFAULT;
}

const char *  CTagText::GetCommand(int row,int col)
{
	_Tag* con = GetAt(row,col);
	if(con)
		return con->comm.c_str();
	else
		return ""; 
}

_Tag* CTagText::GetAt(int row,int col)
{
	if(row < 0 || col < 0 || row >= (int)m_content.size())
		return NULL;

	 if(col >= (int)m_content[row].size())
		 return NULL;

	 return &(m_content.at(row).at(col));
}
void CTagText::ReplaceAll(const string &src,const string &dest)
{
	for (int i = 0; i < m_content.size(); i++)
	{
		vector<_Tag> &VTag = m_content[i];
		for(int j = 0; j < VTag.size(); j ++)
		{
			StringUtil::replace_all(VTag[j].text,src,dest);
		}
	}
}
CTagTextMgr::CTagTextMgr()
{
	Clear();
}

CTagTextMgr::~CTagTextMgr()
{
}

void CTagTextMgr::Clear()
{
	m_NpcText.Clear();
	m_commonData.Clear();
	m_bookData.Clear();
	m_guideData.Clear();
	m_boardData.Clear();
	m_NpcText2.Clear();
	m_GreetingData.Clear();
	m_GreetingDownloadData.Clear();
	m_NewBieData.Clear();
	m_RadioData.Clear();
	m_QiYuText.Clear();	
	m_YiHuoZhanMsg.Clear();
}