#include "Program.h"
#include "Game.h"

#include "Debug.h"

MainArgs g_MainArgs;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd){
#ifdef _DEBUG
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	g_MainArgs.hInstance = hInstance;
	g_MainArgs.hPrevInstance = hPrevInstance;
	g_MainArgs.lpCmdLine = lpCmdLine;
	g_MainArgs.nShowCmd = nShowCmd;

	Game* p = GenerateGame();

	if(!p) return 0;

	p->Initialize();

	p->Run();

	delete p;

	return 0;
};
