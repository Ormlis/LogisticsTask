#include "testlib.h"

using namespace std;

const int N = 300;
const int K = 300;
const int M = 900;
const int T = 200;

const int SumN = 900;
const int SumK = 900;
const int SumM = 900;

int main(int argc, char *argv[]) {
    registerValidation(argc, argv);

    int t = inf.readInt(1, T, "t");
    inf.readEoln();

    int sum_k = 0;
    int sum_n = 0;
    int sum_m = 0;

    for (int test_case = 1; test_case <= t; ++test_case) {
        setTestCase(test_case);

        int k = inf.readInt(1, K, "k");
        inf.readSpace();
        int n = inf.readInt(1, N, "n");
        inf.readEoln();

        sum_k += k;
        sum_n += n;

        ensuref(sum_k <= SumK, "Sum of k exceeds %d.", SumK);
        ensuref(sum_n <= SumN, "Sum of n exceeds %d.", SumN);

        int m = 0;
        for (int i = 1; i <= n; ++i) {
            int m_i = inf.readInt(1, k, "m_i");
            inf.readSpace();

            m += m_i;
            ensuref(m <= M, "Sum of m_i exceeds %d.", M);

            vector<int> a = inf.readInts(m_i, 1, k, "a_ij");
            inf.readEoln();

            sort(a.begin(), a.end());
            ensuref(unique(a.begin(), a.end()) == a.end(), "a_{i, j} are not unique for i=%d", i);
        }

        sum_m += m;
        ensuref(sum_n <= SumM, "Sum of m exceeds %d.", SumM);
    }

    inf.readEof();
    return 0;
}
