/*!
\file
\brief File with declaration of view (V from MVC).
*/

#pragma once

#include "iview.h"
#include "view_elements.h"
#include "../model/imodel.h"
#include "../controller/icontroller.h"

namespace otus {
	/**
	 * Представление:
	 * - набор кнопок, окошек и надписей
	 * - получет в конструкторе ребят, на которых подписывает клики своего содержимого.
	 * - умеет перерисовываться полностью
	 */
	class StandardView: public ViewInterface {
	public:
		StandardView(std::shared_ptr<ModelInterface> model,
		             std::shared_ptr<ControllerInterface> controller);
		void redraw() override;

		// tester is your friend
		friend class TesterFriendClass;

	private:
		std::shared_ptr<ModelInterface> model_;

		// view elements
		OpenButton open_btn_{""};
		SaveButton save_btn_;
		ClearButton clear_btn_;

		ToolButton line_tool_btn_ = ToolButton(ToolTag::LINE);
		ToolButton rect_tool_btn_ = ToolButton(ToolTag::RECTANGLE);
		ToolButton dot_tool_btn_ = ToolButton(ToolTag::DOT);
		ToolButton deleter_tool_btn_ = ToolButton(ToolTag::DELETER);

		ColorButton red_color_btn_ = ColorButton(ColorTag::RED);
		ColorButton blue_color_btn_ = ColorButton(ColorTag::BLUE);
		ColorButton green_color_btn_ = ColorButton(ColorTag::GREEN);

		Canvas canvas_;

		utils::Logger logger_{"View"};
	};

} // namespace otus
