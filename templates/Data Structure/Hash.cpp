class Hash {
private:
    const int sz; // 1 or 2
    vector<int> p, m;
    vector<vector<int>> pref, pow;
    int n;

    void build(const string &s) {
        for (int j = 0; j < sz; ++j) {
            for (int i = 0; i < s.size(); ++i) {
                if (i) pow[j][i] = (1LL * pow[j][i - 1] * p[j]) % m[j];
                // todo: update `s[i]` to (s[i] - 'a' + 1LL) ?
                pref[j][i + 1] = (pref[j][i] + s[i] * 1LL * pow[j][i]) % m[j];
            }
        }
    }

public:
    // todo: update sz to 1 or 2
    Hash(const string &s) : sz(2) {
        init(s);
    }

    void init(const string &s) {
        p.resize(2), m.resize(2);
        // todo: update 'p[i]` if `s` contains non-lowercase chars
        // 211, 223
        p[0] = 31, p[1] = 29, m[0] = 1e9 + 7, m[1] = 1e9 + 9;

        n = s.size();

        pref.resize(sz), pow.resize(sz);
        for (int j = 0; j < sz; ++j) {
            pref[j].assign(s.size() + 1, 0);
            pow[j].assign(s.size(), 1);
        }
        build(s);
    }

    pair<int, int> get_hash(const int l, const int r) const {
        const int diff = (n - 1) - r;
        int a = -1, b = -1;
        for (int j = 0; j < sz; ++j) {
            int x = pref[j][r + 1] - pref[j][l];
            if (x < 0) x += m[j];
            x = (1LL * x * pow[j][diff]) % m[j];
            if (a == -1) a = x;
            else b = x;
        }
        return {a, b};
    }

    bool is_equal(const int a, const int b, const int x, const int y) const {
        if (y - x != b - a) return false;
        return get_hash(a, b) == get_hash(x, y);
    }
};
