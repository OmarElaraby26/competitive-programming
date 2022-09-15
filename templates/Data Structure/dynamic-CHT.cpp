// source code: https://github.com/kth-competitive-programming/kactl/blob/main/content/data-structures/LineContainer.h

/**
 *  Description: Container where you can add lines of the form m*x+c, and query (maximum or minimum) values at point x.
 *  Useful for dynamic programming (`convex hull trick`).
 * Time: O(\log N)
 */
struct Line {
    mutable ll m, c, p;

    bool operator<(const Line &o) const { return m < o.m; }

    bool operator<(ll x) const { return p < x; }
};

class LineContainer : multiset<Line, less<>> {
private:
    const int mode; // maximum mode = 1, minimum mode = -1

    // TODO: (for doubles, use inf = 1/.0, div(a,b) = a/b)
    static const ll inf = LLONG_MAX;

    ll div(ll a, ll b) { // floored division
        return a / b - ((a ^ b) < 0 && a % b);
    }

    bool isect(iterator x, iterator y) {
        if (y == end()) return x->p = inf, 0;
        if (x->m == y->m) x->p = x->c > y->c ? inf : -inf;
        else x->p = div(y->c - x->c, x->m - y->m);
        return x->p >= y->p;
    }

public:
    // maximum mode = 1, minimum mode = -1
    LineContainer(const int md) : mode(md) {}

    void add(ll m, ll c) {
        m *= mode, c *= mode;
        auto z = insert({m, c, 0}), y = z++, x = y;
        while (isect(y, z)) z = erase(z);
        if (x != begin() && isect(--x, y)) isect(x, y = erase(y));
        while ((y = x) != begin() && (--x)->p >= y->p)
            isect(x, erase(y));
    }

    ll query(ll x) {
        assert(!empty());
        auto l = *lower_bound(x);
        return mode * (l.m * x + l.c);
    }
};
