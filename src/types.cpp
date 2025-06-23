#include "../include/types.hpp"

namespace n_types {
    string_view col2string(const colour c) {
        if (c == white) {
            return "white";
        }
        return "black";
    }

    string square2string(const bitboard b) {
        if (!b) {
            return "none";
        }
        bitboard mask = 1;
        u8 row = 0;
        u8 col = 0;
        for (; row < 8; row++) {
            for (; col < 8; col++) {
                if ((b & mask) != 0) {
                    goto escape;
                }
            }
        }
escape:
        string s = "";
        s += (char) ('a' + col);
        s += (char) ('0' + row);
        return s;
    }
}
