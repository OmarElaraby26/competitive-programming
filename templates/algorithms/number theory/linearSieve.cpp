// this code is taken from codeforces
// the author of this code is Neal

vector<int> smallest_factor;
vector<short> smallest_power;
vector<short> mobius;
vector<bool> mark;
vector<int> primes;

// Note: this sieve is O(n).
void sieve(int maximum) {
    maximum = max(maximum, 1);
    smallest_factor.assign(maximum + 1, 0);
    smallest_power.assign(maximum + 1, 0);
    mobius.assign(maximum + 1, 0);
    mark.assign(maximum + 1, true);
    mobius[1] = 1;
    mark[0] = mark[1] = false;
    primes = {};

    for (int i = 2; i <= maximum; i++) {
        if (smallest_factor[i] == 0) { // mark[i] == true
            smallest_factor[i] = i;
            primes.push_back(i);
            smallest_power[i] = 1;
            mobius[i] = -1;
        }

        for (int p : primes) {
            if (p > smallest_factor[i] || i * p > maximum)
                break;

            smallest_factor[i * p] = p;
            mark[i * p] = false;
            smallest_power[i * p] = smallest_factor[i] == p ? short(smallest_power[i] + 1) : 1;
            mobius[i * p] = smallest_factor[i] == p ? 0 : short(-mobius[i]);
        }
    }
}
