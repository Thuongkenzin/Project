// HookDll.cpp : Defines the exported functions for the DLL application.
//
#include "stdafx.h"
#include "HookDll.h"
HHOOK hHook = NULL;
LRESULT CALLBACK MouseHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (wParam == WM_LBUTTONDOWN)
	{
		return TRUE;
	}

	return CallNextHookEx(hHook, nCode, wParam, lParam);
}

	
void CreateHook(HWND hWnd)			
{
	if (hHook != NULL) return;		
	hHook = SetWindowsHookEx(WH_MOUSE_LL, (HOOKPROC)MouseHookProc, 0, 0);	
}
void RemoveHook(HWND hWnd)			
{
	if (hHook == NULL)	return;	
	UnhookWindowsHookEx(hHook);	
	hHook = NULL;
}


