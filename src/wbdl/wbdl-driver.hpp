/*
 * wbdl-driver.hpp
 *
 *  Created on: 20 апр. 2017 г.
 *      Author: dalexies
 */

#ifndef WBDL_WBDL_DRIVER_HPP_
#define WBDL_WBDL_DRIVER_HPP_

#include "wbdl-base.hpp"

class IWBDLDriver
{
public:
	virtual ~IWBDLDriver() {}
	virtual ScreenParameters& parameters() = 0;
	virtual void updateScreen(const FrameBuffer& frameBuffer);
};


#endif /* WBDL_WBDL_DRIVER_HPP_ */
