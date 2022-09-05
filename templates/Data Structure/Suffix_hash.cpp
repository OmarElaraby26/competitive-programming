// this code is taken from `https://github.com/IsmailAkram2000/Competitive-Programing/blob/main/String/Suffix%20Hash.cpp` with light modification.
// modification: better abstraction, adding is_equal function and a constructor.

class Suffix_hash {
private:
    int n;
    string s;
    vector<ll> pow1, pow2, h1, h2;
    const int p1 = 313, p2 = 1013;
    const int m1 = 1e9 + 7, m2 = 1e9 + 9;
public:
    Suffix_hash(const string &x = "") {
        init(x);
    }

    void init(const string &x) {
        s = x;
        n = int(s.size());
        h1.assign(n, 0);
        h2.assign(n, 0);
        pow1.assign(n, 0);
        pow2.assign(n, 0);
        build();
    }

    pair<ll, ll> get_hash(int i, int j) {
        ll a = h1[j];
        if (i) {
            a -= h1[i - 1] * pow1[j - i + 1];
        }
        a = ((a % m1) + m1) % m1;

        ll b = h2[j];
        if (i) {
            b -= h2[i - 1] * pow2[j - i + 1];
        }
        b = ((b % m2) + m2) % m2;

        return {a, b};
    }

    bool is_equal(const int a, const int b, const int x, const int y) {
        assert(a <= b);
        assert(x <= y);
        if (b - a != y - x) return false;
        return get_hash(a, b) == get_hash(x, y);

    }

    pair<ll, ll> merge_hash(int i, int j, int k, int l) {
        assert(j >= i);
        assert(l >= k);
        const pair<ll, ll> a = get_hash(i, j),
                b = get_hash(k, l);
        return {((a.first * pow1[l - k + 1]) + b.first) % m1,
                ((a.second * pow2[l - k + 1]) + b.second) % m2};
    }

private:
    void build() {
        pow1[0] = pow2[0] = 1;
        for (int i = 1; i < n; i++) {
            pow1[i] = (pow1[i - 1] * p1) % m1;
            pow2[i] = (pow2[i - 1] * p2) % m2;
        }
        h1[0] = h2[0] = s[0];
        for (int i = 1; i < n; i++) {
            h1[i] = (h1[i - 1] * p1 + s[i]) % m1;
            h2[i] = (h2[i - 1] * p2 + s[i]) % m2;
        }
    }
};
