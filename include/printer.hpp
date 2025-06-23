#include "types.hpp"
#include <iostream>
#ifndef PRINTER_HPP
#define PRINTER_HPP
namespace n_prt {
    void cprint(const char cVal);
    template<class Iterator>
    void iprint(const Iterator i_start, const Iterator i_end) {
        for (auto i = i_start; i != i_end; i++) {
            cout << (int) *i << ", ";
        }
        cout << endl;
    }

    void bbprint(const bitboard bb);
    void bprint(const n_types::board b);
}
#endif
