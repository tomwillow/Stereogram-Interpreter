#pragma once

#include <atlbase.h>        // ������ATL��
extern CComModule _Module;  // ȫ��_Module
#include <atlwin.h>         // ATL������

#include "TStereogram.h"
#include "TStereogramWindow.h"

class MainWindow :public CWindowImpl<MainWindow, CWindow, CFrameWinTraits>
{
private:
	TStereogramWindow stereo_window;
public:
	DECLARE_WND_CLASS(_T("Stereogram Degenerate"))

	BEGIN_MSG_MAP(MainWindow)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
	END_MSG_MAP()

	MainWindow() {}

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};
