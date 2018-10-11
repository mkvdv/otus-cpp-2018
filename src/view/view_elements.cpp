/*!
\file
\brief File with implementation of button and canvas classes.
*/

#include "view_elements.h"

namespace otus {

	ToolButton::ToolButton(ToolTag tag)
			:tag_(tag) { }
	ToolTag ToolButton::get_tag() const
	{
		return tag_;
	}

	ColorButton::ColorButton(ColorTag tag)
			:tag_(tag) { }
	ColorTag ColorButton::get_tag() const
	{
		return tag_;
	}

	OpenButton::OpenButton(std::string s)
			:path_(std::move(s)) { }
	std::string OpenButton::get_path() const
	{
		return path_;
	}
	void OpenButton::set_path(const std::string& s)
	{
		path_ = s;
	}
} // namespace otus
