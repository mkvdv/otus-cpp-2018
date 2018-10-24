#include <iostream>
#include <string>
#include <vector>
#include <map>

// http://acm.timus.ru/problem.aspx?num=1545
static void run() {
	using namespace std;

	size_t dict_size;
	cin >> dict_size;

	std::string tmp;
	std::map<char, std::vector<std::string >> dict;

	for (size_t i = 0; i != dict_size; ++i) {
		cin >> tmp;
		auto it = dict.find(tmp[0]);
		if (it != dict.end()) {
			it->second.push_back(std::move(tmp));
		} else {
			dict[tmp[0]] = std::vector<std::string>{{tmp}};
		}
	}

	char key;
	cin >> key;

	// print output if it is
	auto it = dict.find(key);
	if (it != dict.end()) {
		for (std::string &s : it->second) {
			std::cout << s << '\n';
		}
	}

}

int main() {
	using namespace std;
	ios::sync_with_stdio(false);

	run();
}
