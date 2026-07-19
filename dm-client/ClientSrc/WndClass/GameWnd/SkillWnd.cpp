#include "SkillWnd.h"
#include "Global/Global.h"
#include "GameControl/GameControl.h"
#include "GameData/GameData.h"
#include "GameData/MagicDefine.h"
#include "Global/Interface/AudioInterface.h"
#include "GameData/MsgBoxMgr.h"
#include "Baseclass/Control/ParserTip.h"
#include "GameAI/AIMgr.h"
//#include "GameData/SkillXmlMgr.h"
#include "GameData/TipsCfg.h"
#include "GameData/ConfigData.h"
#include "GameData/OtherData.h"
#include "GameAI/AIConfigMgr.h"


//增加技能时要修改OnCreate函数中的77这个值！
//并要注意，如果某个职业的魔法个数已经超出了BUTTONCOUNT，
//就是修改BUTTONCOUNT的宏定义

INIT_WND_POSX(CSkillWnd,POS_AUTO,POS_AUTO)

std::string CSkillWnd::m_str[8]={"初级","中级","高级","专家","大师","宗师","权威","王者"};
std::string CSkillWnd::m_strSuper[6]={"","一重","二重","三重","四重","五重"};
 
CSkillWnd::CSkillWnd()
{
	m_sName = "SkillWnd";
	m_iTotalSkillCount  = 0;
	m_iPageType = sm_dwWindowType;


	for(int i=0;i<BUTTONCOUNT;i++)
		m_pLeftSkillButton[i] = NULL;

	for(int i=0;i< SKILL_NUM_PAGE;i++)
	{
		m_pRightSkillButton[i] = NULL;
		m_pAddExpButton[i] = NULL;
	}

	for (int i=0;i<M_CONSKILL_NUM_PAGE;i++)
	{
		m_iRightConSkill[i] = 0;
	}

	m_iIndex = 17430;

	m_iPages = 1;	

	m_pTabPage1 = m_pTabPage2 = m_pTabPage3 = m_pTabPage4 = NULL;
	m_pMarkViewer = NULL;
}

CSkillWnd::~CSkillWnd(void)
{
	//清空闪烁特效
	int iSize = SELF.MagicArray().Size();
	for (int iPos = 0; iPos < iSize; iPos++)
	{
		SELF.GetMagic(iPos).SetFlashTexID(0);
	}
	
	iSize = SELF.ProduceMagicArray().Size();
	for (int iPos = 0; iPos < iSize; iPos++)
	{
		SELF.GetProduceMagic(iPos).SetFlashTexID(0);
	}
}

void CSkillWnd::OnCreate()
{
	SetCloseButton(531,4,80);

	g_pControl->MsgToWnd(MSG_CTRL_PANEL_WND,MSG_CTRL_FLASH_BTN,1002);//停止按钮闪烁

	m_pLeftScroll = 0;
	m_pRightScroll = 0;
	m_pMarkViewer = NULL;
	m_pTabPage1 = m_pTabPage2 = m_pTabPage3 = m_pTabPage4 = NULL;

	if (m_iPageType == 0 || m_iPageType == 1)
	{
		m_pLeftScroll = new CCtrlScroll();
		AddControl(m_pLeftScroll);
		m_pLeftScroll->CreateEx(this,266,55,14,300);
		m_pLeftScroll->SetStep(1);
	  
		if (m_iPageType == 0)
		{
			if(SELF.GetCareer()==0)
			{
				m_pLeftScroll->SetRange(3);
			}
			else
			{
				m_pLeftScroll->SetRange(4);
			}
		}

		m_pLeftScroll->SetPos(m_pLeftScroll->GetRange());

		m_pRightScroll = new CCtrlScroll();
		AddControl(m_pRightScroll);
		m_pRightScroll->CreateEx(this,519,55,14,300);
		m_pRightScroll->SetStep(1);		
	}


	int iPageY = 37;
	m_pTabPage1= new CCtrlButton();
	AddControl(m_pTabPage1);
	m_pTabPage1->CreateEx(this,0,37,85,86,87);
	m_pTabPage1->SetText("战斗技能",COLOR_TABPAGE_NORMAL,COLOR_TABPAGE_MOUSEON,COLOR_TABPAGE_PRESS,COLOR_TABPAGE_DISABLE,FONTSIZE_MIDDLE,0,FONT_YAHEI,true);
	m_pTabPage1->SetTextOff(2,0);

	//iPageY += 86;
	//m_pTabPage2= new CCtrlButton();
	//AddControl(m_pTabPage2);
	//m_pTabPage2->CreateEx(this,0,iPageY,85,86,87);
	//m_pTabPage2->SetText("生活技能",COLOR_TABPAGE_NORMAL,COLOR_TABPAGE_MOUSEON,COLOR_TABPAGE_PRESS,COLOR_TABPAGE_DISABLE,FONTSIZE_MIDDLE,0,FONT_YAHEI,true);
	//m_pTabPage2->SetTextOff(2,0);

	//iPageY += 86;
	//m_pTabPage3 = new CCtrlButton();
	//AddControl(m_pTabPage3);
	//m_pTabPage3->CreateEx(this,0,iPageY,85,86,87);
	//m_pTabPage3->SetText("连续技能",COLOR_TABPAGE_NORMAL,COLOR_TABPAGE_MOUSEON,COLOR_TABPAGE_PRESS,COLOR_TABPAGE_DISABLE,FONTSIZE_MIDDLE,0,FONT_YAHEI,true);
	//m_pTabPage3->SetTextOff(2,0);

	if (g_OtherData.GetHorseManShipInfo().Grade > 0)
	{
		iPageY += 86;
		m_pTabPage4 = new CCtrlButton();
		AddControl(m_pTabPage4);
		m_pTabPage4->CreateEx(this,0,iPageY,85,86,87);
		m_pTabPage4->SetText("骑术技能",COLOR_TABPAGE_NORMAL,COLOR_TABPAGE_MOUSEON,COLOR_TABPAGE_PRESS,COLOR_TABPAGE_DISABLE,FONTSIZE_MIDDLE,0,FONT_YAHEI,true);
		m_pTabPage4->SetTextOff(2,0);
	}
	

	if(m_iPageType == 0)
	{
		m_pTabPage1->SetBackState(BTEX_PUSHED);
	}
	else if (m_iPageType == 1)
	{
		if (m_pTabPage2)
		{
			m_pTabPage2->SetBackState(BTEX_PUSHED);
		}
	}
	else if (m_iPageType == 2)
	{
		if (m_pTabPage3)
		{
			m_pTabPage3->SetBackState(BTEX_PUSHED);
		}
	}
	else if (m_iPageType == 3)
	{
		if (m_pTabPage4)
		{
			m_pTabPage4->SetBackState(BTEX_PUSHED);
		}
	}

	if (m_pTabPage2 && g_OtherData.IsNeedFlashProduceTab())
	{
		m_pTabPage2->SetFlashTexID(MAKELONG(21587,PACKAGE_INTERFACE));
		g_OtherData.SetNeedFlashProduceTab(false);
	}


	g_pControl->MsgToWnd(MSG_CTRL_PANEL_WND,MSG_REMOVE_ARROWTIP_CONTROL,EP_First_StudySkill_PaoPao);


	if( !g_pGameData->HasPaoPaoStatus(EP_First_Skill_SkillWnd_PaoPao) && g_pGameData->HasPaoPaoStatus(EP_First_StudySkill_PaoPao))
	{
		AddArrowTip(EP_First_Skill_SkillWnd_PaoPao,348,94,XAL_TOPLEFT,false,XAL_TOPRIGHT);
	}


	if (m_iPageType == 0 || m_iPageType == 1)
	{
		bool bHaveSkill = false;
		//右边按钮
		for(int i = 0; i < SKILL_NUM_PAGE; i++)
		{
			m_pRightSkillButton[i]= new CCtrlButton();
			AddControl(m_pRightSkillButton[i]);

			m_pRightSkillButton[i]->Create(this,284 + 49,i*34 + 76,CTRL_STYLE_BACKMODE_NODRAW,29,29);

			m_pAddExpButton[i]= new CCtrlButton();
			AddControl(m_pAddExpButton[i]);
			m_pAddExpButton[i]->CreateEx(this,269 + 42,i*34 + 82,14230,14231,14232,14230);			
		}

		UpdateSkill();


		//增加法诀技能,放在最后
		/*if (m_iPageType == 0)
		{
			for(int i = 0; i < SKILL_NUM_PAGE; i++)
			{
				if ( !m_pRightSkillButton[i]->IsEnable() && !m_pRightSkillButton[i]->IsShow())
				{
					m_pRightSkillButton[i]->SetEnable(true);
					m_pRightSkillButton[i]->SetShow(true);
					SetRightBtnTips(i,MAGICID_FAZHEN_CARRYON);
					break;
				}
			}
		}*/
	}



	int iLeftX = 22,iLeftY = 48;
	int iCareer = SELF.GetCareer();
	m_iTotalSkillCount = 0;

 	if (m_iPageType == 0)//普通技能,1:为生产技能
	{
		//左边按钮
		if(iCareer == 0)		// 战士
		{
			// 基本剑术     
			m_pLeftSkillButton[0]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[0]);
			m_pLeftSkillButton[0]->CreateEx(this,114+iLeftX,254+iLeftY,304,0,304);
			m_LeftSkill[0].szName="初级剑法";
			m_LeftSkill[0].dwMagicID = MAGICID_ATTACK_BASE;
			m_LeftSkill[0].iPageNumber = 0;

			//血影刀法
			m_pLeftSkillButton[1]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[1]);
			m_pLeftSkillButton[1]->CreateEx(this,178+iLeftX,205+iLeftY,380,0,380);
			m_LeftSkill[1].szName="血影刀法";
			m_LeftSkill[1].dwMagicID = MAGICID_BLOOD_SHADE;
			m_LeftSkill[1].iPageNumber = 0;

			//残影刀法
			m_pLeftSkillButton[2]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[2]);
			m_pLeftSkillButton[2]->CreateEx(this,114+iLeftX,145+iLeftY,378,0,378);
			m_LeftSkill[2].szName = "残影刀法";
			m_LeftSkill[2].dwMagicID = MAGICID_REMAIN_SHADE;
			m_LeftSkill[2].iPageNumber = 0;

			// 攻杀剑术
			m_pLeftSkillButton[3]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[3]);
			m_pLeftSkillButton[3]->CreateEx(this,51+iLeftX,85+iLeftY,312,0,312);
			m_LeftSkill[3].szName = "攻杀剑法";
			m_LeftSkill[3].dwMagicID = MAGICID_ATTACK_KILL;
			m_LeftSkill[3].iPageNumber = 0;

			//护身真气
			m_pLeftSkillButton[4]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[4]);
			m_pLeftSkillButton[4]->CreateEx(this,114+iLeftX,85+iLeftY,382,0,382);
			m_LeftSkill[4].szName = "护身真气";
			m_LeftSkill[4].dwMagicID = MAGICID_PROTECT_SKIN;
			m_LeftSkill[4].iPageNumber = 0;

			// 刺杀剑术
			m_pLeftSkillButton[5]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[5]);
			m_pLeftSkillButton[5]->CreateEx(this,116+iLeftX,262+iLeftY,322,0,322);
			m_LeftSkill[5].szName = "刺杀剑术";
			m_LeftSkill[5].dwMagicID = MAGICID_ATTACK_STICK;
			m_LeftSkill[5].iPageNumber = 1;

			//抱月刀
			m_pLeftSkillButton[6]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[6]);
			m_pLeftSkillButton[6]->CreateEx(this,51+iLeftX,210+iLeftY,384,0,384);
			m_LeftSkill[6].szName = "抱月刀";
			m_LeftSkill[6].dwMagicID = MAGICID_WHOLE_MOON;
			m_LeftSkill[6].iPageNumber = 1;

			// 半月弯刀
			m_pLeftSkillButton[7]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[7]);
			m_pLeftSkillButton[7]->CreateEx(this,179+iLeftX,208+iLeftY,348,0,348);
			m_LeftSkill[7].szName = "半月弯刀";
			m_LeftSkill[7].dwMagicID = MAGICID_ATTACK_MOON;
			m_LeftSkill[7].iPageNumber = 1;

			// 野蛮冲撞
			m_pLeftSkillButton[8]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[8]);
			m_pLeftSkillButton[8]->CreateEx(this,114+iLeftX,137+iLeftY,352,0,352);
			m_LeftSkill[8].szName = "野蛮冲撞";
			m_LeftSkill[8].dwMagicID =MAGICID_WILD_COLLIDE;
			m_LeftSkill[8].iPageNumber = 1;

			// 烈火
			m_pLeftSkillButton[9]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[9]);
			m_pLeftSkillButton[9]->CreateEx(this,51+iLeftX,58+iLeftY,350,0,350);
			m_LeftSkill[9].szName = "烈火剑法";
			m_LeftSkill[9].dwMagicID = MAGICID_ATTACK_FIRE;
			m_LeftSkill[9].iPageNumber = 1;

			//雷霆剑
			m_pLeftSkillButton[10]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[10]);
		 	m_pLeftSkillButton[10]->CreateEx(this,178+iLeftX,78+iLeftY,386,0,386);
			m_LeftSkill[10].szName = "雷霆剑";
			m_LeftSkill[10].dwMagicID = MAGICID_THUNDER_FIRE;
			m_LeftSkill[10].iPageNumber = 1;

			// 战士兽化
			m_pLeftSkillButton[11]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[11]);
			m_pLeftSkillButton[11]->CreateEx(this,113+iLeftX,23+iLeftY,366,0,366);
			m_LeftSkill[11].szName = "战士兽化";
			m_LeftSkill[11].dwMagicID = MAGICID_FIGHTER_WILD;
			m_LeftSkill[11].iPageNumber = -1;

			//击破
			m_pLeftSkillButton[12]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[12]);
			m_pLeftSkillButton[12]->CreateEx(this,114+iLeftX,15+iLeftY,408,0,408);
			m_LeftSkill[12].szName = "击破";
			m_LeftSkill[12].dwMagicID = MAGICID_DESTROY_WEAPON;
			m_LeftSkill[12].iPageNumber = -1;

			//战魂真悟
			m_pLeftSkillButton[13]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[13]);
			m_pLeftSkillButton[13]->CreateEx(this,115+iLeftX,248+iLeftY,2612,0,2612);
			m_LeftSkill[13].szName = "战魂真悟";
			m_LeftSkill[13].dwMagicID = MAGICID_WARRIOR_KNOW;
			m_LeftSkill[13].iPageNumber = 2;

			//破击剑法
			m_pLeftSkillButton[14]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[14]);
			m_pLeftSkillButton[14]->CreateEx(this,51+iLeftX,180+iLeftY,2613,0,2613);
			m_LeftSkill[14].szName = "破击剑法";
			m_LeftSkill[14].dwMagicID = MAGICID_DESTROY_SHELL;
			m_LeftSkill[14].iPageNumber = 2;

			//破盾斩
			m_pLeftSkillButton[15]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[15]);
			m_pLeftSkillButton[15]->CreateEx(this,51+iLeftX,81+iLeftY,2614,0,2614);
			m_LeftSkill[15].szName = "破盾斩";
			m_LeftSkill[15].dwMagicID = MAGICID_DESTROY_SHIELD;
			m_LeftSkill[15].iPageNumber = 2;

			//金刚护体
			m_pLeftSkillButton[16]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[16]);
			m_pLeftSkillButton[16]->CreateEx(this,114+iLeftX,25+iLeftY,2615,0,2615);
			m_LeftSkill[16].szName = "金刚护体";
			m_LeftSkill[16].dwMagicID = MAGICID_STEEL_PROTECT;
			m_LeftSkill[16].iPageNumber = 2;

			//突斩
			m_pLeftSkillButton[17]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[17]);
			m_pLeftSkillButton[17]->CreateEx(this,178+iLeftX,131+iLeftY,2616,0,2616);
			m_LeftSkill[17].szName = "突斩";
			m_LeftSkill[17].dwMagicID = MAGICID_SUDDEN_KILL;
			m_LeftSkill[17].iPageNumber = 2;

			//战士擒龙手
			m_pLeftSkillButton[18]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[18]);
			m_pLeftSkillButton[18]->CreateEx(this,178+iLeftX,145+iLeftY,2628,0,2628);
			m_LeftSkill[18].szName = "擒龙手";
			m_LeftSkill[18].dwMagicID = MAGICID_GRASP_DRAGON;
			m_LeftSkill[18].iPageNumber = 3;

			//战士移形换影
			m_pLeftSkillButton[19]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[19]);
			m_pLeftSkillButton[19]->CreateEx(this,50+iLeftX,205+iLeftY,2631,0,2631);
			m_LeftSkill[19].szName = "移形换影";
			m_LeftSkill[19].dwMagicID = MAGICID_SHIFT_TRANSPORT;
			m_LeftSkill[19].iPageNumber = 3;


			//狂怒旋风
			m_pLeftSkillButton[20]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[20]);
			m_pLeftSkillButton[20]->CreateEx(this,49+iLeftX,146+iLeftY,17224,0,17225);
			m_LeftSkill[20].szName = "狂怒旋风";
			m_LeftSkill[20].dwMagicID = MAGICID_ANGRYWHIRLWIND;
			m_LeftSkill[20].iPageNumber = 3;

			//英勇咆哮
			m_pLeftSkillButton[21]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[21]);
			m_pLeftSkillButton[21]->CreateEx(this,115+iLeftX,86+iLeftY,17232,0,17233);
			m_LeftSkill[21].szName = "英勇咆哮";
			m_LeftSkill[21].dwMagicID = MAGICID_HEROROAR;
			m_LeftSkill[21].iPageNumber = 3;

			//八方分影斩			
			m_pLeftSkillButton[22]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[22]);
			m_pLeftSkillButton[22]->CreateEx(this,114+iLeftX,23+iLeftY,17236,0,17237);
			m_LeftSkill[22].szName = "八方分影斩";
			m_LeftSkill[22].dwMagicID = MAGICID_SHADOWKILL8;
			m_LeftSkill[22].iPageNumber = 3;

			//连击技能
			m_pLeftSkillButton[23]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[23]);
			m_pLeftSkillButton[23]->CreateEx(this,49+iLeftX,251+iLeftY,17246,0,17247);
			m_LeftSkill[23].szName = "连斩无双";
			m_LeftSkill[23].dwMagicID = MAGICID_LIANJI;
			m_LeftSkill[23].iPageNumber = 0;

			m_iTotalSkillCount = 24;
		}
		else if(iCareer == 1)		// 魔法师
		{
			// 火球
			m_pLeftSkillButton[0]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[0]);
			m_pLeftSkillButton[0]->CreateEx(this,114+iLeftX,253+iLeftY,300,0,300);
			m_LeftSkill[0].szName = "小火球";
			m_LeftSkill[0].dwMagicID = MAGICID_FIREBALL;
			m_LeftSkill[0].iPageNumber = 0;

			// 抗拒火环
			m_pLeftSkillButton[1]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[1]);
			m_pLeftSkillButton[1]->CreateEx(this,178+iLeftX,215+iLeftY,314,0,314);
			m_LeftSkill[1].szName = "抗拒火环";
			m_LeftSkill[1].dwMagicID = MAGICID_DISPUTE_FIRE;
			m_LeftSkill[1].iPageNumber = 0;

			// 诱惑之光
			m_pLeftSkillButton[2]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[2]);
			m_pLeftSkillButton[2]->CreateEx(this,114+iLeftX,175+iLeftY,338,0,338);
			m_LeftSkill[2].szName = "诱惑之光";
			m_LeftSkill[2].dwMagicID = MAGICID_LURE_LIGHT;
			m_LeftSkill[2].iPageNumber = 0;

			// 地狱火焰
			m_pLeftSkillButton[3]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[3]);
			m_pLeftSkillButton[3]->CreateEx(this,115+iLeftX,118+iLeftY,316,0,316);
			m_LeftSkill[3].szName = "地狱火焰";
			m_LeftSkill[3].dwMagicID = MAGICID_HELL_FIRE;
			m_LeftSkill[3].iPageNumber = 0;

			// 雷电术
			m_pLeftSkillButton[4]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[4]);
			m_pLeftSkillButton[4]->CreateEx(this,50+iLeftX,75+iLeftY,320,0,320);
			m_LeftSkill[4].szName = "雷电术";
			m_LeftSkill[4].dwMagicID = MAGICID_THUNDER;
			m_LeftSkill[4].iPageNumber = 0;

			//冰箭术
			m_pLeftSkillButton[5]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[5]);
			m_pLeftSkillButton[5]->CreateEx(this,179+iLeftX,77+iLeftY,396,0,396);
			m_LeftSkill[5].szName = "冰箭术";
			m_LeftSkill[5].dwMagicID = MAGICID_ICE_ARROW;
			m_LeftSkill[5].iPageNumber = 0;

			// 瞬息移动
			m_pLeftSkillButton[6]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[6]);
			m_pLeftSkillButton[6]->CreateEx(this,114+iLeftX,24+iLeftY,340,0,340);
			m_LeftSkill[6].szName = "瞬间移动";
			m_LeftSkill[6].dwMagicID = MAGICID_MOVE_QUICK;
			m_LeftSkill[6].iPageNumber = 0;

			//风火轮
			m_pLeftSkillButton[7]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[7]);
			m_pLeftSkillButton[7]->CreateEx(this,114+iLeftX,262+iLeftY,398,0,398);
			m_LeftSkill[7].szName = "风火轮";
			m_LeftSkill[7].dwMagicID = MAGICID_ROTATE_FIRE;
			m_LeftSkill[7].iPageNumber = 1;

			// 火炎刀
			m_pLeftSkillButton[8]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[8]);
			m_pLeftSkillButton[8]->CreateEx(this,179+iLeftX,262+iLeftY,308,0,308);
			m_LeftSkill[8].szName = "火炎刀";
			m_LeftSkill[8].dwMagicID = MAGICID_ADV_FIREBALL;
			m_LeftSkill[8].iPageNumber = 1;

			// 爆裂火焰
			m_pLeftSkillButton[9]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[9]);
			m_pLeftSkillButton[9]->CreateEx(this,179+iLeftX,179+iLeftY,344,0,344);
			m_LeftSkill[9].szName = "爆裂火焰";
			m_LeftSkill[9].dwMagicID = MAGICID_BLOW_FIRE;
			m_LeftSkill[9].iPageNumber = 1;

			// 火墙
			m_pLeftSkillButton[10]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[10]);
			m_pLeftSkillButton[10]->CreateEx(this,114+iLeftX,127+iLeftY,342,0,342);
			m_LeftSkill[10].szName = "火墙";
			m_LeftSkill[10].dwMagicID = MAGICID_FIRE_WALL;
			m_LeftSkill[10].iPageNumber = 1;

			//冰龙破
			m_pLeftSkillButton[11]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[11]);
			m_pLeftSkillButton[11]->CreateEx(this,51+iLeftX,85+iLeftY,400,0,400);
			m_LeftSkill[11].szName = "冰龙破";
			m_LeftSkill[11].dwMagicID = MAGICID_ICE_DRAG;
			m_LeftSkill[11].iPageNumber = 1;

			// 疾光电影
			m_pLeftSkillButton[12]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[12]);
			m_pLeftSkillButton[12]->CreateEx(this,178+iLeftX,85+iLeftY,318,0,318);
			m_LeftSkill[12].szName = "疾光电影";
			m_LeftSkill[12].dwMagicID = MAGICID_TRACE_THUNDER;
			m_LeftSkill[12].iPageNumber = 1;

			//玄冰刃
			m_pLeftSkillButton[13]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[13]);
			m_pLeftSkillButton[13]->CreateEx(this,51+iLeftX,16+iLeftY,402,0,402);
			m_LeftSkill[13].szName = "玄冰刃";
			m_LeftSkill[13].dwMagicID = MAGICID_ICE_ARRAY;
			m_LeftSkill[13].iPageNumber = 1;

			// 地狱雷光
			m_pLeftSkillButton[14]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[14]);
			m_pLeftSkillButton[14]->CreateEx(this,178+iLeftX,250+iLeftY,346,0,346);
			m_LeftSkill[14].szName = "地狱雷光";
			m_LeftSkill[14].dwMagicID = MAGICID_HELL_THUNDER;
			m_LeftSkill[14].iPageNumber = 2;

			// 魔法盾
			m_pLeftSkillButton[15]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[15]);
		 	m_pLeftSkillButton[15]->CreateEx(this,51+iLeftX,215+iLeftY,360,0,360);
			m_LeftSkill[15].szName = "魔法盾";
			m_LeftSkill[15].dwMagicID = MAGICID_MAGIC_PROTECT;
			m_LeftSkill[15].iPageNumber = 2;

			// 圣言术
			m_pLeftSkillButton[16]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[16]);
			m_pLeftSkillButton[16]->CreateEx(this,114+iLeftX,130+iLeftY,362,0,362);
			m_LeftSkill[16].szName = "圣言术";
			m_LeftSkill[16].dwMagicID = MAGICID_GOD_SAY;
			m_LeftSkill[16].iPageNumber = 2;

			//五雷轰
			m_pLeftSkillButton[17]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[17]);
			m_pLeftSkillButton[17]->CreateEx(this,51+iLeftX,131+iLeftY,404,0,404);
			m_LeftSkill[17].szName = "五雷轰";
			m_LeftSkill[17].dwMagicID = MAGICID_MULTI_THUDER;
			m_LeftSkill[17].iPageNumber = 2;

			// 冰咆哮
			m_pLeftSkillButton[18]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[18]);
			m_pLeftSkillButton[18]->CreateEx(this,178+iLeftX,86+iLeftY,364,0,364);
			m_LeftSkill[18].szName = "冰咆哮";
			m_LeftSkill[18].dwMagicID = MAGICID_ICE_STORM;
			m_LeftSkill[18].iPageNumber = 2;

			// 冰旋风
			m_pLeftSkillButton[19]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[19]);
			m_pLeftSkillButton[19]->CreateEx(this,115+iLeftX,25+iLeftY,406,0,406);
			m_LeftSkill[19].szName = "冰旋风";
			m_LeftSkill[19].dwMagicID = MAGICID_ICE_WHIRLWIND;
			m_LeftSkill[19].iPageNumber = 2;

			// 法之魄
			m_pLeftSkillButton[20]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[20]);
			m_pLeftSkillButton[20]->CreateEx(this,178+iLeftX,240+iLeftY,2622,0,2622);
			m_LeftSkill[20].szName = "法之魄";
			m_LeftSkill[20].dwMagicID = MAGICID_WIZ_KNOW;
			m_LeftSkill[20].iPageNumber = 3;

			// 兽灵术
			m_pLeftSkillButton[21]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[21]);
			m_pLeftSkillButton[21]->CreateEx(this,178+iLeftX,176+iLeftY,2623,0,2623);
			m_LeftSkill[21].szName = "兽灵术";
			m_LeftSkill[21].dwMagicID = MAGICID_BEAST_SKILL;
			m_LeftSkill[21].iPageNumber = 3;

			// 风行术
			m_pLeftSkillButton[22]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[22]);
			m_pLeftSkillButton[22]->CreateEx(this,52+iLeftX,132+iLeftY ,2624,0,2624);
			m_LeftSkill[22].szName = "风影盾";
			m_LeftSkill[22].dwMagicID =  MAGICID_DODGE_SKILL;
			m_LeftSkill[22].iPageNumber = 3;

			//化身蝙蝠
			m_pLeftSkillButton[23]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[23]);
			m_pLeftSkillButton[23]->CreateEx(this,116+iLeftX,25+iLeftY,368,0,368);
			m_LeftSkill[23].szName = "化身蝙蝠";
			m_LeftSkill[23].dwMagicID = MAGICID_ENCHANTER_WILD;
			m_LeftSkill[23].iPageNumber = 3;

			// 狂龙紫电
			m_pLeftSkillButton[24]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[24]);
			m_pLeftSkillButton[24]->CreateEx(this,113+iLeftX,87+iLeftY,2625,0,2625);
			m_LeftSkill[24].szName = "狂龙紫电";
			m_LeftSkill[24].dwMagicID = MAGICID_DRAGON_LIGHT;
			m_LeftSkill[24].iPageNumber = 3;

			// 魔魂术
			m_pLeftSkillButton[25]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[25]);
			m_pLeftSkillButton[25]->CreateEx(this,179+iLeftX,263+iLeftY,2630,0,2630);
			m_LeftSkill[25].szName = "魔魂术";
			m_LeftSkill[25].dwMagicID = MAGICID_SUCK_SOUL;
			m_LeftSkill[25].iPageNumber = 4;

			//流星火雨
			m_pLeftSkillButton[26]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[26]);
			m_pLeftSkillButton[26]->CreateEx(this,114+iLeftX,195+iLeftY,2627,0,2627);
			m_LeftSkill[26].szName = "流星火雨";
			m_LeftSkill[26].dwMagicID = MAGICID_FIRE_RAIN;
			m_LeftSkill[26].iPageNumber = 4;

			//寒冰风暴
			m_pLeftSkillButton[27]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[27]);
			m_pLeftSkillButton[27]->CreateEx(this,177+iLeftX,128+iLeftY,17226,0,17227);
			m_LeftSkill[27].szName = "寒冰风暴";
			m_LeftSkill[27].dwMagicID = MAGICID_COLDSTROM;
			m_LeftSkill[27].iPageNumber = 4;

			//电灵附体
			m_pLeftSkillButton[28]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[28]);
			m_pLeftSkillButton[28]->CreateEx(this,50+iLeftX,128+iLeftY,17234,0,17235);
			m_LeftSkill[28].szName = "电灵附体";
			m_LeftSkill[28].dwMagicID = MAGICID_SOULATTACH;
			m_LeftSkill[28].iPageNumber = 4;

			//强袭烈焰盾
			m_pLeftSkillButton[29]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[29]);
			m_pLeftSkillButton[29]->CreateEx(this,114+iLeftX,33+iLeftY,17238,0,17239);
			m_LeftSkill[29].szName = "强袭烈焰盾";
			m_LeftSkill[29].dwMagicID = MAGICID_FIRE_SHIELD_ATTACK;
			m_LeftSkill[29].iPageNumber = 4;

			//连击技能
			m_pLeftSkillButton[30]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[30]);
			m_pLeftSkillButton[30]->CreateEx(this,49+iLeftX,251+iLeftY,17246,0,17247);
			m_LeftSkill[30].szName = "连斩无双";
			m_LeftSkill[30].dwMagicID = MAGICID_LIANJI;
			m_LeftSkill[30].iPageNumber = 0;


			m_iTotalSkillCount = 31;
		}
		else	// 道士
		{
			// 治愈术
			m_pLeftSkillButton[0]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[0]);
			m_pLeftSkillButton[0]->CreateEx(this,178+iLeftX,253+iLeftY,302,0,302);
			m_LeftSkill[0].szName = "治疗术";
			m_LeftSkill[0].dwMagicID = MAGICID_HEAL;
			m_LeftSkill[0].iPageNumber = 0;

			// 精神战法
			m_pLeftSkillButton[1]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[1]);
			m_pLeftSkillButton[1]->CreateEx(this,115+iLeftX,210+iLeftY,306,0,306);
			m_LeftSkill[1].szName ="精神战法"; 
			m_LeftSkill[1].dwMagicID = MAGICID_ATTACK_SOUL;
			m_LeftSkill[1].iPageNumber = 0;

			//施毒术
			m_pLeftSkillButton[2]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[2]);
			m_pLeftSkillButton[2]->CreateEx(this,51+iLeftX,126+iLeftY,310,0,310);
			m_LeftSkill[2].szName = "施毒术";
			m_LeftSkill[2].dwMagicID = MAGICID_POISON_MAGIC;
			m_LeftSkill[2].iPageNumber = 0;

			//诅咒术
			m_pLeftSkillButton[3]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[3]);
			m_pLeftSkillButton[3]->CreateEx(this,114+iLeftX,126+iLeftY,388,0,388);
			m_LeftSkill[3].szName ="诅咒术";
			m_LeftSkill[3].dwMagicID = MAGICID_CURSE;
			m_LeftSkill[3].iPageNumber = 0;

			// 灵魂火符
			m_pLeftSkillButton[4]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[4]);
			m_pLeftSkillButton[4]->CreateEx(this,114+iLeftX,44+iLeftY,324,0,324);
			m_LeftSkill[4].szName = "灵魂道符";
			m_LeftSkill[4].dwMagicID = MAGICID_PROTECT_SYMBOL;
			m_LeftSkill[4].iPageNumber = 0;

			//赶尸
			m_pLeftSkillButton[5]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[5]);
			m_pLeftSkillButton[5]->CreateEx(this,51+iLeftX,260+iLeftY,390,0,390);
			m_LeftSkill[5].szName = "赶尸";
			m_LeftSkill[5].dwMagicID = MAGICID_CONTROL_CORPSE;
			m_LeftSkill[5].iPageNumber = 1;

			// 召唤骷髅
			m_pLeftSkillButton[6]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[6]);
			m_pLeftSkillButton[6]->CreateEx(this,178+iLeftX,260+iLeftY,332,0,332);
			m_LeftSkill[6].szName = "骷髅召唤术";
			m_LeftSkill[6].dwMagicID = MAGICID_CALL_MONSTER;
			m_LeftSkill[6].iPageNumber = 1;

			//替身法符
			m_pLeftSkillButton[7]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[7]);
			m_pLeftSkillButton[7]->CreateEx(this,51+iLeftX,172+iLeftY,392,0,392);
			m_LeftSkill[7].szName = "替身法符";
			m_LeftSkill[7].dwMagicID = MAGICID_REPLACE_MAGIC;
			m_LeftSkill[7].iPageNumber = 1;

			// 隐身术
			m_pLeftSkillButton[8]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[8]);
			m_pLeftSkillButton[8]->CreateEx(this,178+iLeftX,172+iLeftY,334,0,334);
			m_LeftSkill[8].szName = "隐身术";
			m_LeftSkill[8].dwMagicID = MAGICID_HIDE_SKILL;
			m_LeftSkill[8].iPageNumber = 1;

			//集体隐身术
			m_pLeftSkillButton[9]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[9]);
			m_pLeftSkillButton[9]->CreateEx(this,114+iLeftX,89+iLeftY,336,0,336);
			m_LeftSkill[9].szName = "集体隐身术";
			m_LeftSkill[9].dwMagicID = MAGICID_HIDE_ALL;
			m_LeftSkill[9].iPageNumber = 1;

			// 幽灵盾
			m_pLeftSkillButton[10]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[10]);
			m_pLeftSkillButton[10]->CreateEx(this,178+iLeftX,46+iLeftY,326,0,326);
			m_LeftSkill[10].szName = "幽灵盾";
			m_LeftSkill[10].dwMagicID = MAGICID_PROTECT_GHOST;
			m_LeftSkill[10].iPageNumber = 1;

			// 神圣战甲术
			m_pLeftSkillButton[11]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[11]);
			m_pLeftSkillButton[11]->CreateEx(this,51+iLeftX,18+iLeftY,328,0,328);
			m_LeftSkill[11].szName = "神圣战甲术";
			m_LeftSkill[11].dwMagicID = MAGICID_PROTECT_ARMOR;
			m_LeftSkill[11].iPageNumber = 1;

			// 狮子吼
			m_pLeftSkillButton[12]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[12]);
			m_pLeftSkillButton[12]->CreateEx(this,114+iLeftX,262+iLeftY,372,0,372);
			m_LeftSkill[12].szName = "狮子吼";
			m_LeftSkill[12].dwMagicID = MAGICID_LION_CALL;
			m_LeftSkill[12].iPageNumber = 2;

			//灵魂墙
			m_pLeftSkillButton[13]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[13]);
			m_pLeftSkillButton[13]->CreateEx(this,51+iLeftX,183+iLeftY,394,0,394);
			m_LeftSkill[13].szName = "灵魂墙";
			m_LeftSkill[13].dwMagicID = MAGICID_SOUL_WALL;
			m_LeftSkill[13].iPageNumber = 2;

			// 困魔咒
			m_pLeftSkillButton[14]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[14]);
			m_pLeftSkillButton[14]->CreateEx(this,178+iLeftX,183+iLeftY,330,0,330);
			m_LeftSkill[14].szName = "困魔咒";
			m_LeftSkill[14].dwMagicID = MAGICID_LOCK_MONSTER;
			m_LeftSkill[14].iPageNumber = 2;


			// 斗转星移
			m_pLeftSkillButton[15]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[15]);
			m_pLeftSkillButton[15]->CreateEx(this,114+iLeftX,133+iLeftY,17222,0,17222);
			m_LeftSkill[15].szName = "斗转星移";
			m_LeftSkill[15].dwMagicID = MAGICID_TRANSFER_HURT;
			m_LeftSkill[15].iPageNumber = 2;

			// 群体治愈术
			m_pLeftSkillButton[16]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[16]);
			m_pLeftSkillButton[16]->CreateEx(this,51+iLeftX,80+iLeftY,356,0,356);
			m_LeftSkill[16].szName = "群体治愈术";
			m_LeftSkill[16].dwMagicID = MAGICID_TEAM_HEAL;
			m_LeftSkill[16].iPageNumber = 2;

			//召唤神兽
			m_pLeftSkillButton[17]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[17]);
			m_pLeftSkillButton[17]->CreateEx(this,113+iLeftX,24+iLeftY,358,0,358);
			m_LeftSkill[17].szName = "召唤神兽";
			m_LeftSkill[17].dwMagicID = MAGICID_CALL_JOSS;
			m_LeftSkill[17].iPageNumber = 2;

			// 兽化
			m_pLeftSkillButton[18]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[18]);
			m_pLeftSkillButton[18]->CreateEx(this,114+iLeftX,29+iLeftY,370,0,370);
			m_LeftSkill[18].szName = "道士兽化";
			m_LeftSkill[18].dwMagicID = MAGICID_TAOIST_WILD;
			m_LeftSkill[18].iPageNumber = -1;

			// 道心清明
			m_pLeftSkillButton[19]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[19]);
			m_pLeftSkillButton[19]->CreateEx(this,115+iLeftX,248+iLeftY,2617,0,2617);
			m_LeftSkill[19].szName = "道心清明";
			m_LeftSkill[19].dwMagicID = MAGICID_TAOIST_KNOW;
			m_LeftSkill[19].iPageNumber = 3;

			// 神光术
			m_pLeftSkillButton[20]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[20]);
			m_pLeftSkillButton[20]->CreateEx(this,115+iLeftX,179+iLeftY,2618,0,2618);
			m_LeftSkill[20].szName = "神光术";
			m_LeftSkill[20].dwMagicID = MAGICID_GODLIGHT_SKILL;
			m_LeftSkill[20].iPageNumber = 3;

			// 解毒术
			m_pLeftSkillButton[21]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[21]);
			m_pLeftSkillButton[21]->CreateEx(this,178+iLeftX,119+iLeftY,2619,0,2619);
			m_LeftSkill[21].szName = "解毒术";
			m_LeftSkill[21].dwMagicID = MAGICID_DESTROY_POISON;
			m_LeftSkill[21].iPageNumber = 3;

			// 强化召唤骷髅
			m_pLeftSkillButton[22]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[22]);
			m_pLeftSkillButton[22]->CreateEx(this,115+iLeftX,26+iLeftY,2620,0,2620);
			m_LeftSkill[22].szName = "强化骷髅术";
			m_LeftSkill[22].dwMagicID = MAGICID_ENCALL_MONSTER;
			m_LeftSkill[22].iPageNumber = 3;

			// 幽冥火咒
			m_pLeftSkillButton[23]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[23]);
			m_pLeftSkillButton[23]->CreateEx(this,51+iLeftX,72+iLeftY,2621,0,2621);
			m_LeftSkill[23].szName = "幽冥火咒";
			m_LeftSkill[23].dwMagicID = MAGICID_FIRE_JUJU;
			m_LeftSkill[23].iPageNumber = 3;

			//心灵召唤
			m_pLeftSkillButton[24]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[24]);
			m_pLeftSkillButton[24]->CreateEx(this,115+iLeftX,200+iLeftY,2626,0,2626);
			m_LeftSkill[24].szName = "心灵召唤";
			m_LeftSkill[24].dwMagicID = MAGICID_PET_CALL;
			m_LeftSkill[24].iPageNumber = 4;

			//遁地
			m_pLeftSkillButton[25]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[25]);
			m_pLeftSkillButton[25]->CreateEx(this,177+iLeftX,246+iLeftY,2629,0,2629);
			m_LeftSkill[25].szName = "遁地";
			m_LeftSkill[25].dwMagicID = MAGICID_ESCAPE_QUICK;
			m_LeftSkill[25].iPageNumber = 4;

			//心灵爆破
			m_pLeftSkillButton[26]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[26]);
			m_pLeftSkillButton[26]->CreateEx(this,177+iLeftX,114+iLeftY,17228,0,17229);
			m_LeftSkill[26].szName = "心灵爆破";
			m_LeftSkill[26].dwMagicID = MAGICID_QUNMOTUNSHI;
			m_LeftSkill[26].iPageNumber = 4;

			//骷髅囚笼
			m_pLeftSkillButton[27]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[27]);
			m_pLeftSkillButton[27]->CreateEx(this,49+iLeftX,65+iLeftY,17230,0,17231);
			m_LeftSkill[27].szName = "骷髅囚笼";
			m_LeftSkill[27].dwMagicID = MAGICID_SKULLCAGE;
			m_LeftSkill[27].iPageNumber = 4;

			//剧毒沼泽
			m_pLeftSkillButton[28]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[28]);
			m_pLeftSkillButton[28]->CreateEx(this,178+iLeftX,126+iLeftY,17244,0,17245);
			m_LeftSkill[28].szName = "剧毒沼泽";
			m_LeftSkill[28].dwMagicID = MAGICID_JUDUZHAOZE;
			m_LeftSkill[28].iPageNumber = 0;

			//无量五行诀
			m_pLeftSkillButton[29]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[29]);
			m_pLeftSkillButton[29]->CreateEx(this,114+iLeftX,23+iLeftY,17240,0,17241);
			m_LeftSkill[29].szName = "无量五行诀";
			m_LeftSkill[29].dwMagicID = MAGICID_DAOZUN_JIANGLIN;
			m_LeftSkill[29].iPageNumber = 4;

			//连击技能
			m_pLeftSkillButton[30]= new CCtrlButton();
			AddControl(m_pLeftSkillButton[30]);
			m_pLeftSkillButton[30]->CreateEx(this,49+iLeftX,251+iLeftY,17246,0,17247);
			m_LeftSkill[30].szName = "连斩无双";
			m_LeftSkill[30].dwMagicID = MAGICID_LIANJI;
			m_LeftSkill[30].iPageNumber = 0;

			m_iTotalSkillCount = 31;
		}
	}
	else if (m_iPageType == 1)
	{
		// 猎取
		m_pLeftSkillButton[0]= new CCtrlButton();
		AddControl(m_pLeftSkillButton[0]);
		m_pLeftSkillButton[0]->CreateEx(this,127,280,17214,0,17214);
		m_LeftSkill[0].szName = "猎取";
		m_LeftSkill[0].dwMagicID = MAGICID_HUNTSKILL;
		m_LeftSkill[0].iPageNumber = 0;

		// 伐木
		m_pLeftSkillButton[1]= new CCtrlButton();
		AddControl(m_pLeftSkillButton[1]);
		m_pLeftSkillButton[1]->CreateEx(this,184,223,17212,0,17212);
		m_LeftSkill[1].szName ="伐木"; 
		m_LeftSkill[1].dwMagicID = MAGICID_FELLWOOD;
		m_LeftSkill[1].iPageNumber = 0;

		//挖矿
		m_pLeftSkillButton[2]= new CCtrlButton();
		AddControl(m_pLeftSkillButton[2]);
		m_pLeftSkillButton[2]->CreateEx(this,127,166,17210,0,17210);
		m_LeftSkill[2].szName = "挖矿";
		m_LeftSkill[2].dwMagicID = MAGICID_DIGMINE;
		m_LeftSkill[2].iPageNumber = 0;

		//制炼
		m_pLeftSkillButton[3]= new CCtrlButton();
		AddControl(m_pLeftSkillButton[3]);
		m_pLeftSkillButton[3]->CreateEx(this,70,109,17220,0,17220);
		m_LeftSkill[3].szName = "制炼";
		m_LeftSkill[3].dwMagicID = MAGICID_SMELTSKILL;
		m_LeftSkill[3].iPageNumber = 0;

		//铸造
		m_pLeftSkillButton[4]= new CCtrlButton();
		AddControl(m_pLeftSkillButton[4]);
		m_pLeftSkillButton[4]->CreateEx(this,185,109,17218,0,17218);
		m_LeftSkill[4].szName ="铸造";
		m_LeftSkill[4].dwMagicID = MAGICID_FOUNDSKILL;
		m_LeftSkill[4].iPageNumber = 0;

		// 拆卸
		m_pLeftSkillButton[5]= new CCtrlButton();
		AddControl(m_pLeftSkillButton[5]);
		m_pLeftSkillButton[5]->CreateEx(this,128,52,17216,0,17216);
		m_LeftSkill[5].szName = "拆卸";
		m_LeftSkill[5].dwMagicID = MAGICID_DISASSEMBLE;
		m_LeftSkill[5].iPageNumber = 0;

		m_iTotalSkillCount = 6;
	}
	else if (m_iPageType == 2)
	{
		int csStartX = 70;
		int csStartY = 13;

		// 创建 M_CONSKILL_NUM_PAGE 行， M_MAXCONSKILLSUBNUM 列
		for (int iline = 0; iline < M_CONSKILL_NUM_PAGE;++iline)
		{			
			csStartX = 70;
			for (int submagic = 0; submagic < M_MAXCONSKILLSUBNUM; ++submagic)
			{
				int lefti = iline * M_MAXCONSKILLSUBNUM + submagic;
				m_pLeftSkillButton[lefti]= new CCtrlButton();
				AddControl(m_pLeftSkillButton[lefti]);
				m_pLeftSkillButton[lefti]->Create(this,csStartX+iLeftX,csStartY+iLeftY,CTRL_STYLE_BACKMODE_NODRAW,29,29);
				m_pLeftSkillButton[lefti]->SetShow(false);
				m_pLeftSkillButton[lefti]->SetEnable(false);

				csStartX += 80;

				++m_iTotalSkillCount;
			}

			m_pRightSkillButton[iline]= new CCtrlButton();
			AddControl(m_pRightSkillButton[iline]);
			m_pRightSkillButton[iline]->CreateEx(this,csStartX+iLeftX + 60,csStartY+iLeftY + 5,132,133,134,135);
			m_pRightSkillButton[iline]->SetShow(false);
			m_pRightSkillButton[iline]->SetEnable(false);

			csStartY += 59;
		}

		UpdateConSkillState();
	}
	else if (m_iPageType == 3)
	{
		m_pMarkViewer = new CMarkViewer(16,35);
		AddControl(m_pMarkViewer);
		m_pMarkViewer->Create(this,278,60,253,170);
		m_pMarkViewer->SetTagText(&m_kHelpText);
		m_pMarkViewer->SetEnable(true);
		m_pMarkViewer->SetShow(true);

		string str = g_AICfgMgr.GetQiShuHelpByName("@@main");

		m_kHelpText.Clear();
		m_kHelpText.Parse(str,0,"\\",str.length());


		if (iCareer == JOB_ZHANSHI)
		{
			char* skillname[10] = {"骑战训练","驯服坐骑","浴血奋战","势如破竹","迅疾如风","骑兵冲击","人骑合一","翱风斩","",""};

			for (int j = 0; j < 2; ++j)
			{
				for (int i = 0; i < 5; ++i)
				{
					if (strlen(skillname[j*5+i]) > 0)
					{
						m_LeftSkill[j*5+i].szName = skillname[j*5+i];
						if (strcmp(skillname[j*5+i], "翱风斩") == 0)
						{
							m_LeftSkill[j*5+i].dwMagicID = MAGICID_AOFENGCUT_ATTACK;
							m_LeftSkill[j*5+i].bActive = true;
						}
						else
						{
							m_LeftSkill[j*5+i].dwMagicID = j*5+i+1;
							m_LeftSkill[j*5+i].bActive = false;
						}

						m_LeftSkill[j*5+i].iPageNumber = 1;
					}
				}
			}

			m_iTotalSkillCount = 8;
		}
		else if (iCareer == JOB_FASHI)
		{
			char* skillname[10] = {"妖兽守护","骑兽灵性","魔力压缩","法力复兴","汲取生命","妖兽之心","双重施法","纵雷诀","",""};

			for (int j = 0; j < 2; ++j)
			{
				for (int i = 0; i < 5; ++i)
				{
					if (strlen(skillname[j*5+i]) > 0)
					{						
						m_LeftSkill[j*5+i].szName = skillname[j*5+i];

						if (strcmp(skillname[j*5+i], "纵雷诀") == 0)
						{
							m_LeftSkill[j*5+i].dwMagicID = MAGICID_ZHONGLEI_ATTACK;
							m_LeftSkill[j*5+i].bActive = true;
						}
						else
						{
							m_LeftSkill[j*5+i].dwMagicID = j*5+i+1+7;
							m_LeftSkill[j*5+i].bActive = false;
						}
						
						m_LeftSkill[j*5+i].iPageNumber = 1;
					}
				}
			}

			m_iTotalSkillCount = 8;
		}
		else if (iCareer == JOB_DAO)
		{
			char* skillname[10] = {"骑兽沟通","灵力喂食","灵力虹吸","鬼门秘法","原始秩序","傀儡师","道法自然","毒凌波","",""};

			for (int j = 0; j < 2; ++j)
			{
				for (int i = 0; i < 5; ++i)
				{
					if (strlen(skillname[j*5+i]) > 0)
					{						
						m_LeftSkill[j*5+i].szName = skillname[j*5+i];

						if (strcmp(skillname[j*5+i], "毒凌波") == 0)
						{
							m_LeftSkill[j*5+i].dwMagicID = MAGICID_DULINGBO_ATTACK;
							m_LeftSkill[j*5+i].bActive = true;
						}
						else
						{
							m_LeftSkill[j*5+i].dwMagicID = j*5+i+1+14;
							m_LeftSkill[j*5+i].bActive = false;
						}
						
						m_LeftSkill[j*5+i].iPageNumber = 1;
					}
				}
			}

			m_iTotalSkillCount = 8;
		}
		
	}

	//for(int i=0;i<m_iTotalSkillCount;i++)
	//{
	//	vector<string>* pDesc = NULL;
	//	pDesc = g_SkillXmlMgr.GetSkillDesc(m_LeftSkill[i].szName.c_str());
	//	if(NULL == pDesc)
	//		m_LeftSkill[i].vecDesc.clear();
	//	else
	//	{
	//		m_LeftSkill[i].vecDesc.clear();
	//		for(int iRow = 0;iRow<pDesc->size();iRow++)
	//		{
	//			m_LeftSkill[i].vecDesc.push_back(pDesc->at(iRow));
	//		}
	//	}
	//}

	RefreshLeftSkillButtonState();
}

bool CSkillWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_FLASH_BTN_WND:
		{
			if (dwData == 2)
			{
				if (m_iPageType == 0 && m_pTabPage2)
				{
					m_pTabPage2->SetFlashTexID(MAKELONG(21587,PACKAGE_INTERFACE));
					g_OtherData.SetNeedFlashProduceTab(false);
				}
				return true;
			}

			return false;
		}
	case MSG_CTRL_SCROLL_MOVE:
		{
			if (pControl == m_pLeftScroll)
			{
				RefreshLeftSkillButtonState();
				return true;
			}
			
			if (pControl == m_pRightScroll)
			{
				SetAllRightBtnTips();	
				return true;
			}
		}
		break;
	case MSG_CTRL_UPDATE_SKILLWND:
		{			
			RefreshLeftSkillButtonState();
			UpdateConSkillState();
			UpdateSkill();
			return true;
		}
	case MSG_CTRL_BUTTON_CLICK:
		{
			if (pControl == m_pTabPage1)
			{
				SwitchToPage(0);
				return true;
			}

			if (pControl == m_pTabPage2)
			{
				SwitchToPage(1);
				return true;
			}
			if (pControl == m_pTabPage3)
			{
				SwitchToPage(2);
				return true;
			}
			if (pControl == m_pTabPage4)
			{
				SwitchToPage(3);
				return true;
			}

			//else if(pControl == m_pLeftUpButton) // 左上翻页
			//{
			//	g_pAudio->Play(EAT_OTHER,6,g_pAudio->GetRand()++);

			//	m_nLeftPage++;
			//	if(m_iPageType == 1 || (m_nLeftPage >3 && SELF.GetCareer()==0) || m_nLeftPage>4) 
			//	{
			//		g_MsgBoxMgr.PopSimpleAlert("目前无法查看更多的魔法技能！");
			//		m_nLeftPage -= 1; 
			//		return true;
			//	}

			//	int iCareer = SELF.GetCareer();
			//	SetPageTex(0,PACKAGE_INTERFACE,4900+iCareer*5+m_nLeftPage);
			//	RefreshLeftSkillButtonState();

			//	return true;
			//}
			//else if( pControl == m_pLeftDownButton) // 左下翻页
			//{
			//	g_pAudio->Play(EAT_OTHER,6,g_pAudio->GetRand()++);

			//	if(m_nLeftPage <= 0) 
			//		break;

			//	m_nLeftPage--;

			//	int iCareer = SELF.GetCareer();
			//	SetPageTex(0,PACKAGE_INTERFACE,4900+iCareer*5+m_nLeftPage);
			//	RefreshLeftSkillButtonState();

			//	return true;
			//}
			//else if(pControl == m_pRightUpButton)
			//{
			//	if(m_nRightPage > 0)
			//		m_nRightPage--;

			//	g_pAudio->Play(EAT_OTHER,6,g_pAudio->GetRand()++);
			//	SetAllRightBtnTips();
			//	return true;
			//}
			//else if(pControl == m_pRightDownButton)
			//{
			//	int b = 0;
			//	if (m_iPageType == 0)
			//	{
			//		for(int i = 0 ; i < MAX_MAGIC_SKILL; i++)
			//		{
			//			if(SELF.GetMagic(i).GetMagicID() != 0)
			//				b++;
			//		}
			//	}
			//	else if (m_iPageType == 1)
			//	{
			//		for(int i = 0 ; i < MAX_PRODUCEMAGIC_SKILL; i++)
			//		{
			//			if(SELF.GetProduceMagic(i).GetMagicID() != 0)
			//				b++;
			//		}
			//	}

			//	b = (b-1) / SKILL_NUM_PAGE;

			//	if(m_nRightPage < b)
			//	{
			//		m_nRightPage ++;

			//		g_pAudio->Play(EAT_OTHER,6,g_pAudio->GetRand()++);
			//		SetAllRightBtnTips();
			//	}

			//	return true;
			//}
			else if(pControl != NULL)
			{
				for(int i = 0;i < 8;i++)
				{
					if (m_iPageType == 0)
					{
						if(pControl == m_pRightSkillButton[i])
						{
							g_pAudio->Play(EAT_OTHER,10,g_pAudio->GetRand()++);
							int iPos = i + m_pRightScroll->GetPos() * SKILL_NUM_PAGE;

							if (iPos >= 0 && iPos < m_iRightSkillIndexs.size())
							{
								//如果超过魔法队列,可能是法诀技能
								if (m_iRightSkillIndexs[iPos] < SELF.MagicArray().GetMagicNumber())
								{
									g_AIMgr.SetShortCutKeyMagicPos(m_iRightSkillIndexs[iPos]);
									g_pControl->PopupWindow(MSG_CTRL_SHORTCUT_WND,OPER_CREATE,1);
								} 
								/*else if (m_iRightSkillIndexs[iPos] == SELF.MagicArray().GetMagicNumber())
								{
								g_pControl->PopupWindow(MSG_CTRL_SHORTCUT_WND,OPER_CREATE,3);
								}*/
							}							
							
							return true;
						}
					}
					else if (m_iPageType == 2)
					{
						if(pControl == m_pRightSkillButton[i])
						{
							g_pAudio->Play(EAT_OTHER,10,g_pAudio->GetRand()++);
							g_AIMgr.SetShortCutKeyConSkillID(m_iRightConSkill[i]);
							g_pControl->PopupWindow(MSG_CTRL_SHORTCUT_WND,OPER_CREATE,2);
							return true;
						}
					}

					if (m_iPageType == 0 || m_iPageType == 1)
					{					
						if(pControl == m_pAddExpButton[i])
						{
							g_pAudio->Play(EAT_OTHER,10,g_pAudio->GetRand()++);

							int iPos = i + m_pRightScroll->GetPos() * SKILL_NUM_PAGE;

							if (iPos >= 0 && iPos < m_iRightSkillIndexs.size())
							{
								WORD wMagicID = 0;

								if (m_iPageType == 0)
								{
									wMagicID = SELF.GetMagic(m_iRightSkillIndexs[iPos]).GetMagicID();
								}
								else
								{
									wMagicID = SELF.GetProduceMagic(m_iRightSkillIndexs[iPos]).GetMagicID();
								}

								if (wMagicID >0)
								{
									int npos = SELF.PackageGood().FindGoodByName("强化技能灵丹");
									if (npos >= 0)
									{
										g_pGameControl->SEND_Use_Object(npos,0,wMagicID);
									}
									else
									{
										g_MsgBoxMgr.PopSimpleAlert("您的包裹中没有强化技能灵丹");
									}
								}
							}							

							return true;
						}
					}

				}

				if (m_iPageType == 0)
				{
					for(int i=0;i<m_iTotalSkillCount;i++)
					{
						if(pControl == m_pLeftSkillButton[i])
						{
							for(int iPos = 0; iPos < MAX_MAGIC_SKILL; iPos++)
							{
								DWORD dwMagicID = SELF.GetMagic(iPos).GetMagicID();

								if(dwMagicID == m_LeftSkill[i].dwMagicID)
								{
									g_pAudio->Play(EAT_OTHER,10,g_pAudio->GetRand()++);
									g_AIMgr.SetShortCutKeyMagicPos(iPos);
									g_pControl->PopupWindow(MSG_CTRL_SHORTCUT_WND,OPER_CREATE,1);
									break;
								}
							}						
							return true;
						}
					}
				}
			}
		}
		break;
	case MSG_CTRL_POPUP_ARROWTIP:
		{
		 	if (dwData == EP_First_StudySkill_PaoPao)
			{
				AddArrowTip(EP_First_StudySkill_PaoPao,348,94,XAL_TOPLEFT,false,XAL_TOPRIGHT);
			}
		}
		return true;
	break;

	case MSG_CTRL_SKILLWND:
		{
			if (dwData == 10)
			{
				SwitchToPage(m_iPageType, true);
				return true;
			}
		}
		break;

	default:
		break;
	}
	return  CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}



void CSkillWnd::OnClickCloseButton()
{
	g_pControl->PopupWindow(MSG_CTRL_SHORTCUT_WND,OPER_CLOSE);
	CloseWindow();
}

void CSkillWnd::Draw()
{
	//CCtrlWindowX::Draw();	

	// 绘制背景
	DWORD dwID = 0;
	//if(m_iCurPage < m_vTextures.size())
	if (m_iPageType == 0 || m_iPageType == 1)
	{
		dwID = m_vTexturesID[0];
	}
	else if (m_iPageType == 2)
	{
		dwID = MAKELONG(22500,PACKAGE_INTERFACE);
	}
	else if (m_iPageType == 3)
	{
		dwID = MAKELONG(23214,PACKAGE_INTERFACE);
	}

	LPTexture pTex = NULL;

	pTex = g_pTexMgr->GetTexImm(dwID,EP_UI);

	if (pTex)
	{
		//刚开始没有图,后来读到图了,重设大小及位置
		if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0)
		{
			ResetWidthAndHeight(pTex->GetWidth(),pTex->GetHeight());
		}

		if(m_uStyle & CTRL_STYLE_TRANS)
			DrawTexture(0,0,pTex,g_Config.GetTransColor());
		else
			DrawTexture(0,0,pTex);

	}
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}


	pTex = NULL;
	if (m_iPageType == 0)
	{
		string strCareer[3] = {"战士技能","魔法师技能","道士技能"};
		g_pFont->DrawText(m_iScreenX + 290,m_iScreenY + 10,strCareer[SELF.GetCareer()].c_str(),COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG,DTF_Center);
		pTex = g_pTexMgr->GetTexImm(PACKAGE_INTERFACE,23100 + SELF.GetCareer() * 20 + m_pLeftScroll->GetRange() - m_pLeftScroll->GetPos(),EP_UI);
	}
	else if (m_iPageType == 1)
	{
		g_pFont->DrawText(m_iScreenX + 290,m_iScreenY + 10,"生活技能",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG,DTF_Center);
		pTex = g_pTexMgr->GetTexImm(PACKAGE_INTERFACE,17440 + m_pLeftScroll->GetRange() - m_pLeftScroll->GetPos(),EP_UI);
	}
	else if (m_iPageType == 2)
	{
		g_pFont->DrawText(m_iScreenX + 290,m_iScreenY + 10,"连续技能",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG,DTF_Center);		
	}
	else if (m_iPageType == 3)
	{
		g_pFont->DrawText(m_iScreenX + 290,m_iScreenY + 10,"骑术技能",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG,DTF_Center);		
	}

	if (pTex)
	{
		g_pGfx->DrawTextureNL(m_iScreenX + 65,m_iScreenY + 45,pTex);
	}

	if (m_iPageType == 0 || m_iPageType == 1)
	{
		//画左边部分技能暗图标
		for(int i=0;i<m_iTotalSkillCount;i++)
		{
			if(NULL == m_pLeftSkillButton[i])
				continue;

			if(m_LeftSkill[i].iPageNumber != m_pLeftScroll->GetRange() - m_pLeftScroll->GetPos())
				continue;

			if (m_iPageType == 0)
			{
				if (m_LeftSkill[i].dwMagicID >= MAGICID_SHADOWKILL8 && m_LeftSkill[i].dwMagicID <= MAGICID_DAOZUN_JIANGLIN)
				{
					pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE, GetTexIDByMagicID((WORD)(m_LeftSkill[i].dwMagicID), 1),EP_UI);
					g_pGfx->DrawTextureNL(m_iScreenX + m_pLeftSkillButton[i]->GetX() + 1, m_iScreenY + m_pLeftSkillButton[i]->GetY() + 1,pTex);
				}
			}
		}

		//右边栏按钮底框
		for(int i = 0; i < SKILL_NUM_PAGE ; i ++ )
		{
			int iPos = i + m_pRightScroll->GetPos() * SKILL_NUM_PAGE;			

			if (iPos >= 0 && iPos < m_iRightSkillIndexs.size())
			{
				CMagicData *pMagicData = NULL;

				if (m_iPageType == 0)
				{
					pMagicData = &(SELF.GetMagic(m_iRightSkillIndexs[iPos]));
				}
				else
				{
					pMagicData = &(SELF.GetProduceMagic(m_iRightSkillIndexs[iPos]));
				}

				if(pMagicData->GetMagicID() ==0/* && (i + m_pRightScroll->GetPos() * SKILL_NUM_PAGE != SELF.MagicArray().GetMagicNumber())*/)//增加法诀技能
				{
					continue;
				}

				pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,78,EP_UI);
				if (pTex)
				{
					g_pGfx->DrawTextureNL(m_pRightSkillButton[i]->GetScreenX() - 3,m_pRightSkillButton[i]->GetScreenY() - 3,pTex);
				}
			}
		}
	}
	else if (m_iPageType == 2)
	{
		for (int iline = 0; iline < M_CONSKILL_NUM_PAGE; ++iline)
		{
			for (int submagic = 0; submagic < M_MAXCONSKILLSUBNUM; ++submagic)
			{
				int lefti = iline * M_MAXCONSKILLSUBNUM + submagic;

				int mid = m_LeftSkill[lefti].dwMagicID;
				if (mid != 0)
				{
					pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22502,EP_UI);
					g_pGfx->DrawTextureNL(m_iScreenX + m_pLeftSkillButton[lefti]->GetX() - 5,m_iScreenY + m_pLeftSkillButton[lefti]->GetY() - 5,pTex);
				}				
			}
		}
	}


	// 绘制所有子控件
	CControlContainer::Draw();


	if (m_iPageType == 0 || m_iPageType == 1)
	{
		//右边栏按钮
		for(int i = 0; i < SKILL_NUM_PAGE ; i ++ )
		{
			int iRightPage = m_pRightScroll->GetPos();
			int iSPos = i + iRightPage * SKILL_NUM_PAGE;
			if (iSPos < 0 || iSPos >= m_iRightSkillIndexs.size())
			{
				m_pRightSkillButton[i]->SetEnable(false);
				m_pRightSkillButton[i]->SetShow(false);
				m_pAddExpButton[i]->SetEnable(false);
				m_pAddExpButton[i]->SetShow(false);
				continue;
			}

			m_pRightSkillButton[i]->SetEnable(true);
			m_pRightSkillButton[i]->SetShow(true);
			m_pAddExpButton[i]->SetEnable(true);
			m_pAddExpButton[i]->SetShow(true);
			
			int iPos = m_iRightSkillIndexs[iSPos];

			CMagicData *pMagicData = NULL;
			
			if (m_iPageType == 0)
			{
				pMagicData = &(SELF.GetMagic(iPos));
			}
			else
			{
				pMagicData = &(SELF.GetProduceMagic(iPos));
			}

			if(pMagicData->GetMagicID() ==0)
			{
				m_pRightSkillButton[i]->SetEnable(false);
				m_pRightSkillButton[i]->SetShow(false);
				m_pAddExpButton[i]->SetEnable(false);
				m_pAddExpButton[i]->SetShow(false);
				continue;
			}		

			if (pMagicData->GetMagicID() == MAGICID_LIANJI)//法诀技能没有经验
			{
				m_pAddExpButton[i]->SetEnable(false);
				m_pAddExpButton[i]->SetShow(false);
			}
			//else//法诀技能
			//{
			//	m_pAddExpButton[i]->SetEnable(false);
			//	m_pAddExpButton[i]->SetShow(false);

			//	g_pFont->DrawText(m_iScreenX + 319 + 50,m_iScreenY + m_pRightSkillButton[i]->GetY(),"法诀技能",0xff945121,FONT_YAHEI);
			//	
			//	g_pFont->DrawText(m_iScreenX + 424,m_iScreenY + m_pRightSkillButton[i]->GetY()+ 13,"-",0xffffff00,FONT_YAHEI);

			//	BYTE shortcut = SELF.GetYIHUOINFO().byShortCut;

			//	char strShortCut[32] ={0};

			//	if(shortcut != 0)
			//		shortcut-=0x30;	

			//	if (shortcut > 0 && shortcut < 9)
			//	{
			//		sprintf(strShortCut,"F%d",shortcut);
			//		g_pFont->DrawText(m_iScreenX + 445 ,m_iScreenY + m_pRightSkillButton[i]->GetY() + 2,strShortCut,0xffffffff,FONT_YAHEI);
			//	}

			//	//空经验槽
			//	pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17432,EP_UI);
			//	if (pTex)
			//	{
			//		g_pGfx->DrawTextureNL(m_iScreenX + 400,m_iScreenY + m_pRightSkillButton[i]->GetY() + 27,pTex);
			//	}

			//	if (SELF.GetEquipGood(ITEM_POS_FABAORIGHT).GetID() != 0 
			//		&& SELF.GetEquipGood(ITEM_POS_FABAOLEFT).GetID() != 0 
			//		&& SELF.GetEquipGood(ITEM_POS_FABAO).GetID() != 0)
			//	{
			//		m_pRightSkillButton[i]->ReloadTex(22967,22967,22967,22967);
			//	}
			//	else
			//	{
			//		m_pRightSkillButton[i]->ReloadTex(22968,22968,22968,22968);
			//	}
			//	continue;
			//}
			
			int mid = pMagicData->GetMagicID();
			int key = pMagicData->GetShortCutKey();

			if(key != 0)
				key-=0x30;	

			const char *level = NULL;
			char skillexp[32] ={0};
			char name[32]={0};
			sprintf(name,"%s",pMagicData->GetMagicName());

			int magicLevel = 0;
			if(pMagicData->GetMagicID() == MAGICID_ZHENYUAN_ATTACK)
			{
				int iPhase = SELF.GetCharPhase();

				if(iPhase < 2)			//出内丹之前：初级
				{
					magicLevel = 0;
				}
				else if(iPhase == 2)		//出了内丹：中级
				{
					magicLevel = 1;
				}
				else if(iPhase == 3)		//出了元婴：高级
				{
					magicLevel = 2;
				}
				else 		//出了元神：专家
				{
					magicLevel = 3;
				}
			}
			else
			{
				magicLevel = pMagicData->GetMagicLevel();
			}
			magicLevel %= 8;
			level = m_str[magicLevel].c_str();
			bool bSuperMagic = g_AIMgr.IsSuperMagic(pMagicData->GetMagicID()) ;
			if (bSuperMagic)
			{
				magicLevel %= 6;
				level = m_strSuper[magicLevel].c_str();
			}

			float percent = 0.0f;
			if(pMagicData->GetTrain(magicLevel) > 0)
				percent = (float)(100.0 * pMagicData->GetSkillValue()/pMagicData->GetTrain(magicLevel));

			if(percent>100)
				percent = 100;
			if(percent<0)
				percent =0;

			sprintf(skillexp,"%2.2f%%",percent);

			if(magicLevel== 7 || (m_iPageType == 0 && magicLevel == 3 && !bSuperMagic) || (magicLevel == 5 && bSuperMagic) || (pMagicData->GetMagicID() == MAGICID_LIANJI))
			{
				sprintf(skillexp," -");
				percent = 0;
			}

			m_pRightSkillButton[i]->ReloadTex(GetTexIDByMagicID(mid,0),0,GetTexIDByMagicID(mid,0),GetTexIDByMagicID(mid,1));

			if(mid <= 0 || mid > 150)
			{
				m_pRightSkillButton[i]->SetEnable(false);
				m_pRightSkillButton[i]->SetShow(false);
				m_pAddExpButton[i]->SetEnable(false);
				m_pAddExpButton[i]->SetShow(false);
			}

			if (pMagicData->IsDisable()) 
			{
				m_pRightSkillButton[i]->SetEnable(false);
				m_pAddExpButton[i]->SetEnable(false);

				if (mid >= MAGICID_SWT && mid <= MAGICID_JYL)
				{
					strcat(name,"(未穿齐套装)");
				}
			}


			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17432,EP_UI);
			if (pTex)
			{
				g_pGfx->DrawTextureNL(m_iScreenX + 400,m_iScreenY + m_pRightSkillButton[i]->GetY() + 27,pTex);
			}

			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17433,EP_UI);
			if (pTex)
			{
				int right = (int)(pTex->GetWidth0() * percent/100.0);
				g_pGfx->DrawPartTexture(m_iScreenX + 400,m_iScreenY + m_pRightSkillButton[i]->GetY() + 28,pTex,0,0,right,-1,0xFFFFFFFF);
			}

			g_pFont->DrawText(m_iScreenX + 319 + 50,m_iScreenY + m_pRightSkillButton[i]->GetY() + 2,name,0xff945121,FONT_YAHEI);
			if (mid != MAGICID_LIANJI)
				g_pFont->DrawText(m_iScreenX + 319 + 50,m_iScreenY + m_pRightSkillButton[i]->GetY() + 19,level,0xff7b5d39,FONT_YAHEI);			
			g_pFont->DrawText(m_iScreenX + 424,m_iScreenY + m_pRightSkillButton[i]->GetY()+ 13,skillexp,0xffffff00,FONT_YAHEI);


			if(m_iPageType == 0)
			{
				if (key > 0 && key < 9)
				{
					sprintf(skillexp,"F%d",key);
					g_pFont->DrawText(m_iScreenX + 445 ,m_iScreenY + m_pRightSkillButton[i]->GetY() + 2,skillexp,0xffffffff,FONT_YAHEI);
				}

				DWORD exkey = pMagicData->GetShortCutKeyEx();
				char str[64]={0};
				WORD wLowKey = LOWORD(exkey);
				if(HIWORD(exkey) == 1)
				{
					if(wLowKey>=VK_F1 && wLowKey<=VK_F8)
					{
						sprintf(str,"Esc+F%d",wLowKey-VK_F1+1);
						g_pFont->DrawText(m_iScreenX + 460 ,m_iScreenY + m_pRightSkillButton[i]->GetY() + 2,str,0xff00ff00,FONT_YAHEI);
					}
				}
				else
				{
					if(wLowKey>='A' && wLowKey <='Z')
					{
						sprintf(str,"%c",wLowKey);
						g_pFont->DrawText(m_iScreenX + 460 ,m_iScreenY + m_pRightSkillButton[i]->GetY() + 2,str,0xff00ff00,FONT_YAHEI);
					}
				}		
			}
		}

		//左边按钮和绘制等级
		for(int i=0;i<m_iTotalSkillCount;i++)
		{
			if(m_LeftSkill[i].iPageNumber != m_pLeftScroll->GetRange() - m_pLeftScroll->GetPos())
				continue;

			if(NULL == m_pLeftSkillButton[i])
				continue;

			int iMaxMagic = MAX_MAGIC_SKILL;
			if (m_iPageType == 1)
			{
				iMaxMagic = MAX_PRODUCEMAGIC_SKILL;
			}			

			for(int iMagic = 0;iMagic < iMaxMagic;iMagic++)
			{
				DWORD mid = 0;
				
				if (m_iPageType == 0)
				{
					mid = SELF.GetMagic(iMagic).GetMagicID();
				}
				else if (m_iPageType == 1)
				{
					mid = SELF.GetProduceMagic(iMagic).GetMagicID();
				}

				if(mid<=0) 
				{					
					continue;
				}

				if( mid == m_LeftSkill[i].dwMagicID)
				{
					if ((mid != MAGICID_LIANJI))
					{
						char level[2] = {0};
						if (m_iPageType == 0)
						{
							level[0] = SELF.GetMagic(iMagic).GetMagicLevel() + 0x30;
						}
						else if (m_iPageType == 1)
						{
							level[0] = SELF.GetProduceMagic(iMagic).GetMagicLevel() + 0x30;
						}

						g_pFont->DrawText(m_iScreenX + m_pLeftSkillButton[i]->GetX() + 29 + 5,
							m_iScreenY + m_pLeftSkillButton[i]->GetY() + 29 +2,level,0xffffffff);
					}					
					break;
				}
			}
		}
	}
	else if (m_iPageType == 2)
	{
		for (int iline = 0; iline < M_CONSKILL_NUM_PAGE; ++iline)
		{
			for (int submagic = 0; submagic < M_MAXCONSKILLSUBNUM; ++submagic)
			{
				int lefti = iline * M_MAXCONSKILLSUBNUM + submagic;

					// 技能图标
					int mid = m_LeftSkill[lefti].dwMagicID;
					if (!m_LeftSkill[lefti].bActive && mid != 0)
					{
						pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE, GetTexIDByMagicID(mid),EP_UI);
						g_pGfx->DrawTextureNL(m_iScreenX + m_pLeftSkillButton[lefti]->GetX(),m_iScreenY + m_pLeftSkillButton[lefti]->GetY(),pTex,0x80FFFFFF);
					}

				// 箭头
				if (mid != 0 && submagic + 1 < M_MAXCONSKILLSUBNUM)
				{
					int nextlefti = iline * M_MAXCONSKILLSUBNUM + submagic + 1;
					if (m_LeftSkill[nextlefti].dwMagicID != 0)
					{
						pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE, 17674,EP_UI);
						g_pGfx->DrawTextureNL(m_iScreenX + m_pLeftSkillButton[lefti]->GetX() + 44,m_iScreenY + m_pLeftSkillButton[lefti]->GetY() + 4,pTex);
					}
				}
			}
		}

		g_pFont->DrawText(m_iScreenX + 80,m_iScreenY + 250,"    连续技能介绍：连续技能是当单一技能领悟到一定层次之后，顿悟了技能之",0xFFB48C5A,FONT_YAHEI,FONTSIZE_DEFAULT);
		g_pFont->DrawText(m_iScreenX + 80,m_iScreenY + 265,"间的相同性，当按照顺序施法指定技能，获得额外的强大附加效果，达到事半功",0xFFB48C5A,FONT_YAHEI,FONTSIZE_DEFAULT);
		g_pFont->DrawText(m_iScreenX + 80,m_iScreenY + 280,"倍的目的。",0xFFB48C5A,FONT_YAHEI,FONTSIZE_DEFAULT);
		g_pFont->DrawText(m_iScreenX + 80,m_iScreenY + 295,"    技能要求：只有领悟了连续技能中每个技能的大师级之后，才可以触发连续",0xFFB48C5A,FONT_YAHEI,FONTSIZE_DEFAULT);
		g_pFont->DrawText(m_iScreenX + 80,m_iScreenY + 310,"技能效果，同时随着技能等级越高，效果越强，可杀人于瞬间，极大增强了威慑",0xFFB48C5A,FONT_YAHEI,FONTSIZE_DEFAULT);
		g_pFont->DrawText(m_iScreenX + 80,m_iScreenY + 325,"力。",0xFFB48C5A,FONT_YAHEI,FONTSIZE_DEFAULT);

	}
	else if (m_iPageType == 3)
	{
		if (m_pMarkViewer)
			m_pMarkViewer->Draw();

		g_pFont->DrawText(m_iScreenX+ 57,m_iScreenY + 53,"骑乘状态: ",COLOR_TEXT_SBU_TITLE,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_BlackFrame);

		if (SELF.IsOnLepoard())
		{
			g_pFont->DrawText(m_iScreenX+ 125,m_iScreenY + 53,"骑乘",0xff00ff00,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_BlackFrame);
			g_pFont->DrawText(m_iScreenX+ 57,m_iScreenY + 53+21,"骑术与骑兽的属性有效",0xff00ff00,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
		}
		else
		{
			g_pFont->DrawText(m_iScreenX+ 125,m_iScreenY + 53,"非骑乘",0xffff0000,FONT_YAHEI,FONTSIZE_MIDDLE,DTF_BlackFrame);
			g_pFont->DrawText(m_iScreenX+ 57,m_iScreenY + 53+21,"骑术与骑兽的属性无效",0xffff0000,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);

		}

		HorsemanshipProtocalInfo& HorseManShipInfo = g_OtherData.GetHorseManShipInfo();
		std::vector<HorsemanshipProtocalSkill>& vecSkill = HorseManShipInfo.vecSkill;

		char str[32] = {0};
		int si = 0;
		int skilltexid = 0;
		for (int j = 0; j < 2; ++j)
		{
			for (int i = 0; i < 5; ++i)
			{
				if (si < m_iTotalSkillCount)
				{
					skilltexid = GetTexIDByHouseMagicID((WORD)(m_LeftSkill[si].dwMagicID), m_LeftSkill[si].bActive);
					pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,skilltexid,EP_UI);
					g_pGfx->DrawTextureNL(m_iScreenX + 102 + 86*i,m_iScreenY + 257 + 62*j,pTex, 0xFF707070);

					for (int ii = 0; ii < vecSkill.size(); ++ii)
					{
						HorsemanshipProtocalSkill& skill = vecSkill[ii];
						if (skill.dwSkillID == m_LeftSkill[si].dwMagicID)
						{
							skilltexid = GetTexIDByHouseMagicID((WORD)(m_LeftSkill[si].dwMagicID), m_LeftSkill[si].bActive);
							pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,skilltexid,EP_UI);
							g_pGfx->DrawTextureNL(m_iScreenX + 102 + 86*i,m_iScreenY + 257 + 62*j,pTex);

							sprintf(str, "%d", skill.byLevel);
							g_pFont->DrawText(m_iScreenX + 102 + 86*i + 29 + 6,	m_iScreenY + 257 + 62*j + 29 + 4,str,0xffffffff);
							break;
						}
					}
				}				

				++si;
			}
		}
	}


}

void CSkillWnd::RefreshLeftSkillButtonState()
{
	if (m_iPageType == 0 || m_iPageType == 1)
	{	
		for(int i = 0;i<m_iTotalSkillCount;i++)
		{
			if(NULL == m_pLeftSkillButton[i]) continue;

			if(m_LeftSkill[i].iPageNumber != m_pLeftScroll->GetRange() - m_pLeftScroll->GetPos())
			{
				m_pLeftSkillButton[i]->SetShow(false);
				m_pLeftSkillButton[i]->SetEnable(false);
				continue;
			}

			CMagicData *pMagic = SELF.FindMagic(WORD(m_LeftSkill[i].dwMagicID));
			if(pMagic)
			{
				m_pLeftSkillButton[i]->SetShow(true);
				m_pLeftSkillButton[i]->SetEnable(true);
				if (pMagic->GetFlashTexID() != 0)
				{
					m_pLeftSkillButton[i]->SetFlashTexID(pMagic->GetFlashTexID());
				}
			}
			else
			{
				m_pLeftSkillButton[i]->SetShow(false);
				m_pLeftSkillButton[i]->SetEnable(false);
			}
		}
	}
}


bool CSkillWnd::OnMouseMove(int iX, int iY)
{
	TRY_BEGIN
		CParserTip* pTip = g_pControl->GetTipWnd();
	if(!pTip)
		return true;

	pTip->Clear();
	pTip->SetShow(false);

	if (m_iPageType == 0 || m_iPageType == 1)
	{	
		if (iX>38 && iX<242 && iY> 60 && iY<376)
		{
			for(int i=0;i<m_iTotalSkillCount;i++)
			{
				if(m_LeftSkill[i].iPageNumber != m_pLeftScroll->GetRange() - m_pLeftScroll->GetPos())
					continue;

				if(NULL == m_pLeftSkillButton[i])
					continue;

				if(iX>m_pLeftSkillButton[i]->GetX() && iX<m_pLeftSkillButton[i]->GetX()+29 
					&& iY>m_pLeftSkillButton[i]->GetY() && iY<m_pLeftSkillButton[i]->GetY()+29)
				{
					pTip->AddText(m_LeftSkill[i].szName.c_str(),0xFFFFFF00);

					sTipsCfg * pTipCfg = g_TipsCfg.GetSkillTips(m_LeftSkill[i].szName.c_str());
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
			}
		}
	}
	else if (m_iPageType == 2)
	{
		if (iX>38 && iX<300 && iY> 60 && iY<376)
		{
			for(int i=0;i<m_iTotalSkillCount;i++)
			{
				if(NULL == m_pLeftSkillButton[i])
					continue;

				if (m_LeftSkill[i].dwMagicID == 0)
					continue;

				if (m_LeftSkill[i].szName.empty())
					continue;

				if(iX>m_pLeftSkillButton[i]->GetX() && iX<m_pLeftSkillButton[i]->GetX()+29 
					&& iY>m_pLeftSkillButton[i]->GetY() && iY<m_pLeftSkillButton[i]->GetY()+29)
				{
					pTip->AddText(m_LeftSkill[i].szName.c_str(),0xFFFFFF00);


					pTip->AddText(m_LeftSkill[i].szInfo.c_str(), 0xFFDEAA52);

					int x = m_iScreenX + iX + 10;
					int y = m_iScreenY + iY + 10;
					pTip->AdjustXY(x,y);
					pTip->Move(x,y);
					pTip->SetShow(true);
					return true;
				}
			}
		}
	}
	else if (m_iPageType == 3)
	{
		if (iX>103 && iX<476 && iY> 258 && iY<349)
		{
			int si = 0;
			for (int j = 0; j < 2; ++j)
			{
				for (int i = 0; i < 5; ++i)
				{
					if (si < m_iTotalSkillCount)
					{
						if (iX>101 + 86*i && iX<101 + 86*i+32 && iY> 256 + 62*j && iY<256 + 62*j+32)
						{
							pTip->AddText(m_LeftSkill[si].szName.c_str(),0xFFFFFF00);

							sTipsCfg * pTipCfg = g_TipsCfg.GetSkillTips(m_LeftSkill[si].szName.c_str());
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
					}

					++si;
				}
			}
		}
	}

	TRY_END

		return CCtrlWindowX::OnMouseMove(iX,iY);

}

void CSkillWnd::SetRightBtnTips(int i,int mid)
{
	if(i < 0 || i >= SKILL_NUM_PAGE)
		return;

	if (mid == MAGICID_SWT)
	{
		m_pRightSkillButton[i]->SetTips("只有身穿圣武天战甲(袍)手持怒狂降魔杵才能释放此技能\n单击设置该技能的快捷键");
	}
	else if (mid == MAGICID_LSJ)
	{
		m_pRightSkillButton[i]->SetTips("只有身穿雷神绝魔袍(衣)手持幻龙风雷鞭才能释放此技能\n单击设置该技能的快捷键");
	}
	else if (mid == MAGICID_JYL)
	{
		m_pRightSkillButton[i]->SetTips("只有身穿九幽灵道袍(衣)手持清心碧玉笛才能释放此技能\n单击设置该技能的快捷键");
	}
	else if (m_iPageType != 1)
	{
		m_pRightSkillButton[i]->SetTips("单击设置该技能的快捷键");
	}
}

void CSkillWnd::SetAllRightBtnTips()
{
	if (m_iPageType == 0 || m_iPageType == 1)
	{		
		//右边栏按钮tips
		int iRightPage = m_pRightScroll->GetPos();
		

		for(int i = 0; i < SKILL_NUM_PAGE; i ++ )
		{
			int iSPos = i + iRightPage * SKILL_NUM_PAGE;
			if (iSPos >= 0 && iSPos < m_iRightSkillIndexs.size())
			{
				int iPos = m_iRightSkillIndexs[iSPos];
				if (m_iPageType == 0)
				{
					SetRightBtnTips(i,SELF.GetMagic(iPos).GetMagicID());
				}
				else if (m_iPageType == 1)
				{
					SetRightBtnTips(i,SELF.GetProduceMagic(iPos).GetMagicID());
				}
			}
			else
			{
				SetRightBtnTips(i,0);
			}			
		}
	}
}

bool CSkillWnd::OnLeftButtonUp(int iX, int iY)
{
	//if (iX > 0 && iX < 35)
	//{
	//	if (iY > 0 && iY < 200)
	//	{
	//		SwitchToPage(0);
	//		return true;
	//	}
	//	else if (iY > 200 && iY < m_iHeight)
	//	{
	//		SwitchToPage(1);
	//		return true;
	//	}
	//}

	if (m_pMarkViewer)
	{
		string strCommand = m_pMarkViewer->GetCommand();

		if(!strCommand.empty())
		{
			//up 音乐
			g_pAudio->Play(EAT_OTHER,5,g_pAudio->GetRand()++);
			if(strCommand.substr(0,2) == "@@")
			{
				string str = g_AICfgMgr.GetQiShuHelpByName(strCommand);
				m_kHelpText.Clear();
				m_kHelpText.Parse(str,0,"\\",str.length());
				return true;
			}
		}
	}

	if (m_iPageType == 3)
	{
		if (iX>103 && iX<476 && iY> 258 && iY<349)
		{
			int si = 0;
			for (int j = 0; j < 2; ++j)
			{
				for (int i = 0; i < 5; ++i)
				{
					if (si < m_iTotalSkillCount)
					{
						// 主动技能
						if (m_LeftSkill[si].bActive)
						{
							if (iX>101 + 86*i && iX<101 + 86*i+32 && iY> 256 + 62*j && iY<256 + 62*j+32)
							{
								for(int iPos = 0; iPos < MAX_MAGIC_SKILL; iPos++)
								{
									DWORD dwMagicID = SELF.GetMagic(iPos).GetMagicID();

									if(dwMagicID == m_LeftSkill[si].dwMagicID)
									{
										g_pAudio->Play(EAT_OTHER,10,g_pAudio->GetRand()++);
										g_AIMgr.SetShortCutKeyMagicPos(iPos);
										g_pControl->PopupWindow(MSG_CTRL_SHORTCUT_WND,OPER_CREATE,1);
										return true;
									}
								}
							}
						}
					}

					++si;
				}
			}

			g_pControl->PopupWindow(MSG_CTRL_SHORTCUT_WND,OPER_CLOSE);
		}
	}

	return CCtrlWindowX::OnLeftButtonUp(iX,iY);
}

void CSkillWnd::SwitchToPage(int iPage, bool force)
{
	if (iPage < 0 || iPage > 3)
		return;

	if (iPage == m_iPageType && !force)
		return;

	m_iPageType = iPage;
	RemoveBodyChildControl();	

	for(int i = 0; i < BUTTONCOUNT; i++)
	{
		m_pLeftSkillButton[i] = NULL;
		m_pRightSkillButton[i] = NULL;
		m_pAddExpButton[i] = NULL;
	}

	OnCreate();
}

void CSkillWnd::SetRightPageNum()
{

}

bool CSkillWnd::OnWheel(int iWheel)
{
	int iMouseX, iMouseY;
	g_pControl->GetMouseXY(iMouseX, iMouseY);

	if (iMouseX < m_iScreenX + 290)
	{
		if (m_pLeftScroll)	
			return m_pLeftScroll->OnWheel(iWheel);
	}
	else
	{
		if (m_pRightScroll)		
			return m_pRightScroll->OnWheel(iWheel);
	}
	return CCtrlWindowX::OnWheel(iWheel);
}

void CSkillWnd::UpdateConSkillState()
{
	if (m_iPageType == 2)
	{	
		for (int i=0;i<M_CONSKILL_NUM_PAGE;i++)
		{
			m_iRightConSkill[i] = 0;
		}

		char skillkey[32] ={0};
		int iline = 0;
		ConSkillMap& kConSkillMap = g_AIMgr.GetConSkills();
		ConSkillMap::iterator it = kConSkillMap.begin();

		for (; it != kConSkillMap.end() && iline < M_CONSKILL_NUM_PAGE; ++it)
		{
			ConSkillData* pConSkillData = it->second;
			if (pConSkillData && pConSkillData->nCareer == SELF.GetCareer())
			{
				bool bConSkillActive = true;
				int submagic = 0;
				for (; submagic < pConSkillData->nMagicCount; ++submagic)
				{
					int lefti = iline * M_MAXCONSKILLSUBNUM + submagic;

					ConSubSkill& kConSubSkill = pConSkillData->wMagicIDs[submagic];

					int mid = kConSubSkill.wMagicID;

					CMagicData *pMagic = SELF.FindMagic(mid);
					if (pMagic && pMagic->GetMagicLevel() >= 4)
					{
						m_pRightSkillButton[lefti]->ReloadTex(GetTexIDByMagicID(mid,0),0,GetTexIDByMagicID(mid,0),GetTexIDByMagicID(mid,1));

						m_pLeftSkillButton[lefti]->SetShow(true);
						m_pLeftSkillButton[lefti]->SetEnable(true);

						m_LeftSkill[lefti].szName = pMagic->GetMagicName();
						m_LeftSkill[lefti].szInfo = kConSubSkill.nInfo;
						m_LeftSkill[lefti].bActive = true;
					}
					else
					{
						m_pLeftSkillButton[lefti]->SetShow(false);
						m_pLeftSkillButton[lefti]->SetEnable(false);

						m_LeftSkill[lefti].szName = kConSubSkill.nName;
						m_LeftSkill[lefti].szInfo = kConSubSkill.nInfo;
						m_LeftSkill[lefti].bActive = false;

						bConSkillActive = false;
					}

					m_LeftSkill[lefti].dwMagicID = mid;
				}

				for (;submagic < M_MAXCONSKILLSUBNUM;++submagic)
				{
					int lefti = iline * M_MAXCONSKILLSUBNUM + submagic;
					m_LeftSkill[lefti].bActive = 0;
					m_LeftSkill[lefti].dwMagicID = 0;
					m_LeftSkill[lefti].szName = "";
					m_LeftSkill[lefti].szInfo = "";
				}

				if (pConSkillData->dwKey == 0 || !bConSkillActive)
				{
					m_pRightSkillButton[iline]->SetText("设置快捷键", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);
				}
				else
				{
					WORD wHighKey = HIWORD(pConSkillData->dwKey);
					WORD wLowKey = LOWORD(pConSkillData->dwKey);

					if (wHighKey == 0)
					{
						sprintf(skillkey,"F%d",wLowKey - VK_F1 + 1);
						m_pRightSkillButton[iline]->SetText(skillkey, COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);
					}
					else if (wHighKey == 1)
					{
						sprintf(skillkey,"Esc+F%d",wLowKey-VK_F1+1);
						m_pRightSkillButton[iline]->SetText(skillkey, COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);

					}
				}

				m_iRightConSkill[iline] = pConSkillData->iConSkillID;

				m_pRightSkillButton[iline]->SetShow(true);
				m_pRightSkillButton[iline]->SetEnable(bConSkillActive);

				++iline;

			}
		}

		for (; iline < M_CONSKILL_NUM_PAGE; ++iline)
		{
			for (int submagic = 0; submagic < M_MAXCONSKILLSUBNUM; ++submagic)
			{
				int lefti = iline * M_MAXCONSKILLSUBNUM + submagic;
				m_pLeftSkillButton[lefti]->SetShow(false);
				m_pLeftSkillButton[lefti]->SetEnable(false);

				m_LeftSkill[lefti].bActive = false;
				m_LeftSkill[lefti].dwMagicID = 0;
				m_LeftSkill[lefti].szName = "";
				m_LeftSkill[lefti].szInfo = "";
			}

			m_pRightSkillButton[iline]->SetShow(false);
			m_pRightSkillButton[iline]->SetEnable(false);
		}
	}
}

void CSkillWnd::UpdateSkill()
{
	if (m_iPageType == 0 || m_iPageType == 1)
	{	
		// 更新右边技能列表
		m_iRightSkillIndexs.clear();

		if (m_iPageType == 0)
		{
			for(int iPos = 0; iPos < MAX_MAGIC_SKILL; iPos++)
			{
				DWORD dwMagicID = SELF.GetMagic(iPos).GetMagicID();				
				if (dwMagicID <= 0 || (dwMagicID >= MAGICID_AOFENGCUT_ATTACK && dwMagicID <= MAGICID_DULINGBO_ATTACK))
					continue;

				m_iRightSkillIndexs.push_back(iPos);		
			}
		}
		else if (m_iPageType == 1)
		{
			for(int iPos = 0; iPos < MAX_PRODUCEMAGIC_SKILL; iPos++)
			{
				DWORD dwMagicID = SELF.GetProduceMagic(iPos).GetMagicID();				
				if (dwMagicID <= 0)
					continue;

				m_iRightSkillIndexs.push_back(iPos);		
			}
		}
		

		for(int i = 0; i < SKILL_NUM_PAGE; i++)
		{
			int curstarti = i + m_pRightScroll->GetPos() * SKILL_NUM_PAGE;
			if (curstarti >= 0 && curstarti < m_iRightSkillIndexs.size())
			{
				int mid = SELF.GetMagic(m_iRightSkillIndexs[curstarti]).GetMagicID();
				if (m_iPageType == 1)
					mid = SELF.GetProduceMagic(m_iRightSkillIndexs[curstarti]).GetMagicID();

				m_pRightSkillButton[i]->SetEnable(true);
				m_pRightSkillButton[i]->SetShow(true);
				m_pAddExpButton[i]->SetEnable(true);
				m_pAddExpButton[i]->SetShow(true);
				
				if (mid == MAGICID_LIANJI)
				{
					m_pAddExpButton[i]->SetEnable(false);
					m_pAddExpButton[i]->SetShow(false);
				}
				SetRightBtnTips(i,mid);
				m_pAddExpButton[i]->SetTips("点击后将消耗包裹中的强化技能灵丹,并增加该技能经验值");
			}
			else
			{
				m_pRightSkillButton[i]->SetEnable(false);
				m_pRightSkillButton[i]->SetShow(false);
				m_pAddExpButton[i]->SetEnable(false);
				m_pAddExpButton[i]->SetShow(false);
			}		
		}

		m_pRightScroll->SetRange((m_iRightSkillIndexs.size()-1) / SKILL_NUM_PAGE);
	}
}