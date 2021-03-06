﻿#include "stdafx.h"
#include "CppUnitTest.h"

#include "../libhelper/aho-corasick.h"
#include "../libhelper/aqueue.h"
#include "../libhelper/dlist.h"
#include "../libhelper/overflow.h"
#include "../libhelper/kmp.h"
#include "../libhelper/rbtree.h"
#include "../libhelper/text.h"
#include "../libhelper/sparseset.h"
#include "../libhelper/dequeue.h"
#include "test-material.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace libhelperunittests {
    TEST_CLASS (overflow_tests) {
    public:
        TEST_METHOD (test_checked_32bit_addition_edgecases) {
            int32_t c = 0;
            int32_t a = 0, b = INT32_MAX;
            Assert::IsTrue (false == checked_add_32 (a, b, &c));
            a = 1; b = INT32_MAX;
            Assert::IsTrue (true == checked_add_32 (a, b, &c));
            a = INT32_MAX - 1; b = INT32_MAX;
            Assert::IsTrue (true == checked_add_32 (a, b, &c));
            ++a; b = INT32_MAX;
            Assert::IsTrue (true == checked_add_32 (a, b, &c));
            a = 0; b = INT32_MIN;
            Assert::IsTrue (false == checked_add_32 (a, b, &c));
            a = -1; b = INT32_MIN;
            Assert::IsTrue (true == checked_add_32 (a, b, &c));
            a = INT32_MIN + 1; b = INT32_MIN;
            Assert::IsTrue (true == checked_add_32 (a, b, &c));
            --a; b = INT32_MIN;
            Assert::IsTrue (true == checked_add_32 (a, b, &c));
            a = INT32_MAX; b = INT32_MIN;
            Assert::IsTrue (false == checked_add_32 (a, b, &c));
        }

        TEST_METHOD (test_checked_64bit_addition_edgecases) {
            int64_t c = 0;
            int64_t a = 0, b = INT64_MAX;
            Assert::IsTrue (false == checked_add_64 (a, b, &c));
            a = 1; b = INT64_MAX;
            Assert::IsTrue (true == checked_add_64 (a, b, &c));
            a = INT64_MAX - 1; b = INT64_MAX;
            Assert::IsTrue (true == checked_add_64 (a, b, &c));
            ++a; b = INT64_MAX;
            Assert::IsTrue (true == checked_add_64 (a, b, &c));
            a = 0; b = INT64_MIN;
            Assert::IsTrue (false == checked_add_64 (a, b, &c));
            a = -1; b = INT64_MIN;
            Assert::IsTrue (true == checked_add_64 (a, b, &c));
            a = INT64_MIN + 1; b = INT64_MIN;
            Assert::IsTrue (true == checked_add_64 (a, b, &c));
            --a; b = INT64_MIN;
            Assert::IsTrue (true == checked_add_64 (a, b, &c));
            a = INT64_MAX; b = INT64_MIN;
            Assert::IsTrue (false == checked_add_64 (a, b, &c));
        }
    };

    TEST_CLASS(dlist_tests) {
public:
        TEST_METHOD (test_dlist_single_add_and_remove) {
            DLIST_HEAD (the_head);

            struct item { int id; struct dlist_node entry; };

            auto one = ::new (struct item);
            /* One insertion makes the list non-empty: */
            Assert::IsTrue (dlist_is_empty (&the_head));
            dlist_append (&the_head, &one->entry);
            Assert::IsFalse (dlist_is_empty (&the_head));
            /* The list size is equal to 1: */
            struct dlist_node *pos = nullptr;
            size_t nelements = 0;
            dlist_for_each (&the_head, pos) {
                ++nelements;
            }
            Assert::IsTrue (nelements == 1);
            /* One deletion makes the list empty: */
            dlist_delete (&one->entry);
            delete one;
            Assert::IsTrue (dlist_is_empty (&the_head));

            /* Now prepend instead of append: */
            one = ::new (struct item);
            /* One insertion makes the list non-empty: */
            Assert::IsTrue (dlist_is_empty (&the_head));
            dlist_prepend (&the_head, &one->entry);
            Assert::IsFalse (dlist_is_empty (&the_head));
            /* The list size is equal to 1: */
            pos = nullptr;
            nelements = 0;
            dlist_for_each (&the_head, pos) {
                ++nelements;
            }
            Assert::IsTrue (nelements == 1);
            /* One deletion makes the list empty: */
            dlist_delete (&one->entry);
            delete one;
            Assert::IsTrue (dlist_is_empty (&the_head));
        }

        TEST_METHOD (test_dlist_ordered_insertions) {
            /* This test will add nodes to a list in order,
               and verify the properties of the list: */
            std::random_device rd;
            auto seed = rd ();

            std::minstd_rand generator (seed);
            std::uniform_int_distribution<int> id_dist (0, INT_MAX);

            struct item { int id; struct dlist_node entry;
                item::item (int _id) : id (_id) { entry.next = nullptr; entry.prev = nullptr; };
            };
            DLIST_HEAD (the_head);
            struct dlist_node *pos = nullptr; /* Temporary for looping */

            std::vector<int> ids(4000);
            std::generate (std::begin (ids), std::end (ids),
                           [&id_dist, &generator]() { return id_dist (generator); });

            size_t nelements_expected = 1;
            for (auto id : ids) {
                auto temp = ::new(struct item){ id };
                dlist_append (&the_head, &temp->entry);
                Assert::IsFalse (dlist_is_empty (&the_head));
                
                size_t nelements = 0;
                dlist_for_each (&the_head, pos) {
                    ++nelements;
                }
                Assert::IsTrue (nelements == nelements_expected);
                ++nelements_expected;
            }

            /* Iterating the list forward must show
             * all elements be in order of insertion: */
            size_t idx = 0;
            dlist_for_each (&the_head, pos) {
                auto element = dlist_entry (pos, struct item, entry)->id;
                Assert::IsTrue (element == ids[idx++]);
            }
        }
    };

    TEST_CLASS(kmp_tests) {
        const char *barn = "Loremipsumdolorsitamet,consecteturadipisicingelit,seddoeiusmodtemporincididuntutlaboreetdoloremagnaaliqua.Utenimadminimveniam,quisnostrudexercitationullamcolaborisnisiutaliquipexeacommodoconsequat.Duisauteiruredolorinreprehenderitinvoluptatevelitessecillumdoloreeufugiatnullapariatur.Excepteursintoccaecatcupidatatnonproident,suntinculpaquiofficiadeseruntmollitanimidestlaborum.";
public:
        TEST_METHOD (test_kmp_search_successful) {
            const char test_success[] = "tempor";

            size_t ff1[sizeof test_success] = { 0 };
            kmp_ff (test_success, ff1, ARRAY_SIZE(ff1));
            Assert::IsTrue (kmp (barn, ::strlen (barn), test_success, ::strlen (test_success), ff1) == ::strstr (barn, test_success) - barn);
        }

        TEST_METHOD (test_kmp_search_all_substrings) {
            size_t ff[sizeof barn] = {0};
            for (const char *substring = barn + strlen (barn) - 1; substring != barn; --substring) {
                kmp_ff (substring, ff, ARRAY_SIZE (ff));

                Assert::IsTrue (kmp (barn, ::strlen (barn), substring, ::strlen (substring), ff) == ::strstr (barn, substring) - barn);
            }
        }

        TEST_METHOD(test_kmp_search_unsuccessful) {
            const char test_fail[] = "participate in parachute";
            size_t ff2[sizeof test_fail] = { 0 };

            kmp_ff (test_fail, ff2, ARRAY_SIZE(ff2));
            Assert::IsTrue (kmp (barn, ::strlen (barn), test_fail, ::strlen (test_fail), ff2) == -1);
        }
    };

    TEST_CLASS (double_ended_queue_tests) {
    public:
        TEST_METHOD (test_dlist_api_equivalence) {
            struct item {
                int id; struct dlist_node entry;
                item::item (int _id) : id (_id) { entry.next = nullptr; entry.prev = nullptr; };
            };
            dequeue_alloc (q);
            dequeue_init (&q);
            auto temp = ::new(struct item){ 2 };
            dlist_append (&q, &temp->entry);
            /* Queue is not empty: */
            Assert::IsFalse (dlist_is_empty (&q));
            /* Queue's first element is the only element: */
            Assert::IsTrue (dequeue_front (&q) == &temp->entry);
        }

        TEST_METHOD (test_enqueue) {
            struct item {
                int id; struct dlist_node entry;
                item::item (int _id) : id (_id) { entry.next = nullptr; entry.prev = nullptr; };
            };
            dequeue_alloc (q);
            dequeue_init (&q);
            auto temp = ::new(struct item){ 3 };
            dequeue_enqueue (&q, &temp->entry);
            /* Queue is not empty: */
            Assert::IsFalse (dequeue_is_empty (&q));
            /* Queue's first element is the only element: */
            Assert::IsTrue (dequeue_front (&q) == &temp->entry);
        }

        TEST_METHOD (test_fifo_order) {
            struct item {
                int id; struct dlist_node entry;
                item::item (int _id) : id (_id) { entry.next = nullptr; entry.prev = nullptr; };
            };
            dequeue_alloc (q);
            dequeue_init (&q);
            auto one = ::new(struct item){ 1 };
            auto two = ::new(struct item){ 2 };
            dequeue_enqueue (&q, &one->entry);
            /* Queue is not empty: */
            Assert::IsFalse (dequeue_is_empty (&q));
            dequeue_enqueue (&q, &two->entry);
            /* Queue is not empty: */
            Assert::IsFalse (dequeue_is_empty (&q));

            /* One was added before two, therefore it is the first out: */
            Assert::IsTrue (dlist_is_last (&one->entry, &q));
            Assert::IsTrue (dequeue_front (&q) == &one->entry);
            Assert::IsTrue (dequeue_front (dequeue_front (&q)) == &two->entry);

            /* Deleting one makes two the first, last, and only element: */
            dlist_delete (dequeue_front (&q));
            delete one;
            Assert::IsTrue (dlist_is_last (&two->entry, &q));
            Assert::IsTrue (dequeue_front (&q) == &two->entry);
        }

        TEST_METHOD (test_dequeue_ordered_insertions) {
            /* This test will add nodes to a list in order,
               and verify the properties of the list: */
            std::random_device rd;
            auto seed = rd ();

            std::minstd_rand generator (seed);
            std::uniform_int_distribution<int> id_dist (0, INT_MAX);

            struct item {
                int id; struct dlist_node entry;
                item::item (int _id) : id (_id) { entry.next = nullptr; entry.prev = nullptr; };
            };
            dequeue_alloc (the_head);
            dequeue_init (&the_head);
            struct dlist_node *pos = nullptr; /* Temporary for looping */

            std::vector<int> ids (6000);
            std::generate (std::begin (ids), std::end (ids),
                           [&id_dist, &generator]() { return id_dist (generator); });

            size_t nelements_expected = 1;
            for (auto id : ids) {
                auto temp = ::new(struct item){ id };
                dequeue_enqueue (&the_head, &temp->entry);
                Assert::IsFalse (dequeue_is_empty (&the_head));

                size_t nelements = 0;
                dlist_for_each (&the_head, pos) {
                    ++nelements;
                }
                Assert::IsTrue (nelements == nelements_expected);
                ++nelements_expected;
            }

            /* All elements must be peeked in FIFO order: */
            size_t idx = 0;
            while (!dequeue_is_empty (&the_head)) {
                auto cursor = dequeue_front (&the_head);
                auto element = dlist_entry (cursor, struct item, entry)->id;
                Assert::IsTrue (element == ids[idx++]);
                dlist_delete (cursor);
            }
            /* And the queue must be empty: */
            Assert::IsTrue (dequeue_is_empty (&the_head));
        }
    };

    TEST_CLASS (ac_trie_tests) {
    public:
        TEST_METHOD (test_trie_structure) {
            std::ifstream ifs ("test-tags.txt");
            std::vector<std::string> tags;
            std::string line;
            while (std::getline (ifs, line)) {
                tags.emplace_back (line);
            }
            struct ac_trie trie;
            ac_trie_init (&trie);
            for (auto tag : tags) {
                Assert::IsTrue (ac_trie_insert (&trie, tag.c_str()));
            }

            std::stringstream output;
            struct visit { bool visited; size_t depth; visit (bool v, size_t d) : visited (v), depth (d) {} };

            std::queue<struct ac_state *> q;
            std::map<struct ac_state *, struct visit *> visited;

            visited[trie.root] = new visit (true, 0);
            q.emplace (trie.root);
            output << std::endl;

            /* DFS, for debugging */
            std::function<void(struct ac_state *)> dfs = [&](struct ac_state *s) {
                visited[s] = new visit (true, s->depth);

                for (size_t i = 0; i < 256; ++i) {
                    if (ac_next (s, i) == nullptr || visited.find(ac_next(s, i)) != visited.end()) { continue; }

                    output << " [" << s->depth << "]";

                    output << " = " << (char)i << " |";
                    dfs (ac_next (s, i));
                    output << std::endl;
                    for (size_t d = 0; d < s->depth; ++d) {
                        output << "             ";
                    }
                }
            };

            Assert::IsTrue (ac_build_failure_function (&trie));
            dfs (trie.root);
            /*Logger::WriteMessage (output.str ().c_str ());*/
        }

        TEST_METHOD (test_trie_matches) {
            struct ac_trie trie;
            ac_trie_init (&trie);
            Assert::IsTrue (ac_trie_insert (&trie, "she"));
            Assert::IsTrue (ac_trie_insert (&trie, "tried"));
            Assert::IsTrue (ac_trie_insert (&trie, "hers"));
            Assert::IsTrue (ac_trie_insert (&trie, "he"));
            Assert::IsTrue (ac_trie_insert (&trie, "looking"));
            Assert::IsTrue (ac_trie_insert (&trie, "what"));

            Assert::IsTrue (ac_build_failure_function (&trie));

            struct ac_result res = ac_search (&trie, "head of hers, OK?",
                                              sizeof "head of hers, OK?", 0);
            Assert::IsTrue (ac_search_matched (res));
            Assert::IsTrue (res.start == 0 && res.end == 2);

            res = ac_search_longest (&trie, "head of hers, OK?",
                                     sizeof "head of hers, OK?", 0);
            Assert::IsTrue (res.start == 8 && res.end == 12);
        }

        TEST_METHOD (test_trie_no_match) {
            struct ac_trie trie;
            ac_trie_init (&trie);
            Assert::IsTrue (ac_trie_insert (&trie, "parachute"));
            Assert::IsTrue (ac_trie_insert (&trie, "participation"));
            Assert::IsTrue (ac_trie_insert (&trie, "anagram"));
            Assert::IsTrue (ac_trie_insert (&trie, "herewith"));

            Assert::IsTrue (ac_build_failure_function (&trie));

            char *haystack = "Using for reference, Aho and Corasick, Efficient string matching: an aid to bibliographic search (Communications of the ACM, 18(6):333–340, 1975) (PDF).Algorithm 2 is mostly creating a trie or 'keyword tree' in the paper.Important Note: They make the assumption that g(s,a)=failg(s,a)=fail if either aa is undefined, or g(s,a)g(s,a) hasn't been defined yet. You can think of this function as an edge, it either points to the next state or returns failfail. Assume evaluating and assigning to functions, such as gg or outputoutput, takes constant time.Example:Let's consider the intuition for a single keyword xx. Use the last graph on page 335 with his, hers and she. Let's add 'hello' and verify that it takes linear time in the length. We can generalize from there.For this example, let y=helloy=hello.s=0s=0, the initial state. First, we find out that g(0,h)=1g(0,h)=1 so we set s=1s=1. Next we check g(1,e)g(1,e), which takes us to s=2s=2.Now g(2,l)=failg(2,l)=fail, there is no edge for ll, we have to create a state, s=10s=10 and set g(2,l)=10g(2,l)=10. State 1010 corresponds to the string helhel.Again g(10,l)=failg(10,l)=fail, we create a new state s=11(hell)s=11(hell). The same happens for oo : g(11,o)=failg(11,o)=fail, make a new state 12(hello)12(hello).We've finished hellohello, as a last step we set output(12)=hellooutput(12)=hello.Rough Analysis:Calling g(s,a)g(s,a), assigning values, and creating new states are constant time. Since we perform a constant number of each operation per letter, the complexity is O(|y|)O(|y|).Algorithm 2 does this for each key word. Notice that the same bound holds for each keyword added, regardless of what g(s,a)g(s,a) returns for each letter. Then the total time complexity is O(Sik|yi|)O(Sik|yi|), proportional to the sum of the length of the keywords.The algorithm also sets a self-loop for any unused symbols, or g(0,a)=0g(0,a)=0 for all aa that weren't the first letter of a keyword. We only need to consider letters in the set of keywords, so this step is still bounded by the sum length.";
            struct ac_result res = ac_search_longest (&trie, haystack, sizeof haystack, 0);
            /*Logger::WriteMessage (haystack + res.start);*/
            Assert::IsFalse (ac_search_matched (res));
        }

        TEST_METHOD (test_trie_not_enough_hay_for_match) {
            struct ac_trie trie;
            ac_trie_init (&trie);
            Assert::IsTrue (ac_trie_insert (&trie, "she"));
            Assert::IsTrue (ac_trie_insert (&trie, "tried"));
            Assert::IsTrue (ac_trie_insert (&trie, "hers"));
            Assert::IsTrue (ac_trie_insert (&trie, "he"));
            Assert::IsTrue (ac_trie_insert (&trie, "looking"));
            Assert::IsTrue (ac_trie_insert (&trie, "what"));

            Assert::IsTrue (ac_build_failure_function (&trie));

            struct ac_result res = ac_search (&trie, "h", sizeof "h", 0);
            Assert::IsFalse (ac_search_matched (res));
            res = ac_search (&trie, "he", sizeof "he", 0);
            Assert::IsTrue (ac_search_matched (res));
        }

        TEST_METHOD (test_ac_microbench) {
            std::ifstream ifs ("test-tags.txt");
            std::vector<std::string> tags;
            std::string line;
            while (std::getline (ifs, line)) {
                tags.emplace_back (line);
            }
            std::ifstream mfs ("messages.txt");
            std::vector<std::string> messages;
            while (std::getline (mfs, line)) {
                messages.emplace_back (line);
            }

            auto preprocess_start = std::chrono::high_resolution_clock::now ();
            struct ac_trie trie;
            ac_trie_init (&trie);
            for (const std::string & tag : tags) {
                Assert::IsTrue (ac_trie_insert (&trie, tag.c_str ()));
            }
            ac_build_failure_function (&trie);
            auto preprocess_end = std::chrono::high_resolution_clock::now ();
            auto preprocess_time = std::chrono::duration<double, std::milli> (preprocess_end - preprocess_start).count ();
            /* Micro: */
            size_t nmatches = 0;
            auto time_start = std::chrono::high_resolution_clock::now ();
            for (const auto message : messages) {
                struct ac_result res = ac_search (&trie, message.c_str (), 4, 0);
                if (ac_search_matched (res)) ++nmatches;
            }
            auto time_end = std::chrono::high_resolution_clock::now ();
            auto elapsed_time = std::chrono::duration<double, std::milli> (time_end - time_start).count ();
            std::stringstream out;
            out << "Preprocessing time: " << preprocess_time
                << " ms" << std::endl
                << "Matching time     : " << elapsed_time
                << " ms for " << messages.size () / elapsed_time / 1000 << " Mmps" << std::endl;

            out << "Matches           : " << nmatches << " out of " << messages.size () << std::endl;
            Logger::WriteMessage (out.str ().c_str ());
        }

        /*
         * The spec of is_red says it must return true when
         * it is passed a red node, and false in every other case:
         */
        TEST_METHOD (test_rb_is_red) {
            long dummy_data = 6;
            struct rb_node red_root;
            red_root.parent = NULL;
            red_root.rb_link[0] = NULL;
            red_root.rb_link[1] = NULL;
            red_root.data = &dummy_data;
            red_root.red = true;
            Assert::IsTrue (is_red (&red_root));

            struct rb_node black_root;
            black_root.parent = NULL;
            black_root.rb_link[0] = NULL;
            black_root.rb_link[1] = NULL;
            black_root.data = &dummy_data;
            black_root.red = false;
            Assert::IsFalse (is_red (&black_root));

            struct rb_node red_level1_leaf;
            red_level1_leaf.parent = &black_root;
            red_level1_leaf.rb_link[0] = NULL;
            red_level1_leaf.rb_link[1] = NULL;
            red_level1_leaf.data = &dummy_data;
            red_level1_leaf.red = true;
            Assert::IsTrue (is_red (&red_level1_leaf));

            struct rb_node black_level1_leaf;
            black_level1_leaf.parent = &red_root;
            black_level1_leaf.rb_link[0] = NULL;
            black_level1_leaf.rb_link[1] = NULL;
            black_level1_leaf.data = &dummy_data;
            black_level1_leaf.red = false;
            Assert::IsFalse (is_red (&black_level1_leaf));

            struct rb_node red_level1_inner;
            red_level1_inner.parent = &black_root;
            red_level1_inner.rb_link[0] = &black_level1_leaf;
            red_level1_inner.rb_link[1] = NULL;
            red_level1_inner.data = &dummy_data;
            red_level1_inner.red = true;
            Assert::IsTrue (is_red (&red_level1_inner));

            struct rb_node black_level1_inner;
            black_level1_inner.parent = &red_root;
            black_level1_inner.rb_link[1] = &red_level1_leaf;
            black_level1_inner.data = &dummy_data;
            black_level1_inner.red = false;
            Assert::IsFalse (is_red (&black_level1_inner));
        }

        /**
         * Generate RB-tree ops at random,
         * and perform them with a random selection of item(s),
         * RB-tree invariants must hold at every step:
         *
         * If in trouble, uncomment the logging code.
         */
        TEST_METHOD (test_rb_tree_invariants) {
            std::wstringstream assert_output;

            std::random_device rd;
            auto seed = rd ();

            assert_output << "Seed: " << seed << std::endl;

            std::minstd_rand generator(seed);
            std::bernoulli_distribution ops_dist(0.75); /* .75 of all ops to be rb_insert */
            std::uniform_int_distribution<int> data_dist(0, INT_MAX);

            /* Kept low because running on other people's CPUs,
               and VSTS has limits. Feel free to stress yourself: */
            const size_t testiterations = 100;

            for (size_t idx = 0; idx < testiterations; ++idx) {
                const size_t nops = 1000;

                int nodes_in_order[nops] = {0};
                struct rb_tree high;
                high.rb_node = NULL;
                high.cmp = intcmp;
                /*
                std::stringstream expected_output;
                expected_output << "Iteration: " << idx << std::endl;
                */
                for (size_t iteration = 0; iteration < nops; ++iteration) {
                    nodes_in_order[iteration] = data_dist (generator);
                    auto op = ops_dist (generator) ? rb_insert : rb_remove;
                    /*
                    if (op == rb_insert) {
                        expected_output << "I" << nodes_in_order[iteration] << ",";
                    } else {
                        expected_output << "D" << nodes_in_order[iteration] << ",";
                    }
                    */
                    op (&high, &nodes_in_order[iteration]);
                }
                /*
                Logger::WriteMessage (expected_output.str ().c_str ());

                std::stringstream actual_levelorder_output;
                levelorder (high.rb_node, actual_levelorder_output);
                Logger::WriteMessage (actual_levelorder_output.str ().c_str ());
                */
                auto treesize = rb_size (high.rb_node);
                /* the maximum number of black nodes in any root-null path is log2(n+1) */
                auto max_black_height = static_cast<unsigned long>(::log2 (treesize + 1));
                /* the minimum black-height is at least half the height of the tree: */
                auto min_black_height = static_cast<unsigned long>(::log2 (treesize) / 2);
                auto actual_black_height = rb_invariant (high.rb_node, high.cmp);

                assert_output << "Size: " << treesize
                       << " Expected: [" << min_black_height << ", " << max_black_height << "]"
                       << " Actual: " << actual_black_height << std::endl;

                Assert::IsTrue (treesize == 0
                             || min_black_height < actual_black_height && actual_black_height <= max_black_height,
                                assert_output.str().c_str());
            }
            // Logger::WriteMessage (assert_output.str ().c_str ());
        }
    };
}
