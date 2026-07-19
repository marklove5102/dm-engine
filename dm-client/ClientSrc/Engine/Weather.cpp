#include "stdafx.h"
#include "weather.h"
#include "GraphicD3D.h"
#include "Light.h"
#include "TexManager.h"
#include "Global/MathUtil.h"
#include "Global/Interface/Callbackinterface.h"

extern CCallBackInterface * g_pCallBack;


#define DEFAULT_CLOUD_COLOR	0xFFEFEFEF	//云的颜色
#define DEFAULT_STORM_COLOR	0xFFC8AA70	//风暴的颜色

CWeather::CSandStorm::CSandStorm()
{
	m_dwSandState = 0;
	m_dwLifeTime = 0;
	m_dwLastUpdateTime = 0;
	m_dwColor = 0;
	m_iDist = 0;
	m_fSpeed = 10.0f;
}

//沙尘暴,更新沙尘暴状态
void CWeather::CSandStorm::Update(DWORD dwElapsedTime)
{
	DWORD dwTime = dwElapsedTime - m_dwLastUpdateTime;
	m_iDist += int(float(dwTime)*m_fSpeed*0.1);
	m_iDist = m_iDist % 2048;
	m_dwLastUpdateTime = dwElapsedTime;	
}

//海浪效果类
//以下内容按照从高位到低位的顺序：
//是否产生海浪                 1bit  （0代表无，1代表有）
//海浪类型（近海/远海）        1bit  （0代表近海，1代表远海）
//目前是否启动                 1bit  （1代表已经启动，0代表没有启动）
//发生频率(1*5%,2*5%,3*5%,4*5%)2bit  （0代表5%,1代表10%,2代表15%,3代表20%）
//方向（8方向）                3bit  （0代表向上，1右上，2右...以此类推）
//
//根据主人公位置，每次循环确定海浪发生器运作。
//生成的海浪用一个固定的数组记录，限制最高海浪数。
//每次循环计算海浪运动规律，如果人物离开一定的范围自动消失。
//根据海浪列表，渲染海浪。

CWeather::CSeaTide::CSeaTide()
{
	m_iLifeTime = -1;
}

//	初始化海浪
void CWeather::CSeaTide::StartTide(DWORD dwType,int iInitSize,int iStartX,int iStartY,byte cDir,int iOwnerX,int iOwnerY)
{
	if (m_iLifeTime >= 0 && m_iLifeTime <= 255)
		LifeOver();

	m_dwType = dwType;
	m_iSize = iInitSize;
	m_iX = iStartX;
	m_iY = iStartY;
	m_iX0 = iStartX;
	m_iY0 = iStartY;
	m_cDir = cDir;
	m_iOwnerX = iOwnerX;
	m_iOwnerY = iOwnerY;
	m_iLifeTime = 0;
}

//  海浪运动过程，判断是否结束生命周期（变淡，玩家离开），改变海浪状态
void CWeather::CSeaTide::Update(int iPlayerX_,int iPlayerY_)
{
	static float dd[8][2] = 
	{
		{0.0f,-1.0f},
		{0.7071f,-0.7071f},
		{1.0f,0.0f},
		{0.7071f,0.7071f},
		{0.0f,1.0f},
		{-0.7071f,0.7071f},
		{-1.0f,0.0f},
		{-0.7071f,-0.7071f}
	};

	if ( m_iLifeTime < 0 )  return;

	//判断是否离玩家足够远，如果足够远则立即结束生命周期
	if (abs(m_iX-iPlayerX_*64) > 2000 || abs(m_iY-iPlayerY_*32) > 1500  )
	{
		LifeOver();
		return;
	}

	int dist = m_iLifeTime;
	//海浪运动
	switch(m_dwType)
	{
	case SEATYPE_NEAR:
		if (m_iLifeTime>125)
		{
			dist = 250-m_iLifeTime;
			m_iLifeTime += 4;
		}
		else
		{
			m_iSize +=2;// 100 + int(float(m_iLifeTime)*0.8f);
			m_iLifeTime += 3;
		}
		m_iX = m_iX0+ int( sinf(dist/100.0f)*80.0f*dd[m_cDir][0]*0.35f );
		m_iY = m_iY0+ int( sinf(dist/100.0f)*80.0f*dd[m_cDir][1]*0.35f);
		break;
	case SEATYPE_FAR:
		m_iSize++;
		m_iLifeTime+=2;
		m_iX = m_iX0+ int((80.0f-(cosf(float(dist)/80.0f)*80.0f))*dd[m_cDir][0]*1.0f);
		m_iY = m_iY0+ int((80.0f-(cosf(float(dist)/80.0f)*80.0f))*dd[m_cDir][1]*1.0f);
		break;
	default:
		//未知的海浪类型
		break;
	}
	if (m_iLifeTime > 250)
	{
		LifeOver();
		return;
	}
}

//强制结束海浪生命周期
void CWeather::CSeaTide::LifeOver()
{
	//通知海浪发生器开始工作
	if (m_iLifeTime > 0)
	{
		if(g_pCallBack)
		{
			g_pCallBack->SeaTideLifeOver(m_iOwnerX,m_iOwnerY);
		}
	}

	m_iLifeTime = -1;
}

//绘制海浪
void CWeather::CSeaTide::Draw(int x1,int y1,int x2,int y2,DWORD dwColor)
{
	if (m_iLifeTime <= 0)
		return;

	if ((m_iX+256>x1)&&(m_iX-256<x2)&&(m_iY+256>y1)&&(m_iY-256<y2))//适当减少一些绘制
	{
		DWORD cc = 0;
		if(m_iLifeTime<=75)
			cc = m_iLifeTime*255/75;
		else if(m_iLifeTime>=175)
			cc = (250-m_iLifeTime)*255/75;
		else 
			cc = 255;

		LPTexture pTex = g_pETexMgr->GetTex(PACKAGE_effect,550,EP_NORMAL);

		if(pTex)
		{
			int x = m_iX-x1;
			int y = m_iY-y1;

			DWORD dwColor = 0xFF000000 | (cc << 16) | (cc << 8) | cc;

			g_pEGfx->DrawTideTexture(x,y,pTex,(float)m_iSize,m_cDir,dwColor);
		}
	}
}

CWeather::CSeaTideList::CSeaTideList()
{
	m_iMaxTideNum = 64;
	m_aTides.resize(m_iMaxTideNum);
}

//生成一个新的海浪，如果达到上限，则去掉一个生命时间最长的海浪
//DWORD dwType				近海/远海						0/1
//INT iInitSize			    初始尺寸						
//INT iStartX,INT iStartY	其实位置（象素坐标）			
//UCHAR cDir,				方向
//INT iOwnerX,INT iOwnerY	发生器的位置（格子坐标）
void CWeather::CSeaTideList::NewTide(DWORD dwType,int iInitSize,int iStartX_,int iStartY_,byte cDir,int iOwnerX,int iOwnerY)
{
	int iSlot = 0;
	int	iLife = 0;

	for (int i=0;i<m_iMaxTideNum;i++)
	{
		int tempLife = m_aTides[i].GetLifeTime();
		if (tempLife <= 0)
		{
			//找到一个没有使用的海浪空间
			iSlot = i;
			break;
		}

		if (tempLife>iLife)
		{
			//寻找生命时间更长的海浪
			iSlot = i;
			iLife = tempLife;
		}
	}
	m_aTides[iSlot].StartTide(dwType,iInitSize,iStartX_,iStartY_,cDir,iOwnerX,iOwnerY);
}

//更新所有海浪对象
void CWeather::CSeaTideList::UpdateDraw(int iPlayerX,int iPlayerY,int x,int y,int w,int h)
{
	for (int i=0;i < m_iMaxTideNum;i++)
	{
		m_aTides[i].Update(iPlayerX,iPlayerY);
		m_aTides[i].Draw(x,y,x+w,y+h,0xFFFFFFFF);
	}
}

// 天气效果类---CWeather
CWeather::CWeather()
{
	m_pRainTex			= NULL;
	m_pSnowTex			= NULL;
	m_iDensity			= 0;
	m_dwWeatherColor	= 0xFFFFFFFF;
}

// 析构函数
CWeather::~CWeather()
{
	//释放纹理对象
	g_pETexMgr->ReleaseTex(m_pRainTex);
	g_pETexMgr->ReleaseTex(m_pSnowTex);
}

// 天气系统创建函数
bool CWeather::Create(int iWidth,int iHeight)
{
	// 公共属性
	m_iWidth		= iWidth;
	m_iHeight		= iHeight;
	m_iType			= PARTICLE_NONE;
	m_iLife			= 0;
	m_iElem			= 100;
	m_iGenChance	= 20;
	m_iOldX			= 0;
	m_iOldY			= 0;
	m_bSkip			= false;

	// RAIN属性
	m_iSpeed		= 45;	
	m_iSizeX		= -23;
	m_iSizeY		= 120;

	// SNOW属性
	m_iSpeedX		= 2;
	m_iSpeedY		= 10;
	m_iSize			= 32;

	if( iWidth > 900 )
		m_iElem = 160;
	else
		m_iElem = 120;

	m_sElem.resize(m_iElem);
	return true;
}

// 天气效果属性设置函数
// 设置粒子类型，以及缺省生存周期，生存周期影响粒子的速度
void CWeather::EnableParticle(int type)
{
	int i;
	m_iType = type; 

	switch( m_iType ) 
	{
	case PARTICLE_RAIN:
		m_iLife = 12 * m_iHeight / 500 + 12;  //设置雨点粒子的生命周期
		m_iSpeed		= 45;	
		m_iSizeX		= -23;
		m_iSizeY		= 120;
		m_iGenChance	= 20;
		m_iElem = (m_iWidth > 900)?150:100;
		m_sElem.resize(m_iElem);
		break;

	case PARTICLE_FLOWER1:
	case PARTICLE_FLOWER2:
	case PARTICLE_FLOWER5:
		m_iLife = 120 * m_iHeight / 500;		// 花瓣雨1
		m_iSpeed		= 3;
		m_iSize			= 32;
		m_iGenChance	= 100;
		m_iElem = (m_iWidth > 900)?280:200;
		m_sElem.resize(m_iElem);

		for(i=0;i<m_iElem;i++) 
			m_sElem[i].life = (rand()%200) - 200;
		break;
	case PARTICLE_FLOWER7:
	case PARTICLE_FLOWER8:
	case PARTICLE_FLOWER9:
		m_iLife = 120 * m_iHeight / 500;		// 花瓣雨1
		m_iSpeed		= 3;
		m_iSize			= 32;
		m_iGenChance	= 100;
		m_iElem = (m_iWidth > 900)?200:150;
		m_sElem.resize(m_iElem);

		for(i=0;i<m_iElem;i++) 
			m_sElem[i].life = (rand()%200) - 200;
		break;

	case PARTICLE_FLOWER3:
	case PARTICLE_FLOWER4:
		m_iLife = 120 * m_iHeight / 500;		// 花瓣雨2
		m_iSpeed		= 3;
		m_iSize			= 32;
		m_iGenChance	= 100;
		m_iElem = (m_iWidth > 900)?180:130;
		m_sElem.resize(m_iElem);

		for(i=0;i<m_iElem;i++) 
			m_sElem[i].life = (rand()%200) - 200;
		break;
	case PARTICLE_FLOWER6:
		m_iLife = 12 * m_iHeight / 500;		// 流星雨
		m_iSpeed		= 40;
		m_iGenChance	= 100;
		m_iElem = (m_iWidth > 900)?50:30;
		m_sElem.resize(m_iElem);

		for(i=0;i<m_iElem;i++) 
			m_sElem[i].life = (rand()%200) - 200;
		break;
	}
}

// 设置粒子的数量
void CWeather::SetNumber(int iElem)
{
	m_iElem = iElem;// 更新现有的粒子数
	m_sElem.resize(m_iElem);
}

// 设置粒子出现的几率（0-100）
void CWeather::SetGenChance(int iChance)                           
{
	if(iChance > 100) iChance = 100;
	if(iChance < 0) iChance = 0;
	m_iGenChance = iChance;
}

//-------------------------------------------------------------------------
// 整体移动粒子，用于水平方向卷屏效果
//-------------------------------------------------------------------------
void CWeather::MoveAsNeed(int x,int y)
{
	if(x == m_iOldX && y == m_iOldY)
		return;

	if(m_iOldX == 0 && m_iOldY == 0) //第一次
	{
		m_iOldX = x;
		m_iOldY = y;
		return;
	}

	for(int i=0;i<m_iElem;i++)
	{
		if(m_sElem[i].life <= 0 )                          // 粒子的生命周期结束继续遍厉下一个
			continue;

		SParticle& ele = m_sElem[i];

		ele.x += m_iOldX - x;                  // 设置粒子X偏移
		ele.y += m_iOldY - y;                                  // 设置粒子Y偏移

		if(ele.x < -32 )                            // 水平滚动屏幕
			ele.x = m_iWidth + ele.x + 32;
		else if(ele.x > (m_iWidth+32) )
			ele.x = ele.x - m_iWidth - 32;
	}
	m_iOldX = x;
	m_iOldY = y;
}


//-------------------------------------------------------------------------
//粒子绘制函数
//-------------------------------------------------------------------------
// 雨点粒子绘制函数
void CWeather::DrawRain(bool bUnder)
{
	int i,ratio;
	LPTexture	pTex;
	static int fps = 0;

	g_pEGfx->SetRenderMode();

	if(!bUnder )
	{
		fps++;
		if( fps > 3000 )	// 2min
		{
			fps = 0;
			m_iGenChance = 15 + rand()*20/RAND_MAX;
			m_iSizeX = rand()*20/RAND_MAX - 10;

			if(m_iSizeX<=0)
				m_iSizeX-=8;
			else
				m_iSizeX+=8;
		}
	}

	std::vector<POINT> VPoints;
	std::vector<DWORD> VColors;
	POINT pt1,pt2;

	for(i=0;i<m_iElem;i++) 
	{
		SParticle& ele = m_sElem[i];

		// 显示雨痕
		if( bUnder )
		{
			if( ele.life>0 && ele.life<=12 )    
			{
				//显示雨痕
				if(!m_bSkip) ele.life--;

				pTex = g_pETexMgr->GetTex(3,800+(12-ele.life)/4,EP_NORMAL);
				//if(pTex)
				//	g_pEGfx->DrawTextureNL(ele.x-16,ele.y-16,pTex);
			}
			continue;
		}

		// 粒子生命周期结束就产生新粒子
		if( ele.life <= 0 )             
		{
			if( (rand()%100) >= m_iGenChance )  // 判断随机数是否超出概率范围
				continue;

			ele.x = rand() % m_iWidth;   // 粒子位于随机屏幕顶部
			ele.y = 0;

			ratio = rand() % 100;               // 随机的旋转度

			ele.lengthx = m_iSizeX * ratio / 100 - 1;               // 设置粒子的大小
			ele.lengthy = m_iSizeY * ratio / 100 + 1;

			ele.speedx = m_iSpeed * ele.lengthx / 100;   // 设置粒子的速度
			ele.speedy = m_iSpeed * ele.lengthy / 100;

			ele.life = m_iLife;                                 // 设置粒子生命周期(它和输出高度m_iHeight成比率)                    
		}
		// 绘制粒子
		else if( ele.life > 12 )
		{
			if( !m_bSkip )
			{
				ele.life--;                        
				ele.x += ele.speedx;
				ele.y += ele.speedy;
				if( ele.y >= m_iHeight ) ele.life = 0;
			}

			// 绘制雨点下落的痕迹
			pt1.x = ele.x;
			pt1.y = ele.y;
			pt2.x = ele.x + ele.lengthx;    // 计算雨点轨迹
			pt2.y = ele.y + ele.lengthy;
			VPoints.push_back(pt1);
			VPoints.push_back(pt2);

			if( g_pELight->IsEnable() )
			{
				VColors.push_back(g_pELight->GetLightColor());
				VColors.push_back(g_pELight->GetLightColor());
			}
			else
			{
				VColors.push_back(0x33C8E7FF);
				VColors.push_back(0x77C8E7FF);
			}
		}
	}
	g_pEGfx->DrawBatchLine(VPoints,VColors);
}

// 雪花粒子绘制函数
void CWeather::DrawSnow(bool bUnder)
{
	int i,ratio,w,h;

	for(i=0;i<m_iElem;i++) 
	{
		SParticle& ele = m_sElem[i];

		// 粒子生命周期结束,产生新粒子
		if( ele.life <= 0 )                
		{
			if( (rand()%100) >= m_iGenChance )    // 不满足产生概率继续
				continue;

			// 设置新粒子的属性	
			ele.x = rand() % m_iWidth;     // 设置粒子随机位置
			ele.y = 0;
			ele.angle = 0;                 // 设置粒子旋转的初始角度

			ratio = rand() % 100;                 // 取得一个随机旋转度(最小为20)
			ratio = max(ratio,20);

			ele.size = m_iSize * ratio / 100;        // 设置粒子的大小
			ele.speedx = m_iSpeedX * ratio / 100;    // 设置粒子的速度
			ele.speedy = m_iSpeedY * ratio / 100;    
			ele.speedangle = 2 + rand()%20;          // 设置粒子的自转速度

			ele.life = m_iLife;                      // 设置粒子的生命周期
		}
		else
		{
			// 显示粒子
			ele.life--;                                     // 更新粒子生命周期
			if( ele.y >= m_iHeight )                        // 粒子超出绘制范围就结束该粒子
				ele.life = 0;
			if( ele.life <= 0 )                             
				continue;

			ele.x += ele.speedx;                     // 更新粒子的位置
			ele.y += ele.speedy;
			ele.angle = (ele.angle+ele.speedangle) % 360;      // 更新粒子的旋转角度

			w = (int)(ele.size * g_fCos[ele.angle] / 2);              // 计算粒子的大小
			h = (int)(ele.size * g_fSin[ele.angle] / 2);

			//g_pCG->DrawQuadTexture(PARTICLE_LEVEL,p+p0,p+p1,p+p2,p+p3,*m_pSnowTex); // 对粒子进行纹理

		}
	}
}

void CWeather::DrawFlowerType1(int iFlower,bool bUnder)
{
	int i;
	LPTexture  pTex;

	g_pEGfx->SetRenderMode(RM_LINEAR);
	for(i=0;i<m_iElem;i++) 
	{
		SParticle& ele = m_sElem[i];

		// 画地上的部分
		if( bUnder )
		{
			if( ele.life>0 && ele.life<=150 )    
			{
				if(!m_bSkip)
					ele.life--;

				int iIdx = 0;
				switch(iFlower)
				{
				case PARTICLE_FLOWER1:
					iIdx = 515+ele.lengthy;
					break;
				case PARTICLE_FLOWER2:
					iIdx = 525+ele.lengthy;
					break;
				case PARTICLE_FLOWER3:
					iIdx = 535+ele.lengthy;
					break;
				case PARTICLE_FLOWER5:
					iIdx = 565+ele.lengthy;
					break;
				case PARTICLE_FLOWER7:
					iIdx = 585;
					break;
				case PARTICLE_FLOWER8:
					iIdx = 595;
					break;
				case PARTICLE_FLOWER9: 
					iIdx = 551 + ele.lengthy;
					break;
				default: 
					iIdx = 0;
					break;
				}

				pTex = g_pETexMgr->GetTex(PACKAGE_effect,iIdx,EP_NORMAL);
				if(pTex)
				{
					float fx = (float)ele.size / 100;
					float fy = (float)ele.size / 100;
					g_pEGfx->DrawTextureFX(ele.x,ele.y,pTex,0xFFFFFFFF,NULL,&POS(fx,fy));
				}
			}
			continue;
		}

		// 画空中的部分
		if( ele.life < 0 ) 
		{
			if(!m_bSkip) ele.life++;
			continue;
		}

		if( ele.life == 0 )             
		{		
			ele.x		= rand() % m_iWidth;				// 粒子位于随机屏幕顶部
			ele.y		= 0;
			ele.size		= rand()%80 + 60;					// 设置粒子的大小
			ele.speedy	= m_iSpeed * ele.size / 100;	// 设置粒子的速度
			ele.size		= ele.size + (rand()%50-40);
			ele.size		= max(ele.size,60);
			ele.size		= min(ele.size,130);
			ele.life		= m_iLife*2/3 + rand()%(m_iLife*2/3) + 150;// 设置粒子生命周期(它和输出高度m_iHeight成比率)                    
			ele.lengthx	= rand() % 100;						// 开始帧
			ele.lengthy	= rand() % 4;						// 类型
			ele.angle	= rand() % 5 + 1;					// 自旋转的速度
			if( ele.angle > 3 ) ele.angle = 2;
		}
		else if( ele.life < 150 )
		{
			continue;
		}
		else
		{
			if(!m_bSkip) 
			{
				ele.life--;
				ele.y += ele.speedy;
				ele.lengthx++;
				if( ele.y>=m_iHeight || ele.life<=0 )
					ele.life = (rand()%m_iGenChance) - 100;
			}

			int iIdx = 0;

			switch(iFlower)
			{
			case PARTICLE_FLOWER1:
				iIdx = 510+ele.lengthy;
				break;
			case PARTICLE_FLOWER2:
				iIdx = 520+ele.lengthy;
				break;
			case PARTICLE_FLOWER3:
				iIdx = 530+ele.lengthy;
				break;
			case PARTICLE_FLOWER5:
				iIdx = 560+ele.lengthy;
				break;
			case PARTICLE_FLOWER7:
				iIdx = 580;
				break;
			case PARTICLE_FLOWER8:
				iIdx = 590;
				break;
			case PARTICLE_FLOWER9: 
				iIdx = 555+ele.lengthy;
				break;
			default:
				iIdx = 0;
				break;
			}

			pTex = g_pETexMgr->GetTex(PACKAGE_effect,iIdx,EP_NORMAL);
			if(pTex)
			{
				pTex->EnableSysAnim(false);
				pTex->SetCurFrame(ele.lengthx/ele.angle % pTex->GetFrames());

				float fx = (float)ele.size / 100;
				float fy = (float)ele.size / 100;
				g_pEGfx->DrawTextureFX(ele.x,ele.y,pTex,0xFFFFFFFF,NULL,&POS(fx,fy));
			}
		}
	}
	g_pEGfx->SetRenderMode(RM_POINT);
}

void CWeather::DrawFlowerType2(int iFlower)
{
	int i;

	g_pEGfx->SetRenderMode(RM_LINEAR);
	for(i=0;i<m_iElem;i++) 
	{
		SParticle& ele = m_sElem[i];

		if( ele.life < 0 ) 
		{
			if(!m_bSkip) ele.life++;
			continue;
		}

		if( ele.life == 0 )             
		{		
			ele.x		= rand() % m_iWidth;				// 粒子位于随机屏幕顶部
			ele.y		= 0;
			ele.size		= rand()%80 + 60;				// 设置粒子的大小
			ele.speedy	= m_iSpeed * ele.size / 100;	// 设置粒子的速度
			ele.size		= ele.size + (rand()%50-40);
			ele.size		= max(ele.size,60);
			ele.size		= min(ele.size,130);
			ele.life		= m_iLife*2/3 + rand()%(m_iLife*2/3);// 设置粒子生命周期(它和输出高度m_iHeight成比率)                    
			ele.lengthx	= rand() % 100;						// 开始帧
			ele.lengthy	= rand() % 4;						// 类型
			ele.angle	= rand() % 6 + 1;					// 自旋转的速度
			if( ele.angle > 4 ) ele.angle = 3;
		}
		else
		{
			if(!m_bSkip) 
			{
				ele.life--;
				ele.y += ele.speedy;
				ele.lengthx++;
				if( ele.y>=m_iHeight || ele.life<=0 )
					ele.life = (rand()%m_iGenChance) - 100;
			}

			LPTexture pTex = NULL;

			switch(iFlower)
			{
			case PARTICLE_FLOWER3:
				pTex = g_pETexMgr->GetTex(PACKAGE_effect,530+ele.lengthy,EP_NORMAL);
				break;
			case PARTICLE_FLOWER4:
				pTex = g_pETexMgr->GetTex(PACKAGE_effect,540+ele.lengthy,EP_NORMAL);
				break;
			default:
				pTex = NULL;
				break;
			}
			if(pTex)
			{
				pTex->EnableSysAnim(false);
				pTex->SetCurFrame(ele.lengthx/ele.angle % pTex->GetFrames());

				float fx = (float)ele.size / 100;
				float fy = (float)ele.size / 100;
				g_pEGfx->DrawTextureFX(ele.x,ele.y,pTex,0xFFFFFFFF,NULL,&POS(fx,fy));
			}
		}
	}
	g_pEGfx->SetRenderMode(RM_POINT);
}

void CWeather::DrawFlowerType3(int iFlower)
{
	int i;
	LPTexture  pTex;

	g_pEGfx->SetRenderMode(RM_LINEAR);
	for(i=0;i<m_iElem;i++) 
	{
		SParticle& ele = m_sElem[i];

		if(ele.life < 0 ) 
		{
			if(!m_bSkip) m_sElem[i].life++;
			continue;
		}

		if( ele.life == 0 )             
		{		
			ele.x		= rand() % (m_iWidth+256) - 256;	// 粒子位于随机屏幕顶部
			ele.y		= 0;
			ele.size		= rand()%80 + 60;					// 设置粒子的大小
			ele.size		= ele.size + (rand()%50-40);
			ele.size		= max(ele.size,60);
			ele.size		= min(ele.size,130);
			if( m_iWidth > 900 )
				ele.size = ele.size * 120/100;
			ele.lengthx	= 0;
			ele.angle	= 2;
			ele.life		= ele.angle * 16;
		}
		else
		{
			if(!m_bSkip) 
			{
				ele.life--;
				ele.lengthx++;
				if( ele.y>=m_iHeight || ele.life<=0 )
					ele.life = (rand()%m_iGenChance) - 100;
			}

			pTex = g_pETexMgr->GetTex(PACKAGE_effect,570,EP_NORMAL);
			if(pTex)
			{
				pTex->EnableSysAnim(false);
				pTex->SetCurFrame(ele.lengthx/ele.angle % pTex->GetFrames());

				float fx = (float)ele.size / 100;
				float fy = (float)ele.size / 100;
				g_pEGfx->DrawTextureFX(ele.x,ele.y,pTex,0xFFFFFFFF,NULL,&POS(fx,fy));
			}
		}
	}
	g_pEGfx->SetRenderMode(RM_POINT);
}

// 根据类型绘制粒子
void CWeather::DrawGround(int iX,int iY,int offx,int offy)
{
	if( m_iType == PARTICLE_NONE)
		return;

	MoveAsNeed(iX*64+offx,iY*32+offy);

	switch( m_iType ) 
	{
	case PARTICLE_RAIN:
		DrawRain(true);
		break;
	case PARTICLE_SNOW:
		DrawSnow(true);
		break;
	case PARTICLE_CLOUD:
	case PARTICLE_STORM:
		break;
		break;
	case PARTICLE_FLOWER1:
	case PARTICLE_FLOWER2:
	case PARTICLE_FLOWER5:
	case PARTICLE_FLOWER7:
	case PARTICLE_FLOWER8:
	case PARTICLE_FLOWER9:
		DrawFlowerType1(m_iType,true);
		break;
	case PARTICLE_FLOWER3:
	case PARTICLE_FLOWER4:
	case PARTICLE_FLOWER6:
		break;
	}
}

void CWeather::DrawAir(int iX,int iY,int offx,int offy)
{
	if(m_iType == PARTICLE_NONE)
		return;

	DWORD dwColor = m_dwWeatherColor;

	switch( m_iType ) 
	{
	case PARTICLE_RAIN:
		DrawRain(false);
		break;
	case PARTICLE_SNOW:
		DrawSnow(false);
		break;
	case PARTICLE_CLOUD:
		{
			if(dwColor == -1)
				dwColor = DEFAULT_CLOUD_COLOR;

			DrawCloud(iX,iY,offx,offy,dwColor);
		}
		break;
	case PARTICLE_STORM:
		{
			if(dwColor == -1)
				dwColor = DEFAULT_STORM_COLOR;
			DrawStorm(iX,iY,offx,offy,dwColor);
		}
		break;
	case PARTICLE_FLOWER1:
	case PARTICLE_FLOWER2:
	case PARTICLE_FLOWER5:
	case PARTICLE_FLOWER7:
	case PARTICLE_FLOWER8:
	case PARTICLE_FLOWER9:
		DrawFlowerType1(m_iType,false);
		break;
	case PARTICLE_FLOWER3:
	case PARTICLE_FLOWER4:
		DrawFlowerType2(m_iType);
		break;
	case PARTICLE_FLOWER6:
		DrawFlowerType3(m_iType);
		break;
	}
}

//绘制云雾
//x1,y1代表屏幕左上角的smalltile的坐标
//dx,dy是(x1,y1)的smalltile在屏幕上的像素偏移
//dwDensity_的范围控制在110-190之间！
void CWeather::DrawCloud(int iX,int iY,int iOffX,int iOffY,DWORD dwColor)
{
	if( g_pEGfx->IsNoDraw())
		return;

	int dwDensity = 155 + m_iDensity;

	if (dwDensity<110)
		dwDensity = 110;
	else if (dwDensity>190)
		dwDensity = 190;

	LPTexture pTex[4] = {0};
	for(int ii = 0;ii < 4;ii++)
	{
		pTex[ii] = g_pETexMgr->GetTex(PACKAGE_effect,501+ii,EP_NORMAL);
	}

	g_pEGfx->DrawCloudTexture(iX,iY,iOffX,iOffY,pTex,dwColor,dwDensity);
}

//绘制沙尘暴
//x1,y1代表屏幕左上角的smalltile的坐标
//dx,dy是(x1,y1)的smalltile在屏幕上的像素偏移
void CWeather::DrawStorm(int iX,int iY,int iOffX,int iOffY,DWORD dwColor)
{
	m_SandStorm.Update(GetTickCount());

	LPTexture pTex[4] = {0};
	for(int ii = 0;ii < 4;ii++)
	{
		pTex[ii] = g_pETexMgr->GetTex(PACKAGE_effect,501+ii,EP_NORMAL);
	}

	g_pEGfx->DrawCloudTexture(iX,iY,iOffX,iOffY,pTex,dwColor,m_SandStorm.GetDist(),FALSE);
}
