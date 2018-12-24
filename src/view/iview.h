/*!
\file
\brief File with interface of abstract view (V from MVC).
*/

#pragma once

#include "../utils/observe.h"

namespace otus {
	class ViewInterface {
	public:
		virtual void redraw() = 0;
      virtual ~ViewInterface() = default;
	};
} // namespace otus