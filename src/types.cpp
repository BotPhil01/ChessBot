#include "../include/types.hpp"

namespace n_types {
    string_view col2string(const colour c) {
        if (c == white) {
            return "white";
        }
        return "black";
    }

    string square2string(const square b) {
        if (!b) {
            return "none";
        }
        bitboard mask = 1;
        u8 row = 0;
        u8 col = 0;
        for (; row < 8; row++) {
            for (col = 0; col < 8; col++) {
                if ((b & mask) != 0) {
                    goto escape;
                }
                mask = mask << 1;
            }
        }
escape:
        string s = "";
        s += (char) ('a' + col);
        s += (char) ('1' + row);
        return s;
    }

    string piece2string(const piece p) {
        char c = 0;
        switch (p) {
            case PAWN:
                c = 'p';
                break;
            case ROOK:
                c = 'r';
                break;
            case KNIGHT:
                c = 'n';
                break;
            case BISHOP :
                c = 'b';
                break;
            case QUEEN :
                c = 'q';
                break;
            case KING :
                c = 'k';
                break;
            case EMPTY:
                c = 'x';
                break;
        }
        return string() + c;
    }
}
