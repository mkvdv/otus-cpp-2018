/*!
\file
\brief File with implementation of observer pattern classes.
*/

#include "onclick.h"

namespace otus {
	void OnClickable::add_onclick_listener(OnClickListener* listener)
	{
		onclick_listeners_.push_back(listener);
	}
	void OnClickable::click()
	{
		for (OnClickListener* ptr: onclick_listeners_) {
			ptr->on_click(this);
		}
	}
} // namespace otus

namespace otus {
	void OnCoordinateClickable::add_onclick_listener(OnCoordinateClickListener* listener)
	{
		on_coordinate_click_listeners_.push_back(listener);
	}
	void OnCoordinateClickable::click(int x, int y)
	{
		for (OnCoordinateClickListener* ptr: on_coordinate_click_listeners_) {
			ptr->on_click(this, x, y);
		}
	}
} // namespace otus

