#include "lookleavewordswnd.h"
#include "GameData/GameData.h"
#include "GameControl/GameControl.h"
#include "GameData/MsgBoxMgr.h"
#include "Global/Interface/AudioInterface.h"
#include <algorithm>
#include "GameData/OtherData.h"

#pragma comment(lib, "Urlmon.lib")

//extern bool g_bminiwnd;
//extern int popmsgcurrentpos;

DWORD WINAPI SaveFunc(LPVOID pData)
{
	CSeException::Install_Handler();

	CLookLeaveWordsWnd* p = (CLookLeaveWordsWnd*)pData;
	p->SaveFile();
	return 0;
}

CLookLeaveWordsWnd::CLookLeaveWordsWnd(void)
{
	m_bModel	= false;

	if(g_OtherData.GetLeaveWordsVector().size() > 0)
		m_nCurLeaveWord = 0;
	else
		m_nCurLeaveWord = -1;

}

CLookLeaveWordsWnd::~CLookLeaveWordsWnd(void)
{

	if(m_nCurLeaveWord < g_OtherData.GetLeaveWordsVector().size() && m_nCurLeaveWord >= 0)
		g_OtherData.GetLeaveWordsVector().at(m_nCurLeaveWord).bIsReaded = true;

	//if(m_pHave)
	//{
	//	g_pTexMgr->ReleaseTex(m_pHave);
	//	m_pHave = NULL;
	//}
}
const char* CLookLeaveWordsWnd::GetPic()
{
	if(m_nCurLeaveWord>=0)
		return g_OtherData.GetLeaveWordsVector().at(m_nCurLeaveWord).strPic.c_str();

	return NULL;
}

bool CLookLeaveWordsWnd::Create(CControl *pParent, int iX, int iY)
{
	LPTexture pTex = g_pTexMgr->GetTexImm(PACKAGE_INTERFACE,9601,EP_UI);
	if(pTex)
		return CControl::Create(pParent,iX,iY,pTex->GetWidth(),pTex->GetHeight(),CTRL_STYLE_BACKMODE_TEX|CTRL_STYLE_SELFDELETE_BACKTEX,0xFFFFFFFF,MAKELONG(9601,PACKAGE_INTERFACE));
	else 
		return false;
}

bool CLookLeaveWordsWnd::GetSaving()
{
	if(hSave)
	{
		WaitForSingleObject(hSave, INFINITE);
		return true;
	}
	return false;
}
void CLookLeaveWordsWnd::PlayPic()
{
	//m_picshow.PlayFile(g_OtherData.GetLeaveWordsVector().at(m_nCurLeaveWord).strPic.c_str());
}
void CLookLeaveWordsWnd::PlayMusic()
{
	//m_music.PlayFile(g_OtherData.GetLeaveWordsVector().at(m_nCurLeaveWord).strMusic.c_str());
}
void CLookLeaveWordsWnd::OnCreate(void)
{
	m_picxoff = 387;
	m_picyoff = 57;
	m_picw = 113;
	m_pich = 110;
	//m_picshow.Init(m_picxoff,m_picyoff,m_picw,m_pich);
	OnMove();
	//m_music.Init(0,0,0,0);

	//m_pHave = g_pTexMgr->LoadTex(PACKAGE_INTERFACE,9650);

	m_pPlay = new CCtrlButton();
	AddControl(m_pPlay);
	m_pPlay->CreateEx(this,387,186,0,9611,9612);

	m_pPause= new CCtrlButton();
	AddControl(m_pPause);
	m_pPause->CreateEx(this,421,186,0,9614,9615);

	m_pStop= new CCtrlButton();
	AddControl(m_pStop);
	m_pStop->CreateEx(this,447,186,0,9617,9618);

	m_pSendPage= new CCtrlButton();
	AddControl(m_pSendPage);
	m_pSendPage->CreateEx(this,5,0,0,9606,9607);

	m_pMultiEdit = new CCtrlMultiEdit(1600,58,12);
	AddControl(m_pMultiEdit);
	m_pMultiEdit->Create(this,1,8,50,350,150);

	m_pPre = new CCtrlButton();
	AddControl(m_pPre);
	m_pPre->CreateEx(this,7,220,0,9627,9628);	

	m_pNext = new CCtrlButton();
	AddControl(m_pNext);
	m_pNext->CreateEx(this,58,220,0,9629,9630);

	m_pBigpic = new CCtrlButton();
	AddControl(m_pBigpic);
	m_pBigpic->CreateEx(this,386,217,0,9631,9632);		

	m_pDelete = new CCtrlButton();
	AddControl(m_pDelete);
	m_pDelete->CreateEx(this,6,243,0,9638,9639);

	m_pClearAll = new CCtrlButton();
	AddControl(m_pClearAll);
	m_pClearAll->CreateEx(this,438,243,0,9640,9641);		

	m_pReply = new CCtrlButton();
	AddControl(m_pReply);
	m_pReply->CreateEx(this,89,243,0,9642,9643);

	m_pForward = new CCtrlButton();
	AddControl(m_pForward);
	m_pForward->CreateEx(this,172,243,0,9644,9645);

	m_pSaveas = new CCtrlButton();
	AddControl(m_pSaveas);
	m_pSaveas->CreateEx(this,436,217,0,9648,9649);

	m_pClose = new CCtrlButton();
	AddControl(m_pClose);
	m_pClose->CreateEx(this,491,2,0,9701,9702);

	m_pScroll = new CCtrlScroll();
	AddControl(m_pScroll);
	m_pScroll->Create(this,372,26,11,196,9694);

	hSave = NULL;

	if(m_nCurLeaveWord == 0)
	{
		//popmsgcurrentpos = 0;
		for(;m_nCurLeaveWord < g_OtherData.GetLeaveWordsVector().size() - 1;m_nCurLeaveWord++)
		{
			if(!g_OtherData.GetLeaveWordsVector().at(m_nCurLeaveWord).bIsReaded)
				break;
		}

		if(m_nCurLeaveWord == g_OtherData.GetLeaveWordsVector().size() - 1 && g_OtherData.GetLeaveWordsVector().at(m_nCurLeaveWord).bIsReaded)
			m_nCurLeaveWord = 0;

		m_pMultiEdit->SetText(g_OtherData.GetLeaveWordsVector().at(m_nCurLeaveWord).strMessage.c_str());

		m_pScroll->SetStep(1);
		m_pScroll->SetRange(m_pMultiEdit->GetLines());
		m_pScroll->SetPos(0);

		if(g_OtherData.GetLeaveWordsVector().at(m_nCurLeaveWord).strPic.size())
		{
			PlayPic();
		}
	}

}

void CLookLeaveWordsWnd::OnClickCloseButton()
{
	//if(m_music.IsDwonFiles() || m_picshow.IsDwonFiles()) return;
	if(GetSaving())
	{
		SetEvent(hSave);	
		return;
	}

	g_pControl->PopupWindow("g3ms_pic",OPER_CLOSE);
	g_pControl->PopupWindow("g3ms_music",OPER_CLOSE);
	g_pControl->PopupWindow("g3ms_page",OPER_CLOSE);
}

void CLookLeaveWordsWnd::OnMove()
{
	CCtrlWindow::OnMove();
	//m_picshow.MoveTo(m_iScreenX+m_picxoff,m_iScreenY+m_picyoff);
}
bool CLookLeaveWordsWnd::Msg(DWORD dwMsg, DWORD dwData, CControl *pControl)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_BUTTON_CLICK:
		if(pControl == m_pSendPage)
		{
			g_pControl->PopupWindow(MSG_CTRL_LEAVEWORD_WND,OPER_CREATE,1);
			return true;
		}
		else if(pControl == m_pPic)
		{
			//g_pControl->Msg(MSG_CTRL_G3MS_PIC,0);
			return true;
		}
		if(pControl == m_pPre)
		{
			//if(m_music.IsDwonFiles() || m_picshow.IsDwonFiles()) return true;

			if( m_nCurLeaveWord > 0 )
			{
				//m_picshow.Reset();
				//m_music.Reset();
				g_OtherData.GetLeaveWordsVector().at(m_nCurLeaveWord).bIsReaded = true;
				m_nCurLeaveWord--;
				//popmsgcurrentpos = m_nCurLeaveWord;
				m_pMultiEdit->SetText(g_OtherData.GetLeaveWordsVector().at(m_nCurLeaveWord).strMessage.c_str());
				m_pScroll->SetStep(1);
				m_pScroll->SetRange(m_pMultiEdit->GetLines());
				m_pScroll->SetPos(0);
				if(g_OtherData.GetLeaveWordsVector().at(m_nCurLeaveWord).strPic.size()>0)
				{
					PlayPic();
				}
			}
			return true;
		}
		else if(pControl == m_pNext)
		{		
			//if(m_music.IsDwonFiles() || m_picshow.IsDwonFiles()) return true;
			if(m_nCurLeaveWord < g_OtherData.GetLeaveWordsVector().size() - 1)
			{
				//m_picshow.Reset();
				//m_music.Reset();
				g_OtherData.GetLeaveWordsVector().at(m_nCurLeaveWord).bIsReaded = true;
				m_nCurLeaveWord++;	
				//popmsgcurrentpos = m_nCurLeaveWord;
				m_pMultiEdit->SetText(g_OtherData.GetLeaveWordsVector().at(m_nCurLeaveWord).strMessage.c_str());
				m_pScroll->SetStep(1);
				m_pScroll->SetRange(m_pMultiEdit->GetLines());
				m_pScroll->SetPos(0);
				if(g_OtherData.GetLeaveWordsVector().at(m_nCurLeaveWord).strPic.size()>0)
				{
					PlayPic();
					//m_picshow.PlayFile(g_OtherData.GetLeaveWordsVector().at(m_nCurLeaveWord).strPic.c_str());
				}
			}
			return true;
		}
		else if(pControl == m_pClearAll)
		{
			//if(m_music.IsDwonFiles() || m_picshow.IsDwonFiles()) return true;

			if(g_OtherData.GetLeaveWordsVector().size() > 0)
			{
				g_MsgBoxMgr.PopSimpleComfirm("确定清空所有留言么？",MSG_CTRL_CLEARALL_LEAVEWORD,0);
			}
			return true;
		}
		else if(pControl == m_pDelete)
		{
			//if(m_music.IsDwonFiles() || m_picshow.IsDwonFiles()) return true;

			if(g_OtherData.GetLeaveWordsVector().size() > 0)
			{
				if(m_nCurLeaveWord < g_OtherData.GetLeaveWordsVector().size() && m_nCurLeaveWord >= 0)
				{
					std::vector<_LeaveWords>::iterator pos;
					pos = find( g_OtherData.GetLeaveWordsVector().begin(),
						g_OtherData.GetLeaveWordsVector().end(),
						g_OtherData.GetLeaveWordsVector().at(m_nCurLeaveWord));

					g_OtherData.GetLeaveWordsVector().erase(pos);
				}
				if(m_nCurLeaveWord == g_OtherData.GetLeaveWordsVector().size())
					m_nCurLeaveWord--;

				//popmsgcurrentpos = m_nCurLeaveWord;
				//m_picshow.Reset();
				//m_music.Reset();

				if( m_nCurLeaveWord >= 0 )
				{
					m_pMultiEdit->SetText(g_OtherData.GetLeaveWordsVector().at(m_nCurLeaveWord).strMessage.c_str());
					m_pScroll->SetStep(1);
					m_pScroll->SetRange(m_pMultiEdit->GetLines());
					m_pScroll->SetPos(0);
					if(g_OtherData.GetLeaveWordsVector().at(m_nCurLeaveWord).strPic.size()>0)
					{
						PlayPic();
					}
				}
				else
					m_pMultiEdit->SetText("   ");
			}
			return true;
		}
		else if(pControl == m_pBigpic)
		{
			//if(GetPic())
			//	g_pControl->Msg(MSG_CTRL_G3MS_BIGPIC,OPER_CREATE);
			return true;
		}
		else if(pControl == m_pReply)			// 回复
		{
			if( m_nCurLeaveWord >= 0 )
				((Cg3ms_page*)(this->GetParent()))->Reply(m_nCurLeaveWord);

			g_pControl->Msg(MSG_CTRL_LEAVEWORD_WND,1,this);
			return true;
		}
		else if (pControl == m_pForward)		// 转发
		{
			if( m_nCurLeaveWord >= 0 )
				((Cg3ms_page*)(this->GetParent()))->Forward(m_nCurLeaveWord);

			g_pControl->Msg(MSG_CTRL_LEAVEWORD_WND,1,this);
			return true;
		}
		else if(pControl ==  m_pPhrase)			// 短语
		{
			g_pControl->Msg(MSG_CTRL_PHRASE_WND,m_nCurLeaveWord);
		}
		else if(pControl == m_pPlay)
		{
			if( m_nCurLeaveWord >= 0 )
			{
				if(g_OtherData.GetLeaveWordsVector().at(m_nCurLeaveWord).strMusic.size()>0)
				{
					PlayMusic();
				}
			}
			return true;
		}
		else if(pControl == m_pStop)
		{
			//m_music.Reset();
			return true;
		}
		else if(pControl == m_pPause)
		{
			//m_music.Pause();
			return true;
		}
		else if(pControl==m_pSaveas)
		{
			SaveAS();
			return true;
		}
		else if(pControl==m_pClose)
		{
			OnClickCloseButton();
			return true;
		}
		break;
	default:
		break;
	}	
	return CCtrlWindow::Msg(dwMsg,dwData,pControl);
}

void CLookLeaveWordsWnd::SaveFile()
{
	char temp[1024] = {0};
	char fdown[512];
	char find[64]={0};
	char sfile[256]={0};
	char dir[256]={0};
	bool show = false;
	GetCurrentDirectory(256,dir);
	int i = strlen(dir) - 1;
	for(;i>0;i--)
	{
		if(dir[i] == '\\')
		{
			dir[i] = 0;
			break;
		}
	}
	sprintf(fdown,"%s\\POPTANG",dir);
	CreateDirectory(fdown,NULL);
	sprintf(temp,"已经成功将文件保存到您的POPTANG文件夹内\n地址是： %s\\ \n\n", fdown);
	//if(m_nCurLeaveWord >= 0)
	{
		if(urlpic.size()>0)
		{
			i = 0;
			strcpy(sfile,urlpic.c_str());
			strcpy(sfile,_strlwr(sfile));
			if((strstr(sfile,"http://") || strstr(sfile, "ftp://")) && 
				strstr(sfile+strlen(sfile)-4,".bmp") || strstr(sfile+strlen(sfile)-4,".jpg") || strstr(sfile+strlen(sfile)-4,".gif"))
			{
				sprintf(fdown,"%s\\POPTANG\\%s",dir,name.c_str());

				while(1)
				{
					sprintf(find, "%05d.%s", i, sfile+strlen(sfile)-3);
					strcat(fdown,find);
					FILE* fp = NULL;
					if((fp = fopen(fdown,"rb")) == NULL)
						break;
					fclose(fp);
					fdown[strlen(fdown) -9 ] = 0;
					i++;
				}
				HRESULT hr = URLDownloadToFile(0, sfile,fdown,0,0);
				if(SUCCEEDED(hr))
				{
					strcat(temp, "图片名为：");
					strcat(temp,name.c_str());
					strcat(temp, find);
					strcat(temp, "\n");
					show = true;
				}
			}
		}

		if(urlmusic.size()>0)
		{
			i = 0;
			strcpy(sfile,urlmusic.c_str());
			if((strstr(sfile,"http://") || strstr(sfile, "ftp://")) && 
				strstr(sfile+strlen(sfile)-4,".mid") || strstr(sfile+strlen(sfile)-4,".mp3") || strstr(sfile+strlen(sfile)-4,".wav"))
			{
				sprintf(fdown,"%s\\POPTANG\\%s",dir,g_OtherData.GetLeaveWordsVector().at(m_nCurLeaveWord).strCharName.c_str());

				while(1)
				{
					sprintf(find, "%05d.%s", i, sfile+strlen(sfile)-3);
					strcat(fdown,find);
					FILE* fp = NULL;
					if((fp = fopen(fdown,"rb")) == NULL)
						break;
					fclose(fp);
					fdown[strlen(fdown) -9 ] = 0;
					i++;
				}
				HRESULT hr = URLDownloadToFile(0, sfile,fdown,0,0);
				if(SUCCEEDED(hr))
				{
					strcat(temp, "音乐名为：");
					strcat(temp,g_OtherData.GetLeaveWordsVector().at(m_nCurLeaveWord).strCharName.c_str());
					strcat(temp, find);
					show = true;
				}
			}
		}
		if(show)
		{
			g_MsgBoxMgr.PopSimpleAlert(temp);
		}
	}
	if(hSave)
	{
		CloseHandle(hSave);
		hSave = NULL;
	}
}
void CLookLeaveWordsWnd::SaveAS()
{
	if(GetSaving())
	{
		SetEvent(hSave);	
		return;
	}

	if(m_nCurLeaveWord >= 0)
	{
		urlpic = g_OtherData.GetLeaveWordsVector().at(m_nCurLeaveWord).strPic;
		urlmusic = g_OtherData.GetLeaveWordsVector().at(m_nCurLeaveWord).strMusic;
		name = g_OtherData.GetLeaveWordsVector().at(m_nCurLeaveWord).strCharName;
		hSave = CreateEvent(NULL, 0, 0, NULL);
		SetEvent(hSave);
		downloading = true;
		DWORD IDThread;
		HANDLE h = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)SaveFunc, this, 0,                       // use default creation flags 
			&IDThread);
	}	
}
void CLookLeaveWordsWnd::Show(int s)
{
	if(s)
	{
		if(g_pControl->GetControls() != ((Cg3ms_page*)(this->GetParent()))->GetTop())
		{
			//m_picshow.Show(0);
		}
		else
		{
			//if(m_picshow.IsDwonFiles())
			//{
			//	char text[64];
			//	sprintf(text, "图片下载中...");
			//	g_pFont->DrawText(m_iScreenX + 255, m_iScreenY + 100,text,0xFFFFFFFF,FONTSIZE_DEFAULT);
			//}
			//else
			//	m_picshow.Show(1);
		}
	}
	//else
	//	m_picshow.Show(0);

}
void CLookLeaveWordsWnd::Draw(void)
{
	//char text[64];
	CCtrlWindow::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	//if(g_pControl->GetControls() != ((Cg3ms_page*)(this->GetParent()))->GetTop() || g_bminiwnd)
	//{
	//	m_picshow.Show(0);
	//}
	//else
	//{
	//	if(m_picshow.IsDwonFiles())
	//	{
	//		sprintf(text, "图片下载中...");
	//		g_pFont->DrawText(m_iScreenX + 255, m_iScreenY + 100,text,0xFFFFFFFF,FONTSIZE_DEFAULT);
	//	}
	//	else
	//		m_picshow.Show(1);
	//}

	if(m_nCurLeaveWord >= 0 && m_nCurLeaveWord < g_OtherData.GetLeaveWordsVector().size())
	{
		m_pMultiEdit->SetDisLine(m_pScroll->GetPos());

		char temp[256] = {0};	

		// 是否以读标示
		if( g_OtherData.GetLeaveWordsVector().at(m_nCurLeaveWord).bIsReaded )
			strcpy(temp,"已读");
		else
			strcpy(temp,"未读");
		g_pFont->DrawText(m_iScreenX + 169,m_iScreenY + 13,temp,0xFFFFFFFF);

		// 当前位置
		sprintf(temp,"%d/%d",m_nCurLeaveWord+1,g_OtherData.GetLeaveWordsVector().size());
		g_pFont->DrawText(m_iScreenX + 225,m_iScreenY + 13,temp,0xFFFFFFFF);

		// 留言时间
		g_pFont->DrawText(m_iScreenX + 8,m_iScreenY + 38,"时间:",0xFFFFFF00);
		sprintf(temp,"%s %s",g_OtherData.GetLeaveWordsVector().at(m_nCurLeaveWord).strDate.c_str(),
			g_OtherData.GetLeaveWordsVector().at(m_nCurLeaveWord).strTime.c_str());
		g_pFont->DrawText(m_iScreenX + 32,m_iScreenY + 38,temp,0xFFFFFFFF);

		// 发送人
		g_pFont->DrawText(m_iScreenX + 150,m_iScreenY + 38,"发送人:",0xFFFFFF00);
		strcpy(temp, g_OtherData.GetLeaveWordsVector().at(m_nCurLeaveWord).strCharName.c_str());
		g_pFont->DrawText(m_iScreenX + 185,m_iScreenY + 38,temp,0xFFFFFFFF);
	}
	else
	{
		g_pFont->DrawText(m_iScreenX + 169,m_iScreenY + 13,"无留言",0xFFFFFFFF);
		g_pFont->DrawText(m_iScreenX + 225,m_iScreenY + 13,"0/0",0xFFFFFFFF);
	}
}

void CLookLeaveWordsWnd::SetCurLeaveWord(int nCurLeaveWord)
{
	m_nCurLeaveWord = nCurLeaveWord;
	//popmsgcurrentpos = m_nCurLeaveWord;
	if(nCurLeaveWord == -1)
	{
		m_pMultiEdit->SetText(" ");
		//m_picshow.Reset();
		//m_music.Reset();
	}
}

Cg3ms_send::Cg3ms_send(void)
{

	m_bModel	= false;
	m_pPlay		= NULL;

}

Cg3ms_send::~Cg3ms_send(void)
{



}

bool Cg3ms_send::Create(CControl *pParent, int iX, int iY)
{
	LPTexture pTex = g_pTexMgr->GetTexImm(PACKAGE_INTERFACE,9605,EP_UI);
	if(pTex)
		return CControl::Create(pParent,iX,iY,pTex->GetWidth(),pTex->GetHeight(),CTRL_STYLE_BACKMODE_TEX|CTRL_STYLE_SELFDELETE_BACKTEX,0xFFFFFFFF,MAKELONG(9605,PACKAGE_INTERFACE));
	else 
		return false;
}


void Cg3ms_send::OnCreate(void)
{
	m_picxoff = 387;
	m_picyoff = 57;
	m_picw = 113;
	m_pich = 110;
	//m_picshow.Init(m_picxoff,m_picyoff,m_picw,m_pich);
	OnMove();
	//m_music.Init(0,0,0,0);

	m_pPlay = new CCtrlButton();
	AddControl(m_pPlay);
	m_pPlay->CreateEx(this,387,186,0,9611,9612);

	m_pPause= new CCtrlButton();
	AddControl(m_pPause);
	m_pPause->CreateEx(this,422,186,0,9614,9615);

	m_pStop= new CCtrlButton();
	AddControl(m_pStop);
	m_pStop->CreateEx(this,447,186,0,9617,9618);

	m_pPic= new CCtrlButton();
	AddControl(m_pPic);
	m_pPic->CreateEx(this,6,243,0,9621,9622);

	m_pMusic= new CCtrlButton();
	AddControl(m_pMusic);
	m_pMusic->CreateEx(this,80,243,0,9623,9624);

	m_pOk = new CCtrlButton();
	AddControl(m_pOk);
	m_pOk->CreateEx(this,404,270,0,9619,9620);

	m_pLookPage= new CCtrlButton();
	AddControl(m_pLookPage);
	m_pLookPage->CreateEx(this,83,0,0,9608,9609);

	m_pClose = new CCtrlButton();
	AddControl(m_pClose);
	m_pClose->CreateEx(this,491,2,0,9701,9702);

	m_pHelp = new CCtrlButton();
	AddControl(m_pHelp);
	m_pHelp->CreateEx(this,437,243,0,9705,9706);

	m_pMultiEdit = new CCtrlMultiEdit(900,58,14);
	AddControl(m_pMultiEdit);
	m_pMultiEdit->Create(this,14,8,35,358,168);

	m_pEdit = new CCtrlEdit();
	m_pEdit->Create(this,FONTSIZE_DEFAULT,0xFFFFFFFF,170,270,126,20);
	AddControl(m_pEdit);
}

void Cg3ms_send::OnClickCloseButton()
{
	//if(m_music.IsDwonFiles() || m_picshow.IsDwonFiles()) return;

	g_pControl->Msg(MSG_CTRL_LEAVEWORD_WND,0,this);
}

void Cg3ms_send::OnMove()
{
	CCtrlWindow::OnMove();
	//m_picshow.MoveTo(m_iScreenX+m_picxoff,m_iScreenY+m_picyoff);
}
void Cg3ms_send::ShowPic(int n)
{
	//if(n)
	//{
	//	if(m_picstr.size()>0)
	//		m_picshow.Show(1);
	//	else
	//		m_picshow.Show(0);
	//}
	//else
	//	m_picshow.Show(n);
}
bool Cg3ms_send::IsMobil()
{
	const char *name = m_pEdit->GetText();
	if(strlen(name) == 11 && name[0]=='1' && name[1]=='3')
	{
		for(int i=2;i<11;i++)
		{
			if(name[i] >='0' && name[i] <='9')
			{
			}
			else
			{
				return false;
			}
		}
		return true;
	}
	return false;
}

bool Cg3ms_send::Msg(DWORD dwMsg, DWORD dwData, CControl *pControl)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_BUTTON_CLICK:
		if(pControl == m_pLookPage)
		{
			g_pControl->PopupWindow(MSG_CTRL_LEAVEWORD_WND,OPER_CREATE,2);
			return true;
		}
		else if(pControl == m_pPic)
		{
			//g_pControl->Msg(MSG_CTRL_G3MS_PIC,0);
			return true;
		}
		else if(pControl == m_pMusic)
		{
			//g_pControl->Msg(MSG_CTRL_G3MS_MUSIC,0);
			return true;
		}
		else if(pControl==m_pOk) //发送
		{
			if(strlen(m_pEdit->GetText())>0)
			{
				char temp[1024] = {0};
				if(IsMobil())
					sprintf(temp,"#%s %s",m_pEdit->GetText(),m_pMultiEdit->GetText());
				else
					sprintf(temp,"##%s %s",m_pEdit->GetText(),m_pMultiEdit->GetText());
				//g_pGameControl->SEND_Message_Send(temp,strlen(temp),m_picstr.c_str(),m_musicstr.c_str());
			}
		}
		else if(pControl == m_pPlay)
		{
			if(m_musicstr.size()>0)
			{
				PlayMusic();
			}
			return true;
		}
		else if(pControl == m_pStop)
		{
			//m_music.Reset();
			return true;
		}
		else if(pControl == m_pPause)
		{
			//m_music.Pause();
			return true;
		}
		else if(pControl == m_pClose)
		{
			OnClickCloseButton();
			return true;
		}
		else if(pControl == m_pHelp)
		{
			g_pControl->PopupWindow(MSG_CTRL_HELPWND,OPER_CREATE,1);
			return true;
		}
		break;
	default:
		break;
	}	

	return CCtrlWindow::Msg(dwMsg,dwData,pControl);
}

void Cg3ms_send::Draw(void)
{
	//char text[256];
	CCtrlWindow::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	//if(m_music.IsDwonFiles())
	//{
	//	sprintf(text, "正在缓冲...");
	//	g_pFont->DrawText(m_iScreenX + 388, m_iScreenY + 219,text,0xFFFFFFFF,FONTSIZE_DEFAULT);
	//}

	g_pFont->DrawText(m_iScreenX + 8, m_iScreenY + 219,"发送给其他玩家或手机用户将收取服务费0.1元。",0xFFFFFFFF);
	g_pFont->DrawText(m_iScreenX + 245, m_iScreenY + 245,"每条消息支持450个汉字。",0xFFFFFFFF);

	if(m_picstr.size() > 0)
	{
		//if(g_pControl->GetControls() != ((Cg3ms_page*)(this->GetParent()))->GetTop() || g_bminiwnd)
		//{
		//	m_picshow.Show(0);
		//}
		//else
		//{
		//	if(m_picshow.IsDwonFiles())
		//	{
		//		sprintf(text, "图片下载中...");
		//		g_pFont->DrawText(m_iScreenX + 388, m_iScreenY + 106,text,0xFFFFFFFF,FONTSIZE_DEFAULT);
		//	}
		//	else
		//		m_picshow.Show(1);
		//}
	}
	else
	{
		g_pFont->DrawText(m_iScreenX + 388, m_iScreenY + 106,"点击图片 音乐按钮，",0xFFFFFFFF);
		g_pFont->DrawText(m_iScreenX + 388, m_iScreenY + 121,"可发送图片或音乐。",0xFFFFFFFF);
	}
}

void Cg3ms_send::PlayPic()
{
	//m_picshow.PlayFile(m_picstr.c_str());
}
void Cg3ms_send::PlayMusic()
{
	//m_music.PlayFile(m_musicstr.c_str());
}
void Cg3ms_send::SetPicString(const char *str)
{
	//m_picshow.Reset();
	m_picstr = str;
	if(m_picstr.size()>0) 
	{
		PlayPic();
		//m_picshow.PlayFile(str);
		//m_picshow.Show(1);
	}
	else 
	{
		//m_picshow.Show(0);
	}
}
void Cg3ms_send::Show(int s)
{
	//if(s)
	//{
	//	if(m_picstr.size() > 0)
	//	{
	//		if(m_picshow.IsDwonFiles())
	//		{
	//		}
	//		else
	//		{
	//			m_picshow.Show(1);
	//			return;
	//		}
	//	}
	//}

	//m_picshow.Show(0);
}
void Cg3ms_send::SetMusicString(const char *str)
{
	//m_music.Reset();
	m_musicstr = str;
}

INIT_WND_POSX(Cg3ms_pic,POS_VARIABLE,POS_VARIABLE)

Cg3ms_pic::Cg3ms_pic(void)
{

	m_iIndex = 9602;
	m_iPages = 1;
	
	m_iAlignType = XAL_TOPLEFT;
	m_iOffX = 0;
	m_iOffY = 0;
}

Cg3ms_pic::~Cg3ms_pic(void)
{

}

void Cg3ms_pic::OnCreate(void)
{
	m_pEdit = new CCtrlEdit();
	m_pEdit->Create(this,FONTSIZE_DEFAULT,0xFFFFFFFF,18,160,125,20);
	AddControl(m_pEdit);

	m_pEdit->SetFocus();

	m_pOk = new CCtrlButton();
	AddControl(m_pOk);
	m_pOk->CreateEx(this,76,201,0,9697,9698);

	m_pCancle = new CCtrlButton();
	AddControl(m_pCancle);
	m_pCancle->CreateEx(this,164,201,0,9625,9626);

	m_pClose = new CCtrlButton();
	AddControl(m_pClose);
	m_pClose->CreateEx(this,293,4,0,9701,9702);
}

void Cg3ms_pic::OnClickCloseButton()
{
	//g_pControl->Msg(MSG_CTRL_G3MS_PIC,0,this);
}

bool Cg3ms_pic::Msg(DWORD dwMsg, DWORD dwData, CControl *pControl)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_BUTTON_CLICK:
		if(pControl==m_pOk)
		{
			char test[1024] = {0};
			if(strlen(m_pEdit->GetText())==0)
			{
				//g_pControl->Msg(MSG_CTRL_G3MS_PIC,1,this);
				return true;
			}

			strcpy(test,m_pEdit->GetText());
			strcpy(test,_strlwr(test));
			if(strncmp(test,"http://",7)!=0 && strncmp(test,"ftp://",6)!=0)
			{
				g_MsgBoxMgr.PopSimpleAlert("地址错误，必须为 http:// 或 ftp:// 格式 \n比如：http://www.sdo.com/a.gif\n\n如果您想发送电脑中的文件，请先将该文件上传\n然后再进行发送。");
			}
			else
			{
				if(strstr(test,".bmp") || strstr(test,".gif")||strstr(test,".jpg"))
				{
					//g_pControl->Msg(MSG_CTRL_G3MS_PIC,1,this);
					return true;
				}
				else
				{
					g_MsgBoxMgr.PopSimpleAlert("格式错误，图片必须为 jpg, bmp, gif格式\n\n比如：http://www.sdo.com/a.gif");
				}
			}
		}
		else if(pControl==m_pCancle)
		{
			OnClickCloseButton();
			return true;
		}
		else if(pControl == m_pClose)
		{
			OnClickCloseButton();
			return true;
		}
		break;
	default:
		break;
	}	
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void Cg3ms_pic::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}
}

const char * Cg3ms_pic::GetPic()
{
	return m_pEdit->GetText();
}

const char * Cg3ms_music::GetMusic()
{
	return m_pEdit->GetText();
}

INIT_WND_POSX(Cg3ms_music,POS_VARIABLE,POS_VARIABLE)

Cg3ms_music::Cg3ms_music(void)
{
	m_iIndex = 9603;
	m_iPages = 1;
	
	m_iAlignType = XAL_TOPLEFT;
	m_iOffX = 0;
	m_iOffY = 0;
}

Cg3ms_music::~Cg3ms_music(void)
{



}

void Cg3ms_music::OnCreate(void)
{
	m_pEdit = new CCtrlEdit();
	m_pEdit->Create(this,FONTSIZE_DEFAULT,0xFFFFFFFF,16,160,125,20);
	AddControl(m_pEdit);

	m_pEdit->SetFocus();

	m_pOk = new CCtrlButton();
	AddControl(m_pOk);
	m_pOk->CreateEx(this,76,201,0,9697,9698);

	m_pCancle = new CCtrlButton();
	AddControl(m_pCancle);
	m_pCancle->CreateEx(this,164,201,0,9625,9626);

	m_pClose = new CCtrlButton();
	AddControl(m_pClose);
	m_pClose->CreateEx(this,293,4,0,9701,9702);
}

void Cg3ms_music::OnClickCloseButton()
{
	//g_pControl->Msg(MSG_CTRL_G3MS_MUSIC,0,this);
}

bool Cg3ms_music::Msg(DWORD dwMsg, DWORD dwData, CControl *pControl)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_BUTTON_CLICK:
		if(pControl==m_pOk)
		{
			char test[1024] = {0};
			if(strlen(m_pEdit->GetText())==0)
			{
				//g_pControl->Msg(MSG_CTRL_G3MS_MUSIC,1,this);
				return true;
			}
			strcpy(test,m_pEdit->GetText());
			strcpy(test,_strlwr(test));
			if(strncmp(test,"http://",7)!=0 && strncmp(test,"ftp://",6)!=0)
			{
				g_MsgBoxMgr.PopSimpleAlert("地址错误，必须为 http:// 或 ftp:// 格式 \n比如：http://www.sdo.com/a.mp3\n\n如果您想发送电脑中的文件，请先将该文件上传\n然后再进行发送。");
			}
			else
			{
				if(strstr(test,".mp3") || strstr(test,".wav")||strstr(test,".mid"))
				{
					//g_pControl->Msg(MSG_CTRL_G3MS_MUSIC,1,this);
					return true;
				}
				else
				{
					g_MsgBoxMgr.PopSimpleAlert("格式错误，音乐必须为 mid, mp3格式\n\n比如：http://www.sdo.com/a.mid");
				}
			}
		}
		else if(pControl==m_pCancle)
		{
			OnClickCloseButton();
			return true;
		}
		else if(pControl == m_pClose)
		{
			OnClickCloseButton();
			return true;
		}
		break;
	default:
		break;
	}	
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void Cg3ms_music::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

}

INIT_WND_POSX(Cg3ms_bigpic,POS_VARIABLE,POS_VARIABLE)

Cg3ms_bigpic::Cg3ms_bigpic(void)
{
	m_bModel = true;

	m_iIndex = 9633;
	m_iPages = 1;
	
	m_iAlignType = XAL_TOPLEFT;
	m_iOffX = 0;
	m_iOffY = 0;
}

Cg3ms_bigpic::~Cg3ms_bigpic(void)
{

	//CDShow::CancleResize();

}

void Cg3ms_bigpic::PlayFile(const char *sfile)
{
	//m_picshow.PlayFile(sfile,true);
	//m_picshow.NormalSize();
}
void Cg3ms_bigpic::OnCreate(void)
{
	m_picxoff = 7;
	m_picyoff = 25;
	m_picw = 640;
	m_pich = 480;
	//m_picshow.Init(m_picxoff,m_picyoff,m_picw,m_pich);
	OnMove();

	m_pOk = new CCtrlButton();
	AddControl(m_pOk);
	m_pOk->CreateEx(this,4,4,0,9634,9635);

	m_pCancle = new CCtrlButton();
	AddControl(m_pCancle);
	m_pCancle->CreateEx(this,53,4,0,9636,9637);

	m_pClose = new CCtrlButton();
	AddControl(m_pClose);
	m_pClose->CreateEx(this,634,4,0,9701,9702);
}

void Cg3ms_bigpic::OnClickCloseButton()
{
	//g_pControl->Msg(MSG_CTRL_G3MS_BIGPIC,0,this);
}

bool Cg3ms_bigpic::Msg(DWORD dwMsg, DWORD dwData, CControl *pControl)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_BUTTON_CLICK:
		if(pControl==m_pOk)
		{
			//m_picshow.DoubleSize();
			return true;
		}
		else if(pControl==m_pCancle)
		{
			//m_picshow.HanlSize();
			return true;
		}
		else if(pControl == m_pClose)
		{
			OnClickCloseButton();
			return true;
		}
	default:
		break;
	}	
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}
void Cg3ms_bigpic::OnMove()
{
	CCtrlWindowX::OnMove();
	//m_picshow.MoveTo(m_iScreenX+m_picxoff,m_iScreenY+m_picyoff);
}
void Cg3ms_bigpic::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}
}

INIT_WND_POSX(Cg3ms_page,POS_VARIABLE,POS_VARIABLE)

Cg3ms_page::Cg3ms_page(void)
{
	m_iIndex = 0;
	m_iPages = 1;
	
	m_iAlignType = XAL_TOPLEFT;
	m_iOffX = 128;
	m_iOffY = 128;
	m_iWidth  = 511;
	m_iHeight = 322;

}

Cg3ms_page::~Cg3ms_page(void)
{
}

void Cg3ms_page::OnCreate(void)
{
	m_wndLook = new CLookLeaveWordsWnd();
	m_wndSend = new Cg3ms_send();
	AddControl(m_wndLook);
	AddControl(m_wndSend);
	m_wndLook->Create(this,0,0);
	m_wndSend->Create(this,0,0);
	m_wndSend->SetShow(0);
	pCurrent = m_wndLook;

	pTop = g_pControl->GetControls();
	SetPage(sm_dwWindowType);

}
bool Cg3ms_page::Msg(DWORD dwMsg, DWORD dwData, CControl *pControl)
{
	if(dwMsg == OPER_CREATE)
	{
		SetPage(dwData);
		return true;
	}

	return pCurrent->Msg(dwMsg,dwData,pControl);
}

void Cg3ms_page::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}
}
void Cg3ms_page::SetPage(int page)
{
	if(page==1)
	{
		m_wndLook->SetShow(0);
		m_wndSend->SetShow(1);
		m_wndLook->ShowPic(0);
		m_wndSend->ShowPic(1);
		pCurrent = m_wndSend;
		m_wndSend->Move(m_wndLook->GetX(),m_wndLook->GetY());
	}
	else
	{
		m_wndLook->SetShow(1);
		m_wndSend->SetShow(0);
		m_wndLook->ShowPic(1);
		m_wndSend->ShowPic(0);
		pCurrent = m_wndLook;
		m_wndLook->Move(m_wndSend->GetX(),m_wndSend->GetY());
	}
}

void Cg3ms_page::Reply(int current)
{
	if(m_wndSend)
	{
		m_wndSend->SetSendTo(g_OtherData.GetLeaveWordsVector().at(current).strCharName.c_str());
	}
}
void Cg3ms_page::Forward(int current)
{
	if(m_wndSend)
	{
		m_wndSend->SetPicString(g_OtherData.GetLeaveWordsVector().at(current).strPic.c_str());
		m_wndSend->SetMusicString(g_OtherData.GetLeaveWordsVector().at(current).strMusic.c_str());
		m_wndSend->SetSendText(g_OtherData.GetLeaveWordsVector().at(current).strMessage.c_str());
	}
}
void Cg3ms_page::Show(int s)
{
	if(pCurrent == m_wndLook)
	{
		m_wndLook->Show(s);
	}
	else
	{
		m_wndSend->Show(s);
	}
}
