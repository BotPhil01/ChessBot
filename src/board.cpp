#include "../include/types.hpp"
#include "../include/constants.hpp"
#include "../include/bitboard.hpp"
#include "../include/board.hpp"
#include "../include/sliding.hpp"
#include "../include/rands.hpp"
#include <cctype>
#include <functional>
#include <memory>
#include <string>
#include <cassert>
#include <thread>
#include <chrono>

// fen consists of
// piece placement
// side to cMove
// castling ability
// en passant target
// half cMove clock
// full cMove counter
// player board::p_white;
// player board::p_black;
// colour board::c_sideToMove = white;
// bitboard board::b_enPassantDst;
// u8 board::u_halfMoveClock = 0;
// u64 board::u_fullMoveCounter = 0;

using namespace n_brd;
using namespace n_types;
using namespace n_bbd;
using namespace n_rnd;
board::board() {}

string board::_pos2string() const {
    const u8 u_bbDimension = 8;
    string s_working = "";
    const string s_separator = " ";
    bitboard mask = 1;

    // want to read each row 
    for (u8 u_row = 0; u_row < u_bbDimension; u_row++) {
        string s_row = "";
        for (u8 u_col = 0; u_col < u_bbDimension; u_col++) {
            const char c = this->_getChar(mask);
            s_row += c;
            if (u_col == u_bbDimension - 1) {
                s_row += '\n';
            } else {
                s_row += s_separator;
            }
            mask = mask << 1;
        }
        s_working = s_row + s_working;
    }
    return s_working;
}

string board::_data2string() {
    string s = "";
    s += "side to move: ";
    s += col2string(this->c_sideToMove) ;
    s += "\n";
    s += "en passant target: ";
    s += square2string(this->b_enPassantDst);
    s += "\n";
    s += "half move clock: ";
    s += (char) (this->u_halfMoveClock + '0');
    s += "\n";
    s += "full move clock: ";
    s += to_string(this->u_fullMoveCounter);
    s += "\n";
    s64 s_overCode = this->_isGameOver();
    s+= "Game result: ";
    switch (s_overCode) {
        case FIDDY:
            s += "stalemate 50 move rule";
            break;
        case STALEMATE:
            s += "stalemate no legal moves";
            break;
        case white:
            s += "white win";
            break;
        case black:
            s += "black win";
            break;
        default:
            s += "game continuing";
    }
    s+= "\n";
    return s;
}

string board::str() {
    std::string s;
    s += this->_pos2string();
    s += this->_data2string();
    return s;
}

char board::_getChar(const bitboard mask) const {
    for (u8 i = 0; i < p_white.a_bitboards.size(); i++) {
        const bitboard b_w = p_white.a_bitboards[i];
        const bitboard b_b = p_black.a_bitboards[i];
        if ((b_w & mask) != 0) {
            return n_consts::sv_fenWPieces[i];
        } else if ((b_b & mask) != 0) {
            return n_consts::sv_fenBPieces[i];
        }
    }
    return 'x';
}

void board::_parsePP(string_view sv_pp) {
    // zero boards
    // cout << this <<"\n";
    // cout << &this->p_white.a_bitboards[0].get() <<"\n";
    // cout << &this->p_black.a_bitboards[0].get() <<endl;
    // using namespace chrono_literals;
    // this_thread::sleep_for(5000ms);
    for (u8 i = 0; i < this->p_white.a_bitboards.size(); i++) {
        this->p_white.a_bitboards.at(i).get() = 0;
        this->p_black.a_bitboards.at(i).get() = 0;
    }
    // assign boards
    for (u64 u_row = 0; u_row < 8; u_row++) {
        const string_view sv_part = _isolateEndFenPart(sv_pp, "/");
        u64 u_col = 0;
        for (auto it = sv_part.begin(); it != sv_part.end(); it++) {
            const char c = *it;
            if (isdigit(c)) {
                u_col += (u64) (c - '0');
                continue;
            }

            const u64 u_shift = 8 * u_row + u_col;
            const bitboard bb_mask = ((u64) 1) << u_shift;
            const piece p = piece2Index(c);
            if (isupper(c)) {
                this->p_white.a_bitboards[p] |= bb_mask;
            } else {
                this->p_black.a_bitboards[p] |= bb_mask;
            }
            u_col++;
        }
    }
}

void board::_parseSTM(string_view sv_pp) {
    if (sv_pp.front() == 'w') {
        this->c_sideToMove = white;
    } else {
        this->c_sideToMove = black;
    }
}

void board::_parseCA(string_view sv_pp) {
    this->p_white.b_kCastle = false;
    this->p_white.b_qCastle = false;
    this->p_black.b_kCastle = false;
    this->p_black.b_qCastle = false;
    for (auto it = sv_pp.begin(); it != sv_pp.end(); it++) {
        switch (*it) {
            case 'K':
                this->p_white.b_kCastle = true;
                break;
            case 'Q':
                this->p_white.b_qCastle = true;
                break;
            case 'k':
                this->p_black.b_kCastle = true;
                break;
            case 'q':
                this->p_black.b_qCastle = true;
                break;
        }

    }
}

    void board::_parseEP(string_view sv_pp) {
        u64 u_col = 0;
        u64 u_row = 0;
        for (auto it = sv_pp.begin(); it != sv_pp.end(); it++) {
            char c = *it;
            if (c == '-') {
                this->b_enPassantDst = 0;
                return;
            } else if (isdigit(c)) {
                u_row = c - '0';
            } else {
                u_col = c - 'a';
            }
        }
        this->b_enPassantDst = (1 << (u_row * 8 + u_col));
    }

    void board::_parseHMC(string_view sv_pp) {
        const string s = string(sv_pp);
        this->u_halfMoveClock = atoi(s.c_str());
    }

    void board::_parseFMC(string_view sv_pp) {
        const string s = string(sv_pp);
        this->u_fullMoveCounter = atoi(s.c_str());
    }

    string_view board::_isolateEndFenPart(string_view &sv_fen, const string_view sv_separator) {
        const size_t s_beg = sv_fen.find_last_of(sv_separator);
        if (s_beg == sv_fen.npos) return sv_fen;
        const string_view sv_part = sv_fen.substr(s_beg + 1);
        sv_fen.remove_suffix(sv_part.size() + 1);
        return sv_part;

    }
    string_view board::_isolateFenPart(string_view &sv_fen, const string_view sv_separator) {
        const size_t s_end = sv_fen.find_first_of(sv_separator);
        if (s_end == sv_fen.npos) return sv_fen;
        const string_view sv_part = sv_fen.substr(0, s_end);
        sv_fen.remove_prefix(s_end + 1);
        return sv_part;
    }

    int board::loadFen(string_view sv_fen) {
        const u8 u_fenParts = 6;
        // isolate part 
        // parse part

        // fen consists of
        // piece placement
        // side to cMove
        // castling ability
        // en passant target
        // half cMove clock
        // full cMove counter
        void (board::*a_func[]) (const string_view) = {
            &board::_parsePP,
            &board::_parseSTM,
            &board::_parseCA,
            &board::_parseEP,
            &board::_parseHMC,
            &board::_parseFMC,
        };
        for (u8 i = 0; i < u_fenParts; i++) {
            const string_view sv_pp = _isolateFenPart(sv_fen, " "sv);
            if (sv_pp.empty()) return 1;
            void (board::*f_fen) (string_view) = a_func[i];
            (this->*f_fen)(sv_pp);
        }
        this->u_zob = zobrist(*this);
        return 0;
    }

    /*
     * Extracts and alters the bitboard according to params
     * @param p = player to extract bitboard from
     * @param u_bbIndex = piece type of bitboard
     * @param s_old = square to be removed
     * @param s_new = square to be added
     */
    void board::_extractBitboardAndAlter(
            unique_ptr<player> &p,
            const piece p_bbIndex,
            const square s_old,
            const square s_new) {
        assert(p != nullptr);
        if (p_bbIndex == EMPTY) return;
        bitboard &b = p->a_bitboards.at(p_bbIndex);
        // remove old
        b = b & ~s_old;
        // add new
        b = b | s_new;
        assert(bitPopCount(this->p_white.b_pawn) <= 8);
        assert(bitPopCount(this->p_black.b_pawn) <= 8);
        assert(bitPopCount(b) <= 8);
    }

    void board::_playAtk(unique_ptr<player> &p, const cMove &m) {
        if (p == nullptr) return;
        this->_extractBitboardAndAlter(p, m.p_atk, m.s_atkOld, m.s_atkNew);
    }

    void board::_playDfd(unique_ptr<player> &p, const cMove &m) {
        if (p == nullptr) return;
        this->_extractBitboardAndAlter(p, m.p_dfd, m.s_dfdOld, m.s_dfdNew);
    }

    void board::_unPlayAtk(unique_ptr<player> &p, const cMove &m) {
        if (p == nullptr) return;
        this->_extractBitboardAndAlter(p, m.p_atk, m.s_atkNew, m.s_atkOld);
    }

    void board::_unPlayDfd(unique_ptr<player> &p, const cMove &m) {
        if (p == nullptr) return;
        this->_extractBitboardAndAlter(p, m.p_dfd, m.s_dfdNew, m.s_dfdOld);
    }

    void board::_sethmc(const cMove m) {
        if (
                !m.isQuiet() ||
                m.isCastling() ||
                m.isPromoting() ||
                m.p_atk == PAWN
           ) {
            this->u_halfMoveClock = 0;
        } else {
            this->u_halfMoveClock++;
        }
    }

    void board::_alterZob(const cMove m) {
        u32 u_cMul = m.c_atk == white ? 1 : 2;
        piece p_atk = m.p_atk;
        u32 u_index = square2index(m.s_atkOld);
        u_index *= p_atk * u_cMul;
        this->u_zob ^= a_rands[u_index];

        if (m.isPromoting()) {
            p_atk = QUEEN;
        }
        u_index = square2index(m.s_atkNew);
        u_index *= p_atk * u_cMul;
        this->u_zob ^= a_rands[u_index];

        if (m.p_dfd != n_types::EMPTY && m.s_dfdNew) {
            piece p_dfd = m.p_dfd;
            u_cMul = m.c_dfd == white ? 1 : 2;
            u_index = square2index(m.s_dfdNew);
            u_index *= p_dfd * u_cMul;
            this->u_zob ^= a_rands[u_index];
        }
        
        // ep
        const array<u32, 8> a_enPassant = {
            56, 57, 58, 59, 
            60, 61, 62, 63, 
        };
        if (this->b_enPassantDst) {
            u32 u_epIndex = a_enPassant[this->b_enPassantDst % 8];
            this->u_zob ^= a_rands[u_epIndex];
        }

        // colour to play
        this->u_zob ^= a_rands[0];
    }

    void board::playMove(const cMove m) {
        unique_ptr<player> up_atk = nullptr;
        unique_ptr<player> up_dfd = nullptr;
        if (m.c_atk == white) {
            up_atk = make_unique<player>(this->p_white);
        } else {
            up_atk = make_unique<player>(this->p_black);
        }
        if (m.c_dfd == white) {
            up_dfd = make_unique<player>(this->p_white);
        } else {
            up_dfd = make_unique<player>(this->p_black);
        }
        this->_playAtk(up_atk, m);
        this->_playDfd(up_dfd, m);
        this->c_sideToMove = !this->c_sideToMove;
        this->s_moveHistory.push({
                .m = m,
                .u_halfMoveClock = this->u_halfMoveClock,
                .b_enPassantDst = this->b_enPassantDst,
                });

        // this->u_halfMoveClock++;
        this->_sethmc(m);
        if (m.c_atk == black) {
            this->u_fullMoveCounter++;
        }
        this->_alterZob(m);
    }

    void board::unPlayMove() {
        assert(!this->s_moveHistory.empty());
        const hisItem hi_state = this->s_moveHistory.top();
        const cMove m = hi_state.m;
        this->s_moveHistory.pop();
        unique_ptr<player> up_atk = nullptr;
        unique_ptr<player> up_dfd = nullptr;
        if (m.c_atk == white) {
            up_atk = make_unique<player>(this->p_white);
        } else {
            up_atk = make_unique<player>(this->p_black);
        }
        if (m.c_dfd == white) {
            up_dfd = make_unique<player>(this->p_white);
        } else {
            up_dfd = make_unique<player>(this->p_black);
        }
        this->_unPlayAtk(up_atk, m);
        this->_unPlayDfd(up_dfd, m);
        this->c_sideToMove = !this->c_sideToMove;
        this->u_halfMoveClock = hi_state.u_halfMoveClock;
        this->b_enPassantDst = hi_state.b_enPassantDst;
        if (this->c_sideToMove == black) {
            this->u_fullMoveCounter--;
        }
        this->_alterZob(m);
        assert(bitPopCount(this->p_white.b_pawn) <= 8);
}

player &board::playerToMove() {
    return (this->c_sideToMove == white) ? p_white : p_black;
}

player &board::playerToWatch() {
    return (this->c_sideToMove == white) ? p_black : p_white;
}

// search enemy square for piece type
piece board::_square2piece(const square s) {
    for (player p_dfd : {p_white, p_black}) {
        // player p_dfd = this->playerToWatch();
        auto a_bbs = p_dfd.a_bitboards;
        for (u8 i = 0; i < a_bbs.size(); i++) {
            if (a_bbs[i] & s) return a_pieces[i];
        }
    }
    return EMPTY;
}

void board::_genPawnMoves(vector<cMove> &v_dst, const bitboard bb_friendlies) {
    const player &p_atk = this->playerToMove();
    const player &p_dfd = this->playerToMove();
    bitboard bb_oldAtks = p_atk.b_pawn;
    bitboard bb_isolated = bitIsolate(bb_oldAtks);
    bb_oldAtks = bb_oldAtks & ~bb_isolated;


    typedef const array<reference_wrapper<bitboard>, 6> ptrtype;

    unique_ptr<ptrtype> up_bbDfd = make_unique<ptrtype>(p_dfd.a_bitboards);

    while (bb_isolated) {
        const pShiftData d = pShift(
                bb_isolated,
                this->c_sideToMove,
                this->b_enPassantDst,
                up_bbDfd);
        if (d.s_fwd && independent(d.s_fwd, bb_friendlies)) {
            v_dst.emplace_back(
                    this->c_sideToMove,
                    PAWN,
                    bb_isolated,
                    d.s_fwd,
                    this->c_sideToMove,
                    EMPTY,
                    0
                    );
        }

        if (d.s_fwd2 && independent(bb_friendlies, d.s_fwd2)) {
            v_dst.emplace_back(
                    this->c_sideToMove,
                    PAWN,
                    bb_isolated,
                    d.s_fwd2,
                    this->c_sideToMove,
                    EMPTY,
                    0
                    );
        }

        if (d.s_eCap && independent(bb_friendlies, d.s_eCap)) {
            v_dst.emplace_back(
                    this->c_sideToMove,
                    PAWN,
                    bb_isolated,
                    d.s_eCap,
                    !this->c_sideToMove,
                    _square2piece(d.s_eCap),
                    d.s_eCap
                    );
        }

        if (d.s_wCap && independent(bb_friendlies, d.s_wCap)) {
            v_dst.emplace_back(
                    this->c_sideToMove,
                    PAWN,
                    bb_isolated,
                    d.s_wCap,
                    !this->c_sideToMove,
                    _square2piece(d.s_wCap),
                    d.s_wCap
                    );
        }

        if (d.s_eep) {
            v_dst.emplace_back(
                    this->c_sideToMove,
                    PAWN,
                    bb_isolated,
                    d.s_eep,
                    !this->c_sideToMove,
                    PAWN,
                    this->b_enPassantDst
                    );
        }

        if (d.s_eep) {
            v_dst.emplace_back(
                    this->c_sideToMove,
                    PAWN,
                    bb_isolated,
                    d.s_eep,
                    !this->c_sideToMove,
                    PAWN,
                    this->b_enPassantDst
                    );
        }
        bb_isolated = bitIsolate(bb_oldAtks);
        bb_oldAtks = bb_oldAtks & ~bb_isolated;
    }
}

bitboard board::_calcOccupied() const {
    bitboard bb_ret = 0;
    for (u8 i = 0; i < this->p_white.a_bitboards.size(); i++) {
        bb_ret |= this->p_white.a_bitboards[i];
        bb_ret |= this->p_black.a_bitboards[i];
    }
    return bb_ret;
}

void board::_genRookMoves(vector<cMove> &v_dst, const bitboard bb_friendlies) {
    const player &p_atk = this->playerToMove();
    const player &p_dfd = this->playerToMove();
    bitboard bb_oldAtks = p_atk.b_rook;
    bitboard bb_oldIsolated = bitIsolate(bb_oldAtks);
    bb_oldAtks = bb_oldAtks & ~bb_oldIsolated;

    bitboard bb_occupied =_calcOccupied();
    while (bb_oldIsolated) {
        // bitboard bb_newAtks = n_mgc::lookup(bb_oldIsolated, bb_occupied, ROOK) & ~bb_friendlies;
        bitboard bb_newAtks = n_sld::lookup(bb_oldIsolated, bb_occupied, ROOK);
        bb_newAtks = bb_newAtks & ~bb_friendlies;
        bitboard bb_newIsolated = bitIsolate(bb_newAtks);
        bb_newAtks &= ~bb_newIsolated;
        while (bb_newIsolated) {
            const piece p_dfd = _square2piece(bb_newIsolated); 
            const colour c_dfd = p_dfd == EMPTY ? this->c_sideToMove : !this->c_sideToMove;
            const bitboard bb_dfdOld = p_dfd == EMPTY ? 0 : bb_newIsolated;
            v_dst.emplace_back(
                    this->c_sideToMove,
                    ROOK,
                    bb_oldIsolated,
                    bb_newIsolated,
                    c_dfd,
                    p_dfd,
                    bb_dfdOld
                    );
            bb_newIsolated = bitIsolate(bb_newAtks);
            bb_newAtks &= ~bb_newIsolated;
        }
        bb_oldIsolated = bitIsolate(bb_oldAtks);
        bb_oldAtks &= ~bb_oldIsolated;
    }
}
void board::_genBishopMoves(vector<cMove> &v_dst, const bitboard bb_friendlies) {
    const player &p_atk = this->playerToMove();
    bitboard bb_oldAtks = p_atk.b_bishop;
    bitboard bb_oldIsolated = bitIsolate(bb_oldAtks);
    bb_oldAtks &= ~bb_oldIsolated;

    bitboard bb_occupied =_calcOccupied();
    while (bb_oldIsolated) {
        // bitboard bb_newAtks = n_mgc::lookup(bb_oldIsolated, bb_occupied, BISHOP) & ~bb_friendlies;
        bitboard bb_newAtks = n_sld::lookup(bb_oldIsolated, bb_occupied, BISHOP); 
        bb_newAtks &= ~bb_friendlies;
        bitboard bb_newIsolated = bitIsolate(bb_newAtks);
        bb_newAtks &= ~bb_newIsolated;
        while (bb_newIsolated) {
            const piece p_dfd = _square2piece(bb_newIsolated); 
            const colour c_dfd = p_dfd == EMPTY ? this->c_sideToMove : !this->c_sideToMove;
            const bitboard bb_dfdOld = p_dfd == EMPTY ? 0 : bb_newIsolated;
            v_dst.emplace_back(
                    this->c_sideToMove,
                    BISHOP,
                    bb_oldIsolated,
                    bb_newIsolated,
                    c_dfd,
                    p_dfd,
                    bb_dfdOld
                    );
            bb_newIsolated = bitIsolate(bb_newAtks);
            bb_newAtks &= ~bb_newIsolated;
        }
        bb_oldIsolated = bitIsolate(bb_oldAtks);
        bb_oldAtks &= ~bb_oldIsolated;
    }
}
void board::_genKnightMoves(vector<cMove> &v_dst, const bitboard bb_friendlies) {
    const player &p_atk = this->playerToMove();
    bitboard bb_oldAtks = p_atk.b_knight;
    bitboard bb_oldIsolated = bitIsolate(bb_oldAtks);
    bb_oldAtks &= ~bb_oldIsolated;

    while (bb_oldIsolated) {
        bitboard bb_newAtks = nShift(bb_oldIsolated) & ~bb_friendlies;
        bitboard bb_newIsolated = bitIsolate(bb_newAtks);
        bb_newAtks &= ~bb_newIsolated;
        while (bb_newIsolated) {
            const piece p_dfd = _square2piece(bb_newIsolated); 
            const colour c_dfd = p_dfd == EMPTY ? this->c_sideToMove : !this->c_sideToMove;
            const bitboard bb_dfdOld = p_dfd == EMPTY ? 0 : bb_newIsolated;
            v_dst.emplace_back(
                    this->c_sideToMove,
                    KNIGHT,
                    bb_oldIsolated,
                    bb_newIsolated,
                    c_dfd,
                    p_dfd,
                    bb_dfdOld
                    );
            bb_newIsolated = bitIsolate(bb_newAtks);
            bb_newAtks &= ~bb_newIsolated;
        }
        bb_oldIsolated = bitIsolate(bb_oldAtks);
        bb_oldAtks &= ~bb_oldIsolated;
    }
}

void board::_genQueenMoves(vector<cMove> &v_dst, const bitboard bb_friendlies) {
    const player &p_atk = this->playerToMove();
    bitboard bb_oldAtks = p_atk.b_queen;
    bitboard bb_oldIsolated = bitIsolate(bb_oldAtks);
    bb_oldAtks &= ~bb_oldIsolated;

    const bitboard bb_occupied =_calcOccupied();
    while (bb_oldIsolated) {
        for (const piece p_mask : {BISHOP, ROOK}) {
            bitboard bb_newAtks = n_sld::lookup(
                    bb_oldIsolated,
                    bb_occupied,
                    p_mask);
            bb_newAtks &= ~bb_friendlies;
            bitboard bb_newIsolated = bitIsolate(bb_newAtks);
            bb_newAtks &= ~bb_newIsolated;
            while (bb_newIsolated) {
                const piece p_dfd = _square2piece(bb_newIsolated); 
                const colour c_dfd = p_dfd == EMPTY ? this->c_sideToMove : !this->c_sideToMove;
                const bitboard bb_dfdOld = p_dfd == EMPTY ? 0 : bb_newIsolated;
                v_dst.emplace_back(
                        this->c_sideToMove,
                        QUEEN,
                        bb_oldIsolated,
                        bb_newIsolated,
                        c_dfd,
                        p_dfd,
                        bb_dfdOld
                        );
                bb_newIsolated = bitIsolate(bb_newAtks);
                bb_newAtks &= ~bb_newIsolated;
            }
        }
        bb_oldIsolated = bitIsolate(bb_oldAtks);
        bb_oldAtks &= ~bb_oldIsolated;
    }
}

void board::_genKingMoves(vector<cMove> &v_dst, const bitboard bb_friendlies) {
    const player &p_atk = this->playerToMove();
    bitboard bb_oldAtks = p_atk.b_king;
    bitboard bb_oldIsolated = bitIsolate(bb_oldAtks);
    bb_oldAtks &= ~bb_oldIsolated;

    const bitboard bb_occupied =_calcOccupied();
    while (bb_oldIsolated) {
        kShiftData d = kShift(
                bb_oldIsolated,
                this->c_sideToMove,
                p_atk.b_kCastle,
                p_atk.b_qCastle,
                bb_occupied);

        for (bitboard bb_newIsolated : d.a_quiets) {
            if ((bb_friendlies & bb_newIsolated) || (bb_newIsolated == 0)) continue;
            const piece p_dfd = _square2piece(bb_newIsolated); 
            const colour c_dfd = p_dfd == EMPTY ? this->c_sideToMove : !this->c_sideToMove;
            const bitboard bb_dfdOld = p_dfd == EMPTY ? 0 : bb_newIsolated;
            v_dst.emplace_back(
                    this->c_sideToMove,
                    KING,
                    bb_oldIsolated,
                    bb_newIsolated,
                    c_dfd,
                    p_dfd,
                    bb_dfdOld
                    );
        }

        if (d.s_qCastle) {
            v_dst.emplace_back(
                    this->c_sideToMove,
                    KING,
                    bb_oldIsolated, // atkold
                    d.s_qCastle, // atknew
                    this->c_sideToMove,
                    ROOK,
                    d.s_qCastle // dfdold
                    );
        }
        if (d.s_kCastle) {
            v_dst.emplace_back(
                    this->c_sideToMove,
                    KING,
                    bb_oldIsolated,
                    d.s_kCastle,
                    this->c_sideToMove,
                    ROOK,
                    d.s_kCastle
                    );
        }
        bb_oldIsolated = bitIsolate(bb_oldAtks);
        bb_oldAtks &= ~bb_oldIsolated;
    }
}

vector<cMove> board::genPseudoLegalMoves() {
    bitboard bb_friendlies = 0;
    const array<reference_wrapper<bitboard>, 6>
        a_bbs = this->playerToMove().a_bitboards;
    for (bitboard b : a_bbs) {
        bb_friendlies |= b;
    }
    vector<cMove> v_ret;
    this->_genPawnMoves(v_ret, bb_friendlies);
    this->_genRookMoves(v_ret, bb_friendlies);
    this->_genBishopMoves(v_ret, bb_friendlies);
    this->_genKnightMoves(v_ret, bb_friendlies);
    this->_genQueenMoves(v_ret, bb_friendlies);
    this->_genKingMoves(v_ret, bb_friendlies);
    return v_ret;
}

bool board::_isKingInCheck(const colour c) {
    const vector<cMove> moves = this->genPseudoLegalMoves();
    for (const cMove m : moves) {
        if (m.p_dfd == KING && m.c_dfd == c) {
            return true;
        }
    }
    return false;
}

vector<cMove> _order(const vector<cMove> v_moves) {
    array<array<vector<cMove>, 7>, 7> v_attacks;
    // v_attacks[dfd][atk]
    for (const cMove m : v_moves) {
        v_attacks.at(m.p_dfd).at(m.p_atk).push_back(m);
    }
    vector<cMove> v_ret;
    for (s16 j = 1; j < 7; j++) {
        const s16 s = 6 - j;
        for (u16 i = 0; i < 7; i++) {
            const vector<cMove> v_tmp = v_attacks.at(s).at(i);
            for (const cMove m : v_tmp) {
                v_ret.push_back(m);
            }
        }
    }
    for (u16 i = 0; i < 7; i++) {
        const vector<cMove> v_tmp = v_attacks.at(6).at(i);
        for (const cMove m : v_tmp) {
            v_ret.push_back(m);
        }
    }
    return v_ret;
}
vector<cMove> board::genLegalMoves() {
    vector<cMove> v_ret;
    const vector<cMove> v_pseudo = this->genPseudoLegalMoves();
    // if i were to play the current move
    // could the other person take my king
    // if no then its a legal move
    // if yes then its ilegal (skip)
    // src = 0x1000
    // dst = 0x200000
    for (const cMove m : v_pseudo) {
        this->playMove(m);
        if (this->_isKingInCheck(!this->c_sideToMove)) {
            this->unPlayMove();
            continue;
        }
        v_ret.emplace_back(m);
        this->unPlayMove();
    }

    return _order(v_ret);
}

s64 board::_isGameOver() {
    if (this->genLegalMoves().size() == 0) {
        if (this->_isKingInCheck(this->c_sideToMove)) {
            return !this->c_sideToMove;
        }
        // current player to move has lost
        return STALEMATE;
    }
    if (this->u_halfMoveClock == 50) return FIDDY;
    return NOEND;
}

evl board::eval() {
    const s64 s_overResult = this->_isGameOver();
    switch(s_overResult) {
        case NOEND: return 
                        this->_evalMat() +
                            this->_evalPos() +
                            this->_evalMov();
        case FIDDY:
        case STALEMATE: return 0;
        case white: return INT64_MAX;
        case black: return INT64_MIN;
    }
    return INT64_MIN;
}

s64 board::evalInit() {
    if (this->playerToMove().c_self == white) {
        return INT64_MIN;
    }
    return INT64_MAX;
}

namespace {
#define SQUARES 64
    const s64 u_wPawn = 1;
    const s64 u_wRook = 5;
    const s64 u_wBishop = 3;
    const s64 u_wKnight = 3;
    const s64 u_wKing = 99999999999999;
    const s64 u_wQueen = 9;
    const array<s64, 6> a_pWeights = {
        u_wPawn,
        u_wRook,
        u_wBishop,
        u_wKnight,
        u_wKing,
        u_wQueen,
    };
    const array<s64, SQUARES> a_pTable = {
        0,  0,  0,  0,  0,  0,  0,  0,
        50, 50, 50, 50, 50, 50, 50, 50,
        10, 10, 20, 30, 30, 20, 10, 10,
        5,  5, 10, 25, 25, 10,  5,  5,
        0,  0,  0, 20, 20,  0,  0,  0,
        5, -5,-10,  0,  0,-10, -5,  5,
        5, 10, 10,-20,-20, 10, 10,  5,
        0,  0,  0,  0,  0,  0,  0,  0
    };
    const array<s64, SQUARES> a_rTable = {
        0,  0,  0,  0,  0,  0,  0,  0,
        5, 10, 10, 10, 10, 10, 10,  5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        0,  0,  0,  5,  5,  0,  0,  0
    };
    const array<s64, SQUARES> a_bTable = {
        -20,-10,-10,-10,-10,-10,-10,-20,
        -10,  0,  0,  0,  0,  0,  0,-10,
        -10,  0,  5, 10, 10,  5,  0,-10,
        -10,  5,  5, 10, 10,  5,  5,-10,
        -10,  0, 10, 10, 10, 10,  0,-10,
        -10, 10, 10, 10, 10, 10, 10,-10,
        -10,  5,  0,  0,  0,  0,  5,-10,
        -20,-10,-10,-10,-10,-10,-10,-20,
    };
    const array<s64, SQUARES> a_nTable = {
        -50,-40,-30,-30,-30,-30,-40,-50,
        -40,-20,  0,  0,  0,  0,-20,-40,
        -30,  0, 10, 15, 15, 10,  0,-30,
        -30,  5, 15, 20, 20, 15,  5,-30,
        -30,  0, 15, 20, 20, 15,  0,-30,
        -30,  5, 10, 15, 15, 10,  5,-30,
        -40,-20,  0,  5,  5,  0,-20,-40,
        -50,-40,-30,-30,-30,-30,-40,-50,
    };
    const array<s64, SQUARES> a_kTable = {
        -30,-40,-40,-50,-50,-40,-40,-30,
        -30,-40,-40,-50,-50,-40,-40,-30,
        -30,-40,-40,-50,-50,-40,-40,-30,
        -30,-40,-40,-50,-50,-40,-40,-30,
        -20,-30,-30,-40,-40,-30,-30,-20,
        -10,-20,-20,-20,-20,-20,-20,-10,
        20, 20,  0,  0,  0,  0, 20, 20,
        20, 30, 10,  0,  0, 10, 30, 20
    };
    const array<s64, SQUARES> a_qTable = {
        -20,-10,-10, -5, -5,-10,-10,-20,
        -10,  0,  0,  0,  0,  0,  0,-10,
        -10,  0,  5,  5,  5,  5,  0,-10,
        -5,  0,  5,  5,  5,  5,  0, -5,
        0,  0,  5,  5,  5,  5,  0, -5,
        -10,  5,  5,  5,  5,  5,  0,-10,
        -10,  0,  5,  0,  0,  0,  0,-10,
        -20,-10,-10, -5, -5,-10,-10,-20
    };

    const array<const array<s64, SQUARES>, 6> a_tables = {
        a_pTable,
        a_rTable,
        a_bTable,
        a_nTable,
        a_kTable,
        a_qTable,
    };
}

s64 board::_evalMat() const {
    // eval white and eval black
    s64 s_ret = 0;
    const array<const s64, 2> a_cWeights = {1, -1};
    for (u16 i = 0; i < 2; i++) {
        const s64 s_cWeight = a_cWeights[i];
        const player p = this->a_players[i].get();
        for (u16 j = 0; j < p.a_bitboards.size(); j++) {
            const s64 u_pop = (s64) bitPopCount(p.a_bitboards[j].get());
            const s64 s_pWeight = a_pWeights[j];
            s_ret += s_cWeight * u_pop * s_pWeight;
        }
    }
    return s_ret * 100;
}

s64 board::_evalMov() const {
    return 0;
}

s64 board::_evalPos() const {
    // eval white and eval black
    s64 s_ret = 0;
    const array<bool, 2> a_cRot = {false, true};
    const array<s64, 2> a_cWeight = {1, -1};
    for (u16 i = 0; i < 2; i++) {
        const bool b_rot = a_cRot[i];
        const player p = this->a_players[i].get();
        for (u16 j = 0; j < p.a_bitboards.size(); j++) {
            const array<s64, SQUARES> a_table = a_tables[j];
            bitboard bb_cur = p.a_bitboards[j];
            while (bb_cur) {
                square s = bitIsolate(bb_cur);
                bb_cur &= ~s;
                if (b_rot) s = mirror(s);
                const u16 u_index = square2index(s);
                s_ret += a_cWeight[i] * a_table[u_index];
            }
        }
    }
    return s_ret;
}
