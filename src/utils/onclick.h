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
#include <functional>

namespace otus {
	template<class ...Args>
	class BaseOnClickable {
	public:
		void add_onclick_listener(std::function<void(const Args &...)> listener);
		void click(const Args &...);

		virtual ~BaseOnClickable<Args...>() = default;
	private:
		std::list<std::function<void(const Args &...)>> onclick_listeners_;
	};

	template<class... Args>
	void BaseOnClickable<Args...>::add_onclick_listener(std::function<void(const Args &...)> listener) {
		onclick_listeners_.push_back(std::move(listener));
	}

	template<class... Args>
	void BaseOnClickable<Args...>::click(const Args &...args) {
		for (auto &listener: onclick_listeners_) {
			listener(args...);
		}
	}

	/**
	 * Aliases for classes.
	 */
	using OnClickable = BaseOnClickable<>;
	using OnCoordinateClickable = BaseOnClickable<int, int>;

} // namespace otus



