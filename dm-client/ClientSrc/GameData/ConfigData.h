#pragma once

#include "Global/Global.h"
#include "BaseClass/Control/ControlDefine.h"

enum LIGHTING_COLOR_TYPE
{
	LIGHTING_NORMAL		= 0,
	LIGHTING_DARK,
	LIGHTING_HILIGHT,
	LIGHTING_OPS,
	LIGHTING_NUM,
};

class CConfigData
{
public:
	CConfigData();
	~CConfigData();

	inline int		GetColorStyle()	{return m_iColorStyle;}
	inline void		SetColorStyle(int i) {m_iColorStyle = i;}
	DWORD	GetCurColor(LIGHTING_COLOR_TYPE eLight);
	void	SetCurColor(DWORD dwColor);

	int GetWndNum()						{return m_iWndNum;}
	void SetWndNum(int Num)				{m_iWndNum = Num;}

	int GetSndVol()		{return m_iSndVol;}
	void SetSndVol(int i)	{ m_iSndVol=i;}

	bool GetSndOn()				{return m_bSndOn;}
	void SetSndOn(bool b)		{m_bSndOn =b;}	

	int GetMusicVol()	{return m_iMusicVol;}
	void SetMusicVol(int i)		{m_iMusicVol = i;}

	bool GetMusicOn()	{return m_bMusicOn;}
	void SetMusicOn(bool b)		{m_bMusicOn = b;}

	bool GetBkMusicOn() {return m_bBkMusic;}
	void SetBkMusicOn(bool b){m_bBkMusic = b;}

	int GetBkMusicVol() {return m_iBkMusicVol;}
	void SetBkMusicVol(int i){m_iBkMusicVol = i;}

	bool GetFloodOn()			{return m_bFloodOn;}
	void SetFloodOn(bool b)		{m_bFloodOn =b;}

	bool GetRainbowOn()          {return m_bRainbowOn;}
	void SetRainbowOn(bool b)    {m_bRainbowOn = b;}

	int	GetLight()				{return m_iLightOn;}
	void SetLight(int i)		{m_iLightOn= i;}

	int	GetWeather()			{return m_iWeatherOn;}
	void SetWeather(int i)		{m_iWeatherOn =i;}

	int GetTrans()				{return m_iTransparence;}
	void SetTrans(int i)		{m_iTransparence =i;}

	DWORD GetTransColor()		{return m_dwTrans;}
	void SetTransColor(DWORD i)	{m_dwTrans =i;}

	//bool GetCanTrans()			{return m_bCanTrans;}
	//void SetCanTrans(bool b)	{m_bCanTrans=b;}

	int GetWindowWidth()			{return m_iWindowWidth;}
	void SetWindowWidth(int i)	{m_iWindowWidth = i;}

	int GetRGB()				{return m_iRGB32;}
	void SetRGB(int i)			{m_iRGB32 =i;}

	bool GetAllWndMode()		{return m_bAllWndMode;}
	void SetAllWndMode(bool b)	{m_bAllWndMode=b;}

	int Get2DShadow()			{return m_i2DShadow;}
	void Set2DShadow(int i)		{m_i2DShadow =i;}

	int GetMouse()				{return m_iMouse;}
	void SetMouse(int i)		{ m_iMouse= i;}

	bool GetDefault()			{return m_bDefault;}
	void SetDefault(bool b)		{m_bDefault =b;}

	bool GetSelfAlpha()			{return m_bSelfAlpha;}
	void SetSelfAlpha(bool b)	{m_bSelfAlpha =b;}

	bool GetAutoPath()			{return m_bAutoPath;}
	void SetAutoPath(bool b)	{m_bAutoPath =b;}

	int GetTexPrepare()			{return m_iTexPrepare;}
	void SetTexPrepare(int i)	{m_iTexPrepare = i;}

	int GetMsgSpeed()			{ return m_iMsgSpeed;}
	void SetMsgSpeed(int i)		{ m_iMsgSpeed = i;}

	bool GetSkipCheck()			{return m_bSkipCheck;}
	void SetSkipCheck(bool b)	{m_bSkipCheck=b;}

	//bool GetVSync()				{return m_bVSync;}
	//void SetVSync(bool b)		{m_bVSync=b;}

	int GetSmooth()				{return m_iSmooth;}
	void SetSmooth(int i)		{m_iSmooth=i;}

	bool GetColorHP()			{return m_bColorHP;}
	void SetColorHP(bool b)		{m_bColorHP=b;}
	bool GetHalfSpeed()			{return m_bHalfSpeed;}
	void SetHalfSpeed(bool b)	{m_bHalfSpeed=b;}
	bool GetChase()				{return m_bChase;}
	void SetChase(bool b)		{m_bChase=b;}

	bool GetGM()				{return m_bGM;}
	void SetGM(bool b)			{m_bGM=b;}

	bool GetEnable()			{return m_bEnable;}
	void SetEnable(bool b)		{m_bEnable=b;}

    bool GetCheckQuick()        {return m_bCheckQuick;}
    void SetCheckQuick(bool b)  {m_bCheckQuick = b;}

	//个性化设置
	bool GetHusbandAndWife()    {return m_bHusbandAndWife;  }
	void SetHusbandAndWife(bool b) { m_bHusbandAndWife = b; }

	bool GetShaMember()         {return m_bShaMember;    }
	void SetShaMember(bool b)   { m_bShaMember = b;      }

	bool GetOpenWing()          {return m_bOpenWing;  }
	void SetOpenWing(bool b)    { m_bOpenWing = b;    }

	bool GetTempMask()          {return m_bTempMask;   }
	void SetTempMask(bool b)    { m_bTempMask = b;    }

	const char* GetTitleStr()         {    return m_strTitleStr.c_str(); }
	void  SetTitleStr(const char* str) {   m_strTitleStr = str;  }

	bool GetOpenShine()			{return m_bOpenShine;}
	void SetOpenShine(bool b)   {m_bOpenShine = b;}

	const char* GetTempMaskStr()     {    return m_strTempMaskStr.c_str();  }
	void  SetTempMaskStr(const char* str) {   m_strTempMaskStr = str;  }

	void  ParseStrToken(std::string str,std::vector<std::string>& vec);

	bool IsShowBlock(){return  m_bShowBlock;}   //是否显示阻挡点
	bool IsEnableMagic(){return m_bEnableMagic;} //是否创建魔法
	bool IsShowTalk(){return   m_bShowTalk;}    //是否显示聊天内容
	bool IsShowTestFontText(){return   m_bShowTestFontText;}    //是否显示聊天内容
	bool IsEnablePushAction(){return m_bEnablePushAction;} //是否创建动作

	void SetShowBlock(bool b){m_bShowBlock = b;}  
	void SetEnableMagic(bool b){m_bEnableMagic = b;} 
	void SetShowTalk(bool b){m_bShowTalk = b;}   
	void SetShowTestFontText(bool b){m_bShowTestFontText = b;}   
	void SetEnablePushAction(bool b){m_bEnablePushAction = b;} 

	bool GetShowUIHelp(){return m_bShowUIHelp;}
	void SetShowUIHelp(bool b){m_bShowUIHelp = b;}


	bool IsAutoCombine(){return m_bAutoCombine;}
	void SetAutoCombine(bool b){m_bAutoCombine = b;}

	bool IsAutoShowActivityLog(){return m_bAutoShowActivatyLog;}
	void SetAutoShowActivityLog(bool bShow){m_bAutoShowActivatyLog = bShow;}

private:
	int     m_iWndNum;
//声音（音效）
	int		m_iSndVol;
	bool	m_bSndOn;
	int		m_iMusicVol;
	bool	m_bMusicOn;

	bool	m_bBkMusic;
	int		m_iBkMusicVol;
//游戏配置
	bool	m_bFloodOn;
	int		m_iLightOn;
	int		m_iWeatherOn;
	int		m_iTexPrepare;//0:关1:智能 2：完全
	int		m_iMsgSpeed;
	bool	m_bAssistOn;	//帮助精灵 0:关 1:开
    bool    m_bCheckQuick;
	bool    m_bRainbowOn;

//图像设置
	int		m_iTransparence;	//透明度大小0-50（每个像素表示2）
	//bool    m_bCanTrans;		//可以设置透明度，否则灰色
	DWORD	m_dwTrans;
	int		m_iColorStyle;
	DWORD	m_dwCurColor[LIGHTING_NUM];
	const WORD	 m_wOriColor;

	int	    m_iWindowWidth;
	int		m_iRGB32;
	bool	m_bAllWndMode;
	int		m_i2DShadow;
	int		m_iMouse;
	bool	m_bDefault;

	bool	m_bSelfAlpha;
	bool	m_bAutoPath;
	bool	m_bSkipCheck;
	//bool	m_bVSync;
	int		m_iSmooth;

	bool	m_bColorHP;
	bool	m_bHalfSpeed;
	bool	m_bChase;
	bool	m_bGM;
	bool	m_bEnable;
	//个性化设置
	bool    m_bHusbandAndWife;
	bool    m_bShaMember;
	bool    m_bTempMask;
	bool    m_bOpenWing;
	bool    m_bOpenShine;	//是否开启第二元神的闪光效果
	bool    m_bShowBlock;   //是否显示阻挡点,测试用
	bool    m_bEnableMagic; //是否创建魔法,测试用
	bool    m_bShowTalk;    //是否显示聊天内容,测试用
	bool    m_bShowTestFontText;//是否显示测试字体文字,测试用
    bool    m_bEnablePushAction;//是否创建动作

	bool    m_bShowUIHelp;	//界面功能按钮说明

	std::string m_strTempMaskStr;
	std::string m_strTitleStr;

	bool m_bAutoCombine;//内部自动合并矿石用
	bool m_bAutoShowActivatyLog;
};

//系统设置数据
//临时系统设置数据，在设置时按下确定后存入m_Config，有些数据要等服务器确认设置成功后再存入m_Config

extern CConfigData g_Config;
extern CConfigData g_TempConfig;
