#include "UiManager.h"
#include <assert.h>
#include "Global/StringUtil.h"
#include "Control.h"
#include "CtrlWindowX.h"
#include "Global/Interface/StreamInterface.h"

//------------------------------//
//			窗口名字和路径		//
//------------------------------//
BEGIN_VAR(CXmlUiPath, CXmlObj)
INIT_VAR(string, Path, "")
END_VAR(CXmlUiPath, CXmlObj)

BEGIN_VAR(CXmlUiEntry, CXmlObj)
INIT_VAR(string, Name, "")
INIT_VAR(string, Path, "")
END_VAR(CXmlUiEntry, CXmlObj)

void CXmlUiEntry::Clear()
{
	SUPERCLASS::Clear();
	m_xmlPaths.clear();
}

bool CXmlUiEntry::InitChildElement(const char* pValue)
{
	if(SUPERCLASS::InitChildElement(pValue))
		return true;

	assert(pValue);
	if(stricmp(pValue,"path") == 0)
	{
		CXmlUiPath path;
		path.Create();
		m_xmlPaths.push_back(path);
		return true;
	}

	return false;
}

void CXmlUiEntry::GetAllPath(VString& path)
{
	path.clear();

	if(GetPath().size() != 0)
		path.push_back(GetPath());

	for(int ii = 0;ii < m_xmlPaths.size();ii++)
	{
		string th = m_xmlPaths.at(ii).GetPath();
		if(th.size() != 0)
		{
			path.push_back(th);
		}
	}
}



//------------------------------//
//			UI管理器			//
//------------------------------//
BEGIN_VAR(CUiManager, CXmlObj)
END_VAR(CUiManager, CXmlObj)

CUiManager::CUiManager()
{
	m_wins.clear();

	string path = StringUtil::format("ui\\ui.xml");
	LoadFile(path.c_str());
}

CUiManager::~CUiManager()
{
	this->Clear();
}

void CUiManager::Clear()
{
	SUPERCLASS::Clear();

	//清除
	m_entrys.clear();
	while(!m_wins.empty())
	{
		MXmlWindow::iterator itr = m_wins.begin();
		SAFE_DELETE(itr->second);
		m_wins.erase(itr);
	}
}

bool CUiManager::LoadFile(const char* path)
{
	this->Clear();

	DataStreamInterface* stream = NULL;
	//优先使用目录里的文件
	string dirpath = GetGameDataDir();
	dirpath = dirpath + "\\" + path;
	if(!CXmlObj::LoadFile(dirpath.c_str()))
	{
		stream = g_pStreamMgr->OpenDataFile(path,false,false,false,EP_UI);
		if(!stream)
		{
			return false;
		}

		string buf;
		while(!stream->eof())
		{
			buf += stream->getLine();
			buf += "\n";
		}        
		SAFE_DELETE(stream);

		if(!CXmlObj::LoadFromBuff(buf.c_str()))
			return false;
	}

	MXmlUiEntry::iterator itr;
	for(itr = m_entrys.begin();itr != m_entrys.end();itr++)
	{
		CXmlWindow* head = NULL;

		VString vstr;
		itr->second.GetAllPath(vstr);

		for(int ii = 0;ii < vstr.size();ii++)
		{
			string filename = StringUtil::format("ui\\%s",vstr.at(ii).c_str());

			dirpath = GetGameDataDir();
			dirpath = dirpath + "\\" + filename;

			CXmlWindow* win = new CXmlWindow();
			//优先使用目录里的文件
			if(!win->LoadFile(dirpath.c_str()))
			{
				stream = g_pStreamMgr->OpenDataFile(filename.c_str(),false,false,false,EP_UI);
				if(!stream)
				{
					output_debug("Xml Log: Failed to load '%s'\n",filename.c_str());
					SAFE_DELETE(win);
					continue;
				}

				string strBuf;
				while(!stream->eof())
				{
					strBuf += stream->getLine();
					strBuf += "\n";
				}        
				SAFE_DELETE(stream);

				if(!win->LoadFromBuff(strBuf.c_str()))
				{
					output_debug("Xml Log: Failed to load '%s'\n",filename.c_str());
				}
			}

			if(head)
				head->AddSibling(win);
			else
				head = win;
		}
		if(head)
		{
			m_wins[itr->first] = head;
		}
	}
	return true;
}

bool CUiManager::InitChildElement(const char* pValue)
{
	if(SUPERCLASS::InitChildElement(pValue))
		return true;

	assert(pValue);
	if(stricmp(pValue, "window") == 0)
	{
		CXmlUiEntry entry;
		entry.Create();
		m_entrys[entry.GetName()] = entry;

		return true;
	}
	return false;
}

CXmlWindow*	CUiManager::GetXmlWindow(const string& name,int iPos)
{
	MXmlWindow::iterator itor = m_wins.find(name);
	CXmlWindow* pXmlWindow = NULL;
	if(itor != m_wins.end())
	{
		pXmlWindow = itor->second;
		while(iPos > 0 && pXmlWindow)
		{
			pXmlWindow = pXmlWindow->GetSibling();
			iPos --;
		}
	}
 
	return pXmlWindow;
}

