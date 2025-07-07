#include "../include/constants.hpp"
#include "../include/printer.hpp"
#include "../include/board.hpp"
#include "bitboard.hpp"
#include "tConsts.hpp"
#include <sstream>
namespace n_b {
    using namespace n_prt;
    using namespace n_brd;
    using namespace n_bbd;
    using namespace n_types;
    string msg = "";
    bool ret = true;
    bool testFen() {
        board b;
        b.loadFen(n_consts::sv_fenStart);
        string s_test = b.str();

        stringstream ss_cmp;
        ss_cmp << 
            "r n b q k b n r" << "\n" <<
            "p p p p p p p p" << "\n" <<
            "x x x x x x x x" << "\n" <<
            "x x x x x x x x" << "\n" <<
            "x x x x x x x x" << "\n" <<
            "x x x x x x x x" << "\n" <<
            "P P P P P P P P" << "\n" <<
            "R N B Q K B N R" << "\n" <<
            "side to move: white" << "\n" <<
            "en passant target: none" << "\n" <<
            "half move clock: 0" << "\n" <<
            "full move clock: 1" << "\n";
        string s_cmp = ss_cmp.str();
        msg = "TEST LOAD FEN ";
        ret = s_cmp == s_test;
        if (!ret) {
            goto end;
        }
        b.loadFen("7K/2B2P2/Q3R2N/8/8/r2n2q1/2p2b2/k7 w - - 0 1"sv);
        ss_cmp.str(string());
        ss_cmp <<
            "x x x x x x x K" << "\n" <<
            "x x B x x P x x" << "\n" <<
            "Q x x x R x x N" << "\n" <<
            "x x x x x x x x" << "\n" <<
            "x x x x x x x x" << "\n" <<
            "r x x n x x q x" << "\n" <<
            "x x p x x b x x" << "\n" <<
            "k x x x x x x x" << "\n" <<
            "side to move: white" << "\n" <<
            "en passant target: none" << "\n" <<
            "half move clock: 0" << "\n" <<
            "full move clock: 1" << "\n";
        s_cmp = ss_cmp.str();
        s_test = b.str();
        ret = s_cmp == s_test;
end:
        eval(ret, msg);
        if (!ret) {
            cout << "target:\n" << s_cmp << "actual:\n" << s_test;
        }
        return ret;
    }

    bool testPlayMove() {
        msg = "TEST PLAY MOVE ";

        cMove m_b (
            black,
            PAWN,
            ((u64) 1 << 48),
            n_bbd::shiftOne(SOUTH, ((u64) 1 << 48)),
            black,
            PAWN,
            0
        );

        cMove m_w (
            white,
                e_piece::PAWN,
                (1 << 11),
                n_bbd::shiftOne(NORTH, (1 << 11)),
                white,
                e_piece::PAWN,
                0
        );

        board b;
        b.loadFen(n_consts::sv_fenStart);
        b.playMove(m_w);
        string s_test = b.str();
        stringstream ss_cmp;
        ss_cmp << 
            "r n b q k b n r" << "\n" <<
            "p p p p p p p p" << "\n" <<
            "x x x x x x x x" << "\n" <<
            "x x x x x x x x" << "\n" <<
            "x x x x x x x x" << "\n" <<
            "x x x P x x x x" << "\n" <<
            "P P P x P P P P" << "\n" <<
            "R N B Q K B N R" << "\n" <<
            "side to move: black" << "\n" <<
            "en passant target: none" << "\n" <<
            "half move clock: 1" << "\n" <<
            "full move clock: 1" << "\n";
        string s_cmp = ss_cmp.str();

        ret = s_cmp == s_test;
        if (!ret) {
            goto end;
            cout << "target:\n" << s_cmp << "actual:\n" << s_test;
        }

        b.playMove(m_b);

        s_test = b.str();
        ss_cmp.str(string());
        ss_cmp << 
            "r n b q k b n r" << "\n" <<
            "x p p p p p p p" << "\n" <<
            "p x x x x x x x" << "\n" <<
            "x x x x x x x x" << "\n" <<
            "x x x x x x x x" << "\n" <<
            "x x x P x x x x" << "\n" <<
            "P P P x P P P P" << "\n" <<
            "R N B Q K B N R" << "\n" <<
            "side to move: white" << "\n" <<
            "en passant target: none" << "\n" <<
            "half move clock: 2" << "\n" <<
            "full move clock: 2" << "\n";
        s_cmp.erase();
        s_cmp.append(ss_cmp.str());
end:
        ret = ret & (s_cmp == s_test);

        if (!ret) {
            cout << "target:\n" << s_cmp << "actual:\n" << s_test;
        }
        eval(ret, msg);
        return ret;
    }

    bool testUnPlayMove() {
        cMove m_b (
            black,
            PAWN,
            ((u64) 1 << 48),
            n_bbd::shiftOne(SOUTH, ((u64) 1 << 48)),
            black,
            PAWN,
            0
        );

        cMove m_w (
            white,
                e_piece::PAWN,
                (1 << 11),
                n_bbd::shiftOne(NORTH, (1 << 11)),
                white,
                e_piece::PAWN,
                0
        );
        board b;
        b.loadFen(n_consts::sv_fenStart);
        b.playMove(m_w);
        b.unPlayMove();
        string s_test = b.str();

        stringstream ss_cmp;
        ss_cmp << 
            "r n b q k b n r" << "\n" <<
            "p p p p p p p p" << "\n" <<
            "x x x x x x x x" << "\n" <<
            "x x x x x x x x" << "\n" <<
            "x x x x x x x x" << "\n" <<
            "x x x x x x x x" << "\n" <<
            "P P P P P P P P" << "\n" <<
            "R N B Q K B N R" << "\n" <<
            "side to move: white" << "\n" <<
            "en passant target: none" << "\n" <<
            "half move clock: 0" << "\n" <<
            "full move clock: 1" << "\n";
        string s_cmp = ss_cmp.str();
        ret = s_cmp == s_test;
        msg = "TEST PLAY MOVE ";
        if (!ret) {
            eval(ret, msg);
            cout << "target:\n" << s_cmp << "actual:\n" << s_test;
            return ret;
        }

        b.playMove(m_w);
        b.playMove(m_b);
        b.unPlayMove();
        b.unPlayMove();
        s_test.clear();
        s_test = b.str();

        ss_cmp.str(string());
        ss_cmp << 
            "r n b q k b n r" << "\n" <<
            "p p p p p p p p" << "\n" <<
            "x x x x x x x x" << "\n" <<
            "x x x x x x x x" << "\n" <<
            "x x x x x x x x" << "\n" <<
            "x x x x x x x x" << "\n" <<
            "P P P P P P P P" << "\n" <<
            "R N B Q K B N R" << "\n" <<
            "side to move: white" << "\n" <<
            "en passant target: none" << "\n" <<
            "half move clock: 0" << "\n" <<
            "full move clock: 1" << "\n";
        s_cmp = ss_cmp.str();
        ret = ret && (s_cmp == s_test);
        eval(ret, msg);
        if (!ret) {
            cout << "target:\n" << s_cmp << "actual:\n" << s_test;
        }
        return ret;
    }

    bool testGenMove() {
        board b;
        string_view sv_fen = sv_fenStart;
        b.loadFen(sv_fen);
        vector<cMove> v_moves = b.genPseudoLegalMoves();
        msg = "TEST GEN MOVE ";
        ret = v_moves.size() == 20;


        if (!ret) {
            goto end;
        }
        sv_fen = "PPP5/1q6/8/PPPP4/8/8/PP6/kP6 b - - 1 1";
        b.loadFen(sv_fen);
        v_moves = b.genPseudoLegalMoves();

        ret = v_moves.size() == 14;
end:
        eval(ret, msg);
            for (cMove m : v_moves) {
                mprint(m);
            }
        return true;
    }
    bool testGenMoveLegal() {
        string_view sv_fen = "2B4K/1P4q1/1B5k/1N2R3/8/3r1P2/8/8 w - - 0 1";
        board b;
        b.loadFen(sv_fen);
        vector<cMove> v_moves = b.genLegalMoves();
        ret = v_moves.size() == 0;
        msg = "TEST LEGAL GEN MOVE ";
        eval(ret, msg);
        bprint(b);
        return true;
    }
    bool tests() {
        return testFen() &&
            testPlayMove() &&
            testUnPlayMove() &&
            testGenMove() &&
            testGenMoveLegal();
    }
}
