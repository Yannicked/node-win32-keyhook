// GlobalKeypress.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "GlobalKeypress.h"
#include <iostream>

void CALLBACK dllcallback(DWORD keycode);
typedef void CALLBACK CallbackFunc(DWORD);

static HANDLE KeyboardThread;

static CallbackFunc* callbackUp;
static CallbackFunc* callbackDown;

static bool Created = false;

HHOOK keyboardHook{ NULL };

LRESULT CALLBACK LowLevelKeyBoardProc(const int nCode, const WPARAM wParam, const LPARAM lParam) {
	PKBDLLHOOKSTRUCT press = (PKBDLLHOOKSTRUCT)lParam;
	DWORD vkCode = press->vkCode;
	if (vkCode == 0x0d && (press->flags & LLKHF_EXTENDED) != 0) {
		vkCode = 0x0e;
	}
	switch (wParam) {
	case WM_KEYDOWN:
		callbackDown(vkCode);
		break;
	case WM_SYSKEYDOWN:
		callbackDown(vkCode);
		break;
	case WM_KEYUP:
		callbackUp(vkCode);
		break;
	case WM_SYSKEYUP:
		callbackUp(vkCode);
		break;
	}
	return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

DWORD WINAPI KeyboardAsync(LPVOID lpParam) {
	keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyBoardProc, NULL, 0);
	if (keyboardHook == NULL) {
		return false;
	}
	MSG ThreadMsg;
	while (GetMessage(&ThreadMsg, NULL, 0, 0)) {

	};
	return true;
}

extern "C" {
	GLOBALKEYPRESS_API bool Create(CallbackFunc* cbDown, CallbackFunc* cbUp) {
		if (Created) {
			return false;
		}
		callbackDown = cbDown;
		callbackUp = cbUp;
		KeyboardThread = CreateThread(
			NULL,
			0,
			KeyboardAsync,
			NULL,
			0,
			NULL);
		Created = true;
		return true;
	}

	GLOBALKEYPRESS_API bool Destroy(CallbackFunc* cb) {
		TerminateThread(KeyboardThread, 0);
		UnhookWindowsHookEx(keyboardHook);
		Created = false;
		return true;
	}
}
