#include "wbdl.hpp"
#include "png-driver.hpp"
#include "gtest/gtest.h"

#include <cstring>

using namespace wbdl;

TEST(PNGDriverTest, OrientationTest)
{
	unsigned int w = 256, h = 128;
	FrameBuffer fb;
	{
		PNGDriver png("png-driver-test-horizontal.png", w, h);
		fb.buffer = new char[png.parameters().bufferSize()];
		memset(fb.buffer, 0, png.parameters().bufferSize() * sizeof(char));
		for (unsigned int y = 0; y<png.parameters().sizeY; y++)
			for (unsigned int x = 0; x<png.parameters().sizeX; x++)
			{
				char val = ((2*x+y) % 7 == 0 || x % 6 == 0);
				fb.buffer[x / 8 + y * png.parameters().sizeX / 8] |= val << (x%8);
			}

		ASSERT_NO_THROW(png.updateScreen(fb));
		delete[] fb.buffer;
	}
	{
		PNGDriver png("png-driver-test-vertical.png", w, h, true);
		fb.buffer = new char[png.parameters().bufferSize()];
		memset(fb.buffer, 0, png.parameters().bufferSize() * sizeof(char));
		for (unsigned int y = 0; y<png.parameters().sizeY; y++)
			for (unsigned int x = 0; x<png.parameters().sizeX; x++)
			{
				char val = ((2*x+y) % 7 == 0 || x % 6 == 0);
				fb.buffer[y / 8 + x * png.parameters().sizeY / 8] |= val << (y%8);
			}

		ASSERT_NO_THROW(png.updateScreen(fb));
		delete[] fb.buffer;
	}
}
