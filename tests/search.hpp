#include "../include/search.hpp"
#include "../include/constants.hpp"
#include "tConsts.hpp"
namespace n_sch {
using namespace n_brd;
using namespace n_consts;
    bool _testDepthZero() {
        board b;
        stop_token st;
        bool ret = _negamax(b, 0, 0, 0, st) == b.eval();
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
        if (!ret) cout << s_eval << "\n";
        return ret;
    }

    bool _testSearchEven() {
        board b;
        b.loadFen(sv_fenStart);
        bool ret = true;
        for (s64 s_depth : {0, 2, 4/*, 6*/}) {
            s64 s_res = _searchDepth(b, s_depth);
            if (s_res != 0) cout << "search result: " << s_res << '\n';
            ret &= (s_res == 0);
        }
        string msg = "TEST SEARCH EVEN ";
        tConsts::eval(ret, msg);
        return ret;
    }

    bool tests() {
        return 
            _testDepthZero() &&
            _testSearchInit() &&
            _testSearchEven();
    }
}
