/*
 * wbdl-base.hpp
 *
 *  Created on: 20 апр. 2017 г.
 *      Author: dalexies
 */

#ifndef WBDL_WBDL_BASE_HPP_
#define WBDL_WBDL_BASE_HPP_

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
	unsigned int sizeX;
	unsigned int sizeY;

	FrameBufferOrientation fbOrientation = FrameBufferOrientation::horizontal;
};

struct FrameBuffer
{
	char* buffer;

	unsigned int dirtyX0 = 0;
	unsigned int dirtyY0 = 0;

	unsigned int dirtyX1 = 0;
	unsigned int dirtyY1 = 0;
};

#endif /* WBDL_WBDL_BASE_HPP_ */
