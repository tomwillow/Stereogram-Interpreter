#include "MainWindow.h"
#include <string>

//dpi
#include <ShellScalingAPI.h>
#pragma comment(lib,"Shcore.lib")

#pragma comment(lib,"comctl32.lib")

CComModule _Module;

using namespace std;

const string AppTitle = "Stereogram Interpreter v0.1 beta";

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hInstPrev,
	LPSTR szCmdLine, int nCmdShow)
{
	InitCommonControls();
	SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);
	_Module.Init(NULL, hInst);

	MainWindow wndMain;
	MSG msg;

	if (NULL == wndMain.Create(NULL, CWindow::rcDefault,AppTitle.c_str()))
	{
		return 1;
	}

	wndMain.ShowWindow(nCmdShow);
	wndMain.UpdateWindow();

	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
	}

	_Module.Term();
	return msg.wParam;
}