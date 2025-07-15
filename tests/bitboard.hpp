#pragma once
#include "../include/types.hpp"
#include "../include/bitboard.hpp"
#include "./tConsts.hpp"
#include <vector>

#pragma once
namespace n_bb {
    using namespace tConsts;
    bool ret = true;
    string msg = "";
    const bitboard b_empty = 0x0;
    const bitboard b_unary = 0x1;
    const bitboard b_full = 0xffffffffffffffff;
    const bitboard b_light = 0X55aa55aa55aa55aa;
    const bitboard b_dark = 0xaa55aa55aa55aa55;
    const bitboard b_diagAHP = 0x8040201008040201; 
    const bitboard b_diagAHN = 0x0102040810204080;
    const std::vector<bitboard> bitboards = {
        b_empty,
        b_unary,
        b_full,
        b_light,
        b_dark,
        b_diagAHP, 
        b_diagAHN,
    };

    bool testPopCount() {
        u8 sum = 0;
        sum += n_bbd::bitPopCount(b_empty); // 0
        sum += n_bbd::bitPopCount(b_unary); // 1
        sum += n_bbd::bitPopCount(b_full); // 64
        sum += n_bbd::bitPopCount(b_light); // 32
        sum += n_bbd::bitPopCount(b_dark); // 32
        sum += n_bbd::bitPopCount(b_diagAHP); // 8
        sum += n_bbd::bitPopCount(b_diagAHN); // 8
        ret = sum == (1+64+32+32+8+8);
        msg = "TEST POP COUNT ";
        eval(ret, msg);
        return ret;
    }

    bool testBitIsolate() {
        ret = true;
        for (auto it = bitboards.begin(); it != bitboards.end(); it++) {
            const bitboard b_cur = *it;
            const bitboard b_isolated = n_bbd::bitIsolate(b_cur);
            if (b_cur == b_diagAHN) {
                if (b_isolated != 128) {
                    ret = false;
                    break;
                }
            } else if (b_cur == b_light) {
                if (b_isolated != 2) {
                    ret = false;
                    break;
                }
            } else if (b_cur == b_empty) {
                if (b_isolated != 0) {
                    ret = false;
                    break;
                }
            } else {
                if (b_isolated != 1) {
                    ret = false;
                    break;
                }
            }
        }
        msg = "TEST BIT ISOLATE ";
        eval(ret, msg);
        return ret;
    }

    bool testShiftOne() {
        ret = true;
        for (int i = 0; i < n_consts::a_dirs.size(); i++) {
            const n_types::dir d = n_consts::a_dirs[i];
            const bitboard mask = n_consts::shiftMasks[i];
            const bitboard shifted = n_bbd::shiftOne(d, b_full);
            if (shifted != mask) {
                ret = false;
                cout << " " << i << " " << hex << mask << " " << shifted << " ";
                break;
            }
        }
        msg = "TEST SHIFT ONE ";
        eval(ret, msg);
        return ret;
    }

    bool tests() {
        return testPopCount() && testBitIsolate() && testShiftOne();
    }
}
