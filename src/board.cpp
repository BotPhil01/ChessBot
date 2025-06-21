#include "../include/types.hpp"
#include "../include/constants.hpp"
#include <cctype>
#include <memory>
namespace n_types {
    class board {
        private:

            // fen consists of
            // piece placement
            // side to move
            // castling ability
            // en passant target
            // half move clock
            // full move counter
            player p_white;
            player p_black;
            colour c_sideToMove = white;
            bitboard b_enPassantDst;
            u8 u_halfMoveClock = 0;
            u64 u_fullMoveCounter = 0;

            void _alterSrc(unique_ptr<player> &p, const move &m) {
                if (p == nullptr) return;
                const u8 u_bbIndex = m.p_src;
                bitboard &b = p->a_bitboards[u_bbIndex];
                // remove src from b
                b = b & ~m.b_src;
                // add attack to b
                b = b | m.b_dst;
            }

            void _alterDst(unique_ptr<player> &p, const move &m) {
                if (p == nullptr) return;
                const u8 u_bbIndex = m.p_dst;
                bitboard &b = p->a_bitboards[u_bbIndex];
                // remove dst from b
                b = b & ~m.b_dst;
            }

            void _parsePP(string_view sv_pp) {
                // zero boards
                for (u8 i = 0; i < p_white.a_bitboards.size(); i++) {
                    p_white.a_bitboards[i].get() = 0;
                    p_black.a_bitboards[i].get() = 0;
                }
                // assign boards
                for (u8 i = 0; i < 8; i++) {
                    const string_view sv_part = _isolateFenPart(sv_pp, "/");
                    u8 u_pos = 0;
                    for (auto it = sv_part.begin(); it != sv_part.end(); i++) {
                        char c = *it;
                        if (isdigit(c)) {
                            u_pos += (u8) c;
                            continue;
                        }
                        piece p = cPiece2Index(c);
                        if (isupper(c)) {
                            p_white.a_bitboards[p] |= (1 << (8 * i + u_pos));
                        }
                    }
                }
            }

            void _parseSTM(string_view sv_pp) {
                if (sv_pp.front() == 'w') {
                    c_sideToMove = white;
                } else {
                    c_sideToMove = black;
                }
            }

            void _parseCA(string_view sv_pp) {
                p_white.b_kCastle = false;
                p_white.b_qCastle = false;
                p_black.b_kCastle = false;
                p_black.b_qCastle = false;
                for (auto it = sv_pp.begin(); it != sv_pp.end(); it++) {
                    switch (*it) {
                        case 'K':
                            p_white.b_kCastle = true;
                            break;
                        case 'Q':
                            p_white.b_qCastle = true;
                            break;
                        case 'k':
                            p_black.b_kCastle = true;
                            break;
                        case 'q':
                            p_black.b_qCastle = true;
                            break;
                    }

                }
            }

            void _parseEP(string_view sv_pp) {
                u64 u_col = 0;
                u64 u_row = 0;
                for (auto it = sv_pp.begin(); it != sv_pp.end(); it++) {
                    char c = *it;
                    if (c == '-') {
                        b_enPassantDst = 0;
                    } else if (isdigit(c)) {
                        u_row = c - '0';
                    } else {
                        u_col = c - 'a';
                    }
                }
                b_enPassantDst = (1 << (u_row * 8 + u_col));
            }

            void _parseHMC(string_view sv_pp) {
                const string s = string(sv_pp);
                u_halfMoveClock = atoi(s.c_str());
            }
            void _parseFMC(string_view sv_pp) {
                const string s = string(sv_pp);
                u_fullMoveCounter = atoi(s.c_str());
            }
            string_view _isolateFenPart(string_view &sv_fen, const string_view sv_separator) {
                const size_t s_end = sv_fen.find_first_of(sv_separator);
                if (s_end == sv_fen.npos) return ""sv; 
                const string_view sv_part = sv_fen.substr(s_end);
                sv_fen.remove_prefix(s_end);
                return sv_part;
            }
        public:
            int loadFen(string_view sv_fen) {
                const u8 u_fenParts = 6;
                // isolate part 
                // parse part

                // fen consists of
                // piece placement
                // side to move
                // castling ability
                // en passant target
                // half move clock
                // full move counter
                void (n_types::board::*a_func[]) (const string_view) = {
                    &n_types::board::_parsePP,
                    &n_types::board::_parseSTM,
                    &n_types::board::_parseCA,
                    &n_types::board::_parseEP,
                    &n_types::board::_parseHMC,
                    &n_types::board::_parseFMC,
                };
                for (u8 i = 0; i < u_fenParts; i++) {
                    const string_view sv_pp = _isolateFenPart(sv_fen, " "sv);
                    if (sv_pp.empty()) return 1;
                    void (n_types::board::*f_fen) (string_view) = a_func[i];
                    (this->*f_fen)(sv_pp);
                }
                return 0;
            }

            void playMove(const move m) {
                unique_ptr<player> up_src = nullptr;
                unique_ptr<player> up_dst = nullptr;
                if (m.c_src == white) {
                    up_src = make_unique<player>(p_white);
                } else {
                    up_src = make_unique<player>(p_black);
                }
                if (m.c_dst == white) {
                    up_dst = make_unique<player>(p_white);
                } else {
                    up_dst = make_unique<player>(p_black);
                }
                _alterSrc(up_src, m);
                _alterDst(up_dst, m);

            }
            void unplayMove(move m);
            vector<move> genLegMoves();
            vector<move>  genPseudoLegalMoves();
    };
}
