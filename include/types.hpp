// some universal types for across the project
#include <cctype>
#include <cstdint>
#include <iostream>
#include <string>
#include <atomic>

#pragma once
using namespace std;
#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define u64 uint64_t

#define s8 int8_t
#define s16 int16_t
#define s32 int32_t
#define s64 int64_t

#define as8 atomic_int8_t
#define as16 atomic_int16_t
#define as32 atomic_int32_t
#define as64 atomic_int64_t

#define au8 atomic_uint8_t
#define au16 atomic_uint16_t
#define au32 atomic_uint32_t
#define au64 atomic<uint64_t>


// redefine due to twos complement adding 1 extra number for negative 
// the above makes -INT64_MIN return INT64_MIN
// redefinition makes INT64_MAX the inverse of INT64_MIN under -
#undef INT64_MIN
#define INT64_MIN 0x8000000000000001

// bitboard is defined using little endian rank file
typedef u64 bitboard;

// square is a unary bitboard
typedef u64 square;


namespace n_types {
    typedef enum e_dir {
        NORTH = 8,
        NORTHEAST = 9,
        EAST = 1,
        SOUTHEAST = -7,
        SOUTH = -8,
        SOUTHWEST = -9,
        WEST = -1,
        NORTHWEST = 7,
    }dir;

    typedef enum e_piece {
        PAWN = 0,
        ROOK = 1,
        BISHOP = 2,
        KNIGHT = 3,
        KING = 4,
        QUEEN = 5,
        EMPTY = 6,
    }piece;
    
    typedef s64 evl;

    typedef bool colour;
#define white false
#define black true

    constexpr piece piece2Index(const char c) {
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

    string_view col2string(const colour c);

    string square2string(const square b);

    string piece2string(const piece p);
    
    typedef struct cMove {
        colour c_atk;
        piece p_atk;
        square s_atkOld;
        square s_atkNew;
        colour c_dfd;
        piece p_dfd;
        square s_dfdOld;
        square s_dfdNew = 0;

        cMove(colour c_atk,
                piece p_atk,
                square s_atkOld,
                square s_atkNew,
                colour c_dfd,
                piece p_dfd,
                square s_dfdOld,
                square s_dfdNew = 0) :
         c_atk(c_atk),
         p_atk(p_atk),
         s_atkOld(s_atkOld),
         s_atkNew(s_atkNew),
         c_dfd(c_dfd),
         p_dfd(p_dfd),
         s_dfdOld(s_dfdOld),
         s_dfdNew(s_dfdNew) {}

        constexpr bool isPromoting() const {
            if (p_atk != e_piece::PAWN) {
                return false;
            }
            bitboard mask = 0;
            if (c_atk == white) {
                mask = 0xff00000000000000;
            } else {
                mask = 0x00000000000000ff;
            }
            return (mask & s_atkNew) != 0;
        }

        constexpr bool isCastling() const {
            if (p_atk != e_piece::KING) {
                return false;
            }
            bitboard b_srcMask = 0;
            bitboard b_dstMask = 0;
            if (c_atk == white) {
                b_srcMask = 0x0000000000000010;
                b_dstMask = 0x0000000000000081;
            } else {
                b_srcMask = 0x1000000000000000;
                b_dstMask = 0x8100000000000000;
            }
            return ((b_srcMask & s_atkNew) != 0 &&
                    (b_dstMask & s_atkNew) != 0);
        }

        // returns similar algebraic notation
        string str() const {
            string s_ret = "";
            s_ret += piece2string(p_atk);
            s_ret += " ";
            s_ret += square2string(s_atkOld);
            s_ret += " ";
            s_ret += piece2string(p_dfd);
            s_ret += " ";
            s_ret += square2string(s_atkNew);
            return s_ret;
        }
    }cMove;
}

inline ostream& operator<<(ostream& os, int8_t value) {
    return os << static_cast<int>(value);
}

inline ostream& operator<<(ostream& os, uint8_t value) {
    return os << static_cast<unsigned int>(value);
}
