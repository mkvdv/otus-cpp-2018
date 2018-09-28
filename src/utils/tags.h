/*!
\file
\brief File with declaration of some tool and color tags.
 Used as color and tool mode flag in model (M from MVC).
*/

#pragma once

namespace otus {
	// вот ты новый клас добавишь, а tag забудешь добавить - и что делать? ааа, баги, плохо
	enum ToolTag {
	  LINE, RECTANGLE, DOT, DELETER
	};

	enum ColorTag {
	  RED, GREEN, BLUE
	};
} // namespace otus
