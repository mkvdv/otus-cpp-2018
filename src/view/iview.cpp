/*!
\file
\brief File with implementation of interface of abstract view (V from MVC).
*/

#include "iview.h"

namespace otus {
	void ViewInterface::response()
	{
		redraw();
	}
} // namespace otus