/*!
\file
\brief File with interface of abstract view (V from MVC).
*/

#pragma once

#include "../utils/observe.h"

namespace otus {
	class ViewInterface : public Observer {
	public:
		virtual void redraw() = 0;
		void response() override;
	};
} // namespace otus