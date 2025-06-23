#include <string>
#include <iostream> 

#ifndef TCONSTS_H
#define TCONSTS_H
namespace tConsts {
    using namespace std;
    const string s = "SUCCESS";
    const string f = "FAILED";
    void succ(const string msg) {
        cout << msg << s << endl;
    }
    void fail(const string msg) {
        cout << msg << f << endl;
    }
    void eval(const bool b, const string msg) {
        if (b) {
            succ(msg);
        } else {
            fail(msg);
        }
    }
}
#define eval tConsts::eval
#endif
