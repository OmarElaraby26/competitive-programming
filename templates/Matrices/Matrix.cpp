#define mat_T long long // matrix type
typedef vector<vector<mat_T>> Matrix;
void mat_assign(Matrix &a, const int rowSz, const int colSz,
		const mat_T val = 0) {
	a.assign(rowSz, vector<mat_T>(colSz, val));
}

Matrix mat_multi(const Matrix &a, const Matrix &b, const ll mod = 0) {
	if (!a.size()) { // empty
		assert(!b.size()); // must be empty too
		return a; // empty matrices
	}
	else assert(a[0].size() == b.size()); // a_c must == b_r

	Matrix ans(a.size(), vector<mat_T>(b[0].size()));
	// ans = a * b
	for (int i = 0; i < (int) ans.size(); ++i) {
		for (int k = 0; k < (int) b.size(); ++k) {
			if (a[i][k]) {
				for (int j = 0; j < (int) ans[0].size(); ++j) {
					ans[i][j] += (a[i][k] * b[k][j]);
					if (mod) ans[i][j] %= mod;
				}
			}
		}
	}
	return ans;
}

Matrix mat_power(Matrix a, ll b, const ll mod = 0) {
	if (!a.size()) return a; // empty matrix!

	assert(a.size() == a[0].size()); // MUST be square matrix

	Matrix ans;
	mat_assign(ans, a.size(), a[0].size());
	for (int i = 0; i < (int) ans.size(); ++i) {
		ans[i][i] = 1;
	}

	while (b) {
		if (b & 1) ans = mat_multi(ans, a, mod);
		a = mat_multi(a, a, mod);
		b >>= 1;
	}

	return ans;
}

void mat_print(const Matrix &a) {
	for (int i = 0; i < (int) a.size(); ++i) {
		for (int j = 0; j < (int) a[i].size(); ++j) {
			cout << a[i][j];
			if (j + 1 < (int) a[i].size()) cout << ' ';
		}
		cout << '\n';
	}
}

long double mat_det(const Matrix &aaa) { //  Determinant by Gauss
	// must be square matrix
	if (!aaa.size() || aaa.size() != aaa[0].size()) return 0;
//	assert(aaa.size() == aaa[0].size()); // must be square

// that is if 'aaa' not double matrix
	vector<vector<long double>> a(aaa.size(),
			vector<long double>(aaa[0].size()));
	for (int i = 0; i < (int) a.size(); ++i) {
		for (int j = 0; j < (int) a[i].size(); ++j) {
			a[i][j] = aaa[i][j];
		}
	}

	const int n = a.size();
	long double det = 1;
	for (int i = 0; i < n; ++i) {
		int k = i;
		for (int j = i + 1; j < n; ++j) {
			if (abs(a[j][i]) > abs(a[k][i])) k = j;
		}
		if (abs(a[k][i]) < EPS) {
			det = 0;
			break;
		}
		swap(a[i], a[k]);
		if (i != k) det = -det;
		det *= a[i][i];
		for (int j = i + 1; j < n; ++j) {
			a[i][j] /= a[i][i];
		}
		for (int j = 0; j < n; ++j) {
			if (j != i && abs(a[j][i]) > EPS) {
				for (int k = i + 1; k < n; ++k)
					a[j][k] -= (a[i][k] * a[j][i]);
			}
		}
	}

	return det;
}

int mat_rank(const Matrix &aaa) {
	if (!aaa.size()) return 0; // empty matrix

	// that is if 'aaa' not double matrix
	vector<vector<long double>> a(aaa.size(),
			vector<long double>(aaa[0].size()));
	for (int i = 0; i < (int) a.size(); ++i) {
		for (int j = 0; j < (int) a[i].size(); ++j) {
			a[i][j] = aaa[i][j];
		}
	}

	const int n = a.size();
	const int m = a[0].size();

	int rank = 0;
	vector<bool> row_selected(n, false);
	for (int i = 0; i < m; ++i) {
		int j;
		for (j = 0; j < n; ++j) {
			if (!row_selected[j] && abs(a[j][i]) > EPS) break;
		}

		if (j != n) {
			++rank;
			row_selected[j] = true;
			for (int p = i + 1; p < m; ++p)
				a[j][p] /= a[j][i];
			for (int k = 0; k < n; ++k) {
				if (k != j && abs(a[k][i]) > EPS) {
					for (int p = i + 1; p < m; ++p)
						a[k][p] -= (a[j][p] * a[k][i]);
				}
			}
		}
	}
	return rank;
}
