#pragma once

#include "Global/Global.h"
#include "GameDefine.h"
#include "GameData/Good.h"

struct _Tag
{
	_Tag()
	{
		color = COLOR_DEFAULT;
		dwGoodLooks = 0;
		iOffX = iOffY = 0;
		iDrawX = 0;
		iDrawY = 0;
	}
	std::string text;
	std::string comm;
	std::string strGoodName;//物品名称
	DWORD dwGoodLooks;//有些地方要显示一张图，张不是物品图，那么给出他的looks，里面包括图包信息
	DWORD  color;
	int iOffX,iOffY;//绘制偏移
	int iDrawX,iDrawY;//绘制起始位置
};

class CTagText
{
public:

	CTagText(void);
	~CTagText(void);

	void  Parse(string& str,int nPos = 0,const char* sLR = "\\",int iLimit = -1,DWORD dwDefaultColor = 0);
    bool  ParsePic(const string& str,int nLeft,int nRight,int j,size_t &nLinePos,int iOffX,int iOffY);// nLeft,nRight为"<"及">"的位置
	void  setText(string& str);
	void  Clear();

	int  GetRow();
	int  GetColumn(int n);
	bool IsCommand(int row,int col);
	const char *  GetString(int row,int col);
	DWORD GetColor(int row,int col);
	const char *  GetCommand(int row,int col);
    bool  IsGoodName(int row,int col);
    const char*  GetTagGoodName(int row,int col);
    bool  IsGoodLooks(int row,int col);
    DWORD GetTagGoodLooks(int row,int col);
	_Tag*  GetAt(int row,int col);
	void ReplaceAll(const string &src,const string &dest);

    static map<std::string,CGood>  sm_GoodMap;    //NPC对话中提到的物品
private:
	vector<vector<_Tag> >		m_content;                 // NPC对话框 或者 公告板数据
};

class CTagTextMgr
{
public:
	CTagTextMgr();
	~CTagTextMgr();


	CTagText&     GetNpcText()  { return m_NpcText; }
	CTagText&     GetNpcText2()  { return m_NpcText2; }

	CTagText&     GetCommonData()  { return m_commonData; }
	CTagText&     GetBookData()   { return m_bookData; }
	CTagText&     GetGuideData()  { return m_guideData; }
	CTagText&     GetBoard(){ return m_boardData; }
	CTagText&     GetActData(){ return m_ActData; }
	CTagText&     GetInvestData(){ return m_InvestData; }
	CTagText&     GetHelpData(){ return m_HelpData; }
	CTagText&     GetGreetingData(){ return m_GreetingData; }
	CTagText&     GetGreetingDownloadData(){ return m_GreetingDownloadData; }
	CTagText&     GetRadioData(){ return m_RadioData; }
	CTagText&	  GetPromptInfo(){ return m_PromptInfo;}
	CTagText&	  GetQiYuText(){ return m_QiYuText;}
	CTagText&     GetYiHuoZhanMsgText()  { return m_YiHuoZhanMsg; }

	void Clear();

private:
	//NPC对话,显示等信息
	CTagText                m_NpcText;
	CTagText                m_commonData;//用于CommonHelpWnd中显示NPC文本
	CTagText                m_bookData;//传世要闻
	CTagText                m_guideData;//新手帮助
	CTagText                m_boardData;//公告版
	CTagText                m_NpcText2;//第二个,一个界面分两块的情况下用到,两块有可能都有滚动条
	CTagText                m_NewBieData;//给新玩家的提示信息
	CTagText                m_ActData;//给新玩家的提示信息
	CTagText                m_InvestData;//给新玩家的提示信息
	CTagText                m_HelpData;//给新玩家的提示信息
	CTagText                m_GreetingData;//greetingmsg刚进游戏时的欢迎界面
	CTagText                m_GreetingDownloadData;//greetingmsg下载信息
	CTagText                m_RadioData;//电台信息
	CTagText				m_PromptInfo;//提示信息
	CTagText				m_QiYuText;//奇遇
	CTagText				m_YiHuoZhanMsg;//异火战消息

};

extern CTagTextMgr g_TagTextMgr;  //标记文本编辑
