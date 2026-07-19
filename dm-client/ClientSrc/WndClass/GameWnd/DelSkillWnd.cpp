#include "DelSkillWnd.h"
#include "GameControl/GameControl.h"
#include "GameData/GameData.h"
#include "GameAI/AIMgr.h"
#include "GameData/MsgBoxMgr.h"
#include "GameData/MagicDefine.h"

INIT_WND_POSX(CDelSkillWnd,POS_AUTO,POS_AUTO)

#define DEL_SKILL_LINE_COUNT 7

CDelSkillWnd::CDelSkillWnd(void)
{
	m_iSelectedRow = 0;
	m_iIndex = /*17327*/21600;
	m_dwGoodID = sm_dwWindowType;
}

CDelSkillWnd::~CDelSkillWnd(void)
{
}

void CDelSkillWnd::OnCreate()
{
	//列表控件
	m_pGrid = new CCtrlGrid();
	AddControl(m_pGrid);
	//  [1/27/2010 dujun]
	m_pGrid->CreateEx(this,66,92,292,300,DEL_SKILL_LINE_COUNT,25.0f,21609);
	m_pGrid->AddUpButton(90,0,21601,21602,21603);
	m_pGrid->AddDownButton(90,200,21605,21606,21607);
	m_pGrid->AddScroll(0,0,0,204,0,false);
	m_pGrid->SetLinesPerPage(DEL_SKILL_LINE_COUNT);
	//m_pGrid->SetDrawOffXY(0,24);
	m_pGrid->PushColumn(30);
	m_pGrid->PushColumn(110);
	//[dujun]
	m_pGrid->SetFont(FONT_YAHEI,FONTSIZE_MIDDLE);//设置字体
	m_pGrid->SetTextColor(COLOR_TEXT_MAIN_TITLE,0xffff0000);//设置颜色


	//确认
	m_pSubmitBtn = new CCtrlButton();
	AddControl(m_pSubmitBtn);
	m_pSubmitBtn->CreateEx(this,50,372,165,166,167,168);
	m_pSubmitBtn->SetMask(16900);
	
	m_pCancelBtn = new CCtrlButton();
	AddControl(m_pCancelBtn);
	m_pCancelBtn->CreateEx(this,215,372,165,166,167,168);
	m_pCancelBtn->SetMask(16908);
	
	m_pCloseButton = new CCtrlButton();
	AddControl(m_pCloseButton);
	m_pCloseButton->Create(this,303,30,80);
	m_pCloseButton->SetMask(80);

	SetMask(16903);

	char str[8][16] = {"初级","中级","高级","专家级","大师级","宗师级","权威级","王者级"};

	for(int i = 0;i< MAX_MAGIC_SKILL;i++)
	{
		CMagicData MagicData = SELF.GetMagic(i);

		WORD wMagicID = MagicData.GetMagicID();

		if(wMagicID > 0 && !g_AIMgr.IsRideFightMagic(wMagicID) && !g_AIMgr.IsSuperMagic(wMagicID) && wMagicID != MAGICID_ZHENYUAN_ATTACK && wMagicID != MAGICID_ZHI_YAN && !g_AIMgr.IsTaoZhuangMagic(wMagicID))
		{
			m_VSkill.push_back(SSkill(MagicData.GetMagicName(),MagicData.GetMagicID(),str[MagicData.GetMagicLevel()]));			
		}
	}

	m_pGrid->SetTotalCount((int)m_VSkill.size());
}

void CDelSkillWnd::DrawLineData()
{
	size_t iNum = m_VSkill.size();
	int pos = m_pGrid->GetDisLine();

	for(size_t i = 0; i < DEL_SKILL_LINE_COUNT && i + pos < iNum; i ++)
	{
		//  [1/28/2010 dujun]
		m_pGrid->DrawGrid(i,1,m_VSkill[i+pos].strName.c_str(),true,0,0,22,0);//添加了X的偏移
	}
}

void CDelSkillWnd::Draw( void )
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	//  [1/27/2010 dujun]
	//绘制标题
	g_pFont->DrawText(146+m_iScreenX, 34+m_iScreenY, "技能遗忘", COLOR_TEXT_MAIN_TITLE, FONT_YAHEI, FONTSIZE_BIG);
	g_pFont->DrawText(150+m_iScreenX, 75+m_iScreenY, "技能列表", COLOR_TEXT_MAIN_TITLE, FONT_YAHEI, FONTSIZE_MIDDLE);
	//绘制按钮
	g_pFont->DrawText(80+m_iScreenX, 377+m_iScreenY, "遗 忘", COLOR_TEXT_MAIN_TITLE, FONT_YAHEI, FONTSIZE_MIDDLE);
	g_pFont->DrawText(245+m_iScreenX, 377+m_iScreenY, "取 消", COLOR_TEXT_MAIN_TITLE, FONT_YAHEI, FONTSIZE_MIDDLE);
	
	DrawLineData();
}

bool CDelSkillWnd::Msg( DWORD dwMsg,DWORD dwData,CControl * pControl )
{
	switch(dwMsg)
	{
	case MSG_CTRL_BUTTON_CLICK:
		{
			if(pControl == m_pSubmitBtn)//确认
			{				
				m_iSelectedRow = m_pGrid->GetSelLine();
				
				if(m_iSelectedRow >= 0 && m_iSelectedRow < (int)m_VSkill.size())
				{
					char str[256] = "";
					sprintf(str,"    你选择遗忘的技能为：%s，当前级别为：%s，你是否需要遗忘此技能？",m_VSkill[m_iSelectedRow].strName.c_str(),m_VSkill[m_iSelectedRow].strLevel.c_str());
					g_MsgBoxMgr.PopSimpleComfirm(str,MSG_CTRL_DEL_SKILL,0);
				}					
				else
				{
					g_MsgBoxMgr.PopSimpleAlert("请先选择你需要遗忘的技能。");
				}				
				
				return true;
			}
			else if(pControl == m_pCancelBtn)
			{
				g_pControl->PopupWindow(MSG_CTRL_DEL_SKILL_WND,OPER_CLOSE);
				return true;
			}
		}
		break;
	case MSG_CTRL_GRID_DBCLICK:
		{
			m_iSelectedRow = m_pGrid->GetSelLine();
			if(m_iSelectedRow >= 0 && m_iSelectedRow < (int)m_VSkill.size())
			{
				char str[256] = "";
				sprintf(str,"    你选择遗忘的技能为：%s，当前级别为：%s，你是否需要遗忘此技能？",m_VSkill[m_iSelectedRow].strName.c_str(),m_VSkill[m_iSelectedRow].strLevel.c_str());
				g_MsgBoxMgr.PopSimpleComfirm(str,MSG_CTRL_DEL_SKILL,0);
			}
			else
			{
				g_MsgBoxMgr.PopSimpleAlert("请先选择你需要遗忘的技能。");
			}

			return true;
		}
		break;
	case MSG_CTRL_DEL_SKILL_WND:		//删除技能
		{
			if(dwData == OPER_CUSTOM)
			{
				g_pGameControl->Send_Del_Skill(m_dwGoodID,m_VSkill[m_iSelectedRow].strName);
				g_pControl->PopupWindow(MSG_CTRL_DEL_SKILL_WND,OPER_CLOSE);
				return true;
			}
		}
		break;
	default:
		break;
	}

	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}
