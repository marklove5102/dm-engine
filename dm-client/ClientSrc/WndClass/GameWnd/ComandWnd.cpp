#include "comandwnd.h"
#include "GameControl/GameControl.h"
#include "GameData/MsgBoxMgr.h"
#include "GameData/GameData.h"
#include "GameData/GuildData.h"


int		CComandWnd::m_ButtonSate[]	=	{0,0,0,0,0,0,0};


INIT_WND_POSX(CComandWnd,POS_VARIABLE,POS_VARIABLE)

CComandWnd::CComandWnd(void)
{
	m_pEdit		= NULL;

	m_vButtonData[PRIVATE].sShow1	= "拒绝私聊";
	m_vButtonData[PRIVATE].sShow1	= "开启私聊";
	m_vButtonData[PRIVATE].sShow1	= "重复使用可以切换状态";

	m_vButtonData[PUBLIC].sShow1	= "拒绝私聊";
	m_vButtonData[PUBLIC].sShow1	= "开启私聊";
	m_vButtonData[PUBLIC].sShow1	= "重复使用可以切换状态";

	m_vButtonData[FRIEND].sShow1	= "拒绝私聊";
	m_vButtonData[FRIEND].sShow1	= "开启私聊";
	m_vButtonData[FRIEND].sShow1	= "重复使用可以切换状态";

	m_vButtonData[GUILD].sShow1		= "拒绝私聊";
	m_vButtonData[GUILD].sShow1		= "开启私聊";
	m_vButtonData[GUILD].sShow1		= "重复使用可以切换状态";

	m_vButtonData[UNION].sShow1		= "拒绝私聊";
	m_vButtonData[UNION].sShow1		= "开启私聊";
	m_vButtonData[UNION].sShow1		= "重复使用可以切换状态";

	m_vButtonData[MEMORY].sShow1	= "拒绝私聊";
	m_vButtonData[MEMORY].sShow1	= "开启私聊";
	m_vButtonData[MEMORY].sShow1	= "重复使用可以切换状态";

	m_vButtonData[RESUME].sShow1	= "拒绝私聊";
	m_vButtonData[RESUME].sShow1	= "开启私聊";
	m_vButtonData[RESUME].sShow1	= "重复使用可以切换状态";


	m_iIndex = 729;
	m_iPages = 1;
	
	m_iAlignType = XAL_TOPLEFT;
	m_iOffX = 417;
	m_iOffY = 188;

}

CComandWnd::~CComandWnd(void)
{
}

void CComandWnd::SetDefault(void)
{
	m_ButtonSate[PRIVATE]	=	0;	// 私聊
	m_ButtonSate[PUBLIC]	=	0;	// 喊话
	m_ButtonSate[FRIEND]	=	0;	// 好友
	m_ButtonSate[GUILD]		=	0;	// 行会
	m_ButtonSate[UNION]		=	0;	// 联盟
	m_ButtonSate[MEMORY]	=	0;	// 记忆
	m_ButtonSate[RESUME]	=	0;	// 恢复私聊
}

void CComandWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	g_pFont->DrawText(105+m_iScreenX, 11+m_iScreenY, "指   令", 0xFFFAC896, FONT_YAHEI, FONTSIZE_BIG);

	g_pFont->DrawText(38+m_iScreenX,40+m_iScreenY, "小提示：点击命令后的确定按钮", 0xFFB46428, FONT_YAHEI, FONTSIZE_MIDDLE);
	g_pFont->DrawText(77+m_iScreenX,60+m_iScreenY, "可以实现相关功能", 0xFFB46428, FONT_YAHEI, FONTSIZE_MIDDLE);

	for(int i = 0; i < BUTTON_NUMS; i++)
	{
		const char *p;
		if(m_ButtonSate[i] == 0)
			p = m_vButtonData[i].sShow1.c_str();
		else
			p = m_vButtonData[i].sShow2.c_str();
		if(p)
			g_pFont->DrawText(m_iScreenX + 31,m_iScreenY + 89 + 21 * i,p,0xFFB48C5A, FONT_YAHEI);
	}
}

void CComandWnd::OnCreate(void)
{
	m_pEdit	= new CCtrlEdit();
	AddControl(m_pEdit);
	m_pEdit->CreateEx(this, 50,244,170,12,0,39,39,39);
	m_pEdit->SetFont(FONT_YAHEI);
	m_pEdit->SetColor(0xFFB48C5A);
	m_pEdit->SetMaxLength(14);
	m_pEdit->SetPrompt("请输入玩家名称");

	m_vButtonData[PRIVATE].sShow1	= "拒绝私聊";
	m_vButtonData[PRIVATE].sShow2	= "开启私聊";
	m_vButtonData[PRIVATE].sTip		= "重复使用可以切换状态";

	m_vButtonData[PUBLIC].sShow1	= "拒绝喊话";
	m_vButtonData[PUBLIC].sShow2	= "开启喊话";
	m_vButtonData[PUBLIC].sTip		= "重复使用可以切换状态";

	m_vButtonData[FRIEND].sShow1	= "拒绝好友";
	m_vButtonData[FRIEND].sShow2	= "开启好友";
	m_vButtonData[FRIEND].sTip		= "重复使用可以切换状态";

	m_vButtonData[GUILD].sShow1		= "加入行会";
	m_vButtonData[GUILD].sShow2		= "退出行会";
	m_vButtonData[GUILD].sTip		= "请与行会会长面对面站立";

	m_vButtonData[UNION].sShow1		= "允许联盟";
	m_vButtonData[UNION].sShow2		= "退出联盟";
	m_vButtonData[UNION].sTip		= "请行会会长与结盟会长面对面站立";

	m_vButtonData[MEMORY].sShow1	= "允许使用记忆一套";
	m_vButtonData[MEMORY].sShow2	= "拒绝使用记忆一套";
	m_vButtonData[MEMORY].sTip		= "重复使用可以切换状态";

	m_vButtonData[RESUME].sShow1	= "拒绝(恢复)玩家和你的私聊";
	m_vButtonData[RESUME].sShow2	= "拒绝(恢复)玩家和你的私聊";
	m_vButtonData[RESUME].sTip		= "请输入你要拒绝（恢复）私聊的玩家姓名";

	for(int i = 0; i < BUTTON_NUMS; i++)
	{
		m_vButtonData[i].pButton	= new CCtrlButton();
		AddControl(m_vButtonData[i].pButton);
		//m_vButtonData[i].pButton->Create(this,188,86 + 21 * i,CTRL_STYLE_BACKMODE_NODRAW|CTRL_STYLE_AUTOSIZE,47,17);
		//m_vButtonData[i].pButton->ReloadTex(0,112,113);
		m_vButtonData[i].pButton->CreateEx(this,177,86 + 21 * i, 95, 96, 97, 98);
		m_vButtonData[i].pButton->SetText("确 定");
		m_vButtonData[i].pButton->SetTips(m_vButtonData[i].sTip.c_str());
	}

	SetCloseButton(233,4, 80);
}

bool CComandWnd::Msg(DWORD dwMsg, DWORD dwData, CControl * pControl)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_BUTTON_CLICK:
		{
			char str[1024] = {0};
			if(pControl == m_vButtonData[PRIVATE].pButton)
			{ 
				if(m_ButtonSate[PRIVATE] == 0)
					strcpy(str,"@拒绝私聊");
				else
					strcpy(str,"@允许私聊");
				m_ButtonSate[PRIVATE] = !(m_ButtonSate[PRIVATE]);
			}
			else if(pControl == m_vButtonData[PUBLIC].pButton)
			{
				if(m_ButtonSate[PUBLIC] == 0)
					strcpy(str,"@拒绝喊话");
				else
					strcpy(str,"@允许喊话");
				m_ButtonSate[PUBLIC] = !(m_ButtonSate[PUBLIC]);
			}
			else if(pControl == m_vButtonData[FRIEND].pButton)
			{
				if(m_ButtonSate[FRIEND] == 0)
					strcpy(str,"@拒绝好友");
				else
					strcpy(str,"@允许好友");
				m_ButtonSate[FRIEND] = !(m_ButtonSate[FRIEND]);
			}
			else if(pControl == m_vButtonData[GUILD].pButton)
			{
				if(m_ButtonSate[GUILD] == 0)
					strcpy(str,"@加入门派");
				else
					strcpy(str,"@退出门派");
				m_ButtonSate[GUILD] = !(m_ButtonSate[GUILD]);
			}
			else if(pControl == m_vButtonData[UNION].pButton)
			{
				strcpy(str,"@允许联盟");
				m_ButtonSate[UNION] = !(m_ButtonSate[UNION]);
			}
			else if(pControl == m_vButtonData[MEMORY].pButton)
			{
				if(m_ButtonSate[MEMORY] == 0)
					strcpy(str,"@允许天地合一");
				else
					strcpy(str,"@拒绝天地合一");
				m_ButtonSate[MEMORY] = !(m_ButtonSate[MEMORY]);
			}
			else if(pControl == m_vButtonData[RESUME].pButton)
			{
				if(m_pEdit && strlen(m_pEdit->GetText()) > 0)
				{
					strcpy(str,"@拒绝 ");
					strcat(str,m_pEdit->GetText());
				}
			}
			else
				break;
			if(strlen(str))
			{
				if(strcmp(str,"@退出门派") == 0 && strlen(SELF.GetTitle()) > 0 && !g_GuildData.IsHeader())
				{
					g_MsgBoxMgr.PopSimpleComfirm("离开行会后你所有的行会职位和行会银两都会清空！\n你是否确定离开行会？",MSG_CTRL_LEAVE_GUILD,0);
				}
				else
				{
					g_pGameControl->SEND_Message_Send(str,strlen(str));
				}
				g_pControl->Msg(MSG_CTRL_COMMAND_WND,0,this);
			}
		}
		return true;
	default:
		break;
	}
	return  CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CComandWnd::SwitchState(int i)
{
	if(i >= 0 && i < BUTTON_NUMS)
		m_ButtonSate[i] = !m_ButtonSate[i];
}
