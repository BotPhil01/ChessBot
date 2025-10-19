#include <iostream>
#include "../include/types.hpp"
#include "../include/board.hpp"

namespace n_prt {
    using namespace n_types;
    using namespace n_brd;
    void cprint(const char cVal) {
        cout << cVal << endl;
    }

    void bbprint(const bitboard bb) {
        u8 i = 0;
        bitboard mask = 1;
operation:
        if ((bb & mask) != 0) {
            cout << 1;
        } else {
            cout << 0;
        }
        cout << ", ";
        i++;
        mask = mask << 1;
        if (i % 8 == 0 && i != 0) {
            goto endcheck;
        }
        cout << "\n";
        goto endcheck;
endcheck:
        if (i == 64) {
            return;
        }
        goto operation;
    }

    void bprint(board b) {
        cout << b.str() << endl;
    }

    void mprint(const cMove m) {
        cout << m.str() << endl;
    }
}
