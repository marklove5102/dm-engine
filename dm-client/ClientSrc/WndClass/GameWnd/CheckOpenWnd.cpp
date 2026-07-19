#include "CheckOpenWnd.h"
#include "Global/Interface/StreamInterface.h"
#include "GameClient/VoiceAdapter.h"
#include "GameData/ConfigData.h"
#include "Global/Interface/AudioInterface.h"


INIT_WND_POSX(CCheckOpenWnd,POS_AUTO,POS_AUTO)

CCheckOpenWnd::CCheckOpenWnd(void)
{
	m_iType = sm_dwWindowType;

    m_iIndex = 14071;
    m_sName = "CkeckOpenWnd";
    m_pNoPopRadio = NULL;
    m_pOkButton = NULL;
    m_pCancelButton = NULL;
    m_bModel = true;
}

CCheckOpenWnd::~CCheckOpenWnd(void)
{
}

bool CCheckOpenWnd::Create(CControl* pParent,int iCurPage)
{
    if(m_iType == 1)
    {
        if(/*stricmp(g_pStreamMgr->GetConfigStr("Sound"),"Yes") == 0 && */g_pStreamMgr->GetConfigInt("AllwaysStopVoice",0) == 0)
        {
            m_iType = 2;
            return CCtrlWindowX::Create(pParent,iCurPage);
        }
		else
		{
			g_VoiceAdapter.SetGameVoice(g_Config.GetSndOn());
			g_TempConfig.SetSndOn(false);
			g_pAudio->EnableSound(false);
		}

    }
    else if(m_iType == 3)
    {
        if (g_pStreamMgr->GetConfigInt("ActivateMode") == 2 && stricmp(g_pStreamMgr->GetConfigStr("NoMorePopup","No"),"No") == 0)
        {
            m_iType = 1;
            return CCtrlWindowX::Create(pParent,iCurPage); 
        }
        else
        {
            if(/*stricmp(g_pStreamMgr->GetConfigStr("Sound"),"Yes") == 0 && */g_pStreamMgr->GetConfigInt("AllwaysStopVoice",0) == 0)
            {
                m_iType = 2;
                return CCtrlWindowX::Create(pParent,iCurPage);
            }
			else
			{
				g_VoiceAdapter.SetGameVoice(g_Config.GetSndOn());
				g_TempConfig.SetSndOn(false);
				g_pAudio->EnableSound(false);
			}
        }
    }

    return false;    
}

void CCheckOpenWnd::OnCreate()
{
    m_pOkButton = new CCtrlButton();
    if (m_iType == 1)
	    m_pOkButton->CreateEx(this, 92,109, 95, 96, 97, 98);
    else 
	    m_pOkButton->CreateEx(this, 26,109, 95, 96, 97, 98);

	m_pOkButton->SetText("确定", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);
    AddControl(m_pOkButton);

    if (m_iType == 2)
    {
        m_pCancelButton = new CCtrlButton();
		m_pCancelButton->CreateEx(this, 143,109, 95, 96, 97, 98);
		m_pCancelButton->SetText("取消", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);
        AddControl(m_pCancelButton);
    }

    m_pNoPopRadio = new CCtrlRadio();
	m_pNoPopRadio->Create(this,55,77,125,126,127,128,110,20);
	m_pNoPopRadio->SetText("以后不再提示我",COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,COLOR_TEXT_NORMAL,FONTSIZE_SMALL,0,FONT_YAHEI,8);
    AddControl(m_pNoPopRadio);
    m_pNoPopRadio->SetChecked(false);
}

void CCheckOpenWnd::OnClickCloseButton()
{
    if (m_iType == 1)
    {
        g_pControl->PopupWindow(MSG_CTRL_CHECK_OPEN_WND,OPER_RECREATE,1);
        return;
    }
    else
    {
        g_pControl->PopupWindow(MSG_CTRL_CHECK_OPEN_WND,OPER_CLOSE);
        return;
    }
}

void CCheckOpenWnd::Draw()
{
    CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	if(m_iType == 1)
    {
        g_pFont->DrawText(m_iScreenX + 35,m_iScreenY + 35,"语音激活方式为按键激活，",COLOR_TEXT_NORMAL,FONT_YAHEI);
        g_pFont->DrawText(m_iScreenX + 35,m_iScreenY + 55,"请持续按Ctrl键激活语音！",COLOR_TEXT_NORMAL,FONT_YAHEI);;
    }
    else 
    {	
        g_pFont->DrawText(m_iScreenX + 35,m_iScreenY + 35,"你已进入语音频道，",COLOR_TEXT_NORMAL,FONT_YAHEI);
        g_pFont->DrawText(m_iScreenX + 35,m_iScreenY + 55,"是否关闭游戏声音？",COLOR_TEXT_NORMAL,FONT_YAHEI);
    }
}

bool CCheckOpenWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
    if(dwMsg == MSG_CTRL_BUTTON_CLICK)	
    {
        if (pControl == m_pOkButton)
        {
            if (m_iType == 2)
            {
                g_VoiceAdapter.SetGameVoice(g_Config.GetSndOn());

				g_Config.SetSndOn(false);
				g_pAudio->EnableSound(false);

                if (m_pNoPopRadio->IsChecked())
                {	
                    g_pStreamMgr->SetConfigInt("AllwaysStopVoice",1);//AllwaysStopVoice为0表示弹框，1表示关游戏声音，2表示不关
                }
            }
            else if (m_iType == 1)
            {
                if (m_pNoPopRadio->IsChecked())
                {	
                    g_pStreamMgr->SetConfigStr("NoMorePopup","Yes");
                }
            }
            OnClickCloseButton();
            return true;
        }
        else if (pControl == m_pCancelButton)
        {
            if (m_pNoPopRadio->IsChecked())
            {
                g_pStreamMgr->SetConfigInt("AllwaysStopVoice",2);
            }
            OnClickCloseButton();
            return true;
        }
    }
    return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}