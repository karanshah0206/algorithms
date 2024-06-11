// Identify single-source shortest paths in a simple graph or detect a negative-weight cycle.

#include <iostream>
#include <unordered_set>
#include <vector>
#include <limits>

using namespace std;

const int INF = numeric_limits<int>::max();

struct Vertex {
    uint id, pid;
};

struct Edge {
    uint source, dest;
    int weight;
};

void show_negative_weight_cycle(vector<Vertex>& vertices, uint id, unordered_set<uint>& seen, bool is_first = true) {
    Vertex& vertex = vertices[id];
    seen.insert(vertex.id);

    if (!seen.count(vertex.pid)) {
        show_negative_weight_cycle(vertices, vertex.pid, seen, false);
    }

    cout << vertex.id;
    if (is_first) {
        cout << endl;
    } else {
        cout << ", ";
    }
}

void show_single_source_shortest_paths(vector<int>& distances) {
    for (uint i = 0; i < distances.size(); i++) {
        cout << i << ": " << (distances[i] == INF ? "unreachable" : to_string(distances[i])) << endl;
    }
}

int main() {
    vector<Vertex> vertices;
    vector<Edge> edges;

    // Initialise vertices
    uint vertices_count;
    cin >> vertices_count;
    for (uint i = 0; i < vertices_count; i++) {
        vertices.push_back(Vertex { i, i });
    }

    // Initialise edges
    uint edges_count;
    cin >> edges_count;
    for (uint i = 0; i < edges_count; i++) {
        uint src, dest;
        int weight;
        cin >> src >> dest >> weight;

        // Check vertices are valid
        if (src >= vertices_count || dest >= vertices_count) {
            cout << "Input vertex not found in graph." << endl;
            return 1;
        }

        edges.push_back(Edge{ src, dest, weight });
    }

    // Get source vertex
    uint source;
    cin >> source;

    // Initialise known distances to infinity
    vector<int> distances(vertices_count, INF);

    // Set distance to source to 0
    distances[source] = 0;

    // Relax edges |V| - 1 times (|V| being count of vertices)
    for (uint i = 0; i < vertices_count - 1; i++) {
        for (Edge& edge : edges) {
            if (distances[edge.source] + edge.weight < distances[edge.dest]) {
                distances[edge.dest] = distances[edge.source] + edge.weight;
                vertices[edge.dest].pid = edge.source;
            }
        }
    }

    // Check if a negative-weight cycle exists using another round of relaxation
    uint witness_vertex_id = vertices_count; // `vertices_count` value indicates no witness found
    for (Edge& edge : edges) {
        if (distances[edge.source] + edge.weight < distances[edge.dest]) {
            vertices[edge.dest].pid = edge.source;
            witness_vertex_id = edge.dest;
            break;
        }
    }

    if (witness_vertex_id == vertices_count) {
        // No negative-edge cycle found, print single-source shortest paths
        // Just printing the distances in this example, but can print paths because we have all parents
        show_single_source_shortest_paths(distances);
    } else {
        // Negative-edge cycle found, print the cycle
        unordered_set<uint> seen;
        show_negative_weight_cycle(vertices, witness_vertex_id, seen);
    }

    return 0;
}
