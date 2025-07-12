#include "tConsts.hpp"
#include "../include/nodectr.hpp"
#include <thread>

namespace n_ctr {
    using namespace std;

    bool ret = false;
    string msg = "TEST CTR ";

    void inc() {
        au_ctr++;
    }

    bool tset() {
        msg = "TEST NCTR SET ";
        int u = au_ctr;

        au_ctr = 2;
        ret = au_ctr == 2 && u == 0;
        au_ctr = 0;
        tConsts::eval(ret, msg);
        return ret;
    }

    bool pref() {
        msg = "TEST NCTR PREF ";
        au_ctr = 0;
        au_ctr++;
        thread t1(inc);
        t1.join();
        ret = (au_ctr == 2);
        au_ctr = 0;
        tConsts::eval(ret, msg);
        return ret;
    }

    bool posf() {
        msg = "TEST NCTR POSF ";
        au_ctr++;
        thread t1(inc);
        t1.join();
        ret = (au_ctr == 2);
        au_ctr = 0;
        tConsts::eval(ret, msg);
        return ret;
    }

    bool pos() {
        msg = "TEST NCTR POS ";
        thread t1(inc);
        t1.join();
        au_ctr++;
        ret = (au_ctr == 2);
        au_ctr = 0;
        tConsts::eval(ret, msg);
        return ret;
    }

    bool dub() {
        msg = "TEST NCTR POS ";
        thread t1(inc);
        thread t2(inc);
        t1.join();
        t2.join();
        ret = (au_ctr == 2);
        tConsts::eval(ret, msg);
        return ret;
    }

    bool tests() {
        return 
            tset() &&
            pref() &&
            posf() &&
            pos() &&
            dub();
    }
}
