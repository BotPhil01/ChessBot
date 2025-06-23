#include <iostream>
#include "../include/types.hpp"
namespace n_prt {
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
separator:
        cout << ", ";
increment:
        i++;
        mask = mask << 1;
        if (i % 8 == 0 && i != 0) {
            goto endcheck;
        }
endline:
        cout << "\n";
        goto endcheck;
endcheck:
        if (i == 64) {
            return;
        }
        goto operation;
    }

    void bprint(const n_types::board b) {
        cout << b.str() << endl;
    }
}
