#include "../include/constants.hpp"
#include "../include/printer.hpp"
#include "bitboard.hpp"
#include "tConsts.hpp"
#include <sstream>
namespace n_b {
    string msg = "";
    bool ret = true;
    bool testFen() {
        n_types::board b;
        b.loadFen(n_consts::sv_fenStart);
        const string s_test = b.str();

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
        const string s_cmp = ss_cmp.str();
        ret = s_cmp == s_test;
        msg = "TEST LOAD FEN ";
        eval(ret, msg);
        if (!ret) {
            cout << "target:\n" << s_cmp << "actual:\n" << s_test;
        }
        return ret;
    }

    bool testPlayMove() {
        msg = "TEST PLAY MOVE ";

        n_types::move m_b = {
            .c_atk = black,
            .p_atk = n_types::PAWN,
            .s_atkOld = ((u64) 1 << 48),
            .s_atkNew = n_bbd::shiftOne(n_types::SOUTH, ((u64) 1 << 48)),
            .c_dfd = black,
            .p_dfd = n_types::PAWN,
            .s_dfdOld = 0,
        };

        n_types::move m_w {
            .c_atk = white,
                .p_atk = n_types::e_piece::PAWN,
                .s_atkOld = (1 << 11),
                .s_atkNew = n_bbd::shiftOne(n_types::NORTH, (1 << 11)),
                .c_dfd = white,
                .p_dfd = n_types::e_piece::PAWN,
                .s_dfdOld = 0,
        };

        n_types::board b;
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
        n_types::move m_b = {
            .c_atk = black,
            .p_atk = n_types::PAWN,
            .s_atkOld = ((u64) 1 << 48),
            .s_atkNew = n_bbd::shiftOne(n_types::SOUTH, ((u64) 1 << 48)),
            .c_dfd = black,
            .p_dfd = n_types::PAWN,
            .s_dfdOld = 0,
        };

        n_types::move m_w {
            .c_atk = white,
                .p_atk = n_types::e_piece::PAWN,
                .s_atkOld = (1 << 11),
                .s_atkNew = n_bbd::shiftOne(n_types::NORTH, (1 << 11)),
                .c_dfd = white,
                .p_dfd = n_types::e_piece::PAWN,
                .s_dfdOld = 0,
        };
        n_types::board b;
        b.loadFen(n_consts::sv_fenStart);
        b.playMove(m_w);
        b.unplayMove();
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
        b.unplayMove();
        b.unplayMove();
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

    bool tests() {
        return testFen() && testPlayMove() && testUnPlayMove();

    }
}
