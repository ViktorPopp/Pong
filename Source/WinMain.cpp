#include <Windows.h>
#include <stdint.h>
#include <iostream>
#include "Renderer.h"

bool running = true;

static LRESULT __stdcall windowCallback(_In_ HWND hwnd, _In_ UINT uMsg, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	LRESULT result = 0;

	switch (uMsg) {
	case WM_CLOSE:
	case WM_DESTROY: {
		running = false;
	} break;
	case WM_SIZE: {
		RECT rect;
		GetClientRect(hwnd, &rect);
		rendererStorage.bufferWidth = rect.right - rect.left;
		rendererStorage.bufferHeight = rect.bottom - rect.top;
		uint32_t bufferSize = rendererStorage.bufferWidth * rendererStorage.bufferHeight * sizeof(uint32_t);

		if (rendererStorage.bufferMemory) VirtualFree(rendererStorage.bufferMemory, 0, MEM_RELEASE);
		rendererStorage.bufferMemory = VirtualAlloc(0, bufferSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

		rendererStorage.bitmapInfo.bmiHeader.biSize = sizeof(rendererStorage.bitmapInfo.bmiHeader);
		rendererStorage.bitmapInfo.bmiHeader.biWidth = rendererStorage.bufferWidth;
		rendererStorage.bitmapInfo.bmiHeader.biHeight = rendererStorage.bufferHeight;
		rendererStorage.bitmapInfo.bmiHeader.biPlanes = 1;
		rendererStorage.bitmapInfo.bmiHeader.biBitCount = 32;
		rendererStorage.bitmapInfo.bmiHeader.biCompression = BI_RGB;
	} break;
	default: 
		result = DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	return result;
}

int __stdcall WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	WNDCLASS windowClass = {};
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpszClassName = L"Game Window Class";
	windowClass.lpfnWndProc = windowCallback;
	RegisterClass(&windowClass);
	HWND window = CreateWindow(windowClass.lpszClassName, L"Pong", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0);
	HDC hdevice = GetDC(window);

	while (running) {
		/* Input */
		MSG msg;
		while (PeekMessage(&msg, window, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		/* Update */
		Renderer::ClearScreen(0xff5500);
		Renderer::DrawRectInPixels(0, 0, 20, 100, 0x000000);

		/* Render */
		StretchDIBits(hdevice, 0, 0, rendererStorage.bufferWidth, rendererStorage.bufferHeight, 0, 0, rendererStorage.bufferWidth, rendererStorage.bufferHeight, rendererStorage.bufferMemory, &rendererStorage.bitmapInfo, DIB_RGB_COLORS, SRCCOPY);
	}
}
