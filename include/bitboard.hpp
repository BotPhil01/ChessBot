#include "types.hpp"
#include "constants.hpp"

#include <memory>
#include <vector>

#pragma once
namespace n_bbd {
    using namespace n_consts;
    using namespace n_types;

    // isolates the least significant bit
    constexpr bitboard bitIsolate(const bitboard bb) {
        return bb & -bb;
    }

    constexpr u8 bitPopCount(const bitboard bb) {
        // generated with _createPopLoookup
        u8 u_ret = 0;
        const array<u8, 256> a_popLookup = {
            0, 1, 1, 2, 1, 2, 2, 3,
            1, 2, 2, 3, 2, 3, 3, 4,
            1, 2, 2, 3, 2, 3, 3, 4,
            2, 3, 3, 4, 3, 4, 4, 5,
            1, 2, 2, 3, 2, 3, 3, 4,
            2, 3, 3, 4, 3, 4, 4, 5,
            2, 3, 3, 4, 3, 4, 4, 5,
            3, 4, 4, 5, 4, 5, 5, 6,
            1, 2, 2, 3, 2, 3, 3, 4,
            2, 3, 3, 4, 3, 4, 4, 5,
            2, 3, 3, 4, 3, 4, 4, 5,
            3, 4, 4, 5, 4, 5, 5, 6,
            2, 3, 3, 4, 3, 4, 4, 5,
            3, 4, 4, 5, 4, 5, 5, 6,
            3, 4, 4, 5, 4, 5, 5, 6,
            4, 5, 5, 6, 5, 6, 6, 7,
            1, 2, 2, 3, 2, 3, 3, 4,
            2, 3, 3, 4, 3, 4, 4, 5,
            2, 3, 3, 4, 3, 4, 4, 5,
            3, 4, 4, 5, 4, 5, 5, 6,
            2, 3, 3, 4, 3, 4, 4, 5,
            3, 4, 4, 5, 4, 5, 5, 6,
            3, 4, 4, 5, 4, 5, 5, 6,
            4, 5, 5, 6, 5, 6, 6, 7,
            2, 3, 3, 4, 3, 4, 4, 5,
            3, 4, 4, 5, 4, 5, 5, 6,
            3, 4, 4, 5, 4, 5, 5, 6,
            4, 5, 5, 6, 5, 6, 6, 7,
            3, 4, 4, 5, 4, 5, 5, 6,
            4, 5, 5, 6, 5, 6, 6, 7,
            4, 5, 5, 6, 5, 6, 6, 7,
            5, 6, 6, 7, 6, 7, 7, 8, 
        };

        for (u8 i = 0; i < 8; i++) {
            u8 u_index = (bb >> (8 * i)) & 0xff;
            u_ret += a_popLookup[u_index];
        }
        return u_ret;
    }

    
    // array<u8, 256> const _createPopLookup();
    
    constexpr bitboard shiftOne(const n_types::dir d, const bitboard bb) {
        const bitboard mask = n_consts::shiftMasks[n_consts::dirIndex(d)];
        if (d < 0) {
            return bb >> -d & mask;
        }
        return bb << d & mask;
    }

    constexpr bitboard shift(const dir d, const bitboard bb, const u32 u_n) {
        if (u_n == 0) {
            return bb;
        }
        return shift(d, shiftOne(d, bb), u_n-1);
    }

    constexpr bitboard mirror(bitboard bb) {
        return  (bb << 56) |
            ((bb << 40) & 0x00ff000000000000) |
            ((bb << 24) & 0x0000ff0000000000) |
            ((bb << 8) & 0x000000ff00000000) |
            ((bb >> 8) & 0x00000000ff000000) |
            ((bb >> 24) & 0x0000000000ff0000) |
            ((bb >> 40) & 0x000000000000ff00) |
            ((bb >> 56) );
    }

    constexpr bitboard rotate(bitboard bb) {
        const u64 h1 = 0x5555555555555555;
        const u64 h2 = 0x3333333333333333;
        const u64 h4 = 0x0F0F0F0F0F0F0F0F;
        const u64 v1 = 0x00FF00FF00FF00FF;
        const u64 v2 = 0x0000FFFF0000FFFF;
        bb = ((bb >> 1) & h1) | ((bb & h1) << 1);
        bb = ((bb >> 2) & h2) | ((bb & h2) << 2);
        bb = ((bb >> 4) & h4) | ((bb & h4) << 4);
        bb = ((bb >> 8) & v1) | ((bb & v1) << 8);
        bb = ((bb >> 16) & v2) | ((bb & v2) << 16);
        bb = (bb >> 32) | (bb << 32);
        return bb;
    }

    typedef const array<const reference_wrapper<bitboard>, 6> ptrtype;
    typedef struct pShiftData {
        square s_fwd = 0;
        square s_fwd2 = 0;
        square s_eCap = 0;
        square s_wCap = 0;
        square s_eep = 0;
        square s_wep = 0;
    }pShiftData;

    pShiftData pShift(
            const bitboard bb,
            const n_types::colour c,
            const square s_enPassantTgt,
            unique_ptr<ptrtype> &up_bbDfd);

    constexpr bitboard nShift(const square s) {
        const array<array<dir, 3>, 8> dirs = {
            NORTH, NORTH, EAST,
            NORTH, NORTH, WEST,
            SOUTH, SOUTH, EAST,
            SOUTH, SOUTH, WEST,
            EAST, EAST, NORTH,
            EAST, EAST, SOUTH,
            WEST, WEST, NORTH,
            WEST, WEST, SOUTH,
        };
        bitboard ret = 0;
        for (auto a : dirs) {
            ret |= shiftOne(a[2], shiftOne(a[1], shiftOne(a[0], s)));
        }
        return ret;
    }
    
    constexpr bool independent(const bitboard a, const bitboard b) {
        return (a & b) == 0;
    }

    typedef struct kShiftData {
        array<square, 8> a_quiets = {0};
        square s_kCastle = 0;
        square s_qCastle = 0;
    }kShiftData;

    constexpr kShiftData kShift(const square s,
            const colour c_src,
            const bool b_castlingK,
            const bool b_castlingQ,
            const bitboard bb_blockers
            ) {
        kShiftData d_ret;
        // quiet moves
        for (u8 i = 0; i < a_dirs.size(); i++) {
            dir d = a_dirs[i];
            d_ret.a_quiets[i] = shiftOne(d, s);
        }
        // castling
        bitboard bb_kMask = 0x60;
        bitboard bb_qMask = 0x06;
        bitboard bb_krMask = 0;
        bitboard bb_qrMask = 0;
        bitboard row = 0;
        if (c_src == white) {
            // want (LE) 00000x001
            // as hex: 0x80
            row = bb_blockers & 0x00000000000000ff;
            bb_krMask = 0x8000000000000000; 
            bb_qrMask = 0x0100000000000000;
        } else {
            row = (bb_blockers & 0xff00000000000000) >> 56;
            bb_krMask = 0x0000000000000080; 
            bb_qrMask = 0x0000000000000001;
        }
        if (b_castlingK && !(row & bb_kMask)) {
            d_ret.s_kCastle = bb_krMask;
        }
        if (b_castlingQ && !(row & bb_qMask)) {
            d_ret.s_qCastle = bb_qrMask;
        }
        return d_ret;
    }
    // vector<bitboard> rShift(
    //         const bitboard bb,
    //         const n_types::colour c,
    //         unique_ptr<ptrtype> &up_bbDfd) {
    // }
    u32 square2index(const square s);
}
