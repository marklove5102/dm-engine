#include "levelcheck.h"
#include "GameControl/GameControl.h"
#include "GameData/LoginData.h"

INIT_WND_POS(CLevelCheck,POS_AUTO,POS_AUTO)

CLevelCheck::CLevelCheck()
{
	m_bModel	= true;
	m_pValidate = NULL;
	m_uStyle &= ~CTRL_STYLE_TRANS;
}

CLevelCheck::~CLevelCheck(void)
{
	if(m_pValidate)
	{
		g_pTexMgr->ReleaseTex(m_pValidate );
	}
	DeleteFile("tmp1.tex");
}

void CLevelCheck::OnClickOkButton()
{
	//g_pGameControl->SEND_Level_Check(m_pEdit->GetText());
	g_pGameControl->SEND_Verify_Ack(m_pEdit->GetText());

	//if(g_Login.GetEnterCheckType() == 0)
		g_pControl->Msg(MSG_CTRL_ENTER_GAME_BEGIN,0,0);

	CCtrlMsgBox::OnClickOkButton();
}


void CLevelCheck::Draw()
{
	CCtrlMsgBox::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	m_pEdit->SetMaxLength(6);

	//g_pFont->DrawText(m_iScreenX + 32, m_iScreenY + 35,"请输入验证码：",0xFFFFFFFF);

	if(m_pValidate == NULL)
	{
		string strPath = GetGameDataDir();
		strPath += "\\tmp1.tex";
		FILE * fp = fopen(strPath.c_str(),"rb");
		if(fp)
		{
			fclose(fp);
			m_pValidate = g_pTexMgr->LoadTexFromDiskFile(strPath.c_str(),EP_UI);
		}
	}

	if(m_pValidate != NULL)
	{
		g_pGfx->DrawTextureNL(m_iScreenX + 140,m_iScreenY + 18, m_pValidate);
	}

}

