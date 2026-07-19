#include "HeartKnackWnd.h"
#include "GameData/GameData.h"
#include "Global/GlobalMsg.h"
#include "Global/Interface/AudioInterface.h"
#include "Baseclass/Control/ParserTip.h"

INIT_WND_POSX(CHeartKnackWnd,POS_AUTO,POS_AUTO)

CHeartKnackWnd::CHeartKnackWnd(void)
{
	m_pHelpBtn = NULL;
	m_pCloseHelpBtn = NULL;
	m_bShowHelp = false;
	m_pMarkViewer = NULL;

	string str = "<color=orange 灵力系统的介绍：>\\";
	str += "1.<灵力值的累积/@@jilei>\\";
	str += "2.<灵力值的使用/@@shiyong>\\";
	str += "3.<灵力值的消耗/@@xiaohao>\\";
	
	m_npcText.Clear();
	m_npcText.Parse(str,0,"\\",str.length());

	m_iIndex = 14770;
	m_iPages = 1;
	
}

CHeartKnackWnd::~CHeartKnackWnd(void)
{
}

void CHeartKnackWnd::OnCreate()
{
	m_pLeftBtn = new CCtrlButton();
	AddControl(m_pLeftBtn);
	m_pLeftBtn->CreateEx(this,15,277,4948,4950,4949);
	
	m_pRightBtn = new CCtrlButton();
	AddControl(m_pRightBtn);
	m_pRightBtn->CreateEx(this,572,277,4945,4947,4946);

	m_pHelpBtn = new CCtrlButton();
	AddControl(m_pHelpBtn);
	m_pHelpBtn->CreateEx(this,565,365,14429,14430,14431);
	
	m_pCloseHelpBtn = new CCtrlButton();
	AddControl(m_pCloseHelpBtn);
	m_pCloseHelpBtn->CreateEx(this,714,7,259,257,258);
	m_pCloseHelpBtn->SetShow(false);

	for(int i = 0;i<4;i++)
	{
		m_pBtn[i] = new CCtrlButton();
		AddControl(m_pBtn[i]);
	}

	//数据显示框
	m_pMarkViewer = new CMarkViewer(16,9);
	AddControl(m_pMarkViewer);
	m_pMarkViewer->Create(this,616,18,110,144);
	m_pMarkViewer->SetTagText(&m_npcText);
	m_pMarkViewer->SetShow(false);

	SetCloseButton(573,7);
}

bool CHeartKnackWnd::OnLeftButtonUp(int iX,int iY)
{
	string strCommand = m_pMarkViewer->GetCommand();

	if(!strCommand.empty())
	{
		//up 音乐
		g_pAudio->Play(EAT_OTHER, 5, g_pAudio->GetRand()++);	

		if(strCommand.substr(0,2) == "@@")
		{
			if(strCommand == "@@jilei")//
			{
				string str = "<color=orange 灵力值的累积：>\\";
				str += "    随时到中州魔法\\阵祭祀（429，170）\\处投放七魄灵石可转\\化为灵力值（每2点\\灵石灵力可换取1点\\灵力值）。\\";	
				str += "\\<返回/@@Back>";

				m_npcText.Clear();
				m_npcText.Parse(str,0,"\\",str.length());
			}
			else if(strCommand == "@@shiyong")//
			{
				string str = "<color=orange 灵力值的使用：>\\";
				str += "    灵力值累积到一\\定阶段，就可以激活\\相应的属性加成，随\\着灵力值积累量的增\\加，激活的属性加成\\也越强。\\";
				str += "\\<返回/@@Back>";

				m_npcText.Clear();
				m_npcText.Parse(str,0,"\\",str.length());				
			}
			else if(strCommand == "@@xiaohao")// 
			{
				string str = "<color=orange 灵力值的消耗：>\\";
				str += "    当灵力值不足\\1000点时，灵力值不\\会衰减；超过1000点\\时，灵力值会随时间\\衰减，衰减程度与你\\当天获得的经验值有\\关。\\";
				str += "\\<返回/@@Back>";

				m_npcText.Clear();
				m_npcText.Parse(str,0,"\\",str.length());				
			}
			else if(strCommand == "@@Back")//返回
			{
				string str = "<color=orange 灵力系统的介绍：>\\";
				str += "1.<灵力值的累积/@@jilei>\\";
				str += "2.<灵力值的使用/@@shiyong>\\";
				str += "3.<灵力值的消耗/@@xiaohao>\\";

				m_npcText.Clear();
				m_npcText.Parse(str,0,"\\",str.length());
			}

			return true;
		}
	}
	return CCtrlWindowX::OnLeftButtonUp(iX,iY);
}

bool CHeartKnackWnd::OnMouseMove(int iX,int iY)
{
	CParserTip* pTip = g_pControl->GetTipWnd();
	int x = m_iScreenX + iX + 10;
	int y = m_iScreenY + iY + 10;
	pTip->SetShow(false);

	DWORD dwColor = 0xFFFFCF63;
	if(iX> 249 && iX < 324 && iY > 71 && iY < 102)
	{
		pTip->Clear();
		pTip->AddText("灵力值达到3000以上",dwColor);	
		pTip->AddText("本体命中+3",dwColor);
		pTip->AdjustXY(x,y);
		pTip->Move(x,y);
		pTip->SetShow(true);
		return true;
	}

	if(iX > 494 && iX < 569 && iY > 71 && iY < 102)
	{
		pTip->Clear();
		pTip->AddText("灵力值达到10000以上",dwColor);	
		pTip->AddText("本体幸运+1",dwColor);
		pTip->AdjustXY(x,y);
		pTip->Move(x,y);
		pTip->SetShow(true);
		return true;

	}

	if(iX > 148 && iX < 223 && iY > 238 && iY < 269)
	{
		pTip->Clear();
		pTip->AddText("灵力值达到1000以上",dwColor);	
		pTip->AddText("本体生命值增加30",dwColor);
		pTip->AdjustXY(x,y);
		pTip->Move(x,y);
		pTip->SetShow(true);
		return true;

	}

	if(iX > 364 && iX < 439 && iY > 238 && iY < 269)
	{
		pTip->Clear();
		pTip->AddText("灵力值达到6000以上",dwColor);	
		pTip->AddText("本体躲避+4",dwColor);
		pTip->AddText("本体魔法躲避+10%",dwColor);	
		pTip->AddText("本体毒物躲避+10%",dwColor);
		pTip->AdjustXY(x,y);
		pTip->Move(x,y);
		pTip->SetShow(true);
		return true;

	}

	if(iX > 103 && iX < 585 && iY > 150 && iY < 189)
	{
		char ctemp[100] = {0};
		sprintf(ctemp,"目前的灵力值：%d",SELF.GetLingLi());
		pTip->Clear();
		pTip->AddText(ctemp,dwColor);	
		pTip->AdjustXY(x,y);
		pTip->Move(x,y);
		pTip->SetShow(true);
		return true;

	}

	return CCtrlWindowX::OnMouseMove(iX,iY);
}

void CHeartKnackWnd::Draw()
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	DWORD dwValue = SELF.GetLingLi();
	
	int ix,iy,iright,ibottom;
 	ix = GetScreenX() + 79;
	iy = GetScreenY() + 73;
	ibottom = iy + 75;
	int iTopArrNum = 0,iBotArrNum = 0;
	if(dwValue < 1000)
	{
		iright = (int)(19 + ((float)(dwValue / 1000)) * 100.0);
	}
	else if(dwValue < 3000)
	{
		iBotArrNum = 1;
		iright = (int)(19 + 100 + 102 *((float)(dwValue - 1000))/2000.0);
	}
	else if(dwValue < 6000)
	{
		iBotArrNum = 1;
		iTopArrNum = 1;
		iright = (int)(19 + 204 + 113 *((float)(dwValue - 3000))/3000.0);
	}
	else if(dwValue < 10000)
	{
		iBotArrNum = 2;
		iTopArrNum = 1;
		iright = (int)(19 + 315 + 132 *((float)(dwValue - 6000))/4000.0);
	}
	else if(dwValue >= 10000)
	{
		iBotArrNum = 2;
		iTopArrNum = 2;
		if(dwValue > 11000) dwValue = 11000;
		iright = (int)(19 + 447 + 43 * ((float)(dwValue - 10000))/1000.0);
	}
	
	LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,14763,EP_UI);

	if(pTex)
	{
		g_pGfx->DrawPartTexture(ix - 4,iy + 60,pTex,0 ,0,iright,ibottom);
	}

	//绘制上下箭头
	LPTexture pTexUpArr1 = g_pTexMgr->GetTex(PACKAGE_INTERFACE,14769,EP_UI);
	LPTexture pTexUpArr2 = g_pTexMgr->GetTex(PACKAGE_INTERFACE,14772,EP_UI);
	LPTexture pTexDownArr1 = g_pTexMgr->GetTex(PACKAGE_INTERFACE,14768,EP_UI);
	LPTexture pTexDownArr2 = g_pTexMgr->GetTex(PACKAGE_INTERFACE,14771,EP_UI);
	if(iBotArrNum == 1 && pTexDownArr1)
	{
		g_pGfx->DrawTextureNL(ix + 72,iy + 116,pTexDownArr1);
	}
	else if(iBotArrNum == 2 && pTexDownArr1 && pTexDownArr2)
	{
		g_pGfx->DrawTextureNL(ix + 72,iy + 116,pTexDownArr1);
		g_pGfx->DrawTextureNL(ix + 288,iy + 116,pTexDownArr2);
	}

	if(iTopArrNum == 1 && pTexUpArr1)
	{
		g_pGfx->DrawTextureNL(ix + 175,iy - 14,pTexUpArr1);
	}
	else if(iTopArrNum == 2 && pTexUpArr1 && pTexUpArr2)
	{
		g_pGfx->DrawTextureNL(ix + 175,iy - 14,pTexUpArr1);
		g_pGfx->DrawTextureNL(ix + 420,iy - 13,pTexUpArr2);
	}

	//写字
	g_pFont->DrawText(ix + 69,iy + 168,"启灵心诀",0xFFFFCF63,FONT_LISHU);
	g_pFont->DrawText(ix + 170,iy + 2,"真灵心诀",0xFFFFCF63,FONT_LISHU);
	g_pFont->DrawText(ix + 285,iy + 168,"天灵心诀",0xFFFFCF63,FONT_LISHU);
	g_pFont->DrawText(ix + 416,iy + 2,"傲灵心诀",0xFFFFCF63,FONT_LISHU);

	//绘制特效
	pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,14775,EP_UI);
	if(pTex)
	{
		g_pGfx->SetRenderMode(RM_ADD2);
		g_pGfx->DrawTextureNL(ix + 90,iy,pTex);
		g_pGfx->SetRenderMode();
	}

	//绘制文本
	ix = GetScreenX() + 22;
	iy = GetScreenY() + 300;

	g_pFont->DrawText(ix,iy,"    修真之道在于炼气，以气为本，凝气结丹、炼丹成婴、化婴为神，从而达到修真的至高境界，所以引",0xFFFFCF63);
	iy += 17;
	g_pFont->DrawText(ix,iy,"天地灵气入体洗涤自身进而感悟天地造化可为修真法门之根本。",0xFFFFCF63);
	iy += 17;
	g_pFont->DrawText(ix,iy,"    当修真者步入更深一层境界时，这些蕴藏在体内的灵气将化为修真者的灵力，将通过不同的心决给修",0xFFFFCF63);
	iy += 17;
	g_pFont->DrawText(ix,iy,"真者本身带来莫大的帮助。",0xFFFFCF63);
	iy += 17;
	g_pFont->DrawText(ix,iy,"    同时修真者更需要及时的补充灵力，从而保证各种灵力心决的顺畅运作。",0xFFFFCF63);
	
	if(m_bShowHelp)
	{
		ShowHelp();
	}
}

void CHeartKnackWnd::ShowHelp()
{
	if(!m_bShowHelp)
		return;
	//底图
	LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,14776,EP_UI);
	g_pGfx->DrawTextureNL(m_iScreenX + 600, m_iScreenY, pTex);

	//重绘按钮
	m_pCloseHelpBtn->Draw();
}

bool CHeartKnackWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_BUTTON_CLICK:
		{
			if(pControl == m_pHelpBtn || pControl == m_pCloseHelpBtn)
			{
				m_bShowHelp = !m_bShowHelp;

				if(m_bShowHelp)
				{
					this->ReSize(m_iWidth + 141,m_iHeight);
					m_pCloseHelpBtn->SetEnable(true);
					m_pCloseHelpBtn->SetShow(true);
					m_pMarkViewer->SetEnable(true);
                    m_pMarkViewer->SetShow(true);
				}
				else
				{	
					this->ReSize(m_iWidth - 141,m_iHeight);
					m_pCloseHelpBtn->SetEnable(false);
					m_pCloseHelpBtn->SetShow(false);
					m_pMarkViewer->SetEnable(false);
                    m_pMarkViewer->SetShow(false);
				}
				return true;
			}
		}
	}

	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}