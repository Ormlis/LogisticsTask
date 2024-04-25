#include "testlib.h"

using namespace std;

int main(int argc, char *argv[]) {
    registerTestlibCmd(argc, argv);

    int t = inf.readInt();
    for (int test_case = 1; test_case <= t; ++test_case) {
        setTestCase(test_case);

        int k = inf.readInt();
        int n = inf.readInt();

        vector<vector<bool>> g(n, vector<bool>(k));
        for(int i = 0; i < n; ++i) {
            int m = inf.readInt();
            for(int j = 0; j < m; ++j) {
                int gate = inf.readInt() - 1;
                g[i][gate] = true;
            }
        }

        auto validate_answer = [&](InStream &in) -> int {
            int l = in.readInt(1, n, "answer");

            vector<int> taken(k);
            vector<bool> ok(n);

            for(int i = 1; i <= l; ++i) {
                int c = in.readInt(1, min(n, k), "c_i");
                for(int j = 0; j < c; ++j) {
                    int f = in.readInt(1, n, "f") - 1;
                    int gate = in.readInt(1, k, "g") - 1;

                    if (!g[f][gate]) {
                        in.quitf(_wa, "truck f=%d can't come to gate g=%d", f+1, gate+1);
                    }
                    if (taken[gate] == i) {
                        in.quitf(_wa, "multiple uses of gate g=%d in hour %d", gate+1, i+1);
                    }
                    if (ok[f]) {
                        in.quitf(_wa, "multiple uses of truck f=%d", f+1);
                    }

                    taken[gate] = i;
                    ok[f] = true;
                }
            }

            for(int i = 0; i < n; ++i) {
                if (!ok[i]) {
                    in.quitf(_wa, "truck f=%d was not used", i);
                }
            }

            return l;
        };

        int jury = validate_answer(ans);
        int participant = validate_answer(ouf);

        if (jury < participant) {
            quitf(_wa, "Jury has found an answer less than participant.");
        }
        if (jury > participant) {
            quitf(_fail, "Participant has found an answer less than jury.");
        }
    }
    quitf(_ok, "Everything is ok!");

    return 0;
}
