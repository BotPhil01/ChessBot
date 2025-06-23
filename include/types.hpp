// some universal types for across the project
#include <cctype>
#include <cstdint>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <stack>
#include <list>

#pragma once
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
    
    struct move {
        colour c_atk;
        piece p_atk;
        square s_atkOld;
        square s_atkNew;
        colour c_dfd;
        piece p_dfd;
        square s_dfdOld;
        square s_dfdNew = 0;

        constexpr bool isPromoting() {
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

        constexpr bool isCastling() {
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
    };

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
    typedef struct hisItem {
        move m;
        u8 u_halfMoveClock;
        bitboard b_enPassantDst;
    }hisItem;
    class board {
        private:
            // fen consists of
            // piece placement
            // side to move
            // castling ability
            // en passant target
            // half move clock
            // full move counter
            player p_black {
                .b_pawn = 0x00FF000000000000,
                .b_rook = 0x8100000000000000,
                .b_bishop = 0x4200000000000000,
                .b_knight = 0x2400000000000000,
                .b_king = 0x1000000000000000,
                .b_queen = 0x0800000000000000,
                .c_self = white,
                .b_kCastle = true,
                .b_qCastle = true,
            };
            player p_white {
                .b_pawn = 0x00000000000000FF,
                .b_rook = 0x0000000000008100,
                .b_bishop = 0x0000000000004200,
                .b_knight = 0x0000000000002400,
                .b_king = 0x0000000000001000,
                .b_queen = 0x0000000000000800,
                .c_self = black,
                .b_kCastle = true,
                .b_qCastle = true,
            };
            // player p_white {
            //     .b_pawn = 0,
            //         .b_rook = 0,
            //         .b_bishop = 0,
            //         .b_knight = 0,
            //         .b_king = 0,
            //         .b_queen = 0,
            //         .c_self = white,
            //         .b_kCastle = true,
            //         .b_qCastle = true,
            // };
            // player p_black {
            //     .b_pawn = 0,
            //         .b_rook = 0,
            //         .b_bishop = 0,
            //         .b_knight = 0,
            //         .b_king = 0,
            //         .b_queen = 0,
            //         .c_self = black,
            //         .b_kCastle = true,
            //         .b_qCastle = true,
            // };
            colour c_sideToMove = white;
            bitboard b_enPassantDst = 0;
            u8 u_halfMoveClock = 0;
            u64 u_fullMoveCounter = 1;
            stack<hisItem, list<hisItem>> s_moveHistory;

            void _extractBitboardAndAlter(
                    unique_ptr<player> &p,
                    const u8 u_bbIndex,
                    const square s_old,
                    const square s_new);
            bitboard &extractBitboard(unique_ptr<player> &p, const u8 u_bbIndex);
            void _playAtk(unique_ptr<player> &p, const move &m);
            void _playDfd(unique_ptr<player> &p, const move &m);
            void _unPlayAtk(unique_ptr<player> &p, const move &m);
            void _unPlayDfd(unique_ptr<player> &p, const move &m);
            void _parsePP(string_view sv_pp);
            void _parseSTM(string_view sv_pp);
            void _parseCA(string_view sv_pp);
            void _parseEP(string_view sv_pp);
            void _parseHMC(string_view sv_pp);
            void _parseFMC(string_view sv_pp);
            string_view _isolateEndFenPart(string_view &sv_fen, const string_view sv_separator);
            string_view _isolateFenPart(string_view &sv_fen, const string_view sv_separator);
            char _getChar(const bitboard mask) const;
            string _pos2string() const;
            string _data2string() const;
        public:
            board();
            int loadFen(string_view sv_fen);
            void playMove(const move m);
            void unplayMove();
            vector<move> genLegMoves();
            vector<move>  genPseudoLegalMoves();
            string str() const;
    };
}

inline ostream& operator<<(ostream& os, int8_t value) {
    return os << static_cast<int>(value);
}

inline ostream& operator<<(ostream& os, uint8_t value) {
    return os << static_cast<unsigned int>(value);
}
#endif
