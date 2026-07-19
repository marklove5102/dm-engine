#pragma once

//disable warning of INIT_VAR
#pragma warning(disable : 4867) 

//------------------------------//
//		使用xml的基类			//
//------------------------------//
#include <string>
using namespace std;
class CXmlObj;	//所有用于解释XML脚本的基类

//继承于CXmlObj的类必须在申明中使用该宏
#define DERIVE(_class, _super)	\
public:		\
	typedef _super	SUPERCLASS;	\
	typedef _class	THISCLASS;	\
	static	VarMap	sm_varMap;	\
	static	AttrEntry	sm_entry[];	\
	virtual VarMap*	GetVarMap() const;

//成员变量相对偏移量
#define	MEMBER_OFFSET(_class, _member)	\
	(const int)(&(_class*)0->_member)

//基本类型，用于在变量描述表中标识类型
typedef enum eBassAttrType
{
	BAT_string	= 0,	//字符串
	BAT_bool,			//bool
	BAT_int,			//整形
	BAT_double,			//浮点数
} BaseAttrType;

//公用中间类型，用于对几种基本类型提供通用接口
typedef union uComVal
{
	bool    boolVal;
	int		intVal;
	double	doubleVal;
	const char*	stringVal;

	uComVal(const uComVal& u) { memcpy(this, &u, sizeof(u));}
	uComVal(const int i)	{ intVal = i;}
	uComVal(const double d) { doubleVal = d;}
	uComVal(const char* str) { stringVal = str;}
	uComVal(bool b) { boolVal = b;}
} ComVal;

//赋值函数指针
typedef void (CXmlObj::*SetFn)(ComVal);

//变量描述表的成员结构
typedef struct tagAttrEntry
{
	BaseAttrType	_type;	//类型
	const char*		_name;	//名称
	SetFn			_fn;	//赋值函数的地址
	ComVal			_initval;	//初始值
	struct tagAttrEntry(BaseAttrType type, const char* name, SetFn fn, ComVal initVal)
		:_initval(initVal)
	{
		_type	= type;
		_name	= name;
		_fn		= fn;
	}
} AttrEntry;

//变量描述表
typedef struct tagVarMap
{
	tagVarMap*	pSuperMap;		//父类表
	AttrEntry*	pEntry;			//描述表项
}VarMap;

//申明一个变量，同时提供内联的读写函数。必须与INIT_VAR宏对应，否则无法自动赋值
#define DECLARE_VAR(_type, _name)	\
	private:	\
	_type	m_##_type##_name;	\
	public:		\
	inline const _type&	Get##_name() const	{	return m_##_type##_name;}	\
	inline	void	Set##_name(ComVal val) { m_##_type##_name	= val.##_type##Val;}

//开始定义
#define BEGIN_VAR(_class, _superclass)	\
	VarMap _class::sm_varMap = { &_superclass::sm_varMap,	_class::sm_entry};	\
	VarMap* _class::GetVarMap() const { return &sm_varMap;}	\
	AttrEntry _class::sm_entry[] = {

//在描述表中添加一项
#define INIT_VAR(_type, _name, _initval)	\
	AttrEntry(BAT_##_type, #_name, (SetFn)Set##_name, (_initval)),

//完成构建，添加描述表结束标志
#define END_VAR(_class, _superclass)	\
	AttrEntry(BAT_string, NULL, NULL, 0)	\
	};


//------------------//
//		基本类		//
//------------------//
class TiXmlNode;
class TiXmlElement;
class TiXmlAttribute;
class CXmlObj
{
	//对结点的解释部分
public:
	virtual bool InitChildText(const char* pText) {return false;}	//对文字节点的解释函数
	virtual bool InitChildElement(const char* pValue) {return false;}	//对元素节点的解释函数
	virtual bool InitChildComm(const char* pText) {return false;}	//对注释节点的解释函数
	virtual bool InitSelf(const TiXmlElement* ele) {return false;}		//对自身的其他初始化工作
	virtual void Clear() {}		//清空函数
	inline  bool Create() { return Init(s_pCurParsingElement);}	//引发构建子节点
	virtual void OnCreate() {}	//初始化后的相应函数
	virtual bool LoadFile(const char*);	//对文件的接口
	virtual bool LoadFromBuff(const char* buf);


	//对属性的解释部分
private:
	bool Init(const TiXmlElement* ele);		//初始化自己的属性，通用函数，不能被改写
	virtual void InitAttrVar();				//自动初始化变量表中的成员变量
	bool FindAndSetAttr(const TiXmlAttribute* attr);	//查找名称--变量对应表并赋值，通用函数，不能被改写
public:
	static	VarMap	sm_varMap;
	static	AttrEntry	sm_entry[];
	void	CallSetFn(SetFn fn, ComVal val) { (this->*fn)(val);}	//封装赋值
	virtual VarMap* GetVarMap() const { return &sm_varMap;}	//将被子类自动改写

	//辅助功能
public:
	bool CmpName(const TiXmlNode* pNode, const char* pName, bool bIgnorCase = true) const;	//比较节点名称
	bool CmpString(const char* sz1, const char* sz2, bool bIgnorCase) const;

	static int   s_iLastError;

	//辅助用
protected:
	static const TiXmlElement*	s_pCurParsingElement;
};