/*!
\file
\brief File with controller (from MVC) declaration.
*/

#pragma once

#include "icontroller.h"
#include "../model/model.h"
#include "../view/view_elements.h"
#include "../view/iview.h"
#include "../utils/geomfactory.h"

namespace otus {
	/**
	 * Контроллер
	 * - его дергает view, когда на view нажимают
	 * - но он при этом создает view, и регистрирует себя как Observer-а view
	 * - он может менять модель, и после изменения модель просить view перерисоваться
	 */
	class StandardController : public ControllerInterface {
	public:
		explicit StandardController(ModelInterface* model);

		void open_file(const std::string& newfile_path) override;
		void save_file() override;
		void clear() override;
		void choose_tool(ToolTag tag) override;
		void choose_color(ColorTag tag) override;

		void on_click(OnClickable* ptr) override;
		void on_click(OnCoordinateClickable* ptr, int x, int y) override;

		// tester is your friend
		friend class TesterFriendClass;

	private:
		/**
		 * Реализации обработчиков для каждого типа кнопок
		 */
		void on_click_impl(ToolButton* btn);
		void on_click_impl(ColorButton* btn);
		void on_click_impl(SaveButton* btn);
		void on_click_impl(OpenButton* btn);
		void on_click_impl(ClearButton* btn);

	private:
		ModelInterface* model_;
		std::unique_ptr<ViewInterface> view_;
		std::unique_ptr<AbstractGeometryFactory> factory_;
	};

} // namespace otus

