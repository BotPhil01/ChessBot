#include "../include/types.hpp"
#include "../include/constants.hpp"
#include "../include/bitboard.hpp"
#include "../include/magics.hpp"
#include "../include/board.hpp"
#include <cctype>
#include <functional>
#include <iterator>
#include <memory>
#include <string>
#include <cassert>

namespace n_brd {
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

    using namespace n_types;
    board::board() = default;
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

    string board::_data2string() const {
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
        return s;
    }

    string board::str() const {
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
        for (u8 i = 0; i < this->p_white.a_bitboards.size(); i++) {
            this->p_white.a_bitboards[i].get() = 0;
            this->p_black.a_bitboards[i].get() = 0;
        }
        // assign boards
        for (u64 u_row = 0; u_row < 8; u_row++) {
            const string_view sv_part = _isolateEndFenPart(sv_pp, "/");
            u64 u_col = 0;
            for (auto it = sv_part.begin(); it != sv_part.end(); it++) {
                const char c = *it;
                if (isdigit(c)) {
                    u_col += (u8) c;
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
        bitboard &b = p->a_bitboards[p_bbIndex];
        // remove old
        b &= ~s_old;
        // add new
        b |= s_new;
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
        this->u_halfMoveClock++;
        if (this->u_halfMoveClock % 2 == 0) {
            this->u_fullMoveCounter++;
        }
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
    }

    player &board::playerToMove() {
        return this->c_sideToMove == white ? p_white : p_black;
    }

    player &board::playerToWatch() {
        return this->c_sideToMove == white ? p_black : p_white;
    }

    // search enemy square for piece type
    piece board::_square2piece(const square s) {
        player p_dfd = this->playerToWatch();
        auto a_bbs = p_dfd.a_bitboards;
        for (u8 i = 0; i < a_bbs.size(); i++) {
            if (a_bbs[i] & s) return a_pieces[i];
        }
        return EMPTY;
    }

    void board::_genPawnMoves(vector<cMove> &v_dst, const bitboard bb_friendlies) {
        const player &p_atk = this->playerToMove();
        const player &p_dfd = this->playerToMove();
        bitboard bb_oldAtks = p_atk.b_pawn;
        bitboard bb_isolated = bitIsolate(bb_oldAtks);
        bb_oldAtks &= ~bb_isolated;


        typedef const array<const reference_wrapper<bitboard>, 6> ptrtype;
        
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
            bb_oldAtks &= ~bb_isolated;
        }
    }

    bitboard board::_calcOccupied() const {
        bitboard bb_ret = 0;
        for (u8 i = 0; i < this->p_white.a_bitboards.size(); i++) {
            bb_ret |= p_white.a_bitboards[i];
            bb_ret |= p_black.a_bitboards[i];
        }
        return bb_ret;
    }

    void board::_genRookMoves(vector<cMove> &v_dst, const bitboard bb_friendlies) {
        const player &p_atk = this->playerToMove();
        const player &p_dfd = this->playerToMove();
        bitboard bb_oldAtks = p_atk.b_rook;
        bitboard bb_oldIsolated = bitIsolate(bb_oldAtks);
        bb_oldAtks &= ~bb_oldIsolated;

        bitboard bb_occupied =_calcOccupied();
        while (bb_oldIsolated) {
            bitboard bb_newAtks = m_magics.lookup(bb_oldIsolated, bb_occupied, ROOK) & ~bb_friendlies;
            bitboard bb_newIsolated = bitIsolate(bb_newAtks);
            bb_newAtks ^= bb_newIsolated;
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
                bb_newAtks ^= bb_newIsolated;
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
            bitboard bb_newAtks = m_magics.lookup(bb_oldIsolated, bb_occupied, BISHOP) & ~bb_friendlies;
            bitboard bb_newIsolated = bitIsolate(bb_newAtks);
            bb_newAtks ^= bb_newIsolated;
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
                bb_newAtks ^= bb_newIsolated;
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
            bb_newAtks ^= bb_newIsolated;
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
                bb_newAtks ^= bb_newIsolated;
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
                bitboard bb_newAtks = m_magics.lookup(bb_oldIsolated, bb_occupied, p_mask);
                bitboard bb_newIsolated = bitIsolate(bb_newAtks) & ~bb_friendlies;
                bb_newAtks ^= bb_newIsolated;
                while (bb_newIsolated) {
                    const piece p_dfd = _square2piece(bb_newIsolated); 
                    const colour c_dfd = p_dfd == EMPTY ? this->c_sideToMove : !this->c_sideToMove;
                    const bitboard bb_dfdOld = p_dfd == EMPTY ? 0 : bb_newIsolated;
                    v_dst.emplace_back(
                            this->c_sideToMove,
                            p_mask,
                            bb_oldIsolated,
                            bb_newIsolated,
                            c_dfd,
                            p_dfd,
                            bb_dfdOld
                            );
                    bb_newIsolated = bitIsolate(bb_newAtks);
                    bb_newAtks ^= bb_newIsolated;
                }
                bb_oldIsolated = bitIsolate(bb_oldAtks);
                bb_oldAtks &= ~bb_oldIsolated;
            }
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
                        bb_oldIsolated,
                        d.s_qCastle,
                        this->c_sideToMove,
                        ROOK,
                        d.s_qCastle
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
        auto a_bbs = playerToMove().a_bitboards;
        for (bitboard b : a_bbs) {
            bb_friendlies |= b;
        }
        vector<cMove> v_ret;
        _genPawnMoves(v_ret, bb_friendlies);
        _genRookMoves(v_ret, bb_friendlies);
        _genBishopMoves(v_ret, bb_friendlies);
        _genKnightMoves(v_ret, bb_friendlies);
        _genQueenMoves(v_ret, bb_friendlies);
        _genKingMoves(v_ret, bb_friendlies);
        return v_ret;
    }

    bool board::_isKingInCheck(const colour c) {
        const vector<cMove> moves = this->genLegalMoves();
        for (const cMove m : moves) {
            if (m.p_dfd == KING && m.c_dfd == c) {
                return true;
            }
        }
        return false;
    }
    vector<cMove> board::genLegalMoves() {
        vector<cMove> v_ret;
        const vector<cMove> v_pseudo = this->genPseudoLegalMoves();

        for (const cMove m : v_pseudo) {
            this->playMove(m);
            if (!this->_isKingInCheck(!this->c_sideToMove)) {
                v_ret.emplace_back(m);
            }
            this->unPlayMove();
        }

        return v_ret;
    }
}
