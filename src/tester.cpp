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

namespace otus {
	namespace {
		void print_nl() {
			std::cout << '\n';
		}
	} // anonymous namespace

	/**
	 * Simulate simple scenario of using editor.
	 */
	void TesterFriendClass::simple_test() {
		using namespace std;

		shared_ptr<otus::ModelInterface> model_ptr = make_shared<otus::StandardModel>();
		shared_ptr<otus::ControllerInterface>
			controller_ptr = make_shared<otus::StandardController>(model_ptr);
		shared_ptr<otus::StandardView>
			view_ptr = make_shared<otus::StandardView>(model_ptr, controller_ptr);


		// open some file
		view_ptr->open_btn_.set_path("my_super_druper_file.ep");
		view_ptr->open_btn_.click();
		print_nl();

		// add dot
		view_ptr->dot_tool_btn_.click();
		view_ptr->canvas_.click(1, 1);
		print_nl();

		// add line
		view_ptr->line_tool_btn_.click();
		view_ptr->canvas_.click(2, 2);
		print_nl();

		// add rectangle
		view_ptr->rect_tool_btn_.click();
		view_ptr->canvas_.click(3, 3);
		print_nl();

		// save
		view_ptr->save_btn_.click();
		print_nl();

		// delete line
		view_ptr->deleter_tool_btn_.click();
		view_ptr->canvas_.click(2, 2);

		// save
		view_ptr->save_btn_.click();
		print_nl();
		// clear all
		view_ptr->clear_btn_.click();
		print_nl();
	}
} // namespace otus
