/*!
\file
\brief File with implementation of geometry factory class.
*/


#include "geomfactory.h"

namespace otus {
	std::unique_ptr<Line> otus::StandardGeometryFactory::createLine(int x0, int y0, int x1, int y1,
			ColorTag color)
	{
		logger.info("Will create line");
		return std::make_unique<Line>(x0, y0, x1, y1, color);
	}

	std::unique_ptr<Rectangle> otus::StandardGeometryFactory::createRectangle(int x, int y,
			int width, int height, float angle, ColorTag color)
	{
		logger.info("Will create rectangle");
		return std::make_unique<Rectangle>(x, y, width, height, angle, color);
	}

	std::unique_ptr<Dot> otus::StandardGeometryFactory::createDot(int x, int y, ColorTag color)
	{
		logger.info("Will create dot");
		return std::make_unique<Dot>(x, y, color);
	}

} // namespace otus
