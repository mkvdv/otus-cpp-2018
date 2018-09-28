/*!
\file
\brief File with declaration of observer pattern classes.
 Two pairs of publishers and subscribers: OnClickListener/OnClickable and
 OnCoordinateClickListener/OnCoordinateClickable.
 First pair is for simple buttons. The second pair is for canvas and complex buttons
 (where (x,y) coordinates of click matter)
*/

#pragma once

#include <list>

namespace otus {
	class OnClickable; // forward decl
	class OnClickListener {
	public:
		virtual void on_click(OnClickable* ptr) = 0;
		virtual ~OnClickListener() = default;
	};

	class OnClickable {
	public:
		void add_onclick_listener(OnClickListener* listener);
		void click();
		virtual ~OnClickable() = default;
	private:
		std::list<OnClickListener*> onclick_listeners_;
	};
} // namespace otus


namespace otus {
	class OnCoordinateClickable; // forward decl
	class OnCoordinateClickListener {
	public:
		virtual void on_click(OnCoordinateClickable* ptr, int x, int y) = 0;
		virtual ~OnCoordinateClickListener() = default;
	};

	class OnCoordinateClickable {
	public:
		void add_onclick_listener(OnCoordinateClickListener* listener);
		void click(int x, int y);
		virtual ~OnCoordinateClickable() = default;
	private:
		std::list<OnCoordinateClickListener*> on_coordinate_click_listeners_;
	};
} // namespace otus

