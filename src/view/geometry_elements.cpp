/*!
\file
\brief File with implementation of geometry element classes.
*/

#include "geometry_elements.h"

namespace otus {
	GeometryElement::GeometryElement(ColorTag tag)
			:tag_(tag) { }

	Line::Line(int x0, int y0, int x1, int y1, ColorTag color)
			:GeometryElement(color), x0_(x0), y0_(y0), x1_(x1), y1_(y1)
	{
		logger_.info("line created");
	}
	void Line::draw()
	{
		/**
		 * Some specific drawing work. Need canvas !
		 */
		logger_.info("Line drawed");

	}
	int Line::getX()
	{
		return x0_;
	}
	int Line::getY()
	{
		return y0_;
	}

	Rectangle::Rectangle(int x, int y, int width, int height, float angle, ColorTag color)
			:GeometryElement(color), x_(x), y_(y), width_(width), height_(height), angle_(angle)
	{
		logger_.info("rectangle created");
	}
	void Rectangle::draw()
	{
		/**
		 * Some specific drawing work. Need canvas !
		 */
		logger_.info("Rectangle drawed");

	}
	int Rectangle::getX()
	{
		return x_;
	}
	int Rectangle::getY()
	{
		return y_;
	}

	Dot::Dot(int x, int y, ColorTag color)
			:GeometryElement(color), x_(x), y_(y)
	{
		logger_.info("dot created");
	}
	void Dot::draw()
	{
		/**
		 * Some specific drawing work. Need canvas !
		 */
		logger_.info("Dot drawed");
	}
	int Dot::getX()
	{
		return x_;
	}
	int Dot::getY()
	{
		return y_;
	}

} // namespace otus
