// atomic node counter to count nodes searched by search algo
#include "types.hpp"

#pragma once
namespace n_ctr {
    namespace {
        au64 _au_val {0};
    }
    class nctr {
        public:
            nctr operator++(int) {
                _au_val++;
                return *this;
            }
            nctr operator++() {
                nctr c_old = *this;
                _au_val++;
                return c_old;
            }

        void operator=(u64 u) {
            _au_val = u;
        }

        inline bool operator==(u64 u) {
            return _au_val.load() == u;
        }
        friend ostream& operator<<(ostream &os, nctr v);
    };
}
