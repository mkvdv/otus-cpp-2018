/*!
\file
\brief File with controller (from MVC) implementation.
*/

#include "controller.h"
#include "../view/view.h"

#include <cassert>

namespace otus {

	StandardController::StandardController(std::shared_ptr<ModelInterface> model)
		: model_(model), factory_(std::make_unique<StandardGeometryFactory>()) {
		assert(model);
	}

	void StandardController::open_file(const std::string &newfile_path) {
		model_->open(newfile_path);
	}

	void StandardController::save_file() {
		model_->save();
	}

	void StandardController::clear() {
		model_->clear();
	}

	void StandardController::choose_tool(ToolTag tag) {
		model_->choose_tool(tag);
	}

	void StandardController::choose_color(ColorTag tag) {
		model_->choose_color(tag);
	}

	void StandardController::react_canvas_click(int x, int y) {
		// some default constants, default behaviour is creating fixed figures
		const int SHIFT = 10;
		const float DEFAULT_ANGLE = 0.0;

		switch (model_->get_current_tool()) {
			case ToolTag::LINE: model_->add_element(factory_->createLine(x,
			                                                             y,
			                                                             x + SHIFT,
			                                                             y + SHIFT,
			                                                             model_
				                                                             ->get_current_color()));
				break;

			case ToolTag::RECTANGLE:
				model_->add_element(factory_->createRectangle(x,
				                                              y,
				                                              x + SHIFT,
				                                              x + SHIFT,
				                                              DEFAULT_ANGLE,
				                                              model_->get_current_color()));
				break;

			case ToolTag::DOT: model_
					->add_element(factory_->createDot(x, y, model_->get_current_color()));
				break;

			case ToolTag::DELETER: model_->delete_element_at(x, y);
				break;

			default: throw std::runtime_error("Other tools unsupported now");
		}
	}

} // namespace otus
