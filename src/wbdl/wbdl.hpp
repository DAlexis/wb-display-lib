/*
 * wbdl.hpp
 *
 *  Created on: 20 апр. 2017 г.
 *      Author: dalexies
 */

#ifndef WBDL_WBDL_HPP_
#define WBDL_WBDL_HPP_

#include "wbdl-driver.hpp"

class Display
{
public:
	Display(IWBDLDriver& driver);

private:
	IWBDLDriver& m_driver;
};







#endif /* WBDL_WBDL_HPP_ */
