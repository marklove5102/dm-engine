#pragma once

enum ESeaType
{
	SEATYPE_NEAR = 0,
	SEATYPE_FAR	 = 1,
};

enum ESeaMask
{
	SEAMASK_HASTIDE	= 0x80,
	SEAMASK_TYPE	= 0x40,
	SEAMASK_ON		= 0x20,
	SEAMASK_FRECRUNCY=0x18,
	SEAMASK_DIR		= 0x07,
};

// 粒子效果枚举类型
enum EParticleType
{
	PARTICLE_NONE		= 0 ,	// 无天气效果
	PARTICLE_RAIN		= 1 ,	// 雨天气效果
	PARTICLE_CLOUD,
	PARTICLE_STORM,
	PARTICLE_SNOW,	// 雪天气效果
	PARTICLE_FLOWER1,
	PARTICLE_FLOWER2,
	PARTICLE_FLOWER3,
	PARTICLE_FLOWER4,
	PARTICLE_FLOWER5,
	PARTICLE_FLOWER6,
	PARTICLE_FLOWER7,
	PARTICLE_FLOWER8,
	PARTICLE_FLOWER9,
};

class CWeatherInterface
{	
public:
	virtual ~CWeatherInterface(){}
	class CSeaTideListInterface
	{
	public:		
		virtual void NewTide(DWORD dwType,int iInitSize,int iStartX,int iStartY,BYTE cDir,int iOwnerX,int iOwnerY) = 0;//生成一个新的海浪，如果达到上限，则去掉一个生命时间最长的海浪
		virtual void UpdateDraw(int iPlayerX,int iPlayerY,int x,int y,int w,int h) = 0;//更新所有海浪对象	
	};
	
	virtual bool Create(int iWidth,int iHeight) = 0;			// 参数为加入天气效果的屏幕区域

	// 公共属性控制
	virtual void EnableParticle(int type) = 0;					// 粒子类型
	virtual int  GetParticleType() = 0;
	virtual void SetNumber(int iElem) = 0;						// 粒子数量
	virtual int  GetNumber() = 0;
	virtual void SetLife(int iLife) = 0;	// 粒子生存周期
	virtual int  GetLife() = 0;
	virtual void SetGenChance(int iChance) = 0;					// 新粒子生成概率
	virtual int  GetGenChance() = 0;

	virtual CSeaTideListInterface* GetSeaTideList() = 0;

	// RAIN属性控制
	virtual void SetSpeed(int iSpeed) = 0;					// 速度
	virtual int  GetSpeed() = 0;	
	virtual void SetSize(int sx,int sy) = 0;		// 形状
	virtual void GetSize(int&sx,int&sy) = 0;

	// SNOW属性控制
	virtual void SetSpeed(int sx,int sy) = 0;	// 速度
	virtual void GetSpeed(int&sx,int& sy) = 0;
	virtual void SetSize(int iSize) = 0;					// 大小
	virtual int	 GetSize() = 0;

	// 雾化属性	
	virtual void SetStormSpeed(int iSpeed = 0) = 0;
	virtual void SetDensity(int i) = 0;
	virtual int GetDensity() = 0;

	virtual void SetWeatherColor(DWORD dw) = 0;
	virtual DWORD GetWeatherColor() = 0;

	// 粒子移动
	virtual void MoveAsNeed(int x,int y) = 0;

	// 显示
	virtual void DrawGround(int iX,int iY,int offx,int offy) = 0;
	virtual void DrawAir(int iX,int iY,int offx,int offy) = 0;
	virtual void DrawCloud(int iX,int iY,int iOffX,int iOffY,DWORD dwColor) = 0;                  // 绘制云彩
};

extern CWeatherInterface *   g_pWeather;
