#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

// http://acm.timus.ru/problem.aspx?num=1196
static void run() {
	using namespace std;

	size_t n, m;

	cin >> n;
	std::vector<uint64_t> first;
	first.reserve(n);

	for (size_t i = 0; i != n; ++i) {
		uint64_t tmp;
		cin >> tmp;
		first.push_back(tmp);
	}

	cin >> m;
	std::vector<uint64_t> second;
	second.reserve(m);

	for (size_t i = 0; i != m; ++i) {
		uint64_t tmp;
		cin >> tmp;
		second.push_back(tmp);
	}

	std::sort(first.begin(), first.end());
	std::sort(second.begin(), second.end());

	size_t eq_counter = 0;
	auto first_it = first.begin();
	auto second_it = second.begin();

	// find number of eq elems in both sorted vectors
	while (first_it != first.end() && second_it != second.end()) {
		if (*first_it == *second_it) {
			// only number of elems in second, that alse exist in third
			while (second_it != second.end() && *first_it == *second_it) {
				++eq_counter;
				++second_it;
			}
		} else if (*first_it < *second_it) {
			++first_it;
		} else {
			++second_it;
		}
	}

	cout << eq_counter;

}

int main() {
	using namespace std;
	ios::sync_with_stdio(false);

	run();
}
