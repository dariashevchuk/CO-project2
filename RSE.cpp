#include <iostream>
#include <vector>
#include <utility>
#include <climits>
#include <algorithm>
#include<numeric>

using namespace std;

const int MAX_NODES = 63;

bool isIndependentSet(long long remainingNodes, const vector<long long>& adjacency) {
    for (int i = 0; i < MAX_NODES; ++i) {
        if (remainingNodes & (1LL << i)) {
            if ((remainingNodes & adjacency[i]) != 0) {
                return false;
            }
        }
    }
    return true;
}

void findRepresentation(int index, int selectedCount, vector<pair<int, int>>& selectedPairs,
    const vector<pair<int, int>>& edges, long long remainingNodes,
    const vector<long long>& adjacency, vector<pair<int, int>>& bestRepresentation) {

    if (selectedCount >= bestRepresentation.size() && !bestRepresentation.empty()) {
        return;
    }

    if (index == edges.size()) {
        if (isIndependentSet(remainingNodes, adjacency)) {
            if (bestRepresentation.empty() || selectedCount < bestRepresentation.size()) {
                bestRepresentation = selectedPairs;
            }
        }
        return;
    }

    int u = edges[index].first;
    int v = edges[index].second;
    if ((remainingNodes & (1LL << u)) && (remainingNodes & (1LL << v))) {
        selectedPairs.push_back(edges[index]);
        long long newRemainingNodes = remainingNodes & ~(1LL << u) & ~(1LL << v);
        findRepresentation(index + 1, selectedCount + 1, selectedPairs, edges, newRemainingNodes, adjacency, bestRepresentation);
        selectedPairs.pop_back();
    }

    findRepresentation(index + 1, selectedCount, selectedPairs, edges, remainingNodes, adjacency, bestRepresentation);
}




vector<int> sortVerticesByDegree(int n, const vector<int>& degrees) {
    vector<int> vertices(n);
    iota(vertices.begin(), vertices.end(), 0);

    sort(vertices.begin(), vertices.end(), [&degrees](int a, int b) {
        return degrees[a] > degrees[b];
        });

    return vertices;
}

vector<pair<int, int>> sortEdgesByVertices(const vector<pair<int, int>>& edges, const vector<int>& vertexOrder) {
    vector<pair<int, int>> sortedEdges = edges;
    vector<int> position(vertexOrder.size());
    for (int i = 0; i < vertexOrder.size(); ++i) {
        position[vertexOrder[i]] = i;
    }

    sort(sortedEdges.begin(), sortedEdges.end(), [&position](const pair<int, int>& a, const pair<int, int>& b) {
        return min(position[a.first], position[a.second]) < min(position[b.first], position[b.second]);
        });

    return sortedEdges;
}



int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    int n, m;
    cin >> n >> m;

    vector<long long> adjacency(MAX_NODES, 0);
    vector<pair<int, int>> edges;
    vector<int> degrees(MAX_NODES, 0);

    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        adjacency[u] |= (1LL << v);
        adjacency[v] |= (1LL << u);
        edges.emplace_back(u, v);
        degrees[u]++;
        degrees[v]++;
    }

    vector<int> vertexOrder = sortVerticesByDegree(n, degrees);
    edges = sortEdgesByVertices(edges, vertexOrder);

    vector<pair<int, int>> selectedPairs, bestRepresentation;
    long long remainingNodes = (1LL << n) - 1;
    findRepresentation(0, 0, selectedPairs, edges, remainingNodes, adjacency, bestRepresentation);

    cout << bestRepresentation.size() << "\n";
    for (const auto& pair : bestRepresentation) {
        cout << pair.first << " " << pair.second << "\n";
    }

    return 0;
}