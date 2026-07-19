#ifndef PATHPUBLIC_h
#define PATHPUBLIC_h

#include "Global/Global.h"
#include "Global/MathUtil.h"
#include <vector>
#include <list>

inline int _PointLen(int srcx,int srcy,int dstx,int dsty) 
{
	return (abs(srcx - dstx)  + abs(srcy - dsty)) ;
};

inline int _PointLenAbs(int srcx,int srcy,int dstx,int dsty) 
{
	return (max(abs(srcx - dstx) , abs(srcy - dsty))) ;
};

//判断两点是否靠近
inline bool _closePoint(int srcx,int srcy,int dstx,int dsty,int limit) 
{
	return (abs(srcx - dstx) <= limit && abs(srcy - dsty) <= limit) ;
};

inline bool _closePoint(int srcx,int srcy,int dstx,int dsty,int limitx,int limity)
{
	return (abs(srcx - dstx) <= limitx && abs(srcy - dsty) <= limity);
};

//存储地图上的位置点
struct MapPoint_t : public SPoint
{
	MapPoint_t():SPoint(){};
	MapPoint_t(const char* _mapID,int _x,int _y){SetValue(_mapID, _x, _y);};

	string mapID;

	inline bool IsValidate(){return (!mapID.empty() && x &&y);};
	inline void SetValue(const char* _mapID = "",int _x= 0,int _y= 0){mapID.assign(_mapID);x =_x;y = _y;};
	inline bool IsMapPoint(const char* _mapID,int _x,int _y){return (mapID.compare(_mapID) == 0) && (x ==_x) && (y == _y);};

	inline bool IsMapPoint(MapPoint_t &src){return IsMapPoint(src.mapID.c_str(),src.x,src.y);};
};


#endif