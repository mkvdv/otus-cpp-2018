#include "no_default_constructible.h"


namespace otus {
	NoDefaultConstructible::NoDefaultConstructible(int x)
		: x_(x) {}
	bool NoDefaultConstructible::operator<(const NoDefaultConstructible &) { return true; }
} // namespace otus
