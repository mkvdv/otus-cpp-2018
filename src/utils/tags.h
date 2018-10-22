/*!
\file
\brief File with declaration of some tool and color tags.
 Used as color and tool mode flag in model (M from MVC).
*/

#pragma once

namespace otus {
	enum ToolTag {
	  LINE, RECTANGLE, DOT, DELETER
	};

	enum ColorTag {
	  RED, GREEN, BLUE
	};
} // namespace otus
