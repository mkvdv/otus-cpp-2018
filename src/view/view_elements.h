/*!
\file
\brief File with declaration of button and canvas classes.
*/


#pragma once

#include "../utils/onclick.h"
#include "geometry_elements.h"

#include <string>

namespace otus {
	class ToolButton: public OnClickable {
	public:
		explicit ToolButton(ToolTag tag);
		ToolTag get_tag() const;
	private:
		const ToolTag tag_;
	};

	class ColorButton: public OnClickable {
	public:
		explicit ColorButton(ColorTag tag);
		ColorTag get_tag() const;
	private:
		const ColorTag tag_;
	};

	class OpenButton: public OnClickable {
	public:
		explicit OpenButton(std::string s);
		std::string get_path() const;
		void set_path(const std::string &s);
	private:
		std::string path_;
	};

	class SaveButton: public OnClickable {};

	class ClearButton: public OnClickable {};

	class Canvas: public OnCoordinateClickable {};

} // namespace otus::utils
