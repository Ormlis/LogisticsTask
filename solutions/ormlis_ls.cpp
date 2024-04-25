#include "bits/stdc++.h"

using namespace std;

void solve() {
    int k, n;
    cin >> k >> n;
    vector<vector<int>> g(n);

    for(int i = 0; i < n; ++i) {
        int m; cin >> m;
        g[i].resize(m);
        for(auto &x : g[i]) {
            cin >> x;
            x--;
        }
    }

    auto check = [&] (int l, bool print = false) {
        vector<int> mt(k * l, -1);
        vector<int> used(n);
        int T = 0;
        auto kuhn = [&] (auto &&self, int v) {
            if (used[v] == T) return false;
            used[v] = T;
            for(auto &u : g[v]) {
                for(int t = u; t < k * l; t += k) {
                    if (mt[t] == -1) {
                        mt[t] = v;
                        return true;
                    }
                }
            }
            for(auto &u : g[v]) {
                for(int t = u; t < k * l; t += k) {
                    if (self(self, mt[t])) {
                        mt[t] = v;
                        return true;
                    }
                }
            }
            return false;
        };
        for(int i = 0; i < n; ++i) {
            T++;
            if (!kuhn(kuhn, i)) return false;
        }
        if (!print) return true;
        vector<vector<pair<int, int>>> answer(l);
        for(int i = 0; i < k * l; ++i) {
            if (mt[i] != -1) {
                answer[i / k].emplace_back(mt[i], i % k);
            }
        }
        cout << l << '\n';
        for(int i = 0; i < l; ++i) {
            cout << answer[i].size() << '\n';
            for(auto &[fura, gate] : answer[i]) {
                cout << fura + 1 << ' ' << gate + 1 << '\n';
            }
        }
        return true;
    };

    int ans = 1;
    while (!check(ans, true)) ans++;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    int t = 1;
    cin >> t;
    while (t--) {
        solve();
    }
    return 0;
}