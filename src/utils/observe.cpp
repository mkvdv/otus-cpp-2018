/*!
\file
\brief File with implementation of Observable class.
*/

#include "observe.h"
#include <cassert>

namespace otus {
	void otus::Observable::add_observer(Observer* observer)
	{
		assert(observer);
		observers_.push_back(observer); // добавить наблюдателя, которого будем потом звать
	}
	void otus::Observable::notify_update()
	{
		for (Observer* observer : observers_) {
			observer->response(); // вызвать реакцию наблюдателя
		}
	}
} // namespace otus
