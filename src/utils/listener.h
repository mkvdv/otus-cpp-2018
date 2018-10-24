#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include <utility>

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
		using listener_ptr_t = std::weak_ptr<Listener<Args...>>;

		void add_listener(listener_ptr_t observer) {
			listeners_.push_back(std::move(observer));
		}

		void remove_listener(listener_ptr_t observer) {
			if (auto sptr = observer.lock()) {
				auto weak_cmp = [&sptr](const listener_ptr_t &l) {
					return sptr == l.lock();
				};
				listeners_.erase(std::find_if(listeners_.begin(), listeners_.end(), weak_cmp));
			}
		}

		void notify_all(const Args &...args) {
			for (listener_ptr_t &listener_ptr: listeners_) {
				if (auto spt = listener_ptr.lock()) {
					spt->update(args...);
				}
			}
		}
		virtual ~Listenable() = default;
	private:
		std::vector<listener_ptr_t> listeners_;
	};

} // namespace otus

