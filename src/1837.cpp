#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <algorithm>

static const char *ISENBAEV = "Isenbaev";

static const char *UNDEFINED = "undefined";

using graph_t = std::unordered_map<std::string, std::vector<std::string>>;
using iter_t = graph_t::iterator;

static void add_link(graph_t &graph, const std::string &v1, const std::string &v2) {
	auto it = graph.find(v1);

	if (it != graph.end()) {
		it->second.push_back(v2);
	} else {
		graph[v1] = std::vector<std::string>{{v2}};
	}
}

static void print_results(const std::unordered_map<std::string, int64_t> &isenbaev_number) {
	// sort this map by first key and print -- use vector
	std::vector<std::pair<std::string, int64_t >> vec(isenbaev_number.begin(), isenbaev_number.end());
	std::sort(vec.begin(), vec.end());

	for (auto &p: vec) {
		if (p.second != INT64_MAX) {
			std::cout << p.first << ' ' << p.second << '\n';
		} else {
			std::cout << p.first << ' ' << UNDEFINED << '\n';
		}
	}
}

/**
 * BFS
 */
static void bfs(std::unordered_map<std::string, std::vector<std::string>> &adj,
                std::unordered_map<std::string, std::vector<std::string>>::iterator start,
                std::unordered_map<std::string, int64_t> &distance) {

	distance[start->first] = 0;

	std::queue<std::string> queue;
	queue.push(start->first);

	while (!queue.empty()) {
		std::string u = std::move(queue.front());
		queue.pop();

		for (const std::string &w: adj[u]) {
			if (distance[w] == INT64_MAX) {
				distance[w] = distance[u] + 1;
				queue.push(w);
			}
		}
	}

}

// http://acm.timus.ru/problem.aspx?num=1837
// Simple DFS
static void run() {
	using namespace std;

	size_t number_of_teams;
	cin >> number_of_teams;

	if (!number_of_teams) {
		return;
	}

	std::unordered_map<std::string, int64_t> isenbaev_number;
	std::unordered_map<std::string, bool> is_visited;

	std::unordered_map<std::string, std::vector<std::string>> adj;

	std::string gamer1, gamer2, gamer3;
	for (size_t i = 0; i != number_of_teams; ++i) {
		cin >> gamer1 >> gamer2 >> gamer3;
		add_link(adj, gamer1, gamer2);
		add_link(adj, gamer1, gamer3);

		add_link(adj, gamer2, gamer1);
		add_link(adj, gamer2, gamer3);

		add_link(adj, gamer3, gamer1);
		add_link(adj, gamer3, gamer2);
	}

	isenbaev_number.reserve(adj.size());
	for (auto &p: adj) {
		isenbaev_number[p.first] = INT64_MAX;
	}

	auto isinbaev_it = adj.find(ISENBAEV);
	if (isinbaev_it == adj.end()) {
		print_results(isenbaev_number);
		return;
	}

	// fill isinbaev_number map -- traverse graph in DFS
	bfs(adj, isinbaev_it, isenbaev_number);

	print_results(isenbaev_number);
}

int main() {
	using namespace std;
	ios::sync_with_stdio(false);

	run();
}
