#include "guildedit.h"
#include "GameControl/GameControl.h"
#include "GameData/GameData.h"
#include "GameData/MsgBoxMgr.h"
#include <algorithm>
#include "GameData/GuildData.h"

INIT_WND_POSX(CGuildEdit,POS_VARIABLE,POS_VARIABLE)

CGuildEdit::CGuildEdit()
{
	m_pOk		= NULL;
	m_pCancel   = NULL;
	m_pEdit		= NULL;
	m_iLastFindPos = 0;

	m_iSliderX = 328;
	m_iSliderY = 0;
	m_iSliderBegin = 76;
	m_iSliderEnd = 301;

	m_bSliderClick = false;
	m_pSearchBtn = NULL;
	m_pSearchLastBtn = NULL;
	m_pSearchEdit = NULL;

	m_iSearchLine = 0;


	m_state  = sm_dwWindowType;//1封号,2公告
	if(m_state == 1)
	{
		m_iIndex = 13587;
		m_iSliderX = 204;
		m_iSliderBegin = 68;
		m_iSliderEnd = 181;
	}
	else
	{
		m_iIndex = 13586;
		m_iSliderX = 327;
		m_iSliderBegin = 76;
		m_iSliderEnd = 295;
	}

	m_iPages = 1;
	
	m_iAlignType = XAL_TOPLEFT;
	if((g_pGfx->GetWidth() > 800))
	{
		m_iOffX = 200;
		m_iOffY = 80;
	}
	else
	{
		m_iOffX = 95;
		m_iOffY = 50;
	}
}

CGuildEdit::~CGuildEdit(void)
{
}

void CGuildEdit::OnCreate()
{

	if(m_state == 1)
	{
		//确定按钮
		m_pOk= new CCtrlButton();
		AddControl(m_pOk);
		m_pOk->CreateEx(this,29,259,90,91,92);
		m_pOk->SetText("确 定",COLOR_BTN_NORMAL,COLOR_BTN_MOUSEON,COLOR_BTN_PRESS);

		//最近搜索内容菜单
		m_pSearchBtn = new CCtrlButton();
		AddControl(m_pSearchBtn);
		m_pSearchBtn->CreateEx(this,113,259,90,91,92);
		m_pSearchBtn->SetText("定 位",COLOR_BTN_NORMAL,COLOR_BTN_MOUSEON,COLOR_BTN_PRESS);

		//搜索编辑框
		m_pSearchEdit = new CCtrlEdit();
		AddControl(m_pSearchEdit);
		m_pSearchEdit->CreateEx(this,18,232,175,23,0,12,12,12);
		m_pSearchEdit->SetFont(FONT_YAHEI,FONTSIZE_DEFAULT);

		//m_pSearchEdit->Create(this,FONTSIZE_DEFAULT,0xFFFFFFFF,18,232,182,18);		
		m_pSearchEdit->SetMaxLength(26);

		//编辑框
		m_pEdit= new CCtrlMultiEdit(35000,30,12);
		AddControl(m_pEdit,0);		
		m_pEdit->Create(this,14,18,37,182,170,true);
		m_pEdit->SetFont(FONT_YAHEI,14);
		m_pEdit->SetFocus();
		SetCloseButton(187,3,80);
	}
	else if(m_state == 2)
	{
		//确定按钮
		m_pOk= new CCtrlButton();
		AddControl(m_pOk);
		m_pOk->CreateEx(this,88,290,90,91,92);
		m_pOk->SetText("确 定",COLOR_BTN_NORMAL,COLOR_BTN_MOUSEON,COLOR_BTN_PRESS);

		//取消按钮
		m_pCancel= new CCtrlButton();
		AddControl(m_pCancel);
		m_pCancel->CreateEx(this,197,290,90,91,92);
		m_pCancel->SetText("取 消",COLOR_BTN_NORMAL,COLOR_BTN_MOUSEON,COLOR_BTN_PRESS);

		//编辑框
		m_pEdit= new CCtrlMultiEdit(20000,51,19);
		AddControl(m_pEdit,0);
		m_pEdit->Create(this,14,13,37,325,246,true);
		m_pEdit->SetFocus();
		SetCloseButton(328,2,80);
	}

	string temp = "";
	if(m_state != 1)
	{
		int iCount = (int)g_GuildData.GetBulletin().size();
		for(int i = 2; i < iCount - 1; i ++)
		{
			temp += g_GuildData.GetBulletin()[i].c_str();
			if(temp.size() > 0 && (i != (g_GuildData.GetBulletin().size() - 2)))
				temp += "\r\n";
		}
	}

	m_pEdit->SetText(temp.c_str());    
}

void CGuildEdit::Draw()
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	if(m_state == 1)//封号
	{
		if(strlen(m_pEdit->GetText()) == 0)
		{
			string temp;
			//用新的数据来填充，去掉以前的官职，官职数据走新的协议(任罢免协议)
			if (g_GuildData.GetGuildTowerLevel() <= 0)
			{
				std::vector<CGuildData::sTileNode>& guildMemberOff = g_GuildData.GetGuildMemberOfficer();
				for (int i = 0;i<guildMemberOff.size();i++)
				{
					temp += "#";
					temp += StringUtil::format("%d",guildMemberOff[i].wNum);
					temp += " <";
					temp += guildMemberOff[i].sTile;
					temp += ">\r\n";
					for(unsigned int j = 0; j < guildMemberOff[i].vMember.size(); j++)
					{
						CGuildData::_NewMember& newMem = guildMemberOff[i].vMember[j];
						temp += newMem.strName;
						temp += "\r\n";
					}
				}
			}

			std::vector<CGuildData::sTileNode>& guildMember = g_GuildData.GetGuildMemberNormal();
			for (int i = 0;i<guildMember.size();i++)
			{
				temp += "#";
				temp += StringUtil::format("%d",guildMember[i].wNum);
				temp += " <";
				temp += guildMember[i].sTile;
				temp += ">\r\n";
				for(unsigned int j = 0; j < guildMember[i].vMember.size(); j++)
				{
					CGuildData::_NewMember& newMem = guildMember[i].vMember[j];
					temp += newMem.strName;
					temp += "\r\n";
				}
			}
			
			if(temp.size() != 0) m_pEdit->SetText(temp.c_str());
		}
		
		g_pFont->DrawText(m_iScreenX + 76,m_iScreenY+8,"编辑封号",COLOR_TEXT_NORMAL,FONT_YAHEI,16,DTF_BlackFrame|DTF_Bold);
	}
	else
	{
		g_pFont->DrawText(m_iScreenX + 125,m_iScreenY+8,"编辑行会公告",COLOR_TEXT_NORMAL,FONT_YAHEI,16,DTF_BlackFrame|DTF_Bold);
	}

	int iSpaceLine = m_pEdit->GetLines() - m_pEdit->GetDisRows();
	if(iSpaceLine > 0)
		m_iSliderY = m_pEdit->GetDisLine() * (m_iSliderEnd - m_iSliderBegin)/iSpaceLine;
	else m_iSliderY = 0;	

	if(m_iSliderY > m_iSliderEnd - m_iSliderBegin)
		m_iSliderY = m_iSliderEnd - m_iSliderBegin;

	//LPTexture pSliderTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,4629);
	//if(pSliderTex) g_pGfx->DrawTextureNL(m_iScreenX+m_iSliderX,m_iScreenY+m_iSliderY+m_iSliderBegin,pSliderTex);

}

bool CGuildEdit::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	std::string str;
	char temp[512];
	switch ( dwMsg )
	{
	case MSG_CTRL_BUTTON_CLICK:
		if(pControl == m_pOk)
		{
			if(m_pEdit->IsChanged())			// 有改动才向服务器端发送消息
			{
				if(m_state == 1)
				{
					if (SecurityCheck())
					{
						str.clear();
						bool bSucess = ConstructTile();
						if (bSucess)
						{//构建行会封号新成员成功								
							std::vector<CGuildData::sTileNode>* pguildMem = NULL;
							for (int i = 0;i<m_vNewTile.size();i++)
							{
								WORD wNum = m_vNewTile[i].wNum;
								string sTile = m_vNewTile[i].sTile;
								if (wNum <= 10)
								{//会长										
									pguildMem = &(g_GuildData.GetGuildMemberOfficer());
								}
								else
								{
									pguildMem = &(g_GuildData.GetGuildMemberNormal());
								}

								bool bAddTile = false;
								bool bFoundTile = false;
								for (int j = 0;j<pguildMem->size();j++)
								{
									if (wNum == (*pguildMem)[j].wNum)
									{//如果封号相同	
										bFoundTile = true;
										if (sTile.compare((*pguildMem)[j].sTile.c_str()) == 0)
										{//相同
											std::vector<CGuildData::_NewMember>& strVecName = m_vNewTile[i].vMember;
											std::vector<CGuildData::_NewMember>& strOldName = (*pguildMem)[j].vMember;
											for (int k = 0;k<strVecName.size();k++)
											{
												string strName = strVecName[k].strName;
												bool bFound = false;
												for (int kk = 0;kk<strOldName.size();kk++)
												{
													string strNewName = strOldName[kk].strName;
													if (strName.compare(strNewName.c_str()) == 0)
													{
														bFound = true;
														break;
													}
												}

												if (!bFound)//没有找到
												{
													if (!bAddTile)
													{
														str += "#";
														str += StringUtil::format("%d",wNum);
														str += " <";
														str += sTile;
														str += ">\r\n";
														bAddTile = true;
													}

													str += strName;
													str += "\r\n";
												}
											}
										}
										else
										{
											//封号不同
											str += "#";
											str += StringUtil::format("%d",wNum);
											str += " <";
											str += sTile;
											str += ">\r\n";
											std::vector<CGuildData::_NewMember>& strVecName = m_vNewTile[i].vMember;
											for (int i = 0;i<strVecName.size();i++ )
											{
												str += strVecName[i].strName;
												str += "\r\n";
											}
										}
									}
								}

								if (!bFoundTile)
								{
									//没有找到相同的封号，则增加所有的数据
									str += "#";
									str += StringUtil::format("%d",wNum);
									str += " <";
									str += sTile;
									str += ">\r\n";
									std::vector<CGuildData::_NewMember>& strVecName = m_vNewTile[i].vMember;
									for (int i = 0;i<strVecName.size();i++ )
									{
										str += strVecName[i].strName;
										str += "\r\n";
									}
								}
							}
						}

						str.erase(remove(str.begin(),str.end(),0x0A),str.end());
						str.erase(remove(str.begin(),str.end(),'/'),str.end());
						if(str.size() > 0 && str[str.size() - 1] != 0x0D)
							str.push_back(0x0d);
						g_pGameControl->SEND_Guild_Member_Title_Edit(str.c_str(),true);
					}
				}
				else
				{
					for(int i = 0; i < m_pEdit->GetLines() ; i++)
					{
						m_pEdit->GetLineText(temp,i);
						str += temp;
						if(i != m_pEdit->GetLines() - 1)
							str += 0x0D;
					}
					g_pGameControl->SEND_Guild_Message_Edit(str.c_str());
				}
			}
			g_pControl->Msg(MSG_GUILD_EDIT,OPER_CLOSE);
			return true;
		}else if(pControl == m_pCancel)
		{
			OnClickCloseButton();
			return true;
		}
		else if(pControl == m_pSearchBtn)
		{
			string sSearch = m_pSearchEdit->GetText();
			if(sSearch.size() == 0)	return true;

			str = m_pEdit->GetText();
			int iCurPos = m_pEdit->GetCursorPos();
			if(iCurPos<0 || iCurPos>=str.size()) iCurPos = 0;

			if(sSearch == m_strSearch) //继续查找
				iCurPos = m_iLastFindPos+sSearch.size();
			else
			{
				if(g_GuildData.GetFindStr().size()>=50)
					g_GuildData.GetFindStr().erase(g_GuildData.GetFindStr().begin());

				g_GuildData.GetFindStr().push_back(sSearch);
				m_iSearchLine = 0;
			}

			//新查找
			m_strSearch = sSearch;
			int iFindPos = str.find(sSearch,iCurPos);
			if(iFindPos>0)
			{
				m_pEdit->SetCursorPos(iFindPos);
				int iCurRow = m_pEdit->GetCursorLine();
				m_pEdit->SetDisLine(iCurRow);
				m_iLastFindPos = iFindPos;
				m_pEdit->SetSelRange(iFindPos,iFindPos+sSearch.size());
			}
			else
			{
				iFindPos = str.find(sSearch,0);
				if(iFindPos>0)
				{
					m_pEdit->SetCursorPos(iFindPos);
					int iCurRow = m_pEdit->GetCursorLine();
					m_pEdit->SetDisLine(iCurRow);
					m_iLastFindPos = iFindPos;
					m_pEdit->SetSelRange(iFindPos,iFindPos+sSearch.size());
				}
			}
			m_pEdit->SetFocus();

			return true;
		}
		else if(pControl == m_pSearchLastBtn)
		{
			if(NULL == m_pSearchEdit) return true;

			int iFindStrSize = g_GuildData.GetFindStr().size();
			if(iFindStrSize>0)
			{
				if(m_iSearchLine>= iFindStrSize)  m_iSearchLine = 0;

				string& str = g_GuildData.GetFindStr()[iFindStrSize-m_iSearchLine-1];
				m_pSearchEdit->SetText(str.c_str());
				m_pSearchEdit->SetFocus();
				m_iSearchLine++;
			}
			return true;
		}
		//else if(pControl == m_pUpButton)
		//{
		//	int iLine = m_pEdit->GetDisLine();
		//	if(iLine>0)
		//	{
		//		iLine--;
		//		m_pEdit->SetDisLine(iLine);
		//	}
		//	return true;
		//}
		//else if(pControl == m_pDownButton)
		//{
		//	int iLine = m_pEdit->GetDisLine();
		//	if(iLine+m_pEdit->GetDisRows()<m_pEdit->GetLines())
		//	{
		//		iLine++;
		//		m_pEdit->SetDisLine(iLine);
		//	}
		//	return true;
		//}
		break;
	default:
		break;
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl); 
}

bool CGuildEdit::OnLeftButtonDown(int iX, int iY)
{
	if(iX>=m_iSliderX && iX<=m_iSliderX+16 && iY>=m_iSliderBegin && iY<=m_iSliderEnd+7)
	{
		if(iY>=m_iSliderBegin+m_iSliderY && iY<=m_iSliderBegin+m_iSliderY+7)
			m_bSliderClick = true;
		//else if(iY<m_iSliderBegin+m_iSliderY)
		//	Msg(MSG_CTRL_BUTTON_CLICK,0,m_pUpButton);
		//else Msg(MSG_CTRL_BUTTON_CLICK,0,m_pDownButton);
		return true;
	}
	return CCtrlWindowX::OnLeftButtonDown(iX,iY);
}

bool CGuildEdit::OnLeftButtonUp(int iX, int iY)
{
	m_bSliderClick = false;
	return CCtrlWindowX::OnLeftButtonUp(iX,iY);
}

bool CGuildEdit::OnMouseMove(int iX, int iY)
{
	if(m_bSliderClick)
	{
		m_iSliderY = iY - m_iSliderBegin;

		if(iX>=m_iSliderX && iX<=m_iSliderX+16 && iY>=m_iSliderBegin && iY<=m_iSliderEnd+7)
		{
			int iSpaceLine = m_pEdit->GetLines()-m_pEdit->GetDisRows();
			int iLine = 0;

			if(iSpaceLine > 0)
			{
				iLine = (m_iSliderY*(iSpaceLine))/(m_iSliderEnd-m_iSliderBegin);
				if(iLine>iSpaceLine) iLine = iSpaceLine;
				if(iLine<0)	iLine = 0;
			}
			else iLine = 0;
			m_pEdit->SetDisLine(iLine);
		}
		else m_bSliderClick = false;
		return true;
	}
	return CCtrlWindowX::OnMouseMove(iX,iY);
}

bool CGuildEdit::SecurityCheck()
{
	//安全性检验
	string strTemp = m_pEdit->GetText();
	if (strTemp.length() <= 0) 
		return false;	

	int iPos = 0;
	while(iPos != -1)
	{
		iPos = strTemp.find_first_of('#',iPos);
		if (iPos != -1)
		{
			int iTemp = strTemp.find_first_of(" <",iPos);				
			if (iTemp != -1)
			{
				int nRight = strTemp.find_first_of('>',iTemp);
				if (nRight == std::string::npos)
				{
					g_MsgBoxMgr.PopSimpleAlert("编辑封号格式不对，封号应放在< >内！");
					return false;
				}

				int iLen = nRight - iTemp - 1;
				if (iLen > 30)
				{
					g_MsgBoxMgr.PopSimpleAlert("封号长度不应超过15个汉字，或者30个英文字母！");
					return false;
				}

				string str = strTemp.substr(iPos+1,iTemp);
				int i = atoi(str.c_str());
				if (g_GuildData.GetGuildTowerLevel() > 0)
				{
					if (i<=10 && i>= 0)
					{
						//g_MsgBoxMgr.PopSimpleAlert("封号长度不应超过15个汉字，或者30个英文字母！");
						g_MsgBoxMgr.PopSimpleAlert("不能编辑封号为1-10之间，它们只能在人事中进行任免！");
						return false;
					}
				}
			}
			iPos++;
		}
	}

	return true;
}

bool CGuildEdit::ConstructTile()
{
	string strTemp = m_pEdit->GetText();
	if (strTemp.length() <= 0)	return false;	
	m_vNewTile.clear();
	int iPos = 0;
	while(iPos != -1)
	{
		iPos = strTemp.find_first_of('#',iPos);
		if (iPos != -1)
		{
			CGuildData::sTileNode stile;			
			int iTemp = strTemp.find(" <",iPos);
			if (iTemp != -1)
			{
				//获得封号
				string str = strTemp.substr(iPos+1,iTemp - iPos - 1);
				stile.wNum = atoi(str.c_str());
				int iTemp2 = strTemp.find_first_of('>',iTemp);
				stile.sTile = strTemp.substr(iTemp + 2,iTemp2 - iTemp - 2);
				//获得成员列表
				int iPos1 = strTemp.find_first_of('#',iPos + 1);
				if (iPos1 == -1) iPos1 = strTemp.length();
								
				int iPos2 = strTemp.find("\r\n",iPos + 1);			
				while (iPos2 != - 1 && iPos2 + 2 < iPos1)
				{//查找成员数据
					int iPosStart = iPos2 + 2;
					iPos2 = strTemp.find("\r\n",iPosStart);
					CGuildData::_NewMember newMem;
					if (iPos2 != -1)
					{
						string strName = strTemp.substr(iPosStart,iPos2 - iPosStart);						
						if (strName.length() > 0)
						{
							newMem.strName = strName;
							stile.vMember.push_back(newMem);
						}
						iPos2 = strTemp.find("\r\n",iPosStart);
					}
					else
					{
						string strName = strTemp.substr(iPosStart);
						if (strName.length() > 0)
						{
							newMem.strName = strName;
							stile.vMember.push_back(newMem);
						}
					}
				}

				if (stile.vMember.size() > 0)
				{					
					m_vNewTile.push_back(stile);					
				}
			}
			iPos ++;
		}
	}

	return true;
}