#pragma once
#include "BaseClass/Control/CtrlButton.h"
#include "BaseClass/Control/CtrlWindowX.h"


#include <string>

class CComandWnd :	public CCtrlWindowX
{
	DECLARE_WND_POSX(CComandWnd)

private:
	enum
	{
		PRIVATE = 0,		// 私聊
		PUBLIC,				// 喊话
		FRIEND,				// 好友
		GUILD,				// 行会
		UNION,				// 联盟
		MEMORY,				// 记忆
		RESUME,				// 恢复私聊

		BUTTON_NUMS
	};

	struct ButtonData_t
	{
		CCtrlButton	*pButton;
		std::string		sShow1;			// 状态1显示
		std::string		sShow2;			// 状态2显示
		std::string		sTip;			// 按钮提示
	};

	ButtonData_t	m_vButtonData[BUTTON_NUMS];
	CCtrlEdit *		m_pEdit;			// 恢复私聊需要的编辑框

public:
	static int		m_ButtonSate[BUTTON_NUMS];

	CComandWnd(void);
	~CComandWnd(void);

	static void SetDefault(void);		// 设置成Default
	static void SwitchState(int i);		// 变换某个命令的状态

	virtual void Draw(void);
	virtual void OnCreate(void);
	virtual bool Msg(DWORD dwMsg, DWORD dwData, CControl * pControl);
};
