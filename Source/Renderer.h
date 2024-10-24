#pragma once
#include <stdint.h>
#include <Windows.h>
#include "Utils.h"

struct RendererStorage {
	uint32_t bufferWidth;
	uint32_t bufferHeight;
	uint32_t bufferSize;
	LPVOID bufferMemory;
	BITMAPINFO bitmapInfo;
};

RendererStorage rendererStorage;

struct Renderer {
	static void ClearScreen(uint32_t color) {
		unsigned int* pixel = (uint32_t*)rendererStorage.bufferMemory;
		for (int y = 0; y < rendererStorage.bufferHeight; y++) {
			for (int x = 0; x < rendererStorage.bufferWidth; x++) {
				*pixel++ = color;
			}
		}
	}
	static void DrawRectInPixels(int x0, int y0, int x1, int y1, uint32_t color) { 
		x0 = Clamp(0, x0, rendererStorage.bufferWidth);
		x1 = Clamp(0, x1, rendererStorage.bufferWidth);
		y0 = Clamp(0, y0, rendererStorage.bufferHeight);
		y1 = Clamp(0, y1, rendererStorage.bufferHeight);

		for (int y = y0; y < y1; y++) {
			uint32_t* pixel = (uint32_t*)rendererStorage.bufferMemory + x0 + y * rendererStorage.bufferWidth;
			for (int x = x0; x < x1; x++) {
				*pixel++ = color;
			}
		}
	}
	static void DrawRect() { }
};