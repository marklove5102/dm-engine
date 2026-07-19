#include "global/global.h"
#include "global\PerfCheck.h"
#include "ConfigData.h"

#ifdef _DEBUG
#include "global/new.h"
#define new DEBUG_NEW
#endif

CConfigData g_Config;
CConfigData g_TempConfig;

CConfigData::CConfigData()
:m_wOriColor(0x0FA4)
{
	m_iWndNum = -1;
	//声音
	m_iSndVol = 80;
	m_bSndOn  = true;
	m_iMusicVol = 80;
	m_bMusicOn = true;

	m_iBkMusicVol = 80;
	m_bBkMusic = true;

	//游戏配置
	m_bFloodOn = true;
	m_iLightOn =1;  //0：关 1：开  2：灰色
	m_iWeatherOn =1;
    m_bCheckQuick = true;
	m_bRainbowOn = true;
	//图像设置
	m_iTransparence = 25;	//透明度大小0-50（每个像素表示2）
    //m_bCanTrans = true;		//可以设置透明度，否则灰色
	m_iMsgSpeed = 0;
	m_dwTrans = 0xFFFFFFFF;

	//m_b3DMode		= true;
	m_iWindowWidth	= 1024;
	m_iRGB32		= 1;
	m_bAllWndMode	= true;
	m_i2DShadow		= 1;
	m_iMouse		= 1;
	m_bDefault		= true;

	m_bSelfAlpha	= true;
	m_bAutoPath		= false;
	m_bSkipCheck	= true;
	//m_bVSync		= false;
	m_iSmooth		= 1;

	m_bGM			= false;
	m_bColorHP		= false;
	m_bHalfSpeed	= false;
	m_bChase		= true;

	m_bEnable		= false;

	m_iColorStyle	= 0;

	m_bAssistOn		= true;
	ZeroMemory(m_dwCurColor,sizeof(m_dwCurColor));

	m_bHusbandAndWife = true;
	m_bShaMember   = true;
	m_bOpenWing = true;
	m_bTempMask = false;
	m_bOpenShine = true;
	m_bShowBlock = false;
	m_bEnableMagic = true;
	m_bShowTalk = true;
	m_bShowTestFontText = false;
	m_bEnablePushAction = true;

	m_bShowUIHelp = false;

	m_strTempMaskStr.clear();
	m_strTitleStr.clear();

	m_bAutoCombine = false;
}

CConfigData::~CConfigData()
{

}

void CConfigData::SetCurColor(DWORD dwColor)
{
	//需要改写
	for(int i = 0; i < LIGHTING_NUM; ++i)
	{
		DWORD dwCurColor = dwColor;
		switch(i)
		{
		case LIGHTING_NORMAL:
			dwCurColor	= 0xF0F09F3F;
			break;
		case LIGHTING_DARK:
			dwCurColor	= 0xF08B5C25;
			break;
		case LIGHTING_HILIGHT:
			dwCurColor	= 0xF0F4C48A;
			break;
		case LIGHTING_OPS:
			dwCurColor	= 0xF03F90F0;
			break;
		}
		m_dwCurColor[i] = dwCurColor;
	}
}

DWORD CConfigData::GetCurColor(LIGHTING_COLOR_TYPE eLight)
{
	if(eLight<0||eLight>LIGHTING_NUM)
		return 0xFFFFFFFF;

	return m_dwCurColor[eLight];
}

void CConfigData::ParseStrToken(std::string str,std::vector<std::string>& vec)
{
	vec.clear();

	while(str.size() > 0)
	{
		int iPos = str.find_first_of('/');
		if(iPos < 0)
			iPos = str.find_first_of('\\');

		if(iPos > 0)
		{
			vec.push_back(str.substr(0,iPos));
			str = str.substr(iPos+1,str.size() - iPos - 1);
		}
		else
		{
			vec.push_back(str);
			break;
		}
	}
}