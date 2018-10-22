/*!
\file
\brief File with interface of abstract geometry factory and declaration of standard geometry
 factory.
*/

#pragma once

#include "../view/geometry_elements.h"
#include "../utils/logger.h"

#include <memory>

namespace otus {
	class AbstractGeometryFactory {
	public:
		virtual std::unique_ptr<Line> createLine(int x0, int y0, int x1, int y1,
				ColorTag color) = 0;
		virtual std::unique_ptr<Rectangle> createRectangle(int x, int y, int width, int height,
				float angle, ColorTag color) = 0;
		virtual std::unique_ptr<Dot> createDot(int x, int y, ColorTag color) = 0;
		virtual ~AbstractGeometryFactory() = default;
	};

	class StandardGeometryFactory : public AbstractGeometryFactory {
		std::unique_ptr<Line> createLine(int x0, int y0, int x1, int y1, ColorTag color) override;
		std::unique_ptr<Rectangle> createRectangle(int x, int y, int width, int height, float
		angle, ColorTag color) override;
		std::unique_ptr<Dot> createDot(int x, int y, ColorTag color) override;

	private:
		utils::Logger logger{"Geometry Factory"};
	};

} // namespace otus
