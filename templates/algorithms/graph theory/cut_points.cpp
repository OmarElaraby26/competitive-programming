vector<int> tin, low;
int timer;
vector<int> cuts;

vector<vector<int>> adj;

void dfs(int v, int p = -1) {
    tin[v] = low[v] = timer++;
    int children = 0;
    for (int to: adj[v]) {
        if (to == p) continue;
        if (tin[to] != -1) { // visited
            low[v] = min(low[v], tin[to]);
        }//
        else {
            dfs(to, v);
            low[v] = min(low[v], low[to]);
            ++children;
            if (low[to] >= tin[v] && (p != -1 || children >= 2)) {
                ++cuts[v];
            }
        }
    }
}

vector<int> cut_points;

void find_cutpoints() {
    const int n = adj.size();
    timer = 0;
    tin.assign(n, -1);
    low.assign(n, -1);
    cuts.assign(n, 0);

    for (int i = 0; i < n; ++i) {
        if (tin[i] == -1) { // not visited
            dfs(i);
        }
    }

    for (int i = 0; i < n; ++i) {
        // deg == 0 (isolated point)
        if (adj[i].size() == 0) {
            cuts[i] = -1;
        }
    }

    // NOTE:
    // if cuts[i] == -1 then `i` is an isolated point
    // if cuts[i] == 0 then `i` is a normal point
    // if cuts[i] >= 1 then `i` is a cut point
    // NOTE:
    // if we remove node `i`
    // then we will have new cuts[i] components
    // so the total number of components will be
    // CC + cuts[i]

    cut_points.clear();
    for (int i = 0; i < n; ++i) {
        if (cuts[i] >= 1) {
            cut_points.emplace_back(i);
        }
    }
}
