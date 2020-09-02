#pragma once
#include <Windows.h>
#include <vector>
#include <unordered_map>
#include "tstring.h"

class TControl
{
private:
	bool bCanAcceptDrag;
	using PMsgDealer = LRESULT(*)(HWND hWnd,WPARAM wParam, LPARAM lParam);
	std::unordered_map<UINT,PMsgDealer > msgDealer;

	HFONT m_hFont;
	static LRESULT CALLBACK subControlProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	std::vector<std::tstring> PreDrop(WPARAM wParam) const;
public:
	//LONG m_iWidth, m_iHeight;

	TControl(const TControl& control);
	TControl& operator=(const TControl& control);

	HWND GetHWND();

	void LinkControl(HWND hWndCtrl);
	void LinkControl(HWND hDlg, int id);

	RECT GetClientRect() const;
	RECT GetWindowRect() const;

	void SetRect(RECT &rect);
	void SetRect(int x1, int y1, int x2, int y2); 
	//void SetPos(RECT &rect);
	void Invalidate();

	void SetFont(HFONT hFont);
	void SetDefaultGuiFont();
	void SetFont(TCHAR FontName[], int FontSize);

	void SetText(const std::tstring &s);
	void GetText(TCHAR text[]);
	std::tstring GetText() const;
	int GetLength() const;//��ȡ�ַ�������	

	RECT GetPosition() const;
	void SetPosition(int x, int y, int width, int height);//���ô�С��λ��
	void SetPosition(RECT rect);//���ô�С��λ��
	void SetPositionOnlyOrigin(const RECT &rect);

	void SetVisible(bool bShow);//���ÿɼ���
	bool GetVisible();

	void SetDouble(double d);

	virtual void SetEnable(bool bEnable);
	virtual bool GetEnable();

	void SetID(int id);

	void SetDragAccept(bool bCanAcceptDrop);

	void RegisterMessage(UINT uMsg,PMsgDealer pFun);
protected:
	HWND m_hParent;
	HWND m_hWnd;
	HINSTANCE m_hInst;
	TControl();
	virtual ~TControl();

	void RegisterProc();//�������ں�ע��

	virtual LRESULT WndProc(WNDPROC wndproc, HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);//������Ϣ���������ɸ���

	virtual LRESULT DropProc(const std::vector<std::tstring>& dropFiles);

	virtual void DoAfterLinkControl() {}
};
