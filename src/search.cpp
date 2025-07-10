#include "../include/search.hpp"
#include "../include/types.hpp"
#include <atomic>
#include <condition_variable>
#include <functional>
#include <pthread.h>
#include <stop_token>
#include <thread>
#include <cstdint>
#include <ctime>

using namespace n_sch;
using namespace n_types;
using namespace n_brd;

namespace {
    bool _b_timeUp = false;
    au64 au_searched = 0;
    reference_wrapper<au64> r_counter = ref(au_searched);
}

s64 n_sch::search(board b) {
    jthread j_timer(_startTimer, ref(_b_timeUp));
    // cout << "bool address: " << &::_b_timeUp << '\n';

    stop_token st;
    s64 ret = _negamax(
            b,
            INT64_MIN,
            INT64_MAX,
            MAXDEPTH,
            st);

    if (!::_b_timeUp) {
        // cout << "n_sch::search(board b) finished before reaching maxdepth\n";
        if (j_timer.joinable()) j_timer.join();
        ::_b_timeUp = false;
    }
    cout << "searched nodes: " << au_searched << '\n';
    return ret;
}

s64 n_sch::_negamax(
        board b,
        s64 s_alpha,
        s64 s_beta,
        s16 s_depth,
        stop_token st) {
    if (st.stop_requested()) {
        return 0;
    }
    if (s_depth == 0) {
        // return quiesce();
        return b.eval();
    }
    const vector<cMove> v_moves = b.genLegalMoves();
    s64 s_bestEval = b.evalInit();
    u64 u_move;
    // do parallel search here
    // create a thread pool and spawn threads
    // check if there are finished & unprocessed thread
    // if there are finished but unprocessed threads 
    // update alpha accordingly
    // if beta cutoff met then interupt all threads before they finish

    searchPool pool(b, v_moves, s_alpha, s_beta, s_depth);
    s16 s_poolCode = pool.isFinished();
    while (s_poolCode != P_TERM && !st.stop_requested() && !_b_timeUp) {
        if (s_poolCode == P_EVAL) {
            s_poolCode = pool.isFinished();
            continue;
        }
        const pair<s64, s16> p_res = pool.nextEval();
        // const s64 s_curEval = p_res.first;
        const s64 s_curEval = p_res.first;
        if (s_curEval >= s_beta) {
            // beta cutoff here
            pool.stop();
            return s_beta;
        }
        if (s_curEval > s_alpha) {
            s_alpha = s_curEval;
        }
        s_poolCode = pool.isFinished();
    }
    return s_alpha;

    // for (u_move = 0; u_move < v_moves.size() && !::_b_timeUp; u_move++) {
    //     const cMove m = v_moves[u_move];
    //     // search
    //     b.playMove(m);
    //     s64 s_curEval = -_negamax(b, -s_beta, -s_alpha, s_depth-1);
    //     if (s_curEval >= s_beta) {
    //         b.unPlayMove();
    //         return s_beta;
    //     }
    //     if (s_curEval > s_alpha) {
    //         s_alpha = s_curEval;
    //     }
    //     b.unPlayMove();
    // }
    // return s_alpha;
}

s64 n_sch::_searchDepth(n_brd::board b, s16 s_depth) {
    cout << "starting _searchDepth: " << s_depth << '\n';
    cout << "timeup: " << ::_b_timeUp << '\n';
    stop_token st;

    return _negamax(b, INT64_MIN, INT64_MAX, s_depth, st);
}

void n_sch::_startTimer(reference_wrapper<bool> b_signal) {
    cout << "timer started\n";
    // cout << "bool address: " << &(b_signal.get()) << '\n';
    b_signal.get() = false;
    const time_t t_start = time(nullptr);
    time_t t_end = time(nullptr);
    while (difftime(t_end, t_start) < MAXTIME) {
        t_end = time(nullptr);
    }
    cout << "time up\n";
    b_signal.get() = true;
    // cout << b_signal << '\n';
}


void _resultStack::push(_result r) {
    this->m_access.lock();
    this->s.push(r);
    this->m_access.unlock();
}

_result _resultStack::pop() {
    this->m_access.lock();
    _result r = this->s.top();
    this->s.pop();
    this->m_access.unlock();
    return r;
}

bool _resultStack::empty() {
    this->m_access.lock();
    bool b = this->s.empty();
    this->m_access.unlock();
    return b;
}

void searchPool::stop() {
    for (s16 i = 0; i < this->v_threads.size(); i++) {
        v_threads[i].request_stop();
        v_threads[i].join();
    }
}

s16 searchPool::isFinished() {
    if (this->au_processed.load() == this->v_threads.size()) return P_TERM;
    if (!this->s_results.empty()) return P_PROC;
    return P_EVAL;
}

pair<s64, s16> searchPool::nextEval() {
    bs_availableIndices.acquire();
    r_counter.get()++;
    return s_results.pop();
}

n_sch::searchPool::searchPool(
        board b,
        const vector<cMove> v_moves,
        s64 s_alpha,
        s64 s_beta,
        s16 s_depth)
{
    // cout << "counter address: " << &(r_counter.get()) << '\n';
    for (s16 i = 0; i < v_moves.size(); i++) {
        board b_changed = b;
        b_changed.playMove(v_moves[i]);

        v_stops.emplace_back();
        auto func = bind(
                &n_sch::searchPool::_runWorker,
                this,
                r_counter,
                v_stops[i].get_token(),
                ref(this->s_results),
                i,
                b_changed,
                s_alpha,
                s_beta,
                s_depth);


        v_threads.emplace_back(func);
    }
}

void n_sch::searchPool::_runWorker(
        reference_wrapper<au64> au_nodeCounter,
        stop_token st_stop,
        reference_wrapper<_resultStack> s_results,
        s16 s_index,
        n_brd::board b,
        s64 s_alpha,
        s64 s_beta,
        s16 s_depth) {
    // cout << "counter address: " << &(r_counter.get()) << '\n';
    r_counter = au_nodeCounter;
    s64 s_eval = -_negamax(b, -s_beta, -s_alpha, s_depth-1, st_stop);
    s_results.get().push({s_eval, s_index});
    bs_availableIndices.release();
    // cout << "exiting: " << this_thread::get_id() << "\n";
}
