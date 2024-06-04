#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <limits>
#include <queue>
#include <string>

using namespace std;

struct pair_hash {
    template<class T1, class T2>
    size_t operator()(const pair<T1, T2>& p) const {
        auto hash1 = hash<T1>{}(p.first);
        auto hash2 = hash<T2>{}(p.second);
        return hash1 ^ (hash2 << 1); // Combining the hash values
    }
};

typedef unordered_map<size_t, unordered_set<pair<size_t, int>, pair_hash>> adj_list;

bool dag_checker_dfs(size_t source, adj_list& dag, unordered_set<size_t>& seen,  unordered_set<size_t>& on_path) {
    for (auto [destination, _] : dag[source]) {
        if (on_path.count(destination)) {
            return false;
        }
        if (!seen.count(destination)) {
            seen.insert(destination);
            on_path.insert(destination);
            if (!dag_checker_dfs(destination, dag, seen, on_path)) {
                return false;
            }
        }
    }
    on_path.erase(source);
    return true;
}

bool check_is_dag(adj_list& dag) {
    unordered_set<size_t> seen, on_path;
    for (auto& [source, _] : dag) {
        if (!seen.count(source)) {
            seen.insert(source);
            on_path.insert(source);
            if (!dag_checker_dfs(source, dag, seen, on_path)) {
                return false;
            }
        }
    }
    return true;
}

adj_list create_dag() {
    adj_list dag;

    size_t edges_count;
    cin >> edges_count;
    for (size_t i = 0; i < edges_count; i++) {
        size_t source, destination;
        int weight;
        cin >> source >> destination >> weight;
        dag[source].insert({ destination, weight });
    }

    return dag;
}

unordered_map<size_t, int> find_shortest_paths(adj_list& dag, size_t source) {
    unordered_map<size_t, int> distances;

    // Initialise distances to infinite
    for (auto& [node, _] : dag) {
        distances[node] = numeric_limits<int>::max();
    }

    // Set distance to source node to 0
    distances[source] = 0;

    queue<size_t> frontier;
    frontier.push(source);

    while (!frontier.empty()) {
        size_t u = frontier.front();
        frontier.pop();

        for (auto& [v, w] : dag[u]) {
            // Check violation of triangle inequality
            if (distances[v] > distances[u] + w) {
                distances[v] = distances[u] + w;
                frontier.push(v);
            }
        }
    }

    return distances;
}

void show_shortest_paths(size_t source, unordered_map<size_t, int>& distances) {
    cout << "Shortest distances from source " << source << ":" << endl;
    for (auto& [node, dist] : distances) {
        cout << node << ": " << (dist == numeric_limits<int>::max() ? "unreachable" : to_string(dist)) << endl;
    }
}

int main() {
    // Get DAG from standard input
    adj_list dag = create_dag();
    if (!check_is_dag(dag)) {
        cerr << "Not a DAG." << endl;
        exit(1);
    }

    // Identify source vertex
    size_t source;
    cin >> source;
    if (!dag.count(source)) {
        cerr << "Source not in DAG." << endl;
        exit(1);
    }

    // Calculate shortest paths from source
    unordered_map<size_t, int> distances = find_shortest_paths(dag, source);

    // Print results to standard output
    show_shortest_paths(source, distances);

    return 0;
}
