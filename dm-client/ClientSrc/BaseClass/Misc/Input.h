// Input.h 此类提供对鼠标和键盘操作的封装。
// 这里并没有使用DInput，而是从窗格标准消息循环里抽取数据
#pragma once

#include "Global/Global.h"

// 输入消息循环尺寸，如果溢出，新的消息将被丢弃
#define	INPUT_MSG_BUFFERSIZE		512
        
// 鼠标按钮
enum eMouseMsg
{
	INPUT_MOUSE_LEFTBT = 0,			//左健
	INPUT_MOUSE_RIGHTBT,			//右健
	INPUT_MOUSE_MIDDLEBT,			//中键
	INPUT_MOUSE_MAX
};

class CInput
{
public:
	CInput(void);
	~CInput(void);

	// 主消息循环处理函数
	bool WndMessage(UINT uMsg,WPARAM wParam,LPARAM lParam);

	// 键盘属性相关函数
	void EnableCharMsg(bool bChar = true);				// 是否发送 CHAR 消息
	bool GetCharMsgMode() { return m_bChar; }			
	void EnableKeyDownMsg(bool bMsg = true);			// 是否发送 KEYDOWN 消息
	bool GetKeyDownMsgMode() { return m_bKeyDown; }
	void EnableKeyUpMsg(bool bMsg = true);				// 是否发送 KEYUP 消息
	bool GetKeyUpMsgMode() { return m_bKeyUp; }

	// 键盘状态
	bool IsCapsLock();									// 判断CapsLock键是否按下
	bool IsNumLock();									// 判断NumLock键是否按下
	bool IsShift();										// 判断Shift键是否按下
	bool IsCtrl();										// 判断Ctrl键是否按下
	bool IsAlt();										// 判断Alt键是否按下
	bool IsEscape();                                    // 判断Esc键是否按下

	// 鼠标状态
	bool IsLeftButton();
	bool IsRightButton();
	bool IsMiddleButton();

	// 鼠标属性相关函数
	void EnableMoveMsg(bool bMsg = true);				// 是否发送 MOVE 消息
	bool GetMoveMsgMode() { return m_bMove; }
	void EnableWheelMsg(bool bMsg = true);				// 是否发送 WHEEL 消息
	bool GetWheelMsgMode() { return m_bWheel; }
	void EnableDClickMsg(bool bMsg = true);				// 是否发送 DCLICK 消息
	bool GetDClickMsgMode() { return m_bDClick; }
	void EnableButtonDownMsg(bool bMsg = true);			// 是否发送 BUTTONDOWN 消息
	bool GetButtonDownMsgMode() { return m_bButtonDown; }
	void EnableButtonUpMsg(bool bMsg = true);			// 是否发送 BUTTONUP 消息
	bool GetButtonUpMsgMode() { return m_bButtonUp; }

	bool Acquire();                                     // 使系统处于能够获得输入的状态
	bool Unacquire();                                   // 使系统处于不能够获得输入的状态
	void IsActive(WORD af);								// 判断是否要激活或释放控制权
	bool IsEnable() { return m_bEnable; }               // 察看输入是否处于激活态

	// 获取键盘DX即时状态
	void UpdateKeyState();								// 更新键盘即时状态
	bool IsKeyDown(BYTE vkey);							// 判断一个键是否被按下，输入为机器键盘扫描码

	// 获取鼠标DX即时状态
	void UpdateMouseState(int cltx,int clty);		    // 更新鼠标即时状态
	void GetMousePos(int& x,int& y){ x = m_iX; y = m_iY; }// 得到鼠标当前位置
	BOOL GetCursorPos(LPPOINT lpPoint);

	// 从输入循环缓冲区中提取一个新的消息
	bool GetMsgBuffer(DWORD *msg,DWORD *data);
	// 清除输入循环缓冲区
	void ClearMsgBuffer();

protected:
	bool m_bEnable;							//游戏窗口是否被激活，如果不被激活则不接收输入消息

	// 键盘相关变量
	bool m_bChar;                           //是否发送字符消息
	bool m_bKeyDown;                        //是否发送KeyDown消息
	bool m_bKeyUp;		                    //是否发送KeyUp消息

	UCHAR m_cKeyState[256];					//键盘状态

	// 鼠标相关变量
	bool m_bMove;                           //是否发送鼠标移动消息
	bool m_bWheel;                          //是否发送滚轮移动消息
	bool m_bDClick;		                    //是否发送双击消息
	bool m_bButtonDown;                     //是否发送按键Down消息
	bool m_bButtonUp;			            //是否发送按键Up消息
	int  m_iX,m_iY;							//当前鼠标位置

	// 输入消息循环
	struct SInputBuffer 
	{
		DWORD msg;							//消息类型，以 INPUT_MSG_ 为前缀
		DWORD data;							//数据
	};

	SInputBuffer m_sBuffer[INPUT_MSG_BUFFERSIZE];       //消息队列缓冲	                                        
	int  m_iStart;                                      //消息循环的头
	int  m_iEnd;					                    //消息循环的尾部，m_iEnd记录下一个放入消息的位置，而不是上一次放入消息的位置

	// 鼠标钩子
	bool m_bLButtonDown,m_bRButtonDown,m_bMButtonDown;
	bool m_bLDClickDown,m_bRDClickDown;

	//
protected:
	bool AddMsgBuffer(DWORD msg,DWORD data = 0);		// 把一条消息加入消息循环，如果循环溢出，新消息将被丢弃
};
