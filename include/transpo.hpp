#include "constants.hpp"
#include "types.hpp"
#include "board.hpp"
#include <array>
#pragma once
namespace n_brd {
    extern u64 _state;
    enum node {
        PV = 2,
        ALL = 1,
        CUT = -1,
    };
    typedef struct entry {
        u64 u_hash;
        n_types::cMove m_best = n_consts::m_empty;
        u16 u_depth;
        n_types::evl e;
        node n;
        u32 u_age;
        // 80 bytes 
    }entry;

    const entry e_empty = {
        0,
        n_consts::m_empty,
        0,
        0,
        node::PV,
        0
    };

    u64 zobrist(n_brd::board b);
#define _S_TABLE 50000000 

    u64 _zob2ind(const u64 u_zob) {
        return u_zob % _S_TABLE;
    }

    class table {
        public:
            entry query(const u64 u_hash) {
                return _a_entries[_zob2ind(u_hash)];
            }

            void add(const entry e_cand) {
                const u64 u_zob = _zob2ind(e_cand.u_hash);
                _a_entries[u_zob] = e_cand;
            }
        private:
            array<entry, _S_TABLE> _a_entries{e_empty};
    };
}
