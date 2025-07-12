#include "../include/search.hpp"
#include "../include/types.hpp"
#include "../include/board.hpp"
#include "../include/nodectr.hpp"
#include "../include/timer.hpp"
#include <algorithm>
#include <cstdint>
#include <functional>
#include <thread>

namespace n_sch {
    using namespace n_brd;
    using namespace n_ctr;
    // using namespace n_tmr;
    using namespace n_types;

    namespace {
        // typedef x functype;
        evl _abmax(board b, evl e_alpha, evl e_beta, u16 u_depth, reference_wrapper<bool> b_stop);
        evl _abmin(board b, evl e_alpha, evl e_beta, u16 u_depth, reference_wrapper<bool> b_stop);
        void _thread(
                reference_wrapper<evl> e_ret,
                bool b_isMax,
                board b,
                evl e_alpha,
                evl e_beta,
                u16 u_depth,
                reference_wrapper<bool> b_stopRequest
                ) {
            // cout << "thread id: " << this_thread::get_id() << "\n";
            // cout << "board: " << &b << "\n";

            evl e = 0;
            if (b_isMax) {
                e = _abmax(b, e_alpha, e_beta, u_depth, b_stopRequest);
            }
            e = _abmin(b, e_alpha, e_beta, u_depth, b_stopRequest);
            e_ret.get() = e;
        }

        evl _abmax(board b, evl e_alpha, evl e_beta, u16 u_depth, reference_wrapper<bool> b_stop) {
            if (u_depth == 0 || n_tmr::b_timeUp) {
                return b.eval();
            }
            
            const vector<cMove> v_moves = b.genLegalMoves();
            // for each move evaluate with a thread calling the inverse operation
            
            // create threads
            vector<jthread> v_threads;
            vector<evl> v_results(v_moves.size(), INT64_MIN);
            bool b_childStop = false;
            reference_wrapper<bool> r_childStop = ref(b_childStop);
            // for (s16 i = 0;
            //         i < v_moves.size() &&
            //         !b_stop &&
            //         !n_tmr::b_timeUp;
            //         i++) {
            for (s16 i = 0; i < 2; i++) {
                board b_tmp = b;
                const cMove m = v_moves.at(i);

                b_tmp.playMove(m);
                cout << "board " << i << ": " << &b << "\n";
                reference_wrapper<evl> r_ret = ref(v_results.at(i));

                // create the thread function
                auto f = bind(
                        _thread, 
                        r_ret,
                        false,
                        b_tmp,
                        e_alpha,
                        e_beta,
                        u_depth-1,
                        r_childStop
                        );

                // make thread
                v_threads.emplace_back(f);

                // reset 
                b_tmp.unPlayMove();
            }

            // use evaluation of threads to change alpha and beta
            // can stop early if a cutoff is found
            for (s16 i = 0;
                    i < v_threads.size() &&
                    !b_stop &&
                    !n_tmr::b_timeUp;
                    i++) {
                if (v_threads.at(i).joinable()) v_threads.at(i).join();
            }

            // of the available options need to find the max
            evl e_max = INT64_MIN;
            auto i_max = max_element(v_results.begin(), v_results.end());
            if (i_max != v_results.end()) e_max = *i_max;
            if (e_max >= e_beta) {
                return e_beta;
            }
            return e_max;
        }

        evl _abmin(board b, evl e_alpha, evl e_beta, u16 u_depth, reference_wrapper<bool> b_stop) {
            cout << "board child" << ": " << &b << "\n";
            if (u_depth == 0 || n_tmr::b_timeUp) {
                return b.eval();
            }
            
            const vector<cMove> v_moves = b.genLegalMoves();
            // for each move evaluate with a thread calling the inverse operation
            
            // create threads
            vector<jthread> v_threads;
            vector<evl> v_results(v_moves.size(), INT64_MAX);
            bool b_childStop = false;
            reference_wrapper<bool> r_childStop = ref(b_childStop);
            // for (s16 i = 0;
            //         i < v_moves.size() &&
            //         !b_stop &&
            //         !n_tmr::b_timeUp;
            //         i++) {
                for (s16 i = 0; i < 2; i++) {
                const cMove m = v_moves.at(i);

                board b_tmp = b;

                b_tmp.playMove(m);
                reference_wrapper<evl> r_ret = ref(v_results.at(i));

                // create the thread function
                auto f = bind(
                        _thread, 
                        r_ret,
                        true,
                        b,
                        e_alpha,
                        e_beta,
                        u_depth-1,
                        r_childStop
                        );

                // make thread
                v_threads.emplace_back(f);

                // reset 
                b_tmp.unPlayMove();
            }

            // use evaluation of threads to change alpha and beta
            // can stop early if a cutoff is found
            for (s16 i = 0;
                    i < v_threads.size() &&
                    !b_stop &&
                    !n_tmr::b_timeUp;
                    i++) {
                if (v_threads.at(i).joinable()) v_threads.at(i).join();
            }

            // of the available options need to find the max
            evl e_min = INT64_MAX;
            auto i_min = min_element(v_results.begin(), v_results.end());
            if (i_min != v_results.end()) e_min = *i_min;
            if (e_min >= e_alpha) {
                return e_alpha;
            }
            return e_min;
        }
    }

    evl search(board b) {
        n_tmr::start();
        bool b_tmp = false;
        return _abmax(b, INT64_MIN, INT16_MAX, MAXDEPTH, ref(b_tmp));
    }
}
