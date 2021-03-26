template<typename T>
T combine(const T &a, const T &b) { // combine here = min(a, b)
	return a < b ? a : b;
}

template<typename T, T (*combine)(const T&, const T&)>
class Sparse_table {
private:
	vector<vector<T> > table;
	template<typename TT>
	void build_sparse_table(const TT &v) { // TT = vector<T> or deque<T>
		int idx = 0; // for 0 based array
		for (const T &val : v) {
			table[0][idx] = val;
		}

		for (int row = 1; row < (int) table.size(); ++row) {
			const int c_range = (1 << row);
			for (int col = 0; col + c_range - 1 < (int) table[row].size();
					++col) {
				table[row][col] = combine(table[row - 1][col],
						table[row - 1][col + (c_range >> 1)]);
			}
		}
	}

public:
	template<typename TT>
	Sparse_table(const TT &v) { // TT = vector<T> or deque<T>
		table.assign(__lg(v.size()) + 1, vector<T>(v.size()));
		if (v.size()) build_sparse_table(v);
	}

	T query_o1(int l, int r) const { // overlap friendly
		if (l > r) swap(l, r);
		int msb = __lg(r - l + 1), s_range = (1 << msb);
		return combine(table[msb][l], table[msb][r - s_range + 1]);
	}

	T query_ologn(int l, int r) const { // NON overlap friendly functions as +, * ..
		if (l > r) swap(l, r);
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
