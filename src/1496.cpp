#include <iostream>
#include <string>
#include <map>

// http://acm.timus.ru/problem.aspx?num=1496
static void run() {
	using namespace std;

	size_t submits_number;
	cin >> submits_number;

	std::string tmp;
	std::map<std::string, size_t> dict;

	for (size_t i = 0; i != submits_number; ++i) {
		cin >> tmp;
		auto it = dict.find(tmp);
		if (it == dict.end()) {
			dict[tmp] = 1;
		} else {
			it->second += 1;
		}
	}

	// print output if it is a spammer team
	for (auto &p: dict) {
		if (p.second > 1) {
			cout << p.first << '\n';
		}
	}

}

int main() {
	using namespace std;
	ios::sync_with_stdio(false);

	run();
}
