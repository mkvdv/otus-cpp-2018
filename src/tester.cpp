/*!
\file
\brief File with tester class for the whole programm.

This class is friend for many other classes, so it can test private field of his friends.
*/

#include "tester.h"

#include "model/model.h"
#include "view/view.h"
#include "controller/controller.h"

#include <iostream>
#include <cassert>

namespace otus {
	namespace {
		void print_nl()
		{
			std::cout << '\n';
		}
	} // anonymous namespace

	/**
	 * Simulate simple scenario of using editor.
	 */
	void TesterFriendClass::simple_test()
	{
		otus::StandardModel model;
		otus::StandardController controller(&model);
		otus::StandardView* view = dynamic_cast<otus::StandardView*>(controller.view_.get());
		assert(view);

		// open some file
		view->open_btn_.set_path("my_super_druper_file.ep");
		view->open_btn_.click();
		print_nl();

		// add dot
		view->dot_tool_btn_.click();
		view->canvas_.click(1, 1);
		print_nl();

		// add line
		view->line_tool_btn_.click();
		view->canvas_.click(2, 2);
		print_nl();

		// add rectangle
		view->rect_tool_btn_.click();
		view->canvas_.click(3, 3);
		print_nl();

		// save
		view->save_btn_.click();
		print_nl();

		// delete line
		view->deleter_tool_btn_.click();
		view->canvas_.click(2, 2);

		// save
		view->save_btn_.click();
		print_nl();
		// clear all
		view->clear_btn_.click();
		print_nl();
	}
} // namespace otus