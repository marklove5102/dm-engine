#include "rangewnd.h"
#include "GameControl/GameControl.h"
#include "Global/Interface/AudioInterface.h"
#include "GameData/GameData.h"
#include "BaseClass/Control/ParserTip.h"
#include "GameData/RangeXmlMgr.h"
#include "GameData/GameGlobal.h"
#include "GameClient/WidgetManager.h"
#include "GameAI/AIGoodMgr.h"

#define	 MYCHAR_LEN	14
#define	 GUILDNAME_LEN	30

INIT_WND_POSX(CRangeWnd,POS_AUTO,POS_AUTO)

char * szSkillLevel[8] = {"初级","中级","高级","专家","大师","宗师","权威","王者"};

//1.	等级排名
//2.	名师排名
//3.	元神排名
//4.	宠物豹
//5.	锻造武器
//6.	行会排行
//7.	灭魔排行
//8、 英雄擂台排名
//9、 恶魔城排名
//10、武馆排名
//11、世界英雄
//12、世界尊主
//13. 宗族排行
//16  真火榜
//17  神翼排行榜
//18  骑术排行榜
//RT_KFZ,历届跨服战排名,内嵌IE
CRangeWnd::CRangeWnd(void)
{
	m_iIndex = 10712;
	//m_iMask  = 10712;

	m_ColumnText[1] = "名次          名字           等级      职业     性别          行会          当前经验 ";
	m_ColumnText[2] = "名次          名字           出师徒弟数          职业          性别           等级";
	m_ColumnText[3] = "名次;名字;等级;职业;性别;元神体魄;所属主体;";
	m_ColumnText[4] = "名次;宠物豹;等级;经验;封号;主人;";
	m_ColumnText[5] = "名次;名字;铸造经验;等级;职业;性别;";
	m_ColumnText[6] = "名次      行会名                  行会宝塔等级  46级以上人数   行会经验值     人数";
	m_ColumnText[7] = "名次;名字;性别;等级;职业;所属行会;击杀次数;";
	m_ColumnText[8] = "名次;名字;等级;职业;参赛场次;比赛积分;获得奖金;";
	m_ColumnText[9] = "名次;名字;职业;所用时间;";
	m_ColumnText[10] = "名次;名字;职业;积分;称号;";
	m_ColumnText[13] = "名次            宗派           宗派弟子         争夺次数";
	m_ColumnText[14] = "名次           名字           技能等级          职业             行会       当前经验";
	m_ColumnText[15] = "名次           名字           等级      职业     性别         行会          获胜次数";
	m_ColumnText[16] = "名次       名字           真火境界      职业    性别       行会      当前真火值";
	m_ColumnText[17] = "名次        名字            神翼等级         职业        性别          行会";
	m_ColumnText[18] = "名次       名字           骑术等级      职业    性别       行会      骑术经验";

	m_BottomText[1] = "您当前等级    级        排名第     名       最近排名";
	m_BottomText[2] = "您带出    个徒弟          排名第    名      最近排名";
	m_BottomText[3] = "名次;名字;等级;职业;性别;元神体魄;所属主体;";
	m_BottomText[4] = "名次;宠物豹;等级;经验;封号;主人;";
	m_BottomText[5] = "名次;名字;铸造经验;等级;职业;性别;";
	m_BottomText[6] = "您的行会为                            排名第     名               最近排名";
	m_BottomText[7] = "名次;名字;性别;等级;职业;所属行会;击杀次数;";
	m_BottomText[8] = "名次;名字;等级;职业;参赛场次;比赛积分;获得奖金;";
	m_BottomText[9] = "名次;名字;职业;所用时间;";
	m_BottomText[10] = "名次;名字;职业;积分;称号;";
	m_BottomText[14] = "您当前技能等级     级        排名第      名      最近排名";
	m_BottomText[15] = "您当前获胜次数：             排名第      名      最近排名";
	m_BottomText[16] = "您当前真火等级：                排名第      名      最近排名";
	m_BottomText[17] = "您当前神翼等级：                排名第      名      最近排名";
	m_BottomText[18] = "您当前骑术等级：                排名第      名      最近排名";

	/*for(int i=1;i<11;i++)
	{
		m_ColumnText[i] = g_pTexMgr->LoadTex(PACKAGE_INTERFACE,10769+i);
	}
	for(int i=1;i<11;i++)
	{
		m_BottomText[i] = g_pTexMgr->LoadTex(PACKAGE_INTERFACE,10785+i);
	}*/
	//m_pPet2BottomText = g_pTexMgr->LoadTex(PACKAGE_INTERFACE,10768);
	//m_pNoneBottomText = g_pTexMgr->LoadTex(PACKAGE_INTERFACE,10769);
	//m_pSBKText = g_pTexMgr->LoadTex(PACKAGE_INTERFACE,10812);

	//m_pDragListTex = g_pTexMgr->LoadTex(PACKAGE_INTERFACE,10817);
	//m_pScrollTex = g_pTexMgr->LoadTex(PACKAGE_INTERFACE,10818);
	//m_pSmallPaneTex = g_pTexMgr->LoadTex(PACKAGE_INTERFACE,10819);
	//m_pSmallPaneTex2= g_pTexMgr->LoadTex(PACKAGE_INTERFACE,10820);

	//m_pLeftLight = g_pTexMgr->LoadTex(PACKAGE_INTERFACE,10811);
	//m_pRightLight = g_pTexMgr->LoadTex(PACKAGE_INTERFACE,10810);
	//m_pPingWen = g_pTexMgr->LoadTex(PACKAGE_INTERFACE,10821);
	//m_pUp = g_pTexMgr->LoadTex(PACKAGE_INTERFACE,10822);
	//m_pDown = g_pTexMgr->LoadTex(PACKAGE_INTERFACE,10823);
	//m_pZiTeXiao = g_pTexMgr->LoadTex(PACKAGE_INTERFACE,10818);
	//m_pHelpLight = g_pTexMgr->LoadTex(PACKAGE_INTERFACE,10825);
	//m_pHelpTex = g_pTexMgr->LoadTex(PACKAGE_INTERFACE,10826);
	//m_pGradeText = g_pTexMgr->LoadTex(PACKAGE_INTERFACE,10834);

	m_ColumnWidth[1] = "70;102;55;60;50;130;55;";
	m_ColumnWidth[2] = "65;130;84;100;85;50;";
	m_ColumnWidth[3] = "50;104;40;50;40;55;94;";
	m_ColumnWidth[4] = "50;104;78;46;70;94;";
	m_ColumnWidth[5] = "60;134;50;52;58;55;";
	m_ColumnWidth[6] = "50;180;80;70;90;50;";
	m_ColumnWidth[7] = "50;102;40;40;40;94;55;";
	m_ColumnWidth[8] = "50;102;46;46;65;65;65;";
	m_ColumnWidth[9] = "60;124;65;55;";
	m_ColumnWidth[10] = "58;124;65;65;65;";
	m_ColumnWidth[13] = "65;135;100;100;";
	m_ColumnWidth[14] = "75;110;97;70;115;50;";
	m_ColumnWidth[15] = "75;103;50;64;55;110;100;";
	m_ColumnWidth[16] = "40;90;103;53;35;105;75;100";
	m_ColumnWidth[17] = "40;112;110;75;75;105;75";
	m_ColumnWidth[18] = "40;112;81;53;35;105;75;100";

	m_MerTiPo[0] = "普通";
	m_MerTiPo[1] = "上品";
	m_MerTiPo[2] = "极品";

	m_dwTickCount = 0;

	m_nowGeLevel = 0;
	m_oldPos =0;
	m_nMyPlace = 0;

	//m_pHelpPaneButton = NULL;

	m_bNoMove = true;

	m_bLeftLight = false;
	m_bRightLight = false;
	m_bRightRoll = false;
	m_bLeftRoll = false;
	m_bHelpLight = false;

	m_iFrameCount = 0;
	m_CurrentType = 0;
	m_CurrentSmallType = 0;
	m_wNow=2;
	m_iLine = 10;
	m_nWaitReading = 0;

	m_strColumnWidth = "";

	//m_pSelectTabButton = NULL;

	//m_pLeftTex = NULL;
	//m_pMidTex = NULL;
	//m_pRightTex = NULL;
	m_dwLeftTex = 0;
	m_dwMidTex = 0;
	m_dwRightTex = 0;

	m_pGrid = NULL;

	m_bMySend = true;
	m_bSend = true;
	m_bHelp = false;
	m_bSendUp = false;

	for(int i=0;i<3;i++)
	{
		m_pGunLunTex[i] = NULL;
	}

	m_pRangeText = NULL;

	if(g_RangeNum>0)
		m_pRangeText = new Range_Text[g_RangeNum];

	//初始化上一次位置
	if(m_old_range!=0)
	{
		m_CurrentType = m_old_range >> 8;
		m_CurrentSmallType = m_old_range & 0x00ff;
	}

	InitTexByType();

	SetTexByType();

	SendServerExpUp();

}

CRangeWnd::~CRangeWnd(void)
{
	ClearAllRangeData();

	//for(int i=0;(i<g_RangeNum) && m_pRangeText;i++)
	//{
	//	g_pTexMgr->ReleaseTex(m_pRangeText[i].pLeftTex);
	//	g_pTexMgr->ReleaseTex(m_pRangeText[i].pMidTex);
	//	g_pTexMgr->ReleaseTex(m_pRangeText[i].pRightTex);
	//}

	/*for(int i=1;i<11;i++)
	{
		g_pTexMgr->ReleaseTex(m_ColumnText[i]);
	}
	for(int i=1;i<11;i++)
	{
		g_pTexMgr->ReleaseTex(m_BottomText[i]);
	}*/
	//g_pTexMgr->ReleaseTex(m_pPet2BottomText);
	//g_pTexMgr->ReleaseTex(m_pNoneBottomText);
	//g_pTexMgr->ReleaseTex(m_pSBKText);

	if(g_RangeNum>0 && m_pRangeText)
		SAFE_DELETE_ARRAY(m_pRangeText);

	//g_pTexMgr->ReleaseTex(m_pDragListTex);
	//g_pTexMgr->ReleaseTex(m_pScrollTex);
	//g_pTexMgr->ReleaseTex(m_pSmallPaneTex);
	//g_pTexMgr->ReleaseTex(m_pSmallPaneTex2);
	//g_pTexMgr->ReleaseTex(m_pLeftLight);
	//g_pTexMgr->ReleaseTex(m_pRightLight);

	//g_pTexMgr->ReleaseTex(m_pPingWen);	//水平
	//g_pTexMgr->ReleaseTex(m_pUp);	//上升
	//g_pTexMgr->ReleaseTex(m_pDown);
	//g_pTexMgr->ReleaseTex(m_pZiTeXiao);
	//g_pTexMgr->ReleaseTex(m_pHelpLight);
	//g_pTexMgr->ReleaseTex(m_pHelpTex);

	//g_pTexMgr->ReleaseTex(m_pGradeText);

	m_old_range = (m_CurrentType<<8) + m_CurrentSmallType;

	g_WidgetMgr.SetShow(EWT_KFZ_IE,false);
	g_WidgetMgr.SetFocus(EWT_KFZ_IE,false);

}


void CRangeWnd::OnCreate()
{
	m_pHelpCloseButton = new CCtrlButton();
	AddControl(m_pHelpCloseButton);
	m_pHelpCloseButton->CreateEx(this,516,264,10827,10828,10829);
	m_pHelpCloseButton->SetShow(false);

	SetCloseButton(669,8,80);

	m_pRightButton = new CCtrlButton();
	AddControl(m_pRightButton);
	m_pRightButton->CreateEx(this,85,140,10706,10707,10708);

	m_pLeftButton = new CCtrlButton();
	AddControl(m_pLeftButton);
	m_pLeftButton->CreateEx(this,7,140,10709,10710,10711);

	/*m_pSelectTabButton = new CCtrlButton();
	AddControl(m_pSelectTabButton);
	m_pSelectTabButton->CreateEx(this,118,11,10713,0,0);
	m_pSelectTabButton->SetColor(0xffE3C800);
	m_pSelectTabButton->SetTextOff(0,4);
	m_pSelectTabButton->SetFont(-1,-1,DTF_Bold);*/
	//	m_pSelectTabButton->SetLockText(true);

	for(int i = 0;i < 6;i++)
	{
		m_pUnSelectTabButton[i] = new CCtrlButton();
		AddControl(m_pUnSelectTabButton[i]);
		m_pUnSelectTabButton[i]->Create(this,147 + 86 * i,11,0,66,22);
		m_pUnSelectTabButton[i]->SetColor(0xffaaaa33);
		m_pUnSelectTabButton[i]->SetTextOff(0,7);
		m_pUnSelectTabButton[i]->SetFont(-1,-1,DTF_Bold);
		//m_pUnSelectTabButton[i]->SetLockText(true);
	}

	//各等级
	m_pMBLevel = new CCtrlMenuButton();
	AddControl(m_pMBLevel);
	m_pMBLevel->Create(this,124,42,50,140);
	m_pMBLevel->SetMenuTextColor(0xFFFFFFFF);
	m_pMBLevel->SetShow(false);
	m_pMBLevel->SetFont(FONT_YAHEI);
	char temp_str[5]={0};
	for(int i = 0; i <= 15; i++)
	{
		sprintf(temp_str,"%d级",i + 35);
		m_pMBLevel->AddString(temp_str);
	}
	m_pMBLevel->SetCurSel(0);
	m_pMBLevel->SetText("35级",0xffE3C800,0xffE3C800,0xffE3C800,0xffE3C800,FONTSIZE_DEFAULT,0,FONT_YAHEI);



	
	m_iLine = 10;
	m_pGrid = new CCtrlGrid();
	AddControl(m_pGrid);
	m_pGrid->Create(this,144,72,682,266,m_iLine,19.5);
	m_pGrid->AddUpButton(517,10,10700,10701,10702);
	m_pGrid->AddDownButton(517,172,10703,10704,10705);
	m_pGrid->AddScroll(518,22,13,150,10715);
	m_pGrid->SetTextColor(0xFF000000,0xFFFF0000);
	m_pGrid->SetScrollWidth(25);
	//m_pGrid->SetLineNumType(ECT_PIC);
	m_pGrid->SetFont(FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
	

	m_pTipMyInfo.Create(this);
	m_pTipMyInfo.SetText("点击此处可以移动到自己的排名位置");
	m_pTipMyInfo.SetBackTexByID(0x50000000);
	m_pTipMyInfo.SetFrame(0,0);
	m_pTipMyInfo.SetShow(false);

	SetSmallType();
	Initcolumn();

	m_LastSendTick = GetTickCount();
	m_MyLastSendTick = m_LastSendTick;

}

void CRangeWnd::Draw()
{
	m_dwTickCount = GetTickCount();

	//先让子控件绘制出来再绘制自己，否则CCtrlGrid的选中效果框会把文字覆盖
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	if(m_pRangeText == NULL) return;
	if(m_bRightRoll)
	{
		m_iFrameCount++;
		if(m_iFrameCount%6 == 0)
		{
			m_wNow++;
			if(m_wNow>2)
				m_wNow = 0;
			else if(m_wNow == 2)
			{
				m_bRightRoll = false;
				m_iFrameCount = 0;
				m_CurrentType++;
				if(m_CurrentType>g_RangeNum-1)
					m_CurrentType = 0;

				m_CurrentSmallType=0;

				SetTexByType();
				SetSmallType();
				Initcolumn();
			}
		}
	}
	if(m_bLeftRoll)
	{
		m_iFrameCount++;
		if(m_iFrameCount%6 == 0)
		{
			m_wNow--;
			if(m_wNow<0)
			{
				m_wNow = 2;
				m_bLeftRoll = false;
				m_iFrameCount = 0;
				m_CurrentType--;
				if(m_CurrentType<0)
					m_CurrentType = g_RangeNum-1;

				m_CurrentSmallType = 0;

				SetTexByType();
				SetSmallType();
				Initcolumn();
			}

		}
	}

	LPTexture pGunLunTex = g_pTexMgr->GetTexImm(PACKAGE_INTERFACE,10721+m_wNow,EP_UI);
	if(pGunLunTex)
	{
		int x = m_iScreenX - 26;
		int y = m_iScreenY - 16;

		if(m_wNow==2)
		{
			if(m_bLeftLight)
				g_pGfx->DrawTextureNL(x,y,g_pTexMgr->GetTex(PACKAGE_INTERFACE,10811,EP_UI));
			else if(m_bRightLight)
				g_pGfx->DrawTextureNL(x,y,g_pTexMgr->GetTex(PACKAGE_INTERFACE,10810,EP_UI));
			else
				g_pGfx->DrawTextureNL(x,y,pGunLunTex);
		}
		else
		{
			g_pGfx->DrawTextureNL(x,y,pGunLunTex);
		}
	}

	if(m_bHelpLight)
	{
		g_pGfx->DrawTextureNL(m_iScreenX+m_iWidth-31,m_iScreenY+1,g_pTexMgr->GetTex(PACKAGE_INTERFACE,10825,EP_UI));
	}
	g_pGfx->DrawTextureNL(m_iScreenX+m_iWidth-25,m_iScreenY+136,g_pTexMgr->GetTex(PACKAGE_INTERFACE,10826,EP_UI));

	if(!m_bLeftRoll && !m_bRightRoll)
	{
		DrawRangeMenu();
	}

	if(m_pGrid && m_pGrid->GetScrollPos() != m_oldPos)
	{
		m_bSend = true;
		m_bMySend = true;
		m_oldPos = m_pGrid->GetScrollPos();
	}
	//显示帮助
	if(m_bHelp)
	{
		if(m_pGrid)
		{
			m_pGrid->SetShow(false);
			m_pGrid->SetEnable(false);
		}
		DrawHelp();
		return;
	}

	m_pGrid->SetShow(true);
	m_pGrid->SetEnable(true);
	DrawColumn();
	DrawListData();
	DrawMyInfo();

	m_pMBLevel->Draw();//重绘menubutton
}

bool CRangeWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	if(dwMsg == MSG_CTRL_BUTTON_CLICK)
	{
		if(pControl == m_pRightButton )
		{
			m_bRightRoll = true;
		}
		else if(pControl == m_pLeftButton )
		{
			m_bLeftRoll = true;
		}
		else if(pControl == m_pHelpCloseButton )
		{
			m_bHelp = false;
			//if(m_pHelpPaneButton)
			//{
			//	RemoveControl( (CControl **)(&m_pHelpPaneButton) );
			//	m_pHelpPaneButton = NULL;
			//}

			m_pHelpCloseButton->SetShow(false);
			int type = (g_Range[m_CurrentType]<<8) + m_CurrentSmallType;
			if(type==0x0104)
				m_pMBLevel->SetShow(true);

		}
		else
		{
			for(int i = 0;i < 6;i++)
			{
				if(pControl == m_pUnSelectTabButton[i])
				{
					if(strcmp(m_pUnSelectTabButton[i]->GetText(),"")!=0)
					{
						//m_pSelectTabButton->Move(m_pUnSelectTabButton[i]->GetX(),m_pUnSelectTabButton[i]->GetY());
						m_CurrentSmallType = i;
						//m_pSelectTabButton->SetText(m_pUnSelectTabButton[i]->GetText());
						m_pUnSelectTabButton[i]->SetText(m_pUnSelectTabButton[i]->GetText(),0xfffac897,0xfffac897,0xfffac897,0xfffac897,12,DTF_BlackFrame,FONT_YAHEI);
						Initcolumn();
					}
				}
				else
				{
					m_pUnSelectTabButton[i]->SetText(m_pUnSelectTabButton[i]->GetText(),0xfff1af65,0xfffac897,0xfffac897,0xfffac897,12,DTF_BlackFrame,FONT_YAHEI);
				}
			}
		}

		//音乐
		g_pAudio->Play(EAT_OTHER,2,g_pAudio->GetRand()++);
	}
	else if(dwMsg == MSG_CTRL_MENU_SELCHANGED)
	{
		if(pControl == m_pMBLevel )
		{
			int iCurSel = m_pMBLevel->GetCurSel();
			if(iCurSel >= 0)
			{
				m_nowGeLevel = iCurSel;
				m_bMySend = true;
				m_bSend = true;
			}

			m_pMBLevel->SetText(m_pMBLevel->GetSelText(),0xffE3C800,0xffE3C800,0xffE3C800,0xffE3C800,FONTSIZE_DEFAULT,0,FONT_YAHEI);
		}

		//音乐
		g_pAudio->Play(EAT_OTHER,2,g_pAudio->GetRand()++);
	}
	else if (dwMsg == MSG_CTRL_RANGE_GOTO_PLACE)
	{
		GotoPlace(dwData);
		return true;
	}
	else if (MSG_CTRL_GRID_DBCLICK == dwMsg)
	{
		//int iSelLine = HIWORD(dwData);
		int n = m_pGrid->GetHotLine();
		if(m_pGrid->GetScrollPos() > 0)
			n--;
		std::string strName;
		if(n>=0 && mapRangeData!=NULL)
		{
			RANGE_DADA::iterator itr;
			if((itr=mapRangeData->find(n+1)) == mapRangeData->end())
				return true;

			void *RangeData = ((STRUCT_RANGE_DADA*)(itr->second))->vData;
			if(!RangeData)
				return true;

			switch(g_Range[m_CurrentType])
			{
			case 1:
				strName.assign(((ALL_RANGE*)RangeData)->name,MYCHAR_LEN);
				break;
			case 2:
				strName.assign(((TEACHER_RANGE*)RangeData)->name,MYCHAR_LEN);
				break;
			case 16:
				strName.assign(((RTS_FIRE*)RangeData)->name,MYCHAR_LEN);
				break;
			default:
				return true;
			}
			//strName += " ";
			char szName[32] = {0};
			sprintf(szName,"%s ",strName.c_str());
			g_pControl->Msg(MSG_CTRL_INSERT_TEXT, 1,(CControl*)szName);
		}
		return true;
	}

	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CRangeWnd::DrawRangeMenu()
{

	if(m_dwLeftTex)
		g_pGfx->DrawTextureNL(m_iScreenX-26,m_iScreenY-16,g_pTexMgr->GetTexFromID(m_dwLeftTex,EP_UI));
	if(m_dwMidTex)
		g_pGfx->DrawTextureNL(m_iScreenX-26,m_iScreenY-16,g_pTexMgr->GetTexFromID(m_dwMidTex,EP_UI));
	if(m_dwRightTex)
		g_pGfx->DrawTextureNL(m_iScreenX-26,m_iScreenY-16,g_pTexMgr->GetTexFromID(m_dwRightTex,EP_UI));
}

/*
1.	等级排名
2.	名师排名
3.	元神排名
4.	宠物豹
5.	锻造武器
6.	行会排行
7.	灭魔排行
8、 英雄擂台排名
9、 恶魔城排名
10、武馆排名
11、世界英雄
12、世界尊主
13  宗族排行
RT_KFZ,历届跨服战排名,内嵌IE
*/
void CRangeWnd::InitTexByType()
{
	if(!m_pRangeText) return;

	for(int i=0;i<g_RangeNum;i++)
	{
		m_pRangeText[i].dwLeftTex = 0;
		m_pRangeText[i].dwMidTex= 0;
		m_pRangeText[i].dwRightTex= 0;

		m_pRangeText[i].wType = g_Range[i];
		if(i==0)
			m_pRangeText[i].cNow = 2;
		else if(i==1)
			m_pRangeText[i].cNow = 1;
		else if(i==g_RangeNum-1)
			m_pRangeText[i].cNow = 3;
		else
			m_pRangeText[i].cNow = 0;

		int texno=0;
		switch(g_Range[i])
		{
		case 1:
			texno = 10724;
			break;
		case 2: 
			texno = 10727;
			break;
		case 3:	
			texno = 10730;
			break;
		case 4:
			texno = 10733;
			break;
		case 5:
			texno = 10736;
			break;
		case 6:	
			texno = 10739;
			break;
		case 7:	
			texno = 10742;
			break;
		case 8:
			texno = 10745;
			break;
		case 9:
			texno = 10754;
			break;
		case 10:
			texno = 10757;
			break;
		case 11:
			texno = 10748;
			break;
		case 12:
			texno = 10751;
			break;
		case 13:
			texno = 17055;
			break;
		case 14:
			texno = 20965;
			break;
		case 15:
			texno = 17597;
			break;
		case 16:
			texno = 16520;
			break;
		case 17:
			texno = 16525;
			break;
		case 18:
			texno = 16530;
			break;
		default:
			continue;
		}

		m_pRangeText[i].dwLeftTex = MAKELONG(texno,PACKAGE_INTERFACE);
		m_pRangeText[i].dwMidTex = MAKELONG(texno+1,PACKAGE_INTERFACE);
		m_pRangeText[i].dwRightTex = MAKELONG(texno+2,PACKAGE_INTERFACE);
	}
}

bool CRangeWnd::SetTexByType()
{
	for(int i=0;(i<g_RangeNum) && (g_RangeNum>=3) && m_pRangeText;i++)
	{
		if(m_pRangeText[i].wType == g_Range[m_CurrentType])
		{
			m_dwMidTex = m_pRangeText[i].dwMidTex;
			if(i==0)
			{
				m_dwLeftTex = m_pRangeText[1].dwLeftTex;
				m_dwRightTex = m_pRangeText[g_RangeNum-1].dwRightTex;
			}
			else if(i==g_RangeNum-1)
			{
				m_dwLeftTex = m_pRangeText[0].dwLeftTex;
				m_dwRightTex = m_pRangeText[g_RangeNum-2].dwRightTex;
			}
			else
			{
				m_dwLeftTex = m_pRangeText[i+1].dwLeftTex;
				m_dwRightTex = m_pRangeText[i-1].dwRightTex;
			}

			return true;
		}
	}
	return false;
}

void CRangeWnd::SetSmallType()
{
	//if(!m_pSelectTabButton)
	//	return;

	//m_pSelectTabButton->Move(m_pUnSelectTabButton[m_CurrentSmallType]->GetX(),m_pUnSelectTabButton[m_CurrentSmallType]->GetY());
	switch(g_Range[m_CurrentType])
	{
	case 1:
		m_pUnSelectTabButton[0]->SetText("总排名",0xfff1af65,0xfffac897,0xfffac897,0xfffac897,12,DTF_BlackFrame,FONT_YAHEI);
		m_pUnSelectTabButton[1]->SetText("战士",0xfff1af65,0xfffac897,0xfffac897,0xfffac897,12,DTF_BlackFrame,FONT_YAHEI);
		m_pUnSelectTabButton[2]->SetText("魔法师",0xfff1af65,0xfffac897,0xfffac897,0xfffac897,12,DTF_BlackFrame,FONT_YAHEI);
		m_pUnSelectTabButton[3]->SetText("道士",0xfff1af65,0xfffac897,0xfffac897,0xfffac897,12,DTF_BlackFrame,FONT_YAHEI);
		m_pUnSelectTabButton[4]->SetText("各等级",0xfff1af65,0xfffac897,0xfffac897,0xfffac897,12,DTF_BlackFrame,FONT_YAHEI);
		m_pUnSelectTabButton[5]->SetText("");
		break;
	case 3:	
		m_pUnSelectTabButton[0]->SetText("总排名",0xfff1af65,0xfffac897,0xfffac897,0xfffac897,12,DTF_BlackFrame,FONT_YAHEI);
		m_pUnSelectTabButton[1]->SetText("战士",0xfff1af65,0xfffac897,0xfffac897,0xfffac897,12,DTF_BlackFrame,FONT_YAHEI);
		m_pUnSelectTabButton[2]->SetText("魔法师",0xfff1af65,0xfffac897,0xfffac897,0xfffac897,12,DTF_BlackFrame,FONT_YAHEI);
		m_pUnSelectTabButton[3]->SetText("道士",0xfff1af65,0xfffac897,0xfffac897,0xfffac897,12,DTF_BlackFrame,FONT_YAHEI);
		m_pUnSelectTabButton[4]->SetText("");
		m_pUnSelectTabButton[5]->SetText("");
		break;
	case 2: 
	case 4:
	case 5:
	case 6:
	case 10:
	case 12:
		m_pUnSelectTabButton[0]->SetText("");
		m_pUnSelectTabButton[1]->SetText("");
		m_pUnSelectTabButton[2]->SetText("");
		m_pUnSelectTabButton[3]->SetText("");
		m_pUnSelectTabButton[4]->SetText("");
		m_pUnSelectTabButton[5]->SetText("");
		break;
	case 7:
		m_pUnSelectTabButton[0]->SetText("阿修罗神",0xfff1af65,0xfffac897,0xfffac897,0xfffac897,12,DTF_BlackFrame,FONT_YAHEI);
		m_pUnSelectTabButton[1]->SetText("铁血魔王",0xfff1af65,0xfffac897,0xfffac897,0xfffac897,12,DTF_BlackFrame,FONT_YAHEI);
		m_pUnSelectTabButton[2]->SetText("通天教主",0xfff1af65,0xfffac897,0xfffac897,0xfffac897,12,DTF_BlackFrame,FONT_YAHEI);
		m_pUnSelectTabButton[3]->SetText("禁地魔王",0xfff1af65,0xfffac897,0xfffac897,0xfffac897,12,DTF_BlackFrame,FONT_YAHEI);
		m_pUnSelectTabButton[4]->SetText("");
		m_pUnSelectTabButton[5]->SetText("");
		break;
	case 8:
		m_pUnSelectTabButton[0]->SetText("英雄榜",0xfff1af65,0xfffac897,0xfffac897,0xfffac897,12,DTF_BlackFrame,FONT_YAHEI);
		m_pUnSelectTabButton[1]->SetText("新手榜",0xfff1af65,0xfffac897,0xfffac897,0xfffac897,12,DTF_BlackFrame,FONT_YAHEI);
		m_pUnSelectTabButton[2]->SetText("强手榜",0xfff1af65,0xfffac897,0xfffac897,0xfffac897,12,DTF_BlackFrame,FONT_YAHEI);
		m_pUnSelectTabButton[3]->SetText("高手榜",0xfff1af65,0xfffac897,0xfffac897,0xfffac897,12,DTF_BlackFrame,FONT_YAHEI);
		m_pUnSelectTabButton[4]->SetText("精英榜",0xfff1af65,0xfffac897,0xfffac897,0xfffac897,12,DTF_BlackFrame,FONT_YAHEI);
		m_pUnSelectTabButton[5]->SetText("");
		break;
	case 9:
		m_pUnSelectTabButton[0]->SetText("当天排名",0xfff1af65,0xfffac897,0xfffac897,0xfffac897,12,DTF_BlackFrame,FONT_YAHEI);
		m_pUnSelectTabButton[1]->SetText("当届排名",0xfff1af65,0xfffac897,0xfffac897,0xfffac897,12,DTF_BlackFrame,FONT_YAHEI);
		m_pUnSelectTabButton[2]->SetText("");
		m_pUnSelectTabButton[3]->SetText("");
		m_pUnSelectTabButton[4]->SetText("");
		m_pUnSelectTabButton[5]->SetText("");
		break;
	case 11:
		m_pUnSelectTabButton[0]->SetText("总排名",0xfff1af65,0xfffac897,0xfffac897,0xfffac897,12,DTF_BlackFrame,FONT_YAHEI);
		m_pUnSelectTabButton[1]->SetText("战士",0xfff1af65,0xfffac897,0xfffac897,0xfffac897,12,DTF_BlackFrame,FONT_YAHEI);
		m_pUnSelectTabButton[2]->SetText("魔法师",0xfff1af65,0xfffac897,0xfffac897,0xfffac897,12,DTF_BlackFrame,FONT_YAHEI);
		m_pUnSelectTabButton[3]->SetText("道士",0xfff1af65,0xfffac897,0xfffac897,0xfffac897,12,DTF_BlackFrame,FONT_YAHEI);
		m_pUnSelectTabButton[4]->SetText("");
		m_pUnSelectTabButton[5]->SetText("");
		break;
	case 14:
		m_pUnSelectTabButton[0]->SetText("挖矿",0xfff1af65,0xfffac897,0xfffac897,0xfffac897,12,DTF_BlackFrame,FONT_YAHEI);
		m_pUnSelectTabButton[1]->SetText("伐木",0xfff1af65,0xfffac897,0xfffac897,0xfffac897,12,DTF_BlackFrame,FONT_YAHEI);
		m_pUnSelectTabButton[2]->SetText("猎取",0xfff1af65,0xfffac897,0xfffac897,0xfffac897,12,DTF_BlackFrame,FONT_YAHEI);
		m_pUnSelectTabButton[3]->SetText("拆卸",0xfff1af65,0xfffac897,0xfffac897,0xfffac897,12,DTF_BlackFrame,FONT_YAHEI);
		m_pUnSelectTabButton[4]->SetText("铸造",0xfff1af65,0xfffac897,0xfffac897,0xfffac897,12,DTF_BlackFrame,FONT_YAHEI);
		m_pUnSelectTabButton[5]->SetText("制炼",0xfff1af65,0xfffac897,0xfffac897,0xfffac897,12,DTF_BlackFrame,FONT_YAHEI);
		break;
	default:
		m_pUnSelectTabButton[0]->SetText("");
		m_pUnSelectTabButton[1]->SetText("");
		m_pUnSelectTabButton[2]->SetText("");
		m_pUnSelectTabButton[3]->SetText("");
		m_pUnSelectTabButton[4]->SetText("");
		m_pUnSelectTabButton[5]->SetText("");
		break;
	}
	if(strcmp(m_pUnSelectTabButton[0]->GetText(),"")==0)
	{
		//m_pSelectTabButton->SetShow(false);
	}
	else
	{
		//m_pSelectTabButton->SetText(m_pUnSelectTabButton[m_CurrentSmallType]->GetText());
		//m_pSelectTabButton->SetShow(true);
		m_pUnSelectTabButton[m_CurrentSmallType]->SetText(m_pUnSelectTabButton[m_CurrentSmallType]->GetText(),0xfffac897,0xfffac897,0xfffac897,0xfffac897,12,DTF_BlackFrame,FONT_YAHEI);
	}
	for(int i=0;i < 6;i++)
	{
		if(strcmp(m_pUnSelectTabButton[i]->GetText(),"")==0)
			m_pUnSelectTabButton[i]->SetShow(false);
		else
			m_pUnSelectTabButton[i]->SetShow(true);
	}
}

void  CRangeWnd::OnClickCloseButton()
{
	CloseWindow();
}
/*
1.	等级排名
2.	名师排名
3.	元神排名
4.	宠物豹
5.	锻造武器
6.	行会排行
7.	灭魔排行
8、 英雄擂台排名
9、 恶魔城排名
10、武馆排名
11、世界英雄
12、世界尊主
*/
void  CRangeWnd::Initcolumn()
{
	m_bMySend = true;
	m_bSend = true;

	int type = (g_Range[m_CurrentType]<<8) + m_CurrentSmallType;

	if(m_pGrid)
	{
		m_pGrid->SetShow(true);
		m_pGrid->SetEnable(true);
	}
	SetPageTex(0,PACKAGE_INTERFACE,10712);
	g_WidgetMgr.SetShow(EWT_KFZ_IE,false);
	g_WidgetMgr.SetFocus(EWT_KFZ_IE,false);

	//列表控件
	if(type==0x0104)
	{
		if(!m_bHelp)
			m_pMBLevel->SetShow(true);
	}
	else
	{
		m_pMBLevel->SetShow(false);
	}

	//底板色
	m_pGrid->SetDrawOffXY(0,5);
	m_pGrid->SetTotalCount(0);

	///////////
	m_LastSendTick = GetTickCount();
	m_MyLastSendTick = m_LastSendTick;

	m_pGrid->ClearColumn();

	m_strColumnWidth ="";

	if(m_ColumnWidth.find(g_Range[m_CurrentType]) == m_ColumnWidth.end())
		return;
	m_strColumnWidth = m_ColumnWidth[g_Range[m_CurrentType]];


	int j=0,jpos=0;
	string jtemp;

	while(1)
	{
		j = m_strColumnWidth.find(";",jpos);
		if(j==-1)
			break;
		jtemp.assign(m_strColumnWidth,jpos,j-jpos);
		jpos = j+1;
		m_pGrid->PushColumn(atoi(jtemp.c_str()));
	}



}

void CRangeWnd::DrawListData()
{
	mapRangeData = NULL;
	if(!m_pGrid)
		return;

	SendServerExpUp();

	int type = (g_Range[m_CurrentType]<<8) + m_CurrentSmallType;	//m_CurrentSmallTyp为0,1,2,3,4
	if(type==0x0104)	//如果是各等级
	{
		type += m_nowGeLevel;
	}
	//如果自己信息还没有，获得自己的数据
	m_pMyMapRangeData = NULL;

	if(m_mapMyRangeData.find(type) !=m_mapMyRangeData.end())
	{
		m_pMyMapRangeData = m_mapMyRangeData[type];
		//获得自己的数据,获得版本
		if(m_bMySend && m_dwTickCount - m_MyLastSendTick >3000)
		{
			STRUCT_RANGE_DADA* pRange = ((STRUCT_RANGE_DADA *)(*m_pMyMapRangeData)[0]);
			if(pRange)
			{
				g_pGameControl->Send_Range_Request(SELF.GetID(),type,pRange->version,0);
				m_MyLastSendTick = m_dwTickCount;
				m_bMySend = false;
			}
		}

	}
	else
	{
		if(m_bMySend && m_dwTickCount - m_MyLastSendTick >3000)
		{
			g_pGameControl->Send_Range_Request(SELF.GetID(),type,0,0);
			m_MyLastSendTick = m_dwTickCount;
			m_bMySend = false;
		}

	}

	//如果为空，向服务器取当前大类和当前小类数据
	ALL_RANGE_DADA::iterator big_itr;

	if((big_itr=m_mapRangeData.find(type))==m_mapRangeData.end())
	{
		map<WORD,WORD>::iterator itrs;
		if((itrs=m_mapTotalNum.find(type))!=m_mapTotalNum.end())
		{
			if(itrs->second == 0)
			{
				//3分一次
				if(m_bSend && m_dwTickCount - m_LastSendTick >3000*60)
				{
					g_pGameControl->Send_Range_Request(SELF.GetID(),type,0,1);
					m_LastSendTick = m_dwTickCount;
					m_bSend = false;
				}
				else
				{
					g_pFont->DrawText(300+m_iScreenX ,154+m_iScreenY,"无排名信息!",COLOR_DEFAULT,FONT_DEFAULT,FONTSIZE_MIDDLE,DTF_BlackFrame);
				}
				return;
			}
		}
		//3秒一次
		if(m_bSend && m_dwTickCount - m_LastSendTick >3000)
		{
			g_pGameControl->Send_Range_Request(SELF.GetID(),type,0,1);
			m_LastSendTick = m_dwTickCount;
			m_bSend = false;
		}

		DrawWaitReading();
		return;
	}

	mapRangeData = big_itr->second;
	//如果数据没有所要获得的编号，向服务器获取需要的编号
	void* pRangeData = NULL;
	int pos;
	pos = m_pGrid->GetScrollPos() <=0 ? 1:m_pGrid->GetScrollPos();
	int n=0;

	RANGE_DADA::iterator itr,fistItr,SendItr;
	int num = m_pGrid->GetLinesPerPage();

	for(int i = 0;i < num;i++)
	{
		if( pos+i > m_mapTotalNum[type])
			break;

		if((itr=mapRangeData->find(pos+i)) == mapRangeData->end())
		{
			//3秒一次
			if(m_bSend && m_dwTickCount - m_LastSendTick >3000)
			{
				if(type == 0x0600 && g_HaveSBK) //如果是有沙城行会
				{
					if(pos+i-1<=0)
						g_pGameControl->Send_Range_Request(SELF.GetID(),type,0,1);
					else
						g_pGameControl->Send_Range_Request(SELF.GetID(),type,0,pos+i-1);
				}
				else
					g_pGameControl->Send_Range_Request(SELF.GetID(),type,0,pos+i);

				m_LastSendTick = m_dwTickCount;
				m_bSend = false;
			}

			DrawWaitReading();
			return;
		}
		else
		{
			if(i==0)
				fistItr = itr;
			n++;

			//如果两个版本不一样
			//STRUCT_RANGE_DADA* pRange = ((STRUCT_RANGE_DADA *)(itr->second));
			//if(pRange != NULL && g_RangeVesion[g_Range[m_CurrentType]] != pRange->version)
			//{
			//	//3秒一次
			//	if(m_bSend && m_dwTickCount - m_LastSendTick >3000)
			//	{
			//		if(type == 0x0600 && g_HaveSBK) //如果是有沙城行会
			//		{
			//			if(pos+i-1<=0)
			//				g_pGameControl->Send_Range_Request(SELF.GetID(),type,pRange->version,1);
			//			else
			//				g_pGameControl->Send_Range_Request(SELF.GetID(),type,pRange->version,pos+i-1);
			//		}
			//		else
			//			g_pGameControl->Send_Range_Request(SELF.GetID(),type,pRange->version,pos+i);
			//		m_LastSendTick = m_dwTickCount;
			//		m_bSend = false;
			//	}
			//	DrawWaitReading();
			//	return;

			//}
			//else
			//{
			//	//3分钟一次
			//	if(m_dwTickCount - m_LastSendTick >3000*60)
			//	{
			//		g_pGameControl->Send_Range_Request(SELF.GetID(),type,((STRUCT_RANGE_DADA *)(itr->second))->version,pos+i);
			//		m_LastSendTick = m_dwTickCount;
			//	}
			//}
		}
	}

	//显示数据
	//m_pGrid->SetDisLine(m_pDrapButton->GetPos());
	//m_pGrid->DrawEx(m_iScreenX,m_iScreenY);

	itr = fistItr;

	for(int i = 0;i < n;i++)
	{
		STRUCT_RANGE_DADA* pRange = ((STRUCT_RANGE_DADA *)(itr->second));
		if(pRange != NULL)
		{
			pRangeData = pRange->vData;
			DrawLineData(i,pRangeData);
		}
		itr++;
	}

}

bool CRangeWnd::OnLeftButtonUp(int iX, int iY)
{
	if(iX>=26 && iX<=26+16 && iY>=42 &&  iY<=42+228 )
	{
		m_bRightRoll = true;
		g_pAudio->Play(EAT_OTHER,2,g_pAudio->GetRand()++);
	}
	else if(iX>=66 && iX<=66+16 && iY>=42 &&  iY<=42+228 )
	{
		m_bLeftRoll = true;
		g_pAudio->Play(EAT_OTHER,2,g_pAudio->GetRand()++);
	}
	else if(iX >= 700 && iX <= 700 + 17 && iY >= 42 &&  iY <= 42 + 228 )
	{
		m_bHelp = true;
		//if(m_pHelpPaneButton == NULL)
		//{
			//m_pHelpPaneButton = new CCtrlButton();
			//AddControl(m_pHelpPaneButton,1000);
			//m_pHelpPaneButton->CreateEx(this,109,45,10824,10824,10824);
		//}

		m_pHelpCloseButton->SetShow(true);
		m_pMBLevel->SetShow(false);

		g_pAudio->Play(EAT_OTHER,2,g_pAudio->GetRand()++);
	}
	else if(iX>=124 && iX<=566 && iY>=268 &&  iY<=288 )
	{
		FindMyPlace();
	}

	return CCtrlWindowX::OnLeftButtonUp(iX,iY);
}

bool CRangeWnd::OnMouseMove(int iX,int iY)
{
	CParserTip *pTip = g_pControl->GetTipWnd();
	if(m_pGrid && iX > m_pGrid->GetX() && iY > m_pGrid->GetY() && iX < m_pGrid->GetX() + m_pGrid->GetWidth() && iY < m_pGrid->GetY() + m_pGrid->GetHeight())
	{
		SetTips(iX,iY);
	}
	else
	{
		pTip->SetShow(false);
	}

	if(iX>=26 && iX<=26+16 && iY>=42 &&  iY<=42+228 )
	{
		m_bLeftLight = true;
		m_bRightLight = false;
	}
	else if(iX>=66 && iX<=66+16 && iY>=42 &&  iY<=42+228 )
	{
		m_bLeftLight = false;
		m_bRightLight = true;
	}
	else
	{
		m_bLeftLight = false;
		m_bRightLight = false;
	}

	if(iX >= 700 && iX <= 700 + 17 && iY >= 42 &&  iY <= 42 + 228 )
	{
		m_bHelpLight = true;
	}
	else
		m_bHelpLight = false;

	if(iX>=124 && iX<=566 && iY>=268 &&  iY<=288 )
	{
		m_pTipMyInfo.Move(iX+4,iY+20);
		m_pTipMyInfo.SetShow(true);
		pTip->SetShow(false);
	}
	else
		m_pTipMyInfo.SetShow(false);


	if(pTip->IsShow())
		return true;

	return CCtrlWindowX::OnMouseMove(iX,iY);
}

void CRangeWnd::DrawColumn()
{
	if(m_strColumnWidth =="")
		return;
	int type = (g_Range[m_CurrentType]<<8) + m_CurrentSmallType;
	string temp,jtemp;
	int i=0,pos=0;
	int j=0,jpos=0;
	int ix=m_iScreenX;

	if(type==0x0104)	//如果是各等级
	{
		type += m_nowGeLevel;

		//g_pGfx->DrawTextureNL(110 + ix,m_iScreenY+40,m_pDragListTex);			
	}
	
	if(m_ColumnText.find(g_Range[m_CurrentType])!=m_ColumnText.end())
		g_pFont->DrawText(142 + ix,m_iScreenY + 60,m_ColumnText[g_Range[m_CurrentType]].c_str(),0xffb46428,FONT_YAHEI,12,DTF_BlackFrame);
	
	map<WORD,WORD>::iterator itr;

	if((itr = m_mapTotalNum.find(type))!=m_mapTotalNum.end())
	{
		m_pGrid->SetTotalCount(itr->second);
	}
}

void CRangeWnd::DrawLineData(int i,void *RangeData)
{
	if(RangeData == NULL)
		return;

	char ClickStr[31]={0};
	char str[31]={0};
	int iLevel = 0;
	int tipo = 0;
	BYTE byType = 0;

	char szGuildName[31] = {0};
	string strShortdName;
	string strTemp;

	switch(g_Range[m_CurrentType])
	{
	case 1:	
		//if(((ALL_RANGE*)RangeData)->inmerlevel > 0)
		//{
		//	LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15198);
		//	if(pTex)
		//	{
		//		int iOffset = 77;
		//		//int type = (g_Range[m_CurrentType]<<8) + m_CurrentSmallType;
		//		//if(type==0x0104)	//如果是各等级
		//		//{
		//		//	iOffset = 93;
		//		//}
		//		g_pGfx->DrawTextureNL(GetScreenX() + 162,(int)(GetScreenY() + iOffset + i * (float)19.5),pTex);
		//	}
		//}
		sprintf(str,"%d",((ALL_RANGE*)RangeData)->place);
		m_pGrid->DrawGrid(i,0,str,false,COLOR_TEXT_SBU_TITLE);
		strTemp.assign(((ALL_RANGE*)RangeData)->name,MYCHAR_LEN);
		m_pGrid->DrawGrid(i,1,strTemp.c_str(),false,COLOR_TEXT_MAIN_TITLE);
		sprintf(str,"%d",((ALL_RANGE*)RangeData)->level);
		m_pGrid->DrawGrid(i,2,str,false,COLOR_TEXT_MAIN_TITLE);
		m_pGrid->DrawGrid(i,3,GetJobName(((ALL_RANGE*)RangeData)->work),false,COLOR_TEXT_MAIN_TITLE);
		m_pGrid->DrawGrid(i,4,GetGenderName(((ALL_RANGE*)RangeData)->sex),false,COLOR_TEXT_MAIN_TITLE);


		memcpy(szGuildName,((ALL_RANGE*)RangeData)->guild,GUILDNAME_LEN) ;

		//获取前面几个字符
		if(strlen(szGuildName)>14)
		{
			int i=0;
			while (i< 14)
			{
				char* p = CharNext(szGuildName+i);
				i = (int)(p-szGuildName);
			}
			strShortdName.assign(szGuildName,i);
		}
		else
		{
			strShortdName = szGuildName;
		}
		m_pGrid->DrawGrid(i,5,strShortdName.c_str(),false,COLOR_TEXT_MAIN_TITLE);
		sprintf(str,"%d%%",((ALL_RANGE*)RangeData)->exp);
		m_pGrid->DrawGrid(i,6,str,false,COLOR_TEXT_MAIN_TITLE);

		memcpy(ClickStr,((ALL_RANGE*)RangeData)->name,MYCHAR_LEN);
		break;
	case 2:
		sprintf(str,"%d",((TEACHER_RANGE*)RangeData)->place);
		m_pGrid->DrawGrid(i,0,str,false,COLOR_TEXT_MAIN_TITLE);
		strTemp.assign(((TEACHER_RANGE*)RangeData)->name,MYCHAR_LEN);
		m_pGrid->DrawGrid(i,1,strTemp.c_str(),false,COLOR_TEXT_MAIN_TITLE);
		sprintf(str,"%d",((TEACHER_RANGE*)RangeData)->tudiNum);
		m_pGrid->DrawGrid(i,2,str,false,COLOR_TEXT_MAIN_TITLE);

		m_pGrid->DrawGrid(i,3,GetJobName(((TEACHER_RANGE*)RangeData)->work),false,COLOR_TEXT_MAIN_TITLE);
		m_pGrid->DrawGrid(i,4,GetGenderName(((TEACHER_RANGE*)RangeData)->sex),false,COLOR_TEXT_MAIN_TITLE);
		sprintf(str,"%d",((TEACHER_RANGE*)RangeData)->level);
		m_pGrid->DrawGrid(i,5,str,false,COLOR_TEXT_MAIN_TITLE);

		memcpy(ClickStr,((TEACHER_RANGE*)RangeData)->name,MYCHAR_LEN);
		break;
	case 4:	
		sprintf(str,"%d",((PET_RANGE*)RangeData)->place);
		m_pGrid->DrawGrid(i,0,str);
		strTemp.assign(((PET_RANGE*)RangeData)->name,MYCHAR_LEN);
		m_pGrid->DrawGrid(i,1,strTemp.c_str());
		iLevel = ((PET_RANGE*)RangeData)->level;
		byType = ((PET_RANGE*)RangeData)->type;

		if(iLevel<=7)
		{
			if (byType == 0) sprintf(str,"丛林豹%d级",iLevel %8);//豹子
			else if(byType == 1) sprintf(str,"天狮兽%d级",iLevel %8);
			else if (byType == 2) sprintf(str,"麒麟兽%d级",iLevel %8);
			else if (byType == 3) sprintf(str,"赤翎凤%d级",iLevel %8);
		}
		else if (iLevel <= 15)
		{
			if (byType == 0) sprintf(str,"踏云豹%d级",iLevel %8);//豹子
			else if(byType == 1) sprintf(str,"震天狮%d级",iLevel %8);
			else if (byType == 2) sprintf(str,"圣麒麟%d级",iLevel %8);
			else if (byType == 3) sprintf(str,"傲天凤%d级",iLevel %8);
		}
		else
		{
			if (byType == 0) sprintf(str,"迅天雷豹%d级",iLevel - 16);//豹子
			else if(byType == 1) sprintf(str,"战天狂狮%d级",iLevel - 16);
			else if (byType == 2) sprintf(str,"玄天麒麟%d级",iLevel - 16);
			else if (byType == 3) sprintf(str,"炽天凤凰%d级",iLevel - 16);
		}	

		m_pGrid->DrawGrid(i,2,str);
		sprintf(str,"%d",((PET_RANGE*)RangeData)->exp);
		m_pGrid->DrawGrid(i,3,str);

		strTemp.assign(((PET_RANGE*)RangeData)->Fenhao,14);
		m_pGrid->DrawGrid(i,4,strTemp.c_str());
		strTemp.assign(((PET_RANGE*)RangeData)->mainName,MYCHAR_LEN);
		m_pGrid->DrawGrid(i,5,strTemp.c_str());

		memcpy(ClickStr,((PET_RANGE*)RangeData)->mainName,MYCHAR_LEN);
		break;
	case 5:
		sprintf(str,"%d",((ZHUZAO_RANGE*)RangeData)->place);
		m_pGrid->DrawGrid(i,0,str);

		strTemp.assign(((ZHUZAO_RANGE*)RangeData)->name,MYCHAR_LEN);
		m_pGrid->DrawGrid(i,1,strTemp.c_str());

		m_pGrid->DrawGrid(i,2,GetGenderName(((ZHUZAO_RANGE*)RangeData)->sex));

		sprintf(str,"%d",((ZHUZAO_RANGE*)RangeData)->level);
		m_pGrid->DrawGrid(i,3,str);
		m_pGrid->DrawGrid(i,4,GetJobName(((ZHUZAO_RANGE*)RangeData)->work));

		sprintf(str,"%d",((ZHUZAO_RANGE*)RangeData)->exp);
		m_pGrid->DrawGrid(i,5,str);

		memcpy(ClickStr,((ZHUZAO_RANGE*)RangeData)->name,MYCHAR_LEN);
		break;
	case 6:	
		if(((GUILD_RANGE*)RangeData)->place == 0)
		{
			strcpy(str,"沙城");
			//m_pGrid->SetLineNumType(ECT_NUMBER);
			//m_pGrid->SetFont(FONT_GDI_HANGKAI);
			//m_pGrid->SetTextColor(0xff960000,0xff960000);
			//m_pGrid->SetSelTextColor(0xff960000);
			m_pGrid->DrawGrid(i,0,str,false,COLOR_TEXT_SBU_TITLE);
			//m_pGrid->SetLineNumType(ECT_PIC);
			//m_pGrid->SetFont(FONTSIZE_DEFAULT);
			//m_pGrid->SetTextColor(0xff000000,0xff960000);
			//m_pGrid->SetSelTextColor(0xFFFFFF00);
		}
		else
		{
			sprintf(str,"%d",((GUILD_RANGE*)RangeData)->place);
			m_pGrid->DrawGrid(i,0,str,false,COLOR_TEXT_SBU_TITLE);
		}

		/*if(((GUILD_RANGE*)RangeData)->towerLevel > 0)
		{
			LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,16285);
			if(pTex)
			{
				g_pGfx->DrawTextureNL(GetScreenX() + 178,(int)(GetScreenY() + 76 + i * (float)19.5),pTex);
			}
		}*/

		strTemp.assign(((GUILD_RANGE*)RangeData)->name,MYCHAR_LEN);
		m_pGrid->DrawGrid(i,1,strTemp.c_str(),false,COLOR_TEXT_MAIN_TITLE);

		sprintf(str,"%d",((GUILD_RANGE*)RangeData)->towerLevel);
		m_pGrid->DrawGrid(i,2,str,false,COLOR_TEXT_MAIN_TITLE);

		sprintf(str,"%d",((GUILD_RANGE*)RangeData)->num46);
		m_pGrid->DrawGrid(i,3,str,false,COLOR_TEXT_MAIN_TITLE);
		//sprintf(str,"%d",((GUILD_RANGE*)RangeData)->numShadow);
		//m_pGrid->DrawGrid(i,3,str,false,COLOR_TEXT_MAIN_TITLE);

		sprintf(str,"%u",((GUILD_RANGE*)RangeData)->exp);
		m_pGrid->DrawGrid(i,4,str,false,COLOR_TEXT_MAIN_TITLE);
		sprintf(str,"%d",((GUILD_RANGE*)RangeData)->num);
		m_pGrid->DrawGrid(i,5,str,false,COLOR_TEXT_MAIN_TITLE);

		break;
	case 7:	
		sprintf(str,"%d",((MIEMO_RANGE*)RangeData)->place);
		m_pGrid->DrawGrid(i,0,str);

		strTemp.assign(((MIEMO_RANGE*)RangeData)->name,MYCHAR_LEN);
		m_pGrid->DrawGrid(i,1,strTemp.c_str());

		m_pGrid->DrawGrid(i,2,GetGenderName(((MIEMO_RANGE*)RangeData)->sex));

		sprintf(str,"%d",((MIEMO_RANGE*)RangeData)->level);
		m_pGrid->DrawGrid(i,3,str);
		m_pGrid->DrawGrid(i,4,GetGenderName(((MIEMO_RANGE*)RangeData)->work));
		strTemp.assign(((MIEMO_RANGE*)RangeData)->guild,GUILDNAME_LEN);
		m_pGrid->DrawGrid(i,5,strTemp.c_str());

		sprintf(str,"%d",((MIEMO_RANGE*)RangeData)->cishu);
		m_pGrid->DrawGrid(i,6,str);

		memcpy(ClickStr,((MIEMO_RANGE*)RangeData)->name,MYCHAR_LEN);
		break;
	case 8:
		sprintf(str,"%d",((ARENA_RANGE*)RangeData)->place);
		m_pGrid->DrawGrid(i,0,str);

		strTemp.assign(((ARENA_RANGE*)RangeData)->name,MYCHAR_LEN);
		m_pGrid->DrawGrid(i,1,strTemp.c_str());

		m_pGrid->DrawGrid(i,2,GetJobName(((ARENA_RANGE*)RangeData)->work));
		sprintf(str,"%d",((ARENA_RANGE*)RangeData)->level);
		m_pGrid->DrawGrid(i,3,str);
		sprintf(str,"%d",((ARENA_RANGE*)RangeData)->cishu);
		m_pGrid->DrawGrid(i,4,str);
		sprintf(str,"%d",((ARENA_RANGE*)RangeData)->exp);
		m_pGrid->DrawGrid(i,5,str);
		sprintf(str,"%u",((ARENA_RANGE*)RangeData)->money);
		m_pGrid->DrawGrid(i,6,str);

		memcpy(ClickStr,((ARENA_RANGE*)RangeData)->name,MYCHAR_LEN);
		break;
	case 9:
		sprintf(str,"%d",((RS_RANGE*)RangeData)->place);
		m_pGrid->DrawGrid(i,0,str);

		strTemp.assign(((RS_RANGE*)RangeData)->name,MYCHAR_LEN);
		m_pGrid->DrawGrid(i,1,strTemp.c_str());

		m_pGrid->DrawGrid(i,2,GetJobName(((RS_RANGE*)RangeData)->work));

		sprintf(str,"%u",((RS_RANGE*)RangeData)->haoshi);
		m_pGrid->DrawGrid(i,3,str);

		memcpy(ClickStr,((RS_RANGE*)RangeData)->name,MYCHAR_LEN);
		break;
	case 10:
		sprintf(str,"%d",((WUGUAN_RANGE*)RangeData)->place);
		m_pGrid->DrawGrid(i,0,str);

		strTemp.assign(((WUGUAN_RANGE*)RangeData)->name,MYCHAR_LEN);
		m_pGrid->DrawGrid(i,1,strTemp.c_str());

		m_pGrid->DrawGrid(i,2,GetJobName(((WUGUAN_RANGE*)RangeData)->work));

		sprintf(str,"%d",((WUGUAN_RANGE*)RangeData)->exp);
		m_pGrid->DrawGrid(i,3,str);
		strTemp.assign(((WUGUAN_RANGE*)RangeData)->Fenhao,14);
		m_pGrid->DrawGrid(i,4,strTemp.c_str());

		memcpy(ClickStr,((WUGUAN_RANGE*)RangeData)->name,MYCHAR_LEN);
		break;
	case 13:
		sprintf(str,"%d",((PHYLE_RANGE*)RangeData)->place);
		m_pGrid->DrawGrid(i,0,str,false,COLOR_TEXT_SBU_TITLE);		
		m_pGrid->DrawGrid(i,1,((PHYLE_RANGE*)RangeData)->name,false,COLOR_TEXT_MAIN_TITLE);
		sprintf(str,"%d",((PHYLE_RANGE*)RangeData)->MemberNum);
		m_pGrid->DrawGrid(i,2,str,false,COLOR_TEXT_MAIN_TITLE);
		sprintf(str,"%d",((PHYLE_RANGE*)RangeData)->SeizeNPCNum);
		m_pGrid->DrawGrid(i,3,str,false,COLOR_TEXT_MAIN_TITLE);
		break;
	case 14:
		sprintf(str,"%d",((PRODUCE_SKILL_RANGE*)RangeData)->place);
		m_pGrid->DrawGrid(i,0,str,false,COLOR_TEXT_SBU_TITLE);	
		strTemp.assign(((PRODUCE_SKILL_RANGE*)RangeData)->cPlayerName,MYCHAR_LEN);
		m_pGrid->DrawGrid(i,1,strTemp.c_str(),false,COLOR_TEXT_MAIN_TITLE);

		if(((PRODUCE_SKILL_RANGE*)RangeData)->bySkillLevel >= 0 && ((PRODUCE_SKILL_RANGE*)RangeData)->bySkillLevel <= 7)
		{
			m_pGrid->DrawGrid(i,2,szSkillLevel[((PRODUCE_SKILL_RANGE*)RangeData)->bySkillLevel],false,COLOR_TEXT_MAIN_TITLE);
		}

		m_pGrid->DrawGrid(i,3,GetJobName(((PRODUCE_SKILL_RANGE*)RangeData)->byWork),false,COLOR_TEXT_MAIN_TITLE);
		strTemp.assign(((PRODUCE_SKILL_RANGE*)RangeData)->cGuild,14);
		m_pGrid->DrawGrid(i,4,strTemp.c_str(),false,COLOR_TEXT_MAIN_TITLE);
		sprintf(str,"%d%%",((PRODUCE_SKILL_RANGE*)RangeData)->byExp);
		m_pGrid->DrawGrid(i,5,str,false,COLOR_TEXT_MAIN_TITLE);
		break;
	case 15:
		sprintf(str,"%d",((RTS_RANGE*)RangeData)->place);
		m_pGrid->DrawGrid(i,0,str,false,COLOR_TEXT_SBU_TITLE);	
		strTemp.assign(((RTS_RANGE*)RangeData)->cPlayerName,MYCHAR_LEN);
		m_pGrid->DrawGrid(i,1,strTemp.c_str(),false,COLOR_TEXT_MAIN_TITLE);

		sprintf(str,"%d",((RTS_RANGE*)RangeData)->byLevel);
		m_pGrid->DrawGrid(i,2,str,false,COLOR_TEXT_MAIN_TITLE);	

		m_pGrid->DrawGrid(i,3,GetJobName(((RTS_RANGE*)RangeData)->byWork),false,COLOR_TEXT_MAIN_TITLE);
		strTemp.assign(((RTS_RANGE*)RangeData)->cGuild,14);

		m_pGrid->DrawGrid(i,4,GetGenderName(((RTS_RANGE*)RangeData)->bySex),false,COLOR_TEXT_MAIN_TITLE);

		strTemp.assign(((RTS_RANGE*)RangeData)->cGuild,14);
		m_pGrid->DrawGrid(i,5,strTemp.c_str(),false,COLOR_TEXT_MAIN_TITLE);

		sprintf(str,"%d",((RTS_RANGE*)RangeData)->iWinNum);
		m_pGrid->DrawGrid(i,6,str,false,COLOR_TEXT_MAIN_TITLE);
		break;
	case 16:
		{
			sprintf(str,"%d",((RTS_FIRE*)RangeData)->place);
			m_pGrid->DrawGrid(i,0,str,false,COLOR_TEXT_SBU_TITLE);	
			strTemp.assign(((RTS_FIRE*)RangeData)->name,MYCHAR_LEN);
			m_pGrid->DrawGrid(i,1,strTemp.c_str(),false,COLOR_TEXT_MAIN_TITLE);

			char * p1[9] = 
			{
				"下阶青火境","中阶青火境","上阶青火境",
				"下阶石火境","中阶石火境","上阶石火境",
				"下阶天火境","中阶天火境","上阶天火境",
			};

			int iFireLevel = ((RTS_FIRE*)RangeData)->wSamadhiFireLevel;
			if (iFireLevel > 0 && iFireLevel <= 81)
			{
				sprintf(str, "%d级%s",iFireLevel,p1[(iFireLevel + 8) / 9 - 1]);
				m_pGrid->DrawGrid(i,2,str,false,COLOR_TEXT_MAIN_TITLE);
			}

			m_pGrid->DrawGrid(i,3,GetJobName(((RTS_FIRE*)RangeData)->work),false,COLOR_TEXT_MAIN_TITLE);

			m_pGrid->DrawGrid(i,4,GetGenderName(((RTS_FIRE*)RangeData)->sex),false,COLOR_TEXT_MAIN_TITLE);

			strTemp.assign(((RTS_FIRE*)RangeData)->guild,14);
			m_pGrid->DrawGrid(i,5,strTemp.c_str(),false,COLOR_TEXT_MAIN_TITLE);

			sprintf(str,"%d%%",((RTS_FIRE*)RangeData)->bySamadhiFire);
			m_pGrid->DrawGrid(i,6,str,false,COLOR_TEXT_MAIN_TITLE);	
		}
		break;
	case 17:
		{
			sprintf(str,"%d",((PINNA_RANGE*)RangeData)->place);
			m_pGrid->DrawGrid(i,0,str,false,COLOR_TEXT_SBU_TITLE);	
			strTemp.assign(((PINNA_RANGE*)RangeData)->name,MYCHAR_LEN);
			m_pGrid->DrawGrid(i,1,strTemp.c_str(),false,COLOR_TEXT_MAIN_TITLE);


			char temp[128];
			int wPinnaLevel = ((PINNA_RANGE*)RangeData)->wPinnaLevel;
			sprintf(temp, "%s%d级",g_AIGoodMgr.GetWingNameByLevel(wPinnaLevel).c_str(),wPinnaLevel);
			m_pGrid->DrawGrid(i,2,temp,false,COLOR_TEXT_MAIN_TITLE);

			m_pGrid->DrawGrid(i,3,GetJobName(((PINNA_RANGE*)RangeData)->work),false,COLOR_TEXT_MAIN_TITLE);

			m_pGrid->DrawGrid(i,4,GetGenderName(((PINNA_RANGE*)RangeData)->sex),false,COLOR_TEXT_MAIN_TITLE);

			strTemp.assign(((PINNA_RANGE*)RangeData)->guild,14);
			m_pGrid->DrawGrid(i,5,strTemp.c_str(),false,COLOR_TEXT_MAIN_TITLE);
		}
		break;
	case 18:
		{
			sprintf(str,"%d",((HORSEMANSHIP_RANGE*)RangeData)->place);
			m_pGrid->DrawGrid(i,0,str,false,COLOR_TEXT_SBU_TITLE);	
			strTemp.assign(((HORSEMANSHIP_RANGE*)RangeData)->name,MYCHAR_LEN);
			m_pGrid->DrawGrid(i,1,strTemp.c_str(),false,COLOR_TEXT_MAIN_TITLE);


			char temp[128];
			sprintf(temp, "%d级",((HORSEMANSHIP_RANGE*)RangeData)->wHorsemanshipLevel);
			m_pGrid->DrawGrid(i,2,temp,false,COLOR_TEXT_MAIN_TITLE);

			m_pGrid->DrawGrid(i,3,GetJobName(((HORSEMANSHIP_RANGE*)RangeData)->work),false,COLOR_TEXT_MAIN_TITLE);

			m_pGrid->DrawGrid(i,4,GetGenderName(((HORSEMANSHIP_RANGE*)RangeData)->sex),false,COLOR_TEXT_MAIN_TITLE);

			strTemp.assign(((HORSEMANSHIP_RANGE*)RangeData)->guild,14);
			m_pGrid->DrawGrid(i,5,strTemp.c_str(),false,COLOR_TEXT_MAIN_TITLE);

			sprintf(str,"%d%%",((HORSEMANSHIP_RANGE*)RangeData)->byHorsemanship);
			m_pGrid->DrawGrid(i,6,str,false,COLOR_TEXT_MAIN_TITLE);	
		}
		break;


	default:
		return ;
	}
}

void CRangeWnd::SetTips(int x,int y)
{
	CParserTip *pTip = g_pControl->GetTipWnd();
	if(pTip == NULL)
		return;

	int iMerLevel = 0;
	pTip->Clear();
	char str[256]={0};
	std::string strName;

	int n = m_pGrid->GetHotLine();
	if (m_pGrid->GetScrollPos() > 0)
		n--;
	if(n>=0 && mapRangeData!=NULL)
	{
		RANGE_DADA::iterator itr;
		if((itr=mapRangeData->find(n+1)) == mapRangeData->end())
		{
			pTip->SetShow(false);
			return;
		}
		void *RangeData = ((STRUCT_RANGE_DADA*)(itr->second))->vData;
		if(!RangeData)
			return;

		int ix = m_iScreenX + x + 4;
		int iy = m_iScreenY + y + 20;
		pTip->AdjustXY(ix,iy);
		pTip->Move(ix,iy);
		pTip->SetShow(true);
		switch(g_Range[m_CurrentType])
		{
		case 1:	

			strName.assign(((ALL_RANGE*)RangeData)->name,MYCHAR_LEN);
			sprintf(str,"%s %d级",strName.c_str(),((ALL_RANGE*)RangeData)->level);

			strcat(str,GetGenderName(((ALL_RANGE*)RangeData)->sex));
			strcat(str,GetJobName(((ALL_RANGE*)RangeData)->work));
			pTip->AddText(str,0xFFFFFF00,-1);
			strName.assign(((ALL_RANGE*)RangeData)->guild,GUILDNAME_LEN);
			if(strcmp(strName.c_str(),"")!=0)
			{
				sprintf(str,"所属行会:%s",strName.c_str());
				pTip->AddText(str,0xFFFFFFFF,-1);
			}
			strName.clear();

			/*iMerLevel = ((ALL_RANGE*)RangeData)->merlevel;

			if(iMerLevel > 0 && iMerLevel<100)
			{
				if(((ALL_RANGE*)RangeData)->merwork>=0&&((ALL_RANGE*)RangeData)->merwork<3)
				{
					strName.assign(GetJobName(((ALL_RANGE*)RangeData)->merwork));
				}

				if(((ALL_RANGE*)RangeData)->inmerlevel > 0)
				{
					sprintf(str,"外现元神为:%s %d级",strName.c_str(),((ALL_RANGE*)RangeData)->merlevel);
					pTip->AddText(str,0xFFFFFFFF);
					strName.assign(GetJobName(((ALL_RANGE*)RangeData)->inmerwork));
					sprintf(str,"内敛元神为:%s %d级",strName.c_str(),((ALL_RANGE*)RangeData)->inmerlevel);
					pTip->AddText(str,0xFFFFFFFF);

				}
				else
				{
					sprintf(str,"元神分身为:%s %d级",strName.c_str(),((ALL_RANGE*)RangeData)->merlevel);
					pTip->AddText(str,0xFFFFFFFF);
				}

			}*/

			break;
		case 2:
			strName.assign(((TEACHER_RANGE*)RangeData)->name,MYCHAR_LEN);

			sprintf(str,"%s %d级",strName.c_str(),((TEACHER_RANGE*)RangeData)->level);

			strcat(str,GetGenderName(((TEACHER_RANGE*)RangeData)->sex));
			strcat(str,GetJobName(((TEACHER_RANGE*)RangeData)->work));

			pTip->AddText(str,0xFFFFFF00,-1);
			strName.assign(((TEACHER_RANGE*)RangeData)->guild,GUILDNAME_LEN);
			if(!strName.empty())
			{
				sprintf(str,"所属行会:%s",strName.c_str());
				pTip->AddText(str,0xFFFFFFFF,-1);
			}

			break;
		case 6:
			/*if(((GUILD_RANGE*)RangeData)->towerLevel > 0)
			{
				sprintf(str,"行会宝塔等级： %d级",((GUILD_RANGE*)RangeData)->towerLevel);
				pTip->AddText(str);
			}
			else*/
			{
				pTip->SetShow(false);
			}
			break;
		case 14:
			strName.assign(((PRODUCE_SKILL_RANGE*)RangeData)->cPlayerName,MYCHAR_LEN);
			sprintf(str,"%s %d级",strName.c_str(),((PRODUCE_SKILL_RANGE*)RangeData)->byLevel);

			strcat(str,GetGenderName(((PRODUCE_SKILL_RANGE*)RangeData)->bySex));
			strcat(str,GetJobName(((PRODUCE_SKILL_RANGE*)RangeData)->byWork));
			pTip->AddText(str,0xFFFFFF00,-1);
			strName.assign(((PRODUCE_SKILL_RANGE*)RangeData)->cGuild,GUILDNAME_LEN);
			if(strcmp(strName.c_str(),"")!=0)
			{
				sprintf(str,"所属行会:%s",strName.c_str());
				pTip->AddText(str,0xFFFFFFFF,-1);
			}
			strName.clear();
			break;
		case 15:
			strName.assign(((RTS_RANGE*)RangeData)->cPlayerName,MYCHAR_LEN);
			sprintf(str,"%s %d级",strName.c_str(),((RTS_RANGE*)RangeData)->byLevel);

			strcat(str,GetGenderName(((RTS_RANGE*)RangeData)->bySex));
			strcat(str,GetJobName(((RTS_RANGE*)RangeData)->byWork));
			pTip->AddText(str,0xFFFFFF00,-1);			
			strName.clear();
			break;
		default:
			pTip->SetShow(false);
			return;
		}

	}
	else
	{
		pTip->SetShow(false);
	}

}

void CRangeWnd::DrawMyInfo()
{
	m_nMyPlace = 0;
	if(m_pMyMapRangeData == NULL || m_pMyMapRangeData->find(0) == m_pMyMapRangeData->end())
	{
		return;
	}
	void * RangeData;
	RangeData = (((STRUCT_RANGE_DADA *)(*m_pMyMapRangeData)[0]))->vData;
	if(!RangeData) return;
	WORD iplace =  *(WORD*)(RangeData);
	if(iplace==0xffff) //沙城行会
	{
		//g_pGfx->DrawTextureNL(m_iScreenX+124,m_iScreenY+270,m_pSBKText);
		g_pFont->DrawText(m_iScreenX+130,m_iScreenY+277,"您的行会为沙城行会",0xffb46428,FONT_YAHEI,12,DTF_BlackFrame);
		return;
	}
	else if(iplace==0)
	{
		g_pFont->DrawText(m_iScreenX+130,m_iScreenY+277,"您未进入此排行榜",0xffb46428,FONT_YAHEI,12,DTF_BlackFrame);
		return;
	}
	if(g_Range[m_CurrentType]==4) // 所有宠物的Info图片
	{
		g_pFont->DrawText(m_iScreenX + 150,m_iScreenY + 270,"您的宠物为     级        排名第     名       最近排名",0xffb46428,FONT_YAHEI,12,DTF_BlackFrame);
	}
	else if(g_Range[m_CurrentType]==1 && m_CurrentSmallType == 0) //总等级排名
	{
		g_pFont->DrawText(m_iScreenX + 132,m_iScreenY + 269,"您当前等级    级      排名第      名       昨日排名        最近排名",0xffb46428,FONT_YAHEI,12,DTF_BlackFrame);
	}
	else
	{
		if(m_BottomText.find(g_Range[m_CurrentType])!=m_BottomText.end())			
			g_pFont->DrawText(m_iScreenX+142,m_iScreenY+270,m_BottomText[g_Range[m_CurrentType]].c_str(),0xffb46428,FONT_YAHEI,12,DTF_BlackFrame);
	}

	char str[31]={0};
	int type = (g_Range[m_CurrentType]<<8) + m_CurrentSmallType;
	string strInfo;

	DWORD dwTexID =0;

	if(m_mapMyRangeQushi[type] == 0)
		dwTexID =  MAKELONG(10821,PACKAGE_INTERFACE);
	else if(m_mapMyRangeQushi[type] == 1)
		dwTexID = MAKELONG(10822,PACKAGE_INTERFACE);
	else if(m_mapMyRangeQushi[type] == -1)
		dwTexID = MAKELONG(10823,PACKAGE_INTERFACE);

	LPTexture pTexTemp = g_pTexMgr->GetTexFromID(dwTexID,EP_UI);
	int x = 0;

	switch(g_Range[m_CurrentType])
	{
	case 1:	

		if(type == 0x0100)
		{
			sprintf(str,"%d",((ALL_RANGE*)RangeData)->level);
			g_pFont->DrawText(m_iScreenX+205,m_iScreenY+268,str,0xFF960404,FONT_DEFAULT,FONTSIZE_MIDDLE,DTF_Center);

			sprintf(str,"%d",((ALL_RANGE*)RangeData)->place);
			g_pFont->DrawText(m_iScreenX+302+17,m_iScreenY+268,str,0xFF960404,FONT_DEFAULT,FONTSIZE_MIDDLE,DTF_Center);

			if(m_HesternalPlace.find(type)!=m_HesternalPlace.end())
			{
				sprintf(str,"%d",m_HesternalPlace[type]);
				g_pFont->DrawText(m_iScreenX+432+19,m_iScreenY+268,str,0xFF960404,FONT_DEFAULT,FONTSIZE_MIDDLE,DTF_Center);
			}

			if(pTexTemp)
				g_pGfx->DrawTextureNL(m_iScreenX+536,m_iScreenY+267,pTexTemp);
		}
		else
		{
			sprintf(str,"%d",((ALL_RANGE*)RangeData)->level);
			g_pFont->DrawText(m_iScreenX+213,m_iScreenY+268,str,0xFF960404,FONT_DEFAULT,FONTSIZE_MIDDLE,DTF_Center);

			sprintf(str,"%d",((ALL_RANGE*)RangeData)->place);
			g_pFont->DrawText(m_iScreenX+316+19,m_iScreenY+268,str,0xFF960404,FONT_DEFAULT,FONTSIZE_MIDDLE,DTF_Center);

			if(pTexTemp)
				g_pGfx->DrawTextureNL(m_iScreenX+455,m_iScreenY+268,pTexTemp);
		}
		m_nMyPlace = ((ALL_RANGE*)RangeData)->place;
		break;
	case 2:	

		sprintf(str,"%d",((TEACHER_RANGE*)RangeData)->tudiNum);
		g_pFont->DrawText(m_iScreenX + 190,m_iScreenY + 268,str,0xFF960404,FONT_DEFAULT,FONTSIZE_MIDDLE,DTF_Center);
		sprintf(str,"%d",((TEACHER_RANGE*)RangeData)->place);
		g_pFont->DrawText(m_iScreenX + 346,m_iScreenY + 268,str,0xFF960404,FONT_DEFAULT,FONTSIZE_MIDDLE,DTF_Center);

		if(pTexTemp)
			g_pGfx->DrawTextureNL(m_iScreenX + 460,m_iScreenY + 268,pTexTemp);

		m_nMyPlace = ((TEACHER_RANGE*)RangeData)->place;
		break;
	case 4:	
		{
			int iLevel = ((PET_RANGE*)RangeData)->level;
			if(iLevel <= 15)
			{
				iLevel %= 8;
			}
			else
			{
				iLevel -= 16;			
			}
			sprintf(str,"%d",iLevel);
			g_pFont->DrawText(m_iScreenX+214+13,m_iScreenY+268,str,0xFF960404,FONT_DEFAULT,FONTSIZE_MIDDLE,DTF_Center);
			sprintf(str,"%d",((PET_RANGE*)RangeData)->place);
			g_pFont->DrawText(m_iScreenX+330+19,m_iScreenY+268,str,0xFF960404,FONT_DEFAULT,FONTSIZE_MIDDLE,DTF_Center);
			if(pTexTemp)
				g_pGfx->DrawTextureNL(m_iScreenX+470,m_iScreenY+268,pTexTemp);

			m_nMyPlace = ((PET_RANGE*)RangeData)->place;
		}
		break;
	case 5:	

		sprintf(str,"%d",((ZHUZAO_RANGE*)RangeData)->exp);
		g_pFont->DrawText(m_iScreenX+234 ,m_iScreenY+268,str,0xFF960404,FONT_DEFAULT,FONTSIZE_MIDDLE);
		sprintf(str,"%d",((ZHUZAO_RANGE*)RangeData)->place);
		g_pFont->DrawText(m_iScreenX+348+19,m_iScreenY+268,str,0xFF960404,FONT_DEFAULT,FONTSIZE_MIDDLE,DTF_Center);
		if(pTexTemp)
			g_pGfx->DrawTextureNL(m_iScreenX+489,m_iScreenY+268,pTexTemp);

		m_nMyPlace = ((ZHUZAO_RANGE*)RangeData)->place;

		break;
	case 6:	
		memcpy(str,((GUILD_RANGE*)RangeData)->name,GUILDNAME_LEN);		
		g_pFont->DrawText(m_iScreenX + 220 ,m_iScreenY + 272,str,0xFF960404);
		sprintf(str,"%d",((GUILD_RANGE*)RangeData)->place);
		g_pFont->DrawText(m_iScreenX + 420,m_iScreenY + 269,str,0xFF960404,FONT_DEFAULT,FONTSIZE_MIDDLE,DTF_Center);
		if(pTexTemp)
			g_pGfx->DrawTextureNL(m_iScreenX + 590,m_iScreenY + 268,pTexTemp);

		m_nMyPlace = ((GUILD_RANGE*)RangeData)->place;

		break;
	case 7:	

		sprintf(str,"%d",((MIEMO_RANGE*)RangeData)->cishu);
		g_pFont->DrawText(m_iScreenX+220 ,m_iScreenY+268,str,0xFF960404,FONT_DEFAULT,FONTSIZE_MIDDLE);
		sprintf(str,"%d",((MIEMO_RANGE*)RangeData)->place);
		g_pFont->DrawText(m_iScreenX+316+19 ,m_iScreenY+268,str,0xFF960404,FONT_DEFAULT,FONTSIZE_MIDDLE,DTF_Center);
		if(pTexTemp)
			g_pGfx->DrawTextureNL(m_iScreenX+455,m_iScreenY+268,pTexTemp);

		m_nMyPlace = ((MIEMO_RANGE*)RangeData)->place;

		break;
	case 8:	

		sprintf(str,"%d",((ARENA_RANGE*)RangeData)->exp);
		g_pFont->DrawText(m_iScreenX+234 ,m_iScreenY+268,str,0xFF960404,FONT_DEFAULT,FONTSIZE_MIDDLE);
		sprintf(str,"%d",((ARENA_RANGE*)RangeData)->place);
		g_pFont->DrawText(m_iScreenX+342+20 ,m_iScreenY+268,str,0xFF960404,FONT_DEFAULT,FONTSIZE_MIDDLE,DTF_Center);
		if(pTexTemp)
			g_pGfx->DrawTextureNL(m_iScreenX+482,m_iScreenY+268,pTexTemp);

		m_nMyPlace = ((ARENA_RANGE*)RangeData)->place;

		break;
	case 9:	

		sprintf(str,"%u",((RS_RANGE*)RangeData)->haoshi);
		g_pFont->DrawText(m_iScreenX+227+24,m_iScreenY+268,str,0xFF960404,FONT_DEFAULT,FONTSIZE_MIDDLE,DTF_Center);
		sprintf(str,"%d",((RS_RANGE*)RangeData)->place);
		g_pFont->DrawText(m_iScreenX+378+20 ,m_iScreenY+268,str,0xFF960404,FONT_DEFAULT,FONTSIZE_MIDDLE,DTF_Center);
		if(pTexTemp)
			g_pGfx->DrawTextureNL(m_iScreenX+518,m_iScreenY+268,pTexTemp);

		m_nMyPlace = ((RS_RANGE*)RangeData)->place;

		break;
	case 10:

		sprintf(str,"%d",((WUGUAN_RANGE*)RangeData)->exp);
		g_pFont->DrawText(m_iScreenX+232 ,m_iScreenY+268,str,0xFF960404,FONT_DEFAULT,FONTSIZE_MIDDLE);
		sprintf(str,"%d",((WUGUAN_RANGE*)RangeData)->place);
		g_pFont->DrawText(m_iScreenX+336+20,m_iScreenY+268,str,0xFF960404,FONT_DEFAULT,FONTSIZE_MIDDLE,DTF_Center);
		if(pTexTemp)
			g_pGfx->DrawTextureNL(m_iScreenX+476,m_iScreenY+268,pTexTemp);

		m_nMyPlace = ((WUGUAN_RANGE*)RangeData)->place;

		break;
	case 13:	
		g_pFont->DrawText(m_iScreenX+202,m_iScreenY+268,((PHYLE_RANGE*)RangeData)->name,0xFF960404,FONT_DEFAULT,FONTSIZE_MIDDLE);
		sprintf(str,"%d",((PHYLE_RANGE*)RangeData)->place);
		g_pFont->DrawText(m_iScreenX+365 ,m_iScreenY+268,str,0xFF960404,FONT_DEFAULT,FONTSIZE_MIDDLE);
		if(pTexTemp)
			g_pGfx->DrawTextureNL(m_iScreenX+490,m_iScreenY+268,pTexTemp);
		m_nMyPlace = ((PHYLE_RANGE*)RangeData)->place;
		break;
	case 14:
		if(((PRODUCE_SKILL_RANGE*)RangeData)->bySkillLevel >= 0 && ((PRODUCE_SKILL_RANGE*)RangeData)->bySkillLevel <= 7)
		{
			g_pFont->DrawText(m_iScreenX + 230,m_iScreenY + 270,szSkillLevel[((PRODUCE_SKILL_RANGE*)RangeData)->bySkillLevel],0xFF960404,FONT_DEFAULT,FONTSIZE_SMALL);
		}
		
		sprintf(str,"%d",((PRODUCE_SKILL_RANGE*)RangeData)->place);
		g_pFont->DrawText(m_iScreenX + 365,m_iScreenY + 268,str,0xFF960404,FONT_DEFAULT,FONTSIZE_MIDDLE);
		if(pTexTemp)
			g_pGfx->DrawTextureNL(m_iScreenX + 490,m_iScreenY + 268,pTexTemp);
		m_nMyPlace = ((PRODUCE_SKILL_RANGE*)RangeData)->place;
		break;
	case 15:
		sprintf(str,"%d",((RTS_RANGE*)RangeData)->iWinNum);
		g_pFont->DrawText(m_iScreenX + 250,m_iScreenY + 268,str,0xFF960404,FONT_DEFAULT,FONTSIZE_MIDDLE);
		sprintf(str,"%d",((RTS_RANGE*)RangeData)->place);
		g_pFont->DrawText(m_iScreenX + 365,m_iScreenY + 268,str,0xFF960404,FONT_DEFAULT,FONTSIZE_MIDDLE);
		if(pTexTemp)
			g_pGfx->DrawTextureNL(m_iScreenX + 490,m_iScreenY + 268,pTexTemp);
		m_nMyPlace = ((RTS_RANGE*)RangeData)->place;
		break;
	case 16:
		{
			char * p1[9] = 
			{
				"下阶青火境","中阶青火境","上阶青火境",
				"下阶石火境","中阶石火境","上阶石火境",
				"下阶天火境","中阶天火境","上阶天火境",
			};

			int iFireLevel = ((RTS_FIRE*)RangeData)->wSamadhiFireLevel;
			if (iFireLevel > 0 && iFireLevel <= 81)
			{
				sprintf(str, "%d级%s",iFireLevel,p1[(iFireLevel + 8) / 9 - 1]);
				g_pFont->DrawText(m_iScreenX+234 ,m_iScreenY+268,str,0xFF960404,FONT_DEFAULT,FONTSIZE_MIDDLE);
			}

			sprintf(str,"%d",((RTS_FIRE*)RangeData)->place);
			g_pFont->DrawText(m_iScreenX+342+35 ,m_iScreenY+268,str,0xFF960404,FONT_DEFAULT,FONTSIZE_MIDDLE,DTF_Center);
			if(pTexTemp)
				g_pGfx->DrawTextureNL(m_iScreenX+520,m_iScreenY+268,pTexTemp);

			m_nMyPlace = ((RTS_FIRE*)RangeData)->place;
		}
		break;
	case 17:
		{
			int wPinnaLevel = ((PINNA_RANGE*)RangeData)->wPinnaLevel;
			sprintf(str, "%s%d级",g_AIGoodMgr.GetWingNameByLevel(wPinnaLevel).c_str(),wPinnaLevel);
			g_pFont->DrawText(m_iScreenX+234 ,m_iScreenY+268,str,0xFF960404,FONT_DEFAULT,FONTSIZE_MIDDLE);

			sprintf(str,"%d",((PINNA_RANGE*)RangeData)->place);
			g_pFont->DrawText(m_iScreenX+342+35 ,m_iScreenY+268,str,0xFF960404,FONT_DEFAULT,FONTSIZE_MIDDLE,DTF_Center);
			if(pTexTemp)
				g_pGfx->DrawTextureNL(m_iScreenX+520,m_iScreenY+268,pTexTemp);

			m_nMyPlace = ((PINNA_RANGE*)RangeData)->place;
		}
		break;
	case 18:
		{
			sprintf(str, "%d级", ((HORSEMANSHIP_RANGE*)RangeData)->wHorsemanshipLevel);
			g_pFont->DrawText(m_iScreenX+234 ,m_iScreenY+268,str,0xFF960404,FONT_DEFAULT,FONTSIZE_MIDDLE);

			sprintf(str,"%d",((HORSEMANSHIP_RANGE*)RangeData)->place);
			g_pFont->DrawText(m_iScreenX+342+35 ,m_iScreenY+268,str,0xFF960404,FONT_DEFAULT,FONTSIZE_MIDDLE,DTF_Center);
			if(pTexTemp)
				g_pGfx->DrawTextureNL(m_iScreenX+520,m_iScreenY+268,pTexTemp);

			m_nMyPlace = ((HORSEMANSHIP_RANGE*)RangeData)->place;
		}
		break;

	default:
		return;
	}


	//tip提示
	if(m_pTipMyInfo.IsShow())
	{
		m_pTipMyInfo.Draw();
		g_pGfx->SetRenderMode(RM_ADD1);
		g_pGfx->DrawTextureNL(GetScreenX()+124,GetScreenY()+236,g_pTexMgr->GetTex(PACKAGE_INTERFACE,10818,EP_UI));
		g_pGfx->SetRenderMode();
	}
}

void CRangeWnd::DrawWaitReading()
{
	if(m_dwTickCount - m_LastSendTick > 20000) //20秒显示繁忙
	{
		g_pFont->DrawText(260+m_iScreenX ,154+m_iScreenY,"查询繁忙,请稍候再试",0xFFFFFFFF,FONT_DEFAULT,FONTSIZE_MIDDLE,DTF_BlackFrame);
		return;
	}

	string str;

	if(m_nWaitReading >= 0 && m_nWaitReading<20)
	{
		str = "数据读取中";
		m_nWaitReading++;
	}
	else if(m_nWaitReading >= 20 && m_nWaitReading<40)
	{
		str = "数据读取中。。";
		m_nWaitReading++;
	}
	else if(m_nWaitReading >= 40 && m_nWaitReading<60)
	{
		str = "数据读取中。。。。";
		m_nWaitReading++;
	}
	else if(m_nWaitReading >= 60 && m_nWaitReading<80)
	{
		str = "数据读取中。。。。。";
		m_nWaitReading++;
	}
	else if(m_nWaitReading>=80)
	{
		str = "数据读取中";
		m_nWaitReading = 0;
	}

	g_pFont->DrawText(260+m_iScreenX ,154+m_iScreenY,str.c_str(),0xFFFFFFFF,FONT_DEFAULT,FONTSIZE_MIDDLE,DTF_BlackFrame);
}

void CRangeWnd::DrawHelp()
{

	char temp[20]={0};
	sprintf(temp,"%d",g_Range[m_CurrentType]);
	vector<string>* pDesc = g_RangeXmlMgr.GetRangeDesc(temp);
	int height=45+36+m_iScreenY;
	if(pDesc)
	{
		int position = 0;
		string str,tempstr="";

		for(UINT i=0;i<pDesc->size();i++)
		{
			str =pDesc->at(i);
			position = str.find("<FIRSTNAME=");
			if(position!=-1)
			{
				position =  str.find(">");
				tempstr.assign(str,11,position-11);
				g_pFont->DrawText(109+186+m_iScreenX ,50+m_iScreenY,tempstr.c_str(),0xffffff00,FONT_DEFAULT,FONTSIZE_MIDDLE);
				continue;
			}
			position = str.find("<TITLE=");
			if(position!=-1)
			{
				position =  str.find(">");
				tempstr.assign(str,7,position-7);
				g_pFont->DrawText(109+26+m_iScreenX ,height,tempstr.c_str(),COLOR_TEXT_MAIN_TITLE);
				height += 16;
				continue;
			}
			g_pFont->DrawText(109+26+m_iScreenX ,height,(pDesc->at(i)).c_str(),COLOR_TEXT_NORMAL);
			height += 16;
		}
	}

}

void CRangeWnd::FindMyPlace()
{
	if(m_nMyPlace<=0) 
		return;

	m_pGrid->SetDisLine(m_nMyPlace-1);
}

void CRangeWnd::GotoPlace(int place)
{
	if(g_Range[m_CurrentType]==1 && m_CurrentSmallType == 0) //总等级排名
	{
		m_pGrid->SetDisLine(place - 1);
	}
	else
	{
		m_CurrentType = 0;
		m_CurrentSmallType=0;

		SetTexByType();
		SetSmallType();
		Initcolumn();

		map<WORD,WORD>::iterator itr;
		if((itr = m_mapTotalNum.find(0x0100))!=m_mapTotalNum.end())
		{
			m_pGrid->SetTotalCount(itr->second);
		}

		m_pGrid->SetDisLine(place - 1);
	}
}

void CRangeWnd::SendServerExpUp()
{

	ALL_RANGE_DADA::iterator big_itr;
	RANGE_DADA::iterator range_itr;
	int type = (g_Range[m_CurrentType]<<8) + m_CurrentSmallType;

	if(!m_bSendUp)
	{
		//先获得自己排名数据
		if( (big_itr=m_mapMyRangeData.find(0x0100))!=m_mapMyRangeData.end())
		{
			if((range_itr=big_itr->second->find(0))!=big_itr->second->end())
			{
				STRUCT_RANGE_DADA* pRange = ((STRUCT_RANGE_DADA *)(range_itr->second));
				if(pRange != NULL && ((ALL_RANGE*)(pRange->vData))->place > 0)
				{
					double fBefore;
					int iFinish;
					fBefore = ((double)SELF.GetExp() / SELF.GetLevelUpExp());
					iFinish = ((ALL_RANGE*)(pRange->vData))->exp ;
					if((int)(fBefore * 100) - iFinish >= 1)
					{
						g_pGameControl->Send_Range_Sign_Up(SELF.GetID(),1);
						m_bSendUp = true;
					}
				}
			}
		}
	}
}
