#include <iostream>
#include <cassert>
#include "../include/types.hpp"
#include "../include/search.hpp"
#include "../include/board.hpp"
#include "../include/printer.hpp"
#include "../include/constants.hpp"

using namespace std;
using namespace n_types;
using namespace n_brd;
using namespace n_prt;
using namespace n_consts;
void _gameLoop() {
    cout << "Starting game loop\n";
    board b;
    b.loadFen(sv_fenStart);
    string s_in;
    // a2a3\r\0
    bprint(b);
    while (true) {
        cin >> s_in;
        if (s_in == "quit") break;
        cMove m = b.alg2move(s_in);
        if (m == m_empty) goto error;
        b.playMove(m);
        bprint(b);
        // check game over
        if (b.isGameOver() != NOEND) goto end;
        m = n_sch::search(b).second;
        b.playMove(m);
        bprint(b);
        // check game over
        if (b.isGameOver() != NOEND) goto end;
        continue;
error:
        cout << "Error processing move. Try again\n";
        bprint(b);
    }
end:
    cout << "Game over, thanks for playing\n";
}

int main() {
    cout << "Starting chess engine\n";
    _gameLoop();
}

