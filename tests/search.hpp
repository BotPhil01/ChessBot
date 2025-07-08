#include "../include/search.hpp"
#include "../include/constants.hpp"
#include "tConsts.hpp"
namespace n_sch {
using namespace n_brd;
using namespace n_consts;
    bool _testDepthZero() {
        board b;
        bool ret = _negamax(b, 0, 0, 0) == b.eval();
        string msg = "TEST SEARCH DEPTH ZERO ";
        tConsts::eval(ret, msg);
        return ret;
    }
    
    bool _testSearchInit() {
        board b;
        b.loadFen(sv_fenStart);
        s64 s_eval = search(b);
        bool ret = s_eval == 0;
        string msg = "TEST SEARCH INIT ";
        tConsts::eval(ret, msg);
        // cout << s_eval << "\n";
        return ret;
    }

    bool _testSearchEven() {
        board b;
        b.loadFen(sv_fenStart);
        bool ret = true;
        for (s64 s_depth : {0, 2, 4, 6}) {
            s64 s_res = _searchDepth(b, s_depth);
            cout << s_res << '\n';
            ret &= (s_res == 0);
        }
        string msg = "TEST SEARCH EVEN ";
        tConsts::eval(ret, msg);
        return ret;
    }

    bool tests() {
        return 
            _testSearchInit() &&
            _testSearchEven() &&
            _testDepthZero();
    }
}
