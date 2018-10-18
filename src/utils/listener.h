#pragma once

#include <vector>

namespace otus {
	template<class ...Args>
	class Listener {
	public:
		virtual void update(const Args &...) = 0;
		virtual ~Listener<Args...>() = default;
	};

	template<class ...Args>
	class Listenable {
	public:
		void add_listener(Listener<Args...> *observer) {
			listeners_.push_back(observer);
		}

		void notify_all(const Args &...args) {
			for (Listener<Args...> *listener : listeners_) {
				listener->update(args...);
			}
		}
		virtual ~Listenable() = default;
	private:
		std::vector<Listener<Args...> *> listeners_;
	};

} // namespace otus

