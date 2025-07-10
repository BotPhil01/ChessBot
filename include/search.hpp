#include "types.hpp"
#include "board.hpp"
#include <atomic>
#include <functional>
#include <condition_variable>
#include <thread>
#include <stop_token>
#include <semaphore>
#pragma once
namespace n_sch {
    // max search depth
#define MAXDEPTH 2
    s64 search(n_brd::board b);
    s64 _negamax(
            n_brd::board b,
            s64 alpha,
            s64 beta,
            s16 depth,
            stop_token st);
    s64 _searchDepth(n_brd::board b, s16 depth);

    // search timer 
// max time set to value below (s)
#define MAXTIME 15 
    void _startTimer(reference_wrapper<bool> b_signal);

    typedef pair<s64, s16> _result;
    class _resultStack {
        private:
            stack<_result, list<_result>> s;
            mutex m_access;
        public:
            void push(_result r);
            _result pop();
            bool empty();
    };

    class searchPool {
        public:
            searchPool(
                    n_brd::board b,
                    vector<n_types::cMove> v_moves,
                    s64 s_alpha,
                    s64 s_beta,
                    s16 s_depth);

            /* retrieves the next evaluation available by the pool
             * @return pair of eval and move index
             */
            pair<s64, s16> nextEval();

#define P_PROC -1
#define P_EVAL 0
#define P_TERM 1
            /* @return <0 - branches awaiting processing P_PROC
             * @return 0 - remaining branches awaiting evaluation P_EVAL
             * @return >0 - all branches processed P_TERM
             */ 
            s16 isFinished();

            void stop();
        private:
            vector<jthread> v_threads;
            vector<stop_source> v_stops;
            au16 au_processed = 0;
            binary_semaphore bs_availableIndices {0};
            _resultStack s_results;
            void _runWorker(
                    reference_wrapper<au64> au_nodeCounter,
                    stop_token st_stop,
                    reference_wrapper<_resultStack> s_results,
                    s16 s_index,
                    n_brd::board b,
                    s64 s_alpha,
                    s64 s_beta,
                    s16 s_depth);
    };
}
