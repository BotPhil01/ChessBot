#include "../include/stack.hpp"
#include "../include/types.hpp"

#include <sys/resource.h>
#include <stdio.h>
namespace n_stk {
    void stIncrease() {
        const rlim_t rlStack =  1024UL * 1024UL * 1024UL * 1000; // 8kb
        struct rlimit rl;

        u32 result = getrlimit(RLIMIT_STACK, &rl);
        if (result == 0) {
            if (rl.rlim_cur < rlStack) {
                rl.rlim_cur = rlStack;
                result = setrlimit(RLIMIT_STACK, &rl);
                if (result != 0) 
                    fprintf(stderr, "setrlimit returned result = %d\n", result);
            }
        }
    }
}
