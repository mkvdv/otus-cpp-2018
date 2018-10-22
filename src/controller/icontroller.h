/*!
\file
\brief File with interface of abstract controller (from MVC).
*/

#pragma once

#include "../view/geometry_elements.h"
#include "../utils/onclick.h"

#include <string>

namespace otus {

	/**
	 * todo inherit fields from 2 classes - oh no, dangerous, fix this shit ?
	 */
	class ControllerInterface {
	public:
		virtual void open_file(const std::string &newfile_path) = 0;
		virtual void save_file() = 0;
		virtual void clear() = 0;

		virtual void choose_tool(ToolTag tag) = 0;
		virtual void choose_color(ColorTag tag) = 0;

		virtual void react_canvas_click(int x, int y) = 0;

		virtual ~ControllerInterface() = default;
	};

} // namespace otus
