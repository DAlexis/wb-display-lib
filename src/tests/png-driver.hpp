/*
 * png-driver.hpp
 *
 *  Created on: 22 апр. 2017 г.
 *      Author: dalexies
 */

#ifndef FUNCTIONAL_TESTS_PNG_DRIVER_HPP_
#define FUNCTIONAL_TESTS_PNG_DRIVER_HPP_

#include "wbdl-driver.hpp"

#include <string>

class PNGDriver : public wbdl::IDisplayDriver
{
public:
	PNGDriver(unsigned int sizeX, unsigned int sizeY, bool fbIsVertial = false, const std::string& filename = "display.png");
	~PNGDriver();

	void setFilename(const std::string& filename);

	wbdl::ScreenParameters& parameters() override;
	bool updateScreen(const wbdl::FrameBuffer& frameBuffer) override;

private:
	char* m_horizontalFrameBuffer = nullptr;
	wbdl::ScreenParameters m_parameters;
	std::string m_filename;
};


#endif /* FUNCTIONAL_TESTS_PNG_DRIVER_HPP_ */
