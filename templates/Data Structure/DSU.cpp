class DSU {
public:
    vector<int> par, size;
    int CC;

    DSU(const int n) {
        init(n);
    }

    void init(const int n) {
        CC = n;
        par.resize(n);
        size.resize(n);
        for (int i = 0; i < n; ++i) {
            par[i] = i;
            size[i] = 1;
        }
    }

    int find(const int u) {
        if (par[u] == u) return u;
        return par[u] = find(par[u]);
    }

    bool link(int a, int b) {
        a = find(a), b = find(b);
        if (a == b) return false;
        if (size[a] < size[b]) swap(a, b);
        size[a] += size[b];
        par[b] = a;
        --CC;
        return true;
    }

    bool isConn(const int a, const int b) {
        return find(a) == find(b);
    }
};
