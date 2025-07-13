#include "types.hpp"
#include "rands.hpp"
#include <functional>
#include <list>
#include <stack>
#include <memory>
#include <array>
#include <vector>
#pragma once
namespace n_brd {
    // using namespace n_types;
    typedef struct player {
        bitboard b_pawn;
        bitboard b_rook;
        bitboard b_bishop;
        bitboard b_knight;
        bitboard b_king;
        bitboard b_queen;
        n_types::colour c_self;
        bool b_kCastle;
        bool b_qCastle;

        array<reference_wrapper<bitboard>, 6> a_bitboards;

        player(
                bitboard b_pawn, // 0x00FF000000000000,
                bitboard b_rook, // 0x8100000000000000,
                bitboard b_bishop, // 0x4200000000000000,
                bitboard b_knight, // 0x2400000000000000,
                bitboard b_king, // 0x1000000000000000,
                bitboard b_queen, // 0x0800000000000000,
                n_types::colour c_self, // black,
                bool b_kCastle, // true,
                bool b_qCastle // true,
              ) : 
            b_pawn(b_pawn),
            b_rook(b_rook),
            b_bishop(b_bishop),
            b_knight(b_knight),
            b_king(b_king),
            b_queen(b_queen),
            c_self(c_self),
            b_kCastle(b_kCastle),
            b_qCastle(b_qCastle),
            a_bitboards({
                    ref(this->b_pawn),
                    ref(this->b_rook),
                    ref(this->b_bishop),
                    ref(this->b_knight),
                    ref(this->b_king),
                    ref(this->b_queen)
                    })
        {}

        player(const player &p) :
            b_pawn(p.b_pawn),
            b_rook(p.b_rook),
            b_bishop(p.b_bishop),
            b_knight(p.b_knight),
            b_king(p.b_king),
            b_queen(p.b_queen),
            c_self(p.c_self),
            b_kCastle(p.b_kCastle),
            b_qCastle(p.b_qCastle),
            a_bitboards({
                    ref(b_pawn),
                    ref(b_rook),
                    ref(b_bishop),
                    ref(b_knight),
                    ref(b_king),
                    ref(b_queen)
                    })
        {}

        player &operator=(const player &p) {
            b_pawn = p.b_pawn;
            b_rook = p.b_rook;
            b_bishop = p.b_bishop;
            b_knight = p.b_knight;
            b_king = p.b_king;
            b_queen = p.b_queen;
            c_self = p.c_self;
            b_kCastle = p.b_kCastle;
            b_qCastle = p.b_qCastle;
            a_bitboards =
            {
                ref(b_pawn),
                ref(b_rook),
                ref(b_bishop),
                ref(b_knight),
                ref(b_king),
                ref(b_queen)
            };
            return *this;
        }
        player(const player &&p) :
            b_pawn(p.b_pawn),
            b_rook(p.b_rook),
            b_bishop(p.b_bishop),
            b_knight(p.b_knight),
            b_king(p.b_king),
            b_queen(p.b_queen),
            c_self(p.c_self),
            b_kCastle(p.b_kCastle),
            b_qCastle(p.b_qCastle),
            a_bitboards({
                    ref(b_pawn),
                    ref(b_rook),
                    ref(b_bishop),
                    ref(b_knight),
                    ref(b_king),
                    ref(b_queen)
                    })
        {}
    }player;

    typedef struct hisItem {
        n_types::cMove m;
        u8 u_halfMoveClock;
        bitboard b_enPassantDst;
    }hisItem;

    class board {
        friend u64 zobrist(board b);
        private:
        // fen consists of
        // piece placement
        // side to move
        // castling ability
        // en passant target
        // half move clock
        // full move counter

        player p_black = player(
                0x00FF000000000000,
                0x8100000000000000,
                0x4200000000000000,
                0x2400000000000000,
                0x1000000000000000,
                0x0800000000000000,
                black,
                true,
                true
                );
        player p_white = player(
                0x00000000000000FF,
                0x0000000000008100,
                0x0000000000004200,
                0x0000000000002400,
                0x0000000000001000,
                0x0000000000000800,
                white,
                true,
                true
                );
        array<reference_wrapper<player>, 2> a_players = {ref(p_white), ref(p_black)};
        n_types::colour c_sideToMove = white;
        bitboard b_enPassantDst = 0;
        u8 u_halfMoveClock = 0;
        u64 u_fullMoveCounter = 1;
        stack<hisItem, list<hisItem>> s_moveHistory = {};
        // magics m_magics;

        void _extractBitboardAndAlter(
                unique_ptr<player> &p,
                const n_types::piece p_bbIndex,
                const square s_old,
                const square s_new);
        bitboard &extractBitboard(unique_ptr<player> &p, const u8 u_bbIndex);
        void _playAtk(unique_ptr<player> &p, const n_types::cMove &m);
        void _playDfd(unique_ptr<player> &p, const n_types::cMove &m);
        void _unPlayAtk(unique_ptr<player> &p, const n_types::cMove &m);
        void _unPlayDfd(unique_ptr<player> &p, const n_types::cMove &m);
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
        string _data2string();
        void _enumerateMoveTrgts(
                vector<n_types::cMove> &v_dst,
                const bitboard bb_atk,
                const bitboard bb_dfds,
                const n_types::piece p_atk
                );
        void _enumerateMoves(
                vector<n_types::cMove> &v_dst,
                n_types::colour c_src,
                n_types::piece p_src,
                bitboard bb_src);
        bool _isKingInCheck(const n_types::colour c);
        player &playerToMove();
        player &playerToWatch();
        n_types::piece _square2piece(const square s);
        bitboard _calcOccupied() const;

        void _genPawnMoves(
                vector<n_types::cMove> &v_dst,
                const bitboard bb_friendlies);

        void _genRookMoves(
                vector<n_types::cMove> &v_dst,
                const bitboard bb_friendlies);

        void _genBishopMoves(
                vector<n_types::cMove> &v_dst,
                const bitboard bb_friendlies);

        void _genKnightMoves(
                vector<n_types::cMove> &v_dst,
                const bitboard bb_friendlies);

        void _genQueenMoves(
                vector<n_types::cMove> &v_dst,
                const bitboard bb_friendlies);

        void _genKingMoves(
                vector<n_types::cMove> &v_dst,
                const bitboard bb_friendlies);

        void _sethmc(const n_types::cMove m);
        /*
         * @return colour of winner if game has ended in checkmate
         * @return STALEMATE if stalemate
         * @return FIDDY if fifty move rule is broken
         * @return NOEND if game is not ended
         */
#define STALEMATE -1
#define FIDDY -2
#define NOEND -3
        s64 _isGameOver();
        s64 _evalMat() const;

        s64 _evalPos() const;

        s64 _evalMov() const;

        void _alterZob(const n_types::cMove m);
        public:
        // u64 u_zob = zobrist(*this);
        u64 u_zob = 0;
        board();
        int loadFen(string_view sv_fen);
        void playMove(const n_types::cMove m);
        void unPlayMove();
        vector<n_types::cMove> genLegalMoves();
        vector<n_types::cMove> genPseudoLegalMoves();
        string str();
        n_types::evl eval();
        n_types::evl evalInit();
    };
}
