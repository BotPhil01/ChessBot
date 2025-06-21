// some universal types for across the project
#include <cctype>
#include <cstdint>
#include <functional>
#include <iostream>

#ifndef CTYPES_H
#define CTYPES_H
using namespace std;
#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define u64 uint64_t

#define s8 int8_t
#define s16 int16_t
#define s32 int32_t
#define s64 int64_t

#define bitboard u64
// bitboard is defined using little endian rank file

#define dir s8 
#define piece u8

#define colour bool
#define white false
#define black true

namespace n_types {
    enum e_dir {
        NORTH = 8,
        NORTHEAST = 9,
        EAST = 1,
        SOUTHEAST = -7,
        SOUTH = -8,
        SOUTHWEST = -9,
        WEST = -1,
        NORTHWEST = 7,
    };

    enum e_piece {
        PAWN = 0,
        ROOK = 1,
        BISHOP = 2,
        KNIGHT = 3,
        KING = 4,
        QUEEN = 5,
        EMPTY = 6,
    };

    constexpr piece cPiece2Index(const char c) {
        switch (tolower(c)) {
            case 'p':
                return e_piece::PAWN;
            case 'r': 
                return e_piece::ROOK;
            case 'b': 
                return e_piece::BISHOP;
            case 'n': 
                return e_piece::KNIGHT;
            case 'k': 
                return e_piece::KING;
            case 'q': 
                return e_piece::QUEEN;
            default:
                return e_piece::EMPTY;
        }
        return e_piece::EMPTY;
    }
    
    struct move {
        colour c_src;
        bitboard b_src;
        piece p_src;
        colour c_dst;
        bitboard b_dst;
        piece p_dst;

        constexpr bool isPromoting() {
            if (p_src != e_piece::PAWN) {
                return false;
            }
            bitboard mask = 0;
            if (c_src == white) {
                mask = 0xff00000000000000;
            } else {
                mask = 0x00000000000000ff;
            }
            return (mask & b_dst) != 0;
        }

        constexpr bool isCastling() {
            if (p_src != e_piece::KING) {
                return false;
            }
            bitboard b_srcMask = 0;
            bitboard b_dstMask = 0;
            if (c_src == white) {
                b_srcMask = 0x0000000000000010;
                b_dstMask = 0x0000000000000081;
            } else {
                b_srcMask = 0x1000000000000000;
                b_dstMask = 0x8100000000000000;
            }
            return ((b_srcMask & b_src) != 0 &&
                    (b_dstMask & b_dst) != 0);
        }
    };

    class board;
    typedef struct player {
        bitboard b_pawn;
        bitboard b_rook;
        bitboard b_bishop;
        bitboard b_knight;
        bitboard b_king;
        bitboard b_queen;
        colour c_self;
        bool b_kCastle;
        bool b_qCastle;

        array<const reference_wrapper<bitboard>, 6> a_bitboards = {
            ref(b_pawn),
            ref(b_rook),
            ref(b_bishop),
            ref(b_knight),
            ref(b_king),
            ref(b_queen)
        };
    }player;
}

inline ostream& operator<<(ostream& os, int8_t value) {
    return os << static_cast<int>(value);
}

inline ostream& operator<<(ostream& os, uint8_t value) {
    return os << static_cast<unsigned int>(value);
}
#endif
