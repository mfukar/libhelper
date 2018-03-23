#include "stdafx.h"
#include "CppUnitTest.h"

#include "../libhelper/aho-corasick.h"
#include "../libhelper/overflow.h"
#include "../libhelper/kmp.h"
#include "../libhelper/queue.h"
#include "../libhelper/rbtree.h"

#include "test-material.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace libhelperunittests {
    TEST_CLASS (UnitTest1) {
        const char *barn = "Loremipsumdolorsitamet,consecteturadipisicingelit,seddoeiusmodtemporincididuntutlaboreetdoloremagnaaliqua.Utenimadminimveniam,quisnostrudexercitationullamcolaborisnisiutaliquipexeacommodoconsequat.Duisauteiruredolorinreprehenderitinvoluptatevelitessecillumdoloreeufugiatnullapariatur.Excepteursintoccaecatcupidatatnonproident,suntinculpaquiofficiadeseruntmollitanimidestlaborum.";
    public:
        TEST_METHOD(test_32bit_signed_addition) {
            int32_t a = INT32_MIN, b = INT32_MIN;
            int32_t c = 0;
            Assert::IsTrue (checked_add_32 (a, b, &c));
        }

        TEST_METHOD (test_64bit_signed_addition) {
            int64_t a = 1, b = INT64_MAX;
            int64_t c = 0;
            Assert::IsTrue (checked_add_64 (b, a, &c));
            a = 0; b = INT64_MAX;
            Assert::IsFalse (checked_add_64 (a, b, &c));
        }

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

        TEST_METHOD (test_dequeue) {
            struct ac_state s0, s1, s2;
            s0.id = 0; s1.id = 1; s2.id = 2;

            dequeue (q1);
            ff_queue_element n0, n1, n2;
            n0.s = &s0; n1.s = &s1; n2.s = &s2;
            
            dequeue_enqueue (&q1, &n0.list);
            dequeue_enqueue (&q1, &n1.list);
            dequeue_enqueue (&q1, &n2.list);

            for (unsigned iteration = 0; !dequeue_is_empty (&q1); ++iteration) {
                struct ff_queue_element * t = dequeue_data (dequeue_pop_front (&q1), struct ff_queue_element, list);
                Assert::IsTrue (t->s->id == iteration);
            }
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

            char *haystack = "Using for reference, Aho and Corasick, Efficient string matching: an aid to bibliographic search (Communications of the ACM, 18(6):333–340, 1975) (PDF).Algorithm 2 is mostly creating a trie or 'keyword tree' in the paper.Important Note: They make the assumption that g(s,a)=failg(s,a)=fail if either aa is undefined, or g(s,a)g(s,a) hasn't been defined yet. You can think of this function as an edge, it either points to the next state or returns failfail. Assume evaluating and assigning to functions, such as gg or outputoutput, takes constant time.Example:Let's consider the intuition for a single keyword xx. Use the last graph on page 335 with his, hers and she. Let's add 'hello' and verify that it takes linear time in the length. We can generalize from there.For this example, let y=helloy=hello.s=0s=0, the initial state. First, we find out that g(0,h)=1g(0,h)=1 so we set s=1s=1. Next we check g(1,e)g(1,e), which takes us to s=2s=2.Now g(2,l)=failg(2,l)=fail, there is no edge for ll, we have to create a state, s=10s=10 and set g(2,l)=10g(2,l)=10. State 1010 corresponds to the string helhel.Again g(10,l)=failg(10,l)=fail, we create a new state s=11(hell)s=11(hell). The same happens for oo : g(11,o)=failg(11,o)=fail, make a new state 12(hello)12(hello).We've finished hellohello, as a last step we set output(12)=hellooutput(12)=hello.Rough Analysis:Calling g(s,a)g(s,a), assigning values, and creating new states are constant time. Since we perform a constant number of each operation per letter, the complexity is O(|y|)O(|y|).Algorithm 2 does this for each key word. Notice that the same bound holds for each keyword added, regardless of what g(s,a)g(s,a) returns for each letter. Then the total time complexity is O(∑ik|yi|)O(∑ik|yi|), proportional to the sum of the length of the keywords.The algorithm also sets a self-loop for any unused symbols, or g(0,a)=0g(0,a)=0 for all aa that weren't the first letter of a keyword. We only need to consider letters in the set of keywords, so this step is still bounded by the sum length.";
            struct ac_result res = ac_search_longest (&trie, haystack, sizeof haystack, 0);
            Logger::WriteMessage (haystack + res.start);
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
        }

        /*
         * The spec of is_red says it must return true when
         * it is passed a red node, and false in every other case:
         */
        TEST_METHOD (test_rb_is_red) {
            long dummy_data = 6;
            struct rb_node red_root = {
                .parent = NULL,
                .rb_link = {NULL, NULL},
                .data = &dummy_data,
                .red = true
            };
            Assert::IsTrue (is_red (&red_root));

            struct rb_node black_root = {
                .parent = NULL,
                .rb_link = {NULL, NULL},
                .data = &dummy_data,
                .red = false
            };
            Assert::IsFalse (is_red (&black_root));

            struct rb_node red_level1_leaf = {
                .parent = &black_root,
                .rb_link = {NULL, NULL},
                .data = &dummy_data,
                .red = true
            };
            Assert::IsTrue (is_red (&red_level1_leaf));

            struct rb_node black_level1_leaf = {
                .parent = &red_root,
                .rb_link = {NULL, NULL},
                .data = &dummy_data,
                .red = false
            };
            Assert::IsFalse (is_red (&black_level1_leaf));

            struct rb_node red_level1_inner = {
                .parent = &black_root,
                .rb_link = {&black_level1_leaf, NULL},
                .data = &dummy_data,
                .red = true
            };
            Assert::IsTrue (is_red (&red_level1_inner));

            struct rb_node black_level1_inner = {
                .parent = &red_root,
                .rb_link = {NULL, &red_level1_leaf},
                .data = &dummy_data,
                .red = false
            };
            Assert::IsFalse (is_red (&black_level1_inner));
        }

        /* Before we test the rest of the RB-tree implementation,
         * we must test the function asserting it tests a RB-tree's
         * properties: */
        TEST_METHOD (test_rb_invariants_are_evaluated_correctly) {
        }

        /**
         * Generate 10000 RB-tree ops at random,
         * and perform them with a random selection of item(s),
         * RB-tree invariants must hold at every step:
         */
        TEST_METHOD (test_rb_tree_invariants) {
            std::list<int> nodes_in_order;
            std::random_device rd;
            std::mt19937 generator(rd());
            std::uniform_int_distribution<int> ops_dist(0, 1);
            std::uniform_int_distribution<int> data_dist(0, INT_MAX);
            struct rb_tree high = {0};
            high.cmp = intcmp;

            for (size_t iteration = 0; iteration < 10000; ++iteration) {
                op = ops_dist[generator];
                dat = data_dist[generator];
                /* TBD */
            }

            int arr[] = { 11, 14, 2, 7, 1, 15, 5, 8 };
            for (size_t i = 0; i < sizeof arr / sizeof *arr; ++i) {
                rb_insert (&high, &arr[i]);
            }
            std::stringstream output;
            inorder (high.rb_node, 0, output);
            Logger::WriteMessage (output.str().c_str());
            Assert::IsTrue (rb_invariant (high.rb_node, high.cmp) == 1);

            rb_remove (&high, &arr[6]);
            inorder (high.rb_node, 0, output);
            Logger::WriteMessage (output.str ().c_str ());
            Assert::IsTrue (rb_invariant (high.rb_node, high.cmp) == 1);
        }
    };
}
