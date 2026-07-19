#pragma once

#include "BaseClass/Control/CtrlWindowX.h"
#include "BaseClass/Control/CtrlMultiEdit.h"


class CLookLeaveWordsWnd:public CCtrlWindow
{
public:
	CLookLeaveWordsWnd(void);
	~CLookLeaveWordsWnd(void);


	bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual void Draw(void);	
	void OnClickCloseButton();

	bool Create(CControl * pParent,int iX,int iY);
	void OnCreate(void);
	void SetCurLeaveWord(int nCurLeaveWord);

	virtual void OnMove();
//	void ShowPic(int n){m_picshow.Show(n);};
	void ShowPic(int n){};
	const char* GetPic();
	void Show(int s);
	void PlayPic();
	void PlayMusic();

	void SaveFile();
protected:	
	CCtrlButton * m_pPre;
	CCtrlButton * m_pNext;
	CCtrlButton * m_pDelete;
	CCtrlButton * m_pClearAll;
	CCtrlButton * m_pReply;
	CCtrlButton * m_pForward;
	CCtrlButton * m_pPhrase;

	int m_nCurLeaveWord;
	CCtrlMultiEdit		*m_pMultiEdit;

	CCtrlScroll *m_pScroll;
	//g3ms
	//CDShow				m_picshow;
	//CDShow				m_music;
	int                 m_picxoff;
	int					m_picyoff;
	int					m_picw,m_pich;
	CCtrlButton * m_pPlay;
	CCtrlButton * m_pPause;
	CCtrlButton * m_pStop;
	CCtrlButton * m_pPic;
	CCtrlButton * m_pMusic;
	CCtrlButton * m_pSendPage;
	CCtrlButton * m_pBigpic;
	CCtrlButton * m_pSaveas;

	LPTexture			m_pHave;
	CCtrlButton			*m_pClose;

	bool GetSaving();
	void SaveAS();
	bool downloading;
	string urlpic;
	string urlmusic;
	string name;
	HANDLE hSave;
};

class Cg3ms_send : public CCtrlWindow
{
public:
	Cg3ms_send(void);
	~Cg3ms_send(void);

	bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual void Draw(void);	
	void OnClickCloseButton();

	bool Create(CControl * pParent,int iX,int iY);
	void OnCreate(void);
	void SetCurLeaveWord(int nCurLeaveWord);

	virtual void OnMove();

	void SetPicString(const char *str);
	void SetMusicString(const char *str);
	void SetSendTo(const char * str){m_pEdit->SetText(str);};
	void SetSendText(const char * str){m_pMultiEdit->SetText(str);};
	void ShowPic(int n);
	void Show(int s);
	void PlayPic();
	void PlayMusic();
protected:	
	CCtrlButton * m_pOk;

	int m_nCurLeaveWord;
	CCtrlMultiEdit		*m_pMultiEdit;
	CCtrlEdit			*m_pEdit;

	//g3ms
	//CDShow				m_picshow;
	//CDShow				m_music;
	int                 m_picxoff;
	int					m_picyoff;
	int					m_picw,m_pich;
	CCtrlButton * m_pPlay;
	CCtrlButton * m_pPause;
	CCtrlButton * m_pStop;
	CCtrlButton * m_pPic;
	CCtrlButton * m_pMusic;
	CCtrlButton * m_pLookPage;
	CCtrlButton			*m_pClose;
	CCtrlButton			*m_pHelp;

	std::string				m_picstr;
	std::string				m_musicstr;

	bool IsMobil();
	bool GetDownpic();
	bool GetDownmusic();
};

class Cg3ms_pic :
	public CCtrlWindowX
{
	DECLARE_WND_POSX(Cg3ms_pic)

public:
	Cg3ms_pic(void);
	~Cg3ms_pic(void);

	bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual void Draw(void);	
	void OnClickCloseButton();

	bool Create(CControl * pParent,int iX,int iY);
	void OnCreate(void);

	const char * GetPic();
protected:	
	CCtrlEdit *m_pEdit;
	CCtrlButton * m_pOk;
	CCtrlButton * m_pCancle;
	CCtrlButton			*m_pClose;

};

class Cg3ms_music :
	public CCtrlWindowX
{
	DECLARE_WND_POSX(Cg3ms_music)

public:
	Cg3ms_music(void);
	~Cg3ms_music(void);

	bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual void Draw(void);	
	void OnClickCloseButton();

	bool Create(CControl * pParent,int iX,int iY);
	void OnCreate(void);

	const char *GetMusic();
protected:	
	CCtrlEdit *m_pEdit;
	CCtrlButton * m_pOk;
	CCtrlButton * m_pCancle;
	CCtrlButton			*m_pClose;

};

class Cg3ms_bigpic :
	public CCtrlWindowX
{
	DECLARE_WND_POSX(Cg3ms_bigpic)

public:
	Cg3ms_bigpic(void);
	~Cg3ms_bigpic(void);

	bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual void Draw(void);	
	void OnClickCloseButton();

	bool Create(CControl * pParent,int iX,int iY);
	void OnCreate(void);
	virtual void OnMove();
	void PlayFile(const char *sfile);
	//void Show(int s){m_picshow.Show(s);};
	void Show(int s){}
protected:
	//CDShow				m_picshow;
	int                 m_picxoff;
	int					m_picyoff;
	int					m_picw,m_pich;

	CCtrlButton * m_pOk;
	CCtrlButton * m_pCancle;
	CCtrlButton * m_pClose;
};

class Cg3ms_page :
	public CCtrlWindowX
{
	DECLARE_WND_POSX(Cg3ms_page)

public:
	Cg3ms_page(void);
	~Cg3ms_page(void);

	bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl);
	virtual void Draw(void);	

	void OnCreate(void);
	void SetPage(int page);

	void Reply(int current);
	void Forward(int current);
	void SetPicString(const char *str){m_wndSend->SetPicString(str);};
	void SetMusicString(const char *str){m_wndSend->SetMusicString(str);};
	void SetCurLeaveWord(int n){m_wndLook->SetCurLeaveWord(n);};
	CControl* GetTop(){return pTop;};
	const char *GetPic(){return m_wndLook->GetPic();};
	void Show(int s);
protected:
	CLookLeaveWordsWnd  *m_wndLook;
	Cg3ms_send			*m_wndSend;
	CControl            *pCurrent;
	CControl			*pTop;
};
