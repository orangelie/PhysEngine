#pragma once

class Input
{
private:
	Input() { }
	virtual ~Input() {}

public:
	static Input* GetInstance() { return &_input; }

public:
	void Init();
	void Update();

	uint32 GetKeyState(uint32 keyStroke) { return (_keyState[keyStroke] & 0x80); }
	DIMOUSESTATE GetMouseDelta() const { return _mouseState; }
	POINT GetMousePos() const { return POINT(_mouseX, _mouseY); }

private:
	static Input _input;

private:
	IDirectInput8* _directInput = nullptr;
	IDirectInputDevice8* _mouseDevice = nullptr;
	IDirectInputDevice8* _keyboardDevice = nullptr;

	unsigned char _keyState[256] = {};

	DIMOUSESTATE _mouseState = {};
	int _screenWidth, _screenHeight;
	int _mouseX, _mouseY;

};

