
#pragma once

//点结构
class SPoint
{
public:
	SPoint()
	{
		x = y = 0;
	}
	SPoint(int x0,int y0)
	{
		x = x0,y = y0;
	}
	int x,y;
};

//矩形
class SRect
{
public:
	SRect()
	{
		left = top = right = bottom = 0;
	}
	SRect(int l,int t,int r,int b)
	{
		left = l;
		top  = t;
		right = r;
		bottom = b;
	}
	bool PtInRect(int x,int y)
	{
		if(x < left || x > right || y < top || y > bottom)
			return false;

		return true;
	}
	int Width(){ return (right - left);}
	int Height(){ return (bottom - top); }
	int left,top,right,bottom;
};

extern float	g_fSin[360],g_fCos[360];//sin,cos的360度对应的值
