#include "types.hpp"
#include "magics.hpp"
#include <list>
#include <stack>
#pragma once
namespace n_brd {
    using namespace n_mgc;
    using namespace n_types;
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
        cMove m;
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
            magics m_magics;

            void _extractBitboardAndAlter(
                    unique_ptr<player> &p,
                    const piece p_bbIndex,
                    const square s_old,
                    const square s_new);
            bitboard &extractBitboard(unique_ptr<player> &p, const u8 u_bbIndex);
            void _playAtk(unique_ptr<player> &p, const cMove &m);
            void _playDfd(unique_ptr<player> &p, const cMove &m);
            void _unPlayAtk(unique_ptr<player> &p, const cMove &m);
            void _unPlayDfd(unique_ptr<player> &p, const cMove &m);
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
            void _enumerateMoveTrgts(
                    vector<cMove> &v_dst,
                    const bitboard bb_atk,
                    const bitboard bb_dfds,
                    const piece p_atk
                    );
            void _enumerateMoves(
                    vector<cMove> &v_dst,
                    colour c_src,
                    piece p_src,
                    bitboard bb_src);
            bool _isKingInCheck(const colour c);
            player &playerToMove();
            player &playerToWatch();
            piece _square2piece(const square s);
            void _genPawnMoves(vector<cMove> &v_dst, const bitboard bb_friendlies);

            bitboard _calcOccupied() const;
            void _genRookMoves(vector<cMove> &v_dst, const bitboard bb_friendlies);

            void _genBishopMoves(vector<cMove> &v_dst, const bitboard bb_friendlies);

            void _genKnightMoves(vector<cMove> &v_dst, const bitboard bb_friendlies);

            void _genQueenMoves(vector<cMove> &v_dst, const bitboard bb_friendlies);

            void _genKingMoves(vector<cMove> &v_dst, const bitboard bb_friendlies);

        public:
            board();
            int loadFen(string_view sv_fen);
            void playMove(const cMove m);
            void unPlayMove();
            vector<cMove> genLegalMoves();
            vector<cMove> genPseudoLegalMoves();
            string str() const;

    };
}
