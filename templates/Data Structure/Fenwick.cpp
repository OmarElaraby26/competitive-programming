template<typename T>
T combine(const T &a, const T &b) {
    // +, - , ^, *, /
    return a + b;
}

template<typename T>
T revCombine(const T &a, const T &b) {
    // -, + , ^, /, *
    return a - b;
}

template<class T,
        T (*combine)(const T &, const T &),
        T (*revCombine)(const T &, const T &)>
class Fenwick {
private:
    vector<T> bit; // 1-base
    int n;
public:

    Fenwick(int sz) : n(sz) {
        // 1-base
        bit.assign(sz, 0);
    }

    template<typename G>
    // G = int or (long long)
    Fenwick(const vector<G> &a) // a is 1-base vector
            : Fenwick(a.size()) {
        // 1-base
        for (int i = 1; i < a.size(); ++i) {
            this->updateBy(i, T(a[i]));
        }
    }

    inline long long size() const { return n; }

    T prefix(int idx) const {
        // ret = 0 if +,-,^
        // ret = 1 if *
        T ret = 0;
        while (idx > 0) {
            ret = combine(ret, bit[idx]);
            idx -= idx & -idx;
        }
        return ret;
    }

    T query(int l, int r) const {
        if (l > r) swap(l, r);
        return revCombine(prefix(r), prefix(l - 1));
    }


    // update position 'idx' by delta
    // if operation is '+'
    // then updateBy(5, 3) will increase value in position 5 by 3
    // if operation is ^
    // then [idx] = [idx] ^ delta
    void updateBy(int idx, const T delta) {
        while (idx < n) {
            bit[idx] = combine(bit[idx], delta);
            idx += idx & -idx;
        }
    }
};
