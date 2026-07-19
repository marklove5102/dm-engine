#include "DivinityWingWnd.h"
#include "BaseClass/Control/GoodGrid.h"
#include "GameData/GameData.h"
#include "GameControl/GameControl.h"
#include "GameData/NpcData.h"
#include "GameData/TalkMgr.h"
#include "GameData/MsgBoxMgr.h"
#include "Baseclass/Control/ParserTip.h"
#include "GameAI/AIGoodMgr.h"
#include "GameData/MagicDefine.h"

const POINT CDivinityWingWnd::m_ptLevelPos[5] = {
	{ 10, 236 },
	{ 10, 192 },
	{ 10, 148 },
	{ 10, 104 },
	{ 10, 60 },
};


const std::string CDivinityWingWnd::m_strWingNeedLevel[5] = {
	"需要等级:52",	
	"需要等级:58",
	"需要等级:62",
	"需要等级:66",
	"需要等级:75",
};

const std::string CDivinityWingWnd::m_strWingSkillLevel[5] = {
	"初级",	
	"中级",
	"高级",
	"专家级",
	"大师级",
};

const string CDivinityWingWnd::m_WingAttrTypes[3][14] = {	
	{"生命值","魔法值","攻击力","防御力","命中","暴击","反弹伤害","吸血","魔防力","躲避","破防","抵抗","魔法躲避"},
	{"生命值","魔法值","魔法攻击","防御力","魔法命中","暴击","反弹伤害","吸血","魔防力","躲避","破防","抵抗","魔法躲避"},
	{"生命值","魔法值","道术攻击","防御力","魔法命中","暴击","反弹伤害","吸血","魔防力","躲避","破防","抵抗","魔法躲避"},
};

const string CDivinityWingWnd::m_WingAttrTypes1[3] = {"攻击力","魔法攻击","道术攻击"};
const string CDivinityWingWnd::m_WingAttrTypes2[3] = {"命  中","魔法命中","魔法命中"};


INIT_WND_POSX(CDivinityWingWnd,POS_AUTO,POS_AUTO)


CDivinityWingWnd::CDivinityWingWnd(void)
{
	m_pUp = NULL;
	m_pSkillButton = NULL;

	m_iIndex = 21686;
	m_pMarkViewer = NULL;
	m_npcText.Clear();
}


CDivinityWingWnd::~CDivinityWingWnd(void)
{
	
}

void CDivinityWingWnd::OnCreate()
{
	SetCloseButton(376,1, 80);

	m_pUp = new CCtrlButton();
	AddControl(m_pUp);
	m_pUp->CreateEx(this, 332, 346, 21679,21680,21681,21682);
	m_pUp->SetText("升 级", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);
	m_pUp->SetEnable(false);

	for (int i = 0; i < MAX_HOLYWING_PARTS; ++i)
	{
		m_pRadioBtn[i] = new CCtrlRadio();
		m_pRadioBtn[i]->CreateEx(this,42 + i * 80,336,18,18,21683,21684,21683,21683);
		if (i > 0)
		{
			m_pRadioBtn[0]->AddBuddy(m_pRadioBtn[i]);
			m_pRadioBtn[0]->SetTextAlignType(XAL_CENTER);
			m_pRadioBtn[i]->SetTextAlignType(XAL_CENTER);
		}
		AddControl(m_pRadioBtn[i]);
		m_pRadioBtn[i]->SetChecked(false);
	}

	bool bChecked = false; 
	for (int i = 0; i < MAX_HOLYWING_PARTS; ++i)
	{
		if (SELF.GetHolyWingAttrExp(i) < SELF.GetHolyWingPartLevelUpExpMax())
		{
			m_pRadioBtn[i]->SetChecked(true);	
			bChecked = true;
			break;
		}
	}

	if (!bChecked)
	{
		m_pRadioBtn[0]->SetChecked(true);	
	}


	m_pSkillButton = new CCtrlButton();
	AddControl(m_pSkillButton);
	int iCareer = SELF.GetCareer();
	int mid = MAGICID_FIGHTER_FIGHTER_KILL;
	
	if (iCareer == 1)
		mid = MAGICID_ENCHANTER_ENCHANTER_THUNDER;
	else if (iCareer == 2)
		mid = MAGICID_DAO_FIGHTER_POISON;

	m_pSkillButton->CreateEx(this,346,307,(mid - 58) + 2612,0,(mid - 58) + 2612,(mid - 58) + 2660);	

	//数据显示框
	m_pMarkViewer = new CMarkViewer(14,40);
	AddControl(m_pMarkViewer);
	m_pMarkViewer->Create(this,16,372,20,40);
	m_pMarkViewer->SetTagText(&m_npcText);
	m_pMarkViewer->SetEnable(true);
	m_pMarkViewer->SetShow(true);

	string str = "可以使用道具升级和强化神翼，获得<color=orange 额外属性加成>以及<color=orange 终极技能>。\\";
	str += "升级道具：<color=ggreen 妖翎羽>、<color=5AA0A0 完美妖翎羽>\\";
	str += "产出方式：从<color=ggreen 妖羽禁地使者>处进入妖羽禁地，使用<color=ggreen 轰天雷>、<color=5AA0A0 霸极轰天雷>\\";
	str += "杀怪后获得\\";
	str += "强化道具：<color=5AA0A0 妖羽强化石>\\";
	str += "<color=ggreen 神翼等级与人物等级相关：人物等级达到相应要求后才可以继续升级神>\\";
	str += "<color=ggreen 翼等级。>";
	m_npcText.Parse(str,0,"\\",str.length());
}


bool CDivinityWingWnd::Msg(DWORD dwMsg, DWORD dwData, CControl *pControl)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_DIVINITYWING_WND:
		{
			if (dwData == 10)	// 使用妖翎羽
			{
				int j = 0;
				for (; j < MAX_HOLYWING_PARTS; ++j)
				{
					if (m_pRadioBtn[j]->IsChecked())
						break;
				}				

				if (j < MAX_HOLYWING_PARTS)
				{
					if (SELF.GetHolyWingAttrExp(j) != SELF.GetHolyWingPartLevelUpExpMax())
					{
						g_pGameControl->Send_LevelUpWing(j+1, *(DWORD*)pControl);
					}
					else
					{
						g_TalkMgr.PushSysTalk("神翼经验值已满，请点击神翼界面内的升级按钮升级神翼。");
					}
				}

				return true;
			}			
			return false;
		}
		break;
	case MSG_CTRL_BUTTON_CLICK:
		{
			if (pControl == m_pUp)
			{
				g_pGameControl->Send_LevelUpWing(0);
				return true;
			}
			else if (pControl == m_pSkillButton)
			{
				int iCareer = SELF.GetCareer();
				int mid = MAGICID_FIGHTER_FIGHTER_KILL;

				if (iCareer == 1)
					mid = MAGICID_ENCHANTER_ENCHANTER_THUNDER;
				else if (iCareer == 2)
					mid = MAGICID_DAO_FIGHTER_POISON;

				int iPos = SELF.FindMagicPos(mid);
				if (iPos >= 0)
				{
					g_AIMgr.SetShortCutKeyMagicPos(iPos);
					g_pControl->PopupWindow(MSG_CTRL_SHORTCUT_WND,OPER_CREATE,1);
				}
				return true;
			}
		}
		break;
	default:
		break;
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}


void CDivinityWingWnd::Draw()
{
	int iWingLevel = SELF.GetHolyWingLevel();
	int iWingIndex = iWingLevel / 10;
	
	if (/*iWingLevel < 30
		&& */SELF.GetHolyWingAttrExp(0) == SELF.GetHolyWingPartLevelUpExpMax()
		&& SELF.GetHolyWingAttrExp(1) == SELF.GetHolyWingPartLevelUpExpMax()
		&& SELF.GetHolyWingAttrExp(2) == SELF.GetHolyWingPartLevelUpExpMax()
		&& SELF.GetHolyWingAttrExp(3) == SELF.GetHolyWingPartLevelUpExpMax()
		&& SELF.GetLevel() >= SELF.GetHolyWingLevelUpNeedLevel())
	{
		m_pUp->SetEnable(true);
	}
	else
	{
		m_pUp->SetEnable(false);
	}
		
	
	// 神翼大于10级有神翼技能
	m_pSkillButton->SetEnable(iWingLevel >= 10);

	int lExp[4];
	memset(lExp, 0, sizeof(lExp));

	for (int i = 0; i < MAX_HOLYWING_PARTS; ++i)
	{
		if (SELF.GetHolyWingPartLevelUpExpMax() > 0)
		{
			lExp[i] = SELF.GetHolyWingAttrExp(i) * 100 / SELF.GetHolyWingPartLevelUpExpMax();
		}
	}

	int j = 0;
	for (; j < MAX_HOLYWING_PARTS; ++j)
	{
		if (m_pRadioBtn[j]->IsChecked() && (SELF.GetHolyWingAttrExp(j) == SELF.GetHolyWingPartLevelUpExpMax()))
			break;
	}

	if (j != MAX_HOLYWING_PARTS)
	{
		++j;

		for (int i = 0; i < MAX_HOLYWING_PARTS; ++i,++j)
		{
			if (j >= MAX_HOLYWING_PARTS)
				j = 0;

			if (SELF.GetHolyWingAttrExp(j) != SELF.GetHolyWingPartLevelUpExpMax())
			{
				for (int k = 0; k < 4; ++k)
					m_pRadioBtn[k]->SetChecked(false);
				m_pRadioBtn[j]->SetChecked(true);
				break;
			}
		}
	}

	CCtrlWindowX::Draw();	
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	char temp[128] = {0};

	// 神翼名称 + 强化等级	
	if (iWingIndex >= 0 && iWingIndex < 6)
	{
		if (SELF.GetHolyWingStrongLevel() > 0)
		{
			sprintf(temp,"%s+%d",g_AIGoodMgr.GetWingNameByLevel(iWingLevel).c_str(), SELF.GetHolyWingStrongLevel());	
		}
		else
		{
			sprintf(temp,"%s",g_AIGoodMgr.GetWingNameByLevel(iWingLevel).c_str());	
		}
		
		g_pFont->DrawText(166+m_iScreenX, 6+m_iScreenY, temp, COLOR_TEXT_MAIN_TITLE, FONT_YAHEI, FONTSIZE_BIG);	
	}	

	// 神翼等级
	sprintf(temp,"%d 级",iWingLevel);
	g_pFont->DrawText(176+m_iScreenX, 32+m_iScreenY, temp, 0xFF508C8C, FONT_YAHEI,FONTSIZE_MIDDLE,DTF_BlackFrame);
	

	// 显示每块区域羽毛激活百分比
	LPTexture pBarTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,21678,EP_UI);
	int iBar = 0;

	int iCareer = SELF.GetCareer();

	for (int i = 0; i < MAX_HOLYWING_PARTS; ++i)
	{
		memset(temp, 0, sizeof(temp));
		if (pBarTex)
		{
			iBar = (int)(pBarTex->GetWidth() * lExp[i] / 100);
			g_pGfx->DrawPartTexture(m_iScreenX + 19 + 80 * i,m_iScreenY + 321,pBarTex,0,0,iBar,-1);
		}

		BYTE lAttr = SELF.GetHolyWingAttrType(i);
		WORD lAttrValue = SELF.GetHolyWingAttrValue(i);		
		if (lAttr > 0 && lAttr <= 13 && iCareer >= 0 && iCareer <= 2)
			sprintf(temp,"%s+%d",m_WingAttrTypes[iCareer][lAttr - 1].c_str(), lAttrValue);
		g_pFont->DrawText(m_iScreenX + 22 + 80 * i,m_iScreenY + 304,temp,COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);

		sprintf(temp,"%d%% ",lExp[i]);
		g_pFont->DrawText(m_iScreenX + 40 + 80 * i,m_iScreenY + 317,temp,COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);	
	}
	
	// 左侧大进度条
	if (iWingLevel >=0 && iWingLevel <= 50)
	{
		pBarTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,21677,EP_UI);
		if (pBarTex)
		{
			if (iWingLevel <= 48)
			{
				iBar = (int)((pBarTex->GetHeight() - 6) * iWingLevel / 50);
			}
			else if (iWingLevel == 49)
			{
				iBar = pBarTex->GetHeight() - 6;
			}
			else
			{
				iBar = pBarTex->GetHeight();
			}

			g_pGfx->DrawPartTexture(m_iScreenX+42,m_iScreenY+56,pBarTex,0,pBarTex->GetHeight() - iBar,-1,-1);
		}
	}

	int posy = 56;
	int linespace = 19;

	if (iCareer >= 0 && iCareer <= 2)
	{
		g_pFont->DrawText(m_iScreenX + 255,m_iScreenY + posy,"生命值",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
		posy += linespace;
		g_pFont->DrawText(m_iScreenX + 255,m_iScreenY + posy,"魔法值",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
		posy += linespace;
		g_pFont->DrawText(m_iScreenX + 255,m_iScreenY + posy,m_WingAttrTypes1[iCareer].c_str(),COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
		posy += linespace;
		g_pFont->DrawText(m_iScreenX + 255,m_iScreenY + posy,"防御力",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
		posy += linespace;
		g_pFont->DrawText(m_iScreenX + 255,m_iScreenY + posy,"魔防力",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
		posy += linespace;

		g_pFont->DrawText(m_iScreenX + 255,m_iScreenY + posy,m_WingAttrTypes2[iCareer].c_str(),COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
		posy += linespace;
		g_pFont->DrawText(m_iScreenX + 255,m_iScreenY + posy,"躲  避",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
		posy += linespace;
		g_pFont->DrawText(m_iScreenX + 255,m_iScreenY + posy,"暴  击",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
		posy += linespace;
		g_pFont->DrawText(m_iScreenX + 255,m_iScreenY + posy,"反弹伤害",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
		posy += linespace;
		g_pFont->DrawText(m_iScreenX + 255,m_iScreenY + posy,"吸  血",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
		posy += linespace;

		g_pFont->DrawText(m_iScreenX + 255,m_iScreenY + posy,"破  防",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
		posy += linespace;
		g_pFont->DrawText(m_iScreenX + 255,m_iScreenY + posy,"抵  抗",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
		posy += linespace;
		g_pFont->DrawText(m_iScreenX + 255,m_iScreenY + posy,"魔法躲避",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
		posy += linespace;
	}

	int iStrongAddPercent[8]={0,10,20,30,40,50,70,100};
	int iStrongLevel = SELF.GetHolyWingStrongLevel();
	char szStrongAdd[128] = {0};
	posy = 56;
	
	if (iStrongLevel > 0 && iStrongLevel < 8)
	{
		sprintf(szStrongAdd," +%d%% ",iStrongAddPercent[iStrongLevel]);
	}

	sprintf(temp,"+%d%s",SELF.GetHolyWingAddHp(),szStrongAdd);
	g_pFont->DrawText(m_iScreenX + 318,m_iScreenY + posy,temp,COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	posy += linespace;

	sprintf(temp,"+%d%s",SELF.GetHolyWingAddMp(),szStrongAdd);
	g_pFont->DrawText(m_iScreenX + 318,m_iScreenY + posy,temp,COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	posy += linespace;

	sprintf(temp,"+%d%s",SELF.GetHolyWingAttack(),szStrongAdd);
	g_pFont->DrawText(m_iScreenX + 318,m_iScreenY + posy,temp,COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	posy += linespace;

	sprintf(temp,"+%d%s",SELF.GetHolyWingDefend(),szStrongAdd);
	g_pFont->DrawText(m_iScreenX + 318,m_iScreenY + posy,temp,COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	posy += linespace;

	sprintf(temp,"+%d%s",SELF.GetHolyWingMagicDefend(),szStrongAdd);
	g_pFont->DrawText(m_iScreenX + 318,m_iScreenY + posy,temp,COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	posy += linespace;


	sprintf(temp,"+%d%s",SELF.GetHolyWingMagicHit(),szStrongAdd);
	g_pFont->DrawText(m_iScreenX + 318,m_iScreenY + posy,temp,COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	posy += linespace;

	sprintf(temp,"+%d%s",SELF.GetHolyWingAttackAvoid(),szStrongAdd);
	g_pFont->DrawText(m_iScreenX + 318,m_iScreenY + posy,temp,COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	posy += linespace;

	sprintf(temp,"+%d%s",SELF.GetHolyWingCruelAttack(),szStrongAdd);
	g_pFont->DrawText(m_iScreenX + 318,m_iScreenY + posy,temp,COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	posy += linespace;

	sprintf(temp,"+%d%s",SELF.GetHolyWingReboundHurt(),szStrongAdd);
	g_pFont->DrawText(m_iScreenX + 318,m_iScreenY + posy,temp,COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	posy += linespace;

	sprintf(temp,"+%d%s",SELF.GetHolyWingAbsordBlood(),szStrongAdd);
	g_pFont->DrawText(m_iScreenX + 318,m_iScreenY + posy,temp,COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	posy += linespace;

	sprintf(temp,"+%d%s",SELF.GetHolyWingPoFang(),szStrongAdd);
	g_pFont->DrawText(m_iScreenX + 318,m_iScreenY + posy,temp,COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	posy += linespace;

	sprintf(temp,"+%d%s",SELF.GetHolyWingDiKang(),szStrongAdd);
	g_pFont->DrawText(m_iScreenX + 318,m_iScreenY + posy,temp,COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	posy += linespace;

	sprintf(temp,"+%d%s",SELF.GetHolyWingMagicAvoid(),szStrongAdd);
	g_pFont->DrawText(m_iScreenX + 318,m_iScreenY + posy,temp,COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	posy += linespace;


	// 绘制翅膀
	for (int i = 0; i < MAX_HOLYWING_PARTS; ++i)
	{
		if (SELF.GetHolyWingPartLevelUpExpMax() > 0)
		{
			if (SELF.GetHolyWingAttrExp(i) == SELF.GetHolyWingPartLevelUpExpMax())
			{
				LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,21673 + i,EP_UI);
				if (pTex)
				{
					if (iWingIndex >= 0 && iWingIndex < 6)
						g_pGfx->DrawTextureNL(m_iScreenX + 72, m_iScreenY + 48, pTex, g_AIGoodMgr.GetWingColorByLevel(iWingLevel));
				}
			}
		}
	}

	for (int i = 0; i < 5; ++i)
	{
		sprintf(temp,"%d级",(i+1)*10);
		g_pFont->DrawText(m_iScreenX + 13,m_iScreenY + 235 - 44 * i,temp,COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	}
	
	

}

bool CDivinityWingWnd::OnMouseMove(int iX, int iY)
{
	CParserTip* pTip = g_pControl->GetTipWnd();
	if(!pTip)
		return true;

	pTip->Clear();
	pTip->SetShow(false);
	pTip->GetSelfEquipTips()->SetShow(false);

	if (iX>332 && iX<392 && iY> 346 && iY<366)
	{
		if (m_pUp->IsEnable())
		{
			pTip->AddText("点击即可升级神翼等", COLOR_TEXT_NORMAL);
			pTip->AddText("级", COLOR_TEXT_NORMAL,-1);
		}
		else
		{
			pTip->AddText("4条属性经验值尚未全", COLOR_TEXT_NORMAL);
			pTip->AddText("部涨满，或人物等级还", COLOR_TEXT_NORMAL,-1);
			pTip->AddText("未达到神翼升级要求。", COLOR_TEXT_NORMAL,-1);
		}

		int x = m_iScreenX + iX + 10;
		int y = m_iScreenY + iY + 10;
		pTip->AdjustXY(x,y);
		pTip->Move(x,y);
		pTip->SetShow(true);
		return true;
	}
	else if (iX>341 && iX<378 && iY> 305 && iY<340)
	{
		std::string strSkillName;

		int iCareer = SELF.GetCareer();
		if (iCareer == 0)
			strSkillName = "怒斩天下";
		else if (iCareer == 1)
			strSkillName = "天怒惊雷";
		else
			strSkillName = "火毒攻心剑";
		
		pTip->AddText(strSkillName.c_str(), 0xFFFFFF00);
				
		sTipsCfg * pTipCfg = g_TipsCfg.GetSkillTips(strSkillName.c_str());
		if(pTipCfg)
		{
			pTip->AddSkillCfgTips(pTipCfg);
		}

		int x = m_iScreenX + iX + 10;
		int y = m_iScreenY + iY + 10;
		pTip->AdjustXY(x,y);
		pTip->Move(x,y);
		pTip->SetShow(true);
		return true;
	}
	else
	{
		std::string strSkillName;

		int iCareer = SELF.GetCareer();
		if (iCareer == 0)
			strSkillName = "怒斩天下";
		else if (iCareer == 1)
			strSkillName = "天怒惊雷";
		else
			strSkillName = "火毒攻心剑";

		for (int i = 0; i < 3; ++i)//上面二级还没有开放
		{
			if(iX > m_ptLevelPos[i].x && iX < m_ptLevelPos[i].x + 20 && iY > m_ptLevelPos[i].y && iY < m_ptLevelPos[i].y + 10)
			{				
				pTip->AddText(g_AIGoodMgr.GetWingNameByLevel((i+1)*10).c_str(),g_AIGoodMgr.GetWingColorByLevel((i+1)*10),-1,0,-1,FONT_DEFAULT,FONTSIZE_MIDDLE,DTF_BlackFrame);
				pTip->AddText(m_strWingNeedLevel[i].c_str(),TIPS_GREEN);
				pTip->AddText((m_strWingSkillLevel[i] + strSkillName).c_str(),0xFFFFCF63,-1);

				int x = m_iScreenX + iX + 10;
				int y = m_iScreenY + iY + 10;
				pTip->AdjustXY(x,y);
				pTip->Move(x,y);
				pTip->SetShow(true);
				return true;
			}
		}
	}

	
	return CCtrlWindowX::OnMouseMove(iX,iY);
}

