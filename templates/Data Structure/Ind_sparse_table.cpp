#include <bits/stdc++.h>

using namespace std;

// this Sparse table is different of the original one
// in it has the ability to return the index of (min/max)imum value instead of the value itself
// NOTE: return index makes sense only with minimum and maximum mode
// and not with ex: gcd, +, *, ...etc
// also note that this version have additional property `data`
template<typename T, const bool maximum_mode = false>
// index sparse table
class Ind_sparse_table {
private:
    int n = 0;
    vector<vector<T>> table;
    vector<T> data;

    // a and b are indexes
    /// NOTE: if tie return the larger index
    // why the larger index ?
    // because child_ancestor() in LCA class do need the largest index
    T combine(const int &a, const int &b) const {
        if (maximum_mode)return data[a] > data[b] ? a : b;
        else return data[a] < data[b] ? a : b;
    }

public:
    Ind_sparse_table(const vector<T> &values = {}) {
        if (!values.empty()) build(values);
    }

    void build(const vector<T> &values) {
        data = values;
        n = int(values.size());
        int levels = __lg(n) + 1;
        if (n == 0) levels = 0;
        table.resize(levels);

        for (int k = 0; k < levels; k++) {
            table[k].resize(n - (1 << k) + 1);
        }

        if (n > 0) iota(table[0].begin(), table[0].end(), 0);

        for (int k = 1; k < levels; k++) {
            for (int i = 0; i <= n - (1 << k); i++) {
                table[k][i] = combine(table[k - 1][i],
                                      table[k - 1][i + (1 << (k - 1))]);
            }
        }
    }

    // [a, b) = a, a+1, ..., b-1
    // return value not index
    T query_o1(int a, int b) const {
        return data[ind_query_o1(a, b)];
    }

    // [a, b) = a, a+1, ..., b-1
    // return index not value
    int ind_query_o1(int a, int b) const {
        assert(0 <= a && a < b && b <= n);
        int level = __lg(b - a);
        return combine(table[level][a], table[level][b - (1 << level)]);
    }
};

int main() {

    vector<int> v{0, 1, 3, 2, 1, 5};
    Ind_sparse_table<int> rmq(v);
    cout << rmq.query_o1(1, 6) << '\n'; // output: 1
    cout << rmq.ind_query_o1(1, 6) << '\n'; // output: 4

    return 0;
}
