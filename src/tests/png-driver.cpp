#include "png-driver.hpp"
#include <png.h>
#include <stdio.h>
#include <iostream>
#include <memory>

using namespace wbdl;

struct free_deleter
{
    void operator()(void* x) { free(x); }
};

struct fclose_deleter
{
	void operator()(FILE *fp) { if (fp != nullptr) fclose(fp); }
};

PNGDriver::PNGDriver(unsigned int sizeX, unsigned int sizeY, bool fbIsVertial, const std::string& filename) :
		m_filename(filename)
{
	m_parameters.sizeX = sizeX;
	m_parameters.sizeY = sizeY;
	m_parameters.fbOrientation =
			fbIsVertial ?
					FrameBufferOrientation::vertical :
					FrameBufferOrientation::horizontal;
	if (fbIsVertial)
	{
		m_horizontalFrameBuffer = new char[m_parameters.bufferSize()];
	}
}

PNGDriver::~PNGDriver()
{
	if (m_parameters.fbOrientation == FrameBufferOrientation::vertical && m_horizontalFrameBuffer != nullptr)
		delete[] m_horizontalFrameBuffer;
}

void PNGDriver::setFilename(const std::string& filename)
{
	m_filename = filename;
}

wbdl::ScreenParameters& PNGDriver::parameters()
{
	return m_parameters;
}

bool PNGDriver::updateScreen(const wbdl::FrameBuffer& frameBuffer)
{
	if (m_parameters.fbOrientation == FrameBufferOrientation::vertical)
	{
		// Moving data to horizontal copy
		for (unsigned int y=0; y<m_parameters.sizeY; y++) {
			for (unsigned int x=0; x<m_parameters.sizeX; x++) {
				char bit = ( frameBuffer.buffer[y / 8 + m_parameters.sizeY * x / 8] & (1 << (y % 8)) ) != 0;
				char * targetByte = &m_horizontalFrameBuffer[x / 8 + m_parameters.sizeX * y / 8];
				if (bit)
					*targetByte |= (1 << (x % 8));
				else
					*targetByte &= ~(1 << (x % 8));
			}
		}
	} else {
		m_horizontalFrameBuffer = frameBuffer.buffer;
	}
	std::unique_ptr<FILE, fclose_deleter> fp;
	std::unique_ptr<png_struct, free_deleter> png_ptr;
	std::unique_ptr<png_info, free_deleter> info_ptr;
	std::unique_ptr<png_byte, free_deleter> row;

	// Open file for writing (binary mode)
	fp.reset(fopen(m_filename.c_str(), "wb"));
	if (fp == NULL) {
		return false;
	}

	// Initialize write structure
	png_ptr.reset(png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL));
	if (png_ptr == NULL) {
		std::cerr << "Could not allocate write struct";
		return false;
	}

	// Initialize info structure
	info_ptr.reset(png_create_info_struct(png_ptr.get()));
	if (info_ptr == NULL) {
		std::cerr << "Could not allocate info struct";
		return false;
	}

	// Setup Exception handling
	if (setjmp(png_jmpbuf(png_ptr.get()))) {
		std::cerr << "Error during png creation";
		return false;
	}

	png_init_io(png_ptr.get(), fp.get());

	// Write header (8 bit colour depth)
	png_set_IHDR(png_ptr.get(), info_ptr.get(), m_parameters.sizeX, m_parameters.sizeY,
			8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
			PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

	// Set title
	png_text title_text;
	title_text.compression = PNG_TEXT_COMPRESSION_NONE;
	title_text.key = (png_charp) "Title";
	title_text.text = (png_charp) "wbdl test image";
	png_set_text(png_ptr.get(), info_ptr.get(), &title_text, 1);


	png_write_info(png_ptr.get(), info_ptr.get());

	// Allocate memory for one row (3 bytes per pixel - RGB)
	row.reset((png_bytep) malloc(3 * m_parameters.sizeX * sizeof(png_byte)));

	// Write image data
	unsigned int bytes_per_line = m_parameters.sizeX / 8;
	for (unsigned int y=0; y<m_parameters.sizeY; y++) {
		for (unsigned int x=0; x<m_parameters.sizeX; x++) {
			char bit = (m_horizontalFrameBuffer[x / 8 + bytes_per_line * y] & (1 << (x % 8))) != 0;
			row.get()[x*3] = 255*bit;
			row.get()[x*3 + 1] = 255*bit;
			row.get()[x*3 + 2] = 255*bit;
		}
		png_write_row(png_ptr.get(), row.get());
	}

	// End write
	png_write_end(png_ptr.get(), NULL);

	return true;
}


