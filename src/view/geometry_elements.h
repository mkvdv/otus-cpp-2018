/*!
\file
\brief File with declaration of geometry element classes.
*/

#pragma once

#include "../utils/logger.h"
#include "../utils/tags.h"

namespace otus {
	class GeometryElement {
	public:
		explicit GeometryElement(ColorTag tag);
		virtual void draw() = 0;
		virtual int getX() = 0;
		virtual int getY() = 0;

		virtual ~GeometryElement() = default;

	private:
		ColorTag tag_;
	};

	class Line : public GeometryElement {
	public:
		Line(int x0, int y0, int x1, int y1, ColorTag color);
		void draw() override;
		int getX() override;
		int getY() override;

	private:
		int x0_, y0_;
		int x1_, y1_;
		utils::Logger logger_{"Line"};
	};

	class Rectangle : public GeometryElement {
	public:
		Rectangle(int x, int y, int width, int height, float angle, ColorTag color);
		void draw() override;
		int getX() override;
		int getY() override;

	private:
		int x_, y_;
		int width_;
		int height_;
		float angle_;
		utils::Logger logger_{"Rectangle"};
	};

	class Dot : public GeometryElement {
	public:
		Dot(int x, int y, ColorTag color);
		void draw() override;
		int getX() override;
		int getY() override;

	private:
		int x_, y_;
		utils::Logger logger_{"Dot"};
	};

} // namespace otus
