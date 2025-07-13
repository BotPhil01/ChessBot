#include "../include/types.hpp"
#include "../include/board.hpp"
#include "../include/bitboard.hpp"
#include "../include/rands.hpp"
#include <functional>

namespace n_brd {
    using namespace n_brd;
    using namespace n_bbd;
    using namespace n_rnd;

    u64 zobrist(const board b) {
        u64 u_working = 0;
        // castling
        // const u32 u_wkcastle = 0;
        // const u32 u_wqcastle = 1;
        // const u32 u_bkcastle = 2;
        // const u32 u_bqcastle = 3;
        // if (b.p_black.b_kCastle) u_working ^= a_rands[u_bkcastle];
        // if (b.p_black.b_qCastle) u_working ^= a_rands[u_bqcastle];
        // if (b.p_white.b_kCastle) u_working ^= a_rands[u_wkcastle];
        // if (b.p_white.b_qCastle) u_working ^= a_rands[u_wqcastle];
        // const u32 u_whiteToMove = 5;
        // if (b.c_sideToMove == white) {
        //     u_working ^= a_rands[u_whiteToMove];
        // }

        // ep
        const array<u32, 8> a_enPassant = {
            56, 57, 58, 59, 
            60, 61, 62, 63, 
        };
        if (b.b_enPassantDst) {
            u32 u_epIndex = a_enPassant[b.b_enPassantDst % 8];
            u_working ^= a_rands[u_epIndex];
        }
        // colour 
        if (b.c_sideToMove == white) {
            u_working ^= a_rands[0];
        }
        // if (b.b_enPassantDst) u_working ^= a_rands[a_enPassant[b.b_enPassantDst % 8]];

        for (reference_wrapper<player> r_p : b.a_players) {
            u16 u_mul = r_p.get().c_self == white ? 1 : 2;
            for (u16 i = 0; i < EMPTY; i++) {
                bitboard b_tmp = r_p.get().a_bitboards[i];
                bitboard b_iso = bitIsolate(b_tmp);
                while (b_iso) {
                    b_tmp = b_tmp & ~b_iso;
                    u32 u_index = square2index(b_iso);
                    u_index *= i * u_mul;
                    u_working ^= a_rands[u_index];
                    b_iso = bitIsolate(b_tmp);
                }
            }
        }
        return u_working;
    }
}
