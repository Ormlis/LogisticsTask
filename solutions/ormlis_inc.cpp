#include "bits/stdc++.h"

using namespace std;

void solve() {
    int k, n;
    cin >> k >> n;
    vector<vector<int>> g(n);

    for (int i = 0; i < n; ++i) {
        int m;
        cin >> m;
        g[i].resize(m);
        for (auto &x: g[i]) {
            cin >> x;
            x--;
        }
    }

    vector<int> mt(k * n, -1);
    vector<int> used(n);
    vector<bool> ok(n);

    int l = 1;
    for (;; ++l) {
        int T = 0;
        auto kuhn = [&](auto &&self, int v) {
            if (used[v] == T) return false;
            used[v] = T;
            for (int h = 0; h < l; ++h) {
                for (auto &u: g[v]) {
                    int t = u + h * k;
                    if (mt[t] == -1 || self(self, mt[t])) {
                        mt[t] = v;
                        return true;
                    }
                }
            }
            return false;
        };
        for (int i = 0; i < n; ++i) {
            if (ok[i]) continue;
            T++;
            ok[i] = kuhn(kuhn, i);
        }
        if (count(ok.begin(), ok.end(), true) == n) {
            break;
        }
    }

    vector<vector<pair<int, int>>> answer(l);
    for (int i = 0; i < k * l; ++i) {
        if (mt[i] != -1) {
            answer[i / k].emplace_back(mt[i], i % k);
        }
    }
    cout << l << '\n';
    for (int i = 0; i < l; ++i) {
        cout << answer[i].size() << '\n';
        for (auto &[fura, gate]: answer[i]) {
            cout << fura + 1 << ' ' << gate + 1 << '\n';
        }
    }
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