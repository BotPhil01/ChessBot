#include "types.hpp"
#include <array>

#pragma once
namespace n_consts {
    // shift masks
    constexpr u8 dirIndex(const n_types::dir d) {
        int i = 0;
        switch (d) {
            case n_types::e_dir::NORTHWEST:
                i++;
            case n_types::e_dir::WEST:
                i++;
            case n_types::e_dir::SOUTHWEST:
                i++;
            case n_types::e_dir::SOUTH:
                i++;
            case n_types::e_dir::SOUTHEAST:
                i++;
            case n_types::e_dir::EAST:
                i++;
            case n_types::e_dir::NORTHEAST:
                i++;
            case n_types::e_dir::NORTH:
                return i;
        }
        return i;
    }

    const u8 u_north = dirIndex(n_types::NORTH);
    const u8 u_northeast = dirIndex(n_types::NORTHEAST);
    const u8 u_east = dirIndex(n_types::EAST);
    const u8 u_southeast = dirIndex(n_types::SOUTHEAST);
    const u8 u_south = dirIndex(n_types::SOUTH);
    const u8 u_southwest = dirIndex(n_types::SOUTHWEST);
    const u8 u_west = dirIndex(n_types::WEST);
    const u8 u_northwest = dirIndex(n_types::NORTHWEST);

    const array<n_types::e_dir, 8> a_dirs = {
        n_types::e_dir::NORTH,
        n_types::e_dir::NORTHEAST,
        n_types::e_dir::EAST,
        n_types::e_dir::SOUTHEAST,
        n_types::e_dir::SOUTH,
        n_types::e_dir::SOUTHWEST,
        n_types::e_dir::WEST,
        n_types::e_dir::NORTHWEST,
    };

    const array<n_types::e_piece, 7> a_pieces = {
        n_types::e_piece::PAWN,
        n_types::e_piece::ROOK,
        n_types::e_piece::BISHOP,
        n_types::e_piece::KNIGHT,
        n_types::e_piece::KING,
        n_types::e_piece::QUEEN,
        n_types::e_piece::EMPTY,
    };

    const array<u64, 8> shiftMasks = {
        0xffffffffffffff00,
        0xfefefefefefefe00,
        0xfefefefefefefefe,
        0x00fefefefefefefe,
        0x00ffffffffffffff,
        0x007f7f7f7f7f7f7f,
        0x7f7f7f7f7f7f7f7f,
        0x7f7f7f7f7f7f7f00,
    };

    const string_view sv_fenStart = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

    const array<char, 6> sv_fenWPieces = {
        'P', 'R', 'B', 'N', 'K', 'Q'
    };
    const array<char, 6> sv_fenBPieces = {
        'p', 'r', 'b', 'n', 'k', 'q'
    };

    const n_types::cMove m_empty = {
        black,
        n_types::EMPTY,
        0,
        0,
        black,
        n_types::EMPTY,
        0,
        0};
}
