#include "MainWindow.h"

using namespace std;

LRESULT MainWindow::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	stereo_window.Create(m_hWnd);
	stereo_window.ModifyStyle(WS_OVERLAPPEDWINDOW, WS_CHILD);
	stereo_window.ShowWindow(SW_SHOW);
	stereo_window.SetParent(m_hWnd);
	return 0;
}

LRESULT MainWindow::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	DestroyWindow();
	return 0;
}

LRESULT MainWindow::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{

	PostQuitMessage(0);
	return 0;
}

LRESULT MainWindow::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	RECT rcClient;
	GetClientRect(&rcClient);

	stereo_window.MoveWindow(&rcClient);

	return 0;
}

LRESULT MainWindow::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return stereo_window.OnKeyDown(uMsg, wParam, lParam, bHandled);;
}
