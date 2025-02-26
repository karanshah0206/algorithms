// Find the shortest path between all vertex pairs in a general simple graph

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <queue>
#include <limits>

using namespace std;

const int INF = numeric_limits<int>::max();

struct Vertex {
    int id;
    unordered_map<int, int> outgoing_edges; // { destination id, weight };
    unordered_map<int, pair<int, int>> shortest_to_me; // { root id, { dist from root, parent id } }
};

struct Edge {
    int source_id, dest_id;
    int weight;
};

// Check if traversing given edge yields shorter path from root to edge's destination
bool relax_edge(Edge &edge, vector<Vertex> &vertices, int root_id) {
    if (vertices[edge.source_id].shortest_to_me[root_id].second == INF) {
        return false; // Can't see path from root vertex to edge's source vertex
    }

    if (vertices[edge.dest_id].shortest_to_me[root_id].second == INF || vertices[edge.dest_id].shortest_to_me[root_id].first > vertices[edge.source_id].shortest_to_me[root_id].first + edge.weight) {
        vertices[edge.dest_id].shortest_to_me[root_id] = { vertices[edge.source_id].shortest_to_me[root_id].first + edge.weight, edge.source_id };
        return true; // Relaxed (found better path)
    }

    return false; // Did not relax (original path as good or better)
}

// Calculate SSSP from newly created vertex with zero-weighted edges to all vertices
bool bellman_ford(vector<Vertex> &vertices, vector<Edge> &edges, int root_id) {
    // Relax all edges |V| - 1 times (|V| being number of vertices in the graph)
    for (int i = 0; i < vertices.size() - 1; i++) {
        bool did_relax = false;

        for (Edge &edge : edges) {
            if (relax_edge(edge, vertices, root_id)) {
                did_relax = true;
            }
        }

        if (!did_relax) {
            break;
        }
    }

    // Run 1 more cycle of edge relaxation to check if negative-weight cycle(s) exist
    for (Edge &edge : edges) {
        if (relax_edge(edge, vertices, root_id)) {
            return false; // simple SSSP doesn't exist for graph
        }
    }

    return true; // no negative-weight cycles in graph
}

// Calculate SSSP sourced at root_id for transformed graph having only non-negative edges
void dijkstra(vector<Vertex> &vertices, int root_id) {
    auto comparator = [root_id](Vertex *source, Vertex *dest) {
        return source->shortest_to_me[root_id].first > dest->shortest_to_me[root_id].first;
    };

    unordered_set<int> seen_ids;
    priority_queue<Vertex *, vector<Vertex *>, decltype(comparator)> q(comparator);

    q.push(&vertices[root_id]);

    while (!q.empty()) {
        Vertex *vertex = q.top();
        q.pop();
        if (!seen_ids.count(vertex->id)) {
            seen_ids.insert(vertex->id);
            for (auto &[ dest, weight ] : vertex->outgoing_edges) {
                if (vertices[dest].shortest_to_me[root_id].first > vertex->shortest_to_me[root_id].first + weight) {
                    vertices[dest].shortest_to_me[root_id].first = vertex->shortest_to_me[root_id].first + weight;
                    vertices[dest].shortest_to_me[root_id].second = vertex->id;
                    q.push(&vertices[dest]);
                }
            }
        }
    }
}

// Recursively recalculate stored path weights using original (non-reweighted) edge weights.
void recalculate_path_weights(vector<Vertex> &vertices, int root_id, int my_id, int carry) {
    vertices[my_id].shortest_to_me[root_id].first = carry;
    for (auto &[ dest, weight ] : vertices[my_id].outgoing_edges) {
        if (vertices[dest].shortest_to_me[root_id].second == my_id) {
            recalculate_path_weights(vertices, root_id, dest, carry + weight);
        }
    }
}

// Recursively print vertices on path rooted at `from_id` terminating at `to_id`
void print_path(vector<Vertex> &vertices, int from_id, int to_id) {
    if (to_id != from_id) {
        print_path(vertices, from_id, vertices[to_id].shortest_to_me[from_id].second);
    }
    cout << " " << to_id;
}

// Johnson's algorithm to solve APSP in O(VE log V) time
int main() {
    int num_vertices, num_edges;
    cin >> num_vertices >> num_edges;

    // Initialise vertices
    vector<Vertex> vertices(num_vertices);
    unordered_map<int, pair<int, int>> shortest_init;
    for (int i = 0; i < num_vertices; i++) {
        shortest_init[i] = { INF, INF };
    }
    for (int i = 0; i < num_vertices; i++) {
        vertices[i].id = i;
        vertices[i].shortest_to_me = shortest_init;
        vertices[i].shortest_to_me[i] = { 0, i };
    }

    // Build edges
    vector<Edge> edges(num_edges);
    for (int i = 0; i < num_edges; i++) {
        cin >> edges[i].source_id >> edges[i].dest_id >> edges[i].weight;
        vertices[edges[i].source_id].outgoing_edges[edges[i].dest_id] = edges[i].weight;
    }

    // Add a new vertex with outgoing edges of 0 weight to all vertices
    vertices.push_back({});
    vertices[num_vertices].id = num_vertices;
    vertices[num_vertices].shortest_to_me = shortest_init;
    vertices[num_vertices].shortest_to_me[num_vertices] = { 0, num_vertices };
    for (int i = 0; i < num_vertices; i++) {
        // Add 0-weighted edge from new vertex to existing vertex
        vertices[num_vertices].outgoing_edges[i] = 0;
        Edge e;
        e.source_id = num_vertices;
        e.dest_id = i;
        e.weight = 0;
        edges.push_back(e);

        // Set shortest known path from new vertex to existing vertex to unknown
        vertices[i].shortest_to_me[num_vertices] = { INF, INF };
    }

    // Use Bellman-Ford to determine edge reweight values to avoid negative edges and identify if negative-weight cycles exist
    if (!bellman_ford(vertices, edges, num_vertices)) {
        cout << "Cannot determine simple shortest paths as graph contains negative-weight cycles." << endl;
        return 1;
    }

    // Remove the new vertex and the edges created for the new vertex
    vertices.pop_back();
    for (int i = 0; i < num_vertices; i++) {
        edges.pop_back();
    }

    // Reweight edges by adding shortest distance from new vertex to edge source vertex and subtracting shortest distance from new vertex to edge dest
    for (Edge &edge : edges) {
        vertices[edge.source_id].outgoing_edges[edge.dest_id] = edge.weight + vertices[edge.source_id].shortest_to_me[num_vertices].first - vertices[edge.dest_id].shortest_to_me[num_vertices].first;
    }

    // Erase shortest distance stored between new vertex and all other vertices
    for (Vertex &vertex : vertices) {
        vertex.shortest_to_me.erase(num_vertices);
    }

    // Perform Dijkstra on each vertex using the reweighted edge values
    for (Vertex &vertex : vertices) {
        dijkstra(vertices, vertex.id);
    }

    // Restore original edge weights
    for (Edge &edge : edges) {
        vertices[edge.source_id].outgoing_edges[edge.dest_id] = edge.weight;
    }

    // Recalculate path distances based on original weights
    for (Vertex &vertex : vertices) {
        recalculate_path_weights(vertices, vertex.id, vertex.id, 0);
    }

    // Print All-Pairs Shortest Paths to standard output
    for (Vertex &source : vertices) {
        for (Vertex &destination : vertices) {
            cout << "From " << source.id << " to " << destination.id << ":" << endl;
            if (destination.shortest_to_me[source.id].first == INF) {
                cout << "\tNo Path" << endl;
            } else {
                cout << "\tWeight: " << destination.shortest_to_me[source.id].first << endl << "\tPath:";
                print_path(vertices, source.id, destination.id);
                cout << endl;
            }
        }
    }

    return 0;
}

/*
Sample Test Case 1:
3 3
0 1 -1
1 2 -1
2 0 2

Sample Test Case 2:
8 14
0 3 2
1 0 6
2 0 4
1 2 3
3 1 -7
2 3 5
3 2 -3
4 5 0 
5 4 2
4 7 1
7 4 0
7 6 5
5 6 0
6 4 3
*/
