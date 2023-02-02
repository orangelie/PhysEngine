#include "pch.h"
#include "Client.h"
#include "Engine.h"
#include "Win32Core.h"

void Client::Init(HINSTANCE hInstace)
{
	GEngine->Init(hInstace);
}

void Client::Update()
{
	MSG msg = {};

	for (;;)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			DispatchMessage(&msg);
			TranslateMessage(&msg);
		}

		if (msg.message == WM_QUIT)
			break;
	}

	GEngine.release();
}
