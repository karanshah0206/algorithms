#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <stack>

using namespace std;

typedef unordered_set<size_t> nodeGroup;
typedef unordered_map<size_t, nodeGroup> graph;

// use stack to get nodes in order of finishing times (descending)
void stackBuilderDFS(size_t node, graph& G, stack<size_t>& S, nodeGroup& visited) {
    visited.insert(node);
    for (const size_t& child : G[node])
        if (visited.find(child) == visited.end())
            stackBuilderDFS(child, G, S, visited);
    S.push(node);
}

// traverse all nodes possible using DFS, each traversable node is part of the SCC
void sccFinderDFS(size_t node, graph& T, nodeGroup& visited, nodeGroup& SCC) {
    visited.insert(node);
    SCC.insert(node);
    for (const size_t& child : T[node])
        if (visited.find(child) == visited.end())
            sccFinderDFS(child, T, visited, SCC);
}

// transpose graph (change direction of all edges)
graph transposeGraph(graph& G) {
    graph T;

    for (const pair<size_t, nodeGroup>& routes : G)
        for (const size_t& destNode : routes.second)
            T[destNode].insert(routes.first);

    return T;
}

// generate graph from standard input
graph getGraph() {
    graph G;
    size_t n;

    cout << "Enter number of nodes: " << endl;
    cin >> n;

    for (size_t i = 1; i <= n; i++) G[i] = {};

    cout << "Enter number of (directed) routes: ";
    cin >> n;

    cout << "Enter " << n << " paths in format: {startNodeIndex} {destinationNodeIndex}" << endl;
    while (n--) {
        size_t startNode, destNode;
        cin >> startNode >> destNode;
        G[startNode].insert(destNode);
    }

    return G;
}

int main() {
    graph G, T;
    stack<size_t> S;
    nodeGroup visited;
    vector<nodeGroup> stronglyConnectedComponents;

    G = getGraph();
    T = transposeGraph(G);

    // get nodes in order of finishing times (highest first)
    for (const pair<size_t, nodeGroup>& routes : G)
        if (visited.find(routes.first) == visited.end())
            stackBuilderDFS(routes.first, G, S, visited);

    visited.clear();

    // get all strongly connected components
    while (!S.empty()) {
        nodeGroup SCC;
        size_t node = S.top();
        S.pop();
        if (visited.find(node) == visited.end()) {
            sccFinderDFS(node, T, visited, SCC);
            stronglyConnectedComponents.push_back(SCC);
        }
    }

    // print all strongly connected components to standard output
    for (const nodeGroup& scc : stronglyConnectedComponents) {
        for (const size_t& node : scc) cout << node << " ";
        cout << endl;
    }

    return 0;
}
