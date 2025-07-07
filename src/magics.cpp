#include "../include/types.hpp"
#include "../include/magics.hpp"
#include "../include/bitboard.hpp"
#include <random>
#include <array>
#include <vector>

namespace n_mgc {
    using namespace n_bbd;
    using namespace n_types;

    array<u64, 64> a_bMagics = {
        0x400492088408100ULL, 0x201c401040c0084ULL, 0x840800910a0010ULL, 0x82080240060ULL, 0x2000840504006000ULL, 0x30010c4108405004ULL, 0x1008005410080802ULL, 0x8144042209100900ULL,
        0x111040120082000ULL, 0x6803040141280a00ULL, 0x2101004202410000ULL, 0x8200000041108022ULL, 0x21082088000ULL, 0x2410204010040ULL, 0x40100400809000ULL, 0x822088220820214ULL,
        0x8002024220104080ULL, 0x431008804142000ULL, 0x19001802081400ULL, 0x200014208040080ULL, 0x3308082008200100ULL, 0x41010500040c020ULL, 0x4012020c04210308ULL, 0x208220a202004080ULL,
        0x104442040404200ULL, 0x511080202091021ULL, 0x4022401120400ULL, 0x80c0040400080120ULL, 0x8040010040820802ULL, 0x480810700020090ULL, 0x102008e00040242ULL, 0x809005202050100ULL,
        0x520040470104290ULL, 0x1004040051500081ULL, 0x2002081833080021ULL, 0x400c00c010142ULL, 0x941408200c002000ULL, 0x658810000806011ULL, 0x188071040440a00ULL, 0x4800404002011c00ULL,
        0x208081020014400ULL, 0x4800201208ca00ULL, 0xf18140408012008ULL, 0x1004002802102001ULL, 0x841000820080811ULL, 0x40200200a42008ULL, 0x800054042000ULL, 0x88010400410c9000ULL,
        0x40808090012004ULL, 0x910224040218c9ULL, 0x402814422015008ULL, 0x90014004842410ULL, 0x1000042304105ULL, 0x10008830412a00ULL, 0x2520081090008908ULL, 0x40102000a0a60140ULL,
        0x89a1121896040240ULL, 0x2004844802002010ULL, 0x2068080051921000ULL, 0x62880a0220200808ULL, 0x4042004000000ULL, 0x100822020200011ULL, 0xc00444222012000aULL, 0x28808801216001ULL,
    };

    array<u64, 64> a_rMagics = {
        0x4080008040102101ULL, 0x40002080411d01ULL, 0x2005524060000901ULL, 0x502001008400422ULL, 0x489a000810200402ULL, 0x1004400080a13ULL, 0x4000011008020084ULL, 0x26002114058042ULL,
        0x280009023410300ULL, 0xe0100040002240ULL, 0x200100401700ULL, 0x2244100408008080ULL, 0x8000400801980ULL, 0x2000810040200ULL, 0x8010100228810400ULL, 0x2000009044210200ULL,
        0x40204000808000ULL, 0x8001008040010020ULL, 0x8410820820420010ULL, 0x1003001000090020ULL, 0x804040008008080ULL, 0x12000810020004ULL, 0x1000100200040208ULL, 0x430000a044020001ULL,
        0x491604001800080ULL, 0x100401000402001ULL, 0x4820010021001040ULL, 0x400402202000812ULL, 0x209009005000802ULL, 0x810800601800400ULL, 0x4301083214000150ULL, 0x204026458e001401ULL,
        0x800080204005ULL, 0x1040200040100048ULL, 0x120200402082ULL, 0xd14880480100080ULL, 0x12040280080080ULL, 0x100040080020080ULL, 0x9020010080800200ULL, 0x813241200148449ULL,
        0x22878001e24000ULL, 0x1090810021004010ULL, 0x801030040200012ULL, 0x500808008001000ULL, 0xa08018014000880ULL, 0x8000808004000200ULL, 0x201008080010200ULL, 0x801020000441091ULL,
        0xa000800080400034ULL, 0x4808020004000ULL, 0x2290802004801000ULL, 0x411000d00100020ULL, 0x402800800040080ULL, 0xb000401004208ULL, 0x2409000100040200ULL, 0x1002100004082ULL,
        0xa8002c000108020ULL, 0x6c00049b0002001ULL, 0x100200010090040ULL, 0x2480041000800801ULL, 0x280028004000800ULL, 0x900410008040022ULL, 0x280020001001080ULL, 0x2880002041000080ULL,
    };

    u64 genRandom() {
        u64 u = time(NULL);
        u ^= u << 13;
        u ^= u >> 17;
        u ^= u << 5;
        return u;
    }

    u64 genRandomFew() {
        return genRandom() & genRandom() & genRandom();
    }

    u64 genMagic(const u32 u_squIndex) {
        const bitboard bb_mask = a_rMasks[u_squIndex];
        array<bitboard, 4096> a_usedAttacks;
        bool b_fail = false;

        const u64 u_maskBitCount = bitPopCount(bb_mask);
        const array<bitboard, U_MAXMASKENUM> a_maskPerms = _enumeratBit(u_squIndex, bb_mask);

        // generate a random number

        u64 u_magic = 0;
        for (u64 i = 0; i < 100000000; i++) {
            u_magic = genRandomFew();
            if ((bitPopCount(bb_mask * u_magic) & 0xFF00000000000000ULL) < 6) continue;
            cout << "passed check";
            for (u32 i = 0; i < a_usedAttacks.size(); i++) a_usedAttacks[i] = 0;
            // test each mask permutation to be unique
            for (u64 u_permIndex = 0; u_permIndex < a_maskPerms.size(); u_permIndex++) {
                const u64 u_testIndex = (a_maskPerms[u_testIndex] * u_magic) >> (64 - a_rookShifts[u_squIndex]);
                if (a_usedAttacks[u_testIndex] != 0) a_usedAttacks[u_testIndex] = a_rookAttacks[u_squIndex][u_permIndex];
                else if (a_usedAttacks[u_testIndex] != a_rookAttacks[u_squIndex][u_testIndex]) b_fail = true;
            }
            if (!b_fail) {
                cout << "magic found: " << u_magic << "\n";
                return u_magic;
            }

        }
        return 0;
    }

    void genMagics() {
        // masks have been generated
        // for each square we need to generate a magic bitboar
        // gen magic number
        random_device rd;
        mt19937_64 gen(rd());
        uniform_int_distribution<u64> dis;

        cout << "magics generating\n";
        for (const piece p : {BISHOP, ROOK}) {
            for (u64 i = 0; i < 64; i++) {
                a_rMagics[i] = genMagic(i);
            }
        }
        cout << "magics generated\n";
    } 
    // assume magics and masks are initialised
    // void magics::initAttacks() {
    //     for (piece p : {BISHOP, ROOK}) {
    //         const array<u8, 64> a_shifts = (p == BISHOP) ? a_bishopShifts : a_rookShifts;
    //         for (u64 i = 0; i < 64; i++) {
    //             const u64 bb_mask = p == BISHOP ? a_bMasks[i] : a_rMasks[i];
    //             const u64 u_maskPopCount = bitPopCount(bb_mask);
    //             vector<bitboard> v_maskPerms = _enumeratBit(1UL << i, bb_mask);
    //             for (bitboard bb_maskPerm : v_maskPerms) {
    //                 u64 u_magic = bb_maskPerm * a_bMagics[i] >> (64 - a_shifts[i]);
    //                 if (p == BISHOP) {
    //                     a_bishopAttacks[i][u_magic] = _genBishopAttack(i, bb_mask);
    //                 }
    //                 else {
    //                     a_rookAttacks[i][u_magic] = _genRookAttack(i, bb_mask);
    //                 }
    //             }
    //         }
    //     }
    // }


    // vector<bitboard> magics::_genBlockers(const u8 u_squ, const n_types::e_piece p) {
    //     const bitboard bb_mask = (p == n_types::BISHOP) ? this->a_bishopMasks[u_squ] : this->a_rookMasks[u_squ];
    //     return magics::_enumeratBit(bb_mask, p);
    // }

    // u64 magics::_genRandom() {
    //     u64 u_ret = 0;
    //     for (u8 i = 0; i < 4; i++) {
    //         u_ret |= ((u64) rand()) << ((i+1) * 4);
    //     }
    //     return u_ret;
    // }

    // u64 magics::calculateMagic(const u8 u_squ, const vector<bitboard> bb_blockers, const n_types::piece p) {
    //     array<u64, 8196> used;

    //     const u64 mask = (p == n_types::BISHOP) ? this->a_bishopMasks[u_squ] : this->a_rookMasks[u_squ];

    //     bool fail = false;
    //     u64 magic = 0;
    //     for (u64 i = 0; i < 1000000000; i++) {
    //         // randomly generate magic number
    //         fail = false;


    //         magic = this->_genRandom();
    //         if (n_bbd::bitPopCount((mask * magic) & 0xff00000000000000) < 6) {
    //             continue;
    //         }
    //         for (u32 i = 0; i < used.size(); i++) {
    //             used[i] = 0ULL;
    //         }
    //         for (const bitboard bb_blocker : bb_blockers) {
    //             // for each blocker board
    //             const array<u8, 64> &shifts = p == n_types::BISHOP ? this->a_bishopShifts : this->a_rookShifts;
    //             // generate consec combination (index)
    //             const u64 index = (bb_blocker * magic) >> (64 - shifts[u_squ]);
    //             if (used.at(index) == 0ULL) {
    //                 // if not seen before then continue
    //                 used[index] = bb_blocker;
    //             } else if (used[index] != bb_blocker) {
    //                 // if seen before need to check that the indexed attack is the same
    //                 // ^ checks for a collision
    //                 fail = true;
    //                 break;
    //             }
    //         }
    //         if (!fail) {
    //             cout << "magic: " << magic << "\n";
    //             return magic;
    //         }
    //     }
    //     cout << "failed\n";
    //     return 0;
    // }

    // u64 magics::_getPreDefined(const u32 u_squ, const piece p) {
    //     // magics taken from https://github.com/maksimKorzh/chess_programming
    //     const u64 u_rMagics[64] = {
    //         0x8a80104000800020ULL,
    //         0x140002000100040ULL,
    //         0x2801880a0017001ULL,
    //         0x100081001000420ULL,
    //         0x200020010080420ULL,
    //         0x3001c0002010008ULL,
    //         0x8480008002000100ULL,
    //         0x2080088004402900ULL,
    //         0x800098204000ULL,
    //         0x2024401000200040ULL,
    //         0x100802000801000ULL,
    //         0x120800800801000ULL,
    //         0x208808088000400ULL,
    //         0x2802200800400ULL,
    //         0x2200800100020080ULL,
    //         0x801000060821100ULL,
    //         0x80044006422000ULL,
    //         0x100808020004000ULL,
    //         0x12108a0010204200ULL,
    //         0x140848010000802ULL,
    //         0x481828014002800ULL,
    //         0x8094004002004100ULL,
    //         0x4010040010010802ULL,
    //         0x20008806104ULL,
    //         0x100400080208000ULL,
    //         0x2040002120081000ULL,
    //         0x21200680100081ULL,
    //         0x20100080080080ULL,
    //         0x2000a00200410ULL,
    //         0x20080800400ULL,
    //         0x80088400100102ULL,
    //         0x80004600042881ULL,
    //         0x4040008040800020ULL,
    //         0x440003000200801ULL,
    //         0x4200011004500ULL,
    //         0x188020010100100ULL,
    //         0x14800401802800ULL,
    //         0x2080040080800200ULL,
    //         0x124080204001001ULL,
    //         0x200046502000484ULL,
    //         0x480400080088020ULL,
    //         0x1000422010034000ULL,
    //         0x30200100110040ULL,
    //         0x100021010009ULL,
    //         0x2002080100110004ULL,
    //         0x202008004008002ULL,
    //         0x20020004010100ULL,
    //         0x2048440040820001ULL,
    //         0x101002200408200ULL,
    //         0x40802000401080ULL,
    //         0x4008142004410100ULL,
    //         0x2060820c0120200ULL,
    //         0x1001004080100ULL,
    //         0x20c020080040080ULL,
    //         0x2935610830022400ULL,
    //         0x44440041009200ULL,
    //         0x280001040802101ULL,
    //         0x2100190040002085ULL,
    //         0x80c0084100102001ULL,
    //         0x4024081001000421ULL,
    //         0x20030a0244872ULL,
    //         0x12001008414402ULL,
    //         0x2006104900a0804ULL,
    //         0x1004081002402ULL,
    //     };

    //     const u64 u_bMagics[64] = {
    //         0x40040844404084ULL,
    //         0x2004208a004208ULL,
    //         0x10190041080202ULL,
    //         0x108060845042010ULL,
    //         0x581104180800210ULL,
    //         0x2112080446200010ULL,
    //         0x1080820820060210ULL,
    //         0x3c0808410220200ULL,
    //         0x4050404440404ULL,
    //         0x21001420088ULL,
    //         0x24d0080801082102ULL,
    //         0x1020a0a020400ULL,
    //         0x40308200402ULL,
    //         0x4011002100800ULL,
    //         0x401484104104005ULL,
    //         0x801010402020200ULL,
    //         0x400210c3880100ULL,
    //         0x404022024108200ULL,
    //         0x810018200204102ULL,
    //         0x4002801a02003ULL,
    //         0x85040820080400ULL,
    //         0x810102c808880400ULL,
    //         0xe900410884800ULL,
    //         0x8002020480840102ULL,
    //         0x220200865090201ULL,
    //         0x2010100a02021202ULL,
    //         0x152048408022401ULL,
    //         0x20080002081110ULL,
    //         0x4001001021004000ULL,
    //         0x800040400a011002ULL,
    //         0xe4004081011002ULL,
    //         0x1c004001012080ULL,
    //         0x8004200962a00220ULL,
    //         0x8422100208500202ULL,
    //         0x2000402200300c08ULL,
    //         0x8646020080080080ULL,
    //         0x80020a0200100808ULL,
    //         0x2010004880111000ULL,
    //         0x623000a080011400ULL,
    //         0x42008c0340209202ULL,
    //         0x209188240001000ULL,
    //         0x400408a884001800ULL,
    //         0x110400a6080400ULL,
    //         0x1840060a44020800ULL,
    //         0x90080104000041ULL,
    //         0x201011000808101ULL,
    //         0x1a2208080504f080ULL,
    //         0x8012020600211212ULL,
    //         0x500861011240000ULL,
    //         0x180806108200800ULL,
    //         0x4000020e01040044ULL,
    //         0x300000261044000aULL,
    //         0x802241102020002ULL,
    //         0x20906061210001ULL,
    //         0x5a84841004010310ULL,
    //         0x4010801011c04ULL,
    //         0xa010109502200ULL,
    //         0x4a02012000ULL,
    //         0x500201010098b028ULL,
    //         0x8040002811040900ULL,
    //         0x28000010020204ULL,
    //         0x6000020202d0240ULL,
    //         0x8918844842082200ULL,
    //         0x4010011029020020ULL,
    //     };

    //     if (p == BISHOP) {
    //         return u_bMagics[u_squ];
    //     } else {
    //         return u_rMagics[u_squ];
    //     }
    // }

    // void magics::initMagics()
    // {
    //     // generate relevant occupancy
    //     // trial and error multiply by u64 until a val found s.t:
    //     // occ * val = consec combination >> 64 - (masked)
    //     u64 start = 1;
    //     // for each square generate all possible blocker boards
    //     magics::initMasks();
    //     const array<n_types::e_piece, 2> a_pieces = {
    //         n_types::e_piece::BISHOP,
    //         n_types::e_piece::ROOK,
    //     };

    //     for (const n_types::piece p : a_pieces) {
    //         for (u8 u_square = 0; u_square < 64; u_square++) {
    //             const vector<bitboard> bb_blockers = magics::_genBlockers(u_square, p);
    //             // const u64 magic = magics::calculateMagic(u_square, bb_blockers, p);
    //             const u64 u_magic = magics::_getPreDefined(u_square, p);
    //             if (p == n_types::BISHOP) {
    //                 this->a_bishopMagics[u_square].u_magic = u_magic;
    //                 this->a_bishopMagics[u_square].u_mask = this->a_bishopMasks[u_square];
    //             } else {
    //                 this->a_rookMagics[u_square].u_magic = u_magic;
    //                 this->a_rookMagics[u_square].u_mask = this->a_rookMasks[u_square];
    //             }
    //         }
    //     }
    // }
}
