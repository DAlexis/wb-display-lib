/*
 * wbdl-driver.hpp
 *
 *  Created on: 20 апр. 2017 г.
 *      Author: dalexies
 */

#ifndef WBDL_WBDL_DRIVER_HPP_
#define WBDL_WBDL_DRIVER_HPP_

#include "wbdl-base.hpp"

namespace wbdl {

class IDisplayDriver
{
public:
	virtual ~IDisplayDriver() {}
	virtual bool updateScreen(const FrameBuffer& frameBuffer) = 0;
};

} // namespace wbdl

#endif /* WBDL_WBDL_DRIVER_HPP_ */
