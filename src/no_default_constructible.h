#pragma once

namespace otus {
	struct NoDefaultConstructible {
		NoDefaultConstructible() = delete;

		explicit NoDefaultConstructible(int x);

		bool operator<(const NoDefaultConstructible &);

	private:
		int x_;
	};

} // namespace otus
