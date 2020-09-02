#pragma once

#include <atlbase.h>        // ������ATL��
extern CComModule _Module;  // ȫ��_Module
#include <atlwin.h>         // ATL������

#include "TStereogram.h"

class TStereogramWindow :public CWindowImpl<TStereogramWindow, CWindow, CFrameWinTraits>
{
private:
	RECT rcClient;
	HDC hdcMem;
	TStereogram* img;

	bool isCtrlDown;
public:
	bool doubleBuffer;
	DECLARE_WND_CLASS(_T("my"))

	BEGIN_MSG_MAP(TStereogramWindow)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
		MESSAGE_HANDLER(WM_ERASEBKGND, OnEraseBkGnd)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
		MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
		//MESSAGE_HANDLER(WM_KEYUP, OnKeyUp)
		//MESSAGE_HANDLER(WM_CHAR, OnChar)
		//MESSAGE_HANDLER(WM_SETFOCUS, OnFocus)
		//MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
	END_MSG_MAP()

	TStereogramWindow() :doubleBuffer(true),isCtrlDown(false) {}

	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	void Draw(HDC hdc, const RECT& rect);

	//�ڶ��̶߳���ʱ���϶����ڻ��������������Ĭ������»ᷢ����ɫ��˸��
	//����WM_ERASEBKGND������false������Ϊ��������������˸
	LRESULT OnEraseBkGnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnKeyUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//LRESULT OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//LRESULT OnFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	//LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};