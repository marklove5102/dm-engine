#pragma once

#include "ActionInf.h"
#include "Global/MemPool.h"
#include "Global/Interface/FontInterface.h"

#define NEW_TARGETDATA()	STargetData::NEW_STargetData()
#define DEL_TARGETDATA(p)	STargetData::DEL_STargetData(p)

// 目标数据
class STargetData
{
	DECLARE_POOL(STargetData);		// 缓冲
public:
	STargetData();

    int             iType;          //0表示目标受攻击，1表示目标消失，2表示目标死亡
	DWORD			dwID;			// 被攻击者ID
	DWORD           dwOID;          // 发出攻击动作者ID
	DWORD			dwStatus;		// 被攻击者状态
	BYTE			byType;			// 扣血类型
	BYTE			byReserve;		// 预留
	WORD			wDecHP;			// 减少血量
	WORD			wCurHP;			// 当前血量
	WORD			wMaxHP;			// 最大血量

	STargetData	*	pNext;

	void InitData(void * pBuf);
};

class SNextAction;

enum E_ACTION_FLAG
{
	HIGH_ACTION		=	0x00000001,		// 高优先级动作
	SERVER_ACTION	=	0x00000002,		// 服务器要求的动作
	FORCE_ACTION	=	0x00000004,		// 强迫攻击使用
	RELAY_ACTION	=	0x00000008,		// 动作暂存/中继使用
	MAGIC_ACTION	=   0x00000010,     // 魔法攻击
	COLLIDE_ACTION  =   0x00000020,     // 冲撞的动作
	BACK_ACTION     =   0x00000040,     // 倒退的动作
	DELAY_ACTION	=   0x00000080,		// 延迟动作
	CREATE_MAGIC	=   0x00000100,		// 有这个标记的魔法动作只放魔法不做动作,其它动作有此标记则做动作的同时放一个魔法
	COLLIDE_SHEN	=	0x00000200,
	COLLIDE_MO		=	0x00000400,
	MOVE_ACTION 	=	0x00000800,
	SHADOWKILL_ACTION =	0x00001000,	// 八方分影斩的动作
};

// 当前动作结构
class SAction
{
public:
	SAction();

	bool IsEnd(DWORD tTime);
	int  GetFrame(DWORD tTime);
	float GetRate(DWORD tTime);
	void SetNpcRandStand(WORD wRaceNum);
	int GetMultiActionNpcAction(WORD wRaceNum);

	bool IsStand();
	bool IsMove();
	bool IsDeath();
	bool IsAttack();

	void ClearTarget();
	void AddTarget(DWORD id);

	WORD	wAction;			// 动作类型
	WORD	wDrawAction;		// 绘制动作的类型，如wAction=ACTION_STAND时drawaction可以为ACTION_HSATAND,ACTION_LSTAND
	BYTE    cDir;               // 方向

	int     iStartX;            // 开始格子
	int     iStartY;            // 开始格子
	int		iMoveX;				// 偏移X
	int		iMoveY;				// 偏移Y
	int     iAimX;              // 目标格子
	int     iAimY;				// 目标格子

	DWORD 	tFrameStart;		// 开始当前动作的时间
	int 	iFrameCount;	    // 当前动作的总帧数
	int     iFrameNow;          // 当前是该动作的第几帧
	int     iDealTimesCurFrame; // 当前帧第几次处理
	int	    iFrameSpeed;		// 变帧速度，以ms为单位
	DWORD   uFlag;              // 动作标记
	UINT	uData;				// 附加数据
	int		iData;				// 附加数据
	UINT	uTarget;			// 链表的下一项
	STargetData*    pTarget;         // 单一目标
	DWORD	dwColor;
};

// 下一个动作结构
#define NEW_NEXTACTION()	SNextAction::NEW_SNextAction()
#define DEL_NEXTACTION(p)	SNextAction::DEL_SNextAction(p)

// 下一个动作结构
class SNextAction
{
	DECLARE_POOL(SNextAction);
public:
	SNextAction();

	bool IsStand();
	bool IsMove();
	bool IsAttack();

	void Clear();
	void ClearTarget();
	void AddTarget(DWORD id);

	WORD	wAction;			// 动作类型
	WORD    wDrawAction;        // 绘制动作类型，如wAction=ACTION_STAND时drawaction可以为ACTION_HSATAND,ACTION_LSTAND,ACTION_DEATH动作绘制的可以是ACTION_DEATH2
	BYTE    cDir;               // 方向
	__int64	iLooks;				// 外观
	int 	iAimX;				// 目标X
	int		iAimY;				// 目标Y
	DWORD   uFlag;              // 动作标记
	UINT	uData;				// 附加数据
	UINT	uData1;			    // 附加数据
	UINT	uData2;			    // 附加数据
	UINT	uData3;			    // 附加数据
	int		iData;				// 附加数据
	int     iData1;             // 附加数据
	int     iData2;             // 附加数据
	int     iData3;             // 附加数据
	DWORD	dwColor;            // 颜色
	UINT    uTarget;			// 单一目标
	STargetData*	pTarget;	// 受害者序列

	SNextAction * pNext;		// 链表的下一项
	BYTE    byResv[3];          // 4字节对齐占位
};

void TargetDataClear(STargetData *&dst);
BOOL AddAttacked(DWORD id,DWORD oid,int iData = 0);//id被oid攻击
BOOL EffectAttacked(STargetData*& pTarget);

//特殊魔法处理基类
class  CCharacterAttr;
struct DrawCharInf;
struct Magic_Ref_s;
class  IMagicCase
{
public:
	//
	virtual		const char*GetName() = 0;
	virtual     bool NetInit(CCharacterAttr *pChar,BYTE byMagicID,WORD wPosX,WORD wPosY,DWORD	dwReserve) = 0;
	virtual 	bool Init(CCharacterAttr *pChar) = 0;
	virtual 	bool ShutDown(CCharacterAttr *pChar) = 0;
	virtual 	bool Activate(void) = 0;					//开始处理
	virtual 	bool IsActive(void) = 0;					//开始处理

	virtual 	bool Deal(CCharacterAttr *pChar) = 0;		//帧处理
	virtual 	bool AdjustDraw(DrawCharInf *pDCI,CCharacterAttr *pChar) = 0;		//调整画图内容
	virtual 	bool PrevDraw(int x,int y,DrawCharInf *pDCI,CCharacterAttr *pChar) = 0;		//人物画图前插补内容
	virtual 	bool PostDraw(int x,int y,DrawCharInf *pDCI,CCharacterAttr *pChar) = 0;		//人物画图后插补内容

	//参数
	virtual 	Magic_Ref_s* GetRefCount() = 0;				//技能公共结构
	virtual 	SAction *GetAction() = 0;					//公共动作属性
	virtual 	int GetFrames() = 0;						//总帧

	//特殊
	virtual 	int HitPoints(int iSign,int iDir,POINT *ptAnswer) = 0;	//根据服务端的标记，计算目标位置点
};

enum EST_TYPE
{
	EST_DEFAULT = 0 ,
	EST_CLIP	 ,			//裁剪边界(右边和下边)
	EST_SCALE	 ,			//使用放大参数
	EST_SCALE_CENTER ,		//自计算的放大

	EST_GENERAL_TEXT = 10,	//普通文本
	EST_DIGITAL_TEXT ,		//数字文本
};

#define NEW_TEX_SHOW()	tex_show_t::NEW_tex_show_t()
#define DEL_TEX_SHOW(p)	tex_show_t::DEL_tex_show_t(p)

struct tex_show_t
{
	DECLARE_POOL(tex_show_t);			// 缓冲

	int			iType;					//显示类型；默认为0；正常图片；1显示剪裁图片；2；显示缩放;3显示文本字符串 4显示数字字符串[Tex ID]；
	int			x,y;					//屏幕位置
	int			dx,dy;					//图片剪裁的高度和宽度；为0，表示默认高度和宽度
	WORD		wFrame;					//帧索引
	WORD		wTexID;					//图片ID
	WORD		wPackage;				//包裹
	WORD		wAdd;					//叠加方式
	DWORD		dwColor;				//颜色
	float		fScaleX,fScaleY;		//缩放比例； 默认不放大 = 1.0;X方向和Y方向不同的缩放比例
	float		fRotX,fRotY;			//

	char		text[16];				//附加文本显示[4];
	BYTE        byFontType;             //字体
	BYTE        byFontSize;             //字体大小
	tex_show_t  *next;					//下一个图片信息

	tex_show_t():iType(0),x(0),y(0),dx(0),dy(0),wFrame(0),wTexID(0),
		wPackage(0),wAdd(0),dwColor(-1),fScaleX(0),fScaleY(0),fRotX(0),fRotY(0),next(0),byFontType(FONT_DEFAULT),byFontSize(FONTSIZE_DEFAULT)
	{
		text[0] = 0;
	}
	~tex_show_t(){}
};
void	push_tex_store(tex_show_t	**p_head,tex_show_t	*p_tex);	//push p_tex to the head of p_head
void	clear_tex_store(tex_show_t	**p_head);						//clear p_head