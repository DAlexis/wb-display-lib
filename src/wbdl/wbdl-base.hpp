/*
 * wbdl-base.hpp
 *
 *  Created on: 20 апр. 2017 г.
 *      Author: dalexies
 */

#ifndef WBDL_WBDL_BASE_HPP_
#define WBDL_WBDL_BASE_HPP_

namespace wbdl {

enum class Color
{
	black = 0,
	white
};

enum class BitsOrder
{
	vertical = 0,
};

/**
 * This class store frame with defined bits order
 * and make trivial operations like putPixel and getPixel
 */
struct FrameBuffer
{
	void clearDirty();
	bool isDirty();
	unsigned int bufferSize();
	void makePointDirty(unsigned int x, unsigned int y);

	void putPixelNoDirty(int x, int y, Color c);
	Color getPixel(int x, int y) const;

	/**
	 * Heap may be not avaliable on your MCU, so you
	 * can use even static array as frame buffer
	 */
	char* buffer = nullptr;

	unsigned int dirtyX0 = 0;
	unsigned int dirtyY0 = 0;

	/**
	 * dirtyX1 is coordinate of point after latest dirty pixel,
	 * so dirtyX1 may == sizeX+1
	 */
	unsigned int dirtyX1 = 0;
	unsigned int dirtyY1 = 0;

	unsigned int width;
	unsigned int height;

	BitsOrder order = BitsOrder::vertical;
};

} // namespace wbdl

#endif /* WBDL_WBDL_BASE_HPP_ */
