/*!
\file
\brief File with interface of abstract model (from MVC).
*/

#pragma once

#include "../view/geometry_elements.h"
#include "../utils/observe.h"

#include <memory>

namespace otus {
	class ModelInterface: public Listenable {
	public:
		virtual ColorTag get_current_color() const = 0;
		virtual void choose_color(ColorTag color_tag) = 0;

		virtual ToolTag get_current_tool() const = 0;
		virtual void choose_tool(ToolTag tag) = 0;

		virtual void add_element(std::unique_ptr<GeometryElement> &&) = 0;
		virtual void delete_element_at(int x, int y) = 0;

		virtual void open(const std::string &newfile_path) = 0;
		virtual void clear() = 0;
		virtual void save() const = 0;

		virtual const std::list<std::unique_ptr<GeometryElement>> &get_elements() const = 0;
	};
} // namespace otus
