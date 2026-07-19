#include "LingWuSanWeiFire.h"
#include "GameControl/GameControl.h"
#include "GameData/NpcData.h"

INIT_WND_POSX(CLingWuSanWeiFire,POS_AUTO,POS_AUTO)

CLingWuSanWeiFire::CLingWuSanWeiFire(void)
{
	m_iIndex = 22360;
}

CLingWuSanWeiFire::~CLingWuSanWeiFire(void)
{
}
void CLingWuSanWeiFire::OnCreate()
{	
	SetCloseButton(805,35, 80);

	m_pLingWu = new CCtrlButton;
	AddControl(m_pLingWu);
	m_pLingWu->CreateEx(this,333,507,22803,22804,22805);
}

void CLingWuSanWeiFire::Draw(void)
{
	if(!m_bShow) return;

	//绘制背景
	DrawBkg();

	LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22801,EP_UI);
	if(pTex)
	{
		g_pGfx->DrawTextureNL(m_iScreenX + 38,m_iScreenY + 30,pTex);
	}

	// 绘制所有子控件
	CControlContainer::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	//g_pFont->DrawText(m_iScreenX + 140,m_iScreenY + 425,"三昧真火为木灵之青火、地灵之石火，以及空灵之天火的总称。这种力量普遍存在于中州各地，其中尤以圣殿山",0xFF9E6D34,FONT_DEFAULT,FONTSIZE_DEFAULT);
	//g_pFont->DrawText(m_iScreenX + 140,m_iScreenY + 440,"附近的三昧真火之力最为丰富。当年孟虎在圣殿山苦战，于绝境中领悟了三昧真火之力进而大败魔族军队。但是",0xFF9E6D34,FONT_DEFAULT,FONTSIZE_DEFAULT);
	//g_pFont->DrawText(m_iScreenX + 140,m_iScreenY + 455,"三昧真火之力太难驾驭，若修炼者意志不够则极易反噬进而伤及生灵。孟虎在三思之后终于决定将这种力量封印",0xFF9E6D34,FONT_DEFAULT,FONTSIZE_DEFAULT);
	//g_pFont->DrawText(m_iScreenX + 140,m_iScreenY + 470,"起来，并且派遣其信赖之人宝儿守护三昧真火的秘密。只有得到孟虎认可的人，才能获得三昧真火的真正力量。",0xFF9E6D34,FONT_DEFAULT,FONTSIZE_DEFAULT);
}

bool CLingWuSanWeiFire::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl/* = NULL*/)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_BUTTON_CLICK:
		{
			if(pControl == m_pLingWu)
			{
				g_pGameControl->SEND_Exchange_Menu_Click(g_NPC.GetID(),"@insight");				
				CloseWindow();
				return true;
			}
		}
		break;
	default:
		break;
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}