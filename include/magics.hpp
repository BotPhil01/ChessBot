#include "types.hpp"
#include "bitboard.hpp"
#include <array>
#include <random>
#include <vector>

#pragma once
namespace n_mgc {
    using namespace n_types;
    using namespace n_bbd;
    const u64 U_MAXMASKENUM = 65536;

    // const array<u64, 64> a_bMagics = {
    //     0x89a1121896040240ULL,
    //     0x2004844802002010ULL,
    //     0x2068080051921000ULL,
    //     0x62880a0220200808ULL,
    //     0x4042004000000ULL,
    //     0x100822020200011ULL,
    //     0xc00444222012000aULL,
    //     0x28808801216001ULL,
    //     0x400492088408100ULL,
    //     0x201c401040c0084ULL,
    //     0x840800910a0010ULL,
    //     0x82080240060ULL,
    //     0x2000840504006000ULL,
    //     0x30010c4108405004ULL,
    //     0x1008005410080802ULL,
    //     0x8144042209100900ULL,
    //     0x208081020014400ULL,
    //     0x4800201208ca00ULL,
    //     0xf18140408012008ULL,
    //     0x1004002802102001ULL,
    //     0x841000820080811ULL,
    //     0x40200200a42008ULL,
    //     0x800054042000ULL,
    //     0x88010400410c9000ULL,
    //     0x520040470104290ULL,
    //     0x1004040051500081ULL,
    //     0x2002081833080021ULL,
    //     0x400c00c010142ULL,
    //     0x941408200c002000ULL,
    //     0x658810000806011ULL,
    //     0x188071040440a00ULL,
    //     0x4800404002011c00ULL,
    //     0x104442040404200ULL,
    //     0x511080202091021ULL,
    //     0x4022401120400ULL,
    //     0x80c0040400080120ULL,
    //     0x8040010040820802ULL,
    //     0x480810700020090ULL,
    //     0x102008e00040242ULL,
    //     0x809005202050100ULL,
    //     0x8002024220104080ULL,
    //     0x431008804142000ULL,
    //     0x19001802081400ULL,
    //     0x200014208040080ULL,
    //     0x3308082008200100ULL,
    //     0x41010500040c020ULL,
    //     0x4012020c04210308ULL,
    //     0x208220a202004080ULL,
    //     0x111040120082000ULL,
    //     0x6803040141280a00ULL,
    //     0x2101004202410000ULL,
    //     0x8200000041108022ULL,
    //     0x21082088000ULL,
    //     0x2410204010040ULL,
    //     0x40100400809000ULL,
    //     0x822088220820214ULL,
    //     0x40808090012004ULL,
    //     0x910224040218c9ULL,
    //     0x402814422015008ULL,
    //     0x90014004842410ULL,
    //     0x1000042304105ULL,
    //     0x10008830412a00ULL,
    //     0x2520081090008908ULL,
    //     0x40102000a0a60140ULL,
    // };

    // array<u64, 64> a_bMagics = {
    //     0x400492088408100ULL, 0x201c401040c0084ULL, 0x840800910a0010ULL, 0x82080240060ULL, 0x2000840504006000ULL, 0x30010c4108405004ULL, 0x1008005410080802ULL, 0x8144042209100900ULL,
    //     0x111040120082000ULL, 0x6803040141280a00ULL, 0x2101004202410000ULL, 0x8200000041108022ULL, 0x21082088000ULL, 0x2410204010040ULL, 0x40100400809000ULL, 0x822088220820214ULL,
    //     0x8002024220104080ULL, 0x431008804142000ULL, 0x19001802081400ULL, 0x200014208040080ULL, 0x3308082008200100ULL, 0x41010500040c020ULL, 0x4012020c04210308ULL, 0x208220a202004080ULL,
    //     0x104442040404200ULL, 0x511080202091021ULL, 0x4022401120400ULL, 0x80c0040400080120ULL, 0x8040010040820802ULL, 0x480810700020090ULL, 0x102008e00040242ULL, 0x809005202050100ULL,
    //     0x520040470104290ULL, 0x1004040051500081ULL, 0x2002081833080021ULL, 0x400c00c010142ULL, 0x941408200c002000ULL, 0x658810000806011ULL, 0x188071040440a00ULL, 0x4800404002011c00ULL,
    //     0x208081020014400ULL, 0x4800201208ca00ULL, 0xf18140408012008ULL, 0x1004002802102001ULL, 0x841000820080811ULL, 0x40200200a42008ULL, 0x800054042000ULL, 0x88010400410c9000ULL,
    //     0x40808090012004ULL, 0x910224040218c9ULL, 0x402814422015008ULL, 0x90014004842410ULL, 0x1000042304105ULL, 0x10008830412a00ULL, 0x2520081090008908ULL, 0x40102000a0a60140ULL,
    //     0x89a1121896040240ULL, 0x2004844802002010ULL, 0x2068080051921000ULL, 0x62880a0220200808ULL, 0x4042004000000ULL, 0x100822020200011ULL, 0xc00444222012000aULL, 0x28808801216001ULL,
    // };

    // array<u64, 64> a_rMagics = {
    //     0x4080008040102101ULL, 0x40002080411d01ULL, 0x2005524060000901ULL, 0x502001008400422ULL, 0x489a000810200402ULL, 0x1004400080a13ULL, 0x4000011008020084ULL, 0x26002114058042ULL,
    //     0x280009023410300ULL, 0xe0100040002240ULL, 0x200100401700ULL, 0x2244100408008080ULL, 0x8000400801980ULL, 0x2000810040200ULL, 0x8010100228810400ULL, 0x2000009044210200ULL,
    //     0x40204000808000ULL, 0x8001008040010020ULL, 0x8410820820420010ULL, 0x1003001000090020ULL, 0x804040008008080ULL, 0x12000810020004ULL, 0x1000100200040208ULL, 0x430000a044020001ULL,
    //     0x491604001800080ULL, 0x100401000402001ULL, 0x4820010021001040ULL, 0x400402202000812ULL, 0x209009005000802ULL, 0x810800601800400ULL, 0x4301083214000150ULL, 0x204026458e001401ULL,
    //     0x800080204005ULL, 0x1040200040100048ULL, 0x120200402082ULL, 0xd14880480100080ULL, 0x12040280080080ULL, 0x100040080020080ULL, 0x9020010080800200ULL, 0x813241200148449ULL,
    //     0x22878001e24000ULL, 0x1090810021004010ULL, 0x801030040200012ULL, 0x500808008001000ULL, 0xa08018014000880ULL, 0x8000808004000200ULL, 0x201008080010200ULL, 0x801020000441091ULL,
    //     0xa000800080400034ULL, 0x4808020004000ULL, 0x2290802004801000ULL, 0x411000d00100020ULL, 0x402800800040080ULL, 0xb000401004208ULL, 0x2409000100040200ULL, 0x1002100004082ULL,
    //     0xa8002c000108020ULL, 0x6c00049b0002001ULL, 0x100200010090040ULL, 0x2480041000800801ULL, 0x280028004000800ULL, 0x900410008040022ULL, 0x280020001001080ULL, 0x2880002041000080ULL,
    // };

    extern array<u64, 64> a_bMagics;
    extern array<u64, 64> a_rMagics;

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

    constexpr array<bitboard, 64> _genRookMask()
    {
        array<bitboard, 64> a_ret = {0};
        for (u64 i = 0, bit = 1 << i; i < 64; i++, bit = 1 << i) {
            bitboard bb_mask = 0;

            for (dir d : {
                    NORTH,
                    EAST,
                    SOUTH,
                    WEST}) {
                bitboard bb_old = bit;
                for (u8 i = 0; i < 7; i++) {
                    bb_old = shiftOne(d, bb_old);
                    bb_mask |= bb_old;
                }
            }
            a_ret[i] = bb_mask;
        }
        return a_ret;
    }

    constexpr array<bitboard, 64> _genBishopMask()
    {
        array<bitboard, 64> a_ret = {0};
        for (u64 i = 0, bit = 1 << i; i < 64; i++, bit = 1 << i) {
            bitboard bb_mask = 0;
            const bitboard bb_aDiagMask = 0x0102040810204080;
            const bitboard bb_diagMask = 0x8040201008040201;

            for (dir d : {
                    NORTHEAST,
                    NORTHWEST,
                    SOUTHEAST,
                    SOUTHWEST}) {
                bitboard bb_old = bit;
                for (u8 i = 0; i < 7; i++) {
                    bb_old = shiftOne(d, bb_old);
                    bb_mask |= bb_old;
                }
            }
            a_ret[i] = bb_mask;
        }
        return a_ret;
    }

    const array<bitboard, 64> a_bMasks = _genBishopMask();
    const array<bitboard, 64> a_rMasks = _genRookMask();

    constexpr array<bitboard, U_MAXMASKENUM> _enumeratBit(const u8 u_squ, const bitboard bb_mask) {
        array<bitboard, U_MAXMASKENUM> v_ret = {0};
        const u64 u_mBitCount = bitPopCount(bb_mask);
        const u64 u_variations = 1ULL << u_mBitCount;
        for (u64 i = 0; i < u_variations; i++) {
            u64 bb_enumeration = 0ULL;

            bitboard bb_mTmp = bb_mask;
            for (u64 j = 0; j < u_mBitCount; j++) {
                const square s_isolated = bitIsolate(bb_mTmp);
                const u64 u_index = square2index(s_isolated);
                bb_mTmp &= ~s_isolated;

                if (i & (1ULL << j)) {
                    bb_enumeration |= (1ULL << u_index);
                }
            }
            v_ret[i] = bb_enumeration;
        }
        return v_ret;
    }

    constexpr bitboard _genBishopAttack(const square s, const bitboard bb_blockers) {
        const array<dir, 4> a_dirs = {
            NORTHEAST,
            NORTHWEST,
            SOUTHEAST,
            SOUTHWEST
        };
        bitboard bb_ret = 0;
        for (const dir d : a_dirs) {
            // keep shifting until its the same as the previous or a blocker is intersected
            bitboard bb_shifts = 0;
            bitboard bb_shifted = shiftOne(d, s); 
            while (bb_shifted) {
                bb_shifts |= bb_shifted;
                if (bb_shifted & bb_blockers) {
                    break;
                }
                bb_shifted = shiftOne(d, bb_shifted);
            }
            bb_ret |= bb_shifts;
        }
        return bb_ret;
    }

    constexpr array<array<u64, 512>, 64> _genBishopAttacks() {
        array<array<u64, 512>, 64> a_ret = {0};
        const array<u8, 64> a_shifts = a_bishopShifts;
        for (u64 i = 0; i < 64; i++) {
            const u64 bb_mask = a_bMasks[i];
            const u64 u_maskPopCount = bitPopCount(bb_mask);
            array<bitboard, U_MAXMASKENUM> v_maskPerms = _enumeratBit(1UL << i, bb_mask);
            for (bitboard bb_maskPerm : v_maskPerms) {
                u64 u_magic = bb_maskPerm * a_bMagics[i] >> (64 - a_shifts[i]);
                a_ret[i][u_magic] = _genBishopAttack(i, bb_mask);
            }
        }
        return a_ret;
    }

    constexpr bitboard _genRookAttack(const square s, const bitboard bb_blockers) {
        const array<dir, 4> a_dirs = {
            NORTH,
            EAST,
            SOUTH,
            WEST
        };
        bitboard bb_ret = 0;
        for (const dir d : a_dirs) {
            // keep shifting until its the same as the previous or a blocker is intersected
            bitboard bb_shifts = 0;
            bitboard bb_shifted = shiftOne(d, s); 
            while (bb_shifted) {
                bb_shifts |= bb_shifted;
                if (bb_shifted & bb_blockers) {
                    break;
                }
                bb_shifted = shiftOne(d, bb_shifted);
            }
            bb_ret |= bb_shifts;
        }
        return bb_ret;
    }

    constexpr array<array<u64, 4096>, 64> _genRookAttacks() {
        array<array<u64, 4096>, 64> a_ret = {0};
        const array<u8, 64> a_shifts = a_rookShifts;
        for (u64 i = 0; i < 64; i++) {
            const u64 bb_mask = a_rMasks[i];
            const u64 u_maskPopCount = bitPopCount(bb_mask);
            array<bitboard, 65536> v_maskPerms = _enumeratBit(1UL << i, bb_mask);
            for (bitboard bb_maskPerm : v_maskPerms) {
                u64 u_magic = bb_maskPerm * a_rMagics[i] >> (64 - a_shifts[i]);
                a_ret[i][u_magic] = _genRookAttack(i, bb_mask);
            }
        }
        return a_ret;
    }

    const array<array<bitboard, 512>, 64>  a_bishopAttacks = _genBishopAttacks();
    const array<array<bitboard, 4096>, 64> a_rookAttacks = _genRookAttacks();

    constexpr u64 bishopAttacks(
            const u64 u_occupancy,
            const u8 u_squIndex
            ) {
        const u64 u_masked = u_occupancy & a_bMasks[u_squIndex];
        const u64 u_magicd = u_masked * a_bMagics[u_squIndex];
        const u64 u_shifted = u_magicd >> (64 - a_bishopShifts[u_squIndex]);
        return a_bishopAttacks[u_squIndex][u_shifted];
    }

    constexpr u64 rookAttacks(
            const u64 u_occupancy,
            const u8 u_squIndex
            ) {
        const u64 u_masked = u_occupancy & a_rMasks[u_squIndex];
        const u64 u_magicd = u_masked * a_rMagics[u_squIndex];
        const u64 u_shifted = u_magicd >> (64 - a_rookShifts[u_squIndex]);
        return a_rookAttacks[u_squIndex][u_shifted];
    }

    constexpr bitboard lookup(
            const square s_src,
            const bitboard bb_occupied,
            const piece p
            ) {
        if (s_src == 0) return 0;
        // change square to index
        if (p == BISHOP) {
            return bishopAttacks(bb_occupied, square2index(s_src));
        } else {
            return rookAttacks(bb_occupied, square2index(s_src));
        }
    }


    // u64 genMagic(u32 u_squIndex, piece p) {
    u64 genMagic(const u32 u_squIndex);

    void genMagics();
    //     class magics
    //     {
    //         constexpr bitboard _genAttack(
    //                 const square u_squ,
    //                 const bitboard b_occ,
    //                 const piece p
    //                 ) const {
    //             const array<dir, 4> a_bDirs = {
    //                 NORTHEAST,
    //                 NORTHWEST,
    //                 SOUTHEAST,
    //                 SOUTHWEST,
    //             };

//             const array<dir, 4> a_rDirs = {
//                 NORTH,
//                 EAST,
//                 SOUTH,
//                 WEST,
//             };

//             bitboard ret = 0;
//             if (p == BISHOP) {
//                 const array<bitboard, 4> a_masks = {
//                     0xff80808080808080,
//                     0x80808080808080ff,
//                     0xff01010101010101,
//                     0x01010101010101ff,
//                 };
//                 for (u8 j = 0; j < 4; j++) {
//                     bitboard line = u_squ;
//                     const dir d = a_bDirs[j];
//                     const bitboard mask = a_masks[j];
//                     while ((line & mask) == 0) {
//                         line |= shiftOne(d, line);
//                     }
//                     ret |= line;
//                 }
//             } else {
//                 const array<u64, 4> a_masks = {
//                     0xff00000000000000,
//                     0x8080808080808080,
//                     0x0101010101010101,
//                     0x00000000000000ff
//                 };
//                 for (u8 j = 0; j < 4; j++) {
//                     bitboard line = u_squ;
//                     const dir d = a_bDirs[j];
//                     const bitboard mask = a_masks[j];
//                     while ((line & mask) == 0) {
//                         line |= shiftOne(d, line);
//                     }
//                     ret |= line;
//                 }
//             }
//             ret ^= u_squ;
//             return ret;
//         }

//         // assume magics and masks are initialised


//         vector<bitboard> _genBlockers(const u8 u_squ, const n_types::e_piece p);

//         // u64 calculateMagic(const u8 u_squ, const vector<bitboard> bb_blockers, const n_types::piece p);
//         // u64 _getPreDefined(const u32 u_squ, const piece p);
//         // void initMagics();
//         u64 _genRandom();

//         public:
//         magics() {
//             srand(time(NULL));
//             this->initAttacks();
//         }
//     };

}
