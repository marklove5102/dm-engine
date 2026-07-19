#include "HuanHuaWnd.h"
#include "GameData/NpcData.h"
#include "GameData/GameData.h"
#include "BaseClass/Control/GoodGrid.h"
#include "GameControl/GameControl.h"
#include "Global/Interface/AudioInterface.h"
#include "GameData\TalkMgr.h"
#include "GameData\MsgBoxMgr.h"
#include "GameData\PetData.h"
#include "BaseClass\Control\ParserTip.h"
#include "GameData\GameGlobal.h"

INIT_WND_POSX(CHuanHuaWnd,POS_AUTO,POS_AUTO)
CHuanHuaWnd::CHuanHuaWnd(void)
{
    m_iIndex = 15845;

    m_pOkButton = NULL;
    m_pCancelButton = NULL;
    g_NPC.GetPetMergeData().Seal.SetID(0);
    g_NPC.GetPetMergeData().FirstPet.SetID(0);
    g_NPC.GetPetMergeData().SecondPet.SetID(0);
    m_bClickOKButton = false;
	m_bPetPicFlash = false;
	m_bSendHuanHua = false;
	m_bPlayEffect = false;
	m_bGradualDisappear = false;

	m_bMouseOnFirstPet = false;
	m_bMouseOnSecondPet = false;

	m_iFrameCount = 0;

	g_NPC.GetPetMergeData().byTestResult = 0xff;
	g_NPC.GetPetMergeData().byMergeResult = 0xff;
	g_NPC.GetPetMergeData().iLingLi = 0;
	g_NPC.GetPetMergeData().iMoney = 0;
	g_NPC.GetPetMergeData().result.clear();

    m_recGrids[0].left = 211;
    m_recGrids[0].top = 263;
    m_recGrids[0].right = 255;
    m_recGrids[0].bottom = 315;

    m_recGrids[1].left = 371;
    m_recGrids[1].top = 264;
    m_recGrids[1].right = 417;
    m_recGrids[1].bottom = 315;

    m_recGrids[2].left = 529;
    m_recGrids[2].top = 263;
    m_recGrids[2].right = 576;
    m_recGrids[2].bottom = 315;
}

CHuanHuaWnd::~CHuanHuaWnd(void)
{
    if(g_NPC.GetPetMergeData().FirstPet.GetID() != 0)
    {
        SELF.PackageGood().BackToArray(g_NPC.GetPetMergeData().FirstPet);
        g_NPC.GetPetMergeData().FirstPet.SetID(0);
    }

    if(!m_bSendHuanHua || (m_bSendHuanHua && g_NPC.GetPetMergeData().byMergeResult != 0))
    {
        if(g_NPC.GetPetMergeData().Seal.GetID() != 0)
        {
            SELF.PackageGood().BackToArray(g_NPC.GetPetMergeData().Seal);
            g_NPC.GetPetMergeData().Seal.SetID(0);
        }        

        if(g_NPC.GetPetMergeData().SecondPet.GetID() != 0)
        {
            SELF.PackageGood().BackToArray(g_NPC.GetPetMergeData().SecondPet);
            g_NPC.GetPetMergeData().SecondPet.SetID(0);
        }
    }
}

void CHuanHuaWnd::Draw()
{
    CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

    
    CGoodGrid::DrawOneGood(m_iScreenX + m_recGrids[1].left + 22,m_iScreenY + m_recGrids[1].top + 25,g_NPC.GetPetMergeData().Seal);
    
    LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15858,EP_UI);
    
    if(g_NPC.GetPetMergeData().FirstPet.GetID() == 0 && pTex)
    {
        g_pGfx->DrawTextureNL(m_iScreenX + m_recGrids[0].left,m_iScreenY + m_recGrids[0].top,pTex);
    }
    else
    {
        DrawPetPic(g_NPC.GetPetMergeData().FirstPet,m_iScreenX + m_recGrids[0].left - 125,m_iScreenY + m_recGrids[0].top - 115,true);
    }

    if(g_NPC.GetPetMergeData().SecondPet.GetID() == 0 && pTex)
    {
		g_pGfx->DrawTextureNL(m_iScreenX + m_recGrids[2].left,m_iScreenY + m_recGrids[2].top,pTex);
		if(m_bGradualDisappear) 
		{
			if(m_iFrameCount < 255)
			{
				DrawPetPic(m_DisappearPet,m_iScreenX + m_recGrids[2].left - 125,m_iScreenY + m_recGrids[2].top - 115,false,255 - (m_iFrameCount += 3));
			}
			else
			{
				m_DisappearPet.SetID(0);
				m_bGradualDisappear = false;
				m_iFrameCount = 0;
			}
		}
    }
	
    else
    {
        DrawPetPic(g_NPC.GetPetMergeData().SecondPet,m_iScreenX + m_recGrids[2].left - 125,m_iScreenY + m_recGrids[2].top - 115,false);
    }

	PlayEffect();

	//g_pFont->SetFont(FONT_GDI_LISHU);
	if(g_NPC.GetPetMergeData().iMoney != 0 || g_NPC.GetPetMergeData().iLingLi != 0)
	{
		char temp[64] = "";
		sprintf(temp,"此次幻化需要灵力:%d点，金币:%d万",g_NPC.GetPetMergeData().iLingLi,g_NPC.GetPetMergeData().iMoney);
		g_pFont->DrawText(m_iScreenX + 395,m_iScreenY + 447,temp,0xffffff00,FONT_DEFAULT,12,DTF_Shadow | DTF_Center);
	}

	if(g_NPC.GetPetMergeData().result.size() >= 40)
	{
		VString vecStr;
		CutByUnicode(g_NPC.GetPetMergeData().result.c_str(),vecStr,80);
		
		g_pFont->DrawText(m_iScreenX + 153,m_iScreenY + 460,vecStr[0].c_str(),0xffffff00,FONT_DEFAULT,12,DTF_Shadow);
		if(vecStr.size() > 1)
		{
			g_pFont->DrawText(m_iScreenX + 153,m_iScreenY + 473,vecStr[1].c_str(),0xffffff00,FONT_DEFAULT,12,DTF_Shadow);
		}
		
	}
	else if(g_NPC.GetPetMergeData().result.size() != 0)
	{
		g_pFont->DrawText(m_iScreenX + 395,m_iScreenY + 467,g_NPC.GetPetMergeData().result.c_str(),0xffffff00,FONT_DEFAULT,12,DTF_Shadow | DTF_Center);
	}

	//g_pFont->SetFont();
}

void CHuanHuaWnd::DrawPetPic(CGood& good,int iX,int iY,bool bIsFirstPet,byte byAlpha)
{
	if(good.GetID() != 0)
    {
		DWORD color = 0xffffffff & ((byAlpha << 24) | 0x00ffffff);	

		int id = 0;
		int iType = (good.GetSC() & 0x7);  //2为需要画盔甲
		LPTexture pArm = NULL;
		LPTexture pEff = NULL;

		if(good.GetSC() >= 24 && good.GetSC() <= 26)	//凤凰
		{
			if(iType == 0)
			{
				id = 17180;
			}
			else if(iType == 1 || iType == 2)
			{
				id = (good.GetDC2() >> 4) * 2 + 17182;

				if(iType == 2)
				{
					pEff = g_pTexMgr->GetTex(PACKAGE_INTERFACE,(good.GetDC2() >> 4) + 17190,EP_UI);
				}
			}
		}
		else
		{
			if(iType == 0)
			{			
				id = (good.GetSC() >> 3) * 2 + 15859;			
			}
			else if(iType == 1 || iType == 2)
			{
				id = (good.GetSC() >> 3) * 6 + (good.GetDC2() >> 4) * 2 + 15865;

				if(iType == 2)
				{
					pArm = g_pTexMgr->GetTex(PACKAGE_INTERFACE,(good.GetSC() >> 3) + 15883,EP_UI);
				}
			}
		}

		if(bIsFirstPet && m_bMouseOnFirstPet)
		{
			id++;
		}
		else if(!bIsFirstPet && m_bMouseOnSecondPet)
		{
			id++;
		}

		LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,id,EP_UI);
		if(pTex)
		{
			g_pGfx->DrawTextureNL(iX,iY,pTex,color);
		}       

		if(pArm)
		{
			g_pGfx->DrawTextureNL(iX + 117,iY + 137,pArm,color);
		}

		if(pEff)
		{
			g_pGfx->SetRenderMode(RM_ADD1);
			g_pGfx->DrawTextureNL(iX,iY,pEff,color);
			g_pGfx->SetRenderMode();
		}
    }
}

bool CHuanHuaWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
    switch ( dwMsg )
    {
	case MSG_CTRL_HUANHUA_WND:
		if(dwData == OPER_CUSTOM + 1)
		{
			StartHuanHua();
			return true;
		}
		break;
    case MSG_CTRL_BUTTON_CLICK:
        {
            if(pControl == m_pOkButton)
            {
                if(!m_bPlayEffect && g_NPC.GetPetMergeData().FirstPet.GetID() != 0 && g_NPC.GetPetMergeData().SecondPet.GetID() != 0 && g_NPC.GetPetMergeData().byTestResult == 0)
                {	
					if((g_NPC.GetPetMergeData().FirstPet.GetSC() & 0x7) < (g_NPC.GetPetMergeData().SecondPet.GetSC() & 0x7))	//副零售转生次数多
					{
						g_MsgBoxMgr.PopSimpleComfirm("此次幻化的副灵兽等级高于主灵兽，你确定要幻化吗？",MSG_CTRL_START_HUANHUA,0);
					}
					else if((g_NPC.GetPetMergeData().FirstPet.GetSC() & 0x7) == (g_NPC.GetPetMergeData().SecondPet.GetSC() & 0x7) && g_NPC.GetPetMergeData().FirstPet.GetDC() < g_NPC.GetPetMergeData().SecondPet.GetDC())				//转生次数一样副零售等级高
					{
						g_MsgBoxMgr.PopSimpleComfirm("此次幻化的副灵兽等级高于主灵兽，你确定要幻化吗？",MSG_CTRL_START_HUANHUA,0);
					}
					else
					{
						StartHuanHua();
					}
					return true;
                }				
            }	
            else if(pControl == m_pCancelButton)
            {
                g_pControl->PopupWindow(MSG_CTRL_HUANHUA_WND,OPER_CLOSE);
                return true;
            }
        }
    }

    return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CHuanHuaWnd::OnCreate()
{
    m_pOkButton = new CCtrlButton();
    AddControl(m_pOkButton);
    m_pOkButton->CreateEx(this,165,501,15849,15850,15851);

    m_pCancelButton = new CCtrlButton();
    AddControl(m_pCancelButton);
    m_pCancelButton->CreateEx(this,391,501,15852,15853,15854);

    SetMask(15845);

    m_pCloseButton = new CCtrlButton();
    AddControl(m_pCloseButton);
    m_pCloseButton->CreateEx(this,680,121,15846,15847,15848);
}

bool CHuanHuaWnd::IsCanHuanHua(CGood& good,int iGrid)
{
	bool bCanHuanHua = true;
	char temp[128] = {0};

	PET_SENDOUT_MAP& petMap = g_PetData.GetSendOutPetList();

	DWORD m = good.GetRecordTime();	//	死亡的灵兽不可幻化
	if( m != 0 )
	{
		int iDays = g_Timer.GetServerHours(m)/24;
		if(iDays >= 4)
		{
			bCanHuanHua = false;
			strcpy(temp,"死亡的灵兽不可幻化");
		}
	}

	if(bCanHuanHua)		//释放的副灵兽不可幻化
	{
		PET_SENDOUT_MAP::iterator petItr = petMap.find(good.GetID());
		if(petItr != petMap.end())
		{
			if((petItr->second).szReserved[0] != 0)	
			{
				bCanHuanHua = false;
				strcpy(temp,"已放出的灵兽不可进行幻化，请收回该灵兽");
			}
		}
	}

	if(iGrid == 0 && good.GetDuraMax() == 0 && bCanHuanHua)		//没有资质的灵兽不可幻化
	{
		bCanHuanHua = false;
		strcpy(temp,"没有资质的灵兽不可幻化");
	}

	if(iGrid == 0 && g_NPC.GetPetMergeData().SecondPet.GetID() != 0 && bCanHuanHua)	//	非同类型不可幻化
	{
		if((good.GetSC() >> 3) != (g_NPC.GetPetMergeData().SecondPet.GetSC() >> 3))
		{
			bCanHuanHua = false;
			strcpy(temp,"主灵兽和副灵兽非同类型不可幻化");
		}
	}
	else if(iGrid == 2 && g_NPC.GetPetMergeData().FirstPet.GetID() != 0 && bCanHuanHua)	//	非同类型不可幻化
	{
		if((good.GetSC() >> 3) != (g_NPC.GetPetMergeData().FirstPet.GetSC() >> 3))
		{
			bCanHuanHua = false;
			strcpy(temp,"主灵兽和副灵兽非同类型不可幻化");
		}
	}

	if(!bCanHuanHua)
	{
		SELF.PackageGood().BackToArray(good,good.GetPos());
		good.SetID(0);
		g_TalkMgr.PushSysTalk(temp);
		m_bClick = false;
		g_MsgBoxMgr.PopSimpleAlert(temp);
	}

	return bCanHuanHua;
}

bool CHuanHuaWnd::OnLeftButtonUp(int iX, int iY)
{
    if((iX > 141 && iX < 320 && iY > 200 && iY < 380)||(iX > 460 && iX < 630 && iY > 200 && iY < 380)
        ||(iX > m_recGrids[1].left && iX < m_recGrids[1].right && iY > m_recGrids[1].top && iY < m_recGrids[1].bottom))
    {
        if(g_pGameData->GetDropMoneyFrom() != EDMYF_NONE)		//鼠标上有钱
        {
            return true;
        }

        stGoodFrom& GoodFrom = CGoodGrid::GetDropGoodFrom();

        if(GoodFrom.DropGood.GetID() != 0 && GoodFrom.eFromWnd != Package_Wnd)	//只接受来自本体包裹栏的物品
        {
            return true;
        }

		CGood* pGood = NULL;
        int iGrid = 0;

        if(iX > 460 && iX < 630)
        {
            iGrid = 2;
            pGood = &(g_NPC.GetPetMergeData().SecondPet);
        }
        else if(iX > 141 && iX < 320)
		{
			iGrid = 0;
			pGood = &(g_NPC.GetPetMergeData().FirstPet);            
        }
        else
        {
			iGrid = 1;
			pGood = &(g_NPC.GetPetMergeData().Seal);
        }

        if(pGood->GetID() == 0 && GoodFrom.DropGood.GetID() == 0)	//鼠标上没东西并且喂食栏也没有东西
        {
            return true;
        }

		if(GoodFrom.DropGood.GetID() != 0 && pGood->GetID() == 0 && !m_bGradualDisappear && !m_bPlayEffect)	//鼠标上有东西喂食栏没有东西,放东西到喂食栏
        {
			if((iGrid == 1 && GoodFrom.DropGood.GetStdMode() == 49 && GoodFrom.DropGood.GetShape() == 55)
                || (iGrid != 1 && GoodFrom.DropGood.GetStdMode() == 49 && GoodFrom.DropGood.GetShape() == 51))
            {
				if(iGrid != 1)	//转生条件
				{
					if(SELF.GetLingLi() < 10 || SELF.GetGold() < 200000)
					{
						m_bClick = false;
						SELF.PackageGood().BackToArray(GoodFrom.DropGood,GoodFrom.iWndPos);
						GoodFrom.DropGood.SetID(0);						
						m_bClick = false;
						g_MsgBoxMgr.PopSimpleAlert("金币或灵力不足，双击灵皓石、元灵晶石能迅速提升灵力");
						return true;
					}

					if(!IsCanHuanHua(GoodFrom.DropGood,iGrid))
						return true;
				}

                *pGood = GoodFrom.DropGood;
                CGoodGrid::ClearGoodFrom();
				m_bSendHuanHua = false;
				m_bClick = false;

                if(g_NPC.GetPetMergeData().FirstPet.GetID() != 0 && g_NPC.GetPetMergeData().SecondPet.GetID() != 0)
                {					
                    g_pGameControl->SEND_PET_MERGE(0,g_NPC.GetPetMergeData().FirstPet.GetID(),g_NPC.GetPetMergeData().SecondPet.GetID(),g_NPC.GetPetMergeData().Seal.GetID());
                }
            }

            return true;
        }

        if(GoodFrom.DropGood.GetID() == 0 && pGood->GetID() != 0 && !m_bGradualDisappear && !m_bPlayEffect)	//鼠标上没东西喂食栏有东西，取出东西
        {
            GoodFrom.DropGood = *pGood;
            pGood->SetID(0);
            GoodFrom.eFromWnd = Package_Wnd;	//此时的iWndPos不知道
			
			if(g_NPC.GetPetMergeData().FirstPet.GetID() != 0 && g_NPC.GetPetMergeData().SecondPet.GetID() != 0)
			{	
				m_bClick = false;
				g_pGameControl->SEND_PET_MERGE(0,g_NPC.GetPetMergeData().FirstPet.GetID(),g_NPC.GetPetMergeData().SecondPet.GetID(),g_NPC.GetPetMergeData().Seal.GetID());
			}
			else
			{
				g_NPC.GetPetMergeData().byTestResult = 0xff;
				g_NPC.GetPetMergeData().byMergeResult = 0xff;
				g_NPC.GetPetMergeData().iLingLi = 0;
				g_NPC.GetPetMergeData().iMoney = 0;
				g_NPC.GetPetMergeData().result.clear();
			}

			m_bSendHuanHua = false;
            return true;
        }

        if(GoodFrom.DropGood.GetID() != 0 && pGood->GetID() != 0 && !m_bGradualDisappear && !m_bPlayEffect)	//鼠标上有东西并且喂食栏也有东西，交换
        {
            if((iGrid == 1 && GoodFrom.DropGood.GetStdMode() == 49 && GoodFrom.DropGood.GetShape() == 55)
                || (iGrid != 1 && GoodFrom.DropGood.GetStdMode() == 49 && GoodFrom.DropGood.GetShape() == 51))
            {
				if(iGrid != 1)	//转生条件
				{
					if(!IsCanHuanHua(GoodFrom.DropGood,iGrid))
						return true;
				}

                CGood temp = *pGood;
                *pGood = GoodFrom.DropGood;
                GoodFrom.DropGood = temp;
				m_bSendHuanHua = false;
                
                if(g_NPC.GetPetMergeData().FirstPet.GetID() != 0 && g_NPC.GetPetMergeData().SecondPet.GetID() != 0)
                {				
					m_bClick = false;
                    g_pGameControl->SEND_PET_MERGE(0,g_NPC.GetPetMergeData().FirstPet.GetID(),g_NPC.GetPetMergeData().SecondPet.GetID(),g_NPC.GetPetMergeData().Seal.GetID());
                }
            }

            return true;
        }
    }

    return CCtrlWindowX::OnLeftButtonUp(iX,iY);
}

void CHuanHuaWnd::PlayEffect()
{
	if(!m_bPlayEffect)
		return;

	LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15886,EP_UI);

	if(pTex)
	{
		pTex->EnableSysAnim(false);
		pTex->SetCurFrame(m_iFrameCount++ / 6);
		g_pGfx->SetRenderMode(RM_ADD1);
		g_pGfx->DrawTextureNL(m_iScreenX,m_iScreenY,pTex);
		g_pGfx->SetRenderMode();
	}
	
	if(m_iFrameCount >= 180)
	{
		m_DisappearPet = g_NPC.GetPetMergeData().SecondPet;
		m_bClick = false;
		g_pGameControl->SEND_PET_MERGE(1,g_NPC.GetPetMergeData().FirstPet.GetID(),g_NPC.GetPetMergeData().SecondPet.GetID(),g_NPC.GetPetMergeData().Seal.GetID());
		m_bSendHuanHua = true;
		m_bPlayEffect = false;	
		m_bGradualDisappear = true;
		m_iFrameCount = 0;
	}
}

bool CHuanHuaWnd::OnMouseMove(int iX,int iY)
{
	CParserTip* pTip = g_pControl->GetTipWnd();

	int x = m_iScreenX + iX + 10;
	int y = m_iScreenY + iY + 10;
	
	m_bMouseOnFirstPet = false;
	m_bMouseOnSecondPet = false;
	pTip->Clear();

	if(iX > 141 && iX < 320 && iY > 200 && iY < 380)
	{		
		if(g_NPC.GetPetMergeData().FirstPet.GetID() != 0)
		{
			pTip->Parse(g_NPC.GetPetMergeData().FirstPet);
		}
		else
		{
			pTip->AddText("请放入需要幻化的主灵兽石",0xffffff00);
		}
		pTip->AdjustXY(x,y);
		pTip->Move(x,y);
		pTip->SetShow(true);
		m_bMouseOnFirstPet = true;
		return true;
	}
	else if(iX > 460 && iX < 630 && iY > 200 && iY < 380)
	{
		if(g_NPC.GetPetMergeData().SecondPet.GetID() != 0)
		{
			pTip->Parse(g_NPC.GetPetMergeData().SecondPet);
		}
		else
		{
			pTip->AddText("请放入需要幻化的副灵兽石",0xffffff00);
		}
		pTip->AdjustXY(x,y);
		pTip->Move(x,y);
		pTip->SetShow(true);
		m_bMouseOnSecondPet = true;
		return true;
	}
	else if(iX > m_recGrids[1].left && iX < m_recGrids[1].right && iY > m_recGrids[1].top && iY < m_recGrids[1].bottom)
	{		
		pTip->AddText("可放入封元印提高合成概率",0xffffff00);
		pTip->AdjustXY(x,y);
		pTip->Move(x,y);
		pTip->SetShow(true);
		return true;
	}
	else
	{
		pTip->SetShow(false);		
	}



	return CCtrlWindowX::OnMouseMove(iX,iY);
}

void CHuanHuaWnd::StartHuanHua()
{
	m_bClickOKButton = true;                   
	m_bPlayEffect = true;
	g_pAudio->Play(EAT_MAGIC,2603,g_pAudio->GetRand()++);
}