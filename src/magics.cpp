#include "../include/types.hpp"
#include "../include/magics.hpp"
#include "../include/bitboard.hpp"
#include <random>
#include <array>
#include <vector>

namespace n_mgc {
    using namespace n_bbd;
    using namespace n_types;
    u64 _genRookMask(const u64 u_squ)
    {
        u64 ret = 0;
        const array<n_types::dir, 4> a_dirs = {
            n_types::NORTH,
            n_types::EAST,
            n_types::SOUTH,
            n_types::WEST
        };

        const array<u64, 4> a_masks = {
            0xff00000000000000,
            0x8080808080808080,
            0x0101010101010101,
            0x00000000000000ff
        };

        for (u8 j = 0; j < 4; j++) {
            u64 u_shifted = u_squ;
            for (u8 i = 0; i < 8; i++) {
                u_shifted = n_bbd::shiftOne(a_dirs[j], u_shifted) & ~a_masks[j];
                ret |= u_shifted;
            }
        }
        return ret;
    }

    u64 _genBishopMask(const u64 u_squ)
    {
        u64 ret = 0;
        const array<n_types::dir, 4> a_dirs = {
            n_types::NORTHEAST,
            n_types::SOUTHEAST,
            n_types::NORTHWEST,
            n_types::SOUTHWEST,
        };

        const array<u64, 4> a_masks = {
            0xff80808080808080,
            0x80808080808080ff,
            0xff01010101010101,
            0x01010101010101ff,
        };

        for (u8 j = 0; j < 4; j++) {
            u64 u_shifted = u_squ;
            for (u8 i = 0; i < 8; i++) {
                u_shifted = n_bbd::shiftOne(a_dirs[j], u_shifted) & ~a_masks[j];
                ret |= u_shifted;
            }
        }
        return ret;
    }

    // assume magics and masks are initialised
    void magics::initAttacks() {
        for (u8 u_squ = 0; u_squ < 64; u_squ++) {
            for (const n_types::piece p : {
                    n_types::piece::BISHOP,
                    n_types::piece::ROOK
                    }) {
                array<magic, 64> &a_magics = this->a_bishopMagics;
                u8 shift = this->a_bishopShifts[u_squ];
                vector<bitboard> v_blockers = magics::_genBlockers(u_squ, p);
                if (p == n_types::ROOK) {
                    a_magics = this->a_rookMagics;
                    shift = this->a_rookShifts[u_squ];
                }
                for (const bitboard u_occupancy : v_blockers) {
                    const u64 u_masked = u_occupancy & a_magics[u_squ].u_mask;
                    const u64 u_magicd = u_masked * a_magics[u_squ].u_magic;
                    const u64 u_shifted = u_magicd >> shift;
                    if (p == n_types::BISHOP) {
                        this->a_bishopAttacks[u_shifted] = magics::_genAttack(u_squ, u_masked, p);
                    } else {
                        this->a_rookAttacks[u_shifted] = magics::_genAttack(u_squ, u_masked, p);
                    }
                }
            }
        }
    }


    void magics::initMasks() {
        for (u64 i = 0; i < 64; i++) {
            const u64 bit = 1 << i;
            this->a_rookMasks[i] = _genRookMask(bit);
            this->a_bishopMasks[i] = _genBishopMask(bit);
        }
        // cout << "printing rookmasks\n";
        // for (u32 i = 0; i < 64; i++) {
        //     cout << hex << rookMasks[i] << ", ";
        //     if (i != 0 && (i % 8 == 0)) {
        //         cout << "\n";
        //     }
        // }
        // cout << "printing bishopmasks\n";
        // for (u32 i = 0; i < 64; i++) {
        //     cout << hex << bishopMasks[i] << ", ";
        //     if (i != 0 && (i % 8 == 0)) {
        //         cout << "\n";
        //     }
        // }
        // cout << endl;
    }

    vector<bitboard> magics::_enumeratBit(const u8 u_squ, const bitboard bb_mask) {
        vector<bitboard> ret;
        ret.push_back(bb_mask);
        const u8 u_bitCount = n_bbd::bitPopCount(bb_mask);
        vector<bitboard> a_bits;
        bitboard bb_tmp = bb_mask;
        while (bb_tmp) {

            const bitboard bb_isolated = n_bbd::bitIsolate(bb_tmp);
            bb_tmp ^= bb_isolated;
            a_bits.push_back(bb_isolated);
        }

        for (u8 i = 0; i < u_bitCount; i++) {
            bb_tmp = bb_mask & ~a_bits.at(i);
            vector<bitboard> enumerated = _enumeratBit(u_squ, bb_tmp);
            for (bitboard b : enumerated) {
                ret.push_back(b);
            }
        }
        return ret;
    }

    vector<bitboard> magics::_genBlockers(const u8 u_squ, const n_types::e_piece p) {
        const bitboard bb_mask = (p == n_types::BISHOP) ? this->a_bishopMasks[u_squ] : this->a_rookMasks[u_squ];
        return magics::_enumeratBit(bb_mask, p);
    }

    u64 magics::calculateMagic(const u8 u_squ, const vector<bitboard> bb_blockers, const n_types::piece p) {
        array<u64, 8196> used;

        const u64 mask = (p == n_types::BISHOP) ? this->a_bishopMasks[u_squ] : this->a_rookMasks[u_squ];

        bool fail = false;
        u64 magic = 0;
        for (u64 i = 0; i < 10000000; i++) {
            // randomly generate magic number
            fail = false;
            random_device rd;
            mt19937_64 rne(rd());
            uniform_int_distribution<unsigned long long> dist;
            magic = dist(rne);
            if (n_bbd::bitPopCount((mask * magic) & 0xff00000000000000) < 6) {
                continue;
            }
            for (u32 i = 0; i < used.size(); i++) {
                used[i] = 0ULL;
            }
            for (const bitboard bb_blocker : bb_blockers) {
                // for each blocker board
                const array<u8, 64> &shifts = p == n_types::BISHOP ? this->a_bishopShifts : this->a_rookShifts;
                // generate consec combination (index)
                const u64 index = (bb_blocker * magic) >> (64 - shifts[u_squ]);
                if (used.at(index) == 0ULL) {
                    // if not seen before then continue
                    used[index] = bb_blocker;
                } else if (used[index] != bb_blocker) {
                    // if seen before need to check that the indexed attack is the same
                    // ^ checks for a collision
                    fail = true;
                    break;
                }
            }
            if (!fail) {
                cout << "magic: " << magic << "\n";
                return magic;
            }
        }
        cout << "failed\n";
        return 0;
    }

    void magics::initMagics()
    {
        // generate relevant occupancy
        // trial and error multiply by u64 until a val found s.t:
        // occ * val = consec combination >> 64 - (masked)
        u64 start = 1;
        // for each square generate all possible blocker boards
        magics::initMasks();
        const array<n_types::e_piece, 2> a_pieces = {
            n_types::e_piece::BISHOP,
            n_types::e_piece::ROOK,
        };

        for (const n_types::piece p : a_pieces) {
            for (u8 u_square = 0; u_square < 64; u_square++) {
                const vector<bitboard> bb_blockers = magics::_genBlockers(u_square, p);
                const u64 magic = magics::calculateMagic(u_square, bb_blockers, p);
                if (p == n_types::BISHOP) {
                    this->a_bishopMagics[u_square].u_magic = magic;
                    this->a_bishopMagics[u_square].u_mask = this->a_bishopMasks[u_square];
                } else {
                    this->a_rookMagics[u_square].u_magic = magic;
                    this->a_rookMagics[u_square].u_mask = this->a_rookMasks[u_square];
                }
            }
        }
    }
}
