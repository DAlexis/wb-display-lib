#include "wbdl.hpp"

#include <algorithm>

using namespace wbdl;

//////////////////////////
// FrameBuffer

void FrameBuffer::clearDirty()
{
	dirtyX0 = 0;
	dirtyY0 = 0;

	dirtyX1 = 0;
	dirtyY1 = 0;
}

bool FrameBuffer::isDirty()
{
	return dirtyX0 != dirtyX1;
}

unsigned int FrameBuffer::bufferSize()
{
	return width * height / 8;
}

void FrameBuffer::makePointDirty(unsigned int x, unsigned int y)
{
	if (isDirty())
	{
		dirtyX0 = x; dirtyX1 = x+1;
		dirtyY0 = y; dirtyY1 = y+1;
		return;
	}

	if (x < dirtyX0)
		dirtyX0 = x;
	else if (x+1 > dirtyX1)
		dirtyX1 = x+1;

	if (y < dirtyY0)
		dirtyY0 = y;
	else if (y+1 > dirtyY1)
		dirtyY1 = y+1;
}

void FrameBuffer::putPixelNoDirty(int x, int y, Color c)
{
	if (x < 0 || y < 0 || x >= int(width) || y >= int(height))
		return;
	unsigned int index;
	switch(order)
	{
	case BitsOrder::vertical:
		index = x + (y / 8) * width;
		if (c == Color::white)
			buffer[index] |= 1 << (y%8);
		else
			buffer[index] &= ~(1 << (y%8));
		break;
	}
}

Color FrameBuffer::getPixel(int x, int y) const
{
	if (x < 0 || y < 0 || x >= int(width) || y >= int(height))
		return Color::black;
	unsigned int index;
	switch(order)
	{
	case BitsOrder::vertical:
		index = x + (y / 8) * width;
		return (buffer[index] & (1 << (y%8))) != 0 ? Color::white : Color::black;
	}
	return Color::black;
}

//////////////////////////
// Display

Display::Display(IDisplayDriver& driver, FrameBuffer& frameBuffer) :
	m_driver(driver),
	m_frameBuffer(frameBuffer)
{
}

void Display::updateScreen()
{
	m_driver.updateScreen(m_frameBuffer);
	m_frameBuffer.clearDirty();
}

void Display::putPixel(int x, int y, Color c)
{
	m_frameBuffer.putPixelNoDirty(x, y, c);
	m_frameBuffer.makePointDirty(x, y);
}

void Display::line(int x0, int y0, int x1, int y1, Color c)
{
	m_frameBuffer.makePointDirty(x0, y0);
	m_frameBuffer.makePointDirty(x1, y1);

	/**
	 * It is a realization of Bresenham's line algorithm
	 * without floats
	 */
	int dx = (x0 < x1) ? (x1 - x0) : (x0 - x1);
	int dy = (y0 < y1) ? (y1 - y0) : (y0 - y1);
	int sx = (x0 < x1) ? 1 : -1;
	int sy = (y0 < y1) ? 1 : -1;

	int err = ((dx > dy) ? dx : -dy) / 2;

	for(;;)
	{
		m_frameBuffer.putPixelNoDirty(x0, y0, c);
		if (x0 == x1 && y0 == y1) {
			break;
		}

		int oldErr = err;
		if (oldErr > -dx)
		{
			err -= dy;
			x0 += sx;
		}
		if (oldErr < dy)
		{
			err += dx;
			y0 += sy;
		}
	}
}

void Display::circle(int x0, int y0, int r, Color c)
{
	int x = 0;
	int y = r;
	int delta = 1 - 2 * r;
	int error = 0;
	while (x <= y)
	{
		m_frameBuffer.putPixelNoDirty(x0 + x, y0 + y, c);
		m_frameBuffer.putPixelNoDirty(x0 + x, y0 - y, c);
		m_frameBuffer.putPixelNoDirty(x0 - x, y0 + y, c);
		m_frameBuffer.putPixelNoDirty(x0 - x, y0 - y, c);

		m_frameBuffer.putPixelNoDirty(x0 + y, y0 + x, c);
		m_frameBuffer.putPixelNoDirty(x0 + y, y0 - x, c);
		m_frameBuffer.putPixelNoDirty(x0 - y, y0 + x, c);
		m_frameBuffer.putPixelNoDirty(x0 - y, y0 - x, c);

		error = 2 * (delta + y) - 1;
		if ((delta < 0) && (error <= 0))
		{
		   delta += 2 * ++x + 1;
		   continue;
		}
		error = 2 * (delta - x) - 1;
		if ((delta > 0) && (error > 0))
		{
		   delta += 1 - 2 * --y;
		   continue;
		}
		x++;
		delta += 2 * (x - y);
		y--;
	}
}

int Display::left()
{
	return 0;
}

int Display::right()
{
	return m_frameBuffer.width-1;
}

int Display::top()
{
	return 0;
}

int Display::bottom()
{
	return m_frameBuffer.height-1;
}

int Display::centerX()
{
	return m_frameBuffer.width / 2;
}

int Display::centerY()
{
	return m_frameBuffer.height / 2;
}
