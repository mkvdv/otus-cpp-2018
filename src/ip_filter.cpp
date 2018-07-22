#include "filters.h"

#include <cassert>
#include <fstream>

// #define DEBUG

const int ERR_CODE = -1;

int main()
{
	using namespace std;

	try {
		vector<ip_t> ip_pool;

		for (string line; getline(cin, line);) {
			assert(line != "\n" && line != "\t" && !line.empty());

			vector<string> v = split<string>(line, '\t', identity());
			ip_pool.push_back(split<byte>(v.at(0), '.', [](const string &s) -> byte {
			  return static_cast<byte>((stoul(s)));
			}));
		}

		// reverse lexicographically sort
		sort(ip_pool.begin(),
			 ip_pool.end(),
			 lexicographical_ip_comparator<std::greater<>>());
#ifdef DEBUG
		cout << "######## reverse lexicographically sort" << endl;
#endif
		print(ip_pool);

		// filter by first byte and output
		auto ips1 = filter(static_cast<byte>(1), ip_pool);
#ifdef DEBUG
		cout << "######## filter(1)" << endl;
#endif
		print(ips1);

		// filter by first and second bytes and output
		auto ips2 = filter(static_cast<const byte>(46), static_cast<const byte>(70), ip_pool);
#ifdef DEBUG
		cout << "######## filter(46, 70)" << endl;
#endif
		print(ips2);

		// filter by any byte and output
		auto ips_any = filter_any(static_cast<const byte>(46), ip_pool);
#ifdef DEBUG
		cout << "######## filter_any(46)" << endl;
#endif
		print(ips_any);

	}
	catch (const exception &e) {
		cerr << e.what() << endl;
		return ERR_CODE;
	}

	return 0;
}
