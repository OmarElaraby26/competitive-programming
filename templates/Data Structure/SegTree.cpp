#include <bits/stdc++.h>

//#include <ext/pb_ds/assoc_container.hpp>
//#include <ext/pb_ds/tree_policy.hpp>
//using namespace __gnu_pbds; // pb_ds
//#define ordered_set tree<int, null_type, less<int>, rb_tree_tag, tree_order_statistics_node_update>


using namespace std;
typedef long long ll;
typedef long double ld;
#define fi first
#define se second
#define EPS (1e-9)
#define all(v) v.begin(), v.end()
#define rall(v) v.rbegin(), v.rend()
#define debug(x) cerr << #x << ": " << (x) << endl;

void _IO_() {
    ios::sync_with_stdio(false), cin.tie(0), cout.tie(0);
#ifdef ELARABY
    freopen("in.txt", "r", stdin);
    freopen("out.txt", "w", stdout);
#endif
}

//             N  E  S  W  NW  NE SE SW
//int dr[] = {-1, 0, 1, 0, -1, -1, 1, 1};
//int dc[] = {0, 1, 0, -1, -1, 1, 1, -1};


template<typename T, const int mode = 0>
class SegNode {
public:
    T val;
//    int st, ed;

    SegNode() : val(T(0)) {}

    SegNode(const T &a) : val(a) {}

    SegNode<T, mode> merge(const SegNode &a) {
        if (mode == 0) return SegNode(min(val, a.val));
        else if (mode == 1)return SegNode(max(val, a.val));
        else if (mode == 2) return SegNode(__gcd(val, a.val));
        else if (mode == 3) return SegNode(val + a.val);
        else if (mode == 4) return SegNode(val * a.val);
        else
            assert(mode >= 0 && mode <= 4);
    }

    static SegNode<T, mode> identity() {
        // TODO: 1e18 & -1e18 must be 1e9,-1e9
        // if T is int
        if (mode == 0) return SegNode<T, mode>(1e9); // min so return infinity
        if (mode == 1) return SegNode<T, mode>(-1e9);// max so return -(infinity)
        if (mode == 2) return SegNode<T, mode>(0); // gcd
        if (mode == 3) return SegNode<T, mode>(0); // additive identity
        if (mode == 4) return SegNode<T, mode>(1); // multiplicative identity
        else
            assert(mode >= 0 && mode <= 4);
    }
};

template<typename T, const int mode = 0>
class SegTree {
public:
    SegTree(const vector<T> &v = {}) {
        init(v);
    }

    void init(const vector<T> &v = {}) {
        n = v.size();
        if (!n) {
            tree.clear();
            return;
        }
        // set size of tree
        int lg = __lg(n);
        if ((1 << lg) < n) ++lg;
        n = 1 << lg;
        tree.assign(n << 1, SegNode<T, mode>());
        build(v, 0, 0, n - 1);
    }

    SegNode<T, mode> pointUpdate(int idx, T newVal) {
        return pointUpdate(idx, newVal, 0, 0, n - 1);
    }

    // range from l to r inclusive [l:r]
    SegNode<T, mode> rangeQuery(int l, int r) {
        return rangeQuery(l, r, 0, 0, n - 1);
    }

private:

    vector<SegNode<T, mode>> tree;
    // n is the length of the extended orignial array
    int n;

    SegNode<T, mode> build(const vector<T> &v, const int nodeIdx, const int st, const int ed) {
//        tree[nodeIdx].st = st;
//        tree[nodeIdx].ed = ed;

        if (st == ed) {
            if (st >= v.size()) return SegNode<T, mode>::identity();
            return tree[nodeIdx] = SegNode<T, mode>(v[st]);
        }

        const int lChild = (nodeIdx << 1) + 1;
        const int mid = st + (ed - st) / 2;
        return tree[nodeIdx] = build(v, lChild, st, mid)
                .merge(
                        build(v, lChild + 1, mid + 1, ed));
    }


    SegNode<T, mode> pointUpdate(const int targetIdx, const T newVal, const int nodeIdx, const int st, const int ed) {
        if (ed < targetIdx) return tree[nodeIdx];
        if (st > targetIdx) return tree[nodeIdx];
        if (st == ed) return tree[nodeIdx] = SegNode<T, mode>(newVal);

        const int lChild = (nodeIdx << 1) + 1;
        const int mid = st + (ed - st) / 2;
        return tree[nodeIdx] = pointUpdate(targetIdx, newVal, lChild, st, mid)
                .merge(pointUpdate(targetIdx, newVal, lChild + 1, mid + 1, ed));
    }


    // range from l to r inclusive [l:r]
    SegNode<T, mode> rangeQuery(const int l, const int r, const int nodeIdx, const int st, const int ed) {
        if (ed < l) return SegNode<T, mode>::identity();
        if (st > r) return SegNode<T, mode>::identity();
        if (st >= l && ed <= r) return tree[nodeIdx];

        const int lChild = (nodeIdx << 1) + 1;
        const int mid = st + (ed - st) / 2;
        return rangeQuery(l, r, lChild, st, mid).merge(
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
            cout << segtree.rangeQuery(l, r - 1).val << '\n';
        }
    }

    return 0;
}
