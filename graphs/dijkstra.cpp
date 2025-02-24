// Find single-source shortest paths to all vertices in directional graph with only non-negative edges.

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <limits>

using namespace std;

uint INF = numeric_limits<uint>::max();

struct Vertex {
    bool explored;
    uint id, parent_id, dist;
    unordered_map<uint, uint> out_edges; // { dest, weight }

    bool operator<(const Vertex &aOther) const {
        return dist > aOther.dist;
    }
};

int main() {
    uint num_vertices, num_edges;
    cin >> num_vertices >> num_edges;

    // Initialise vertices
    vector<Vertex> vertices(num_vertices);
    for (int i = 0; i < num_vertices; i++) {
        vertices[i].explored = false;
        vertices[i].id = i;
        vertices[i].parent_id = INF;
        vertices[i].dist = INF;
    }

    // Build edges
    for (int i = 0; i < num_edges; i++) {
        uint source_id, dest_id, weight;
        cin >> source_id >> dest_id >> weight;
        vertices[source_id].out_edges[dest_id] = weight;
    }

    // Get source vertex and set its distance to 0 and parent to itself
    uint source_id;
    cin >> source_id;
    vertices[source_id].dist = 0;
    vertices[source_id].parent_id = source_id;

    // Dijkstra's algorithm at O((E + V) log V)
    priority_queue<Vertex*> q;
    q.push(&vertices[source_id]);
    while (!q.empty()) {
        Vertex *vertex = q.top();
        q.pop();
        if (!vertex->explored) {
            vertex->explored = true;
            for (auto &[ dest, weight ] : vertex->out_edges) {
                if (vertices[dest].dist > vertex->dist + weight) {
                    vertices[dest].dist = vertex->dist + weight;
                    vertices[dest].parent_id = vertex->id;
                    q.push(&vertices[dest]);
                }
            }
        }
    }

    // Print results to standard output
    cout << "Single-Source Shortest Paths:" << endl;
    for (auto &vertex : vertices) {
        cout << "ID: " << vertex.id;
        
        cout << "\tParent ID: ";
        vertex.parent_id == INF ? cout << "NONE" : cout << vertex.parent_id;

        cout << "\tDistance: ";
        vertex.dist == INF ? cout << "INF" : cout << vertex.dist;

        cout << endl;
    }

    return 0;
}
