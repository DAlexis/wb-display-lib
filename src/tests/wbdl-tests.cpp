#include "wbdl.hpp"
#include "png-driver.hpp"
#include "gtest/gtest.h"

#include <fstream>
#include <memory>
#include <cstring>

using namespace wbdl;

class DisplayTest : public ::testing::Test
{
protected:
	void SetUp() override
	{

	}

	void initFrameBuffer()
	{
		fb.buffer = new char[png->parameters().bufferSize()];
		memset(fb.buffer, 0, png->parameters().bufferSize() * sizeof(char));
	}

	void initHorizontal()
	{
		png.reset(new PNGDriver(width, height, false));
		display.reset(new Display(*png, fb));
		initFrameBuffer();
	}

	void initVertical()
	{
		png.reset(new PNGDriver(width, height, true));
		display.reset(new Display(*png, fb));
		initFrameBuffer();
	}

	void TearDown() override
	{
		png.reset();
		display.reset();
		delete[] fb.buffer;
	}

	constexpr static unsigned int width = 256;
	constexpr static unsigned int height = 128;

	FrameBuffer fb;
	std::unique_ptr<PNGDriver> png;
	std::unique_ptr<Display> display;
};

bool compareFiles(const std::string& file1, const std::string& file2)
{
	return false;
}



TEST(PNGDriverTest, OrientationTest)
{
	unsigned int w = 256, h = 128;
	FrameBuffer fb;
	{
		PNGDriver png(w, h, false, "png-driver-test-horizontal.png");
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
		PNGDriver png(w, h, true, "png-driver-test-vertical.png");
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

TEST_F(DisplayTest, PutPixel)
{
	ASSERT_NO_THROW(initHorizontal());
	png->setFilename("put-pixel-test.png");
	ASSERT_NO_THROW(display->putPixel(10, 10, Color::white));
	ASSERT_NO_THROW(display->putPixel(20, 10, Color::white));
	ASSERT_NO_THROW(display->putPixel(20, 10, Color::black));
	ASSERT_NO_THROW(display->putPixel(100, 10, Color::white));
	ASSERT_NO_THROW(display->putPixel(10, 100, Color::white));
	ASSERT_NO_THROW(display->putPixel(100, 100, Color::white));
	ASSERT_NO_THROW(display->putPixel(display->left(), display->top(), Color::white));
	ASSERT_NO_THROW(display->putPixel(display->right(), display->top(), Color::white));
	ASSERT_NO_THROW(display->putPixel(display->left(), display->bottom(), Color::white));
	ASSERT_NO_THROW(display->putPixel(display->right(), display->bottom(), Color::white));

	display->updateScreen();
}

TEST_F(DisplayTest, Line)
{
	ASSERT_NO_THROW(initHorizontal());
	png->setFilename("line-test.png");

	ASSERT_NO_THROW(display->line(display->left(), display->top(), display->left()+10, display->top()+10, Color::white));
	ASSERT_NO_THROW(display->line(display->left(), display->top(), display->left()+20, display->top()+2, Color::white));
	ASSERT_NO_THROW(display->line(display->left(), display->top(), display->left()+2, display->top()+20, Color::white));

	ASSERT_NO_THROW(display->line(display->right(), display->top(), display->right()-10, display->top()+10, Color::white));
	ASSERT_NO_THROW(display->line(display->right(), display->top(), display->right()-2, display->top()+20, Color::white));
	ASSERT_NO_THROW(display->line(display->right(), display->top(), display->right()-20, display->top()+2, Color::white));

	ASSERT_NO_THROW(display->line(display->right(), display->bottom(), display->right()-10, display->bottom()-10, Color::white));
	ASSERT_NO_THROW(display->line(display->right(), display->bottom(), display->right()-2, display->bottom()-20, Color::white));
	ASSERT_NO_THROW(display->line(display->right(), display->bottom(), display->right()-20, display->bottom()-2, Color::white));


	ASSERT_NO_THROW(display->line(display->left(), display->bottom(), display->left()+10, display->bottom()-10, Color::white));
	ASSERT_NO_THROW(display->line(display->left(), display->bottom(), display->left()+2, display->bottom()-20, Color::white));
	ASSERT_NO_THROW(display->line(display->left(), display->bottom(), display->left()+20, display->bottom()-2, Color::white));

	display->updateScreen();
}
