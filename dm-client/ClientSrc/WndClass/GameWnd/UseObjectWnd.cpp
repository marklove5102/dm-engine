#include "UseObjectWnd.h"
#include "GameControl\GameControl.h"
#include "GameData/GameData.h"
#include "GameMap/GameMap.h"
#include "GameData/MsgBoxMgr.h"
#include "GameData/OtherData.h"
#include "GameMap/MinMap.h"
#include <algorithm>

INIT_WND_POSX(CUseObjectWnd,POS_AUTO,POS_AUTO)

CUseObjectWnd::CUseObjectWnd()
{
	m_iObjectType = sm_dwWindowType;
	m_dwObjID = 0;
	m_iMonsterNum = 0;

	m_iIndex = 14501;
	m_iPages = 1;
	

}	

CUseObjectWnd::~CUseObjectWnd(void)
{
}

void CUseObjectWnd::Draw(void)
{	
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	//绘制说明文字
	DrawMsg();
	//绘制列表框内容
	DrawLineData();

}

void CUseObjectWnd::OnCreate()
{
	//列表控件
	m_pGrid = new CCtrlGrid();
	AddControl(m_pGrid);
	m_pGrid->Create(this,18,80,202,252,USE_OBJECT_LINE_COUNT,19.0f);
	//m_pGrid->AddUpButton(168,0,51,52,53);
	//m_pGrid->AddDownButton(168,158,56,57,58);
	m_pGrid->AddScrollEx(168,0,16,172);
	m_pGrid->SetLinesPerPage(USE_OBJECT_LINE_COUNT);
	m_pGrid->SetDrawOffXY(0,7);
	m_pGrid->PushColumn(110);
	m_pGrid->SetTextColor(0xFF9E6D34, COLOR_BTN_MOUSEON);
	m_pGrid->SetBackColor(0);
	m_pGrid->SetSelBackColor(0);
	m_pGrid->SetSelTextColor(COLOR_BTN_PRESS);
	m_pGrid->SetFont(FONT_YAHEI,FONTSIZE_SMALL);

	//确认
	m_pSubmitBtn = new CCtrlButton();
	AddControl(m_pSubmitBtn);
	m_pSubmitBtn->CreateEx(this,71,258,90,91,92);
	m_pSubmitBtn->SetText("确定", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE, 0, FONT_YAHEI);

	m_pEdit = new CCtrlEdit();
	AddControl(m_pEdit);
	m_pEdit->Create(this,FONTSIZE_DEFAULT,COLOR_BTN_NORMAL,61,55,105,15);
	m_pEdit->SetMaxLength(14);


	SetCloseButton(188,1, 80);

	switch(m_iObjectType)
	{
	case 1:
		CreateFLSSList();
		break;
	case 2:
		CreateSTSZList();		
		break;
	case 3:
		CreateJYXLList();
		break;
	case 4:
		CreateHCSSList();
		break;

	default:
		break;
	}

	m_pGrid->SetTotalCount((int)m_VLine.size());
	if(m_VLine.size() > 0)
		m_pEdit->SetText(m_VLine[0].c_str(), COLOR_TEXT_NORMAL, COLOR_TEXT_NORMAL, COLOR_TEXT_NORMAL, -1, FONTSIZE_DEFAULT, 0, FONT_YAHEI);

}	

bool CUseObjectWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch(dwMsg)
	{
	case MSG_CTRL_USE_OBJECT_WND:
		{
			SetLines((const char*)pControl);
			return true;
		}
	case OPER_CREATE:
		{
			m_dwObjID = dwData;
			return true;
		}
	case MSG_CTRL_BUTTON_CLICK:
		{
			if(pControl == m_pSubmitBtn)//确认
			{
				OnSubmit();
				return true;
			}
		}
		break;
	case MSG_CTRL_GRID_DBCLICK:
		{
			int row = m_pGrid->GetSelLine();
			if(row >= 0 && row < (int)m_VLine.size())
			{
				m_pEdit->SetText(m_VLine[row].c_str(), COLOR_TEXT_NORMAL, COLOR_TEXT_NORMAL, COLOR_TEXT_NORMAL, -1, FONTSIZE_DEFAULT, 0, FONT_YAHEI);
				OnSubmit();
			}
			return true;
		}
		break;
	case MSG_CTRL_GRID_SELECT_LINE_COL:
		{
			int row = m_pGrid->GetSelLine();
			if(row >= 0 && row < (int)m_VLine.size())
			{
				m_pEdit->SetText(m_VLine[row].c_str(), COLOR_TEXT_NORMAL, COLOR_TEXT_NORMAL, COLOR_TEXT_NORMAL, -1, FONTSIZE_DEFAULT, 0, FONT_YAHEI);
			}
			return true;
		}
		break;
	default:
		break;
	}

	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CUseObjectWnd::DrawMsg()
{
	switch(m_iObjectType)
	{
	case 1:
	case 2:
		g_pFont->DrawText(m_iScreenX + 50, m_iScreenY + 27, "请输入您想要传送到",0xFFFFFF00, FONT_YAHEI);  
		g_pFont->DrawText(m_iScreenX + 57, m_iScreenY + 42, "其身边的目标名字",0xFFFFFF00, FONT_YAHEI);      
		break;
	case 3:
		g_pFont->DrawText(m_iScreenX + 50, m_iScreenY + 27, "请输入您想要传送到",0xFFFFFF00, FONT_YAHEI);    
		g_pFont->DrawText(m_iScreenX + 57, m_iScreenY + 42, "其身边的好友名字",0xFFFFFF00, FONT_YAHEI);     
		break;
	case 4:
		g_pFont->DrawText(m_iScreenX + 25, m_iScreenY + 37, "请设置您想要传送到的城市名字",0xFFFFFF00, FONT_YAHEI);      
		break;
	case 5:
		g_pFont->DrawText(m_iScreenX + 42, m_iScreenY + 37, "请选择想要召唤出的怪物",0xFFFFFF00, FONT_YAHEI);        
		break;
	default:
		break;
	}

}

void CUseObjectWnd::DrawLineData()
{
	size_t iNum = m_VLine.size();
	int pos = m_pGrid->GetDisLine();

	if(m_iObjectType == 5)		//新手怪物召唤卡特殊处理
	{
		DWORD dwColor = 0xFFFFFFFF;
		for(size_t i = 0; i < USE_OBJECT_LINE_COUNT && i + pos < iNum; i ++)
		{
			if((i + pos) < m_iMonsterNum) dwColor = 0xFFFFFFFF;
			else dwColor = 0xff7f7f7f;
			m_pGrid->SetTextColor(dwColor,0xfff7f705);
			m_pGrid->DrawGrid(i,0,m_VLine[i+pos].c_str());
		}

		return;
	}

	for(size_t i = 0; i < USE_OBJECT_LINE_COUNT && i + pos < iNum; i ++)
	{
		m_pGrid->DrawGrid(i,0,m_VLine[i+pos].c_str());
	}
}

void CUseObjectWnd::CreateList(DWORD dwRelation)
{
	int iSize = g_OtherData.GetRelationVector().size();
	for(int i = 0;i< iSize;i++)
	{
		_RelationStruct& rl = g_OtherData.GetRelationVector()[i];
		if((rl.iRelType & dwRelation)!=0 && rl.iOnline > 0)
		{
			m_VLine.push_back(rl.strName);
		}
	}
}

//飞来神石 好友、夫妻、师徒名称列表
void CUseObjectWnd::CreateFLSSList()
{
	DWORD dwRelation = RT_FRIEND | RT_HUSBAND | RT_WIFE | RT_MASTER | RT_PRENTICE;
	CreateList(dwRelation);
}
//师徒神镯 师徒名称列表
void CUseObjectWnd::CreateSTSZList()	
{
	DWORD dwRelation = RT_MASTER | RT_PRENTICE;
	CreateList(dwRelation);
}
//结义项链 好友名称列表
void CUseObjectWnd::CreateJYXLList()
{
	DWORD dwRelation = RT_FRIEND;
	CreateList(dwRelation);
}
//回城神石 可记录回城点的城市名字
void CUseObjectWnd::CreateHCSSList()
{
	m_VLine.push_back("落霞岛");
	m_VLine.push_back("中州");
	m_VLine.push_back("土城");
	m_VLine.push_back("禁地");
	m_VLine.push_back("西域奇境");
	m_VLine.push_back("死水沼泽");
	m_VLine.push_back("桃花岛");
	m_VLine.push_back("夕霞岛");
}

void CUseObjectWnd::SetLines(const char * pChar)
{
	vector<string> VMonsterInMap;
	vector<string> VMonsterNotInMap;
	string MonsterName;

	if(pChar)
	{
		const char* szCurrentMap = g_pGameMap->GetMapTitle();
		m_VLine.clear();

		int iStart = 0;
		int iLen = strlen(pChar);
		while(iStart < iLen)
		{
			char strTemp[128] = {0};
			strcpy(strTemp,StringUtil::toStr(pChar,iStart).c_str());
			if(strcmp(strTemp,"") != 0)	//怪物名字
			{
				MonsterName = strTemp;	
			}
			strcpy(strTemp,StringUtil::toStr(pChar,iStart).c_str());
			if(strcmp(strTemp,"") != 0)  //怪物所在地图
			{
				char* sep = ";";
				const char* szMonsterMap = NULL;

				char* szToken = strtok(strTemp,sep);
				while(szToken)
				{
					szMonsterMap = g_pMinMap->MapFile2Name(szToken);
					if(strcmp(szCurrentMap,szMonsterMap) == 0)
					{
						VMonsterInMap.push_back(MonsterName);
						MonsterName.clear();
						break;
					}	
					szToken = strtok(NULL,sep);
				}
				if(!MonsterName.empty())				//怪物不在Player所在的地图
				{
					VMonsterNotInMap.push_back(MonsterName);
					MonsterName.clear();
				}					
			}
			else
				break;
		}

		sort(VMonsterInMap.begin(),VMonsterInMap.end());
		sort(VMonsterNotInMap.begin(),VMonsterNotInMap.end());
		m_iMonsterNum = VMonsterInMap.size();

		for(int i = 0;i < VMonsterInMap.size();i++)
		{
			m_VLine.push_back(VMonsterInMap[i]);
		}
		for(int i = 0;i < VMonsterNotInMap.size();i++)
		{
			m_VLine.push_back(VMonsterNotInMap[i]);
		}
	}

	m_pGrid->SetTotalCount((int)m_VLine.size());
	if(m_VLine.size() > 0)
		m_pEdit->SetText(m_VLine[0].c_str(), COLOR_TEXT_NORMAL, COLOR_TEXT_NORMAL, COLOR_TEXT_NORMAL, -1, FONTSIZE_DEFAULT, 0, FONT_YAHEI);

}

void CUseObjectWnd::OnSubmit()
{
	string str = m_pEdit->GetText();
	if(str.length() <= 0)
		return;

	switch(m_iObjectType)
	{
	case 1://飞来神石
		str = "@chuan " + str;
		g_pGameControl->SEND_Message_Send(str.c_str(),str.length());
		break;
	case 2://师徒神镯
		str = "@师徒合一 " + str;
		g_pGameControl->SEND_Message_Send(str.c_str(),str.length());
		break;
	case 3://结义项链
		str = "@手足合一 " + str;
		g_pGameControl->SEND_Message_Send(str.c_str(),str.length());
		break;
	case 4://回城神石、回城神石（大）
		{
			size_t iNum = m_VLine.size();
			bool bFind = false;
			for(size_t i = 0; i < iNum; i ++)
			{
				if(str == m_VLine[i])
				{
					g_pGameControl->SEND_Set_Use_Object_Para(0,m_dwObjID,(BYTE)i);
					bFind = true;
					break;
				}
			}

			if(!bFind)
			{
				g_MsgBoxMgr.PopSimpleAlert("请选择或者输入一个正确的地图名。");
			}

		}

		break;
	case 5://是新手怪物召唤卡
		{
			size_t iNum = m_VLine.size();
			bool bFind = false;
			for(size_t i = 0; i < iNum; i ++)
			{
				if(str == m_VLine[i])
				{
					g_pGameControl->SEND_Set_Use_Object_Para(1,m_dwObjID,(BYTE)i,str.c_str());
					bFind = true;
					break;
				}
			}

			if(!bFind)
			{
				g_MsgBoxMgr.PopSimpleAlert("请选择或者输入一个正确的怪物名称。");
			}

		}

		break;
	default:
		break;
	}

	m_dwLifeTime = GetTickCount() + 20;
}