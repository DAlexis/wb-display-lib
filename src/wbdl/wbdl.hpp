/*
 * wbdl.hpp
 *
 *  Created on: 20 апр. 2017 г.
 *      Author: dalexies
 */

#ifndef WBDL_WBDL_HPP_
#define WBDL_WBDL_HPP_

#include "wbdl-driver.hpp"

namespace wbdl {

class Display
{
public:
	Display(IDisplayDriver& driver, FrameBuffer& frameBuffer);
	void updateScreen();

	// Filling
	void fill(Color c = Color::white);

	// Graphic primitives
	void putPixel(int x, int y, Color c = Color::white);
	void line(int x0, int y0, int x1, int y1, Color c = Color::white);
	void circle(int x0, int y0, int r, Color c = Color::white);

	// Coordinate constants
	int left();
	int right();

	int top();
	int bottom();

	int centerX();
	int centerY();

private:
	void enlargeDirty(int x, int y);
	IDisplayDriver& m_driver;
	FrameBuffer& m_frameBuffer;
};

}


#endif /* WBDL_WBDL_HPP_ */
