#include <utility>

/*!
\file
\brief File with implementation of view (V from MVC).
*/

#include "view.h"

namespace otus {
	StandardView::StandardView(std::shared_ptr<ModelInterface> model,
	                           std::shared_ptr<ControllerInterface> controller)
		: model_(std::move(model)) {

		model_->add_listener([this]() { this->redraw(); });

		open_btn_.add_onclick_listener([controller, this]() {
			controller->open_file(this->open_btn_.get_path());
		});

		save_btn_.add_onclick_listener([controller]() { controller->save_file(); });
		clear_btn_.add_onclick_listener([controller]() { controller->clear(); });

		// template "partially applied" lambda
		auto tool_tag_setter_generator = [controller](ToolButton &button) {
			return [controller, &button]() { controller->choose_tool(button.get_tag()); };
		};

		line_tool_btn_.add_onclick_listener(tool_tag_setter_generator(line_tool_btn_));
		rect_tool_btn_.add_onclick_listener(tool_tag_setter_generator(rect_tool_btn_));
		dot_tool_btn_.add_onclick_listener(tool_tag_setter_generator(dot_tool_btn_));
		deleter_tool_btn_.add_onclick_listener(tool_tag_setter_generator(deleter_tool_btn_));


		// template "partially applied" lambda
		auto color_tag_setter_generator = [controller](ColorButton &button) {
			return [controller, &button]() { controller->choose_color(button.get_tag()); };
		};

		red_color_btn_.add_onclick_listener(color_tag_setter_generator(red_color_btn_));
		blue_color_btn_.add_onclick_listener(color_tag_setter_generator(blue_color_btn_));
		green_color_btn_.add_onclick_listener(color_tag_setter_generator(green_color_btn_));

		auto canvas_listener = [controller](int x, int y) {
			controller->react_canvas_click(x, y);
		};
		canvas_.add_onclick_listener(canvas_listener);
	}

	void StandardView::redraw() {
		/**
		 * Here must be some work clear the scren before drawing.
		 */

		for (const auto &geom_elem : model_->get_elements()) {
			geom_elem->draw();
		}
		logger_.info("view redrawed");
	}

} // namespace otus
