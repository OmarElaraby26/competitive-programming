#include <bits/stdc++.h>

//#include <ext/pb_ds/assoc_container.hpp>
//#include <ext/pb_ds/tree_policy.hpp>
//using namespace __gnu_pbds; // pb_ds
//#define ordered_set tree<int, null_type, less<int>, rb_tree_tag, tree_order_statistics_node_update>


using namespace std;
typedef long long ll;
typedef long double ld;
#define EPS (1e-9)

void _IO_() {
    ios::sync_with_stdio(false), cin.tie(0), cout.tie(0);
#ifndef ONLINE_JUDGE
    freopen("in.txt", "r", stdin);
    freopen("out.txt", "w", stdout);
#endif
}

//             N  E  S  W  NW  NE SE SW
//int dr[] = {-1, 0, 1, 0, -1, -1, 1, 1};
//int dc[] = {0, 1, 0, -1, -1, 1, 1, -1};

template<typename T>
struct segNode {
    T val;
//    int st, ed;

    segNode() : val(T(0)) {}

    segNode(const T &a) : val(a) {}
};

template<typename T, const int mode = 0>
class SegTree {
public:
    SegTree(const vector<T> &v = {}) {
        n = v.size();
        if (!n) return;
        // set size of tree
        int lg = __lg(n);
        if ((1 << lg) < n) ++lg;
        n = 1 << lg;
        tree.assign(n << 1, segNode<T>());
        build(v, 0, 0, n - 1);
    }

    T pointUpdate(int idx, T newVal) {
        return pointUpdate(idx, newVal, 0, 0, n - 1);
    }

    T rangeQuery(int l, int r) {
        return rangeQuery(l, r, 0, 0, n - 1);
    }

private:
    static T identity() {
        // TODO: 1e18 & -1e18 must be 1e9,-1e9
        // if T is int
        if (mode == 0) return 1e18; // min so return infinity
        if (mode == 1) return -1e18;// max so return -(infinity)
        if (mode == 2) return 0; // gcd
        if (mode == 3) return 0; // additive identity
        if (mode == 4) return 1; // multiplicative identity
        else
            assert(mode >= 0 && mode <= 4);
    }

    static T combine(const T &a, const T &b) {
        if (mode == 0) return min(a, b);
        else if (mode == 1) return max(a, b);
        else if (mode == 2) return __gcd(a, b);
        else if (mode == 3) return a + b;
        else if (mode == 4) return a * b;
        else
            assert(mode >= 0 && mode <= 4);
    }

    vector<segNode<T>> tree;
    // n is the length of the extended orignial array
    int n;

    T build(const vector<T> &v, const int nodeIdx, const int st, const int ed) {
//        tree[nodeIdx].st = st;
//        tree[nodeIdx].ed = ed;

        if (st == ed) {
            if (st >= v.size()) return identity();
            return tree[nodeIdx].val = v[st];
        }

        const int lChild = (nodeIdx << 1) + 1;
        const int mid = st + (ed - st) / 2;
        return tree[nodeIdx].val = combine(build(v, lChild, st, mid),
                                           build(v, lChild + 1, mid + 1, ed));
    }


    T pointUpdate(const int targetIdx, const T newVal, const int nodeIdx, const int st, const int ed) {
        if (ed < targetIdx) return tree[nodeIdx].val;
        if (st > targetIdx) return tree[nodeIdx].val;
        if (st == ed) return tree[nodeIdx].val = newVal;

        const int lChild = (nodeIdx << 1) + 1;
        const int mid = st + (ed - st) / 2;
        return tree[nodeIdx].val = combine(pointUpdate(targetIdx, newVal, lChild, st, mid),
                                           pointUpdate(targetIdx, newVal, lChild + 1, mid + 1, ed));
    }


    T rangeQuery(const int l, const int r, const int nodeIdx, const int st, const int ed) {
        if (ed < l) return identity();
        if (st > r) return identity();
        if (st >= l && ed <= r) return tree[nodeIdx].val;

        const int lChild = (nodeIdx << 1) + 1;
        const int mid = st + (ed - st) / 2;
        return combine(rangeQuery(l, r, lChild, st, mid),
                       rangeQuery(l, r, lChild + 1, mid + 1, ed));
    }
};

int main() {
    _IO_();
//    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

    int n, m;
    cin >> n >> m;
    vector<ll> v(n);
    for (int i = 0; i < n; ++i) {
        cin >> v[i];
    }
    SegTree<ll, 3> segtree(v);

    while (m--) {
        int t;
        cin >> t;
        if (t == 1) {
            int idx, newVal;
            cin >> idx >> newVal;
            segtree.pointUpdate(idx, newVal);
        }//
        else {
            int l, r;
            cin >> l >> r;
            cout << segtree.rangeQuery(l, r - 1) << '\n';
        }
    }

    return 0;
}
