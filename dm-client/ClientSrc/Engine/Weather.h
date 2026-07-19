#pragma once

#include "Define.h"
#include "TextureD3D.h"
#include "Global/Interface/WeatherInterface.h"


//天气效果类
class CWeather : public CWeatherInterface
{
public:
	class CSandStorm	// 沙尘暴
	{
	public:
		CSandStorm();
		void Update(DWORD dwElapsedTime);
		inline DWORD GetColor(){return m_dwColor;}
		inline int	  GetDist(){return m_iDist;}
		void SetSpeed(float fSpeed) {	m_fSpeed = fSpeed; };
	private:
		float	m_fSpeed;
		DWORD	m_dwSandState;
		DWORD	m_dwLifeTime;
		DWORD	m_dwLastUpdateTime;
		DWORD	m_dwColor;
		int		m_iDist;
	};

	//------------------------------------------------------------------------------------------
	//地图上海浪发生器产生的海浪
	//由地图的海浪列表来维护，有一定的生命周期，并且可以根据玩家的相对位置决定是否提前结束生命
	//在生命周期结束的时候将通知地图上的海浪发生器，是海浪发生器重新处于工作状态。
	//------------------------------------------------------------------------------------------
	class CSeaTide
	{
	public:
		CSeaTide();
		void	StartTide(DWORD dwType,int iInitSize,int iStartX,int iStartY,byte cDir,int iOwnerX,int iOwnerY);//启动海浪
		void	Update(int iPlayerX,int iPlayerY); //海浪运动过程，判断是否结束生命周期（变淡，玩家离开），改变海浪状态
		void	LifeOver();//强制结束海浪生命周期
		int		GetLifeTime(){return m_iLifeTime;}
		void	Draw(int iSx1,int iSy1,int iSx2,int iSy2,DWORD dwColor);

	private:
		DWORD	m_dwType; //海浪类型，只有近海和远海之分，近海做简谐振动，速度较快，远海做匀速运动较慢
		int		m_iSize;  //海浪尺寸
		int		m_iX0,m_iY0;//海浪起始位置
		int		m_iX,m_iY;//海浪位置（像素坐标）
		int		m_iOwnerX,m_iOwnerY;//产生该海浪的发生器的位置（格子坐标），用于在生命周期结束的时候能够通知海浪发生器开始工作
		int		m_iLifeTime;//从0增加到255，到255则生命周期完结,-1表示海浪没有启动
		BYTE	m_cDir;//海浪方向
	};

	class CSeaTideList : public CWeatherInterface::CSeaTideListInterface
	{
	public:
		CSeaTideList();
		void NewTide(DWORD dwType,int iInitSize,int iStartX,int iStartY,BYTE cDir,int iOwnerX,int iOwnerY);//生成一个新的海浪，如果达到上限，则去掉一个生命时间最长的海浪
		void UpdateDraw(int iPlayerX,int iPlayerY,int x,int y,int w,int h);//更新所有海浪对象
	private:
		int				m_iMaxTideNum;
		vector<CSeaTide> m_aTides;
	};
public:
	CWeather();
	~CWeather();
	bool Create(int iWidth,int iHeight);			// 参数为加入天气效果的屏幕区域

	// 公共属性控制
	void EnableParticle(int type);					// 粒子类型
	int  GetParticleType() { return m_iType; }
	void SetNumber(int iElem);						// 粒子数量
	int  GetNumber() { return m_iElem; }
	void SetLife(int iLife) { m_iLife = iLife; }	// 粒子生存周期
	int  GetLife() { return m_iLife; }
	void SetGenChance(int iChance);					// 新粒子生成概率
	int  GetGenChance() { return m_iGenChance; }

	CSeaTideListInterface* GetSeaTideList(){ return &m_SeaTideList; }

	// RAIN属性控制
	void SetSpeed(int iSpeed) { m_iSpeed = iSpeed; }					// 速度
	int  GetSpeed() { return m_iSpeed; }	
	void SetSize(int sx,int sy) { m_iSizeX = sx; m_iSizeY = sy; }		// 形状
	void GetSize(int&sx,int&sy) { sx = m_iSizeX; sy = m_iSizeY; }

	// SNOW属性控制
	void SetSpeed(int sx,int sy) { m_iSpeedX = sx; m_iSpeedY = sy; }	// 速度
	void GetSpeed(int&sx,int& sy) { sx = m_iSpeedX; sy = m_iSpeedY; }
	void SetSize(int iSize) { m_iSize = iSize; }						// 大小
	int	 GetSize() { return m_iSize; }

	// 雾化属性	
	void SetStormSpeed(int iSpeed = 0){	m_SandStorm.SetSpeed( 10.f*(100+iSpeed)/100 ); }
	void SetDensity(int i) { m_iDensity = i; }
	int GetDensity() { return m_iDensity; }

	void SetWeatherColor(DWORD dw) { m_dwWeatherColor = dw; }
	DWORD GetWeatherColor() { return m_dwWeatherColor; }

	// 粒子移动
	void MoveAsNeed(int x,int y);

	// 显示
	void DrawGround(int iX,int iY,int offx,int offy);
	void DrawAir(int iX,int iY,int offx,int offy);
	void DrawCloud(int iX,int iY,int iOffX,int iOffY,DWORD dwColor);                  // 绘制云彩

private:
	int m_iWidth,m_iHeight;							// 加入天气效果的屏幕区域
	int m_iType;									// 天气效果类型，NONE表示没有天气效果，目前支持SNOW和RAIN
	int m_iElem;									// 粒子数量
	int m_iLife;									// 粒子生存周期，单位为帧
	int m_iGenChance;								// 新粒子产生概率，为百分比数
	bool m_bSkip;

	// RAIN 参数
	int m_iSpeed;									// 速度，为百分比数
	int m_iSizeX,m_iSizeY;							// 粒子的形状的大小

	// SNOW 参数
	int m_iSpeedX,m_iSpeedY;						// 粒子速度，单位为象素
	int m_iSize;									// 粒子最大尺寸

	// 雾化和沙尘暴属性
	DWORD m_iDensity;
	DWORD m_dwWeatherColor;

	int m_iOldX,m_iOldY;

	// 粒子纹理
	LPTexture m_pRainTex;
	LPTexture m_pSnowTex;			
	int m_iRainTexW,m_iRainTexH;					// 粒子纹理尺寸

	// 粒子构造
	struct SParticle 
	{
		SParticle()
		{
			x = y = 0;
			life = 0;
			speedx = speedy = 0;
			lengthx = lengthy = 0;
			size = 0;
			angle = speedangle = 0;
		}
		int x,y;									// 粒子屏幕位置
		int life;									// 粒子生存周期
		int speedx,speedy;							// 粒子速度，单位为象素
		// RAIN 参数
		int lengthx,lengthy;						// 粒子形状
		// SNOW 参数
		int size;									// 粒子尺寸
		int angle,speedangle;						// 粒子自身旋转速度，和当前角度
	};
	vector<SParticle> m_sElem;

	//函数
	void DrawRain(bool bUnder);						// 显示RAIN
	void DrawSnow(bool bUnder);						// 显示SNOW
	//void DrawCloud(int iX,int iY,int iOffX,int iOffY,DWORD dwColor);                  // 绘制云彩
	void DrawStorm(int iX,int iY,int iOffX,int iOffY,DWORD dwColor);				  // 用这个控制沙尘暴运动
	void DrawFlowerType1(int iFlower,bool bUnder);
	void DrawFlowerType2(int iFlower);
	void DrawFlowerType3(int iFlower);

	CSandStorm		m_SandStorm;
	CSeaTideList	m_SeaTideList;
};

extern CWeather * g_pEWeather;