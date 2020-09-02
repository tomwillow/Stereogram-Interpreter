#include "TStereogramWindow.h"

#include <stdexcept>
using namespace std;

LRESULT TStereogramWindow::OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	img = new TStereogram("dog.jpg");//stereo.jpg
	SetFocus();
	return 0;
}

LRESULT TStereogramWindow::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	DestroyWindow();
	return 0;
}

LRESULT TStereogramWindow::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (img)
		delete img;

	ReleaseDC(hdcMem);

	PostQuitMessage(0);
	return 0;
}



void TStereogramWindow::Draw(HDC hdc, const RECT& rect)
{
	img->Draw(hdc, 0, 0, rcClient.right, rcClient.bottom);
}

LRESULT TStereogramWindow::OnEraseBkGnd(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return false;
}

LRESULT TStereogramWindow::OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(&ps);

	if (doubleBuffer)
	{
		Draw(hdcMem, ps.rcPaint);

		BitBlt(hdc, 0, 0, ps.rcPaint.right, ps.rcPaint.bottom, hdcMem, 0, 0, SRCCOPY);
	}
	else
		Draw(ps.hdc, ps.rcPaint);

	EndPaint(&ps);
	return 0;
}


LRESULT TStereogramWindow::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	//��ΪWM_SIZE���ֶ����ͣ�����rcClient����ȡ��wParam�е�ֵ
	GetClientRect(&rcClient);

	HDC hdc = GetDC();

	ReleaseDC(hdcMem);
	hdcMem = CreateCompatibleDC(hdc);

	//����hdcMem��С
	HBITMAP hBitmapMem = CreateCompatibleBitmap(hdc, rcClient.right, rcClient.bottom);
	SelectObject(hdcMem, hBitmapMem);
	DeleteObject(hBitmapMem);
	ReleaseDC(hdc);

	return 0;
}

LRESULT TStereogramWindow::OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	static int offset = 0;
	switch (wParam)
	{
	case VK_LEFT:
		offset -= 1;
		if (offset == 0)
			img->Restore();
		else
			img->SetOffset(offset);
		Invalidate();
		break;
	case VK_RIGHT:
		offset += 1;
		if (offset == 0)
			img->Restore();
		else
			img->SetOffset(offset);
		Invalidate();
		break;
	case VK_CONTROL:
		isCtrlDown = true; break;
	case 'V':
		if (isCtrlDown)
		{
			unsigned char* data = nullptr;
			DWORD data_size = 0;

#ifdef _DEBUG
			AllocConsole();
			freopen("CONOUT$", "w+t", stdout);
#endif

			if (OpenClipboard() == false)
				MessageBox("�򿪼�����ʧ�ܡ�", "��Ϣ", MB_ICONINFORMATION);

			UINT clipboard_format = 0;
			while (clipboard_format = EnumClipboardFormats(clipboard_format))
			{
				printf("Clipboard Data Format = 0x%x\n", clipboard_format);
				HGLOBAL global_memory = GetClipboardData(clipboard_format);
				LPCSTR clipboard_data = (LPCSTR)GlobalLock(global_memory);
				if (clipboard_data != NULL)
				{

					// ���а������ݴ�С
					data_size = GlobalSize(global_memory);

#ifdef _DEBUG
					// ���а������ݵĵ�ַ
					printf("Clipboard Data Address = 0x%x\n", global_memory);
					printf("Data Size = %d\n", data_size);

					char buf[MAX_PATH];
					GetClipboardFormatName(clipboard_format, buf, MAX_PATH);
					printf(buf);
					printf("\n");
#endif

					// �����������
					switch (clipboard_format)
					{
					case CF_DIB://8
					{
						data = new unsigned char[data_size];
						memcpy(data, clipboard_data, data_size);
						break;
					}
					}

					if (data_size < 1024)
					{
						printf(clipboard_data);
					}

				}
				printf("\n\n");
				GlobalUnlock(global_memory);
			}
			CloseClipboard();

			//�õ����ݣ�����ͼƬ
			if (data)
			{
				if (img)
					delete img;
				img = new TStereogram((unsigned char*)data, data_size, true);
				Invalidate();
				offset = 0;
			}
			else
			{
				MessageBox("�ڼ�������û�з�����ЧͼƬ��", "��Ϣ", MB_ICONINFORMATION);
			}
		}
		break;
	}
	return 0;
}

LRESULT TStereogramWindow::OnKeyUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	switch (wParam)
	{
	case VK_CONTROL:
		isCtrlDown = false; break;
	}
	return 0;
}

//LRESULT TStereogramWindow::OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
//{
//	switch (wParam)
//	{
//	case 0x03:						// ETX 
//		if (isCtrlDown && isCDown)
//		{
//			MessageBeep(0);
//		}
//			//PostMessage(hwnd, WM_COPY, 121, 0);
//		break;
//	}
//	return 0;
//}

//LRESULT TStereogramWindow::OnFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
//{
//	OutputDebugString("WM_SETFOCUS\n");
//	return 0;
//}
//
//LRESULT TStereogramWindow::OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
//{
//	OutputDebugString("WM_LBUTTONDOWN\n");
//	SetFocus();
//	return 0;
//}
