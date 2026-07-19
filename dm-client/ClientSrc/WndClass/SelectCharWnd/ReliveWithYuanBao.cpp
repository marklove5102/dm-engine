
#include "relivewithyuanbao.h"
#include "gamedata/gamedata.h"
#include "GameControl\GameControl.h"
#include "GameData/MsgBoxMgr.h"
#include "Global/Interface/AudioInterface.h"
#include "GameData/OtherData.h"

INIT_WND_POSX(CReliveWithYuanBaoWnd,POS_VARIABLE,POS_VARIABLE)

CReliveWithYuanBaoWnd::CReliveWithYuanBaoWnd(void)
{
	m_sName = "ReliveWithYuanBaoWnd";
	SetLifeTime(GetTickCount() + 20000);
	m_bClickedReliveBtn = false;
	m_iLastSecond = 20;
	m_iIndex = 12805;
	m_uStyle &= ~CTRL_STYLE_TRANS;
}

CReliveWithYuanBaoWnd::~CReliveWithYuanBaoWnd(void)
{

}

void CReliveWithYuanBaoWnd::OnCreate()
{
	//复活按钮
	m_pRelive = new CCtrlButton();
	AddControl(m_pRelive);
	m_pRelive->CreateEx(this,20,20,0,12807,12808);

	SetCloseButton(220,3);
}

//单击关闭按钮
void CReliveWithYuanBaoWnd::OnClickCloseButton()
{
	g_pControl->Msg(MSG_CTRL_QUIT_WND,OPER_CREATE);
	CloseWindow();
}

void CReliveWithYuanBaoWnd::Draw(void)
{
	CReliveWnd::Draw();

	//符的价格
	char strPrice[12] = {0};
	sprintf(strPrice,"%d",g_OtherData.GetReLivePrice());
	g_pFont->DrawText(m_iScreenX+122,m_iScreenY+73 ,strPrice,0xFFFFFF00,FONT_DEFAULT,FONTSIZE_BIG,DTF_BlackFrame);
}


void CReliveWithYuanBaoWnd::OnReliveImm()
{
	ReliveUseYanBao(TRUE);
}