template<typename T, const int mode = 0>
class Sparse_table {
private:
    int n = 0;
    vector<vector<T>> table;

    static T combine(const T &a, const T &b) {
        if (mode == 0) return min(a, b);
        else if (mode == 1) return max(a, b);
        else if (mode == 2) return __gcd(a, b);
        else if (mode == 3) return a + b;
        else if (mode == 4) return a * b;
        else
            assert(mode <= 4);
    }

public:
    Sparse_table(const vector<T> &values = {}) {
        if (!values.empty()) build(values);
    }

    void build(const vector<T> &values) {
        n = int(values.size());
        int levels = __lg(n) + 1;
        if (n == 0) levels = 0;
        table.resize(levels);

        for (int k = 0; k < levels; k++) {
            table[k].resize(n - (1 << k) + 1);
        }

        if (n > 0) table[0] = values;

        for (int k = 1; k < levels; k++) {
            for (int i = 0; i <= n - (1 << k); i++) {
                table[k][i] = combine(table[k - 1][i],
                                      table[k - 1][i + (1 << (k - 1))]);
            }
        }
    }

    // [a, b) = a, a+1, ..., b-1
    T query_o1(int a, int b) const {
        assert(0 <= a && a < b && b <= n);
        int level = __lg(b - a);
        return combine(table[level][a], table[level][b - (1 << level)]);
    }

    // [a, b) = a, a+1, ..., b-1
    T query_ologn(int l, int r) const { // NON overlap friendly functions as +, * ..
        assert(0 <= l && l < r && l <= n);
        --r;
        T ans;
        bool started = 0; // not to add initall value to ans
        while (l <= r) {
            int msb = __lg(r - l + 1);
            T nxt = table[msb][l];
            l += (1 << msb);
            if (started) ans = combine(ans, nxt);
            else ans = nxt, started = true;
        }
        return ans;
    }
};
