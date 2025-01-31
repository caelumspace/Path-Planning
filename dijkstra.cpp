#include <iostream>
#include <vector>
#include <queue>
#include <limits>
using namespace std;

/**
 * Dijkstra's Algorithm
 *
 * Given a directed or undirected weighted graph (with non-negative weights),
 * find the shortest path distances from a source vertex to all other vertices.
 *
 * Key Points:
 *  1. We use an adjacency list where graph[u] contains pairs (v, weight)
 *     indicating there's an edge from u to v with a given weight.
 *  2. We use a min-heap priority queue that always gives us the vertex
 *     with the smallest current distance from the source.
 *  3. Distances are stored in a vector `dist`. Initially, dist[u] = infinity
 *     for all vertices u except the source which is 0.
 *  4. We repeatedly extract the vertex with the smallest distance, then
 *     update its neighbors if a shorter path is found via this vertex.
 */

vector<int> dijkstra(int n, // number of vertices
                     const vector<vector<pair<int,int>>>& graph, // adjacency list
                     int source)
{
    // Initialize distance array with "infinity"
    const int INF = numeric_limits<int>::max();
    vector<int> dist(n, INF);
    dist[source] = 0;

    // Min-heap priority queue of (distance, vertex).
    // Using a pair here ensures it sorts by the first element (distance).
    priority_queue<pair<int,int>,
                   vector<pair<int,int>>,
                   greater<pair<int,int>>> pq;

    // Start by pushing the source vertex with distance 0
    pq.push({0, source});

    // While there are vertices to process in the queue
    while(!pq.empty()) {
        // Extract vertex u with the smallest distance
        auto [currentDist, u] = pq.top();
        pq.pop();

        // If we've already found a better path before, skip this one
        if(currentDist > dist[u]) {
            continue;
        }

        // Relax edges out of u
        for(const auto& edge : graph[u]) {
            int v = edge.first;      // neighbor vertex
            int weight = edge.second; // edge weight u -> v

            // If a shorter path to v is found
            if(dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                pq.push({dist[v], v});
            }
        }
    }

    return dist;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Example usage:
    // Input format:
    // n m
    // Then m lines of: u v w
    //   where u, v are vertices (0-based) and w is edge weight
    // Then a line containing the source vertex s.
    // This example demonstrates reading an undirected graph, but
    // Dijkstra works for directed graphs with non-negative weights as well.
    //
    // Sample input:
    // 5 6
    // 0 1 4
    // 0 2 2
    // 1 2 3
    // 1 3 2
    // 2 3 4
    // 3 4 1
    // 0
    //
    // Explanation of sample:
    // There are 5 vertices (0 through 4), 6 edges. We read them and
    // then run Dijkstra from source = 0.

    int n, m;
    cin >> n >> m;

    // Graph representation: graph[u] = list of (v, weight) pairs
    vector<vector<pair<int,int>>> graph(n);

    for(int i = 0; i < m; i++){
        int u, v, w;
        cin >> u >> v >> w;
        
        // For undirected graphs, add edges both ways:
        graph[u].push_back({v, w});
        graph[v].push_back({u, w});
        
        // If directed, only add the edge (u->v)
        // graph[u].push_back({v, w});
    }

    int source;
    cin >> source;

    // Run Dijkstra from the given source
    vector<int> distances = dijkstra(n, graph, source);

    // Output distances
    cout << "Shortest distances from vertex " << source << ":\n";
    for(int i = 0; i < n; i++){
        if(distances[i] == numeric_limits<int>::max()) {
            cout << "Vertex " << i << ": INF\n";
        } else {
            cout << "Vertex " << i << ": " << distances[i] << "\n";
        }
    }

    return 0;
}
