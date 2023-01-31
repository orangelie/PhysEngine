#include "pch.h"
#include "Input.h"
#include "Engine.h"
#include "Win32Core.h"

Input Input::_input = {};

void Input::Init()
{
	SetCursorPos(0, 0);
	_mouseX = 0;
	_mouseY = 0;

	_screenWidth = GetSystemMetrics(SM_CXSCREEN);
	_screenHeight = GetSystemMetrics(SM_CYSCREEN);

	HWND hwnd = GEngine->GetWin32Core()->GetHwnd();
	HINSTANCE hInst = GetModuleHandle(0);

	HR(DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&_directInput, 0));
	HR(_directInput->CreateDevice(GUID_SysKeyboard, &_keyboardDevice, 0));
	HR(_directInput->CreateDevice(GUID_SysMouse, &_mouseDevice, 0));

	HR(_keyboardDevice->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	HR(_keyboardDevice->SetDataFormat(&c_dfDIKeyboard));
	_keyboardDevice->Acquire();

	HR(_mouseDevice->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	HR(_mouseDevice->SetDataFormat(&c_dfDIMouse));
	_mouseDevice->Acquire();
}

void Input::Update()
{
	HRESULT result;

	result = this->_keyboardDevice->GetDeviceState(sizeof(_keyState), &_keyState);
	if (FAILED(result)) {
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED) {
			_keyboardDevice->Acquire();
		}
		else {
			exit(-1);
			return;
		}
	}

	result = _mouseDevice->GetDeviceState(sizeof(DIMOUSESTATE), &_mouseState);
	if (FAILED(result)) {
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED) {
			_mouseDevice->Acquire();
		}
		else {
			exit(-1);
			return;
		}
	}

	_mouseX += _mouseState.lX;
	_mouseY += _mouseState.lY;

	if (_mouseX < 0) { _mouseX = 0; }
	if (_mouseY < 0) { _mouseY = 0; }
	if (_mouseX > _screenWidth) { _mouseX = _screenWidth; }
	if (_mouseY > _screenHeight) { _mouseY = _screenHeight; }
}
