#include "petinfownd.h"
#include "GameData/GameData.h"
#include "GameControl/GameControl.h"
#include "GameData/GameGlobal.h"
#include "GameData/PetData.h"

INIT_WND_POSX(CPetInfoWnd,POS_VARIABLE,POS_VARIABLE)

CPetInfoWnd::CPetInfoWnd()
{
	m_iIndex = 11344;
	m_iPages = 1;

	m_iLineIndex = 0;

	m_iAlignType = XAL_TOPLEFT;
	m_iOffX = 250;
	m_iOffY = 100;

	g_pGameControl->SEND_Player_Pet_State(); //获得宠物的信息
}

bool CPetInfoWnd::Create(CControl* pParent,int iCurPage)
{
	m_pMainWnd = g_pControl;
	return CCtrlWindowX::Create(pParent,iCurPage);
}

CPetInfoWnd::~CPetInfoWnd(void)
{
}

void CPetInfoWnd::OnCreate()
{
	SetCloseButton(433,5, 80);

	m_pPriorPage = new CCtrlButton();
	AddControl(m_pPriorPage);
	m_pPriorPage->CreateEx(this,110,192,11330,11331,11332);

	m_pNextPage = new CCtrlButton();
	AddControl(m_pNextPage);
	m_pNextPage->CreateEx(this,186,192,11333,11334,11335);
}

void CPetInfoWnd::Draw(void)
{	
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	g_pFont->DrawText(193+m_iScreenX, 11+m_iScreenY, "操纵的宠物", 0xFFFAC896, FONT_YAHEI, FONTSIZE_BIG);

	g_pFont->DrawText(29+m_iScreenX, 50+m_iScreenY, "级 别       名 称          生 命      攻 击", 0xFFB46428, FONT_YAHEI, FONTSIZE_MIDDLE);
	g_pFont->DrawText(355+m_iScreenX, 50+m_iScreenY, "防 御", 0xFFB46428, FONT_YAHEI, FONTSIZE_MIDDLE);
	g_pFont->DrawText(400+m_iScreenX, 50+m_iScreenY, "魔 御", 0xFFB46428, FONT_YAHEI, FONTSIZE_MIDDLE);

	int iSize = g_PetData.GetPetStatus().size();
	for(size_t i = 0; i < MAX_DEMON_PET_NUM && i + m_iLineIndex < iSize; i++)
	{
		char temp[64] = {0};
		int y = m_iScreenY + 73 + i * 20;

		PET_STATUS pet = g_PetData.GetPetStatus().at(i + m_iLineIndex);

		sprintf(temp,"%d",pet.cPetLevel);
		g_pFont->DrawText(m_iScreenX + 42,y,temp,0xFFB48C5A, FONT_YAHEI);

		sprintf(temp,"%s",pet.cPetName);
		g_pFont->DrawText(m_iScreenX + 129 - strlen(pet.cPetName)*3,y,temp,0xFFB48C5A, FONT_YAHEI);

		sprintf(temp,"%d/%d",pet.wHp,pet.wHpMax);
		g_pFont->DrawText(m_iScreenX + 215,y,temp,0xFFB48C5A, FONT_YAHEI);

		sprintf(temp,"%d-%d",pet.cDc,pet.cDcMax);
		g_pFont->DrawText(m_iScreenX + 295,y,temp,0xFFB48C5A, FONT_YAHEI);

		sprintf(temp,"%d",pet.cAc);
		g_pFont->DrawText(m_iScreenX + 363,y,temp,0xFFB48C5A, FONT_YAHEI);

		sprintf(temp,"%d",pet.cAcMax);
		g_pFont->DrawText(m_iScreenX + 411,y,temp,0xFFB48C5A, FONT_YAHEI);
	}

	if(iSize <= MAX_DEMON_PET_NUM)
	{
		if(m_pPriorPage)
		{
			m_pPriorPage->SetShow(false);
			m_pPriorPage->SetEnable(false);
		}

		if(m_pNextPage)
		{
			m_pNextPage->SetShow(false);
			m_pNextPage->SetEnable(false);
		}
	}
	else
	{
		if(m_iLineIndex > 0)
		{
			m_pPriorPage->SetShow(true);
			m_pPriorPage->SetEnable(true);
		}
		else
		{
			m_pPriorPage->SetShow(false);
			m_pPriorPage->SetEnable(false);
		}

		if(m_iLineIndex + MAX_DEMON_PET_NUM < iSize)
		{
			m_pNextPage->SetShow(true);
			m_pNextPage->SetEnable(true);
		}
		else
		{
			m_pNextPage->SetShow(false);
			m_pNextPage->SetEnable(false);
		}
	}
}

bool CPetInfoWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch(dwMsg)
	{
	case OPER_UPDATE:
		{
			if (dwData == 2)
			{
				CloseWindow();
				return true;
			}
			else if(dwData == 1)
			{
				SwitchToPage(0);
				return true;
			}
		}
	case MSG_CTRL_BUTTON_CLICK:
		{
			if(pControl == m_pPriorPage)
			{
				m_iLineIndex -= MAX_DEMON_PET_NUM;
				if(m_iLineIndex < 0)
					m_iLineIndex = 0;
				return true;
			}
			else if(pControl == m_pNextPage)
			{
				int iSize = g_PetData.GetPetStatus().size();
				if(iSize > 0 && m_iLineIndex + MAX_DEMON_PET_NUM < iSize)
				{
					m_iLineIndex += MAX_DEMON_PET_NUM;
				}
				return true;
			}
		}
		break;
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

bool CPetInfoWnd::OnLeftButtonUp(int iX,int iY)
{
	return CCtrlWindowX::OnLeftButtonUp(iX,iY);
}
