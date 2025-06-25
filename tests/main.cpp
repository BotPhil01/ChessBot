#include "../include/magics.hpp"
#include "bitboard.hpp"
#include "board.hpp"
#include <sys/resource.h>
int main() {
    // u32 res = 0;
    // res += n_bb::tests();
    // res += n_b::tests();
    const rlim_t rl_stackSize = 1024 * 1024 * 64;
    struct rlimit rl;
    int res;
    res = getrlimit(RLIMIT_STACK, &rl);
    if (res == 0) {
        cout << "current limit: " << rl.rlim_cur << endl;
        if (rl.rlim_cur < rl_stackSize) {
            rl.rlim_cur = rl_stackSize;
            res = setrlimit(RLIMIT_STACK, &rl);
            if (res != 0) {
                fprintf(stderr, "setrlimit returned result = %d\n", res);
                return 1;
            }
            cout << "current limit: " << rl.rlim_cur << endl;
        }
    }
    n_b::tests();
}

