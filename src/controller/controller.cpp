/*!
\file
\brief File with controller (from MVC) implementation.
*/

#include "controller.h"
#include "../view/view.h"

#include <cassert>

namespace otus {
	StandardController::StandardController(ModelInterface* model)
			:model_(model), view_(std::make_unique<StandardView>(model, this)),
			 factory_(std::make_unique<StandardGeometryFactory>())
	{
		assert(model);
	}

	void StandardController::open_file(const std::string& newfile_path)
	{
		model_->open(newfile_path);
	}

	void StandardController::save_file()
	{
		model_->save();
	}

	void StandardController::clear()
	{
		model_->clear();
	}

	void StandardController::choose_tool(ToolTag tag)
	{
		model_->choose_tool(tag);
	}

	void StandardController::choose_color(ColorTag tag)
	{
		model_->choose_color(tag);
	}

	void StandardController::on_click(OnClickable* ptr)
	{
		assert(ptr);

		// oh no...
		if (dynamic_cast<ToolButton*>(ptr)) {
			on_click_impl(dynamic_cast<ToolButton*>(ptr));
		}
		else if (dynamic_cast<ColorButton*>(ptr)) {
			on_click_impl(dynamic_cast<ColorButton*>(ptr));
		}
		else if (dynamic_cast<OpenButton*>(ptr)) {
			on_click_impl(dynamic_cast<OpenButton*>(ptr));
		}
		else if (dynamic_cast<SaveButton*>(ptr)) {
			on_click_impl(dynamic_cast<SaveButton*>(ptr));
		}
		else if (dynamic_cast<ClearButton*>(ptr)) {
			on_click_impl(dynamic_cast<ClearButton*>(ptr));
		}
		else {
			throw std::runtime_error("Invalid button class in handler method");
		}
	}

	void StandardController::on_click_impl(ToolButton* btn)
	{
		choose_tool(btn->get_tag());
	}
	void StandardController::on_click_impl(ColorButton* btn)
	{
		choose_color(btn->get_tag());
	}

	void StandardController::on_click_impl(OpenButton* btn)
	{
		open_file(btn->get_path());
	}
	void StandardController::on_click_impl([[maybe_unused]]SaveButton* btn)
	{
		save_file();
	}
	void StandardController::on_click_impl([[maybe_unused]]ClearButton* btn)
	{
		clear();
	}

	void StandardController::on_click([[maybe_unused]] OnCoordinateClickable* ptr, int x, int y)
	{
		assert(ptr);

		// some default constants, default behaviour is creating fixed figures
		const int SHIFT = 10;
		const float DEFAULT_ANGLE = 0.0;

		switch (model_->get_current_tool()) {
		case ToolTag::LINE:
			model_->add_element(factory_->createLine(x, y, x+SHIFT, y+SHIFT,
					model_->get_current_color()));
			break;

		case ToolTag::RECTANGLE:
			model_->add_element(factory_->createRectangle(x, y, x+SHIFT, x+SHIFT, DEFAULT_ANGLE,
					model_->get_current_color()));
			break;

		case ToolTag::DOT:
			model_->add_element(factory_->createDot(x, y,
					model_->get_current_color()));
			break;

		case ToolTag::DELETER: model_->delete_element_at(x, y);
			break;

		default: throw std::runtime_error("Other tools unsupported now");
		}
	}

} // namespace otus
