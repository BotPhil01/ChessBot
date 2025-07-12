#include "../include/search.hpp"
#include "../include/constants.hpp"
#include "../include/nodectr.hpp"
#include "tConsts.hpp"
namespace n_sch {
using namespace n_brd;
using namespace n_consts;
    // bool _testDepthZero() {
    //     board b;
    //     stop_token st;
    //     bool ret = _negamax(b, 0, 0, 0, st) == b.eval();
    //     string msg = "TEST SEARCH DEPTH ZERO ";
    //     tConsts::eval(ret, msg);
    //     return ret;
    // }
    
    bool _testSearchInit() {
        board b;
        b.loadFen(sv_fenStart);
        // cout << "running search\n";
        s64 s_eval = search(b);
        // cout << "terminating search\n";
        bool ret = s_eval == 0;
        string msg = "TEST SEARCH INIT ";
        tConsts::eval(ret, msg);
        if (!ret) cout << s_eval << "\n";
        cout << "final count" << n_ctr::au_ctr << "\n";
        // cout << "exiting test search init\n";
        return ret;
    }

    bool _testSearchWhite() {
        board b;
        b.loadFen("3k4/8/8/8/8/8/8/6QK w - - 0 1");

        s64 s_eval = search(b);

        bool ret = s_eval > 0;
        string msg = "TEST SEARCH WHITE ";
        
        tConsts::eval(ret, msg);
        if (!ret) cout << s_eval << "\n";
        cout << "final count" << n_ctr::au_ctr << "\n";
        // cout << "exiting test search init\n";
        return ret;
    }

    bool _testSearchBlack() {
        board b;
        b.loadFen("3K4/8/8/8/8/8/8/6qk w - - 0 1");

        s64 s_eval = search(b);

        bool ret = s_eval < 0;
        string msg = "TEST SEARCH BLACK ";
        
        tConsts::eval(ret, msg);
        if (!ret) cout << s_eval << "\n";
        cout << "final count" << n_ctr::au_ctr << "\n";
        // cout << "exiting test search init\n";
        return ret;
    }

    bool tests() {
        // cout << "entering search tests\n";
        return _testSearchInit()
            && _testSearchWhite()
            &&_testSearchBlack();
            // _testDepthZero() &&
            // _testSearchEven();
    }
}
