#include "types.hpp"
#include "bitboard.hpp"
#include <array>
#include <vector>

#pragma once
namespace n_mgc {

    using namespace n_types;
    using namespace n_bbd;
    u64 _genRookMask(const u64 u_squ);

    u64 _genBishopMask(const u64 u_squ);
    class magics
    {
        typedef struct magic
        {
            u64 u_mask;
            u64 u_magic;
        }magic;

        const u64 u_bishopShift = 63-11;
        const u64 u_rookShift = 63-13;
        array<bitboard, 2048> a_bishopAttacks;
        array<bitboard, 8196> a_rookAttacks;
        array<magic, 64> a_bishopMagics;
        array<magic, 64> a_rookMagics;

        array<bitboard, 64> a_rookMasks;
        array<bitboard, 64> a_bishopMasks;
        const array<u8, 64> a_rookShifts = {
            12, 11, 11, 11, 11, 11, 11, 12,
            11, 10, 10, 10, 10, 10, 10, 11,
            11, 10, 10, 10, 10, 10, 10, 11,
            11, 10, 10, 10, 10, 10, 10, 11,
            11, 10, 10, 10, 10, 10, 10, 11,
            11, 10, 10, 10, 10, 10, 10, 11,
            11, 10, 10, 10, 10, 10, 10, 11,
            12, 11, 11, 11, 11, 11, 11, 12
        };

        const array<u8, 64> a_bishopShifts = {
            6, 5, 5, 5, 5, 5, 5, 6,
            5, 5, 5, 5, 5, 5, 5, 5,
            5, 5, 7, 7, 7, 7, 5, 5,
            5, 5, 7, 9, 9, 7, 5, 5,
            5, 5, 7, 9, 9, 7, 5, 5,
            5, 5, 7, 7, 7, 7, 5, 5,
            5, 5, 5, 5, 5, 5, 5, 5,
            6, 5, 5, 5, 5, 5, 5, 6
        };

        constexpr u64 bishopAttacks(
                const u64 u_occupancy,
                const u8 u_squIndex
                ) const {
            const u64 u_masked = u_occupancy & a_bishopMagics[u_squIndex].u_mask;
            const u64 u_magicd = u_masked * a_bishopMagics[u_squIndex].u_magic;
            const u64 u_shifted = u_magicd >> (63 -a_bishopShifts[u_squIndex]);
            return a_bishopAttacks.at(u_shifted);
        }

        constexpr u64 rookAttacks(
                const u64 u_occupancy,
                const u8 u_squIndex
                ) const {
            const u64 u_masked = u_occupancy & a_rookMagics[u_squIndex].u_mask;
            const u64 u_magicd = u_masked * a_rookMagics[u_squIndex].u_magic;
            const u64 u_shifted = u_magicd >> (63 -a_rookShifts[u_squIndex]);
            return a_rookAttacks.at(u_shifted);
        }

        constexpr bitboard _genAttack(
                const square u_squ,
                const bitboard b_occ,
                const piece p
                ) const {
            const array<dir, 4> a_bDirs = {
                NORTHEAST,
                NORTHWEST,
                SOUTHEAST,
                SOUTHWEST,
            };

            const array<dir, 4> a_rDirs = {
                NORTH,
                EAST,
                SOUTH,
                WEST,
            };

            bitboard ret = 0;
            if (p == BISHOP) {
                const array<bitboard, 4> a_masks = {
                    0xff80808080808080,
                    0x80808080808080ff,
                    0xff01010101010101,
                    0x01010101010101ff,
                };
                for (u8 j = 0; j < 4; j++) {
                    bitboard line = u_squ;
                    const dir d = a_bDirs[j];
                    const bitboard mask = a_masks[j];
                    while ((line & mask) == 0) {
                        line |= shiftOne(d, line);
                    }
                    ret |= line;
                }
            } else {
                const array<u64, 4> a_masks = {
                    0xff00000000000000,
                    0x8080808080808080,
                    0x0101010101010101,
                    0x00000000000000ff
                };
                for (u8 j = 0; j < 4; j++) {
                    bitboard line = u_squ;
                    const dir d = a_bDirs[j];
                    const bitboard mask = a_masks[j];
                    while ((line & mask) == 0) {
                        line |= shiftOne(d, line);
                    }
                    ret |= line;
                }
            }
            ret ^= u_squ;
            return ret;
        }

        // assume magics and masks are initialised
        void initAttacks();

        void initMasks();

        vector<bitboard> _enumeratBit(const u8 u_squ, const bitboard bb_mask);

        vector<bitboard> _genBlockers(const u8 u_squ, const n_types::e_piece p);

        u64 calculateMagic(const u8 u_squ, const vector<bitboard> bb_blockers, const n_types::piece p);

        public:
        void initMagics();
        constexpr bitboard lookup(
                const square s_src,
                const bitboard bb_occupied,
                const piece p
                ) const {
            // change square to index
            if (p == BISHOP) {
                return this->bishopAttacks(bb_occupied, s_src);
            } else {
                return this->rookAttacks(bb_occupied, s_src);
            }
        }
    };

}
