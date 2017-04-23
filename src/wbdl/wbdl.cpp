#include "wbdl.hpp"

#include <algorithm>

using namespace wbdl;

int ScreenParameters::bufferSize()
{
	return sizeX * sizeY / 8;
}

//////////////////////////

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

//////////////////////////

Display::Display(IDisplayDriver& driver, FrameBuffer& frameBuffer) :
	m_driver(driver),
	m_frameBuffer(frameBuffer),
	m_parameters(driver.parameters())
{
}

void Display::updateScreen()
{
	m_driver.updateScreen(m_frameBuffer);
	m_frameBuffer.clearDirty();
}

void Display::putPixel(int x, int y, Color c)
{
	putPixelNoDirty(x, y, c);
	m_frameBuffer.makePointDirty(x, y);
}

void Display::line(int x0, int y0, int x1, int y1, Color c)
{
	m_frameBuffer.makePointDirty(x0, y0);
	m_frameBuffer.makePointDirty(x1, y1);

	if (abs(x1-x0) > abs(y1-y0))
	{
		if (x1 < x0)
		{
			std::swap(x0, x1);
			std::swap(y0, y1);
		}
		for (int x = x0; x<=x1; x++)
			putPixelNoDirty(x, y0 + (x-x0) * (y1 - y0) / (x1-x0), c);
	} else {
		if (y1 < y0)
		{
			std::swap(x0, x1);
			std::swap(y0, y1);
		}
		for (int y = y0; y<=y1; y++)
			putPixelNoDirty(x0 + (y-y0) * (x1 - x0) / (y1-y0), y, c);
	}
}

int Display::left()
{
	return 0;
}

int Display::right()
{
	return m_parameters.sizeX-1;
}

int Display::top()
{
	return 0;
}

int Display::bottom()
{
	return m_parameters.sizeY-1;
}

int Display::centerX()
{
	return m_parameters.sizeX / 2;
}

int Display::centerY()
{
	return m_parameters.sizeY / 2;
}

void Display::putPixelNoDirty(int x, int y, Color c)
{
	if (x < 0 || y < 0 || x >= int(m_parameters.sizeX) || y >= int(m_parameters.sizeY))
		return;
	unsigned int index;
	switch(m_parameters.fbOrientation)
	{
	case FrameBufferOrientation::horizontal:
		index = x / 8 + y * m_parameters.sizeX / 8;
		if (c == Color::white)
			m_frameBuffer.buffer[index] |= 1 << (x%8);
		else
			m_frameBuffer.buffer[index] &= ~(1 << (x%8));
		break;
	case FrameBufferOrientation::vertical:
		index = y / 8 + x * m_parameters.sizeY / 8;
		if (c == Color::white)
			m_frameBuffer.buffer[index] |= 1 << (x%8);
		else
			m_frameBuffer.buffer[index] &= ~(1 << (x%8));
		break;
	}
}
