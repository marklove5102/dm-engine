#include "stdafx.h"
#include "GameServer.h"
#include "server.h"
#include "fmttextfile.h"
#include "monsterex.h"
#include "scriptobject.h"
#include "HumanPlayerMgr.h"

CServerForm g_Form;
CServer* g_pServer = nullptr;

// 崩溃前数据保存回调
static VOID OnPreCrashSave()
{
	if (g_pServer)
		g_pServer->OnTerminated(TRUE);
}

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, ".936");
	TRY_INIT
	CLogFile::GetInstance()->Init("..\\日志\\龙路引擎");
	// 组合命令行参数
	std::array<char, 256> cmdLine = { 0 };
	for (int i = 1; i < argc; i++)
	{
		if (i > 1) strcat(cmdLine.data(), " ");
		strcat(cmdLine.data(), argv[i]);
	}
	g_Form.Create(SERVER_NAME, cmdLine.data());

	g_Form.SetArenaReserve(256 * 1024);
	g_pServer = CServer::GetInstance();
	// 注册崩溃前数据保存回调
	CrashHandler::SetPreCrashSaveCallback(OnPreCrashSave);
	g_pServer->SetServerName(SERVER_NAME);
	g_pServer->SetIoConsole(&g_Form);
	g_Form.SetInputListener(g_pServer);
	g_Form.SetServer(g_pServer);
	g_Form.SetStatus(TRUE);
	return g_Form.EnterMessageLoop();
}