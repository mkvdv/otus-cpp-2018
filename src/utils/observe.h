/*!
\file
\brief File with declaration of observer pattern - Observer and Observable.
*/

#pragma once

#include <list>
#include <functional>
#include "onclick.h"

namespace otus {
	/**
	 * Adapter for BaseOnClickable
	 * just give u other names for methods
	 * @tparam Args
	 */
	template<class ... Args>
	class BaseListenable {
	public:
		void add_listener(std::function<void(const Args &...)> &&observer) {
			on_clickable_.add_onclick_listener(std::move(observer));
		}
		void notify_update(const Args &...args) {
			on_clickable_.click(args...);
		}
		virtual ~BaseListenable() = default;

	private:
		BaseOnClickable<Args...> on_clickable_;
	};

	using Listenable = BaseListenable<>;
} // namespace otus
