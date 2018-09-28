/*!
\file
\brief File with implementation of model (from MVC).
*/

#include "model.h"

namespace otus {
	void StandardModel::choose_tool(ToolTag tag)
	{
		current_tool_tag_ = tag;
		logger_.info("tool changed");
	}
	void StandardModel::choose_color(ColorTag color_tag)
	{
		current_tool_tag_ = color_tag;
		logger_.info("color changed");

	}
	void StandardModel::add_element(std::unique_ptr<GeometryElement>&& elem)
	{
		drawed_elements_.push_back(std::move(elem));
		logger_.info("element added");
		notify_update(); // view will redraw canvas
	}

	void StandardModel::delete_element_at(int x, int y)
	{
		// delete one LAST added element with (x, y) coordinates
		for (auto it = drawed_elements_.rbegin(); it!=drawed_elements_.rend(); ++it) {
			if ((*it)->getX()==x && (*it)->getY()==y) {
				drawed_elements_.erase(it.base());

				logger_.info("element deleted");
				notify_update();
				break;
			}
		}
	}

	void StandardModel::open(const std::string& newfile_path)
	{
		clear();
		current_image_path_ = newfile_path;

		if (current_image_path_.empty()) {
			throw std::runtime_error("New file path is empty!");
		}

		// load data from file
		// ...
		logger_.info("file opened");
		notify_update(); // view will redraw
	}

	void StandardModel::clear()
	{
		current_tool_tag_ = ToolTag::DOT;
		current_color_tag_ = ColorTag::RED;
		current_image_path_.clear();
		drawed_elements_.clear();
		logger_.info("cleared");
		notify_update();
	}
	void StandardModel::save() const
	{
		if (!current_image_path_.empty()) {
			/**
			 * Save all model state to filepath
			 */
			logger_.info("saved");
		}
		else {
			throw std::runtime_error("Can't save, path is empty");
		}
	}
	const std::list<std::unique_ptr<GeometryElement>>& StandardModel::get_elements() const
	{
		return drawed_elements_;
	}
	ToolTag StandardModel::get_current_tool() const
	{
		return static_cast<ToolTag>(current_tool_tag_);
	}
	ColorTag StandardModel::get_current_color() const
	{
		return static_cast<ColorTag>(current_color_tag_);
	}
} // namespace otus
