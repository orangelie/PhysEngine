#include "pch.h"
#include "Client.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLIne, INT hCmdShow)
{
	Client client;
	client.Init(hInstance);
	client.Update();

	return 0;
}