#pragma once
#include <stdint.h>
#include <Windows.h>
#include "Utils.h"

struct RendererStorage {
	uint32_t bufferWidth = 0;
	uint32_t bufferHeight = 0;
	uint32_t bufferSize = 0;
	LPVOID bufferMemory = 0;
	BITMAPINFO bitmapInfo = BITMAPINFO();
	float renderScale = 0.01f;
};

RendererStorage rendererStorage;

struct Renderer {
	static void ClearScreen(uint32_t color) {
		uint32_t* pixel = (uint32_t*)rendererStorage.bufferMemory;
		for (uint32_t y = 0; y < rendererStorage.bufferHeight; y++) {
			for (uint32_t x = 0; x < rendererStorage.bufferWidth; x++) {
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

	static void DrawRect(float x, float y, float half_size_x, float half_size_y, uint32_t color) {
		x *= rendererStorage.bufferHeight * rendererStorage.renderScale;
		y *= rendererStorage.bufferHeight * rendererStorage.renderScale;
		half_size_x *= rendererStorage.bufferHeight * rendererStorage.renderScale;
		half_size_y *= rendererStorage.bufferHeight * rendererStorage.renderScale;

		x += rendererStorage.bufferWidth / 2.f;
		y += rendererStorage.bufferHeight / 2.f;

#pragma warning(push, 0)
		int x0 = x - half_size_x;
		int x1 = x + half_size_x;
		int y0 = y - half_size_y;
		int y1 = y + half_size_y;
#pragma warning(pop)

		DrawRectInPixels(x0, y0, x1, y1, color);
	}
};