/*!
\file
\brief File with implementation of view (V from MVC).
*/

#include "view.h"

namespace otus {
	StandardView::StandardView(ModelInterface* model, ControllerInterface* controller)
			:model_(model)
	{
		model_->add_observer(this);

		open_btn_.add_onclick_listener(controller);
		save_btn_.add_onclick_listener(controller);
		clear_btn_.add_onclick_listener(controller);

		line_tool_btn_.add_onclick_listener(controller);
		rect_tool_btn_.add_onclick_listener(controller);
		dot_tool_btn_.add_onclick_listener(controller);
		deleter_tool_btn_.add_onclick_listener(controller);

		red_color_btn_.add_onclick_listener(controller);
		blue_color_btn_.add_onclick_listener(controller);
		green_color_btn_.add_onclick_listener(controller);

		canvas_.add_onclick_listener(controller);
	}

	void StandardView::redraw()
	{
		/**
		 * Here must be some work clear the scren before drawing.
		 */

		for (const auto& geom_elem : model_->get_elements()) {
			geom_elem->draw();
		}
		logger_.info("view redrawed");
	}
} // namespace otus
