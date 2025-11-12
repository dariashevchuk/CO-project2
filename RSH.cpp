#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <algorithm>
#include <cmath>
#include <random>
#include <chrono>
#include <ctime>
using namespace std;

int N, M;
static double T_SMALL = 27.0;
static double T_LARGE = 27.0;

struct EdgeSmall {
    int u, v;
};

struct EdgeLarge {
    int u, v;
    long long prio;
    bool active;
};

vector<EdgeSmall> edges_small;
vector<vector<int>> adj_small;
unordered_map<int, unordered_set<int>> adjList_small;
vector<bool> chosen_small;
int chosenCount_small = 0;

bool isIndependentSet(const vector<bool>& c, const vector<EdgeSmall>& e, const vector<vector<int>>& adj, int n) {
    vector<int> partner(n, -1);
    for (int i = 0; i < (int)e.size(); i++) {
        if (c[i]) {
            int u = e[i].u, v = e[i].v;
            partner[u] = v;
            partner[v] = u;
        }
    }
    for (int u = 0; u < n; u++) {
        if (partner[u] == -1) {
            for (int w : adj[u]) {
                if (partner[w] == -1) return false;
            }
        }
    }
    return true;
}

void buildInitialSolution(int n, int m) {
    vector<bool> used(n, false);
    chosen_small.assign(m, false);
    chosenCount_small = 0;
    for (int i = 0; i < m; i++) {
        int u = edges_small[i].u;
        int v = edges_small[i].v;
        if (!used[u] && !used[v]) {
            chosen_small[i] = true;
            chosenCount_small++;
            used[u] = true;
            used[v] = true;
        }
    }
}

bool tryRemoveEdge(int n) {
    vector<int> c;
    for (int i = 0; i < (int)chosen_small.size(); i++) {
        if (chosen_small[i]) c.push_back(i);
    }
    if (c.empty()) return false;
    int e = c[rand() % c.size()];
    chosen_small[e] = false;
    int oldCount = chosenCount_small;
    chosenCount_small--;
    if (isIndependentSet(chosen_small, edges_small, adj_small, n)) {
        return true;
    }
    else {
        chosen_small[e] = true;
        chosenCount_small = oldCount;
        return false;
    }
}

bool trySwapEdge(int n) {
    vector<int> c, nc;
    for (int i = 0; i < (int)chosen_small.size(); i++) {
        if (chosen_small[i]) c.push_back(i);
        else nc.push_back(i);
    }
    if (c.empty() || nc.empty()) return false;
    int ce = c[rand() % c.size()];
    int nce = nc[rand() % nc.size()];
    int oldCount = chosenCount_small;
    chosen_small[ce] = false;
    chosen_small[nce] = true;
    if (!isIndependentSet(chosen_small, edges_small, adj_small, n)) {
        chosen_small[ce] = true;
        chosen_small[nce] = false;
        chosenCount_small = oldCount;
        return false;
    }
    return true;
}

void attemptMove(double& T, double alpha, int& bestCount, vector<bool>& bestSol, int n) {
    bool ok = (rand() % 2 == 0) ? tryRemoveEdge(n) : trySwapEdge(n);
    if (ok && chosenCount_small < bestCount) {
        bestCount = chosenCount_small;
        bestSol = chosen_small;
    }
    T *= alpha;
}

double calcTemp(double s, double t, double mx) {
    return s * pow(0.001, t / mx);
}

void solveLarge(int n, int m, const vector<pair<int, int>>& inputEdges) {
    vector<long long> deg(n, 0LL);
    vector<EdgeLarge> edges(m);
    vector<vector<int>> adj(n);
    for (int i = 0; i < m; i++) {
        int u = inputEdges[i].first;
        int v = inputEdges[i].second;
        edges[i].u = u;
        edges[i].v = v;
        edges[i].active = true;
        deg[u]++;
        deg[v]++;
        adj[u].push_back(i);
        adj[v].push_back(i);
    }
    auto getPrio = [&](int i) {return deg[edges[i].u] + deg[edges[i].v]; };
    priority_queue<pair<long long, int>> pq;
    for (int i = 0; i < m; i++) {
        edges[i].prio = getPrio(i);
        pq.push({ edges[i].prio, i });
    }
    vector<bool> matched(n, false);
    vector<int> chosen;
    chosen.reserve(n / 2);
    while (!pq.empty()) {
        auto topItem = pq.top();
        pq.pop();
        long long oldP = topItem.first;
        int idx = topItem.second;
        if (!edges[idx].active) continue;
        long long curP = getPrio(idx);
        if (curP != oldP) {
            edges[idx].prio = curP;
            pq.push({ curP, idx });
            continue;
        }
        int u = edges[idx].u;
        int v = edges[idx].v;
        if (matched[u] || matched[v]) {
            edges[idx].active = false;
            continue;
        }
        chosen.push_back(idx);
        matched[u] = true;
        matched[v] = true;
        for (int e2 : adj[u]) {
            if (edges[e2].active) {
                edges[e2].active = false;
                int x = edges[e2].u;
                int y = edges[e2].v;
                deg[x]--;
                deg[y]--;
            }
        }
        for (int e2 : adj[v]) {
            if (edges[e2].active) {
                edges[e2].active = false;
                int x = edges[e2].u;
                int y = edges[e2].v;
                deg[x]--;
                deg[y]--;
            }
        }
    }
    cout << chosen.size() << "\n";
    for (auto& i : chosen) {
        cout << edges[i].u << " " << edges[i].v << "\n";
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    cin >> N >> M;
    vector<pair<int, int>> allEdges(M);
    for (int i = 0; i < M; i++) {
        int u, v;
        cin >> u >> v;
        allEdges[i] = { u, v };
    }
    if (N < 5000) {
        edges_small.resize(M);
        adj_small.assign(N, {});
        adjList_small.clear();
        adjList_small.reserve(N);
        for (int i = 0; i < M; i++) {
            int u = allEdges[i].first;
            int v = allEdges[i].second;
            edges_small[i] = { u, v };
            adj_small[u].push_back(v);
            adj_small[v].push_back(u);
            adjList_small[u].insert(v);
            adjList_small[v].insert(u);
        }
        buildInitialSolution(N, M);
        int bestCount = chosenCount_small;
        vector<bool> bestSolution = chosen_small;
        double startT = 1000.0;
        double alpha = 0.995;
        srand((unsigned)time(NULL));
        clock_t startTime = clock();
        while (true) {
            clock_t nowTime = clock();
            double elapsed = double(nowTime - startTime) / CLOCKS_PER_SEC;
            if (elapsed >= T_SMALL) break;
            double T = calcTemp(startT, elapsed, T_SMALL);
            attemptMove(T, alpha, bestCount, bestSolution, N);
        }
        vector<pair<int, int>> ans;
        for (int i = 0; i < M; i++) {
            if (bestSolution[i]) ans.push_back({ edges_small[i].u, edges_small[i].v });
        }
        cout << ans.size() << "\n";
        for (auto& p : ans) cout << p.first << " " << p.second << "\n";
    }
    else {
        solveLarge(N, M, allEdges);
    }
    return 0;
}
