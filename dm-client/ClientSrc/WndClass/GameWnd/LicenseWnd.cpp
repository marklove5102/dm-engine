#include "LicenseWnd.h"
#include "GameData/LoginData.h"


INIT_WND_POSX(CLicenseWnd,POS_AUTO,POS_AUTO)

CLicenseWnd::CLicenseWnd(void)
{
	m_iPages = 1;
	m_iIndex = 16072;
	m_bDisableEscape = true;
	m_bModel = true;;
}

CLicenseWnd::~CLicenseWnd(void)
{

}

void CLicenseWnd::OnCreate()
{
	m_pOk = new CCtrlButton();
	AddControl(m_pOk);
	m_pOk->CreateEx(this,120,455,59,60,61);
	m_pOk->SetText("接受",COLOR_BTN_NORMAL,COLOR_BTN_MOUSEON,COLOR_BTN_PRESS);

	m_pCancel = new CCtrlButton();
	AddControl(m_pCancel);
	m_pCancel->CreateEx(this,251,455,59,60,61);
	m_pCancel->SetText("不接受",COLOR_BTN_NORMAL,COLOR_BTN_MOUSEON,COLOR_BTN_PRESS);

	m_pMarkViewer = new CMarkViewer(15,7,2,3);
	AddControl(m_pMarkViewer);
	m_pMarkViewer->Create(this,28,52,422,110);
	m_pMarkViewer->SetTagText(&m_TagText);
	m_pMarkViewer->SetFont(FONT_YAHEI);
	m_pMarkViewer->SetDefaultColor(COLOR_TEXT_SBU_TITLE);

	m_pInfo = new CCtrlMultiEdit(102400,56,14,false,20);
	AddControl(m_pInfo);
	m_pInfo->Create(this,12,52,158,364,273,true);

	string strPath = GetGameDataDir();
	strPath += "\\license.txt";

	FILE * fp = fopen(strPath.c_str(),"rb");
	if(fp)
	{
		long length = 0;
		fseek( fp, 0, SEEK_END );
		length = ftell( fp );
		fseek( fp, 0, SEEK_SET );
		char tmpbuf[102400]={0};
		fread(tmpbuf,min(102400,length),1,fp);
		m_pInfo->SetText(tmpbuf,FONT_YAHEI,12);	
		m_pInfo->SetColor(COLOR_TEXT_NORMAL);
		fclose(fp);
	}
}

void CLicenseWnd::OnDraw()
{
	g_pFont->DrawText(189+m_iScreenX, 25+m_iScreenY, "用户协议", 0xFFFAC896, FONT_YAHEI, FONTSIZE_BIG);
}

bool CLicenseWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_BUTTON_CLICK:
		{
			if(pControl == m_pOk)
			{

				WIN32_FIND_DATA ffd;
				string strFileName = GetGameDataDir();
				strFileName += "\\license.txt";

				HANDLE h = FindFirstFile(strFileName.c_str(),&ffd);  
				if(h != INVALID_HANDLE_VALUE)   
				{ 
					FindClose(h);  

					char strTemp[32]={0};
					sprintf(strTemp,"%u%u",ffd.ftLastWriteTime.dwHighDateTime,ffd.ftLastWriteTime.dwLowDateTime);

					strFileName = GetGameDataDir();
					strFileName += "\\config\\LoginIdCfg.ini";

					WritePrivateProfileString("LicenseTime",g_Login.GetLoginID(),strTemp,strFileName.c_str());
				}


				CloseWindow(true);

				g_pControl->Msg(MSG_CTRL_LICENSE2WND_WND,OPER_CREATE);

				return true;
			}

			if(pControl == m_pCancel)
			{
				PostMessage(g_hWnd,WM_DESTROY,0,0);
				return true;
			}
		}
		break;
	case MSG_CTRL_LICENSEWND_WND:
		if(dwData == 1 && pControl)
		{
			string str = (const char *)pControl;
			m_TagText.Parse(str,0,"|");
		}
		break;
	default:
		break;
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}