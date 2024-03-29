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
    T query_o1(int a, int b) const { // [a, b)
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


template<typename T_string = std::string>
class Suffix_array {
public:
    const int n = 0;
    const T_string str; // string to build suffix array for it
    vector<int> suffix; // suffix array
    vector<int> rank;
    vector<int> lcp;
    // rmq is only for using compare(), get_lcp()
    Sparse_table<int, 0> rmq;

    Suffix_array() : n(0), str("") {}

    Suffix_array(const T_string &_str, const bool build_rmq = false)
            : str(_str), n(_str.size()) {
        build(_str, build_rmq);
    }

    void build(const T_string &_str, const bool build_rmq = false) {
        // you MUST call the first two functions to build suffix array
        build_suffix(); // O(n) or O(n.log(n)) if alphabet >= 128
        build_rank(); // O(nlog(n))
        build_lcp(); // O(n)
        // build_rmq only if you will use compare() or get_lcp()
        if (build_rmq) rmq.build(lcp); // O(nlog(n))
    }

private:
    void build_suffix() {
        suffix.assign(n, 0);

        for (int i = 0; i < n; i++) {
            suffix[i] = i;
        }

        bool large_alphabet = false;

        for (int i = 0; i < n; i++) {
            if (str[i] < 0 || str[i] >= 128) {
                large_alphabet = true;
            }
        }

        // Sort each suffix by the first character.
        if (large_alphabet) {
            sort(suffix.begin(), suffix.end(), [&](int a, int b) {
                return str[a] < str[b];
            });
        } //
        else {
            vector<int> freq(128, 0);

            for (int i = 0; i < n; i++) {
                freq[str[i]]++;
            }

            for (int c = 1; c < 128; c++) {
                freq[c] += freq[c - 1];
            }

            for (int i = 0; i < n; i++)
                suffix[--freq[str[i]]] = i;
        }
    }

    void build_rank() {
        // Compute the rank of each suffix. Tied suffixes share the same rank.
        rank.assign(n, 0);
        rank[suffix[0]] = 0;

        for (int i = 1; i < n; i++) {
            rank[suffix[i]] = str[suffix[i]] == str[suffix[i - 1]] ?
                              rank[suffix[i - 1]] : i;
        }

        vector<int> next_index(n);
        vector<int> values(n);
        bool done = false;

        for (int len = 1; len < n && !done; len *= 2) {
            // next_index[i] = the next index to use for a suffix of rank i.
            // We insert them in order of the rank of the
            // suffix that comes len characters after the current suffix.
            for (int i = 0; i < n; i++) {
                next_index[i] = i;
            }

            // Compute the suffix array for 2 * len.
            // Suffixes of length <= len are prioritized first.
            for (int i = n - len; i < n; i++) {
                values[next_index[rank[i]]++] = i;
            }

            for (int i = 0; i < n; i++) {
                int prev = suffix[i] - len;

                if (prev >= 0)
                    values[next_index[rank[prev]]++] = prev;
            }

            swap(suffix, values);

            // Compute the rank array for 2 * len.
            values[suffix[0]] = 0;
            done = true;

            for (int i = 1; i < n; i++) {
                int s = suffix[i], prev = suffix[i - 1];

                if (s + len < n && prev + len < n && rank[s] == rank[prev] && rank[s + len] == rank[prev + len]) {
                    values[s] = values[prev];
                    done = false;
                } else values[s] = i;
            }
            swap(rank, values);
        }
    }

    void build_lcp() {
        lcp.assign(n, 0);
        int match = 0;

        for (int i = 0; i < n; i++) {
            if (rank[i] == 0) continue;

            int a = suffix[rank[i]] + match;
            int b = suffix[rank[i] - 1] + match;

            while (a < n && b < n && str[a++] == str[b++]) {
                match++;
            }

            // lcp[r] = the longest common prefix length of the suffixes starting at suffix[r] and at suffix[r - 1].
            // Note that lcp[0] is always 0.
            lcp[rank[i]] = match;
            match = max(match - 1, 0);
        }
    }

// -----------------Helper functions -------------------------//
// ------(not necessary for suffix array to work-------------//
public:
    int get_lcp(int a, int b) const {
        if (a >= n || b >= n) return 0;
        if (a == b) return n - a;
        return get_lcp_from_ranks(rank[a], rank[b]);
    }

    // Compares the substrings starting at `a` and `b` up to `length` in O(1).
    int compare(int a, int b, int length = -1) const {
        if (length < 0) length = n;
        if (a == b) return 0;

        int common = get_lcp(a, b);
        if (common >= length) return 0;
        if (a + common >= n || b + common >= n) {
            return a + common >= n ? -1 : 1;
        }
        return str[a + common] < str[b + common] ?
               -1 : (str[a + common] == str[b + common] ? 0 : 1);
    }

    // find the firstIdx in suffix vector that >= string 'p'
    int lower_bound(const T_string &p) const { // O(p.size()*log(suffix.size()))
        int st = 0, ed = suffix.size() - 1, best = ed + 1;
        while (st <= ed) {
            const int mid = st + (ed - st) / 2;
            const int c = sp_cmp(str, suffix[mid], p, 0);
            if (c >= 0) {
                best = mid;
                ed = mid - 1;
            } else st = mid + 1;
        }
        return best;
    }

    // find the firstIdx in suffix vector that > string 'p'
    int upper_bound(const T_string &p) const { // O(p.size()*log(suffix.size()))
        int st = 0, ed = suffix.size() - 1, best = ed + 1;
        while (st <= ed) {
            const int mid = st + (ed - st) / 2;
            const int c = sp_cmp(str, suffix[mid], p, 0);
            if (c > 0) {
                best = mid;
                ed = mid - 1;
            } else st = mid + 1;
        }
        return best;
    }

    bool binary_search(const T_string &p) const {
        return lower_bound(p) < upper_bound(p);
    }

private:
    // the following function is used by get_lcp()
    // be sure that you have built rmq before using this function
    int get_lcp_from_ranks(int a, int b) const {
        if (a == b) return n - suffix[a];
        if (a > b) swap(a, b);
        return rmq.query_o1(a + 1, b + 1);// query from a+1 to b
    }

    // special compare function for
    // lower/upper_bound() and binary_search()
    int sp_cmp(const T_string &a, int sta, const T_string &b, const int stb) const {
//      if T_string is string
//      you can return the following line
//        return a.compare(sta, b.size(), b);
//      otherwise use the following code:
        int i = sta, k = stb;
        for (; i < a.size() && k < b.size(); ++i, ++k) {
            if (a[i] < b[k]) return -1;
            else if (a[i] > b[k]) return 1;
        }
        if (k >= b.size()) return 0;
        return -1;
    }
};
