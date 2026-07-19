#pragma once

//定义游戏中使用的全局变量和全局函数
//不会引起底层模块的全部编译

#include "Global/Global.h"
#include "Global/MathUtil.h"
#include "Global/StringUtil.h"


//游戏内部使用的公
// 游戏8方向计算
int		GetDir8(int iTX, int iTY, int iOX, int iOY);
int		GetDir16(int iTX,int iTY,int iOX,int iOY,bool bFlag = false);
int		GetDir8Attack(int iTX,int iTY,int iOX,int iOY);
int 	GetLineDirect(int x1,int y1,int x2,int y2);

// 根据方向计算偏移
void	GetDirStep(int iDir,int &iX,int &iY);
void    GetDir16Step(int iDir,float &fX,float &fY);
const char* GetToDir(int iTX,int iTY,int iOX,int iOY);

//将[0,iDirCount)以外的方向修正到此之内
int	ModifyDir(int iDir,int iDirCount);

//新技能方向缩减支持
void	DirMapping(int dirCur,int dirMax,int &dirOut,int &bMapping);

const char* GetJobName(int iJob);
const char* GetGenderName(int iSex);
const char* GetHairStyle(int i);
const char* GetHairColor(int i);

// 检测字符串的象素长度(主要针对有动态图标的字符串)
UINT	STRLEN(const char* str, int iFontSize = 12);
UINT	STRLEN(const char* p0,const char* p1, int iFontSize = 12);

//------------------------------------------------------------------------------
// 从字符串中取得每个以回车分割的字段
const char* GoThroughEnter(const char* szSource, int& iPos);
void  CutByUnicode(const char* strMsg,VString& vec,UINT iLimit = 60,bool bCutByEnter = true,char token = '\n');//bCutByEnter遇到回车换行是否另取一行
//------------------------------------------------------------------------------

BOOL	CopyText2Clip(const char *buf,int size);
BOOL	PasteFromClipboard(string& str);

// 计算百分比
float   CalRate(int val,int vmax);

//
char GetDirByTile(int iDeltaX,int iDeltaY);

