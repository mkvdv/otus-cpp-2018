/*!
\file
\brief File with declaration of observer pattern - Observer and Observable.
*/

#pragma once

#include <list>

namespace otus {
	class Observer {
	public:
		virtual void response() = 0; // отреагировать
		virtual ~Observer() = default;
	};


	class Observable {
	public:
		void add_observer(Observer* observer);
		void notify_update();
	private:
		std::list<Observer*> observers_;
	};
} // namespace otus
