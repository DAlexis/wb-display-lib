#include "wbdl.hpp"

using namespace wbdl;

unsigned int ScreenParameters::bufferSize()
{
	return sizeX * sizeY / 8;
}

//////////////////////////

Display::Display(IDisplayDriver& driver) :
	m_driver(driver)
{
}
