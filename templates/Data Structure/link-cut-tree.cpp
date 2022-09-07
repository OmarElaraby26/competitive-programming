class LCT_Node {
public:
    int p, ch[2];
    ll val, sub; // sub contains the subtree values on the splay tree which is the path on the original tree
    bool rev;
    int sz;
    ll lazy;

//    LCT_Node() {}

    LCT_Node(int v = 0) : p(-1), val(v), sub(v), rev(0), sz(1), lazy(0) {
        ch[0] = ch[1] = -1;
    }
};

template<const int base = 1> // 1-based or 0-based ?
// todo, note: 0-base is not tested yet
// so for safety work with 1-based

// todo, note: query on the path is sum of all nodes on the path
// what if values on edges not on the vertices ?
// what if we have to calculate another functions ex: xor ?

// LCT means Link-Cut Tree
class LCT {
public:
    LCT(const int mx_sz) {
        t.assign(mx_sz + base, LCT_Node());
    }

    void create_tree(int v, int w) { t[v] = LCT_Node(w); }

    // link vertex v with vertex w
    // v is the parent and w is the child.
    bool link(int v, int w) {
        if (is_conn(v, w)) return false;
        rootify(w);
        t[w].p = v;
        return true;
    }

    bool cut(int v, int w) {
        if (!is_conn(v, w)) return false;
        rootify(w), access(v);
        t[v].ch[0] = t[t[v].ch[0]].p = -1;
        return true;
    }
    
    // is v connected with w ?
    bool is_conn(int v, int w) {
        access(v), access(w);
        return v == w ? true : t[v].p != -1;
    }

    // find answer on the path v to w
    ll query(int v, int w) {
        rootify(w), access(v);
        return t[v].sub;
    }

    // add x on all vertices from v to w
    void update(int v, int w, int x) {
        rootify(w), access(v);
        t[v].lazy += x;
    }

    int get_lca(int v, int w) {
        if (!is_conn(v, w)) return base - 1;
        access(v);
        return access(w);
    }

private:
    vector<LCT_Node> t;

    void prop(int x) { // O(1)
        if (t[x].lazy) {
            t[x].val += t[x].lazy, t[x].sub += t[x].lazy * t[x].sz;
            if (t[x].ch[0] + 1) t[t[x].ch[0]].lazy += t[x].lazy;
            if (t[x].ch[1] + 1) t[t[x].ch[1]].lazy += t[x].lazy;
        }
        if (t[x].rev) {
            swap(t[x].ch[0], t[x].ch[1]);
            if (t[x].ch[0] + 1) t[t[x].ch[0]].rev ^= 1;
            if (t[x].ch[1] + 1) t[t[x].ch[1]].rev ^= 1;
        }
        t[x].lazy = 0, t[x].rev = 0;
    }

    void update(int x) { // O(1)
        t[x].sz = 1, t[x].sub = t[x].val;
        for (int i = 0; i < 2; i++) {
            if (t[x].ch[i] + 1) {
                prop(t[x].ch[i]);
                t[x].sz += t[t[x].ch[i]].sz;
                t[x].sub += t[t[x].ch[i]].sub;
            }
        }
    }

    // make v is the root
    void rootify(int v) {
        access(v);
        t[v].rev ^= 1;
    }

    int find_root(int v) {
        access(v), prop(v);
        while (t[v].ch[0] + 1) v = t[v].ch[0], prop(v);
        return splay(v);
    }

    bool is_root(int x) { // O(1)
        return t[x].p == -1 || (t[t[x].p].ch[0] != x && t[t[x].p].ch[1] != x);
    }

    void rotate(int x) {
        int p = t[x].p, pp = t[p].p;
        if (!is_root(p)) t[pp].ch[t[pp].ch[1] == p] = x;
        bool d = t[p].ch[0] == x;
        t[p].ch[!d] = t[x].ch[d], t[x].ch[d] = p;
        if (t[p].ch[!d] + 1) t[t[p].ch[!d]].p = p;
        t[x].p = pp, t[p].p = x;
        update(p), update(x);
    }

    int splay(int x) {
        while (!is_root(x)) {
            int p = t[x].p, pp = t[p].p;
            if (!is_root(p)) prop(pp);
            prop(p), prop(x);
            if (!is_root(p)) rotate((t[pp].ch[0] == p) ^ (t[p].ch[0] == x) ? x : p);
            rotate(x);
        }
        return prop(x), x;
    }

    int access(int v) {
        int last = -1;
        for (int w = v; w + 1; update(last = w), splay(v), w = t[v].p) {
            splay(w);
            t[w].ch[1] = (last == -1 ? -1 : v);
        }
        return last;
    }
};
