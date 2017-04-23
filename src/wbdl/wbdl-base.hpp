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

enum class FrameBufferOrientation
{
	horizontal = 0,
	vertical
};

struct ScreenParameters
{
	int bufferSize();

	unsigned int sizeX;
	unsigned int sizeY;

	FrameBufferOrientation fbOrientation = FrameBufferOrientation::horizontal;
};

struct FrameBuffer
{
	void clearDirty();
	bool isDirty();
	void makePointDirty(unsigned int x, unsigned int y);
	/**
	 * For those devices or people that may have problems with heap on MCU,
	 * I don't use unique_ptr here, so manage memory manually.
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
};

} // namespace wbdl

#endif /* WBDL_WBDL_BASE_HPP_ */
