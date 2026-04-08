#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const int N = 100005;

struct Edge {
    int to;
    int id;
};

vector<Edge> adj[N];
int dfn[N], low[N];
int timer = 0;
vector<pair<int, int>> st;

vector<int> bcc_adj[N];
int color[N];
bool good[N];

void process_bcc(const vector<pair<int, int>>& edges) {
    if (edges.empty()) return;
    vector<int> nodes;
    for (auto e : edges) {
        nodes.push_back(e.first);
        nodes.push_back(e.second);
        bcc_adj[e.first].push_back(e.second);
        bcc_adj[e.second].push_back(e.first);
    }
    sort(nodes.begin(), nodes.end());
    nodes.erase(unique(nodes.begin(), nodes.end()), nodes.end());
    
    bool is_bipartite = true;
    vector<int> q;
    for (int start_node : nodes) {
        if (color[start_node] == 0) {
            color[start_node] = 1;
            q.push_back(start_node);
            int head = 0;
            while (head < (int)q.size()) {
                int u = q[head++];
                for (int v : bcc_adj[u]) {
                    if (color[v] == 0) {
                        color[v] = 3 - color[u];
                        q.push_back(v);
                    } else if (color[v] == color[u]) {
                        is_bipartite = false;
                    }
                }
            }
        }
    }
    
    if (!is_bipartite) {
        for (int node : nodes) {
            good[node] = true;
        }
    }
    
    for (int node : nodes) {
        bcc_adj[node].clear();
        color[node] = 0;
    }
}

void dfs(int u, int p_edge_id) {
    dfn[u] = low[u] = ++timer;
    for (auto& edge : adj[u]) {
        int v = edge.to;
        int id = edge.id;
        if (id == p_edge_id) continue;
        if (dfn[v]) {
            low[u] = min(low[u], dfn[v]);
            if (dfn[v] < dfn[u]) {
                st.push_back({u, v});
            }
        } else {
            st.push_back({u, v});
            dfs(v, id);
            low[u] = min(low[u], low[v]);
            if (low[v] >= dfn[u]) {
                vector<pair<int, int>> bcc_edges;
                while (true) {
                    auto e = st.back();
                    st.pop_back();
                    bcc_edges.push_back(e);
                    if (e == make_pair(u, v)) break;
                }
                process_bcc(bcc_edges);
            }
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    
    int n, m;
    if (!(cin >> n >> m)) return 0;
    
    for (int i = 1; i <= m; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back({v, i});
        adj[v].push_back({u, i});
    }
    
    for (int i = 1; i <= n; ++i) {
        if (!dfn[i]) {
            dfs(i, -1);
        }
    }
    
    int ans = 0;
    for (int i = 1; i <= n; ++i) {
        if (!good[i]) {
            ans++;
        }
    }
    
    cout << ans << "\n";
    
    return 0;
}
