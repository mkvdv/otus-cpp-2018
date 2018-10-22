/*!
\file
\brief File with declaration of model (from MVC).
*/

#pragma once

#include "imodel.h"

namespace otus {
	/**
	 * Model is a current state of editor.
	 * - стучит во View, если меняется
	 * - её могут читать View и Controller, если надо
	 * - Controller может её менять
	 */
	class StandardModel: public ModelInterface {
	public:
		/**
		 * Change current tool tag to passed tag.
		 * @param tag new tag
		 */
		void choose_tool(ToolTag tag) override;

		/**
		 * Change current color tag to passed tag.
		 * @param tag new tag
		 */
		void choose_color(ColorTag color_tag) override;

		void add_element(std::unique_ptr<GeometryElement> &&elem) override;

		/**
		 * Dymmy delete function. Remove last added element at this coordinates.
		 * @param x,y cordinates of center figure on canvas
		 */
		void delete_element_at(int x, int y) override;

		void open(const std::string &newfile_path) override;

		/**
		 * Clear model state.
		 */
		void clear() override;

		/**
		 * Save model to file.
		 */
		void save() const override;

		/**
		 * Getter for inner list of pointers to geometry_element.
		 * @return const refernce to private member - list of geometry elements
		 */
		const std::list<std::unique_ptr<GeometryElement>> &get_elements() const override;

		ToolTag get_current_tool() const override;

		ColorTag get_current_color() const override;

		// tester is your friend
		friend class TesterFriendClass;

	private:
		std::string current_image_path_;
		int current_tool_tag_ = ToolTag::DOT;
		int current_color_tag_ = ColorTag::RED;

		// store elements, drawed on canvas
		// shared_ptr, cause we can show it
		std::list<std::unique_ptr<GeometryElement>> drawed_elements_;

		utils::Logger logger_{"Model"};
	};
} // namespace otus
