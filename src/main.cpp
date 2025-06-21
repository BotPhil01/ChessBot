#include "../include/printer.hpp"
#include "../include/bitboard.hpp"
#include "../include/constants.hpp"
#include "../include/types.hpp"

int main() {
    const bitboard full = 0xFFFFFFFFFFFFFFFF;
    n_prt::bbprint(full);
}
