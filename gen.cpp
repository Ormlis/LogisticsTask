#include "testlib.h"
#include <assert.h>
#include <algorithm>
#include <numeric>

using namespace std;



const int N = 300;
const int K = 300;
const int M = 900;
const int T = 200;

const int SumN = 900;
const int SumK = 900;
const int SumM = 900;

struct TestCase {
    int n, k;
    vector<vector<int>> graph;
    int m;

    TestCase(int n = 0, int k = 0) : n(n), k(k), m(0) {
        graph.resize(n);
    }

    void add_edge(int from, int to) {
        graph[from].emplace_back(to);
        m++;
    }

    void shuffle_edges() {
        for (int i = 0; i < n; ++i) {
            shuffle(graph[i].begin(), graph[i].end());
        }
    }

    void shuffle_left() {
        shuffle(graph.begin(), graph.end());
    }

    void shuffle_right() {
        vector<int> order(k);
        iota(order.begin(), order.end(), 0);
        shuffle(order.begin(), order.end());
        for (int i = 0; i < n; ++i) {
            for (auto &j: graph[i]) {
                j = order[j];
            }
        }
    }

    void shuffle_all() {
        shuffle_left();
        shuffle_right();
        shuffle_edges();
    }

    bool validate() const {
        bool ok = true;
        for (int i = 0; i < n; ++i) ok &= !graph[i].empty();
        return ok;
    }
};

vector<TestCase> tests;

void GenAll() {
    int sum_m = 0;
    int sum_k = 0;
    for (int n = 1; (int) tests.size() < T && sum_m + n <= SumM; ++n) {
        for (int k = 1; k <= n && (int) tests.size() < T && sum_k + k <= SumK && sum_m + n <= SumM; ++k) {
            vector<pair<int, int>> edges;
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < k; ++j) {
                    edges.emplace_back(i, j);
                }
            }
            int sz = (int) edges.size();
            for (int mask = 0; mask < (1 << sz) && sum_k + k <= SumK; ++mask) {
                int m = __builtin_popcount(mask);
                if (m < n || sum_m + m > SumM) continue;
                TestCase test(n, k);
                for (int i = 0; i < sz; ++i) {
                    if (mask & (1 << i)) {
                        test.add_edge(edges[i].first, edges[i].second);
                    }
                }
                assert(test.m == m);
                if (test.validate()) {
                    tests.push_back(test);
                    sum_m += m;
                    sum_k += k;
                }
            }
        }
    }
    if (tests.size() > T) tests.resize(T);
}

void GenRandom() {
    int n1 = opt<int>("n", -1);
    int k1 = opt<int>("k", -1);
    int m1 = opt<int>("m", -1);
    int sum_m = 0;
    int sum_k = 0;
    for (int _ = 0; _ < T && sum_m < SumM && sum_k < SumK; _++) {
        int n, m, k;
        if (n1 == -1) {
            m = rnd.next(1, min(SumM - sum_m, M));
            n = rnd.next(1, min(m, N));
        } else {
            n = n1;
            m = m1;
        }
        if (k1 == -1) {
            k = rnd.next(1, min(SumK - sum_k, n));
        } else {
            k = k1;
        }
        m = min(m, n * k);

        sum_m += m;
        sum_k += k;

        TestCase test(n, k);
        vector<pair<int, int>> edges;
        for (int i = 0; i < n; ++i) {
            int one = rnd.next(k);
            test.add_edge(i, one);
            for (int j = 0; j < k; ++j) {
                if (j != one) {
                    edges.emplace_back(i, j);
                }
            }
        }
        shuffle(edges.begin(), edges.end());
        edges.resize(m - n);
        for (auto &[i, j]: edges) {
            test.add_edge(i, j);
        }
        assert(test.validate());
        tests.push_back(test);
    }
}

void GenCycles() {
    int n = opt<int>("n", 10);
    int m = opt<int>("m", n * 2);
    int k = opt<int>("k", n / 3);
    int add = opt<int>("add", 0);
    int t = min({SumM / (m + add), SumK / k, SumN / n});
    t = min(t, T);
    for (int _ = 0; _ < t; ++_) {
        assert(n > 1);
        TestCase test(n, k);
        vector<vector<bool>> g(n, vector<bool>(k));

        int need = m;
        auto add_valid = [&](int i, int j) {
            if (g[i][j]) return;
            need--;
            test.add_edge(i, j);
            g[i][j] = true;
        };

        for (int i = 0; i < n; ++i) {
            int j = rnd.next(k);
            add_valid(i, j);
            add_valid((i + 1) % n, j);
        }

        while (need > 0) {
            int len = rnd.next(1, max(1, need / 2));
            vector<int> a(len), b(len);
            for (int i = 0; i < len; ++i) {
                a[i] = rnd.next(n);
                b[i] = rnd.next(k);
            }
            for (int i = 0; i < len; ++i) {
                add_valid(a[i], b[i]);
                add_valid(a[(i + 1) % len], b[i]);
            }
        }
        need += add;
        while (need > 0) {
            add_valid(rnd.next(n), rnd.next(k));
        }

        tests.push_back(test);
    }
}

void GenBigAns() {
    int n = opt<int>("n", 10);
    int good = opt<int>("good", 3);
    int deg = opt<int>("deg", 1);

    assert(deg <= good);
    int t = min({SumN / n, SumK / good, SumM / (n * deg)});
    int k = min(SumK / t, K);

    for (int _ = 0; _ < t; ++_) {
        TestCase test(n, k);

        for (int i = 0; i < n; ++i) {
            vector<int> sec(good);
            iota(sec.begin(), sec.end(), 0);
            shuffle(sec.begin(), sec.end());
            sec.resize(rnd.next(1, deg));
            for (auto &j: sec) {
                test.add_edge(i, j);
            }
        }

        tests.push_back(test);
    }
}

void PostProcess() {
    int sh = opt<int>("shuffle", 7);
    for (auto &test: tests) {
        if (sh & 1) test.shuffle_left();
        if (sh & 2) test.shuffle_right();
        if (sh & 4) test.shuffle_edges();
    }
}

void PrintTestCases() {
    cout << tests.size() << '\n';
    for (auto &test: tests) {
        cout << test.k << ' ' << test.n << '\n';
        for (int i = 0; i < test.n; ++i) {
            cout << test.graph[i].size();
            for (auto &j: test.graph[i]) {
                cout << ' ' << j + 1;
            }
            cout << '\n';
        }
    }
}

int main(int argc, char *argv[]) {
    registerGen(argc, argv, 1);
    auto mode = opt<string>("mode", "random");

    if (mode == "all") {
        GenAll();
    } else if (mode == "random") {
        GenRandom();
    } else if (mode == "cycle") {
        GenCycles();
    } else if (mode == "big") {
        GenBigAns();
    }

    PostProcess();
    PrintTestCases();

    return 0;
}
